#include "sk_guitar_tuner.hpp"
#include "sound_data.hpp"
#include "audio_recorder.hpp"
#include <android/log.h>

#include <thread>
#include <vector>
#include <fstream>

#define SAVE_SOUND_TO_FILE 1

static const char * NAME_LOG = "sk_guitar_tuner";

extern "C"
{

FFI_PLUGIN_EXPORT SoundData* allocate_sound_data() {
    __android_log_print(ANDROID_LOG_INFO, "Audio", "\033[1;46m  Allocate sound data \033[0m");

    SoundData* sound_data = new SoundData;
    return sound_data;
}

FFI_PLUGIN_EXPORT SoundData* allocate_sound_data_with_size(std::int32_t size) {
    __android_log_print(ANDROID_LOG_INFO, "Audio", "\033[1;46m  Allocate sound data with size \033[0m");

    SoundData* sound_data = new SoundData(size);
    return sound_data;
}

FFI_PLUGIN_EXPORT void free_sound_data(SoundData* sound_data) {
    __android_log_print(ANDROID_LOG_INFO, "Audio", "\033[1;42m  Free sound data \033[0m");

    delete sound_data;
    sound_data = nullptr;
}

FFI_PLUGIN_EXPORT void start_audio_recorder(SoundData* sound_data, char* path) {
    __android_log_print(ANDROID_LOG_INFO, "Audio", "\033[1;93m  Start record audio %s\033[0m", path);
    if (sound_data == nullptr) {
        __android_log_print(ANDROID_LOG_INFO, "Audio", "\033[1;93m  Sound data is null\033[0m");
        return;
    }

    try {
        AudioRecorder audioRecorder;
            if (audioRecorder.startRecording()) {
                // Wait for seconds of recording
                std::this_thread::sleep_for(std::chrono::milliseconds(RECORDER_TIME_MS));
            } else {
                __android_log_print(ANDROID_LOG_INFO, "Audio", "\033[1;93m  Failed to start recording\033[0m");
                return;
            }
            audioRecorder.stopRecording();

#if SAVE_SOUND_TO_FILE
            if(path != nullptr) {
                auto sound = audioRecorder.get_recorded_sound();
                std::string all_path = std::string(path) + "/";// + std::to_string(milliseconds);
                std::string filename = all_path + "record.wav";

                __android_log_print(ANDROID_LOG_INFO, "Audio", "\033[1;93m  Save record audio %s\033[0m", filename.c_str());

                std::ofstream outFile(filename, std::ios::binary);

                // Write WAV header
                outFile.write("RIFF", 4);
                int fileSize = 36 + sound.size() * sizeof(float);
                outFile.write(reinterpret_cast<const char*>(&fileSize), 4);
                outFile.write("WAVE", 4);
                outFile.write("fmt ", 4);
                int fmtChunkSize = 16;
                outFile.write(reinterpret_cast<const char*>(&fmtChunkSize), 4);
                short audioFormat = 3; // IEEE float
                outFile.write(reinterpret_cast<const char*>(&audioFormat), 2);
                short numChannels = 1; // Mono
                outFile.write(reinterpret_cast<const char*>(&numChannels), 2);
                outFile.write(reinterpret_cast<const char*>(&SAMPLE_RATE), 4);
                int byteRate = SAMPLE_RATE * numChannels * sizeof(float);
                outFile.write(reinterpret_cast<const char*>(&byteRate), 4);
                short blockAlign = numChannels * sizeof(float);
                outFile.write(reinterpret_cast<const char*>(&blockAlign), 2);
                short bitsPerSample = 32;
                outFile.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
                outFile.write("data", 4);
                int dataChunkSize = sound.size() * sizeof(float);
                outFile.write(reinterpret_cast<const char*>(&dataChunkSize), 4);

                // Write audio data
                outFile.write(reinterpret_cast<const char*>(sound.data()), dataChunkSize);
                outFile.close();
            }
#endif
            __android_log_print(ANDROID_LOG_INFO, NAME_LOG, "\033[1;43m Stop record data: %d\033[0m", audioRecorder.get_recorded_sound().size());

            audioRecorder.clear();
        }
    catch (const std::exception& e) {
        __android_log_print(ANDROID_LOG_INFO, NAME_LOG,
                "\033[1;33mWRITE thread iexception: %s \033[0m", e.what());
    }
    catch (...) {
        __android_log_print(ANDROID_LOG_INFO, NAME_LOG, "\033[1;33mWRITE thread iexception\033[0m");
    }
    __android_log_print(ANDROID_LOG_INFO, NAME_LOG, "\033[1;33mWRITE thread is stopping\033[0m");
}

FFI_PLUGIN_EXPORT void stop_audio_recorder() {
   __android_log_print(ANDROID_LOG_INFO, "Audio", "\033[1;95m  Stop record audio \033[0m");
}

}
