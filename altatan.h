/* 
 *  alternate atan2 algorithms 
 *   Sources: 
 *   http://www-labs.iro.umontreal.ca/~mignotte/IFT2425/Documents/EfficientApproximationArctgFunction.pdf
 *   https://www.st.com/resource/en/design_tip/dm00441302-coordinate-rotation-digital-computer-algorithm-cordic-to-compute-trigonometric-and-hyperbolic-functions-stmicroelectronics.pdf
*/

#ifndef ALTATAN_H
#define ALTATAN_H
#include <Arduino.h>

typedef double (*atan2Function) (int16_t y, int16_t x);
typedef int16_t (*atan2FunctionInteger) (int16_t y, int16_t x);

double _atan2default(int16_t Y, int16_t X);
double _atan2ApproxA(int16_t Y, int16_t X);
double _atan2ApproxB(int16_t Y, int16_t X);
double _atan2CORDIC18(int16_t Y, int16_t X);
double _atan2CORDIC16(int16_t Y, int16_t X);
double _atan2CORDIC14(int16_t Y, int16_t X);
double _atan2CORDIC12(int16_t Y, int16_t X);
double _atan2CORDIC11(int16_t Y, int16_t X);
double _atan2CORDIC10(int16_t Y, int16_t X);
double _atan2CORDIC9(int16_t Y, int16_t X);
double _atan2CORDIC8(int16_t Y, int16_t X);

int16_t _atan2IntDefault(int16_t Y, int16_t X);
int16_t _atan2Int16(int16_t Y, int16_t X);
int16_t _atan2Int15(int16_t Y, int16_t X);
int16_t _atan2Int14(int16_t Y, int16_t X);
int16_t _atan2Int13(int16_t Y, int16_t X);
int16_t _atan2Int12(int16_t Y, int16_t X);
int16_t _atan2Int11(int16_t Y, int16_t X);
int16_t _atan2Int10(int16_t Y, int16_t X);
int16_t _atan2Int9(int16_t Y, int16_t X);
int16_t _atan2Int8(int16_t Y, int16_t X);

#endif
