#include "sk_guitar_tuner.hpp"

extern "C"
{
FFI_PLUGIN_EXPORT int sum(int a, int b) { return a + b; }
FFI_PLUGIN_EXPORT int sum_long_running(int a, int b) {
    return a + b;
}
}
