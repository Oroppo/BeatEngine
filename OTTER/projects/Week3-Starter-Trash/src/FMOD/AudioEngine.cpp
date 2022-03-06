#include "AudioEngine.h"

void AudioEngine::init()
{
	assert(&Studio != nullptr);
	Studio.LoadBank("Master.bank");
	Studio.LoadBank("Master.strings.bank");
	Studio.LoadBank("Level1.bank");
}
ToneFire::FMODStudio* AudioEngine::GetContext()
{
	return &Studio;
}
ToneFire::StudioSound* AudioEngine::GetContextBanks()
{
	return &Banks;
}