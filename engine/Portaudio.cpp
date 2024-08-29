#include "Portaudio.hpp"
#include "fftw3.h"
#include "Engine.hpp"

#include <iostream>

static void shiftArrayLeft(float* arr, int length, float newValue) {
    // Shift elements left
    for (int i = 0; i < length - 1; ++i) {
        arr[i] = arr[i + 1];
    }
    // Append new value at the end
    arr[length - 1] = newValue;
}

Portaudio::Portaudio(Engine* engine) : EngineModule(engine) {
    this->m_pLowHistory = new float[PORTAUDIO_HISTORY_LENGTH];
    this->m_pMidHistory = new float[PORTAUDIO_HISTORY_LENGTH];
    this->m_pHighHistory = new float[PORTAUDIO_HISTORY_LENGTH];

    for (size_t i = 0; i < PORTAUDIO_HISTORY_LENGTH; i++) {
        this->m_pLowHistory[i] = 0.f;
        this->m_pMidHistory[i] = 0.f;
        this->m_pHighHistory[i] = 0.f;
    }
}

Portaudio::~Portaudio() {
    delete this->m_pLowHistory;
    delete this->m_pMidHistory;
    delete this->m_pHighHistory;
}

bool Portaudio::initialise() {
    PaError err = Pa_Initialize(); // Initialise the library
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    for (size_t i = 0; i < Pa_GetDeviceCount(); i++) {
        auto pDevice = Pa_GetDeviceInfo(i);
        auto pHostApi = Pa_GetHostApiInfo(pDevice->hostApi);

        std::cout << "[" << i << "] " << "Device: " << pDevice->name << "(O: " << pDevice->maxOutputChannels << " I: " << pDevice->maxInputChannels << ")  -- Host : " << pHostApi->name << std::endl;
    }

    auto params = PaStreamParameters{
        (PaDeviceIndex)1,
        2,
        paFloat32,
        Pa_GetDeviceInfo(0)->defaultLowInputLatency
    };

    // Open up the handle to the default stream.
    err = Pa_OpenStream(&this->m_pStream, &params, nullptr, PORTAUDIO_SAMPLE_RATE, PORTAUDIO_FRAMES_PER_BUFFER, paNoFlag, Portaudio::portAudioCallback, nullptr);
    //err = Pa_OpenDefaultStream(&this->m_pStream, 1, 0, paFloat32, PORTAUDIO_SAMPLE_RATE, PORTAUDIO_FRAMES_PER_BUFFER, Portaudio::portAudioCallback, nullptr);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    // Start listening. This opens on a new thread.
    err = Pa_StartStream(m_pStream);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }   
}

bool Portaudio::cleanup() {
    auto err = Pa_StopStream(m_pStream);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    
    err = Pa_CloseStream(m_pStream);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    err = Pa_Terminate();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    return true;
}

float Portaudio::getLow() {
    return this->m_fLowMagnitude;
}

float Portaudio::getMid() {
    return this->m_fMidMagnitude;
}

float Portaudio::getHigh() {
    return this->m_fHighMagnitude;
}

float* Portaudio::getPLow() {
    return &this->m_fLowMagnitude;
}

float* Portaudio::getPMid() {
    return &this->m_fMidMagnitude;
}

float* Portaudio::getPHigh() {
    return &this->m_fHighMagnitude;
}

float* Portaudio::getLowHistory() {
    return this->m_pLowHistory;
}

float* Portaudio::getMidHistory() {
    return this->m_pMidHistory;
}

float* Portaudio::getHighHistory() {
    return this->m_pHighHistory;
}

PaStream* Portaudio::getPaStream() {
    return this->m_pStream;
}

int Portaudio::portAudioCallback(
    const void* inputBuffer,
    void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData
) {
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
    double freqStep = (double)PORTAUDIO_SAMPLE_RATE / framesPerBuffer;
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

        if (frequency < PORTAUDIO_LOW_FREQ_END) {
            lowAmplitude += magnitude;
        }
        else if (frequency < PORTAUDIO_MID_FREQ_END) {
            midAmplitude += magnitude;
        }
        else if (frequency < PORTAUDIO_HIGH_FREQ_END) {
            highAmplitude += magnitude;
        }
    }

    // Normalize the amplitudes (example normalization)
    lowAmplitude /= framesPerBuffer / 2;
    midAmplitude /= framesPerBuffer / 2;
    highAmplitude /= framesPerBuffer / 2;

    const auto pEngine = &Engine::getInstance();
    const auto pPortaudio = pEngine->getPortaudio();

    pPortaudio->m_fLowMagnitude = lowAmplitude;
    pPortaudio->m_fMidMagnitude = midAmplitude;
    pPortaudio->m_fHighMagnitude = highAmplitude;

    shiftArrayLeft(pPortaudio->m_pLowHistory, PORTAUDIO_HISTORY_LENGTH, lowAmplitude);
    shiftArrayLeft(pPortaudio->m_pMidHistory, PORTAUDIO_HISTORY_LENGTH, midAmplitude);
    shiftArrayLeft(pPortaudio->m_pHighHistory, PORTAUDIO_HISTORY_LENGTH, highAmplitude);

    // Cleanup
    fftwf_destroy_plan(p);
    fftwf_free(outFFT);

    return paContinue;
}