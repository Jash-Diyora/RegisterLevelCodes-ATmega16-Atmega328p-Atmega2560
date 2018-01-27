//#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    unsigned char baudrate=103; //9600
    int data;
    
    
    DDRB = (1<<PINB0);
 
    
    UBRRH =  (baudrate>>8);
    UBRRL =  baudrate;
    UCSRB = (1<<RXEN) | (1<<TXEN);
    UCSRC = (1<<USBS) | (1<<UCSZ0);
    
    
    while(1)
    {
        while(!(UCSRA&(1<<RXC)));
        
        data=UDR;
        
        if(data!=0b1)
        {
            PORTB^= (1<<PINB0);
        }
        
        else
        {
            PORTB^= (0<<PINB0);
        }
    
    }
    
}


