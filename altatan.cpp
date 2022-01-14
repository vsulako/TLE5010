#include "altatan.h"

double _atan2default(int16_t Y, int16_t X)
{
  return atan2(Y,X);
}

inline double _atanApproxA(double r)
{
 return (M_PI/4 + 0.273-0.273*fabs(r))*r;
}
double _atan2ApproxA(int16_t Y, int16_t X)
{
  if (abs(X)>=abs(Y))
  {
    if (X>0)
    {
        return _atanApproxA((double)Y/X);
    }
    else
    {
        if (Y>0)
            return M_PI+_atanApproxA((double)Y/X);
        else
            return -M_PI+_atanApproxA((double)Y/X);
    }
  }
  else
  {
      if (X>0)
      {
          if (Y>0)
            return M_PI/2-_atanApproxA((double)X/Y);
          else
            return -M_PI/2-_atanApproxA((double)X/Y);
      }
      else
      {
          if (Y>0)
             return M_PI/2-_atanApproxA((double)X/Y);
          else
             return -M_PI/2-_atanApproxA((double)X/Y);
      }
  }
  return 0;
}

inline double _atanApproxB(double r)
{
 return r*(M_PI/4 - (fabs(r)-1) * (0.2447 + 0.0663*fabs(r)));
}
double _atan2ApproxB(int16_t Y, int16_t X)
{
  if (abs(X)>=abs(Y))
  {
    if (X>0)
    {
        return _atanApproxB((double)Y/X);
    }
    else
    {
        if (Y>0)
            return M_PI+_atanApproxB((double)Y/X);
        else
            return -M_PI+_atanApproxB((double)Y/X);
    }
  }
  else
  {
      if (X>0)
      {
          if (Y>0)
            return M_PI/2-_atanApproxB((double)X/Y);
          else
            return -M_PI/2-_atanApproxB((double)X/Y);
      }
      else
      {
          if (Y>0)
             return M_PI/2-_atanApproxB((double)X/Y);
          else
             return -M_PI/2-_atanApproxB((double)X/Y);
      }
  }
  return 0;
}

#define CORDICFUNC(BITS, CORDIC_PI) \
double _atan2CORDIC##BITS( int16_t y, int16_t x)\
{\
    long k, tx, z=0, fl=0;\
    if (x<0) {\
        fl=((y>0)?+1:-1);\
        x=-x;\
        y=-y;\
    }\
    for (k=0; k<(BITS-2); k++)\
    {\
        tx = x;\
        if (y<=0) {\
            x -= (y>>k);\
            y += (tx>>k);\
            z -= CORDICTABLE##BITS[k];\
        }\
        else {\
            x += (y>>k);\
            y -= (tx>>k);\
            z += CORDICTABLE##BITS[k];\
        }\
    }\
    if (fl!=0) {\
        z += fl*CORDIC_PI;\
    }\
    return (double)z/(1<<(BITS-3));\
}

const uint16_t CORDICTABLE18[] = {0x6488,  0x3B59,  0x1F5B,  0x0FEB,  0x07FD,  0x0400,  0x0200,  0x0100, 0x0080,  0x0040,  0x0020,  0x0010,  0x0008,  0x0004,  0x0002,  0x0001 };
CORDICFUNC(18, 0x00019220)

const uint16_t CORDICTABLE16[] = {0x1922,  0x0ED6,  0x07D7,  0x03FB,  0x01FF,  0x0100,  0x0080,  0x0040, 0x0020,  0x0010,  0x0008,  0x0004,  0x0002,  0x0001 };
CORDICFUNC(16, 0x6488)

const uint16_t CORDICTABLE14[] = {0x0648,  0x03B6,  0x01F6,  0x00FF,  0x0080,  0x0040,  0x0020,  0x0010,  0x0008,  0x0004,  0x0002,  0x0001 };
CORDICFUNC(14, 0x1922)

const uint16_t CORDICTABLE12[] = {0x0192,  0x00ED,  0x007D,  0x0040,  0x0020,  0x0010,  0x0008,  0x0004,  0x0002,  0x0001 };
CORDICFUNC(12, 0x0648)

