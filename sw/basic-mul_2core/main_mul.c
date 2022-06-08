//#include "string"
#include "string.h"
//#include "cassert"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include <stdbool.h> 
//#include <iomanip>
//#include <iostream>
//using std::cerr;
//using std::endl;
//#include <cstdio>
//#include <cstdlib>
///using namespace std;
//#include <fstream>
//using std::ofstream;
//#include "Testbench.h"


// FFT ACC
static char* const FFT0_START_ADDR = (char* const) (0x73000000);
static char* const FFT0_READ_ADDR  =(char* const)(0x73000004);
static char* const FFT1_START_ADDR = (char* const)(0x83000000);
static char* const FFT1_READ_ADDR  = (char* const)(0x83000004);

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

int sem_init (uint32_t *__sem, uint32_t count) __THROW
{
  *__sem=count;
  return 0;
}

int sem_wait (uint32_t *__sem) __THROW
{
  uint32_t value, success; //RV32A
  __asm__ __volatile__("\
L%=:\n\t\
     lr.w %[value],(%[__sem])            # load reserved\n\t\
     beqz %[value],L%=                   # if zero, try again\n\t\
     addi %[value],%[value],-1           # value --\n\t\
     sc.w %[success],%[value],(%[__sem]) # store conditionally\n\t\
     bnez %[success], L%=                # if the store failed, try again\n\t\
"
    : [value] "=r"(value), [success]"=r"(success)
    : [__sem] "r"(__sem)
    : "memory");
  return 0;
}

int sem_post (uint32_t *__sem) __THROW
{
  uint32_t value, success; //RV32A
  __asm__ __volatile__("\
L%=:\n\t\
     lr.w %[value],(%[__sem])            # load reserved\n\t\
     addi %[value],%[value], 1           # value ++\n\t\
     sc.w %[success],%[value],(%[__sem]) # store conditionally\n\t\
     bnez %[success], L%=                # if the store failed, try again\n\t\
"
    : [value] "=r"(value), [success]"=r"(success)
    : [__sem] "r"(__sem)
    : "memory");
  return 0;
}

int barrier(uint32_t *__sem, uint32_t *__lock, uint32_t *counter, uint32_t thread_count) {
	sem_wait(__lock);
	if (*counter == thread_count - 1) { //all finished
		*counter = 0;
		sem_post(__lock);
		for (int j = 0; j < thread_count - 1; ++j) sem_post(__sem);
	} else {
		(*counter)++;
		sem_post(__lock);
		sem_wait(__sem);
	}
	return 0;
}

//Total number of cores
//static const int PROCESSORS = 2;
#define PROCESSORS 2
//the barrier synchronization objects
uint32_t barrier_counter=0; 
uint32_t barrier_lock; 
uint32_t barrier_sem; 
//the mutex object to control global summation
uint32_t lock;  
//print synchronication semaphore (print in core order)
uint32_t print_sem[PROCESSORS]; 


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

