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
    this->m_frequencies = std::vector<Portaudio::FrequencyBand*>(7);

    this->m_frequencies[0] = new Portaudio::FrequencyBand{ std::string("0 Hz - 60 Hz"), 0, 60, 0.f, new float[PORTAUDIO_HISTORY_LENGTH] };
    this->m_frequencies[1] = new Portaudio::FrequencyBand{ std::string("60 Hz - 150 Hz"), 60, 150, 0.f, new float[PORTAUDIO_HISTORY_LENGTH] };
    this->m_frequencies[2] = new Portaudio::FrequencyBand{ std::string("150 Hz - 400 Hz"), 150, 400, 0.f, new float[PORTAUDIO_HISTORY_LENGTH] };
    this->m_frequencies[3] = new Portaudio::FrequencyBand{ std::string("400 Hz - 1.0 KHz"), 400, 1000, 0.f, new float[PORTAUDIO_HISTORY_LENGTH] };
    this->m_frequencies[4] = new Portaudio::FrequencyBand{ std::string("1.0 KHz - 2.4 KHz"), 1000, 2400, 0.f, new float[PORTAUDIO_HISTORY_LENGTH] };
    this->m_frequencies[5] = new Portaudio::FrequencyBand{ std::string("2.4 KHz - 15 KHz"), 2400, 15000, 0.f, new float[PORTAUDIO_HISTORY_LENGTH] };
    this->m_frequencies[6] = new Portaudio::FrequencyBand{ std::string("15 KHz+"), 15000, INT_MAX, 0.f, new float[PORTAUDIO_HISTORY_LENGTH] };

    for (const auto pFrequencyData : this->m_frequencies) {
        pFrequencyData->m_pAmpHistorySmooth = new float[PORTAUDIO_HISTORY_LENGTH];
    }
}

