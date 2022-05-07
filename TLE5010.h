#ifndef TLE5010_H
#define TLE5010_H

#include <Arduino.h>
#include <SPI.h>
#include "altatan.h"

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
	double readAngleRadians(atan2Function _atan2Func);
    double readAngleDegrees(atan2Function _atan2Func);
    int16_t readInteger(atan2FunctionInteger _atan2FuncInt);
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
    void readXY();
    void setCS(uint8_t _cs);
};

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
