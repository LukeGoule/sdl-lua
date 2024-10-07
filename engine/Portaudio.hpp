#pragma once

#ifndef PORTAUDIO_HPP
#define PORTAUDIO_HPP

#include "EngineModule.hpp"
#include "portaudio.h"

#include <vector>
#include <string>

#define PORTAUDIO_MIN_MAG 0.f
#define PORTAUDIO_MAX_MAG 1.f

#define PORTAUDIO_SAMPLE_RATE 44100
#define PORTAUDIO_FRAMES_PER_BUFFER 1024

// Define frequency band ranges
#define PORTAUDIO_LOW_FREQ_END 1000.0
#define PORTAUDIO_MID_FREQ_END 5000.0
#define PORTAUDIO_HIGH_FREQ_END 20000.0

#define PORTAUDIO_HISTORY_LENGTH 150

#define PORTAUDIO_FI__0_60		0 // 0 Hz       -> 60 Hz
#define PORTAUDIO_FI__60_150	1 // 60 Hz      -> 150 Hz
#define PORTAUDIO_FI__150_400	2 // 150 Hz     -> 400 Hz
#define PORTAUDIO_FI__400_1K	3 // 400 Hz     -> 1,000 Hz
#define PORTAUDIO_FI__1K_2_4K	4 // 1,000 Hz   -> 2,400 Hz
#define PORTAUDIO_FI__2_4K_15K	5 // 2,400 Hz   -> 15,000 Hz
#define PORTAUDIO_FI__15K_PLUS	6 // 15,0000 Hz onwards

/*
 *
 * Audio input controller.
 *
 */
class Portaudio: public EngineModule {
public:
	float m_fSampleRate = PORTAUDIO_SAMPLE_RATE;

	struct Device {
		std::string m_name;
		const PaDeviceInfo* m_pInfo;
		const PaHostApiInfo* m_pHostApiInfo;
	};

	struct FrequencyBand {
		std::string m_name;
		int m_startFrequency;
		int m_endFrequency;
		float m_fCurrentAmp = PORTAUDIO_MIN_MAG;
		float* m_pAmpHistory = nullptr;
		float* m_pAmpHistorySmooth = nullptr;
		float m_fTotalAmp = 0.f;
		float m_fCurrentAmpSmoothed = 0.f;
	};
private:
	
	PaStream* m_pStream = nullptr;

	std::vector<Portaudio::Device*> m_devices;
	std::vector<Portaudio::FrequencyBand*> m_frequencies;

public:

	/* Constructor for the keyboard controller. Allocates memory for the key states. */
	Portaudio(Engine* instance);

	/* De-allocate the memory for the key states. */
	~Portaudio();

	/* Initialise Portaudio. */
	bool initialise();

	/* Cleanup Portaudio */
	bool cleanup();

	/* Load device list */
	void enumerateDevices();

	/* Clear all frequency history arrays. */
	void clearHistories();
	
	/* Re-initialise everything with a new device index. */
	int selectDevice(size_t index);

	/* List the known devices. Call enumerateDevices to refresh. */
	std::vector<Portaudio::Device*> getDevices();

	/* Returns the data of all frequency bands. */
	std::vector<Portaudio::FrequencyBand*> getFrequencyBands();

	/* Returns data of single frequency band */
	Portaudio::FrequencyBand* getSingleFrequencyBand(size_t index);

	/* Get current stream object. */
	PaStream* getPaStream();

	/* Callback for when we receive audio. Note that this is asynchronous, and will execute on another thread. */
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