Portaudio::~Portaudio() {
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
    err = Pa_OpenStream(&this->m_pStream, &params, nullptr, this->m_fSampleRate, PORTAUDIO_FRAMES_PER_BUFFER, paNoFlag, Portaudio::portAudioCallback, nullptr);
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

void Portaudio::enumerateDevices() {
    this->m_devices.clear();

    for (size_t i = 0; i < Pa_GetDeviceCount(); i++) {
        const auto pDevice = new Portaudio::Device;
        pDevice->m_pInfo = Pa_GetDeviceInfo(i);
        pDevice->m_pHostApiInfo = Pa_GetHostApiInfo(pDevice->m_pInfo->hostApi);
        pDevice->m_name = std::string(pDevice->m_pInfo->name);
        this->m_devices.push_back(pDevice);
    }
}

void Portaudio::clearHistories() {
    for (const auto pFreqData : this->m_frequencies)
    {
        delete pFreqData->m_pAmpHistory;
        pFreqData->m_pAmpHistory = new float[PORTAUDIO_HISTORY_LENGTH];
    }
}

int Portaudio::selectDevice(size_t index) {
    Pa_CloseStream(this->m_pStream);

    const auto deviceInfo = Pa_GetDeviceInfo(index);

    auto params = PaStreamParameters{
        (PaDeviceIndex)index,
        deviceInfo->maxInputChannels,
        paFloat32,
        deviceInfo->defaultLowInputLatency
    };

    // Define a set of common sample rates to check
    const double sampleRates[] = {
        192000.0,  // 192 kHz, high-definition audio formats
        176400.0,  // 176.4 kHz, rare but used in some high-fidelity formats
        96000.0,   // 96 kHz, high-definition audio formats
        88200.0,   // 88.2 kHz, higher fidelity than CD, sometimes used in high-quality audio
        48000.0,   // 48 kHz, used in professional audio/video formats
        44100.0,   // 44.1 kHz, standard for audio CDs
        32000.0,   // 32 kHz, used for FM radio and some video/audio formats
        22050.0,   // 22.05 kHz, half of CD quality, used for speech/music with lower bandwidth
        16000.0,   // 16 kHz, used in telephony or speech processing
        11025.0,   // 11.025 kHz, used for low-quality audio
        8000.0     // 8 kHz, used for telephony
    };

    bool found = false;

    for (double sampleRate : sampleRates) {
        PaError err = Pa_IsFormatSupported(nullptr, &params, sampleRate);

        if (err == paFormatIsSupported) {
            this->m_fSampleRate = sampleRate;
            found = true;
            std::cout << "Using " << sampleRate << " Hz as sample rate" << std::endl;
            break;
        }
    }

    // Choose default sample rate if none we know of are supported.
    if (!found) {
        this->m_fSampleRate = deviceInfo->defaultSampleRate;
        std::cout << "Using default sample rate: " << this->m_fSampleRate << std::endl;
    }

    // Open up the handle to the default stream.
    auto err = Pa_OpenStream(&this->m_pStream, &params, nullptr, this->m_fSampleRate, PORTAUDIO_FRAMES_PER_BUFFER, paNoFlag, Portaudio::portAudioCallback, nullptr);
    //err = Pa_OpenDefaultStream(&this->m_pStream, 1, 0, paFloat32, this->m_fSampleRate, PORTAUDIO_FRAMES_PER_BUFFER, Portaudio::portAudioCallback, nullptr);
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

std::vector<Portaudio::Device*> Portaudio::getDevices() {
    return this->m_devices;
}

std::vector<Portaudio::FrequencyBand*> Portaudio::getFrequencyBands() {
    return this->m_frequencies;
}

Portaudio::FrequencyBand* Portaudio::getSingleFrequencyBand(size_t index) {
    return this->m_frequencies[index];
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
    /*if (inputBuffer == nullptr) {
        std::cerr << "Input buffer is null." << std::endl;
        return paContinue;
    }*/

    if (framesPerBuffer == 0) {
        std::cerr << "Buffer size is zero. Please check the buffer setup." << std::endl;
        return paContinue;
    }

    // Cast input buffer to correct type
    float* in = (float*)inputBuffer;

    // lol.
    if (!in) {
        in = (float*)outputBuffer;
    }

    if (!in) {
        std::cout << "no data!" << std::endl;
        return paContinue;
    }

    // FFTW setup
    fftwf_complex* outFFT = (fftwf_complex*)fftw_malloc(sizeof(fftw_complex) * framesPerBuffer);
    fftwf_plan p = fftwf_plan_dft_r2c_1d(framesPerBuffer, (float*)in, outFFT, FFTW_MEASURE);// FFTW_ESTIMATE);

    // Execute FFT
    fftwf_execute(p);

    int fftSize = framesPerBuffer / 2;
    double freqStep = (double)PORTAUDIO()->m_fSampleRate / framesPerBuffer;

    if (freqStep <= 0) {
        std::cerr << "Frequency step calculation error." << std::endl;
        return paContinue;
    }

    // Reset total amplitude for calculation.
    for (const auto pFreqBand : PORTAUDIO()->getFrequencyBands())
    {
        pFreqBand->m_fTotalAmp = 0.f;
    }

    // Calculate amplitudes within FFT, within our ranges.
    for (int i = 0; i <= fftSize; ++i) 
    {
        double frequency = i * freqStep;
        double magnitude = sqrt(outFFT[i][0] * outFFT[i][0] + outFFT[i][1] * outFFT[i][1]);

        if (isnan(magnitude) || isinf(magnitude)) {
            magnitude = 0.0;
        }

        for (const auto pFreqBand : PORTAUDIO()->getFrequencyBands())
        {
            if (frequency > pFreqBand->m_startFrequency && frequency <= pFreqBand->m_endFrequency) {
                pFreqBand->m_fTotalAmp += magnitude;
            }
        }
    }

    // Shift the history.
    for (const auto pFreqBand : PORTAUDIO()->getFrequencyBands())
    {
        float alpha = OPTIONS()->get()->m_fSmoothFactor;                      // Smoothing factor (adjust as needed)

        pFreqBand->m_fCurrentAmp         = pFreqBand->m_fTotalAmp / (fftSize + 1);
        pFreqBand->m_fCurrentAmpSmoothed = alpha * pFreqBand->m_fCurrentAmp + (1.0f - alpha) * pFreqBand->m_fCurrentAmpSmoothed;

        shiftArrayLeft(pFreqBand->m_pAmpHistory, PORTAUDIO_HISTORY_LENGTH, pFreqBand->m_fCurrentAmp);
        shiftArrayLeft(pFreqBand->m_pAmpHistorySmooth, PORTAUDIO_HISTORY_LENGTH, pFreqBand->m_fCurrentAmpSmoothed);
    }

    // Cleanup
    fftwf_destroy_plan(p);
    fftwf_free(outFFT);

    return paContinue;
}