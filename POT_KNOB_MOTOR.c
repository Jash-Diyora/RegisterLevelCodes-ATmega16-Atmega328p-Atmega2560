// 8 BIT OPERATION
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

void adcinstall()
{
    ADMUX = (1<<REFS1) | (1<<REFS0) | (1<<ADLAR);
    // ADLAR=0 THEREFORE D0-D7 BITS SETS IN ADCL  , D8-D9 SETS IN ADCH , VICE VERSA IF ADLAR=1
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
    }

void converter(int channel)
{
    ADMUX = (ADMUX | channel);
    ADCSRA |= (1 << ADSC);
    while ((ADCSRA & (1 << ADIF)) == 0);
    return 0;
}

int main(void)
{
    DDRB = 0xFF;
    DDRA = 0x00;
    adcinstall();
    
    while(1)
    {
        converter(0);
        PORTB=ADCH;
    }
}




/*
 
 
 #define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


void adcinstall()
{
    ADMUX = (1<<REFS1) | (1<<REFS0) | (0<<ADLAR);
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
    
}

void converter(int channel)
{
    ADMUX = (ADMUX | channel );
    ADCSRA |= (1 << ADSC);
    while ((ADCSRA&(1<<ADIF))==0);
    return 0;
}


int main(void)
{
    DDRB = 0xFF;
    DDRA = 0x00;
    DDRD = 0xFF;
    TCCR0=0b01101001;
    int x;
    adcinstall();
    
    while(1)
    {
        converter(0);
        OCR0= (ADC/4);
        
    }
}
 */