const uint8_t CORDICTABLE11[] = {0xC9,  0x77,  0x3F,  0x20,  0x10,  0x08,  0x04,  0x02, 0x01 };
CORDICFUNC(11, 0x0324)

const uint8_t CORDICTABLE10[] = {0x65,  0x3B,  0x1F,  0x10,  0x08,  0x04,  0x02,  0x01 };
CORDICFUNC(10, 0x0192)

const uint8_t CORDICTABLE9[] = {0x32,  0x1E,  0x10,  0x08,  0x04,  0x02,  0x01 };
CORDICFUNC(9, 0x00C9)

const uint8_t CORDICTABLE8[] = {0x19,  0x0F,  0x08,  0x04,  0x02,  0x01 };
CORDICFUNC(8, 0x0065)


int16_t _atan2IntDefault(int16_t Y, int16_t X)
{
  return 0;
}
#define CORDICINTFUNC(BITS) \
int16_t _atan2Int##BITS( int16_t y, int16_t x)\
{\
    int16_t k, tx, z=0, fl=0;\
    if (x<0) {\
        fl=((y>0)?+1:-1);\
        x=-x;\
        y=-y;\
    }\
    for (k=0; k<(BITS-1); k++)\
    {\
        tx = x;\
        if (y<=0) {\
            x -= (y>>k);\
            y += (tx>>k);\
            z -= CORDICINTTABLE##BITS[k];\
        }\
        else {\
            x += (y>>k);\
            y -= (tx>>k);\
            z += CORDICINTTABLE##BITS[k];\
        }\
    }\
    if (fl!=0) {\
		z += fl*(1<<(BITS-1));\
    }\
    return z;\
}

const uint16_t CORDICINTTABLE16[] = {0x2000, 0x12E4, 0x09FB, 0x0511, 0x028B, 0x0146, 0x00A3, 0x0051, 0x0029, 0x0014, 0x000A, 0x0005, 0x0003, 0x0001, 0x0001 };
CORDICINTFUNC(16)

const uint16_t CORDICINTTABLE15[] = { 0x1000, 0x0972, 0x04FE, 0x0289, 0x0146, 0x00A3, 0x0051, 0x0029,  0x0014, 0x000A, 0x0005, 0x0003, 0x0001, 0x0001 };
CORDICINTFUNC(15)

const uint16_t CORDICINTTABLE14[] = { 0x00800, 0x04B9, 0x027F, 0x0144, 0x00A3, 0x0051, 0x0029, 0x0014,  0x0000A, 0x0005, 0x0003, 0x0001, 0x0001 };
CORDICINTFUNC(14)

const uint16_t CORDICINTTABLE13[] = {0x0400, 0x025D, 0x013F, 0x00A2, 0x0051, 0x0029, 0x0014, 0x000A, 0x0005, 0x0003, 0x0001, 0x0001 };
CORDICINTFUNC(13)
 
const uint16_t CORDICINTTABLE12[] = {0x0200, 0x012E, 0x00A0, 0x0051, 0x0029, 0x0014, 0x000A, 0x0005,  0x0003, 0x0001, 0x0001 };
CORDICINTFUNC(12)

const uint16_t CORDICINTTABLE11[] = {0x0100, 0x0097, 0x0050, 0x0029, 0x0014, 0x000A, 0x0005, 0x0003, 0x0001, 0x0001 };
CORDICINTFUNC(11)

const uint8_t CORDICINTTABLE10[] = {0x80, 0x4C, 0x28, 0x14, 0x0A, 0x05, 0x03, 0x01, 0x01 };
CORDICINTFUNC(10)

const uint8_t CORDICINTTABLE9[] = {0x40, 0x26, 0x14, 0x0A, 0x05, 0x03, 0x01, 0x01 };
CORDICINTFUNC(9)

const uint8_t CORDICINTTABLE8[] = {0x20, 0x13, 0x0A, 0x05, 0x03, 0x01, 0x01 };
CORDICINTFUNC(8)
