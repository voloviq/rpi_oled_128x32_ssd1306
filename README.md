# rpi_oled_128x32_ssd1306

Check minimal configuration of OLED display with 128x32 size based on SSD1306

Hardware configuration

![Hardware_Configuration](rpi_oled_128x32_ssd1306_bb.png)

### One wire temperature device setup

It is necessary to aligh into a code following line

fd = open("/sys/bus/w1/devices/28-051700dadeff/w1_slave", O_RDONLY);

which depends on connected one wire device and serial number

### I2C bus check

First thing to do is to check if oled display is present on I2C buf

issue following command into a terminal

i2cdetect -y 0

if i2cdetect is not installed issue

sudo apt-get install -y i2c-tools

If only oled display is connected to I2C bus answer should look like

0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f <br>
00:          -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
30: -- -- -- -- -- -- -- -- -- -- -- -- 3c -- -- -- <br>
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
70: -- -- -- -- -- -- -- -- <br>
