#include <avr/io.h>
#include <util/delay.h>

#include "i2c_master.h"
#include "lcd8bit.h"

#define ALAMAT_SENSOR       0xE0
#define MAX_RANGE_CM        150
#define MAX_RANGE_REGISTER  ((((MAX_RANGE_CM * 10) - 85) / 43) + 1)

uint8_t lamp;
uint8_t flag;

char* itoa(uint16_t i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

void time_up(){

    _delay_ms(1000);
    PORTA = lamp;
    DDRA = lamp;    
    // Turn LEFT
    if (flag) {
        if (lamp & 0b10000000) {
            lamp = (lamp << 1) | 0b00000001;
        }
        else {
            flag = 0b0;
            lamp = (lamp >> 1) | 0b10000000;
        }

        // Turn RIGHT
        } else {
        if (lamp & 0b00000001) {
            lamp = (lamp >> 1) | 0b10000000;
        }
        else {
            flag = 0b1;
            lamp = (lamp << 1) | 0b00000001;
        }
    }
}


int main() {
    lcd_init();
    lcd_clear();

    
    i2c_init();
    uint16_t last_value;

    i2c_transmit(ALAMAT_SENSOR, 2, MAX_RANGE_REGISTER);

    print_string("SELAMAT DATANG",1);
    _delay_ms(1000);
    lcd_clear();
    print_string("MASUKKANPASSWORD",1);
    _delay_ms(1000);
    uint16_t count = 0;
    lcd_clear();
    lcd_init();
    
    while (1) {
        i2c_transmit(ALAMAT_SENSOR, 0, 81);
        _delay_ms(65); 
        lcd_set_ddram_addr(0x0);
        print_string("PASSWORD",1);
        lcd_set_ddram_addr(0x42);
        print_string("____",0);
        set_cursor_left(4);
        //print_string("P",0);
        //lcd_set_ddram_addr(0x42);
        last_value = (i2c_read(ALAMAT_SENSOR, 2) << 8) | (i2c_read(ALAMAT_SENSOR, 3) << 0);    
        print_string(itoa(range_calib(last_value),""),0);
        //set_cursor_left();
        //DDRA = _BV(last_value % 8);
        //time_up();
        
    }

    return 0;
    
}

int mains(void){
    DDRA = 0b11111111;

    uint8_t lamp = 0b11111110;
    uint8_t flag = 0b1;
    uint8_t counter = 0;

    while(1){
        _delay_ms(200);
        PORTA = lamp;

        
        // Turn LEFT
        if (flag) {
            if (lamp & 0b10000000) {
                lamp = (lamp << 1) | 0b00000001;
            }
            else {
                flag = 0b0;
                lamp = (lamp >> 1) | 0b10000000;
            }

            // Turn RIGHT
            } else {
            if (lamp & 0b00000001) {
                lamp = (lamp >> 1) | 0b10000000;
            }
            else {
                flag = 0b1;
                lamp = (lamp << 1) | 0b00000001;
            }
        }
    }
}


uint8_t range_calib(uint8_t range){
    if (range < 30) return range/3;
    return 9;
}

/*void sukses(){
    print_string("AKSES   DITERIMA");
}

void gagal(){
    print_string("PASSWORDSALAH");
}*/




// SET CURSOR           = 00001110
// GESER CURSOR KANAN   = 00010100 
// GESER CURSOR KIRI    = 00010000