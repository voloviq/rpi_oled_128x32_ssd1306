#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include <stdint.h>

#define OLED_I2C_ADDRESS    0x3C

void oled_write_cmd(unsigned char cmd);
void oled_write_data(unsigned char data);
void oled_init(int fd);
void oled_set_position(unsigned char x, unsigned char y);
void oled_clear_screen(void);
void oled_clear_one_row(unsigned char row_idx);
void oled_on(void);
void oled_off(void);
void oled_show_string(uint8_t x, uint8_t y, char *ch, uint8_t text_size);
void oled_draw_bmp(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[]);

#endif
