#include <avr/io.h>
#include <util/delay.h>

int main(){
	DDRA = 0xff;
	uint8_t lampu = 0b11111010;
	uint8_t flag = 0b1;
	while(1){
		PORTA = lampu;
		_delay_ms(200);
		if (flag)
		{
			if (lampu == 0b01011111)
			{
				flag = 0b0;
			}else{
				lampu = (lampu << 1) | 1;
			}	
		}else{
			if (lampu == 0b11111010)
			{
				flag = 0b1;
			}else{
				lampu = (lampu >> 1) | 0b10000000;
			}
		}
		
	}
}