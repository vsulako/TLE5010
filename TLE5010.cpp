#include "TLE5010.h"

//macro for port manipulation
#define __TLE5010_SCK_LOW       {*sckO &= sckIBM;}
#define __TLE5010_SCK_TOGGLE    {*sckI = sckBM;}
#define __TLE5010_SCK_PULSE     {__TLE5010_SCK_TOGGLE;__TLE5010_SCK_TOGGLE;}

#define __TLE5010_DATA_INPUT    {*dataPM &= dataIBM;*dataO|=dataBM;}
#define __TLE5010_DATA_OUTPUT   {*dataO&=dataIBM;*dataPM |= dataBM;}

#define __TLE5010_DATA_LOW      {*dataO &= dataIBM;}
#define __TLE5010_IS_DATA_HIGH  (*dataI & dataBM)


//Base class
TLE5010::TLE5010()
{
  atan2FuncInt=_atan2IntDefault;
}
double TLE5010::readAngleRadians()
{
  readXY();
  return atan2Func(y, x);
}
double TLE5010::readAngleDegrees()
{
  return readAngleRadians()*(180/PI);
}
int16_t TLE5010::readInteger()
{
  readXY();
  return atan2FuncInt(y, x);
}


/* SPI mode */
TLE5010_SPI::TLE5010_SPI(uint8_t _cs)
{
  //using bitmask as temporary variable for pinCS until begin()
  csBM=_cs; 
}
void TLE5010_SPI::begin(atan2Function _atan2Func)
{
   atan2Func=_atan2Func;
   setCS(csBM);   
   SPI.begin();
}
void TLE5010_SPI::setCS(uint8_t _cs)
{
  //caching CS PORT/DDR for faster IO
  csO=portOutputRegister(digitalPinToPort(_cs));
  csBM=digitalPinToBitMask(_cs);
  csIBM=~csBM;

   *csO |= csBM;  //CS high
   *(portModeRegister(digitalPinToPort(_cs)))|=csBM;  //=pinMode(_cs, OUTPUT);
}

void TLE5010_SPI::readXY()
{
  SPI.beginTransaction(SPISettings(3000000, MSBFIRST, SPI_MODE1));	//speed 

  *csO &= csIBM;  //CS low

  #ifndef __TLE5010_TEST_MODE
    //command for reading 4 bytes X&Y
    //10001100 = 1(read) 0001(register #1) 100 (4 bytes)
    SPI.transfer16(0x008c);
  #else
    //testmode: command for reading 4 reserved bytes (expected FF 00 00 00)
    //11000100 = 1(read) 1000(register #8) 100 (4 bytes)
    SPI.transfer16(0x00c4);
  #endif

  //reading bytes

  ((uint8_t*)&x)[0]=SPI.transfer(0xff);
  ((uint8_t*)&x)[1]=SPI.transfer(0xff);
  ((uint8_t*)&y)[0]=SPI.transfer(0xff);
  ((uint8_t*)&y)[1]=SPI.transfer(0xff);

  *csO |= csBM; //CS high
  
  SPI.endTransaction();  
}

