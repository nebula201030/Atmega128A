#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

//  PC7 PC6 PC5 PC4 PC3 PC2 PC1 PC0
//  DB7 DB6 DB5 DB4          E   RW  RS

void COMMAND(unsigned char byte);
void DATA(unsigned char byte);
void LCD_INIT(void);
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

	PORTC=byte&0xf0;// 상위4비트 명령어 쓰기
	PORTC&=0b11111100;  // RS = 0, RW = 0, 명령어를 쓰도록 설정
	_delay_us(1);
	PORTC|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTC&=0b11111011;// E = 0

	PORTC=(byte<<4)&0xf0;// 하위4비트 명령어 쓰기
	PORTC&=0b11111100;// RS = 0, RW = 0, 명령어를 쓰도록 설정
	_delay_us(1);
	PORTC|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTC&=0b11111011;// E = 0
}
void DATA(unsigned char byte)
{
	_delay_ms(2);

	PORTC=byte&0xf0;// 상위4비트 명령어 쓰기
	PORTC |= 0b00000001;// RS = 1, 데이터 설정
	PORTC &= 0b11111101;// RW = 0, 쓰기 설정
	_delay_us(1);
	PORTC|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTC&=0b11111011;// E = 0

	PORTC=(byte<<4)&0xf0;// 하위4비트 데이터 쓰기
	PORTC |= 0b00000001;// RS = 1, 데이터 설정
	PORTC &= 0b11111101;// RW = 0, 쓰기 설정
	_delay_us(1);
	PORTC|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTC&=0b11111011;// E = 0
}
void MOVE(int y, int x)     //커서 이동
{
	unsigned char data;
	if(y==1) 	data=0x80+x-1;           //1행
	else     	data=0xc0+x-1;            //2행
	COMMAND(data);
}
void STRING(unsigned char font[], unsigned char n)
{
	unsigned char i;
	for(i=0;i<n;i++)
	{
		DATA(font[i]);
	}
}
int main(void)
{	
	unsigned char string_M[]="COUNT";
	unsigned char count[]="0000";
	DDRC = 0xff;
	PORTC = 0x00;
	int one=0;
	int ten=0;
	int hun=0;
	int tho=0;
	LCD_INIT();
	
	MOVE (1,7);
	STRING(string_M,5);
	MOVE (2,8);
	STRING(count,4);

	while(1)
	{
		MOVE(2,8);
		DATA(tho+0x30);
		MOVE(2,9);
		DATA(hun+0x30);
		MOVE(2,10);
		DATA(ten+0x30);
		MOVE(2,11);
		DATA(one+0x30);
		_delay_ms(100);
		
		one++;
		if(one==10) one=0,ten++;
		if(ten==10) ten=0, hun++;
		if(hun==10) hun=0, tho++; 
		1
	}
}