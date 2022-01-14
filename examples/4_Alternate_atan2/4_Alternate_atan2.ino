/*
 * TLE5010 Alternate atan2 example
 * 
 * SPI mode
 * 
 * hardware:
 *   Arduino Uno
 *   CLK: pin 9
 *   SCK: pin 13  
 *   CS:  pin 8  
 *   DATA: pin 12 
 *   connect pin 11 to DATA through resistor 10kOhm
*/

#include <TLE5010.h>

#define PIN_CS 8

TLE5010_SPI sensor(PIN_CS);

void setup() {

  //4 MHZ square wave on pin 9
  TCCR1A=(0<<COM1A1) | (1<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
  TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
  OCR1A=0x01;
  ICR1=0;
  pinMode(9,OUTPUT);

  //initialization
  sensor.begin();

  Serial.begin(9600);
}

void loop() {

  double angle[3];
  uint16_t _time[4];

  /*
    included alternate atan2 functions:

    _atan2CORDIC18
    _atan2CORDIC16
    _atan2CORDIC14
    _atan2CORDIC12
    _atan2CORDIC11
    _atan2CORDIC10
    _atan2CORDIC8
    _atan2ApproxA
    _atan2ApproxB
   */

  _time[0]=micros();

  //atan2 approximation function
  sensor.atan2Func=_atan2ApproxA;
  angle[0]=sensor.readAngleRadians();

  _time[1]=micros();

  //atan2 CORDIC 16bit function
  angle[1]=sensor.readAngleRadians(_atan2CORDIC16);

  _time[2]=micros();
  
  //revert to default atan2
  sensor.atan2Func=_atan2default; 
  angle[2]=sensor.readAngleRadians();

  _time[3]=micros();

    
  Serial.println(
          " ApproxA: " +String(angle[0])+" ("+String(_time[1]-_time[0])+"us); "+
          " CORDIC16: "+String(angle[1])+" ("+String(_time[2]-_time[1])+"us); "+
          " Default: " +String(angle[2])+" ("+String(_time[3]-_time[2])+"us); "
          );
}
