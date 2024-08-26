#ifndef _RECORDER_HPP
#define _RECORDER_HPP

#include "sk_guitar_tuner.hpp"

#include <android/log.h>
#include <oboe/Oboe.h>
#include "oboe/AudioStreamBuilder.h"

#include <chrono>
#include <cmath>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class AudioRecorder : public oboe::AudioStreamDataCallback {
public:
    AudioRecorder() : _isRecording(false) {}

    bool startRecording() {
        oboe::AudioStreamBuilder builder;
        builder.setDirection(oboe::Direction::Input)
                ->setFormat(oboe::AudioFormat::Float)
                ->setChannelCount(oboe::ChannelCount::Mono)
                ->setSampleRate(SAMPLE_RATE)
                ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
                ->setDataCallback(this);

        oboe::Result result = builder.openStream(_recordingStream);
        if (result != oboe::Result::OK) {
            __android_log_print(ANDROID_LOG_INFO, NAME_LOG,
                    "Failed to open recording stream. Error: %s", oboe::convertToText(result));
            return false;
        }

        result = _recordingStream->requestStart();
        if (result != oboe::Result::OK) {
            __android_log_print(ANDROID_LOG_INFO, NAME_LOG,
                    "Failed to open recording stream. Error: %s", oboe::convertToText(result));
            return false;
        }

        _isRecording = true;
        return true;
    }

    void stopRecording() {
        if (_recordingStream) {
            _recordingStream->requestStop();
            _recordingStream->close();
            _recordingStream.reset();
        }
        _isRecording = false;
    }

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream* stream,
            void* audioData,
            int32_t numFrames) override {
        float* floatData = static_cast<float*>(audioData);

        std::lock_guard<std::mutex> lock(_dataMutex);

        if (_isRecording) {
            _recordedData.insert(_recordedData.end(), floatData,
                    floatData + numFrames);

            if (_recordedData.size() >= SAMPLE_RATE * RECORDER_TIME_MS / MC_IN_SEC) {

                return oboe::DataCallbackResult::Stop;
            }

            return oboe::DataCallbackResult::Continue;
        } else {
            std::fill(floatData, floatData + numFrames, 0.0f);
        }

        return oboe::DataCallbackResult::Stop;
    }

    void clear(){
        _recordedData.clear();
    }

    std::vector<float>& get_recorded_sound() {
        return _recordedData;
    }

private:
    const char* NAME_LOG = "AudioRecorder";

    std::shared_ptr<oboe::AudioStream> _recordingStream;
    std::vector<float> _recordedData;
    size_t _playbackIndex = 0;
    bool _isRecording;
    std::mutex _dataMutex;
};

#endif //_RECORDER_HPP
