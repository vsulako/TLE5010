/*
 * TLE5010 Multiturn sensor example
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
 *   
 * Example of multiturn angle sensor.
*/

#include <TLE5010.h>

#define PIN_CS 8

TLE5010_SPI sensor(PIN_CS);


class MultiTurn
{
    private:
      TLE5010* sensor;
      int turns;
      double prevVal;
      bool first=true;
    public:
      MultiTurn(TLE5010* _sensor);
      double getValue();
};

MultiTurn::MultiTurn(TLE5010* _sensor)
{
  sensor=_sensor;
}

double MultiTurn::getValue()
{
  double value=sensor->readAngleRadians();
  
  //Assuming that rotation speed is not faster than 1/4 turn in time between readings
  
  //Detect crossing bounds and increase/decrease turn count accordingly
  if ((prevVal>PI/2) && (value<-PI/2))
    turns++;
  if ((value>PI/2) && (prevVal<-PI/2))
    turns--;
  prevVal=value; 
     
  if (first)    
  {
    turns=0;
    first=false;
  }
  return turns*2*PI+value;
}


MultiTurn MTSensor(&sensor);


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

  //read angle
  angle=MTSensor.getValue();
  
  Serial.println(
          " Angle: "+String(angle)
          );
}
