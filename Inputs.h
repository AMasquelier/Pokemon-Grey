#ifndef INPUTS_H_INCLUDED
#define INPUTS_H_INCLUDED

#include <iostream>
#include <Windows.h>
#include <SDL2\SDL.h>
#include <thread>
#include <Xinput.h>
using namespace std;

class Gamepad
{
public:
	Gamepad();
	bool isConnected();
	void Update();
	void Vibrate(float LMotor, float RMotor, double duration);
	void StopVibration();

public:
	float Triggers[3];
	bool Buttons[14][2];
	float Sticks[2][2];
	enum { Last, Actual };
	enum ButtonsName { DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT, START, BACK, LEFT_THUMB, RIGHT_THUMB, LB, RB, A, B, X, Y };
	enum AxisName { X_AXIS, Y_AXIS };
	enum TriggersName { LEFT, RIGHT, MAX };
private:
	double VChrono = 0, Vduration = 0;
	XINPUT_STATE state;
};

class Inputs
{
private:
	Gamepad gamepad;

	static int unichar;
	static bool _typing;
	static int _Event;
	bool _Keys[64][2];



public:
	static bool CloseGame;
	SDL_Event _event;
	int _xRel, _yRel;
	int _x, _y, _z = 0;
	int _AFK = 1;
	int _nbJoysticks;
	Inputs();
	//void Load();
	//static void UpdateEvents();
	void UpdateKeyboardInputs(bool mode);
	void UpdateMouseInputs(bool mode);
	void UpdateControllerInputs(bool mode);
	//void SetInput(const int k, bool a);
	//void Entry(string *text, bool Num);
	//int GetEvent();
	bool GetInput(int k);
	bool isHoldingInput(int k);
	bool hasReleasedInput(int k);
	bool pushedInput(int k);
	bool isHoldingGPInput(int k);
	bool hasReleasedGPInput(int k);
	bool pushedGPInput(int k);
	bool GetControllerInput(int k);
	float GetStickValue(int stick, int axis);
	float GetTriggerValue(int trigger);
	void Vibrate(float LM, float RM, double duration);
	int GetXRel();
	int GetX();
	int GetYRel();
	int GetY();
	enum { Last, Actual };
	enum m_JButt { Ab, Bb, Yb, Xb, LB, RB, SELECT, START };
	enum m_keys { F1, F2, F3, F4, F5, Return, Delete, WheelDown, WheelUp, Down, Up, Right, Left, Space, A, Q, Z, E, R, F, S, P, D, W, L, T, ESCAPE, L_SHIFT, Clic, L_Clic, R_Clic, Ctrl, Quit, Move, WhD, WhU, Input, Back, Next, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, ZERO };
};

#endif
