#include "main.h"
#include "Map.h"
#include "GUI.h"
#include "Database.h"

//Main
SDL_Window *Main::screen = NULL;
SDL_Renderer *Main::renderer = NULL;
SDL_GLContext Main::GL_context = NULL;
int Main::Disp_w = 0;
int Main::Disp_h = 0;
int Main::GUI = 0;
Inputs Main::input;

void Main::Exit()
{
	Database::Destroy();

	SDL_DestroyWindow(screen);
	TTF_Quit();
	SDL_Quit();
}

void Main::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		cout << "SDL initialisation error : " << SDL_GetError() << endl;
		SDL_Quit();

		Exit();
	}

	if (TTF_Init() == -1)
	{
		cout << "TTF initialisation error : " << TTF_GetError() << endl;
		SDL_Quit();

		Exit();
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		cout << "SDL_mixer initialisation error : " << Mix_GetError() << endl;
		SDL_Quit();

		Exit();
	}
	cout << "Allocated Channels : " << Mix_AllocateChannels(32) << endl;
	//cout << "SDL_mixer initialisation error : " << Mix_GetError() << endl;
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void Main::MainLoop()
{
	double ActTime = 0, LastTime = 0, frame_rate = 90;
	bool Keep = true;

	double zoom = 1;
	Clock framerate_timer; framerate_timer.start();

	Player *player = Player_info::GetPlayer_ptr();
	player->SetPos(8, 10);
	player->SetName("Red");


	Map map;
	map.Load(1);
	map.BindPlayer(player);

	Camera cam;
	cam.SetPos(0, 0, 0);
	cam.Follow(player->GetCenter());

	double lx = 0;

	double px, py;

	int DisplayMode = 0;

	int wx = 0, wy = 0;
	float opacity = 1;

	//Main::GUI = 3;

	DialogueGUI::LoadScript("TEST");

	while (Keep && !input.CloseGame)
	{
		if (framerate_timer.duration() > 1000000.0 / frame_rate)
		{
			framerate_timer.start();
			
			// Inputs
			input.UpdateControllerInputs(true);
			input.UpdateKeyboardInputs(true);

			if (GUI == 0)
			{
				if (input.pushedInput(input.T)) GUI = 1;
				if (input.pushedInput(input.E)) GUI = 2;
				if (input.GetInput(input.S)) map.MovePlayer(input.GetInput(input.L_SHIFT), 0);
				if (input.GetInput(input.Z)) map.MovePlayer(input.GetInput(input.L_SHIFT), 1);
				if (input.GetInput(input.Q)) map.MovePlayer(input.GetInput(input.L_SHIFT), 2);
				if (input.GetInput(input.D)) map.MovePlayer(input.GetInput(input.L_SHIFT), 3);
			}
			else if (GUI == 1) PkmnTeamGUI::Update();
			else if (GUI == 2) BagGUI::Update();
			else if (GUI == 3) FightGUI::Battle(Player_info::GetPlayer_ptr(), 5, 100, 0);

			map.Update();
			player->Animate();

			// Display
			glClear(GL_COLOR_BUFFER_BIT);

			cam.SetLimit(0, map.GetW() * 32, 0, map.GetH() * 32);
			cam.Update();
			cam.Perspective();

			map.Display();

			cam.GUI();
			MainInfoGUI::Display();

			if (GUI == 1) PkmnTeamGUI::Display();
			if (GUI == 2) BagGUI::Display();

			if (!input.CloseGame) SDL_GL_SwapWindow(screen);
			LastTime = ActTime;
		}
		else Clock::sleep(1000.0 / frame_rate - framerate_timer.duration() * 0.001);
	}


}

void Main::Editor()
{
	double ActTime = 0, LastTime = 0, frame_rate = 90;
	bool Keep = true;

	double zoom = 1;
	Clock framerate_timer; framerate_timer.start();

	Map map;
	map.Load(1);

	Camera cam;
	cam.SetPos(0, 0, 0);

	double px, py;
	double time = 17;

	while (Keep && !input.CloseGame)
	{
		if (framerate_timer.duration() >= 1000000.0 / frame_rate)
		{
			//cout << framerate_timer.duration() * 0.001 << endl;
			framerate_timer.start();
			// Inputs
			input.UpdateControllerInputs(true);
			input.UpdateKeyboardInputs(true);
			if (input.GetInput(input.S)) cam.SetY(cam.Y() + 4);
			if (input.GetInput(input.Z)) cam.SetY(cam.Y() - 4);
			if (input.GetInput(input.Q)) cam.SetX(cam.X() - 4);
			if (input.GetInput(input.D)) cam.SetX(cam.X() + 4);

			map.Update();


			// Display
			glClear(GL_COLOR_BUFFER_BIT);

			cam.Update();
			cam.Perspective();

			map.Display();

			// GUI
			cam.GUI();

			if (!input.CloseGame) SDL_GL_SwapWindow(screen);
			
		}
		else Clock::sleep(1000.0 / frame_rate - framerate_timer.duration() * 0.001);
	}

}

void Main::Create_window(const char * name, int x, int y, int w, int h)
{
	Disp_w = w; Disp_h = h;
	Display_info::width = w; Display_info::height = h;

	Disp_w = w; Disp_h = h;
	Display_info::width = w; Display_info::height = h;

	screen = SDL_CreateWindow(name, x, y, w, h, SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_OPENGL);

	GL_context = SDL_GL_CreateContext(screen);

	if (GL_context == 0)
	{
		cout << "OpenGL context creation error : " << SDL_GetError() << endl;
		SDL_DestroyWindow(screen);
		SDL_Quit();

		Exit();
	}
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	Display_info::renderer = renderer;

	SDL_Surface *icon = IMG_Load("Icon.png");

	if (icon)
	{
		SDL_SetWindowIcon(screen, icon);
		SDL_FreeSurface(icon);
	}
}



int main(int argc, char *argv[])
{
	Main::Init();

	Main::Create_window("Pokemon Grey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720);
	//Main::Create_window("Pokemon Grey - Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1400, 900);
	Database::Load();
	Player_info::Load();
	MainInfoGUI::Init();
	PkmnTeamGUI::Init();
	BagGUI::Init();
	FightGUI::Init();

	Main::MainLoop();
	Main::Editor();

	Main::Exit();

	return 0;
}