int c[16][16];
int main(unsigned hart_id) {

  //read_bmp("lena_std_short.bmp");
  printf("======================================\n");
  printf("\t  Reading from array\n");
  printf("======================================\n");
	printf(" Vector lenght\t\t\t\t= %d\n", nbr_vect);
	
	printf(" bytes_per_entry\t\t= %d\n",sizeof(char)/8);
  printf("======================================\n");

  unsigned char buffer[4] = {0};
 unsigned char buffer1[4] = {0}; 
  union word data0;
   union word data1;
  int total0, total1;
  printf("Start processing...\n");
 
if (hart_id == 0) {
		// create a barrier object with a count of PROCESSORS
		sem_init(&barrier_lock, 1);
		sem_init(&barrier_sem, 0); //lock all cores initially
	
		sem_init(&lock, 1);
	}

 //no parallel 
  /*
  sem_wait(&lock);
  if(hart_id == 0)
  {
    for(int i = 0; i<16;i+=4)
    {
      for(int j = 0; j< 16;j++)
      {
        for(int u = 0; u<16; u++)
        {
          
            buffer[0] = a[i][u];
            buffer[1] = b[u][j];
            buffer[3] = 0;
            buffer[2] = 0;
            write_data_to_ACC(FFT0_START_ADDR, buffer, 4);

            buffer1[0] = a[i+2][u];
            buffer1[1] = b[u][j];
            buffer1[3] = 0;
            buffer1[2] = 0;
            write_data_to_ACC(FFT1_START_ADDR, buffer1, 4);

        }
     
        read_data_from_ACC(FFT0_READ_ADDR, buffer, 4);
        memcpy(data0.uc, buffer, 4);
        total0 = (data0).sint;  
        c[i][j] = total0;

        read_data_from_ACC(FFT1_READ_ADDR, buffer1, 4);
        memcpy(data1.uc, buffer1, 4);
        total1 = (data1).sint;
        c[i+2][j] = total1;
    
        
      }
    }
  }
 else
 {
   for(int i = 0; i<16;i+=4)
    {
      for(int j = 0; j< 16;j++)
      {
        for(int u = 0; u<16; u++)
        {
          
            buffer[0] = a[i+1][u];
            buffer[1] = b[u][j];
            buffer[3] = 0;
            buffer[2] = 0;

            write_data_to_ACC(FFT0_START_ADDR, buffer, 4);

            buffer1[0] = a[i+3][u];
            buffer1[1] = b[u][j];
            buffer1[3] = 0;
            buffer1[2] = 0;
            write_data_to_ACC(FFT1_START_ADDR, buffer1, 4);

         
        }

        read_data_from_ACC(FFT0_READ_ADDR, buffer, 4);
        memcpy(data0.uc, buffer, 4);
        total0 = (data0).sint;  
        c[i+1][j] = total0;
        //printf("\n");
        read_data_from_ACC(FFT1_READ_ADDR, buffer1, 4);
        memcpy(data1.uc, buffer1, 4);
        total1 = (data1).sint;
        c[i+3][j] = total1;

        
      }
    }
 }
    sem_post(&lock);
    barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);
  */


//parallel computation
  sem_wait(&lock);
  if(hart_id == 0)
  {
    for(int i = 0; i<16;i+=4)
    {
      for(int j = 0; j< 16;j++)
      {
        for(int u = 0; u<8; u++)
        {
          
            buffer[0] = a[i][u];
            buffer[1] = b[u][j];
            buffer[3] = a[i][u+8];
            buffer[2] = b[u+8][j];
            write_data_to_ACC(FFT0_START_ADDR, buffer, 4);

            buffer1[0] = a[i+2][u];
            buffer1[1] = b[u][j];
            buffer1[3] = a[i+2][u+8];
            buffer1[2] = b[u+8][j];
            write_data_to_ACC(FFT1_START_ADDR, buffer1, 4);

        }
     
        read_data_from_ACC(FFT0_READ_ADDR, buffer, 4);
        memcpy(data0.uc, buffer, 4);
        total0 = (data0).sint;  
        c[i][j] = total0;

        read_data_from_ACC(FFT1_READ_ADDR, buffer1, 4);
        memcpy(data1.uc, buffer1, 4);
        total1 = (data1).sint;
        c[i+2][j] = total1;
    
        
      }
    }
  }
 else
 {
   for(int i = 0; i<16;i+=4)
    {
      for(int j = 0; j< 16;j++)
      {
        for(int u = 0; u<8; u++)
        {
          
            buffer[0] = a[i+1][u];
            buffer[1] = b[u][j];
            buffer[3] = a[i+1][u+8];
            buffer[2] = b[u+8][j];

            write_data_to_ACC(FFT0_START_ADDR, buffer, 4);

            buffer1[0] = a[i+3][u];
            buffer1[1] = b[u][j];
            buffer1[3] = a[i+3][u+8];
            buffer1[2] = b[u+8][j];
            write_data_to_ACC(FFT1_START_ADDR, buffer1, 4);

         
        }

        read_data_from_ACC(FFT0_READ_ADDR, buffer, 4);
        memcpy(data0.uc, buffer, 4);
        total0 = (data0).sint;  
        c[i+1][j] = total0;
        //printf("\n");
        read_data_from_ACC(FFT1_READ_ADDR, buffer1, 4);
        memcpy(data1.uc, buffer1, 4);
        total1 = (data1).sint;
        c[i+3][j] = total1;

        
      }
    }
 }
    sem_post(&lock);
    barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);

if(hart_id == 0)
{
  for(int i =0;i <16;i++)
  {
    for(int j=0;j<16;j++)
    {
      printf("%d ", c[i][j]);
    }
    printf("\n");
  }
  
}




  return 0;

}
