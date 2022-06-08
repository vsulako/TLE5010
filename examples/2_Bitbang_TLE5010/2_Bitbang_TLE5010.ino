/*
 * TLE5010 Bitbang example
 * 
 * hardware:
 *   Arduino Uno
 *   CLK: pin 9
 *   SCK: pin 13  
 *   CS:  pin 8  
 *   DATA: pin 12 
*/

#include <TLE5010.h>

#define PIN_SCK 13
#define PIN_DATA 12
#define PIN_CS 8

TLE5010_BB sensor(PIN_CS, PIN_SCK, PIN_DATA);
TLE5010_BBFast<PIN_SCK, PIN_DATA> sensorF(PIN_CS);

void setup() {

  //4 MHZ square wave on pin 9
  TCCR1A=(0<<COM1A1) | (1<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
  TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
  OCR1A=0x01;
  ICR1=0;
  pinMode(9,OUTPUT);

  //initialization
  sensor.begin();
  sensorF.begin();

  Serial.begin(9600);
}

void loop() {

  double angle,angleF;
  int t[3];

  //read angle and measure time
  t[0]=micros();
  angle=sensor.readAngleRadians();
  t[1]=micros();
  angleF=sensorF.readAngleRadians();
  t[2]=micros();
  
  Serial.print("X: ");
  Serial.print(sensor.x);
  Serial.print("/");
  Serial.print(sensorF.x);
  Serial.print(" Y: ");
  Serial.print(sensor.y);
  Serial.print("/");
  Serial.print(sensorF.y);
  Serial.print(" Angle: ");
  Serial.print(angle);
  Serial.print("/");
  Serial.print(angleF);
  Serial.print(" Time(bitbang): ");
  Serial.print(t[1]-t[0]);
  Serial.print(" Time(bitbang fast): ");
  Serial.print(t[2]-t[1]);
  Serial.println();
}