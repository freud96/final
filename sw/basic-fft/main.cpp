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
#include <time.h>

// FFT ACC
static char* const FFT_START_ADDR = reinterpret_cast<char* const>(0x73000000);
static char* const FFT_READ_ADDR  = reinterpret_cast<char* const>(0x73000004);

// DMA 
static volatile uint32_t * const DMA_SRC_ADDR  = (uint32_t * const)0x70000000;
static volatile uint32_t * const DMA_DST_ADDR  = (uint32_t * const)0x70000004;
static volatile uint32_t * const DMA_LEN_ADDR  = (uint32_t * const)0x70000008;
static volatile uint32_t * const DMA_OP_ADDR   = (uint32_t * const)0x7000000C;
static volatile uint32_t * const DMA_STAT_ADDR = (uint32_t * const)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

unsigned int nbr_vect = 8;


bool _is_using_dma = true;
class Complex{
  public:
    int real;
    int imag;
    //void bit_reverse();
   // unsigned char reverse_bits(unsigned char input);
   // void generate_vector();
};


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

  /*             
void generate_matrix(signed char matrix_a[][16], signed char matrix_b[][16])
{
    

    //randomly generated vector(value from 0 to 15), only real value will be issued
    
        int k =1;
        for(int i=0;i<16;i++)
        {
          for(int j = 0; j<16; j++)
          {
            matrix_a[i][j] = (rand()%11) - 5;
            printf(" %d ", matrix_a[i][j]);
            matrix_b[i][j] = (rand()%11) -5;
          }
            
        }

        printf(" \n\n\n");
          
}
*/

 /*
void output(string outfile_name)
{
    ofstream outdata;
    outdata.open(outfile_name); // opens the file
    if( !outdata )   // file couldn't be opened
    {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    cout<<"\nVector input "<<endl;
    for(int i=0; i<8;i++)
    {
        cout<<in[i].real<<" "<<in[i].imag<<" ";
    }

    cout<<"\nOutput after computation "<<endl;
    for(int i=0; i<8;i++)
    {
        cout<<GM[i].real<<" "<<GM[i].imag<<" ";
    }
    //for(int i=0; i<MASK_N;i++)
   // {
      //  cout<<vec_out[0][i]<<" ";
   // }

    cout<<endl;

    outdata<<"vector input\t\t\t\t\t\t\t\t\t"<<"DFT vector"<<endl;
    //for(int j =0;j<MAX_nbr_vector;j++)
   // {
   //     //cout<<res[i]<<endl;
    //    outdata<<"("<<vec[j][0]<<", "<<vec[j][1]<<", "<<vec[j][2]<<", "<<vec[j][3]<<", "<<vec[j][4]<<", "<<vec[j][5]<<", "<<vec[j][6]<<", "<<vec[j][7]<<")\t\t\t";
    //    outdata<<"("<<vec_out[j][0]<<", "<<vec_out[j][1]<<", "<<vec_out[j][2]<<", "<<vec_out[j][3]<<", "<<vec_out[j][4]<<", "<<vec_out[j][5]<<", "<<vec_out[j][6]<<", "<<vec_out[j][7]<<")"<<endl;
    //    outdata<<endl;
   // }
}

*/

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
   word data;
  int total;
  printf("Start processing...\n");
 

  int c[16][16];
 
 //no parallelism
/*
for(int i = 0; i<16;i++)
    {
      for(int j = 0; j< 16;j++)
      {
        for(int u = 0; u<16; u++)
        {
          
            buffer[0] = a[i][u];
            buffer[1] = b[u][j];
            buffer[3] = 0;
            buffer[2] = 0;
         //   printf("Sending sample data ")
            write_data_to_ACC(FFT_START_ADDR, buffer, 4);
        }
        //printf("\n");
        read_data_from_ACC(FFT_READ_ADDR, buffer, 4);
        memcpy(data.uc, buffer, 4);
        total = (data).sint;
        
        c[i][j] = total;
        
      }
    }
*/


//row parallelism
for(int i = 0; i<16;i++)
    {
      for(int j = 0; j< 16;j++)
      {
        for(int u = 0; u<8; u++)
        {
          
            buffer[0] = a[i][u];
            buffer[1] = b[u][j];
            buffer[2] = a[i][u+8];
            buffer[3] = b[u+8][j];
         //   printf("Sending sample data ")
            write_data_to_ACC(FFT_START_ADDR, buffer, 4);
        }
        //printf("\n");
        read_data_from_ACC(FFT_READ_ADDR, buffer, 4);
        memcpy(data.uc, buffer, 4);
        total = (data).sint;
        
        c[i][j] = total;
        
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
