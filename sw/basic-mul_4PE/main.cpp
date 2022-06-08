#include "string"
#include "string.h"
#include "cassert"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include <iomanip>
#include <iostream>
using std::cerr;
using std::endl;
#include <cstdio>
#include <cstdlib>
using namespace std;
#include <fstream>
using std::ofstream;
//#include "Testbench.h"

// FFT ACC
static char* const FFT0_START_ADDR = reinterpret_cast<char* const>(0x73000000);
static char* const FFT0_READ_ADDR  = reinterpret_cast<char* const>(0x73000004);
static char* const FFT1_START_ADDR = reinterpret_cast<char* const>(0x83000000);
static char* const FFT1_READ_ADDR  = reinterpret_cast<char* const>(0x83000004);
static char* const FFT2_START_ADDR = reinterpret_cast<char* const>(0x93000000);
static char* const FFT2_READ_ADDR  = reinterpret_cast<char* const>(0x93000004);
static char* const FFT3_START_ADDR = reinterpret_cast<char* const>(0xA3000000);
static char* const FFT3_READ_ADDR  = reinterpret_cast<char* const>(0xA3000004);

// DMA 
static volatile uint32_t * const DMA_SRC_ADDR  = (uint32_t * const)0x70000000;
static volatile uint32_t * const DMA_DST_ADDR  = (uint32_t * const)0x70000004;
static volatile uint32_t * const DMA_LEN_ADDR  = (uint32_t * const)0x70000008;
static volatile uint32_t * const DMA_OP_ADDR   = (uint32_t * const)0x7000000C;
static volatile uint32_t * const DMA_STAT_ADDR = (uint32_t * const)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

unsigned int nbr_vect = 8;


bool _is_using_dma = true;

const signed char a[16][16] ={
                {1, 0, 4, 2, -6, 4, -3, -2, 8, 1, 2, -4, -6, 3, -3, 0},
                {3, 4, 0, -2, 1, 1, 0, 0, 0, 1, 2, -1, 4, 1, 3, 2},
                {6, 0, 1, 1, -1, -2, 4, 1, 0, 0, -2, -2, 8, 1, -3, 1},
                {-5, -4, 4, 0, 0, 4, -1, 0, -8, 1, 0, 0, -1, 1, -8, 1},
                {-3, 1, 2, 2, -2, -4, -3, -1, 2, 1, 0, 0, -5, 1, 1, 0},
                {-2, 2, 4, 2, -6, 4, -1, -1, 0, 1, 2, 0, 0, 1, -5, -4},
                {6,  -2, 4, 1, 0, 0, -2,  1, 0, 0, -1, 3, 1, 0, 1, -2},
                {0, 1, 1, -1, -2, 4, -3, -2, 8, 1, 2, -1, -1, 0, 1, -4},
                {5, 0, 4, 2, -6, 4, -3, -1, -1, 0, 1, -4, -6, 3, 0, -1},
                {6, 0, 4, 2,  8, 1, 2, -4, -6, -6, 4, -3, -2, 3, 0, -7},
                {8, 0, 4, 2, -6, 4, -3, -2, 8, 1, 2, -4, -6, 3, -2, -4},
                {9, -3, 1, 2, 2, -2, -4, -3, -1, 1, 2, -4, -6, 3, -3, 1},
                {-8, 1, 0, 0, -1, 1, -8, 1, -5, -4, 4, 0, 0, 4, -1, 0},
                {-1, 0, 4, 2, -6, 4, -3, -2, 8, 1, 2, -4, -6, 3, 1, 3},
                {0, 4, 2, 0, -6,  2, -4, -6, 3, 0, 4, -3, -2, 8, 1, 4},
                {4, 0, 4, 2, -6, 4, -3, -2, 8, 1, 2, -4, -6, 3, 3, 0}};


