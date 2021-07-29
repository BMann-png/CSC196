#include "AudioSystem.h"

namespace pbls
{
	void AudioSystem::Startup()
	{
		FMOD::System_Create(&fmodSystem);

		void* extradriverdata = nullptr;
		fmodSystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
	}

	void AudioSystem::Shutdown()
	{
		for (auto& sound : sounds)
		{
			sound.second->release();
		}
		sounds.clear();

		fmodSystem->close();
		fmodSystem->release();
	}

	void AudioSystem::Update(float dt)
	{
		fmodSystem->update();
	}

	void AudioSystem::AddAudio(const std::string& name, const std::string& filename)
	{
		if (sounds.find(name) == sounds.end())
		{
			FMOD::Sound* sound{ nullptr };
			fmodSystem->createSound(filename.c_str(), FMOD_DEFAULT, 0, &sound);
			sounds[name] = sound;
		}
	}

	void AudioSystem::PlayAudio(const std::string& name)
	{
		auto iter = sounds.find(name);
		if (iter != sounds.end())
		{
			FMOD::Sound* sound = iter->second;
			sound->setMode(FMOD_LOOP_OFF);
			FMOD::Channel* channel;
			fmodSystem->playSound(sound, 0, false, &channel);
		}
	}
}
