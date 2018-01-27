/*
 * ROBOCON_ATMEGA16.cpp
 *
 * Created: 9/14/2016 12:36:05 PM
 *  Author: Jash Diyora
 */ 

 #include<avr/io.h>
 #include<util/delay.h>
 #define F_CPU 160000000UL
 
  int main(void)
  {
  DDRB = 0xFF;
  while (1)
  {
    PORTB = 0b11111111;
    _delay_ms(500);
    PORTB = 0b00000000;
    _delay_ms(500);
    PORTB = 0b10000000;
    _delay_ms(500);
    PORTB = 0b11000000;
    _delay_ms(500);
    PORTB = 0b11100000;
    _delay_ms(500);
    PORTB = 0b11110000;
    _delay_ms(500);
    PORTB = 0b11111000;
    _delay_ms(500);
    PORTB = 0b11111100;
    _delay_ms(500);
    PORTB = 0b11111110;
    _delay_ms(500);
    PORTB = 0b11111111;
    _delay_ms(500);
    PORTB = 0b00000001;
    _delay_ms(500);
    PORTB = 0b00000011;
    _delay_ms(500);
    PORTB = 0b00000111;
    _delay_ms(500);
    PORTB = 0b00001111;
    _delay_ms(500);
    PORTB = 0b00011111;
    _delay_ms(500);
    PORTB = 0b00111111;
    _delay_ms(500);
    PORTB = 0b01111111;
    _delay_ms(500);
    PORTB = 0b11111111;
    _delay_ms(500);
  }
  }