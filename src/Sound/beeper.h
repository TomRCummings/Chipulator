/*
Beeper: Uses SDL to handle sound generation. Only produces one tone at a time, but frequency and type of wave generated can be changed. The sound generated is very clippy when being turned on and off: needs an
envelope to smooth it out.
*/

#pragma once

#include <SDL.h>
#include <SDL_audio.h>
#include <cmath>
#include "../Utility/logger.h"

class Beeper {
public:
	//Initializes SDL audio, audio device, and audio specs
	void initializeAudio();
	//Function called by SDL audio_callback that fills stream with samples
	void generateSamples(short* stream, int length);
	//Turn sound on or off
	void setSoundOn(bool soundOnOrOff);
	//Turn mute on or off
	void setMuteOn();
	//Set timbre of tone produced by beeper
	void setWaveType(int waveTypeID);
	//Set tone (in Hz) produced by beeper
	void setWaveTone(int waveHz);

private:
	//Should there be sound right now
	bool soundOn = false;
	//Is sound muted
	bool muteOn = false;
	//Type of wave that should be generated
	int waveType = 0;
	//Tone that the wave will produce (may or may not be applicable based on wave type)
	float waveTone = 440;
	//Running index for sampling
	float samplingIndex = 0;

	//Device ID of active audio device
	SDL_AudioDeviceID activeDeviceID;

	//These are useful variables that cannot be changed outside of this file:
	//Volume
	const short amplitude = 25000;
	//Sampling rate
	const int samplingRate = 44100;
	//Buffer size
	const int bufferSize = 1024;

	//Samples a sine wave at a given index
	float sampleSine(float index);
	//Samples a square wave at a given index
	float sampleSquare(float index);
};