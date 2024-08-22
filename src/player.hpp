#ifndef _PLAYER_HPP

#include "oboe/AudioStreamBuilder.h"
#include "sk_guitar_tuner.hpp"

#include <android/log.h>
#include <chrono>
#include <cmath>
#include <memory>
#include <mutex>
#include <oboe/Oboe.h>
#include <thread>
#include <map>

class AudioData {
    AudioData(char* path) {
        if (path == nullptr) {
            return;
        }
        if (sound.empty()) {
//            WAVHeader header;
//            sound = readFloatWAV(path, header);
//
//            __android_log_print(ANDROID_LOG_INFO, NAME_LOG, "SOUND WAV SIZE (SAMPLE_SIZE): %lu",
//                    sound.size());
//            __android_log_print(ANDROID_LOG_INFO, NAME_LOG, "SOUND WAV Sample Rate: %d", header.sampleRate);
//            __android_log_print(ANDROID_LOG_INFO, NAME_LOG, "SOUND WAV Channels: %d", header.numChannels);
//            __android_log_print(ANDROID_LOG_INFO, NAME_LOG, "SOUND WAV Bits per Sample: %d",
//                    header.bitsPerSample);

        }
    }

    AudioData(const AudioData& oth) = delete;

    AudioData(AudioData&& oth) = delete;

    ~AudioData() {
    }

public:
    static AudioData* get_audio_data(char* path=nullptr) {
        static AudioData audio_data(path);
        return &audio_data;
    }

    const char * NAME_LOG = "Player";

    std::vector<float> sound;
};

class AudioPlayer : public oboe::AudioStreamDataCallback {
public:
    AudioPlayer() : _isPlaying(false) {
        oboe::Result result = startPlayback();
        if (result != oboe::Result::OK) {
            __android_log_print(ANDROID_LOG_INFO, "OboeAudioPlayer",
                    "\033[1;41m Stream started wrong.\033[0m");
        } else {
            __android_log_print(ANDROID_LOG_INFO, "OboeAudioPlayer",
                    "\033[1;42m Stream started successfully.\033[0m");
        }
    }

    oboe::Result startPlayback() {
        oboe::AudioStreamBuilder builder;
        builder.setDirection(oboe::Direction::Output)
                ->setFormat(oboe::AudioFormat::Float)
                ->setChannelCount(oboe::ChannelCount::Mono)
                ->setSampleRate(SAMPLE_RATE)
                ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
                ->setDataCallback(this);

        oboe::Result result = builder.openStream(_playbackStream);
        if (result != oboe::Result::OK) {
            std::cerr << "Failed to open playback stream. Error: "
                      << oboe::convertToText(result) << std::endl;
            return oboe::Result::ErrorBase;
        }

        result = _playbackStream->requestStart();
        if (result != oboe::Result::OK) {
            std::cerr << "Failed to start playback stream. Error: "
                      << oboe::convertToText(result) << std::endl;
            return oboe::Result::ErrorBase;
        }

        _isPlaying = true;
        return oboe::Result::OK;
    }

    void stopPlayback() {
        if (_playbackStream) {
            _playbackStream->requestStop();
            _playbackStream->close();
            _playbackStream.reset();
        }
        _isPlaying = false;
    }

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream* stream,
            void* audioData,
            int32_t numFrames) override {
        AudioData* audio_data = AudioData::get_audio_data();

        float* floatData = static_cast<float*>(audioData);
        std::lock_guard<std::mutex> lock(_dataMutex);

        if (not audio_data || audio_data->sound.empty()) {
            return oboe::DataCallbackResult::Stop;
        }
        if(audio_data and not audio_data->sound.empty()){//} and _isPlaying) {
            __android_log_print(ANDROID_LOG_INFO, "OboeAudioPlayer",
                "Playback index: %zu, Sound size: %zu", _playbackIndex, audio_data->sound.size());

            if (_playbackIndex < audio_data->sound.size()) {
                int32_t framesToCopy =
                        std::min(numFrames, static_cast<int32_t>(audio_data->sound.size() -
                                _playbackIndex));
                std::copy(audio_data->sound.begin() + _playbackIndex,
                        audio_data->sound.begin() + _playbackIndex + framesToCopy,
                        floatData);
                _playbackIndex += framesToCopy;

                if (_playbackIndex >= audio_data->sound.size()) {
                    _isPlaying = false;
                    return oboe::DataCallbackResult::Stop;
                }
                return oboe::DataCallbackResult::Continue;

            } else {
                std::fill(floatData, floatData + numFrames, 0.0f);
                _isPlaying = false;
                return oboe::DataCallbackResult::Stop;
            }
        }
        return oboe::DataCallbackResult::Stop;
    }

private:
    std::shared_ptr<oboe::AudioStream> _playbackStream;
    size_t _playbackIndex = 0;
    bool _isPlaying;
    std::mutex _dataMutex;
};
#endif //_PLAYER_HPP
