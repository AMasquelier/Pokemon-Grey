#include "Sound.h"


// Music
Music::Music()
{
}

Music::Music(string filename)
{
	_filename = filename;
	_music = Mix_LoadMUS(filename.c_str());
}

Music::~Music()
{
	Mix_FreeMusic(_music);
}

void Music::Load(string filename)
{
	if (_music != nullptr)
	{
		if (filename != _filename)
		{
			Mix_FreeMusic(_music);
			_music = Mix_LoadMUS(filename.c_str());
		}
	}
	else 
		_music = Mix_LoadMUS(filename.c_str());
	_filename = filename;
}

void Music::Destroy()
{
	Mix_FreeMusic(_music);
}

string Music::Get_filename()
{
	return _filename;
}

void Music::Play()
{
	Mix_PlayMusic(_music, -1);
}


// Sound
Sound::Sound()
{
}

Sound::Sound(string filename)
{
	_filename = filename;
	_sound = Mix_LoadWAV(filename.c_str());
}

Sound::~Sound()
{
	Mix_FreeChunk(_sound);
}

void Sound::Load(string filename)
{
	if (_sound != nullptr)
		Mix_FreeChunk(_sound);

	_sound = Mix_LoadWAV(filename.c_str());
	_filename = filename;
}

void Sound::Destroy()
{
	Mix_FreeChunk(_sound);
}

string Sound::Get_filename()
{
	return _filename;
}

void Sound::Play()
{
	Mix_PlayChannel(1, _sound, 0);
}



// Sound Manager
float Sound_manager::_volume = MIX_MAX_VOLUME / 2;
bool Sound_manager::_fade_in = false;
string Sound_manager::_last_music_filename;
void Sound_manager::fade_in()
{
	_fade_in = true;
	_volume = 0;
}

void Sound_manager::Update_sound()
{
	if (_fade_in)
	{
		_volume += MIX_MAX_VOLUME / 200.0;
		if (_volume >= MIX_MAX_VOLUME / 2)
		{
			_volume = MIX_MAX_VOLUME / 2;
			_fade_in = false;
		}
	}
	Mix_VolumeMusic(_volume);
}

void Sound_manager::Play_music(Music &m)
{
	if (_last_music_filename != m.Get_filename())
	{
		_last_music_filename = m.Get_filename();
		m.Play();
	}
}

bool Sound_manager::is_playing_this(string filename)
{
	return (filename == _last_music_filename);
}
