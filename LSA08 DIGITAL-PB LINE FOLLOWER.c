#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define S1 PINA
#define S2 PINA>>1
#define S3 PINA>>2
#define S4 PINA>>3
#define S5 PINA>>4
#define S6 PINA>>5
#define S7 PINA>>6
#define S8 PINA>>7

int main(void)

{
    DDRB=0xFF;
    DDRA=0X00;
    PORTA=0X00;
    
    while (1)
    {
        
         //PORTB=0b11000011; //back
        //----1-------------------------------
        
        
       if (PINA & ((1<<PINA3)|(1<<PINA4))) // 0B00011000
        {
          PORTB=0b10000010;  //fronT
        }
        
        //-----2------------------------------
        
       // else if (PINA & ) //0B11100111
        //{
           
       // }
        
        //------3-----------------------------
        
        else if (PINA & ((1<<PINA0)|(1<<PINA1)|(1<<PINA2)) ) //0B00000011
        {
     
           PORTB=0b11000010; //left
        }
        
        //-------4----------------------------
        
        else if (PINA & ((1<<PINA7)|(1<<PINA6)|(1<<PINA5)) ) //0B11000000
        {
            PORTB=0b10000011; //right
        }

        
        else
        {
            PORTB=0b00000000;
        }


    }
   
}
