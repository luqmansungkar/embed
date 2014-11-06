/**
* Tugas Praktikum 2 - Embbeded Systems
* 1. Irvi
* 2. Kandito
* 3. Luqman
* 4. Zaka
*/
#define SWITCH_PORT PORTB
#define SWITCH0_BIT PB0
#define SWITCH7_BIT PB7
#define SENSE_PORTC PINB
#define DEBOUNCE_TIME 20 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int16_t count = 100;
volatile uint16_t nilai = 100;

int switch1_is_pressed();
int switch7_is_pressed();

int main(){
	DDRA = 0xff;

	SWITCH_PORT |= _BV(SWITCH0_BIT) |  _BV(SWITCH7_BIT);

	TCCR0 |= (1 << WGM01);

	TIMSK |= (1 << OCIE0);

	sei();

	OCR0 = 187;

	TCCR0 |= (1 << CS01) | (1 << CS00);

	uint8_t lampu = 0b11111110;
	uint8_t flag = 0b1;
	while(1){
		

		if (count <= 0)
		{
			if (switch1_is_pressed())
			{
				if (nilai<500)
				{
					nilai += 100;
				}
			}
			if (switch7_is_pressed())
			{
				if (nilai>100)
				{
					nilai -= 100;
				}
			}

			count += nilai;
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
		}
		
	}
}

int switch1_is_pressed(){
	if (bit_is_clear(SENSE_PORTC, SWITCH0_BIT))
	{
		_delay_ms(DEBOUNCE_TIME);
		if (bit_is_clear(SENSE_PORTC, SWITCH0_BIT))
		{
			return 1;
		}
	}
	return 0;
}

int switch7_is_pressed(){
	if (bit_is_clear(SENSE_PORTC, SWITCH7_BIT))
	{
		_delay_ms(DEBOUNCE_TIME);
		if (bit_is_clear(SENSE_PORTC, SWITCH7_BIT))
		{
			return 1;
		}
	}
	return 0;
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
	count--;
}
