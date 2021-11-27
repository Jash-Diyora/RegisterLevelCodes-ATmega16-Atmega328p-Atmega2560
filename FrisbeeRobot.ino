#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#define MISO 3//4 //data
#define MOSI 2//3 //cmnd
#define SCK 1// 5 //clock
#define SS 0// 2 //attention
int pwm = 0, checkflag = 0, mainkillswitch = 0, steppercalib = 0 , loadingflag = 1 , stepman = 1 ,
rota = 0 , rot , rot1 , once , only = 0, servoflag = 0 , rackkillswitch = 0;
int mannualmode = 0 , rackkillswitchs = 0, automode = 1 , stepperflag = 0, junctionpulse = 0 ,
positions = 0 , rpmflag = 0 , steps = 0, increment = 0 , decrement = 0;
int data0 = 0, data1 = 0, data2 = 0, data3 = 0, data4 = 0, data5 = 0, killflag = 0;
float degree = 19 , input = 0;
double c = 0 ;
double ticks = 0;
double rpm = 0;
double count = 0;
long double error = 0;
long double errorsum = 0;
long double ierror = 0;
double output = 0;
double dpwm = 0;
double pwmi = 0 ;
int lasterror = 0;
double desirerpm = 0;
double rotation = 0;
unsigned short angle = 0;
int uvpce = 0;
float kp = 27;
float ki = 0.0065;
long double calculationflag = 0;
long double counterflag = 0;
int main(void)
{
Serial3.begin(9600);
INIT_SPI_MSTR();
config_analog();
sei();
/*******DATA DIRECTION , PWM FOR BASE MOTORS & MAIN ROTOR************/
/***********DATA DIRECTION FOR BASE MOTORS & MAIN ROTOR ************/
DDRE |= (1 << PIN5) | (1 << PIN4) | (1 << PIN3) | (1 << PIN6) | (1 << PIN7);
DDRH |= (1 << PIN4) | (1 << PIN5) | (1 << PIN3) | (1 << PIN2) | (1 << PIN7); //L-R-MAIN
ROTOR
DDRB |= (1 << PINB4);
TCCR4A |= (1 << COM4A1) | (1 << COM4B1) | (1 << COM4C1) | (1 << WGM40); //OCR4A
OCR4B F-B
TCCR4B |= (1 << WGM42) | (1 << CS40);
TCCR3A |= (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) | (1 << WGM30); //OCR3A
OCR3B OCR3C MAIN MOTOR-L-R
TCCR3B |= (1 << CS30) | (1 << WGM32);
TCCR2A |= (1 << COM2A1) | (0 << COM2A0) | (1 << WGM21) | (1 << WGM20);
TCCR2B |= (1 << CS20);
/****** SERVO PWM ********/
// DDRL |= (1 << PIN5);
// TCCR5A |= (1 << COM5C1) | (1 << WGM41);
// TCCR5B |= (1 << WGM53) | (1 << WGM52) | (1 << CS51) | (1 << CS50);
// ICR5 = 4999;
// OCR5C = 270;
/***** STEPPER DATA DIRECTIONS********/
DDRD |= (1 << PIN7) | (1 << PIN6) | (1 << PIN5) ;
DDRD &= ~ (1 << PIN0);
DDRD &= ~ (1 << PIN4);
/********* INTERRUPT FOR LSA 08 JUNCTION PULSES AND ENCODER *********/
PORTD |= (1 << PIN0) | (1 << PIN4);
EIMSK |= (1 << INT0);
EICRA |= (0 << ISC01) | (1 << ISC00);
/***** PNEUMATIC DATA DIRECTION*****/
DDRA |= (1 << PIN0) | (1 << PIN1) | (1 << PIN2);
/***TIMER FOR RPM******/
OCR1A = 887;
TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);
TIMSK1 |= (1 << OCIE1A);
/******RACK PINION DIRECTION AND KILL SWITCH*********/
DDRL |= (1 << PIN6); //RP DIR
DDRJ &= ~ (1 << PIN2);
DDRJ &= ~ (1 << PIN3);
PORTJ = (1 << PIN2) | (1 << PIN3);
/*****HERCULES DRIVE MAIN MOTOR*******/
DDRG |= (1 << PIN0) | (1 << PIN1);
DDRG &= ~(1 << PIN2);
OCR4C = 0;
desirerpm = 0;
dpwm = 0;
OCR2A = 0;
while (1)
{
mainkillswitch = (PING & 0b00000100);
if (mainkillswitch == 0 && killflag == 0)
{
//polling mode-42
PORTB &= ~(1 << SS);
hey_slave(0x01);
hey_slave(0x42);
hey_slave(0x00);
data0 = hey_slave(0x00);
data1 = hey_slave(0x00);
data4 = hey_slave(0x00);
data5 = hey_slave(0x00);
data2 = hey_slave(0x00);
data3 = hey_slave(0x00);
_delay_us(1);
PORTB |= (1 << SS);
_delay_us(10);
PORTG |= (1 << PIN0);
steppercalib = (PIND & 0b00010000);
rackkillswitch = (PINJ & 0b00000100); //>
rackkillswitchs = (PINJ & 0b00001000); //<
/*******<> <> **** SERIAL PRINTING DATA ***** <> <> ********/
Serial3.print(angle);
Serial3.print(" ");
Serial3.println(desirerpm);
/**** SIDE MOTOR CALCULATIONS*****/
pwm = ( desirerpm * 255 / 1800);
OCR2A = (pwm * 0.3);
if (OCR2A >= 255)
{
OCR2A = 255;
}
if (OCR2A <= 0)
{
OCR2A = 0;
}
/*******<> <> **** AUTO AND MANAUL STEPPER ***** <> <> ********/
if ((data2 == 255) && (data3 == 255) && (data4 == 255) && (data5 == 255))
{
config_analog();
stopthebot();
}
if ((rota < angle) && (rota != angle) && (stepman == 1) && (steppercalib == 16))
{
PORTD |= (1 << PIN7) | (1 << PIN6);
rot = (angle - rota);
for ( rot1 = 1; rot1 <= rot; rot1++ )
{
angle--;
OCR4A = 0;
OCR4B = 0;
OCR3C = 0;
OCR3B = 0;
//Serial.println(angle);
for ( steps = 0 ; steps < 200 ; steps++)
{
PORTD |= (1 << PIN5);
_delay_us(370);
PORTD &= ~ (1 << PIN5);
_delay_us(370);
if ((data2 == 255) && (data3 == 255) && (data4 == 255) && (data5 == 255))
{
config_analog();
stopthebot();
}
}
}
}
else if ((rota > angle) && (rota != angle) && (stepman == 1) && (steppercalib == 16))
{
PORTD |= (1 << PIN7);
PORTD &= ~(1 << PIN6);
rot = (rota - angle);
for ( rot1 = 1; rot1 <= rot; rot1++ )
{
OCR4A = 0;
OCR4B = 0;
OCR3C = 0;
OCR3B = 0;
angle++;
//Serial.println(angle);
for ( steps = 0 ; steps < 200 ; steps++)
{
PORTD |= (1 << PIN5);
_delay_us(370);
PORTD &= ~ (1 << PIN5);
_delay_us(370);
if ((data2 == 255) && (data3 == 255) && (data4 == 255) && (data5 == 255))
{
config_analog();
stopthebot();
}
}
}
}
if ((steppercalib == 16) && (stepperflag == 0))
{
PORTD |= (1 << PIN7) | (1 << PIN6);
for ( steps = 0 ; steps < 400 ; steps++)
{
PORTD |= (1 << PIN5);
_delay_us(370);
PORTD &= ~ (1 << PIN5);
_delay_us(370);
}
}
else if ((steppercalib == 0))
{
stepperflag = 1;
PORTD |= (1 << PIN7);
PORTD &= ~ (1 << PIN6);
for ( steps = 0 ; steps < 200 ; steps++)
{
PORTD |= (1 << PIN5);
_delay_us(370);
PORTD &= ~ (1 << PIN5);
_delay_us(370);
if (steppercalib == 16)
{
PORTD &= ~ (1 << PIN7);
PORTD &= ~ (1 << PIN5);
}
}
}
else if (angle == 65535)
{
angle++;
PORTD |= (1 << PIN7);
PORTD &= ~(1 << PIN6);
for ( steps = 0 ; steps < 200 ; steps++)
{
PORTD |= (1 << PIN5);
_delay_us(370);
PORTD &= ~ (1 << PIN5);
_delay_us(370);
if (angle == 0)
{
PORTD &= ~ (1 << PIN7);
PORTD &= ~ (1 << PIN5);
}
}
}
else if ((data3 == 255) && (angle >= 0) && (angle < 45))
{
angle++;
stepman = 0;
PORTD |= (1 << PIN7);
PORTD &= ~(1 << PIN6);
for ( steps = 0 ; steps < 200 ; steps++)
{
PORTD |= (1 << PIN5);
_delay_us(370);
PORTD &= ~ (1 << PIN5);
_delay_us(370);
}
}
else if ((data3 == 0) && (angle >= 0) && (angle <= 45))
{
angle--;
stepman = 0;
PORTD |= (1 << PIN7) | (1 << PIN6);
for ( steps = 0 ; steps < 200 ; steps++)
{
PORTD |= (1 << PIN5);
_delay_us(370);
PORTD &= ~ (1 << PIN5);
_delay_us(370);
}
}
else if ((data3 == 123))
{
PORTD &= ~ (1 << PIN7);
PORTD &= ~ (1 << PIN5);
}
/********* <> <> *********** POLE SELECTION ******* <> <> ************/
if (data0 == 191) //POLE 1
{
stepman = 1;
desirerpm = 665; // 565
rota = 17; // 11
}
else if (data0 == 127 ) //POLE 2
{
stepman = 1;
desirerpm = 873; // 763
rota = 4; //0
}
else if (data0 == 223 ) //POLE 3
{
stepman = 1;
desirerpm = 968; //828
rota = 25; // 22
}
else if (data0 == 254 ) //POLE 4
{
stepman = 1;
desirerpm = 1035; //895
rota = 42; //39
}
else if (data0 == 247) //POLE 5 - 3 COPY
{
stepman = 1;
desirerpm = 968; //828
rota = 25; // 22
}
else if (data1 == 127) //POLE 6 - 2 COPY
{
stepman = 1;
desirerpm = 873; // 763
rota = 4; //0
}
else if (data0 == 239) //POLE 7
{
stepman = 1;
desirerpm = 1255; //1065
rota = 13; //8
}
else if (data0 == 251) //PNEUMATIC GETS BACK
{
PORTA &= ~ (1 << PIN1);
loadingflag = 1;
}
else if (data0 == 253) // VIBRATOR
{
PORTA |= (1 << PIN2);
_delay_ms(100);
PORTA &= ~ (1 << PIN2);
}
/***** <> <> ****** PNEUMATIC SHOOT RELAY****** <> <>*******/
if ((data1 == 239)) // && (loadingflag == 1)
{
PORTA |= (1 << PIN0);
PORTA |= (1 << PIN2);
_delay_ms(100);
PORTA &= ~ (1 << PIN0);
PORTA &= ~ (1 << PIN2);
// loadingflag = 0;
}
//
// else if ((data1 == 239) && (loadingflag == 0))
// {
// loadingflag = 0;
// PORTA |= (1 << PIN0);
// PORTA |= (1 << PIN1);
// PORTA |= (1 << PIN2);
// _delay_ms(100);
// PORTA &= ~ (1 << PIN0);
// PORTA &= ~ (1 << PIN1);
// _delay_ms(200);
// PORTA &= ~ (1 << PIN1);
// PORTA &= ~ (1 << PIN2);
// _delay_ms(200);
// PORTA |= (1 << PIN1);
// }
/*******<> <> **** RACK PINION ***** <> <> ********/
if (data1 == 251) //RACK PINION < // left slide
{
if (rackkillswitchs == 8)
{
PORTA &= ~ (1 << PIN1);
PORTL &= ~ (1 << PIN6);
OCR3A = 150;
loadingflag = 1;
}
else
{
OCR3A = 0;
}
}
else if (data1 == 247) //RACK PINION > //Right slide
{
if (rackkillswitch == 4)
{
PORTA &= ~ (1 << PIN1);
PORTL |= (1 << PIN6);
OCR3A = 150;
loadingflag = 1;
}
else
{
OCR3A = 0;
}
}
else
{
OCR3A = 0;
}
/*******<> <> **** RPM + - ***** <> <> ********/
if (data1 == 191 && once == 0) //RPM INCREASE
{
PORTG |= (1 << PIN1);
once = 1;
desirerpm -= 10;
if (desirerpm < 4)
{
desirerpm = 0;
}
}
else if (data1 == 223 && once == 0) // RPM DECREASE
{
PORTG |= (1 << PIN1);
once = 1;
desirerpm += 10;
if (desirerpm > 1800)
{
desirerpm = 1600;
}
}
else if (data1 == 255 && once == 1)
{
once = 0;
}
/*******<> <> **** SERVO ARM ***** <> <> ********/
if (data1 == 254)
{
OCR4A = 70;
OCR4B = 70;
PORTH |= (1 << PIN2);
PORTH |= (1 << PIN7);
}
else if (data1 == 253)
{
OCR4A = 70;
OCR4B = 70;
PORTH &= ~ (1 << PIN2);
PORTH &= ~ (1 << PIN7);
}
/*******<> <> **** MOTIONS ***** <> <> ********/
else if ((data4 > 123))
{
input = ((data4 - 123) * 1.5530);
forward();
}
else if ((data4 < 123 ))
{
input = ((123 - data4) * 1.6667);
backward();
}
else if ( (data5 < 123) )
{
input = (((123 - data5) * 1.6667) / 2);
backwardslide();
}
else if ( (data5 > 123) )
{
input = (((data5 - 123) * 1.5530) / 2);
forwardslide();
}
else if (data2 < 20)
{
rightslide(); // anticlockwise
}
else if (data2 > 240)
{
leftslide(); //clockwise
}
else
{
stopthebot();
}
}
else if (mainkillswitch == 4 )
{
killflag = 1;
PORTD &= ~ (1 << PIN7);
PORTD &= ~ (1 << PIN5) ;
desirerpm = 0;
dpwm = 0;
OCR3B = 0;
OCR3C = 0;
OCR4A = 0;
OCR4B = 0;
OCR4C = 0;
OCR3A = 0;
OCR2A = 0;
}
} //WHILE END
} //MAIN END
/**********<> <>**** MAIN PROGRAM ENDS , INTERRUPTS AND USER DEFINE FUNCTION
BEGINS****<> <>**********/
void INIT_SPI_MSTR()
{
DDRB |= (1 << SS) | (1 << SCK) | (1 << MOSI);
DDRB &= ~(1 << MISO);
SPCR |= (1 << SPE) | (1 << DORD) | (1 << MSTR) | (1 << CPOL) | (1 << CPHA) | (1 << SPR1) |
(1 << SPR0);
PORTB |= (1 << SS);
}
int hey_slave(int cmnd)
{
int data;
SPDR = cmnd;
while (!(SPSR & (1 << SPIF)));
_delay_us(170);
data = SPDR;
return (data);
}
void config_analog()
{
int doneconfig = 0;
while (doneconfig != 0x73)
{
//put controller in config mode-43
PORTB &= ~(1 << SS);
hey_slave(0x01);
hey_slave(0x43);
hey_slave(0x00);
hey_slave(0x01); //enter confi
hey_slave(0x00);
_delay_us(1);
PORTB |= (1 << SS);
_delay_us(10);
//Turn on analog mode-44
PORTB &= ~(1 << SS);
hey_slave(0x01);
hey_slave(0x44);
hey_slave(0x00);
hey_slave(0x01);
hey_slave(0x03);
hey_slave(0x00);
hey_slave(0x00);
hey_slave(0x00);
hey_slave(0x00);
_delay_us(1);
PORTB |= (1 << SS);
_delay_us(10);
//Exit config mode-43
PORTB &= ~(1 << SS);
hey_slave(0x01);
hey_slave(0x43);
hey_slave(0x00);
hey_slave(0x00);
hey_slave(0x5A);
hey_slave(0x5A);
hey_slave(0x5A);
hey_slave(0x5A);
hey_slave(0x5A);
_delay_us(1);
PORTB |= (1 << SS);
_delay_us(10);
//polling mode-42
PORTB &= ~(1 << SS);
hey_slave(0x01);
doneconfig = hey_slave(0x42);
hey_slave(0x00);
hey_slave(0x00);
hey_slave(0x00);
hey_slave(0x00);
hey_slave(0x00);
hey_slave(0x00);
_delay_us(1);
PORTB |= (1 << SS);
_delay_us(10);
}
}
void forward()
{
OCR3B = (50 + input);
OCR3C = (50 + input);
PORTE |= (1 << PIN6);
PORTE &= ~ (1 << PIN7);
}
void backward()
{
OCR3B = (50 + input);
OCR3C = (50 + input);
PORTE &= ~(1 << PIN6);
PORTE |= (1 << PIN7);
}
void backwardslide()
{
OCR4A = (50 + input);
OCR4B = (50 + input);
PORTH &= ~ (1 << PIN2);
PORTH |= (1 << PIN7);
}
void forwardslide()
{
OCR4A = (50 + input);
OCR4B = (50 + input);
PORTH |= (1 << PIN2);
PORTH &= ~ (1 << PIN7);
}
void leftslide()
{
OCR4A = 80;
OCR4B = 80;
PORTH &= ~ (1 << PIN2);
PORTH &= ~ (1 << PIN7);
}
void rightslide()
{
OCR4A = 80;
OCR4B = 80;
PORTH |= (1 << PIN2);
PORTH |= (1 << PIN7);
}
void anticlockwise()
{
OCR4A = (50 + input);
OCR4B = (50 + input);
PORTH |= (1 << PIN2);
PORTH |= (1 << PIN7);
}
void clockwise()
{
OCR4A = (50 + input);
OCR4B = (50 + input);
PORTH &= ~ (1 << PIN2);
PORTH &= ~ (1 << PIN7);
}
void stopthebot()
{
OCR4A = 0;
OCR4B = 0;
OCR3C = 0;
OCR3B = 0;
}
/****** MAIN MOTOR PID ********/
void PID()
{
error = (desirerpm - rpm);
errorsum = errorsum + error;
output = (kp * error) + (ki * errorsum); // + kd*ierror;
if (output > 255)
{
dpwm = 255;
}
else if (output <= 0)
{
dpwm = 0;
}
else
{
dpwm = output;
}
}
/****** ENCODER RPM ********/
ISR (INT0_vect)
{
ticks = (ticks + 2);
}
ISR(TIMER1_COMPA_vect)
{
// uvpce++;
count = ticks / 2000;
ticks = 0;
rpm = (count * 1056);
PID();
OCR4C = dpwm;
}
