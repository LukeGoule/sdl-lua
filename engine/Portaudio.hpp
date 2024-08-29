#pragma once

#ifndef PORTAUDIO_HPP
#define PORTAUDIO_HPP

#include "EngineModule.hpp"
#include "portaudio.h"

#define PORTAUDIO_MIN_MAG 0.f
#define PORTAUDIO_MAX_MAG 1.f

#define PORTAUDIO_SAMPLE_RATE 44100
#define PORTAUDIO_FRAMES_PER_BUFFER 1024

// Define frequency band ranges
#define PORTAUDIO_LOW_FREQ_END 1000.0
#define PORTAUDIO_MID_FREQ_END 5000.0
#define PORTAUDIO_HIGH_FREQ_END 20000.0

#define PORTAUDIO_HISTORY_LENGTH 70

/*
 *
 * Audio input controller.
 *
 */
class Portaudio: public EngineModule {
private:

	float m_fLowMagnitude = PORTAUDIO_MIN_MAG;
	float m_fMidMagnitude = PORTAUDIO_MIN_MAG;
	float m_fHighMagnitude = PORTAUDIO_MIN_MAG;

	float* m_pLowHistory = nullptr;
	float* m_pMidHistory = nullptr;
	float* m_pHighHistory = nullptr;

	PaStream* m_pStream = nullptr;

public:

	/*
	 * Constructor for the keyboard controller. Allocates memory for the key states.
	 */
	Portaudio(Engine* instance);

	/*
	 * De-allocate the memory for the key states.
	 */
	~Portaudio();

	/*
	 * Initialise Portaudio.
	 */
	bool initialise();

	/*
	 * Cleanup Portaudio.
	 */
	bool cleanup();


	/*
	 * Get low end amp.
	 */
	float getLow();

	/*
	 * Get mid end amp.
	 */
	float getMid();

	/*
	 * Get high end amp.
	 */
	float getHigh();

	/*
	 * Get low end amp pointer.
	 */
	float* getPLow();

	/*
	 * Get mid end amp pointer.
	 */
	float* getPMid();

	/*
	 * Get high end amp pointer.
	 */
	float* getPHigh();


	/*
	 * Get low end amp.
	 */
	float* getLowHistory();

	/*
	 * Get mid end amp.
	 */
	float* getMidHistory();

	/*
	 * Get high end amp.
	 */
	float* getHighHistory();

	/*
	 * Get current stream object.
	 */
	PaStream* getPaStream();

	/*
	 * Callback for when we receive audio. Note that this is asynchronous, and will execute on another thread.
	 */
	static int portAudioCallback(
		const void* inputBuffer, 
		void* outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, 
		void* userData
	);
};

#endif // !PORTAUDIO_HPP