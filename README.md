# MMA8451 Accelerometer Driver API 

This project is an I2C-based driver API written for the MMA8451 Accelerometer in bare metal C. The API allows for the configuration of the sensor whilst in standby mode (8-bit fast mode data reading and full scale range value) and the reading of values for the X, Y, and Z axes (raw and converted).

# How to Set Up and Use

To set up and use the API, the user needs to clone the repository into their workspace and include "mma8451.h" in their working code.

Because the sensor can be wired in two ways (Pin Slave Address 0, a.k.a SA0, is either 3.3V or GND), please ensure that the "sa0" uint8_t variable at the top of "mma8451_helpers.c" is configured to either 0 or 1 to reflect the wiring setup.

Before calling any API code, please ensure that the "init_mma8451" function is called to initialize the sensor. 

# Standby Mode and Active Mode

The MMA8451 sensor has two different modes that it can be in, Standby Mode and Active Mode, which can be activated through the API. 

**Standby Mode**: the sensor is not producing readable values, but rather in state of having its settings be configured. The user can refer to the MMA8451 technical reference manual linked at the bottom for more information.

**Active Mode**: the sensor is working and producing readable values.

# UART Example Terminal Output Screenshots



# Functions

**void init_mma8451(void)**: initialize the sensor

**uint8_t mma8451_who_am_i(void)**: check if the sensor communication works, a successful check returns 0x1A

**void r_configure_ctrl_reg1(uint8_t)**: configure the control register 1 with 8 bit raw input, see technical reference manual for settings

**void r_configure_ctrl_reg2(uint8_t)**: configure the control register 2 with 8 bit raw input, see technical reference manual for settings

**void r_configure_ctrl_reg3(uint8_t)**: configure the control register 3 with 8 bit raw input, see technical reference manual for settings

**void r_configure_ctrl_reg4(uint8_t)**: configure the control register 4 with 8 bit raw input, see technical reference manual for settings

**void r_configure_ctrl_reg5(uint8_t)**: configure the control register 5 with 8 bit raw input, see technical reference manual for settings

**void r_configure_xyz_data_cfg(uint8_t)**: configure the high-pass filter output data and the full scale range (mainly used for full scale range)

**void configure_ctrl_reg1(5x uint8_t)**: simplified configuration of control register 1 with setting control through function parameters

**void configure_ctrl_reg2(5x uint8_t)**: simplified configuration of control register 2 with setting control through function parameters

**void configure_ctrl_reg3(5x uint8_t)**: simplified configuration of control register 3 with setting control through function parameters

**void configure_ctrl_reg4(5x uint8_t)**: simplified configuration of control register 4 with setting control through function parameters

**void configure_ctrl_reg5(5x uint8_t)**: simplified configuration of control register 5 with setting control through function parameters

**void configure_xyz_data_cfg(2x uint8_t)**: simplified configuration of xyz_data_cfg register with setting control through function parameters

**void software_reset(void)**: reset the sensor 

**void set_active_mode_only(void)**: set only the active mode without any other setting configurations

**void set_standby_mode_only(void)**: set only the standby mode without any other setting configurations

**int16_t read_accel(2x char)**: get the raw sensor value of chosen axis (x, y, or z) in either regular mode (14 bit) or fast mode (8 bit)

**void read_all_accel(int16_t\* [input array], char)**: fill a size 3 input data array of read values for all axes in either regular mode or fast mode

**float read_accel_converted(2x char, uint8_t)**: get the raw sensor value of a chosen axis in either regular mode or fast mode, must also specify the full scale range value set (default is 2)

**float read_accel_converted(float\* [input array], char, uint8_t)**: get the raw sensor value of all axes in either regular mode or fast mode and insert it into input array. The user must also specify the full scale range value set (default is 2)

# Links
Sensor that I Used (Adafruit Triple-Axis Accelerometer):

https://www.adafruit.com/product/2019?srsltid=AfmBOorL9qwDZYYb3YobpwR4gcqpDrOQlWoXR5EKw6VXGj5_gHzxG8W3

MMA8451 Technical Reference Manual:

https://www.nxp.com/docs/en/data-sheet/MMA8451Q.pdf

