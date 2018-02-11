#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRD=0XFF;
    TCCR1A |= 1<<WGM11 | 1<<COM1A1 | 1<<COM1A0 ; //INVERTED
    TCCR1B |= 1<<WGM12 | 1<<WGM13 | 1<<CS10 ;    // NO PRESCALE F-PWM
    ICR1= 19999; //2MS  0-19999=20000 50HZ
	
	while (1)
    {
        OCR1A=ICR1-2200 ;
        _delay_ms(500);
        OCR1A=ICR1-200;
        _delay_ms(500);
	}
	return 0;
}
