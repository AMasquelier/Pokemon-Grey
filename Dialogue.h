#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

static bool debug = false;


typedef struct Choice
{
	vector<string> choices;
	int pos;
	int data = -1;
} Choice;

typedef struct If
{
	string i1, i2;
	int v1, v2;
	string condition;

	bool test()
	{
		if (condition == "==")
			return v1 == v2;
		else if (condition == "<")
			return v1 < v2;
		else if (condition == "<=")
			return v1 <= v2;
		else if (condition == ">")
			return v1 > v2;
		else if (condition == ">=")
			return v1 >= v2;
		else if (condition == "!=")
			return v1 != v2;
	}
	int pos, end;
} If;


class Dialogue
{
public:
	void Init();
	void Destroy();
	void Update();
	void Play();
	void LoadScript(string ID);
	string GetNext();
	Choice *Choice_to_make();

private:
	int find_ID(string ID, ifstream *file);


	string _ID;
	vector<string> _text;
	vector<int> _data;
	vector<Choice> _choices;
	vector<If> _conditions;

	int _act_pos = 0;
};