#include "Arduino.h"
#include "CAN.h"
#include "mcp2515.h"
#include <SPI.h>

spi_bus_config_t bus_config;
int PIN_NUM_CLK = 18;
int PIN_NUM_MOSI = 23;
int PIN_NUM_MISO = 19;
int PIN_NUM_CS = 5;

spi_device_handle_t spi;


//The setup function is called once at startup of the sketch
void setup()
{
	// Add your initialization code here

	Serial.begin(9600);
	struct can_frame canMsg;

	// Define the SPI bus configuration
	bus_config.sclk_io_num = PIN_NUM_CLK; // change this to your actual CLK pin
	bus_config.mosi_io_num = PIN_NUM_MOSI; // change this to your actual MOSI pin
	bus_config.miso_io_num = PIN_NUM_MISO; // change this to your actual MISO pin
	bus_config.quadwp_io_num = -1;
	bus_config.quadhd_io_num = -1;
	bus_config.max_transfer_sz = 4096;

	// Initialize the SPI bus
	esp_err_t ret = spi_bus_initialize(HSPI_HOST, &bus_config, 1);
	assert(ret == ESP_OK);

	// Define the SPI device configuration
	spi_device_interface_config_t dev_config;
	dev_config.address_bits = 0;
	dev_config.command_bits = 0;
	dev_config.dummy_bits = 0;
	dev_config.mode = 0;
	dev_config.duty_cycle_pos = 128;
	dev_config.cs_ena_pretrans = 0;
	dev_config.cs_ena_posttrans = 0;
	dev_config.clock_speed_hz = 10000000;
	dev_config.spics_io_num = PIN_NUM_CS; // change this to your actual CS pin
	dev_config.flags = 0;
	dev_config.queue_size = 1;
	dev_config.pre_cb = NULL;
	dev_config.post_cb = NULL;

	// Add the device to the bus
	ret = spi_bus_add_device(HSPI_HOST, &dev_config, &spi);
	assert(ret == ESP_OK);

	MCP2515  mcp2515(&spi);

	mcp2515.reset();
	mcp2515.setNormalMode();

	mcp2515.setBitrate(CAN_125KBPS);
	mcp2515.setLoopbackMode();

	struct can_frame frame;
	frame.can_id = 0x000;
	frame.can_dlc = 4;
	frame.data[0] = 0xFF;
	frame.data[1] = 0xFF;
	frame.data[2] = 0xFF;
	frame.data[3] = 0xFF;
	frame.data[4] = 0xFF;
	frame.data[5] = 0xFF;
	frame.data[6] = 0xFF;
	frame.data[7] = 0xFF;

	/* send out the message to the bus and
	tell other devices this is a standard frame from 0x00. */
	mcp2515.sendMessage(&frame);
	Serial.println("Message sent!");
}

// The loop function is called in an endless loop

int i = 0;
void loop()
{
	MCP2515  mcp2515(&spi);

	mcp2515.reset();
	mcp2515.setNormalMode();

	mcp2515.setBitrate(CAN_125KBPS);
	mcp2515.setLoopbackMode();

	struct can_frame frame;
	frame.can_id = 0x000;
	frame.can_dlc = 4;
	frame.data[0] = 0xFF;
	frame.data[1] = 0xFF;
	frame.data[2] = 0xFF;
	frame.data[3] = 0xFF;
	frame.data[4] = 0xFF;
	frame.data[5] = 0xFF;
	frame.data[6] = 0xFF;
	frame.data[7] = 0xFF;

	/* send out the message to the bus and
		tell other devices this is a standard frame from 0x00. */

	mcp2515.sendMessage(&frame);
	Serial.print("Message sent:"); Serial.print(i++); Serial.print(" Error code: "); Serial.println(mcp2515.sendMessage(&frame));

	delay(2000);
}
