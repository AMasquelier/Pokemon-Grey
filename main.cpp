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
	double ActTime = 0, LastTime = 0, frame_rate = 60;
	bool Keep = true;

	double zoom = 1;
	Chrono c;

	Player *player = Player_info::GetPlayer_ptr();
	player->Load(8, 11);


	Map map;
	map.Load(3);
	map.BindPlayer(player);

	Camera cam;
	cam.SetPos(0, 0, 0);
	cam.Follow(player->GetCenter());

	double lx = 0;

	double px, py;

	int DisplayMode = 0;

	int wx = 0, wy = 0;
	float opacity = 1;

	while (Keep && !input.CloseGame)
	{
		ActTime = GE_getTime();

		if (ActTime - LastTime > 1000.0 / frame_rate)
		{
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

			map.Update();
			player->Animate();

			// Display
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			cam.Update();
			cam.Perspective();

			map.Display();

			cam.GUI();
			MainInfoGUI::Display();

			if (GUI == 1) PkmnTeamGUI::Display();
			if (GUI == 2) BagGUI::Display();

			SDL_GL_SwapWindow(screen);
			LastTime = ActTime;
		}
		else GE_rest(1000.0 / frame_rate - (ActTime - LastTime));
	}


}

void Main::Create_window(const char * name, int x, int y, int w, int h)
{
	Disp_w = w; Disp_h = h;
	Display_info::width = w; Display_info::height = h;

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

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
	glEnable(GL_DEPTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_MULTISAMPLE);
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
	Database::Load();
	Player_info::Load();
	MainInfoGUI::Init();
	PkmnTeamGUI::Init();
	BagGUI::Init();

	Main::MainLoop();

	Main::Exit();

	return 0;
}

