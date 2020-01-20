#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdint.h>
#include "oled.h"
#include "fonts_codetables.h"

void oled_write_cmd(unsigned char cmd)
{
    wiringPiI2CWriteReg8(OLED_I2C_ADDRESS, 0x00, cmd);
}

void oled_write_data(unsigned char data)
{
    wiringPiI2CWriteReg8(OLED_I2C_ADDRESS, 0x40, data);
}

void i2c_start_addr_writebytes_const(uint8_t hw_i2c_adr, uint8_t character, uint16_t nbr)
{
    int i;

    for(i=0;i<nbr;i++)
        wiringPiI2CWriteReg8(hw_i2c_adr, 0x40, character);
}

void i2c_start_addr_writebytes(uint8_t hw_i2c_adr, uint8_t *data_array, uint16_t nbr)
{
    int i;

    for(i=0;i<nbr;i++)
        wiringPiI2CWriteReg8(hw_i2c_adr, 0x40, data_array[i]);
}

void oled_init(int fd)
{
    uint8_t i;

    unsigned char initSequence[26] = {0x00,0xAE,0xA8,0x3F,0xD3,0x00,0x40,0xA1,0xC8,0xDA,0x12,0x81,0x7F,
                                      0xA4,0xA6,0xD5,0x80,0x8D,0x14,0xD9,0x22,0xD8,0x30,0x20,0x00,0xAF};
    unsigned char setFullRange[7]  = {0x00,0x21,0x00,0x7F,0x22,0x00,0x07};
    
    unsigned char letterA[5] = {0x40,0x7E,0x12,0x12,0x7E};
    
    delay(200);

    for(i=0;i<26;i++)
        oled_write_cmd(initSequence[i]);

    for(i=0;i<7;i++)
        oled_write_cmd(setFullRange[i]);

    for(i=0;i<5;i++)
        oled_write_cmd(letterA[i]);
    
//	oled_write_cmd(0xAE);   /* Display off */
//	oled_write_cmd(0x20);   /* Set Memory Addressing Mode	*/
//	oled_write_cmd(0x01);   /* 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid */
//	oled_write_cmd(0xb0);   /* Set Page Start Address for Page Addressing Mode,0-7 */
//	oled_write_cmd(0xc8);   /* Set COM Output Scan Direction */
//	oled_write_cmd(0x00);   /* Set low column address */
//	oled_write_cmd(0x10);   /* Set high column address */
//	oled_write_cmd(0x40);   /* Set start line address */
//	oled_write_cmd(0x81);   /* Set contrast control register */
//	oled_write_cmd(0xff);   /* 0x00~0xff */
//	oled_write_cmd(0xa1);   /* Set segment re-map 0 to 127 */
//	oled_write_cmd(0xa6);   /* Set normal display */
//	oled_write_cmd(0xa8);   /* Set multiplex ratio(1 to 64) */
//	oled_write_cmd(0x3F);   /* TBD */
//	oled_write_cmd(0xa4);   /* 0xa4,Output follows RAM content;0xa5,Output ignores RAM content */
//	oled_write_cmd(0xd3);   /* Set display offset */
//	oled_write_cmd(0x00);   /* Non offset */
//	oled_write_cmd(0xd5);   /* Set display clock divide ratio/oscillator frequency */
//	oled_write_cmd(0xf0);   /* Set divide ratio */
//	oled_write_cmd(0xd9);   /* Set pre-charge period */
//	oled_write_cmd(0x22);   /* TBD */
//	oled_write_cmd(0xda);   /* Set com pins hardware configuration */
//	oled_write_cmd(0x12);   /* TBD */
//	oled_write_cmd(0xdb);   /* Set vcomh */
//	oled_write_cmd(0x20);   /* 0x20,0.77xVcc */
//	oled_write_cmd(0x8d);   /* Set DC-DC enable */
//	oled_write_cmd(0x14);   /* TBD */
//    oled_clear_screen();    /* Clear all screen surface */
//	oled_write_cmd(0xaf);   /* Turn on oled panel */
}

void oled_set_position(unsigned char x, unsigned char y)
{ 
	oled_write_cmd(0xb0+y);
	oled_write_cmd(((x&0xf0)>>4)|0x10);
	oled_write_cmd((x&0x0f)|0x01);
}

