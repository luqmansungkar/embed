#include <avr/io.h>
#include <avr/interrupt.h>

volatile int16_t count = 10;
volatile uint16_t nilai = 10;

int main(){
	DDRA = 0xff;

	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);

	MCUCR |= (1 << ISC01) | (1 << ISC00);
	GICR |= (1 << INT0);

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

ISR(TIMER0_COMP_vect){
	//PORTA = ~PORTA;
	count--;
}

ISR(INT0_vect){
	nilai += 100;
	if (nilai >=1010)
	{
		nilai = 10;
	}
}
