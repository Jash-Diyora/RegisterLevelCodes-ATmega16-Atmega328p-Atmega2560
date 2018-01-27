#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
unsigned short pulse = 0;
unsigned short  i = 0;


void lcdcommand (unsigned char jash)
{
    PORTC = 0xF0 & jash;
    PORTC &=~(1<<PIN2);   // RS0
    PORTC |= (1<<PIN3);   // EN1
    _delay_ms(8);
    PORTC &=~ (1<<PIN3);  // EN0
    jash = jash<<4;
    PORTC = 0xF0 & jash;  // LOWER BITS
    PORTC &=~ (1<<PIN2);   // RS0
    PORTC |= (1<<PIN3);   // EN1
    _delay_ms(8);
    PORTC &=~ (1<<PIN3);  // EN0
}

void lcdclear()
{
    lcdcommand(0x01);
}


void lcddata (unsigned char data)
{
    PORTC = 0xF0 & data;
    PORTC |= (1<<PIN2);    //RS
    PORTC |= (1<<PIN3);    //EN1
    _delay_ms(8);
    PORTC &=~ (1<<PIN3);   //EN0
    data = data<<4;
    PORTC = 0xF0 & data;
    PORTC |= (1<<PIN2);   //RS1
    PORTC |= (1<<PIN3);   //EN1
    _delay_ms(8);
    PORTC &=~ (1<<PIN3);  //EN0
    
}

void lcdinstall ()
{
    DDRC |= 0xFF;
    lcdcommand(0x02);
    lcdcommand(0x28);
    lcdcommand(0X0C);
    lcdcommand(0x06);
    lcdcommand(0x01);
    lcdcommand(0x80);
    _delay_us(200);
}

void lcdxy(unsigned char x , unsigned char y)
{
    unsigned char jashline[] = {0x80,0xC0,0x90,0xD0};
    lcdcommand(jashline[x-1] + y-1);
    _delay_ms(10);
}

void lcddisplaystring(const char *label)
{
    while(*label!='\0')
    {
        lcddata(*label);
        label++;
    }
}


void runtime()
{
    lcdinstall();
    lcdclear();
    lcdxy(1,1);
    lcddisplaystring(" TEAM UVPCE 5.0 ");
    lcdxy(2,1);
    lcddisplaystring("  ROBOCON 2017  ");
    lcdxy(3,1);
    lcddisplaystring("PROGRAMMED BY - ");
    lcdxy(4,1);
    lcddisplaystring("");
    _delay_ms(1000);
    lcdclear();
    
    lcdxy(1,1);
    lcddisplaystring(" TEAM UVPCE 5.0 ");
    lcdxy(2,1);
    lcddisplaystring("  ROBOCON 2017  ");
    lcdxy(3,1);
    lcddisplaystring("ULTRASONIC  TEST");
    lcdxy(4,1);
    lcddisplaystring("DISTANCE CM.:   ");
    
}


int main(void)
{
    runtime();
    

    DDRC = 0xFF;
    DDRD = 0b11111011;
    GICR|=(1<<INT0);
    MCUCR|=(1<<ISC00);
    
    TCCR1A = 0;
    uint16_t count = 0;
    char str[5]={0,0,0,0,0};
    
    sei();
    
    while(1)
    {
        PORTD|=(1<<PIND0);
        _delay_ms(5);
        PORTD &=~(1<<PIND0);
       // count=(pulse*17013.0);
        count = (pulse/50);
        itoa(count,str,10);
        lcdxy(4,14);
        lcddisplaystring(str);
    }
}

ISR(INT0_vect)
{
    if (i==1)
    {
        TCCR1B=0;
        pulse=TCNT1;
        TCNT1=0;
        i=0;
    }
    
    if (i==0)
    {
        TCCR1B|=(1<<CS10);
        i=1;
    }
}
