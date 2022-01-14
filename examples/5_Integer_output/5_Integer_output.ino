/*
 * TLE5010 Integer output example
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

  /*
    included integer atan2 functions:

    _atan2Int16
    _atan2Int15
    _atan2Int14
    _atan2Int13
    _atan2Int12
    _atan2Int11
    _atan2Int10
    _atan2Int9
    _atan2Int8
    
   */

  //set integer function 
  sensor.atan2FuncInt=_atan2Int16;

  Serial.begin(9600);
}

void loop() {

  int angleInt;

  //read integer value
  angleInt=sensor.readInteger();
  
  Serial.println(
          " Integer value: " +String(angleInt)
          );
}
