#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC=0xFF;
	DDRG=0x0F;
	PORTG=0x0f;
	PORTC=0xff;
	unsigned char what[11]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x7f};
	unsigned int count=0;
	unsigned char thou=0,hund=0,ten=0,one=0;
	int i;
	while (1)
	{
		if((PINE&0x10) == 0x00)
		{
			while(1){
				thou=count/1000;
				hund=count/100%10;
				ten=count/10%10;
				one=count%10;
				
				for(i=0;i<10;i++)
				{
					PORTG=0x01;
					PORTC=what[thou];
					_delay_ms(1);
					
					PORTG=0x02;
					PORTC=what[hund];
					_delay_ms(1);

					PORTG=0x04;
					PORTC=what[ten];
					_delay_ms(1);

					PORTG=0x08;
					PORTC=what[one];
					_delay_ms(1);

				}

				count++;
				if(count==9999)
				{
					break;
				}
				
				if((PINE&0x20) == 0x00)
				{
					break;
				}
			}
		}
		
		else if((PINE&0x20) == 0x00){
			while(1){
				thou=count/1000;
				hund=count/100%10;
				ten=count/10%10;
				one=count%10;
				
				for(i=10;i>0;i--)
				{
					PORTG=0x01;
					PORTC=what[thou];
					_delay_ms(1);
					
					PORTG=0x02;
					PORTC=what[hund];
					_delay_ms(1);

					PORTG=0x04;
					PORTC=what[ten];
					_delay_ms(1);

					PORTG=0x08;
					PORTC=what[one];
					_delay_ms(1);

				}

				count--;
				if(count==0)
				{
					break;
				}
			}
			
		}
		
	}
}