/* Bitbang mode  */
TLE5010_BB::TLE5010_BB(uint8_t _cs, uint8_t _sck, uint8_t _data)
{
  sckBM=_sck;
  csBM=_cs;
  dataBM=_data;
}
void TLE5010_BB::begin(atan2Function _atan2Func)
{
   atan2Func=_atan2Func;

   pinMode(sckBM, OUTPUT);
   
   //caching PORT/PIN/DDR registers
   sckI=portInputRegister(digitalPinToPort(sckBM));
   sckO=portOutputRegister(digitalPinToPort(sckBM));
   sckBM=digitalPinToBitMask(sckBM);
   sckIBM=~sckBM;

   dataO=portOutputRegister(digitalPinToPort(dataBM));
   dataI=portInputRegister(digitalPinToPort(dataBM));
   dataPM=portModeRegister(digitalPinToPort(dataBM));
   dataBM=digitalPinToBitMask(dataBM);
   dataIBM=~dataBM;

   setCS(csBM);

   __TLE5010_DATA_INPUT;
}
void TLE5010_BB::setCS(uint8_t _cs)
{
   csO=portOutputRegister(digitalPinToPort(_cs));
   csBM=digitalPinToBitMask(_cs);
   csIBM=~csBM;

   *csO |= csBM; //CS high
   *(portModeRegister(digitalPinToPort(_cs)))|=csBM;    //=pinMode(_cs, OUTPUT);
}
void TLE5010_BB::readXY()
{
  __TLE5010_SCK_LOW;

  __TLE5010_DATA_OUTPUT;
  
    *csO &= csIBM;  //CS low

    //update command - 0x00
    __TLE5010_SCK_PULSE;
    __TLE5010_SCK_PULSE;
    __TLE5010_SCK_PULSE;
    __TLE5010_SCK_PULSE;
    __TLE5010_SCK_PULSE;
    __TLE5010_SCK_PULSE;
    __TLE5010_SCK_PULSE;
    __TLE5010_SCK_PULSE;

  #ifndef __TLE5010_TEST_MODE
  {
    //command for reading 4 bytes X&Y
    //10001100 = 1(read) 0001(register #0001) 100 (4 bytes)
    __TLE5010_DATA_INPUT; __TLE5010_SCK_PULSE;
    __TLE5010_DATA_OUTPUT;__TLE5010_SCK_PULSE;__TLE5010_SCK_PULSE;__TLE5010_SCK_PULSE;
    __TLE5010_DATA_INPUT; __TLE5010_SCK_PULSE;__TLE5010_SCK_PULSE;
    __TLE5010_DATA_OUTPUT;__TLE5010_SCK_PULSE;__TLE5010_SCK_PULSE;
  }
  #else
  {
    //testmode: command for reading 4 reserverd bytes (expected FF 00 00 00)
    //11000100 = 1(read) 1000(register #8) 100 (4 bytes)
    __TLE5010_DATA_INPUT; __TLE5010_SCK_PULSE;__TLE5010_SCK_PULSE;
    __TLE5010_DATA_OUTPUT;__TLE5010_SCK_PULSE;__TLE5010_SCK_PULSE;__TLE5010_SCK_PULSE;
    __TLE5010_DATA_INPUT; __TLE5010_SCK_PULSE;
    __TLE5010_DATA_OUTPUT;__TLE5010_SCK_PULSE;__TLE5010_SCK_PULSE;
  }
  #endif
  
  //reading answer
  __TLE5010_DATA_INPUT;
 
	uint8_t i,j;
	uint8_t tmp;

	j=4;
	do
	{
		//read byte
		tmp=0;
		i=0x80;
		do
		{
		    __TLE5010_SCK_PULSE;
		    if (__TLE5010_IS_DATA_HIGH) tmp|=i;
		}
		while(i>>=1);

		//output
		switch(j)
		{
			case 4:
          ((uint8_t*)&x)[0]=tmp;
				break;
			case 3:
          ((uint8_t*)&x)[1]=tmp;
				break;
			case 2:
          ((uint8_t*)&y)[0]=tmp;
				break;
			case 1:
          ((uint8_t*)&y)[1]=tmp;
				break;
		}
    
		j--;
	}while(j);

  
  *csO |= csBM; //CS high
}

/* Bitbang mode with default digitalWrite - for reference */
TLE5010_BBSlow::TLE5010_BBSlow(uint8_t _cs, uint8_t _sck, uint8_t _data)
{
   pinSCK=_sck;
   pinDATA=_data;
   pinCS=_cs;
}
void TLE5010_BBSlow::begin(atan2Function _atan2Func)
{
   atan2Func=_atan2Func;
   
   pinMode(pinSCK, OUTPUT);
   pinMode(pinDATA, INPUT_PULLUP);

   setCS(pinCS);
   
   digitalWrite(pinSCK, 0);
}

void TLE5010_BBSlow::setCS(uint8_t _cs)
{
  pinCS=_cs;
  pinMode(pinCS, OUTPUT);
  digitalWrite(pinCS, 1);
}
void TLE5010_BBSlow::readXY()
{
  
  uint8_t i,cmd;
  uint8_t data[4];

  x=0;
  y=0;

  digitalWrite(pinDATA, 0);
  pinMode(pinDATA, OUTPUT);

  //enable transfer
  digitalWrite(pinCS, 0);
  
  //send update command - 0x00
  for(i=0;i<8;i++)
  {
    digitalWrite(pinSCK, 1);
    digitalWrite(pinSCK, 0);
  }
  
  //command for reading 4 bytes X&Y - 0x8C
  cmd=0x8c;

  //send command
  i=0b10000000;
  do
  { 
     //To simulate OpenDrain, data pin is set in hiZ pullup mode to send '1' and output mode low to send '0'.
     if (cmd & i)
        pinMode(pinDATA, INPUT_PULLUP);
     else
     {
        digitalWrite(pinDATA, 0);
        pinMode(pinDATA, OUTPUT);
     }

     digitalWrite(pinSCK, 1);
     digitalWrite(pinSCK, 0);
     
     i=i>>1;
  }while(i);

  //reading answer
  pinMode(pinDATA, INPUT_PULLUP);
  for(i=0;i<4;i++)
  {
    data[i]=shiftIn(pinDATA, pinSCK, MSBFIRST);
  }

  digitalWrite(pinCS, 1);

  x=(data[1]<<8) | data[0];
  y=(data[3]<<8) | data[2];
}

//clean up macro
#undef __TLE5010_SCK_LOW
#undef __TLE5010_SCK_TOGGLE
#undef __TLE5010_SCK_PULSE

#undef __TLE5010_DATA_INPUT
#undef __TLE5010_DATA_OUTPUT

#undef __TLE5010_DATA_LOW
#undef __TLE5010_IS_DATA_HIGH
