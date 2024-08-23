#ifndef SOUND_DATA_HPP
#define SOUND_DATA_HPP

#include <cstdint>
#include <algorithm>

struct SoundData {
    double* frequencies{nullptr};
    std::int32_t frequency_size{0};

public:
    SoundData(){}
    SoundData(int size) {
        frequency_size = size;
        frequencies = new double[size];
    }

    SoundData(const SoundData& oth) {
        if (oth.frequencies) {
            frequencies = new double[oth.frequency_size];
            std::copy(oth.frequencies, oth.frequencies + oth.frequency_size, frequencies);
        }
    }

    SoundData(SoundData&& oth) {
        std::swap(frequencies, oth.frequencies);
        std::swap(frequency_size, oth.frequency_size);
    }

    SoundData& operator=(const SoundData& oth) {
        if (this == &oth) {
            return *this;
        }
        if (frequencies) {
            delete[] frequencies;
            frequencies = nullptr;
        }

        if (oth.frequencies) {
            if (!frequencies) {
                frequencies = new double[oth.frequency_size];
            }
            std::copy(oth.frequencies, oth.frequencies + oth.frequency_size, frequencies);
        }

        frequency_size = oth.frequency_size;
        return *this;
    }

    SoundData& operator=(SoundData&& oth) {
        if (this == &oth) {
            return *this;
        }
        std::swap(frequencies, oth.frequencies);
        std::swap(frequency_size, oth.frequency_size);
        return *this;
    }

    ~SoundData() {
        if (frequencies) {
            delete[] frequencies;
            frequencies = nullptr;
        }
        frequency_size = 0;
    }

    void set_frequency_size(std::int32_t size) {
        if (not frequencies) {
            frequencies = new double[size];
        } else if (frequency_size != size) {
            delete frequencies;
            frequencies = new double [frequency_size];
        }
        frequency_size = size;
    }

    void set_frequency(std::int32_t index, double f) {
        if (frequencies) {
            if (index < frequency_size) {
                frequencies[index] = f;
            }
        }
    }

    double get_frequency(std::int32_t index) {
        if(frequencies) {
            if (index < frequency_size) {
                return frequencies[index];
            }
        }
        return -1;
    }
};

#endif //SOUND_DATA_HPP