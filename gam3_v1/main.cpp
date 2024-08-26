#include "Engine.hpp"

#include "portaudio.h"
#include "fftw3.h"

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 1024

// Define frequency band ranges
#define LOW_FREQ_END 1000.0
#define MID_FREQ_END 5000.0
#define HIGH_FREQ_END 20000.0

// Example callback function for PortAudio
static int audioCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags, void* userData) {

    // Check if the buffer is receiving data
    if (inputBuffer == nullptr) {
        std::cerr << "Input buffer is null." << std::endl;
        return paContinue;
    }

    if (framesPerBuffer == 0) {
        std::cerr << "Buffer size is zero. Please check the buffer setup." << std::endl;
        return paContinue;
    }
    
    // Cast input buffer to correct type
    float* in = (float*)inputBuffer;

    // FFTW setup
    fftwf_complex* outFFT = (fftwf_complex*)fftw_malloc(sizeof(fftw_complex) * framesPerBuffer);
    fftwf_plan p = fftwf_plan_dft_r2c_1d(framesPerBuffer, (float*)in, outFFT, FFTW_MEASURE);// FFTW_ESTIMATE);

    // Execute FFT
    fftwf_execute(p);

    // Frequency band separation
    double lowAmplitude = 0.0, midAmplitude = 0.0, highAmplitude = 0.0;
    double freqStep = (double)SAMPLE_RATE / framesPerBuffer;
    if (freqStep <= 0) {
        std::cerr << "Frequency step calculation error." << std::endl;
        return paContinue;
    }
    
    for (int i = 0; i < framesPerBuffer / 2; ++i) {
        double frequency = i * freqStep;
        double magnitude = sqrt(outFFT[i][0] * outFFT[i][0] + outFFT[i][1] * outFFT[i][1]);
        
        if (isnan(magnitude) || isinf(magnitude)) {
            magnitude = 0.0;
        }

        if (frequency < LOW_FREQ_END) {
            lowAmplitude += magnitude;
        }
        else if (frequency < MID_FREQ_END) {
            midAmplitude += magnitude;
        }
        else if (frequency < HIGH_FREQ_END) {
            highAmplitude += magnitude;
        }
    }

    // Normalize the amplitudes (example normalization)
    lowAmplitude /= framesPerBuffer / 2;
    midAmplitude /= framesPerBuffer / 2;
    highAmplitude /= framesPerBuffer / 2;

    const auto instance = &Engine::getInstance();
    instance->getRender()->setAmplitudes(lowAmplitude, midAmplitude, highAmplitude);

    //std::cout << "Frames: " << framesPerBuffer << std::endl;
    //std::cout << "Low: " << lowAmplitude << " Mid: " << midAmplitude << " High: " << highAmplitude << std::endl;

    printf("Low: %8.f Mid: %.8f High: %.8f\n", lowAmplitude * 100.f, midAmplitude * 100.f, highAmplitude * 100.f);

    // Cleanup
    fftwf_destroy_plan(p);
    fftwf_free(outFFT);

    return paContinue;
}


#undef main

int main(int argc, char** argv) {
	const auto m_engine = &Engine::getInstance();

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    const auto defaultOut = Pa_GetDefaultOutputDevice();
    const auto deviceInfo = Pa_GetDeviceInfo(defaultOut);
    std::cout << "Using device: " << deviceInfo->name << "with " << deviceInfo->maxOutputChannels << " output channels, with a sample rate of " << deviceInfo->defaultSampleRate << std::endl;
    getchar();

    PaStream* stream;

    const auto params = new PaStreamParameters{
        defaultOut,
        1,
        paFloat32,
        deviceInfo->defaultLowOutputLatency,
        NULL
    };
    
    //err = Pa_OpenStream(&stream, NULL, params, SAMPLE_RATE, FRAMES_PER_BUFFER, paNoFlag, audioCallback, NULL);

    err = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, nullptr);
    
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }
    
    err = Pa_StartStream(stream);
    
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    std::cout << "Listening to audio..." << std::endl;
	
	m_engine
		->Init()
		->LoadScript("lua_base/main.lua");

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

#ifdef _DEBUG
	return getchar();
#else
	return 0;
#endif
}