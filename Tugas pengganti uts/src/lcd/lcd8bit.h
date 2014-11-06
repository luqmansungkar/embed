#ifndef LCD8BIT_H_
#define LCD8BIT_H_

#include <stdint.h>
#include <stdio.h>

#include "lcd8bit_conf.h"

void lcd_init();

void lcd_inst_write(uint8_t data);
void print_string(char *data, uint8_t home);
void set_cursor_left(uint8_t pos);
void set_cursor_right(uint8_t pos);

uint8_t lcd_inst_read();
uint8_t range_calib(uint8_t data);

void lcd_data_write(uint8_t data);
uint8_t lcd_data_read();

#define lcd_clear()                 lcd_inst_write(0b00000001)
#define lcd_return_home()           lcd_inst_write(0b00000010)
#define lcd_set_ddram_addr(addr)    lcd_inst_write(0b10000000 | addr)
#define lcd_shift_right             lcd_inst_write(0b00011100)
#define lcd_shift_left              lcd_inst_write(0b00011000)

void lcd_display_mode(uint8_t display_on, uint8_t cursor_on, uint8_t blink_on);
void lcd_cursor_mode(uint8_t inc, uint8_t shift);

FILE *lcd_file_out();

#endif /* LCD8BIT_H_ */
