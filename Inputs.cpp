#include "Inputs.h"
#include "main.h"

//Gamepad

Gamepad::Gamepad()
{

}

bool Gamepad::isConnected()
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	return (XInputGetState(0, &state) == ERROR_SUCCESS);
}

void Gamepad::Update()
{
	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		if (SDL_GetTicks() - VChrono > Vduration)
			StopVibration();

		for (int i = 0; i < 14; i++)
			Buttons[i][Last] = Buttons[i][Actual];

		if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			state.Gamepad.sThumbLX = 0;
			state.Gamepad.sThumbLY = 0;
		}

		Triggers[LEFT] = (float)state.Gamepad.bLeftTrigger / 255;
		Triggers[RIGHT] = (float)state.Gamepad.bRightTrigger / 255;
		if (Triggers[LEFT] > Triggers[RIGHT])
			Triggers[MAX] = Triggers[LEFT];
		else
			Triggers[MAX] = Triggers[RIGHT];

		Buttons[DPAD_UP][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
		Buttons[DPAD_DOWN][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
		Buttons[DPAD_LEFT][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
		Buttons[DPAD_RIGHT][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);

		Buttons[START][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_START);
		Buttons[BACK][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);

		Buttons[LEFT_THUMB][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
		Buttons[RIGHT_THUMB][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);

		Buttons[RB][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
		Buttons[LB][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);

		Buttons[A][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
		Buttons[B][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
		Buttons[X][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
		Buttons[Y][Actual] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);

		Sticks[LEFT][X_AXIS] = (float)round(state.Gamepad.sThumbLX / 3.2768) / 10000;
		Sticks[LEFT][Y_AXIS] = (float)round(state.Gamepad.sThumbLY / 3.2768) / 10000;

		Sticks[RIGHT][X_AXIS] = (float)round(state.Gamepad.sThumbRX / 3.2768) / 10000;
		Sticks[RIGHT][Y_AXIS] = (float)round(state.Gamepad.sThumbRY / 3.2768) / 10000;


	}
	else
	{
		for (int i = 0; i < 14; i++)
			Buttons[i][Actual] = false;

		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				Sticks[i][j] = 0;

		Triggers[0] = Triggers[1] = 0;
	}
}

void Gamepad::Vibrate(float LMotor, float RMotor, double duration)
{
	VChrono = SDL_GetTicks();
	Vduration = duration;
	XINPUT_VIBRATION Vibration;

	memset(&Vibration, 0, sizeof(XINPUT_VIBRATION));

	int leftVib = (int)(LMotor*65535.0f);
	int rightVib = (int)(RMotor*65535.0f);

	Vibration.wLeftMotorSpeed = leftVib;
	Vibration.wRightMotorSpeed = rightVib;

	XInputSetState(0, &Vibration);
}

void Gamepad::StopVibration()
{
	XINPUT_VIBRATION Vibration;

	memset(&Vibration, 0, sizeof(XINPUT_VIBRATION));

	int leftVib = 0;
	int rightVib = 0;

	Vibration.wLeftMotorSpeed = leftVib;
	Vibration.wRightMotorSpeed = rightVib;

	XInputSetState(0, &Vibration);
}

//Inputs
int  Inputs::unichar = 0;
bool Inputs::_typing = false;
int  Inputs::_Event = 0;
bool Inputs::CloseGame = false;


Inputs::Inputs()
{
	for (int i = 0; i < 32; i++)
	{
		_Keys[i][Last] = false;
		_Keys[i][Actual] = false;
	}
}

void Inputs::UpdateControllerInputs(bool mode)
{
	gamepad.Update();
}

void Inputs::UpdateKeyboardInputs(bool mode)
{
	for (int i = 0; i < 64; i++)
		_Keys[i][Last] = _Keys[i][Actual];

	while (SDL_PollEvent(&_event))
	{
		switch (_event.type)
		{
		case SDL_QUIT:
			CloseGame = true;
			break;
		case SDL_KEYDOWN:
			_Keys[Up][Actual] |= (_event.key.keysym.sym == SDLK_UP);
			_Keys[Down][Actual] |= (_event.key.keysym.sym == SDLK_DOWN);
			_Keys[Right][Actual] |= (_event.key.keysym.sym == SDLK_RIGHT);
			_Keys[Left][Actual] |= (_event.key.keysym.sym == SDLK_LEFT);
			_Keys[A][Actual] |= (_event.key.keysym.sym == SDLK_a);
			_Keys[Q][Actual] |= (_event.key.keysym.sym == SDLK_q);
			_Keys[Z][Actual] |= (_event.key.keysym.sym == SDLK_z);
			_Keys[E][Actual] |= (_event.key.keysym.sym == SDLK_e);
			_Keys[D][Actual] |= (_event.key.keysym.sym == SDLK_d);
			_Keys[R][Actual] |= (_event.key.keysym.sym == SDLK_r);
			_Keys[F][Actual] |= (_event.key.keysym.sym == SDLK_f);
			_Keys[P][Actual] |= (_event.key.keysym.sym == SDLK_p);
			_Keys[S][Actual] |= (_event.key.keysym.sym == SDLK_s);
			_Keys[W][Actual] |= (_event.key.keysym.sym == SDLK_w);
			_Keys[L][Actual] |= (_event.key.keysym.sym == SDLK_l);
			_Keys[T][Actual] |= (_event.key.keysym.sym == SDLK_t);
			_Keys[ONE][Actual] |= (_event.key.keysym.sym == SDLK_1);
			_Keys[TWO][Actual] |= (_event.key.keysym.sym == SDLK_2);
			_Keys[THREE][Actual] |= (_event.key.keysym.sym == SDLK_3);
			_Keys[FOUR][Actual] |= (_event.key.keysym.sym == SDLK_4);
			_Keys[FIVE][Actual] |= (_event.key.keysym.sym == SDLK_5);
			_Keys[SIX][Actual] |= (_event.key.keysym.sym == SDLK_6);
			_Keys[SEVEN][Actual] |= (_event.key.keysym.sym == SDLK_7);
			_Keys[EIGHT][Actual] |= (_event.key.keysym.sym == SDLK_8);
			_Keys[NINE][Actual] |= (_event.key.keysym.sym == SDLK_9);
			_Keys[ZERO][Actual] |= (_event.key.keysym.sym == SDLK_0);
			_Keys[ESCAPE][Actual] |= (_event.key.keysym.sym == SDLK_ESCAPE);
			_Keys[Space][Actual] |= (_event.key.keysym.sym == SDLK_SPACE);
			_Keys[L_SHIFT][Actual] |= (_event.key.keysym.sym == SDLK_LSHIFT);
			_Keys[Ctrl][Actual] |= (_event.key.keysym.sym == SDLK_LCTRL);
			_Keys[Delete][Actual] |= (_event.key.keysym.sym == SDLK_DELETE);
			_Keys[Return][Actual] |= (_event.key.keysym.sym == SDLK_RETURN);
			_Keys[F1][Actual] |= (_event.key.keysym.sym == SDLK_F1);
			_Keys[F2][Actual] |= (_event.key.keysym.sym == SDLK_F2);
			_Keys[F3][Actual] |= (_event.key.keysym.sym == SDLK_F3);
			_Keys[F4][Actual] |= (_event.key.keysym.sym == SDLK_F4);
			_Keys[F5][Actual] |= (_event.key.keysym.sym == SDLK_F5);
			break;
		case SDL_KEYUP:
			_Keys[Up][Actual] &= (_event.key.keysym.sym != SDLK_UP);
			_Keys[Down][Actual] &= (_event.key.keysym.sym != SDLK_DOWN);
			_Keys[Right][Actual] &= (_event.key.keysym.sym != SDLK_RIGHT);
			_Keys[Left][Actual] &= (_event.key.keysym.sym != SDLK_LEFT);
			_Keys[A][Actual] &= (_event.key.keysym.sym != SDLK_a);
			_Keys[Q][Actual] &= (_event.key.keysym.sym != SDLK_q);
			_Keys[Z][Actual] &= (_event.key.keysym.sym != SDLK_z);
			_Keys[E][Actual] &= (_event.key.keysym.sym != SDLK_e);
			_Keys[D][Actual] &= (_event.key.keysym.sym != SDLK_d);
			_Keys[R][Actual] &= (_event.key.keysym.sym != SDLK_r);
			_Keys[F][Actual] &= (_event.key.keysym.sym != SDLK_f);
			_Keys[P][Actual] &= (_event.key.keysym.sym != SDLK_p);
			_Keys[S][Actual] &= (_event.key.keysym.sym != SDLK_s);
			_Keys[W][Actual] &= (_event.key.keysym.sym != SDLK_w);
			_Keys[L][Actual] &= (_event.key.keysym.sym != SDLK_l);
			_Keys[T][Actual] &= (_event.key.keysym.sym != SDLK_t);
			_Keys[ONE][Actual] &= (_event.key.keysym.sym != SDLK_1);
			_Keys[TWO][Actual] &= (_event.key.keysym.sym != SDLK_2);
			_Keys[THREE][Actual] &= (_event.key.keysym.sym != SDLK_3);
			_Keys[FOUR][Actual] &= (_event.key.keysym.sym != SDLK_4);
			_Keys[FIVE][Actual] &= (_event.key.keysym.sym != SDLK_5);
			_Keys[SIX][Actual] &= (_event.key.keysym.sym != SDLK_6);
			_Keys[SEVEN][Actual] &= (_event.key.keysym.sym != SDLK_7);
			_Keys[EIGHT][Actual] &= (_event.key.keysym.sym != SDLK_8);
			_Keys[NINE][Actual] &= (_event.key.keysym.sym != SDLK_9);
			_Keys[ZERO][Actual] &= (_event.key.keysym.sym != SDLK_0);
			_Keys[ESCAPE][Actual] &= (_event.key.keysym.sym != SDLK_ESCAPE);
			_Keys[Space][Actual] &= (_event.key.keysym.sym != SDLK_SPACE);
			_Keys[L_SHIFT][Actual] &= (_event.key.keysym.sym != SDLK_LSHIFT);
			_Keys[Ctrl][Actual] &= (_event.key.keysym.sym != SDLK_LCTRL);
			_Keys[Delete][Actual] &= (_event.key.keysym.sym != SDLK_DELETE);
			_Keys[Return][Actual] &= (_event.key.keysym.sym != SDLK_RETURN);
			_Keys[F1][Actual] &= (_event.key.keysym.sym != SDLK_F1);
			_Keys[F2][Actual] &= (_event.key.keysym.sym != SDLK_F2);
			_Keys[F3][Actual] &= (_event.key.keysym.sym != SDLK_F3);
			_Keys[F4][Actual] &= (_event.key.keysym.sym != SDLK_F4);
			_Keys[F5][Actual] &= (_event.key.keysym.sym != SDLK_F5);
			break;
		case SDL_MOUSEMOTION:
			_x = _event.motion.x;
			_y = _event.motion.y;
			_xRel = _event.motion.xrel;
			_yRel = _event.motion.yrel;
			break;
		case SDL_MOUSEBUTTONDOWN:
			_Keys[R_Clic][Actual] |= (_event.button.button == SDL_BUTTON_RIGHT);
			_Keys[L_Clic][Actual] |= (_event.button.button == SDL_BUTTON_LEFT);
			break;
		case SDL_MOUSEBUTTONUP:
			_Keys[R_Clic][Actual] &= (_event.button.button != SDL_BUTTON_RIGHT);
			_Keys[L_Clic][Actual] &= (_event.button.button != SDL_BUTTON_LEFT);
			break;
		}
	}


}


bool Inputs::GetInput(int k) { return _Keys[k][Actual]; }

bool Inputs::isHoldingInput(int k)
{
	return (_Keys[k][Last] == _Keys[k][Actual] && _Keys[k][Last]);
}

bool Inputs::hasReleasedInput(int k)
{
	return (_Keys[k][Last] != _Keys[k][Actual] && !_Keys[k][Actual]);
}

bool Inputs::pushedInput(int k)
{
	return (_Keys[k][Last] != _Keys[k][Actual] && _Keys[k][Last] == false);
}

bool Inputs::isHoldingGPInput(int k)
{
	return (gamepad.Buttons[k][Last] == gamepad.Buttons[k][Actual] && gamepad.Buttons[k][Last]);
}

bool Inputs::hasReleasedGPInput(int k)
{
	return (gamepad.Buttons[k][Last] != gamepad.Buttons[k][Actual] && !gamepad.Buttons[k][Actual]);
}

bool Inputs::pushedGPInput(int k)
{
	return (gamepad.Buttons[k][Last] != gamepad.Buttons[k][Actual] && gamepad.Buttons[k][Last] == false);
}

bool Inputs::GetControllerInput(int k)
{
	return gamepad.Buttons[k][Actual];
}

float Inputs::GetStickValue(int stick, int axis)
{
	return gamepad.Sticks[stick][axis];
}

float Inputs::GetTriggerValue(int trigger)
{
	return gamepad.Triggers[trigger];
}

void Inputs::Vibrate(float LM, float RM, double duration)
{
	gamepad.Vibrate(LM, RM, duration);
}

int Inputs::GetXRel()
{
	return _xRel;
}

int Inputs::GetYRel()
{
	return _yRel;
}

int Inputs::GetX()
{
	return _x;
}

int Inputs::GetY()
{
	return _y;
}