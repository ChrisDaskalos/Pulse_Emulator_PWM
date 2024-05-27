# Pulse Generator and LED Blinker

This project is an mbed-based application that generates digital pulses and blinks an LED based on user input via a serial interface. The user can specify the number of pulses, the total duration over which to generate these pulses, and the interval for blinking an LED.

## Table of Contents
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Dependencies](#dependencies)
- [Code Overview](#code-overview)
- [Debugging](#debugging)
- [License](#license)

## Project Structure
-  PulseGenerator/
                ├── main.cpp
                ├── PulseGenerator.cpp
                ├── PulseGenerator.h
                └── README.md


## Getting Started

### Prerequisites
- Mbed OS compatible hardware (e.g., Nucleo, FRDM, etc.)
- [Mbed CLI](https://os.mbed.com/docs/mbed-os/v6.15/build-tools/mbed-cli.html)
- Serial terminal software (e.g., PuTTY, Tera Term)

### Setup
1. **Clone the repository:**
    ```sh
    git clone https://github.com/ChrisDaskalos/Pulse_Emulator_PWM
    cd PulseGenerator
    ```

2. **Compile the project:**
    ```sh
    mbed compile -t <toolchain> -m <target>
    ```
    Replace `<toolchain>` with your desired toolchain (e.g., GCC_ARM) and `<target>` with your target board (e.g., NUCLEO_F446RE).

3. **Flash the binary to your board:**
    ```sh
    mbed flash -t <toolchain> -m <target>
    ```

4. **Open a serial terminal:**
    - Configure the serial port to 9600 baud rate.

## Usage

### Input Format
To configure the pulse generator and LED blinker, use the following format in the serial terminal:
PULSES,<num_pulses>,<total_period_ms>,<led_blink_interval_ms>

- `<num_pulses>`: Number of pulses to generate (e.g., 200)
- `<total_period_ms>`: Total duration in milliseconds to generate pulses (e.g., 10000)
- `<led_blink_interval_ms>`: Interval in milliseconds for the LED to blink (e.g., 500)

### Example
PULSES,200,10000,500

This command will generate 200 pulses over 10000 ms and blink the LED every 500 ms for the duration.

## Dependencies
- Mbed OS
- Serial library

## Code Overview

### `main.cpp`
This file initializes the `PulseGenerator` class and sets up the main loop to handle serial input.

### `PulseGenerator.h`
Header file for the `PulseGenerator` class. It contains class definitions, method declarations, and necessary includes.

### `PulseGenerator.cpp`
Implementation file for the `PulseGenerator` class. It contains the logic for generating pulses and blinking the LED based on user input.

#### Key Methods:
- **`PulseGenerator()`**: Constructor to initialize the pulse generation thread.
- **`~PulseGenerator()`**: Destructor to clean up the pulse generation thread.
- **`generate_pulses()`**: Generates digital pulses based on the current configuration.
- **`parse_input(const char *input, PulseConfig &config, LedConfig &ledConfig)`**: Parses user input to configure the pulse and LED settings.
- **`sanitize_input(const char *input)`**: Sanitizes user input to ensure it is in the correct format.
- **`handle_serial_input(BufferedSerial &serial)`**: Handles serial input from the user.
- **`blink_led()`**: Blinks the LED based on the specified interval and duration.

## Debugging

### Debug Messages
- The code contains several `printf` statements that output debug information to the serial terminal.
- To monitor the debug messages, ensure your serial terminal is connected and configured to the correct baud rate (9600).

### Common Issues
1. **LED stops blinking**: Ensure the `total_duration` and `blink_interval` are set correctly and the values are within reasonable ranges.
2. **Invalid input format**: Make sure the input follows the `PULSES,<num_pulses>,<total_period_ms>,<led_blink_interval_ms>` format.

## License
This project is licensed under the Apache-2.0 License. See the [LICENSE](LICENSE) file for details.