const signed char b[16][16] = {
                {8, 1, 2, -4, -6, 3, -3, 0, 1, 0, 4, 2, -6, 4, -3, -2},
                {3, 4, 0, -2, 4, 1, 3, 2, 1, 1, 0, 0, 0, 1, 2, -1},
                {6, -2, 8, 1,  0, 1, 1, -1, -2, 4, 1, 0, 0, -2, -3, 1},
                {-5, -4, 4, 0, 0, 4, -1, 0, -8, 1, 0, 0, -1, 1, -8, 1},
                {-3, 1, 2, -1, 2, 1, 0, 0, -5, 1, 1, 0, 2, -2, -4, -3},
                {-2, 2, 4, 2, -6, 4, -1, -1, 0, 1, 2, 0, 0, 1, -5, -4},
                {6,  -2, 1, 0, 4, 1, 0, 0, -2,  1, 0, 0, -1, 3, 1, -2},
                {0, 1, 1, -1, -2, 4, -3, -2, 8, 1, 2, -1, -1, 0, 1, -4},
                {5,  0, -1, 0, 4, 2, -6, 4, -3, -1, -1, 0, 1, -4, -6, 3},
                { -4, -6, -6, 4, -3, -2, 6, 0, 4, 2,  8, 1, 2, 3, 0, -7},
                {8, 0, 4, 2, -6, 4, -3, -2, 8, 1, 2, -4, -6, 3, -2, -4},
                {9, -3,  2, -4, -6, 3, -3, 1, 2, 2, -2, -4, -3, -1, 1, 1},
                {-8, 1, 0, 0, -1, 1, -8, 1, -5, -4, 4, 0, 0, 4, -1, 0},
                {-6, 4, -3, -2, 8, 1, 2, -4, -6, 3, -1, 0, 4, 2, 1, 3},
                { -6,  2, -4, 0, 4, 2, 0, -6, -2, 8, 3, 0, 4, -3,  1, 4},
                { 2, -4, -6, 4, 0, 4, 2, -6, 4, -3, -2, 8, 1, 3, 3, 0}};
               


//Pair data;
union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};




void write_data_to_ACC(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){  
    // Using DMA 
    *DMA_SRC_ADDR = (uint32_t)(buffer);
    *DMA_DST_ADDR = (uint32_t)(ADDR);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR  = DMA_OP_MEMCPY;
  }else{
    // Directly Send
    memcpy(ADDR, buffer, sizeof(unsigned char)*len);
  }
}

void read_data_from_ACC(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){
    // Using DMA 
    *DMA_SRC_ADDR = (uint32_t)(ADDR);
    *DMA_DST_ADDR = (uint32_t)(buffer);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR  = DMA_OP_MEMCPY;
  }else{
    // Directly Read
    memcpy(buffer, ADDR, sizeof(unsigned char)*len);
  }
}


