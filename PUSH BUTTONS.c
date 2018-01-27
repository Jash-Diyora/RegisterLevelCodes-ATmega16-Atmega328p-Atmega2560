#include <avr/io.h>
#include <util/delay.h>
#define x PINA
#define y PINA>>1

int main(void)

{
    DDRC=0XFF;
    DDRB=0xFF;
    DDRA=0X00;
    PORTA=0XFF;
    
    while (1)
    {
        //-----------------------------------
        
        if ((x&0b1)==0)
        {
            PORTC=(0<<PINC0)|(1<<PINC1);
            PORTB=(0<<PINB0)|(1<<PINB1);
        }
        
         //-----------------------------------
        
        else if ((y&0b1)==0)
        {
            PORTC=(1<<PINC0)|(0<<PINC1);
            PORTB=(1<<PINB0)|(0<<PINB1);
        }
        
         //-----------------------------------
        
        else if (((y&0b1)&(x&0b1))==0)
        {
            PORTC=(0<<PINC0)|(1<<PINC1);
            PORTB=(0<<PINB0)|(1<<PINB1);
            _delay_ms(100);
            PORTC=(1<<PINC0)|(0<<PINC1);
            PORTB=(1<<PINB0)|(0<<PINB1);
            _delay_ms(100);
        }
        
         //-----------------------------------

        else
        {
            PORTC=(0<<PINC0)|(0<<PINC1);
            PORTB=(0<<PINB0)|(0<<PINB1);
        }
    }
    return 0;
}
