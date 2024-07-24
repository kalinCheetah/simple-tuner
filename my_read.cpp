#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <kiss_fft.h>
#include <sndfile.h>

using std::cout;
using std::endl;

#if 0
// приводить к float и делить на 100 при использовании
enum Notes {
     C1 = 3270, C1d = 3465, D1 = 3695, D1d = 3888, E1 = 4121, F1 = 4365, 
     F1d = 4625, G1 = 4900, G1d = 5190, A1 = 5500, A1d = 5826, B1 = 6174,
     C2 = 6541, C2d = 6930, D2 = 7391, D2d = 7778, E2 = 8241, F2 = 8731, 
     F2d = 9250, G2 = 9800, G2d = 10380, A2 = 11000, A2d = 11654, B2 = 12348,
     C3 = 13082, C3d = 13859, D3 = 14783, D3d = 15556, E3 = 16481, F3 = 17462, 
     F3d = 18500, G3 = 19600, G3d = 20700, A3 = 22000, A3d = 23308, B3 = 24696,
     C4 = 26163, C4d = 27718, D4 = 29366, D4d = 31113, E4 = 32963, F4 = 34923, 
     F4d = 36999, G4 = 39200, G4d = 41530, A4 = 44000, A4d = 46616, B4 = 49388,
     C5 = 52325, C5d = 55436, D5 = 58732, D5d = 62226, E5 = 65926, F5 = 69846, 
     F5d = 73998, G5 = 78400, G5d = 83060, A5 = 88000, A5d = 93232, B5 = 98775,
};
#endif

int main()
{
    SF_INFO sfinfo;
    SNDFILE* infile = sf_open("foo.wav", SFM_READ, &sfinfo);
    if (!infile) {
        std::cerr << "Error opening file" << sf_strerror(NULL) << endl;
    }
    
    auto& sampleRate = sfinfo.samplerate;    // short pseudonims
    auto& frameSize = sfinfo.frames;

    std::vector<double> frames(frameSize * sfinfo.channels);
    sf_count_t numFrames = sf_readf_double(infile, frames.data(), frameSize);
    
    if (numFrames < 1) {
        std::cerr << "Error reading file!";
        sf_close(infile);
        return 1;
    } else {
        cout << "Num frames = " << numFrames << endl;
    }

    kiss_fft_cfg cfg = kiss_fft_alloc(frameSize, 0, NULL, NULL);
    if (!cfg) {
        std::cerr << "Error alloc kiss_cfg" << endl;
        sf_close(infile);
        return 1;
    }
    
    std::vector<kiss_fft_cpx> samples(frameSize);
    std::vector<kiss_fft_cpx> spectrum(frameSize);
    for (size_t i = 0; i < frameSize; ++i) {
        samples[i].r = frames[i];
        samples[i].i = 0;
    }
    kiss_fft(cfg, samples.data(), spectrum.data());
    double frequency;
    double amplitude;
    for (size_t i = 0; i < frameSize / 2 + 1; ++i) {
        frequency = i * sampleRate / frameSize;
        amplitude = 2 * std::sqrt(spectrum[i].r * spectrum[i].r + spectrum[i].i * spectrum[i].i) / frameSize;
        if (amplitude > 0.01) {
            cout << "Freq = " <<  frequency << " Hz, " << "Amplitude = " << amplitude << endl;
        }
    }

    kiss_fft_free(cfg);
    sf_close(infile);

    return 0;
}
