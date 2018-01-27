//  Author  - Jash Diyora.
//  Email   - jashdiyora7@gmail.com
//  Thanks  - Kaushal Garala.
//  Remarks -


#define F_CPU 160000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define b1 PINA
#define b2 PINA>>1
#define b3 PINA>>2
#define b4 PINA>>3
#define b5 PINA>>4


int main(void)

{
    unsigned short position = 0;
    unsigned short movement = 0;
    int time = 0;
    
    
    DDRA = 0X00;
    DDRB = 0XFF;
    PORTA = 0XFF;
    
    while (1)
    {
        
        //  PORTB=0b10000011; //forward
        //  PORTB=0b11000010;  //backward
        
        
        //--------------------------------------
        
        if ((b1 & 0b1) == 0) //-B
        {
            if (1 < position)
            {
                movement = position - 1;
                PORTB = 0b11000010;
                for (time = 0; time <= movement; time++)
                {
                    _delay_ms(100);
                }
                
                PORTB = 0x00;
                position = 1;
                
            }
            
            //****************************
            
            if (position < 1) //+F
            {
                movement = 1 - position;
                PORTB = 0b10000011;
                for (time = 0; time <= movement; time++)
                {
                    _delay_ms(100);
                }
                PORTB = 0x00;
                position = 1;
                
            }
        }
        
        //-------------------------------------
        
        else   if ((b2 & 0b1) == 0)
        {
            if (2 < position)
            {
                movement = position - 2;
                PORTB = 0b11000010;
                for (time = 0; time <= movement; time++)
                {
                    _delay_ms(100);
                }
                PORTB = 0x00;
                position = 2;
                
            }
            
            //****************************
            
            if (position < 2)
            {
                movement = 2 - position;
                PORTB = 0b10000011;
                for (time = 0; time <= movement; time++)
                {
                    _delay_ms(100);
                }
                PORTB = 0x00;
                position = 2;
                
            }
        }
        
        //--------------------------------------
        
        else  if ((b3 & 0b1) == 0)
        {
            if (3 < position)
            {
                movement = position - 3;
                PORTB = 0b11000010;
                for (time = 0; time <= movement; time++)
                {
                    _delay_ms(100);
                }
                PORTB = 0x00;
                position = 3;
                
            }
            
            //****************************
            
            if (position < 3)
            {
                movement = 3 - position;
                PORTB = 0b10000011;
                for (time = 0; time <= movement; time++)
                {
                    _delay_ms(100);
                }
                PORTB = 0x00;
                position = 3;
                
            }
        }
        
        //--------------------------------------
        
        else  if ((b4 & 0b1) == 0)
        {
            if (4 < position)
            {
                movement = position - 4;
                PORTB = 0b11000010;
                for (time = 0; time <= movement; time++)
                {
                    _delay_ms(100);
                }
                PORTB = 0x00;
                position = 4;
                
            }
            
            //****************************
            
            if (position < 4)
            {
                movement = 4 - position;
                PORTB = 0b10000011;
                for (time = 0; time <= movement; time++)
                {
                    _delay_ms(100);
                }
                PORTB = 0x00;
                position = 4;
                
            }
        }
        
        //--------------------------------------
        
        else  if ((b5 & 0b1) == 0)
        {
            if (5 < position)
            {
                movement = position - 5;
                PORTB = 0b11000010;
                for (time = 0; time <= movement; time++)
                {
                    _delay_ms(100);
                }
                PORTB = 0x00;
                position = 5;
                
            }
            
            //****************************
            
            if (position < 5)
            {
                movement = 5 - position;
                PORTB = 0b10000011;
                for (time = 0; time <= movement; time++)
                {
                    _delay_ms(100);
                }
                PORTB = 0x00;
                position = 5;
                
            }
            
        }
        
        //--------------------------------------
        
        else
        {
            PORTB = 0x00;
        }
    }
    return 0;
}
