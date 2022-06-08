#ifndef DFT_MATRIX_H_
#define DFT_MATRIX_H_

#include <complex>
using namespace std;

#include <cmath>
#define CLOCK_PERIOD 10
const int MASK_N = 5;
//const int MASK_N = 5;
const int DMA_TRANS = 64;
const int MUL_R_ADDR = 0x00000000;
const int MUL_RESULT_ADDR = 0x00000004;
const int DFT_CHECK_ADDR = 0x00000008;


union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};



#endif