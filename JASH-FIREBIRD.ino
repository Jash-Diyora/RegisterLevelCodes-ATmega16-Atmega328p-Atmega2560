/*
  Project Name:   <Vision Guided Autonomous Mobile Robot In Static Environment>
  Purpose: Firmware for Nex Robotics Firebird Robot to accept commands from MATLAB.
  Author List:  <Jash Diyora, Dhaval Patel>
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>

void USART_Init( unsigned int ubrr)
{
  UBRR3H = (unsigned char)(ubrr >> 8); // Set baud rate
  UBRR3L = (unsigned char)ubrr;
  UCSR3B = (1 << RXEN3) | (1 << TXEN3); //Enable receiver and transmitter
  UCSR3C = (1 << USBS3) | (3 << UCSZ30); // Set frame format: 8data, 2stop bit
}

void motorconfig(void)
{
  DDRA = 0xFF; //set motor pins to output
  DDRL = 0xFF; //set motor enable pins to output
  PORTL = 0b00011000; //right and left motor enable
}

void stop()
{
  PORTA = 0x00; // set PORTA to zero to stop motors
}

void left()
{

  PORTA = 0x05;     // start motion in left direction
}

void right()
{

  PORTA = 0x0A;     // Set motors motion in hard right turn
}

void forward()
{

  PORTA = 0x06;       //set robot in forward motion
}

void back()
{

  PORTA = 0x09;       //set robot in forward motion
}

int main(void)
{
  unsigned long int x;
  motorconfig();      // enable motors       // enable encoders
  USART_Init(7);          // initialize serial communication
  stop();         // stop the bot if already moving

  while (1)         // loop to start motion according to data recieved on serial port
  {
    while (!(UCSR3A & 1 << RXC3));  // wait till data is recieved
    x = UDR3;       // store the data into variable x
    if (x == 50)  // checks if data is recieved from MATLAB and not anyother device
    {
      forward();
    }

    else if (x == 49)  // checks if data is recieved from MATLAB and not anyother device
    {
      left();
    }

    else if (x == 51)  // checks if data is recieved from MATLAB and not anyother device
    {
      right();
    }

    else if (x == 52)  // checks if data is recieved from MATLAB and not anyother device
    {
      back();
    }

    else
    {
      stop();  // If data is not recieved from MATLAB, do nothing and continue recieving data
    }
  }

}

