#include "AudioEngine.h"

void AudioEngine::init()
{
	assert(&Studio != nullptr);
	Studio.LoadBank("Master.bank");
	Studio.LoadBank("Master.strings.bank");
	Studio.LoadBank("Level1.bank");

	//Load all Sounds you use here
	AudioEngine::GetContextBanks()->LoadEvent("event:/Music");
	AudioEngine::GetContextBanks()->LoadEvent("event:/MenuMusic");
	AudioEngine::GetContextBanks()->LoadEvent("event:/LoseMusic");
	AudioEngine::GetContextBanks()->LoadEvent("event:/WinMusic");
	AudioEngine::GetContextBanks()->LoadEvent("event:/Ambience");
	AudioEngine::GetContextBanks()->LoadEvent("event:/CountIn");
	AudioEngine::GetContextBanks()->LoadEvent("event:/Coin Pickup");
	AudioEngine::GetContextBanks()->LoadEvent("event:/Jump");
	AudioEngine::GetContextBanks()->LoadEvent("event:/Death");
	AudioEngine::GetContextBanks()->LoadEvent("event:/Walk");
	AudioEngine::GetContextBanks()->SetVolume("event:/Music", 0.8f);
}
void AudioEngine::SetEventParameters(const std::string eventName, const std::string parameterName, float parameterValue) {
	Banks.SetEventParameter(eventName, parameterName, parameterValue);
}

void AudioEngine::setPitch(const float pitch) {
	//this does nothing woohoo
}
void AudioEngine::setCurrentMusic(std::string eventName) {

	if (eventName != currentMusic) {
		Banks.StopEvent("event:/Music");
		Banks.StopEvent("event:/WinMusic");
		Banks.StopEvent("event:/LoseMusic");
		Banks.StopEvent("event:/MenuMusic");
		Banks.PlayEvent(eventName);
	}

}

ToneFire::FMODStudio* AudioEngine::GetContext()
{
	return &Studio;
}
ToneFire::StudioSound* AudioEngine::GetContextBanks()
{
	return &Banks;
}