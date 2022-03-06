#pragma once
#include "Fmod.h"
#include "FMOD/ToneFire.h"
#include <memory>
#include <assert.h>

//Static Helper Class. Do not instantiate. I will make the compiler yell at you.
//Note: this class is not 100% thread safe for multithreaded systems.
static class AudioEngine
{
public:
	typedef std::shared_ptr<AudioEngine> Sptr;

	static void init();

	static ToneFire::FMODStudio* GetContext();

	static ToneFire::StudioSound* GetContextBanks();

	//Static Singletons for use throughout the project
	inline static ToneFire::FMODStudio Studio;
	inline static ToneFire::StudioSound Banks;
};