int main(int argc, char *argv[]) {

  //read_bmp("lena_std_short.bmp");
  printf("======================================\n");
  printf("\t  Reading from array\n");
  printf("======================================\n");
	printf(" Vector lenght\t\t\t\t= %d\n", nbr_vect);
	
	printf(" bytes_per_entry\t\t= %d\n",sizeof(char)/8);
  printf("======================================\n");

  unsigned char buffer[4] = {0};
  unsigned char buffer2[4] = {0};
 unsigned char buffer1[4] = {0};
 unsigned char buffer3[4] = {0}; 
   word data0;
   word data2;
   word data1;
   word data3;
  int total0, total1, total2, total3;
  printf("Start processing...\n");
 
 
  int c[16][16];
  //no parallel
  /*
for(int i = 0; i<16;i = i+4)
    {
      for(int j = 0; j< 16;j++)
      {
        for(int u = 0; u<16; u++)
        {
          
            buffer[0] = a[i][u];
            buffer[1] = b[u][j];
            buffer[3] = 0;
            buffer[2] = 0;

            buffer1[0] = a[i+1][u];
            buffer1[1] = b[u][j];
            buffer1[3] = 0;
            buffer1[2] = 0;

            buffer2[0] = a[i+2][u];
            buffer2[1] = b[u][j];
            buffer2[3] = 0;
            buffer2[2] = 0;

            buffer3[0] = a[i+3][u];
            buffer3[1] = b[u][j];
            buffer3[3] = 0;
            buffer3[2] = 0;
        
            write_data_to_ACC(FFT0_START_ADDR, buffer, 4);
            write_data_to_ACC(FFT1_START_ADDR, buffer1, 4);
            write_data_to_ACC(FFT2_START_ADDR, buffer2, 4);
            write_data_to_ACC(FFT3_START_ADDR, buffer3, 4);
        }
        //printf("\n");
        read_data_from_ACC(FFT0_READ_ADDR, buffer, 4);
        read_data_from_ACC(FFT1_READ_ADDR, buffer1, 4);
        read_data_from_ACC(FFT2_READ_ADDR, buffer2, 4);
        read_data_from_ACC(FFT3_READ_ADDR, buffer3, 4);

        memcpy(data0.uc, buffer, 4);
        memcpy(data1.uc, buffer1, 4);
        memcpy(data2.uc, buffer2, 4);
        memcpy(data3.uc, buffer3, 4);
        total0 = (data0).sint;
        total1 = (data1).sint;
        total2 = (data2).sint;
        total3 = (data3).sint;
        
        c[i][j] = total0;
        c[i+1][j] = total1;
        c[i+2][j] = total2;
        c[i+3][j] = total3;
        
      }
    }

*/
//parallel
for(int i = 0; i<16;i = i+4)
    {
      for(int j = 0; j< 16;j++)
      {
        for(int u = 0; u<8; u++)
        {
          
            buffer[0] = a[i][u];
            buffer[1] = b[u][j];
            buffer[3] = a[i][u+8];
            buffer[2] = b[u+8][j];

            buffer1[0] = a[i+1][u];
            buffer1[1] = b[u][j];
           buffer1[3] = a[i+1][u+8];
            buffer1[2] = b[u+8][j];

            buffer2[0] = a[i+2][u];
            buffer2[1] = b[u][j];
            buffer2[3] = a[i+2][u+8];
            buffer2[2] = b[u+8][j];

            buffer3[0] = a[i+3][u];
            buffer3[1] = b[u][j];
            buffer3[3] = a[i+3][u+8];
            buffer3[2] = b[u+8][j];
        
            write_data_to_ACC(FFT0_START_ADDR, buffer, 4);
            write_data_to_ACC(FFT1_START_ADDR, buffer1, 4);
            write_data_to_ACC(FFT2_START_ADDR, buffer2, 4);
            write_data_to_ACC(FFT3_START_ADDR, buffer3, 4);
        }
        //printf("\n");
        read_data_from_ACC(FFT0_READ_ADDR, buffer, 4);
        read_data_from_ACC(FFT1_READ_ADDR, buffer1, 4);
        read_data_from_ACC(FFT2_READ_ADDR, buffer2, 4);
        read_data_from_ACC(FFT3_READ_ADDR, buffer3, 4);

        memcpy(data0.uc, buffer, 4);
        memcpy(data1.uc, buffer1, 4);
        memcpy(data2.uc, buffer2, 4);
        memcpy(data3.uc, buffer3, 4);
        total0 = (data0).sint;
        total1 = (data1).sint;
        total2 = (data2).sint;
        total3 = (data3).sint;
        
        c[i][j] = total0;
        c[i+1][j] = total1;
        c[i+2][j] = total2;
        c[i+3][j] = total3;
        
      }
    }
  for(int i =0;i <16;i++)
  {
    for(int j=0;j<16;j++)
    {
      printf("%d ", c[i][j]);
    }
    printf("\n");
  }
 // output("out.txt");
 // write_bmp("lena_std_out.bmp");
}
