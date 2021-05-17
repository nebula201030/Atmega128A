#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	unsigned char fnd[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};
	unsigned char digit, fnd_position=0x01;
	DDRC=0xFF;
	DDRG=0x0F;
	while(1)
	{
		for(digit=0;digit<10;digit++)
		{
			PORTG=fnd_position;
			fnd_position<<=1;
			
			if(fnd_position==0x10)
			fnd_position=0x01;
			
			PORTC=fnd[digit];
			_delay_ms(1000);
		}
	}
	return 0;
}