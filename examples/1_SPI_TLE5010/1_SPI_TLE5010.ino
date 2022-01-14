/*
 * TLE5010 SPI example
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

  double angle;
  int X, Y;

  int _micros[2];

  //read angle and measure time
  _micros[0]=micros();
  angle=sensor.readAngleRadians();
  _micros[1]=micros();
  X=sensor.x;
  Y=sensor.y;
  
  Serial.print("X: ");
  Serial.print(X);
  Serial.print(" Y: ");
  Serial.print(Y);
  Serial.print(" Angle: ");
  Serial.print(angle);
  Serial.print(" Time: ");
  Serial.print(_micros[1]-_micros[0]);
  Serial.println();;
}