void oled_clear_screen(void)
{
	unsigned char m;

	for(m=0;m<8;m++){
		oled_write_cmd(0xb0+m);       /* page0-page7 */
		oled_write_cmd(0x00);         /* low column start address */
		oled_write_cmd(0x10);         /* high column start address */
        i2c_start_addr_writebytes_const(OLED_I2C_ADDRESS, 0x00, 128);
	}
}

void oled_clear_one_row(unsigned char row_idx)
{
    oled_write_cmd(0xb0+row_idx);     /* page0-page7 */
    oled_write_cmd(0x00);             /* low column start address */
    oled_write_cmd(0x10);             /* high column start address */
    i2c_start_addr_writebytes_const(OLED_I2C_ADDRESS, 0x00, 128);
}

void oled_on(void)
{
	oled_write_cmd(0x8d);   /* Set DC-DC enable */
	oled_write_cmd(0x14);   /* TBD */
	oled_write_cmd(0xaf);   /* Turn on oled panel */
}

void oled_off(void)
{
	oled_write_cmd(0x8d);   /* Set DC-DC enable */
	oled_write_cmd(0x10);   /* 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid */
	oled_write_cmd(0xae);   /* Turn off oled panel */
}

void oled_show_string(uint8_t x, uint8_t y, char *ch, uint8_t text_size)
{
	char c = 0;
    uint16_t i = 0, j = 0;
    uint8_t tmparr[64];
    
	switch(text_size){
		case 1:{
			while(ch[j] != '\0'){
                c = (char)(*(ch+j))-32;
				if(x > 126){
					x = 0;
					y++;
				}

				for(i=0;i<6;i++)
                    tmparr[i] = F6x8[c][i];
                
                oled_set_position(x,y);
                i2c_start_addr_writebytes(OLED_I2C_ADDRESS, tmparr, 6);
                
				x += 6;
				j++;
			}
            break;
		}
		case 2:{
			while(ch[j] != '\0'){
				c = (char)(*(ch+j))-32;
				if(x > 120){
					x = 0;
					y++;
				}

				for(i=0;i<8;i++){
                    tmparr[i] = F8X16[(uint16_t)(((uint16_t)c)*16)+i];
                    tmparr[i+8] = F8X16[(uint16_t)(((uint16_t)c)*16)+i+8];
                }
				
                oled_set_position(x,y);                
                i2c_start_addr_writebytes(OLED_I2C_ADDRESS, tmparr, 8);
				oled_set_position(x,y+1);
                i2c_start_addr_writebytes(OLED_I2C_ADDRESS, tmparr+8, 8);

				x += 8;
				j++;
			}
            break;
		}
        case 3:{
			while(ch[j] != '\0'){
				c = (char)(*(ch+j))-48;
				if(x > 120){
					x = 0;
					y++;
				}

				for(i=0;i<16;i++){
                    tmparr[i]    = F16X32[(uint16_t)(((uint16_t)c)*64)+(i*4)];
                    tmparr[i+16] = F16X32[(uint16_t)(((uint16_t)c)*64)+(((i*4)+1))];
                    tmparr[i+32] = F16X32[(uint16_t)(((uint16_t)c)*64)+(((i*4)+2))];
                    tmparr[i+48] = F16X32[(uint16_t)(((uint16_t)c)*64)+(((i*4)+3))];
                }
				
                oled_set_position(x,y);                
                i2c_start_addr_writebytes(OLED_I2C_ADDRESS, tmparr, 16);
				oled_set_position(x,y+1);
                i2c_start_addr_writebytes(OLED_I2C_ADDRESS, tmparr+16, 16);
                oled_set_position(x,y+2);
                i2c_start_addr_writebytes(OLED_I2C_ADDRESS, tmparr+32, 16);
                oled_set_position(x,y+3);
                i2c_start_addr_writebytes(OLED_I2C_ADDRESS, tmparr+48, 16);

				x += 16;
				j++;
			}
            break;
		}
        default:{
            break;
        }
	}
}

void oled_draw_bmp(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[])
{
    uint16_t j=0;
    uint8_t x,y;

    if(y1%8==0)
        y = y1/8;
    else
        y = y1/8 + 1;
	
    for(y=y0;y<y1;y++){
        oled_set_position(x0,y);
        for(x=x0;x<x1;x++){
			oled_write_data(bmp[j++]);
		}
	}
}
