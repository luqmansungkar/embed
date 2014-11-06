#include "lcd8bit.h"
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#define send()                  LCD_CONT_PORT |=  (1 << LCD_CONT_EN);   \
                                _delay_us(0.5);                         \
                                LCD_CONT_PORT &= ~(1 << LCD_CONT_EN)

#define data()                  LCD_CONT_PORT |=  (1 << LCD_CONT_RS)

#define inst()                  LCD_CONT_PORT &= ~(1 << LCD_CONT_RS)

#define write()                 LCD_DATA_DDR = 0xFF;                    \
                                LCD_CONT_PORT &= ~(1 << LCD_CONT_RW)

#define read()                  LCD_DATA_DDR = 0x00;                    \
                                LCD_DATA_PORT = 0xFF;                   \
                                LCD_CONT_PORT |=  (1 << LCD_CONT_RW)

#define lcd_wait()              while(lcd_inst_read() & 0b10000000);


void print_string(char *data, uint8_t home){
    uint8_t i = 0;
    if (home) lcd_set_ddram_addr(0x0);
    while(i < strlen(data)){
        if (i == 8) lcd_set_ddram_addr(0x40);
        lcd_data_write(data[i++]);
    }
}

void lcd_display_mode(uint8_t display_on, uint8_t cursor_on, uint8_t blink_on) {
    uint8_t        tmp =  0b00001000;
    if(display_on) tmp |= 0b00000100;
    if(cursor_on)  tmp |= 0b00000010;
    if(blink_on)   tmp |= 0b00000001;
    lcd_inst_write(tmp);
}

void lcd_cursor_mode(uint8_t inc, uint8_t shift) {
    uint8_t   tmp =  0b00000100;
    if(inc)   tmp |= 0b00000010;
    if(shift) tmp |= 0b00000001;
    lcd_inst_write(tmp);
}

void lcd_init() {
    LCD_CONT_DDR |= (1 << LCD_CONT_RS) | (1 << LCD_CONT_RW) | (1 << LCD_CONT_EN);
    lcd_inst_write(0b00111000); // 8 bit, 2 line, 5x8 font
    lcd_cursor_mode(1, 0);
    lcd_display_mode(1, 1, 1);
}

void set_cursor_right(uint8_t pos){
    uint8_t inst = 0b00010100;
    uint8_t state = 0;
    while (state++ < pos) {
        lcd_inst_write(inst);
    }
}

void set_cursor_left(uint8_t pos){
    uint8_t inst = 0b00010000;
    uint8_t state = 0;
    while (state++ < pos) {
        lcd_inst_write(inst);
    }
}

void lcd_inst_write(uint8_t data) {
    lcd_wait();
    write(); inst();
    LCD_DATA_PORT = data;
    send();
}

uint8_t lcd_inst_read() {
    read(); inst();
    send();
    return LCD_DATA_PIN;
}

void lcd_data_write(uint8_t data) {
    lcd_wait();
    write(); data();
    LCD_DATA_PORT = data;
    send();

}

uint8_t lcd_data_read() {
    read(); data();
    send();
    return LCD_DATA_PIN;
}
