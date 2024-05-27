#include "PulseGenerator.h"
#include "mbed.h"

// Define serial communication
BufferedSerial serial(USBTX, USBRX, 9600);

int main() {
    PulseGenerator pulseGenerator;  // Create instance of PulseGenerator

    printf("PULSE GENERATOR INITIALIZED\n");
    printf("ADD PULSE CONFIGURATION IN FORMAT: PULSES,<NUM_PULSES>,<TOTAL_PERIOD_MS>,<LED_BLINK_INTERVAL_MS>\n");

    // Create a thread for LED blinking
    Thread ledThread(osPriorityLow);
    ledThread.start(callback(&pulseGenerator, &PulseGenerator::blink_led));

    // Main loop handles serial input
    while (true) {
        pulseGenerator.handle_serial_input(serial);
        ThisThread::sleep_for(PulseGenerator::MAIN_LOOP_DELAY);
    }
}
