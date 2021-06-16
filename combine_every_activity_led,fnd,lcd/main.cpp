#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void COMMAND(unsigned char byte);
void DATA(unsigned char byte);
void LCD_INIT(void);

void STRING(unsigned char font[], unsigned char n)
{
	unsigned char i;
	for(i=0;i<n;i++)
	{
		DATA(font[i]);
	}
}
void MOVE(int y, int x)     //커서 이동
{
	unsigned char data;
	if(y==1) 	data=0x80+x-1;           //1행
	else     	data=0xc0+x-1;            //2행
	COMMAND(data);
}
void LCD_INIT(void)
{
	_delay_ms(30);	// 전원 투입 후 30ms 이상 지연
	
	//Function set
	COMMAND(0b00101000);
	// 인터페이스(DL)=0(4bit), 라인(N)=1(2라인), 폰트(F)=0(5*8 dot)
	_delay_us(39);	// 39us 이상 지연

	//Display ON/OFF Control
	COMMAND(0b00001100);
	// 화면 표시(D)=1(on), 커서(C)=0(off), 블링크(B)=0(off)
	_delay_us(39);	// 39us 이상 지연

	//Clear Display
	COMMAND(0b00000001);
	// 화면을 클리어하고 , 커서가 홈위치인 0번지로 돌아감.
	_delay_ms(1.53);	// 1.53ms 이상 지연

	//Entry Mode Set
	COMMAND(0b00000110);
	// 커서방향(I/D)=1(address증가), 표시이동(S)=0(이동하지 않음)
}
void COMMAND(unsigned char byte)
{
	_delay_ms(2);

	PORTD=byte&0xf0;// 상위4비트 명령어 쓰기
	PORTD&=0b11111100;  // RS = 0, RW = 0, 명령어를 쓰도록 설정
	_delay_us(1);
	PORTD|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTD&=0b11111011;// E = 0

	PORTD=(byte<<4)&0xf0;// 하위4비트 명령어 쓰기
	PORTD&=0b11111100;// RS = 0, RW = 0, 명령어를 쓰도록 설정
	_delay_us(1);
	PORTD|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTD&=0b11111011;// E = 0
}
void DATA(unsigned char byte)
{
	_delay_ms(2);

	PORTD=byte&0xf0;// 상위4비트 명령어 쓰기
	PORTD |= 0b00000001;// RS = 1, 데이터 설정
	PORTD &= 0b11111101;// RW = 0, 쓰기 설정
	_delay_us(1);
	PORTD|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTD&=0b11111011;// E = 0

	PORTD=(byte<<4)&0xf0;// 하위4비트 데이터 쓰기
	PORTD |= 0b00000001;// RS = 1, 데이터 설정
	PORTD &= 0b11111101;// RW = 0, 쓰기 설정
	_delay_us(1);
	PORTD|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTD&=0b11111011;// E = 0
}
int main(void)
{
	unsigned int SEC=0,MIN=0,HOUR=0;
	
	unsigned char Title[]="TIME";
	unsigned char Time[]="00:00:00";
	DDRA = 0xff;
	PORTA = 0x00;
	
	DDRG = 0xff;
	PORTG = 0x00;
	DDRC = 0xff;
	PORTC = 0x00;
	
	DDRD = 0xff;
	PORTD = 0x00;
	
	unsigned char what [7] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82};
	unsigned char thou = 0, hund = 0, ten = 0, one = 0;
	
	thou = MIN/10, hund = MIN % 10, ten = SEC/10, one = SEC;
	
	LCD_INIT();
	
	MOVE(1,7);
	STRING(Title,4);
	MOVE(2,5);
	STRING(Time,8);
		while(1)
		{	
			PORTA = 0x00;
			_delay_ms(125);
			PORTA = 0x01;
			_delay_ms(125);
			PORTA = 0x03;
			_delay_ms(125);
			PORTA = 0x07;
			_delay_ms(125);
			PORTA= 0x0f;
			_delay_ms(125);
			PORTA = 0x1f;
			_delay_ms(125);
			PORTA = 0x3F;
			_delay_ms(125);
			PORTA = 0x7F;
			_delay_ms(125);
			PORTA = 0xFF;

;
			
			MOVE(2,5);
			DATA(HOUR/10+0x30);		//시의 10자리
			DATA(HOUR%10+0x30);		//시의 1자리
			MOVE(2,8);
			DATA(MIN/10+0x30);		//분의 10자리
			DATA(MIN%10+0x30);		//분의 1자리
			MOVE(2,11);
			DATA(SEC/10+0x30);		//초의 10자리
			DATA(SEC%10+0x30);		//초의 1자리
			
			for(int k=0;k<10;k++)
			{
			PORTG = 0x01;
			PORTC = what[thou];
			_delay_ms(1);
			PORTG = 0x02;
			PORTC = what[hund];
			_delay_ms(1);
			PORTG = 0x04;
			PORTC = what[ten];
			_delay_ms(1);
			PORTG = 0x08;
			PORTC = what[one];
			_delay_ms(1);	
			}
			
						
			_delay_ms(1000);

			SEC++;
			if(SEC==60)	SEC=0, MIN++;
			if(MIN==60) MIN=0, HOUR++;
			if(HOUR==24) HOUR=0;
		}
		
	}

