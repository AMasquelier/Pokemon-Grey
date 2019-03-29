#include "Dialogue.h"


void Dialogue::Init()
{
}

void Dialogue::Destroy()
{
	_act_pos = 0;
	_text.clear();
	_data.clear();
	_choices.clear();
	_conditions.clear();
}

void Dialogue::Update()
{
	for (int i = 0; i < _conditions.size(); i++)
	{
		if (_conditions[i].i1[0] == '{')
		{
			string buf = _conditions[i].i1;
			buf.erase(buf.begin());
			buf.erase(buf.end());
			int n = stoi(buf);
			if (n < _choices.size())
				_conditions[i].v1 = _choices[n].data;
			else
				cout << "Invalid choice number in condition n." << i << endl;
		}
		else _conditions[i].v1 = stoi(_conditions[i].i1);

		if (_conditions[i].i2[0] == '{')
		{
			string buf = _conditions[i].i2;
			buf.erase(buf.begin());
			buf.erase(buf.end());
			int n = stoi(buf);
			if (n < _choices.size())
				_conditions[i].v2 = _choices[n].data;
			else
				cout << "Invalid choice number in condition n." << i << endl;
		}
		else _conditions[i].v2 = stoi(_conditions[i].i2);
	}
}

void Dialogue::Play()
{
	int line_n = 0;
	int choice = 0;
	vector<int> next_lines;

	while (line_n < _text.size())
	{
		for (int i = 0; i < _choices.size(); i++)
		{
			if (_choices[i].pos == line_n)
			{
				for (int j = 0; j < _choices[i].choices.size(); j++)
					cout << "    " << j + 1 << " : " << _choices[i].choices[j] << endl;

				cin >> choice;
				_choices[i].data = choice;
			}
		}
		Update();
		for (int i = 0; i < _conditions.size(); i++)
		{
			if (_conditions[i].pos == line_n)
			{
				if (!_conditions[i].test())
					line_n = _conditions[i].end + 1;
			}
		}

		cout << line_n << " > " << _text[line_n] << endl;


		Sleep(500);

		line_n++;
	}
}

void Dialogue::LoadScript(string ID)
{
	ifstream file("Dialogue.txt", std::ios::in);
	int pos = 0;
	string act_line;
	if (file)
	{
		int pos_ID = find_ID(ID, &file);
		if (pos_ID != -1)
		{
			file.seekg(pos_ID);

			file >> act_line;
			while (act_line != "<END>")
			{
				if (act_line == ">")
				{
					string buf;
					file >> buf;
					_text.push_back(buf);
					pos++;
				}
				else if (act_line == "<CHOICE>")
				{

					Choice buf1;
					string buf2;
					buf1.pos = pos;
					file >> buf2;
					while (buf2 != "</>")
					{
						buf1.choices.push_back(buf2);
						file >> buf2;
					}
					_choices.push_back(buf1);
				}
				else if (act_line == "<IF")
				{
					If buf;
					string i1, i2;
					string cond, trash;
					file >> i1 >> cond >> i2 >> trash;
					i2.erase(i2.end() - 1);
					int last_pos = file.tellg();

					buf.i1 = i1; buf.i2 = i2; buf.condition = cond;
					buf.pos = pos;
					file >> trash;
					int size_count = 0;
					while (trash != "}")
					{
						if (trash == ">")
						{
							file >> trash;
							size_count++;
						}
						file >> trash;
					}
					buf.end = pos - 1 + size_count;
					_conditions.push_back(buf);
					file.seekg(last_pos);
				}

				file >> act_line;
			}
			file.close();
		}
		else cout << "Missing Dialogues file" << endl;

		if (debug)
		{
			cout << "Dialogue - debug" << endl << endl;
			cout << _text.size() << " Dialogue lines" << endl;
			for (int i = 0; i < _text.size(); i++)
				cout << "    " << i << " > " << _text[i] << endl;
			cout << "------------------------------------------------------" << endl;
			cout << _choices.size() << " choices to make" << endl;
			for (int i = 0; i < _choices.size(); i++)
			{
				cout << "    Choice n." << i + 1 << "(size : " << _choices[i].choices.size() << ", pos : " << _choices[i].pos << ")" << endl;
				for (int j = 0; j < _choices[i].choices.size(); j++)
					cout << "        " << _choices[i].choices[j] << endl;
			}

			cout << _conditions.size() << " conditions" << endl;
			for (int i = 0; i < _conditions.size(); i++)
			{
				cout << "    Condition n." << i + 1 << "(start : " << _conditions[i].pos << ", end : " << _conditions[i].end << ", " << _conditions[i].i1 << _conditions[i].condition << _conditions[i].i2 << ")" << endl;

			}
		}
		cout << "_____________________________________________________" << endl << endl;
	}
	else cout << "No " << ID << " found" << endl;
	_act_pos = 0;
}

string Dialogue::GetNext()
{
	Update();
	if (_act_pos < _text.size())
	{
		for (int i = 0; i < _conditions.size(); i++)
		{
			if (_conditions[i].pos == _act_pos)
			{
				if (!_conditions[i].test())
					_act_pos = _conditions[i].end + 1;
			}
		}
		_act_pos++;
		return _text[_act_pos - 1];
	}
	return "#END_OF_DIALOGUE#";
}

Choice * Dialogue::Choice_to_make()
{
	Choice *ret = nullptr;
	for (int i = 0; i < _choices.size(); i++)
	{
		if (_choices[i].pos == _act_pos)
		{
			ret = &_choices[i];
			break;
		}
	}
	return ret;
}

int Dialogue::find_ID(string ID, ifstream *file)
{
	int beg_pos = file->tellg();
	bool found = false;
	string act_line;

	while (!file->eof() && !found)
	{
		int last_pos = file->tellg();
		(*file) >> act_line;
		if (act_line == ("#" + ID))
		{
			cout << "pos : " << last_pos << endl;
			(*file) >> act_line;
			found = true;
			return last_pos;
		}

	}
	return -1;
}