#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC=0xFF;
	DDRG=0x0F;
	PORTC=0xFF;
	while(1)
	{
		PORTC=0x00;
		PORTG=0x08;
		_delay_ms(5);
		PORTC=0x00;
		PORTG=0x04;
		_delay_ms(5);
		PORTC=0x00;
		PORTG=0x02;
		_delay_ms(5);
		PORTC=0x00;
		PORTG=0x01;
		_delay_ms(5);
	}
}