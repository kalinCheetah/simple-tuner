#include "sk_guitar_tuner.hpp"
#include "sound_data.hpp"

#include <android/log.h>

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

FFI_PLUGIN_EXPORT void start_audio_recorder(SoundData* sound_data) {
    __android_log_print(ANDROID_LOG_INFO, "Audio", "\033[1;93m  Start record audio \033[0m");
}

FFI_PLUGIN_EXPORT void stop_audio_recorder() {
   __android_log_print(ANDROID_LOG_INFO, "Audio", "\033[1;95m  Stop record audio \033[0m");
}

}
