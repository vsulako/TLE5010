#ifndef TLE5010_H
#define TLE5010_H

#include <Arduino.h>
#include <SPI.h>
#include "altatan.h"
#include "fastIO.h"

//#define TLE5010_TEST_MODE

//Base class, do not use
class TLE5010
{
  public:
    int16_t x;
    int16_t y;
    TLE5010();
    virtual void readXY(){};
    atan2Function atan2Func;
    atan2FunctionInteger atan2FuncInt;
    double readAngleRadians();
    double readAngleDegrees();
    int16_t readInteger();
};

//SPI
class TLE5010_SPI: public TLE5010
{
  private:
    volatile uint8_t* csO;
    uint8_t  csBM;
    uint8_t  csIBM;
  public:
    TLE5010_SPI(uint8_t _cs);
    void begin(atan2Function _atan2Func=_atan2default);
    void readXY();
    void setCS(uint8_t _cs);
};

//Bitbang 
class TLE5010_BB: public TLE5010
{
  private:
    volatile uint8_t* csO;
    uint8_t  csBM;
    uint8_t  csIBM;
    
    volatile uint8_t* sckI;
    volatile uint8_t* sckO;
    uint8_t  sckBM;
    uint8_t  sckIBM;
  
    volatile uint8_t* dataO;
    volatile uint8_t* dataI;
    volatile uint8_t* dataPM;
    uint8_t  dataBM;
    uint8_t  dataIBM;

  public:
    TLE5010_BB(uint8_t _cs, uint8_t _sck, uint8_t _data);
    void begin(atan2Function _atan2Func=_atan2default);
    void readXY(/*const bool testmode=false*/);
    void setCS(uint8_t _cs);
};

//Fast bitbang mode
#define __TLE5010_BBF_DELAY __builtin_avr_delay_cycles(3);

template <uint8_t __PIN_SCK, uint8_t __PIN_DATA, bool __EXT_PULLUP=false>
class TLE5010_BBFast: public TLE5010
{
   private:
    volatile uint8_t* csO;
    uint8_t  csBM;
    uint8_t  csIBM;
   public:
    TLE5010_BBFast(uint8_t _cs)
    {
      //using bitmask as temporary variable for pinCS until begin()
      csBM=_cs; 
    };
    void begin(atan2Function _atan2Func=_atan2default)
    {
       atan2Func=_atan2Func;
       setCS(csBM);

       pinModeF(__PIN_SCK, OUTPUT);
    };
    void setCS(uint8_t _cs)
    {
      //caching CS PORT/DDR for faster IO
      csO=portOutputRegister(digitalPinToPort(_cs));
      csBM=digitalPinToBitMask(_cs);
      csIBM=~csBM;
    
       *csO |= csBM;  //CS high
       
       *(portModeRegister(digitalPinToPort(_cs)))|=csBM;  //=pinMode(cs, OUTPUT)
    };
    
    void readXY()
    {
      uint8_t i,j;
      uint8_t tmp;

      digitalWriteF(__PIN_SCK, 0);

      digitalWriteF(__PIN_DATA, 0);
      pinModeF(__PIN_DATA, OUTPUT);
      
      *csO &= csIBM;  //put CS low
            
      //update command - 0x00;
      digitalWriteF(__PIN_SCK, 1);
      for(i=0;i<7;i++)
      {
        digitalWriteF(__PIN_SCK, 0);
        digitalWriteF(__PIN_SCK, 1);
      }
      digitalWriteF(__PIN_SCK, 0);

      #ifndef __TLE5010_TEST_MODE
          //command for reading 4 bytes X&Y
          //10001100 = 1(read) 0001(register #0001) 100 (4 bytes)
          if (__EXT_PULLUP)
            {
                pinModeF(__PIN_DATA, INPUT_ANY); 
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                pinModeF(__PIN_DATA, OUTPUT);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                pinModeF(__PIN_DATA, INPUT_ANY);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                pinModeF(__PIN_DATA, OUTPUT);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
        
                //prepare for reading
                pinModeF(__PIN_DATA, INPUT_ANY);
            }
            else
            {
                pinModeF(__PIN_DATA, INPUT_PULLUP);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                digitalWrite(__PIN_DATA,0);pinModeF(__PIN_DATA, OUTPUT);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                pinModeF(__PIN_DATA, INPUT_PULLUP);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                digitalWrite(__PIN_DATA,0);pinModeF(__PIN_DATA, OUTPUT);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                //prepare for reading
                pinModeF(__PIN_DATA, INPUT_PULLUP);
            }
      #else
            //testmode: command for reading reserved bytes (expected ff 00 00 00)
            //11000100 = 1(read) 1000(register #1000) 100 (4 bytes)
            if (__EXT_PULLUP)
            {
                pinModeF(__PIN_DATA, INPUT_ANY); 
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                pinModeF(__PIN_DATA, OUTPUT);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                pinModeF(__PIN_DATA, INPUT_ANY);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                pinModeF(__PIN_DATA, OUTPUT);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
        
                //prepare for reading
                pinModeF(__PIN_DATA, INPUT_ANY);
            }
            else
            {
                pinModeF(__PIN_DATA, INPUT_PULLUP);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                digitalWrite(__PIN_DATA,0);pinModeF(__PIN_DATA, OUTPUT);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                pinModeF(__PIN_DATA, INPUT_PULLUP);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                digitalWrite(__PIN_DATA,0);pinModeF(__PIN_DATA, OUTPUT);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                digitalWriteF(__PIN_SCK, 1);__TLE5010_BBF_DELAY;digitalWriteF(__PIN_SCK, 0);
                
                //prepare for reading
                pinModeF(__PIN_DATA, INPUT_PULLUP);
            }
      #endif

      //read  
      digitalWriteF(__PIN_SCK, 1);
      j=4;
      do
      {
        //read byte
        tmp=0;
        i=0x80;
        do
        {   
            digitalWriteF(__PIN_SCK, 0);
            digitalWriteF(__PIN_SCK, 1);
            
            if (digitalReadF(__PIN_DATA)) 
              tmp|=i;
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

      digitalWriteF(__PIN_SCK, 0);
      
      
      *csO |= csBM;   //CS to HIGH
    };
};
#undef __TLE5010_BBF_DELAY



//Bitbang slow
class TLE5010_BBSlow: public TLE5010
{
  private:
    uint8_t  pinCS;
    uint8_t  pinSCK;
    uint8_t  pinDATA;
  public:
    TLE5010_BBSlow(uint8_t _cs, uint8_t _sck, uint8_t _data);
    void begin(atan2Function _atan2Func=_atan2default);
    void readXY();
    void setCS(uint8_t _cs);
};

#endif
