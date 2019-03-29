#pragma once
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
using namespace std;

class Music
{
	public:
		Music();
		Music(string filename);
		~Music();
		void Load(string filename);
		void Destroy();
		string Get_filename();
		void Play();

	private:
		Mix_Music *_music;
		string _filename;
		
};

class Sound
{
	public:
		Sound();
		Sound(string filename);
		~Sound();
		void Load(string filename);
		void Destroy();
		string Get_filename();
		void Play();

	private:
		Mix_Chunk *_sound;
		string _filename;

};

class Sound_manager
{
	public:
		static void fade_in();
		static void Update_sound();
		static void Play_music(Music &m);
		static bool is_playing_this(string filename);

	private:
		static string _last_music_filename;
		static float _volume;
		static bool _fade_in;
};
