//  Author - Jash Diyora
//  jashdiyora7@gmail.com
//  Jr. Programmer
//  TEAM UVPCE 5.0


#define F_CPU 8000000UL
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define enter PINA
#define exit PINA>>1

char str[5]={0,0,0,0,0};
char str1[5]={0,0,0,0,0};


void lcdcommand(unsigned char cmnd)
{
    PORTC = 0xF0 & cmnd;
    
    PORTC &=~(1<<PIN2);
    PORTC |= (1<<PIN3);
    _delay_ms(4);
    PORTC &=~ (1<<PIN3);
    cmnd = cmnd<<4;
    PORTC = 0xF0 & cmnd;
    
    PORTC &=~ (1<<PIN2);
    PORTC |= (1<<PIN3);
    _delay_ms(4);
    PORTC &=~ (1<<PIN3);
}



void lcddata(unsigned char data)
{
    PORTC = 0xF0 & data;
    
    PORTC |= (1<<PIN2);
    PORTC |= (1<<PIN3);
    _delay_ms(4);
    PORTC &=~ (1<<PIN3);
    
    data = data<<4;
    PORTC = 0xF0 & data;
    
    PORTC |= (1<<PIN2);
    PORTC |= (1<<PIN3);
    _delay_ms(4);
    PORTC &=~ (1<<PIN3);
}



void lcdinit(void)
{
    DDRC |= 0xFF;
    lcdcommand(0x02);   //return home
    lcdcommand(0x28);     //5x7 4 bit mode
    lcdcommand(0x0E);     //display on cursor off
    lcdcommand(0x06);    //increment cursor
    lcdcommand(0x01);       //clear display
    lcdcommand(0x80);       //force cursor to begin from 1st line
}	



void lcdclear()
{
    lcdcommand(0x01);
}


void lcdline(unsigned char x,unsigned char y)
{
    unsigned char linestart[] = {0x80,0xC0,0x94,0xD4}; //90 D0 FOR 16X4
    lcdcommand(linestart[x-1]+y-1);
    _delay_ms(1);
}


void lcddisplaystring(const char *label)
{
    while(*label!='\0')
    {
        lcddata(*label);
        label++;
    }
}	 	






void entryled()
{
    PORTB=0b00011000;
    _delay_ms(50);
    PORTB=0x00;
}


void exitled()
{
    PORTB=0b10000001;
    _delay_ms(50);
    PORTB=0x00;
}

void print()
{
    lcddisplaystring("          ");
    lcdline(4,7);
    lcddisplaystring(str);
    return 0;
}


//----------
int main(void)
{
    unsigned short  visitorvalue=0;
    unsigned short a=0;
    unsigned short b=0;
    DDRC=0xFF;
    DDRA=0x00;
    
    lcdinit();
    lcdclear();
    lcdline(1,1);
    lcddisplaystring("INGIT BHAI TASK ");
    lcdline(2,1);
    lcddisplaystring("BY- JASH DIYORA ");
    lcdline(3,1);
    lcddisplaystring("NIK CHAL SUBWAY.");
    lcdline(4,1);
    lcddisplaystring("AFTER THIS TASK ");
    _delay_ms(1500);
    lcdclear();
    
    lcdline(1,1);
    lcddisplaystring(" TEAM UVPCE 5.0 ");
    lcdline(2,1);
    lcddisplaystring(" LAB  VISITOR'S ");
    
    while (1)
    {
        if((enter&0b1)==0)
        {
            entryled();
            visitorvalue++;
            itoa(visitorvalue,str,10);
            print();
        }
        
        else if((exit&0b1)==0)
        {
            _delay_ms(200);
            a=1;
        }
        
        else if((exit&0b1)==0 && (a=1))
        {
             b=1;
            exitled();
            visitorvalue--;
            itoa(visitorvalue,str,10);
            print();
            a=0;
        }
        
       else if((enter&0b1)==0 && (b=1))
        {
            visitorvalue=visitorvalue;
            itoa(visitorvalue,str,10);
            print();
            b=0;
        }

        else
        {
            visitorvalue=visitorvalue;
            itoa(visitorvalue,str,10);
            print();
        }
        
    }
    
}


