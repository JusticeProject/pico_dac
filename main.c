#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0

// MISO is unconnected since the DAC does not support that
#define PIN_MISO 16
// CS is pin 2 on the DAC, physical pin 22 on the Pico
#define PIN_CS   17
// SCK is pin 3 on the DAC, physical pin 24 on the Pico
#define PIN_SCK  18
// MOSI is pin 4 on the DAC, physical pin 25 on the Pico
#define PIN_MOSI 19

// other pins on the DAC:
// pin 1 is VDD and should be connected to 3.3V
// pin 5 is LDAC, active low, should be connected to GND
// pin 6 is VREF, should be connected to 3.3V
// pin 7 is VSS, should be connected to GND
// pin 8 is Vout, should be connected to low pass filter

// using unbuffered for the voltage reference amplififer, this allows Vref to range from 0 to VDD
#define DAC_COMMAND  0b0011000000000000

int main()
{
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 20MHz.
    spi_init(SPI_PORT, 20*1000*1000);
    spi_set_format(SPI_PORT, 16, 0, 0, SPI_MSB_FIRST);

    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    while (true)
    {
        // the analog output should be about 3.3 * value / 4096
        uint16_t value = 2048;
        uint16_t full_command = DAC_COMMAND | (value & 0xfff);
        spi_write16_blocking(SPI_PORT, &full_command, 1);

        printf("Sent %u %u\n", value, full_command);
        sleep_ms(1000);
    }
}
