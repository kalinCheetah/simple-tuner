#ifndef _SK_GUITAR_TUNER_HPP
#define _SK_GUITAR_TUNER_HPP

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

#if _WIN32
#define FFI_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FFI_PLUGIN_EXPORT
#endif

extern "C"
{
const int SAMPLE_RATE = 44100;
const int RECORDER_TIME_MS = 3000;
const int MC_IN_SEC = 1000;
}

#endif //_SK_GUITAR_TUNER_HPP