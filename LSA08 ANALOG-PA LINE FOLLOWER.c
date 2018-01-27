#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#define JPIN PINA>>1


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
}



int main(void)
{
    DDRB = 0xFF;
    DDRA = 0x00;
    PORTA=0X00;
    int positionvalue;
    adcinstall();
   
    
    
    
    //  PORTK=0b10000010; //FRONT
    
    // PORTK=0b11000011; //BACK
    
    //  PORTK=0b11000010; //LEFT
    
    //PORTK=0b10000011; //RIGHT
    
    while(1)
    {
        
        
        converter(0);
        
        //    positionvalue=((ADC/921)*70);
        
        
        if ((JPIN&0b1))
        {
            while ((JPIN&0b1))
            {
                PORTB=0x00;
            }
        }
        
        if((ADC>=440 && ADC<=600))   //(positionvalue>=30 && positionvalue<=40)
        {
            PORTB=0b10000010; //FRONT
        }
        
        
        else if((ADC>=0 && ADC<=400))  //(positionvalue>=0 && positionvalue<=29)
        {
            PORTB=0b11000010; //LEFT
        }
        
        
        else if((ADC>=800 && ADC<=980))  //(positionvalue>41 && positionvalue<=70)
        {
            PORTB=0b10000011; //RIGHT
        }
        
        else
        {
            PORTB=0x00;
        }
        
    }
}

