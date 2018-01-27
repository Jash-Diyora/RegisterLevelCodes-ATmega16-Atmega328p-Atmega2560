#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void led1()
{
    PORTA=0b11111111;
    _delay_ms(1000);
    PORTA=0b11110000;
    _delay_ms(1000);
    PORTA=0b00001111;
    _delay_ms(1000);
    led2();
}

void led2()
{
    PORTA=0b00011000;
    _delay_ms(1000);
    PORTA=0b01000010;
    _delay_ms(1000);
    PORTA=0b10000001;
    _delay_ms(100);
    led1();
}

int main(void)
{
    MCUCR=0x02;
    GICR = (1<<INT0)|( 1<<INT1)|(1<<INT2);
    sei();
    
    DDRB = 0x00;
    DDRD = 0x00;
    DDRC = 0xff;
    DDRA = 0xff;
    led1();
    led2();
    while(1);
    {
        //NOTHING INTRRUPT IS ON
    }
}
ISR(INT0_vect)
{
    PORTC ^= (1<<PINC2);
}

ISR(INT1_vect)
{
    PORTC ^= (1<<PINC3);
}

ISR(INT2_vect)
{
    PORTC ^= (1<<PINC4);
}

/*
 
 int main(void)
   {
       sei();
       DDRB=0xff;
       DDRC=0xff;
       TCNT0= -35;
       TCCR0=0b01101001;
       TIMSK|=(1<<OCIE0);
       OCR0= 100;
       while (1)
       {
           PORTC=0xff;
           _delay_ms(1000);
           PORTC=0x00;
           _delay_ms(1000);
       }

   }

ISR(TIMER0_COMP_vect)
{
    TCNT0= -35;
    PORTB ^= 1<<PINB0;
}

*/
