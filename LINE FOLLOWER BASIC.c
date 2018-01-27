#include<avr/io.h>
#include<util/delay.h>
#define F_CPU 8000000UL


void jashdelay()
{
    int z;
    
    for(z=0;z<123;z++)
    {
        DDRB|=(1<<3);
        OCR2=254;
        TCCR2=0b00000101;   // normal , prescale 1/1024
        while(TIFR&(1<<OCF2)==0);
        TCCR2=0;
        // TIFR = 0b10000000;
    }
}


int main(void)
{
    DDRB=0xFF;
    DDRD=0xFF;
    DDRA=0x00;
    TCCR0=0b01101101;
    PORTD = 0b00000011;
    
    
    while (1)
    {
        int x=((PINA&0b00000001)  && (PINA&0b00000010));

        if(x==1)
        {
            OCR0=0;
            _delay_ms(3000);
            
        }
else
{
    	PORTB=0xFF;
        OCR0=100;
	
        //jashdelay();
    }
    
}

}
