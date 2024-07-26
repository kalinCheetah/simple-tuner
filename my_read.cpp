#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cmath>
#include <kiss_fft.h>
#include <sndfile.h>

using std::cout;
using std::endl;

std::list<std::pair<std::string, double>> note_freq = {
    {"C0", 16.35}, {"C#0", 17.32}, {"D0", 18.35}, {"D#0", 19.45}, {"E0", 20.60}, {"F0", 21.83},
    {"F#0", 23.12}, {"G0", 24.50}, {"G#0", 25.96}, {"A0", 27.50}, {"A#0", 29.14}, {"B0", 30.87},
    {"C1", 32.70}, {"C#1", 34.65}, {"D1", 36.71}, {"D#1", 38.89}, {"E1", 41.20}, {"F1", 43.65},
    {"F#1", 46.25}, {"G1", 49.00}, {"G#1", 51.91},  {"A1", 55.00}, {"A#1", 58.27}, {"B1", 61.74},
    {"C2", 65.41}, {"C#2", 69.30}, {"D2", 73.42}, {"D#2", 77.78}, {"E2", 82.41}, {"F2", 87.31},
    {"F#2", 92.50}, {"G2", 98.00}, {"G#2", 103.83}, {"A2", 110.00}, {"A#2", 116.54}, {"B2", 123.47},
    {"C3", 130.81}, {"C#3", 138.59}, {"D3", 146.83}, {"D#3", 155.56}, {"E3", 164.81}, {"F3", 174.61},
    {"F#3", 185.00}, {"G3", 196.00}, {"G#3", 207.65}, {"A3", 220.00}, {"A#3", 233.08}, {"B3", 246.94},
    {"C4", 261.63}, {"C#4", 277.18}, {"D4", 293.66}, {"D#4", 311.13}, {"E4", 329.63}, {"F4", 349.23},
    {"F#4", 369.99}, {"G4", 392.00}, {"G#4", 415.30}, {"A4", 440.00}, {"A#4", 466.16}, {"B4", 493.88},
    {"C5", 523.25}, {"C#5", 554.37}, {"D5", 587.33}, {"D#5", 622.25}, {"E5", 659.26}, {"F5", 698.46},
    {"F#5", 739.99}, {"G5", 783.99}, {"G#5", 830.61}, {"A5", 880.00}, {"A#5", 932.33}, {"B5", 987.77},
    {"C6", 1046.50}, {"C#6", 1108.73}, {"D6", 1174.66}, {"D#6", 1244.51}, {"E6", 1318.51}, {"F6", 1396.91},
    {"F#6", 1479.98}, {"G6", 1567.98}, {"G#6", 1661.22}, {"A6", 1760.00}, {"A#6", 1864.66}, {"B6", 1975.53},
    {"C7", 2093.00}, {"C#7", 2217.46}, {"D7", 2349.32}, {"D#7", 2489.02}, {"E7", 2637.02}, {"F7", 2793.83},
    {"F#7", 2959.96}, {"G7", 3135.96}, {"G#7", 3322.44}, {"A7", 3520.00}, {"A#7", 3729.31}, {"B7", 3951.07},
    {"C8", 4186.01}, {"C#8", 4434.92}, {"D8", 4698.64}, {"D#8", 4978.03}, {"E8", 5274.04}, {"F8", 5587.65},
    {"F#8", 5919.91}, {"G8", 6271.93}, {"G#8", 6644.88}, {"A8", 7040.00}, {"A#8", 7458.62}, {"B8", 7902.13},
};

/* function prototypes */
void getSpectrum(std::vector<double>& freqs);
double identifyNoteRange(std::vector<double> freqs);
std::pair<std::string, double> identifyNearestNote(double averagePitch);
void setArrowDirectionAndPosition(double pitch, std::pair<std::string, double> nearestNote);

int main()
{
    std::vector<double> frequencies;
    getSpectrum(frequencies);    

    double averagePitch = identifyNoteRange(frequencies);

    std::pair<std::string, double> nearestNote = identifyNearestNote(averagePitch);

    cout << "Nearest note is " << nearestNote.first << ", " << nearestNote.second << " Hz"<< endl;

    setArrowDirectionAndPosition(averagePitch, nearestNote);
    

    return 0;
}

void setArrowDirectionAndPosition(double pitch, std::pair<std::string, double> nearestNote)
{
    std::string direction = pitch > nearestNote.second ? "more" : "less";
    double position;
    if (direction == "more") {
        auto it = note_freq.begin();
        while (it->second != nearestNote.second) {
            ++it;
        }
        position = std::abs(pitch - nearestNote.second) / std::abs(nearestNote.second - (++it)->second);
    } else {                                                                 // different ^
        auto it = note_freq.begin();
        while (it->second != nearestNote.second) {
            ++it;
        }
        position = std::abs(pitch - nearestNote.second) / std::abs(nearestNote.second - (--it)->second);
    }                                                                        // different ^
    cout << "Arrow direction: " << direction << endl << "To tune = " << position << endl; 
    
}

/* get only target Hz meteings and average this */
double identifyNoteRange(std::vector<double> freqs)
{
    int sumHz = 0;
    int countMetering = 0;
    for (size_t i = 0, size = freqs.size(); i != size; ++i) {
        cout << "Freq = " << freqs[i] << " Hz" << endl;
        if (std::abs(freqs[i] - freqs[i+1]) < 3) {   // 3 это погрешность между ближайшими частотами
            sumHz += freqs[i];
            ++countMetering;
        } else {
            break;
        }
    }
    double averagePitch = sumHz / countMetering;
    cout << "Current pitch = " << averagePitch << " Hz" << endl;

    return averagePitch;
}

/* Returns item of global list */
std::pair<std::string, double> identifyNearestNote(double averagePitch)
{
    std::pair<std::string, double> nearestNote;
    // search of minimum algo 
    double minDiff = 9000;  // just big number
    for (auto it = note_freq.begin(), end = note_freq.end(); it != end; ++it) {
        if (std::abs(it->second - averagePitch) < minDiff) {
            minDiff = std::abs(it->second - averagePitch);
            nearestNote = *it;
        }
    }

    return nearestNote;
}
    
void getSpectrum(std::vector<double>& freqs)
{
    SF_INFO sfinfo;
    SNDFILE* infile = sf_open("audio/3new.wav", SFM_READ, &sfinfo);
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
        return;
    } else {
        cout << "Num frames = " << numFrames << endl;
    }

    kiss_fft_cfg cfg = kiss_fft_alloc(frameSize, 0, NULL, NULL);
    if (!cfg) {
        std::cerr << "Error alloc kiss_cfg" << endl;
        sf_close(infile);
        return;
    }
    
    std::vector<kiss_fft_cpx> samples(frameSize);
    std::vector<kiss_fft_cpx> spectrum(frameSize);
    for (size_t i = 0; i < frameSize; ++i) {
        samples[i].r = frames[i];
        samples[i].i = 0;
    }
    kiss_fft(cfg, samples.data(), spectrum.data());

    for (size_t i = 0; i < frameSize / 2 + 1; ++i) {
        double frequency = i * sampleRate / frameSize;
        double amplitude = 2 * std::sqrt(spectrum[i].r * spectrum[i].r + spectrum[i].i * spectrum[i].i) / frameSize;
        if (amplitude > 0.01) {
            freqs.push_back(frequency); 
            //cout << "Freq = " << frequency << " Hz, Amplitude = " << amplitude << endl;
        }
    }

    kiss_fft_free(cfg);
    sf_close(infile);
    
}
