#ifndef PULSE_GENERATOR_H
#define PULSE_GENERATOR_H

#include "mbed.h"

constexpr std::chrono::milliseconds DEFAULT_LED_BLINK_INTERVAL_MS = 500ms;
constexpr size_t SERIAL_BUFFER_SIZE = 32;
constexpr std::chrono::seconds MAIN_LOOP_DELAY = 10s;
constexpr std::chrono::milliseconds LED_THREAD_SLEEP_INTERVAL = 100ms;

struct PulseConfig 
{
    uint32_t num_pulses;
    std::chrono::milliseconds total_period;
};

struct LedConfig 
{
    std::chrono::milliseconds blink_interval;
    std::chrono::milliseconds total_duration;
    bool update;
};

class PulseGenerator 
{
    public:
        PulseGenerator();
        ~PulseGenerator();

        void blink_led();
        void handle_serial_input(BufferedSerial &serial);
        void generate_pulses();

        static constexpr std::chrono::seconds MAIN_LOOP_DELAY = 10s;

    private:
        bool parse_input(const char *input, PulseConfig &config, LedConfig &ledConfig);
        bool sanitize_input(const char *input);

        PulseConfig currentConfig;
        LedConfig ledConfig;
        Mutex ledConfigMutex;
        Thread pulseThread;
};

#endif // PULSE_GENERATOR_H
