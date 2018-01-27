//#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
unsigned char baudrate=103; //9600
    
    DDRB |= (1<<PINB1);   //LED
    PORTB &= ~(1<<PINB1); //LED
    
    DDRB &= ~(1<<PINB0);
    PORTB |= (1<< PINB0);
    
    int buttonpress=0 , pressdebounce =0 , releasedebounce = 0;
    
    UBRRH =  (baudrate>>8);
    UBRRL =  baudrate;
    UCSRB = (1<<RXEN) | (1<<TXEN);
    UCSRC = (1<<USBS) | (3<<UCSZ0);
    
    while(1)
    {
       if(bit_is_set(PINB, 0))
       {
           pressdebounce++;
           releasedebounce=0;
           
            if(pressdebounce>100)
             {
                if(buttonpress ==0 )
                 {
                   buttonpress=1;
                   PORTB ^= (1<<PINB1);
                   while (!(UCSRA & (1<<UDRE)));
                   UDR = 0b1;
                     
                 }
               pressdebounce=0;
            }
        }
        
        else
        {
            releasedebounce++;
            pressdebounce=0;
            
            if(releasedebounce>100)
            {
                buttonpress=0;
                releasedebounce=0;
            }
        }
        
    }
    
}


