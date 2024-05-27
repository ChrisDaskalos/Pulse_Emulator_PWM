#include "PulseGenerator.h"
#include <cstdio>
#include <cstring>
#include <cctype>

// Define the output pin for the pulses
DigitalOut pulsePin(D2);  // You can change D2 to any suitable pin

// Define a pin for LED debugging
DigitalOut debugLed(LED1);

PulseGenerator::PulseGenerator() : pulseThread(osPriorityNormal, 8 * 1024) {
    pulseThread.start(callback(this, &PulseGenerator::generate_pulses));
}

PulseGenerator::~PulseGenerator() {
    pulseThread.terminate();
}

void PulseGenerator::generate_pulses() {
    while (true) {
        ThisThread::flags_wait_all(0x1);  // Wait for a flag to start pulse generation
        auto interval = currentConfig.total_period / currentConfig.num_pulses;
        uint32_t generated_pulses = 0;

        printf("GENERATING %u PULSES WITH INTERVAL %lld MS\n", currentConfig.num_pulses, interval.count());

        for (uint32_t i = 0; i < currentConfig.num_pulses; ++i) {
            pulsePin = 1;
            printf("1\n");
            ThisThread::sleep_for(interval / 2);
            pulsePin = 0;
            printf("0\n");
            ThisThread::sleep_for(interval / 2);

            printf("PULSE %u: HIGH FOR %lld MS, LOW FOR %lld MS\n", i + 1, (interval / 2).count(), (interval / 2).count());
            generated_pulses++;
        }
        printf("PULSE GENERATION COMPLETE\n");

        if (generated_pulses != currentConfig.num_pulses) {
            printf("PULSE GENERATION ERROR: MISMATCH IN PULSE COUNT\n");
        } else {
            printf("PULSE GENERATION VERIFIED: %u PULSES GENERATED SUCCESSFULLY\n", currentConfig.num_pulses);
        }
    }
}

bool PulseGenerator::parse_input(const char *input, PulseConfig &config, LedConfig &ledConfig) {
    uint32_t pulses;
    uint32_t period;
    uint32_t ledBlinkMs;

    if (sscanf(input, "PULSES,%u,%u,%u", &pulses, &period, &ledBlinkMs) == 3) {
        if ((pulses > 0U) && (period >= (pulses * 2U)) && (ledBlinkMs > 0U)) {
            config.num_pulses = pulses;
            config.total_period = std::chrono::milliseconds(period);
            ledConfig.blink_interval = std::chrono::milliseconds(ledBlinkMs);
            ledConfig.total_duration = std::chrono::milliseconds(period);
            ledConfig.update = true;
            return true;
        }
    }
    return false;
}

bool PulseGenerator::sanitize_input(const char *input) {
    for (size_t i = 0; i < strlen(input); ++i) {
        if (!isalnum(input[i]) && input[i] != ',' && input[i] != '\n' && input[i] != '\r') {
            return false;
        }
    }
    return true;
}

void PulseGenerator::handle_serial_input(BufferedSerial &serial) {
    char input_buffer[SERIAL_BUFFER_SIZE];

    if (serial.readable()) {
        memset(input_buffer, 0, sizeof(input_buffer));
        serial.read(input_buffer, sizeof(input_buffer) - 1);

        if (sanitize_input(input_buffer)) {
            PulseConfig config;
            if (parse_input(input_buffer, config, ledConfig)) {
                printf("CONFIGURING: %u PULSES OVER %lld MILLISECONDS, LED BLINK INTERVAL: %lld MS\n", 
                       config.num_pulses, config.total_period.count(), ledConfig.blink_interval.count());

                currentConfig = config;  // Update current configuration
                pulseThread.flags_set(0x1);  // Set flag to start pulse generation
            } else {
                printf("INVALID INPUT FORMAT. USE: PULSES,<NUM_PULSES>,<TOTAL_PERIOD_MS>,<LED_BLINK_INTERVAL_MS>\n");
                printf("ENSURE THAT TOTAL_PERIOD >= NUM_PULSES * 2 MS\n");
            }
        } else {
            printf("INVALID CHARACTERS IN INPUT\n");
        }
    }
}

void PulseGenerator::blink_led() {
    while (true) {
        ledConfigMutex.lock();
        if (ledConfig.update) {
            ledConfig.update = false;
            auto blink_interval = ledConfig.blink_interval;
            auto total_duration = ledConfig.total_duration;
            uint32_t num_blinks = total_duration.count() / blink_interval.count();
            ledConfigMutex.unlock();

            printf("BLINKING LED FOR %lld MS WITH INTERVAL %lld MS\n", total_duration.count(), blink_interval.count());

            for (uint32_t i = 0; i < num_blinks; ++i) {
                debugLed = !debugLed;
                ThisThread::sleep_for(blink_interval);
            }

            printf("LED BLINKING COMPLETE\n");

            debugLed = 0;
        } else {
            ledConfigMutex.unlock();
            ThisThread::sleep_for(LED_THREAD_SLEEP_INTERVAL);
        }
    }
}
