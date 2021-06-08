#include <beeper.h>

//Callback function passed to SDL
void fillBuffer(void* userdata, Uint8* _stream, int len) {
	short * stream = reinterpret_cast<short*>(_stream);
	int length = len;
	Beeper* beeper = (Beeper*)userdata;

	beeper->generateSamples(stream, length);
}

void Beeper::initializeAudio() {
	SDL_AudioSpec desired, returned;
	SDL_AudioDeviceID devID;

	SDL_zero(desired);
	desired.freq = samplingRate;
	desired.format = AUDIO_S16SYS; //16-bit audio
	desired.channels = 1;
	desired.samples = bufferSize;
	desired.callback = &fillBuffer;
	desired.userdata = this;

	devID = SDL_OpenAudioDevice(NULL, 0, &desired, &returned, 0);
	SDL_PauseAudioDevice(devID, 0);
	activeDeviceID = devID;
}

void Beeper::generateSamples(short *stream, int length) {
	int samplesToWrite = length / sizeof(short);
	for (int i = 0; i < samplesToWrite; i++) {
		if (soundOn && !muteOn) {
			if (waveType == 0) {
				stream[i] = (short)(amplitude * sampleSine(samplingIndex));
			}
			else if (waveType == 1) {
				stream[i] = (short)(amplitude * 0.2 * sampleSquare(samplingIndex));
			}
		}
		else {
			stream[i] = 0;
		}
		samplingIndex += (waveTone * M_PI * 2) / samplingRate;
		if (samplingIndex >= (M_PI*2)) {
			samplingIndex -= M_PI * 2;
		}
	}
}

void Beeper::setSoundOn(bool soundOnOrOff) {
	SDL_LockAudioDevice(activeDeviceID);
	soundOn = soundOnOrOff;
	SDL_UnlockAudioDevice(activeDeviceID);
}

void Beeper::setMuteOn() {
	SDL_LockAudioDevice(activeDeviceID);
	muteOn = !muteOn;
	SDL_UnlockAudioDevice(activeDeviceID);
}

void Beeper::setWaveType(int waveTypeID) {
	SDL_LockAudioDevice(activeDeviceID);
	waveType = waveTypeID;
	SDL_UnlockAudioDevice(activeDeviceID);
}

void Beeper::setWaveTone(int waveHz) {
	SDL_LockAudioDevice(activeDeviceID);
	waveTone = waveHz;
	SDL_UnlockAudioDevice(activeDeviceID);
}

float Beeper::sampleSine(float index) {
	double result = sin((index));
	return result;
}

float Beeper::sampleSquare(float index)
{
	float unSquaredSin = sin((index));
	if (unSquaredSin >= 0) {
		return 1.0f;
	}
	else {
		return -1.0f;
	}
}