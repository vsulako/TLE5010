/*
 * TLE5010 Multiple sensors example
 * 
 * SPI mode
 * 
 * hardware:
 *   Arduino Uno
 *   CLK: pin 9
 *   SCK: pin 13  
 *   DATA: pin 12 
 *   connect pin 11 to DATA through resistor 10kOhm
 *   
 *   first sensor CS:  pin 8  
 *   second sensor CS: pin 7  
*/

#include <TLE5010.h>

#define PIN_CS1 8
#define PIN_CS2 7

TLE5010_SPI sensor(PIN_CS1);

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

  double angle[2];
  
  //read angle from 1st sensor
  sensor.setCS(PIN_CS1);
  angle[0]=sensor.readAngleRadians();

  //read angle from 2nd sensor
  sensor.setCS(PIN_CS2);
  angle[1]=sensor.readAngleRadians();
  
  Serial.print(" Angle #1: ");
  Serial.print(angle[0]);
  Serial.print(" Angle #2: ");
  Serial.print(angle[1]);
  Serial.println();
}
