#define SWITCH_PORT PORTB
#define SWITCH0_BIT PB0
#define SENSE_PORTC PINB
#define DEBOUNCE_TIME 20

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "i2c_master.h"
#include "lcd8bit.h"

#define ALAMAT_SENSOR       0xE0
#define MAX_RANGE_CM        150
#define MAX_RANGE_REGISTER  ((((MAX_RANGE_CM * 10) - 85) / 43) + 1)

volatile int16_t counter = 0;

typedef struct{
	uint16_t waktuJalan;
	uint8_t timeout;
	void (*fungsi)();
}struk;

void srf();
void led();
void tombol();
void reset();

struk hola[] = {
		{0,65,srf},
		{0,50,led},
		{0,50,tombol}
	};

uint8_t lampu = 0b11111110;
uint8_t flag = 0b1;

uint8_t srfState = 0;
uint8_t ledState = 0;
uint8_t masukanState = 0;

uint16_t last_value;

uint8_t tombolTertekan;
uint16_t password = 1234;
uint16_t masukan = 0; 

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

int main(){

	DDRA = 0xff;
	PORTA = 0b11111111;

	SWITCH_PORT |= _BV(SWITCH0_BIT);

	TCCR0 |= (1 << WGM01);

	TIMSK |= (1 << OCIE0);

	sei();

	OCR0 = 187;

	TCCR0 |= (1 << CS01) | (1 << CS00);

	lcd_init();
    lcd_clear();

    
    i2c_init();
    

    i2c_transmit(ALAMAT_SENSOR, 2, MAX_RANGE_REGISTER);
    print_string("SELAMAT DATANG",1);
    _delay_ms(1000);
    lcd_clear();
    print_string("MASUKKANPASSWORD",1);
    _delay_ms(1000);
    lcd_clear();
    lcd_init();

    lcd_set_ddram_addr(0x0);
    print_string("PASSWORD",1);
    lcd_set_ddram_addr(0x42);
    print_string("____",0);
    set_cursor_left(4);

    uint8_t haha = 1;
	while(haha){
		for (int i = 0; i < sizeof(hola)/sizeof(hola[0]); i++)
		{
			if (counter>=hola[i].waktuJalan)
			{
				hola[i].fungsi();
				hola[i].waktuJalan = counter+hola[i].timeout;
			}
		}
		if (counter >= 15000)
		{
			hola[1].timeout = 30;
		}else if(counter >= 25000){
			hola[1].timeout = 5;
		}

		if (masukanState == 4 || counter >= 30000 )
		{
			lcd_clear();
			if (counter >= 30000)
			{
				print_string("WAKTU",1);
				lcd_set_ddram_addr(0x40);
		    	print_string("HABIS",0);
				PORTA = 0b00000000;
		    	haha = 0;
			}
			else if (masukan / 10 == password)
			{
				print_string("AKSES",1);
				lcd_set_ddram_addr(0x40);
		    	print_string("DITERIMA",0);
		    	PORTA = 0b00000000;
		    	haha = 0;
			}else{
				print_string("PASSWORD",1);
				lcd_set_ddram_addr(0x40);
		    	print_string("SALAH",0);
		    	reset();
				_delay_ms(1000);
				lcd_clear();
				lcd_set_ddram_addr(0x0);
			    print_string("PASSWORD",1);
			    lcd_set_ddram_addr(0x42);
			    print_string("____",0);
			    set_cursor_left(4);
			}
		}
	}
	TCCR0 &= 0b00000000;
	while(1){

	}


	
}

void reset(){
	masukanState = 0;
	masukan = 0;
	for (int i = 0; i < sizeof(hola)/sizeof(hola[0]); i++){
		hola[i].waktuJalan = 0;
	}
	counter = 0;
}
ISR(TIMER0_COMP_vect){
	//PORTA = ~PORTA;
	if (OCR0 == 186)
	{
		OCR0 = 187;
	}else if (OCR0 == 187)
	{
		OCR0 = 186;
	}
	counter++;
}

uint8_t range_calib(uint8_t range){
    if (range < 30) return range/3;
    return 9;
}

void srf(){
	switch(srfState){
		case 0:
			i2c_transmit(ALAMAT_SENSOR, 0, 81);
			srfState++;
			break;
		case 1:
			last_value = (i2c_read(ALAMAT_SENSOR, 2) << 8) | (i2c_read(ALAMAT_SENSOR, 3) << 0);
			srfState++;
			break;
		default:
			print_string(itoa(range_calib(last_value),""),0);
			srfState = 0;
			set_cursor_left(1);
			break;
	}
	return;
}

void led(){
	if (ledState == 1)
	{
		PORTA = lampu;

		if (flag)
		{
			if (lampu == 0b01111111)
			{
				flag = 0b0;
				lampu = (lampu >> 1) | 0b10000000;
			}else{
				lampu = (lampu << 1) | 1;
			}	
		}else{
			if (lampu == 0b11111110)
			{
				flag = 0b1;
				lampu = (lampu << 1) | 1;
			}else{
				lampu = (lampu >> 1) | 0b10000000;
			}
		}

		ledState = 0;
	}else{
		ledState++;
	}
	
	return;

}

void tombol(){
	uint8_t tombolDitekan = bit_is_clear(SENSE_PORTC, SWITCH0_BIT);
	if (!tombolTertekan && tombolDitekan)
	{
		masukan = (masukan + range_calib(last_value)) * 10;
		set_cursor_right(1);
		masukanState++;
	}
	tombolTertekan = tombolDitekan;
	return; //nope
}