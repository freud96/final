# final

one can download the updates source code to their working directory
  git clone https://github.com/freud96/final
  
  copy the accelerator virtual platform
  $ cp -r final/vp/src/platform/* $<working directory>/riscv-vp/vp/src/platform
  
  copy the software code
  $ cp -r final/sw/* $<working directory>/riscv-vp/sw
  
  go to vp/build under the riscv-vp directory and do the following:
  $ cmake ..
  $ make install
  
  build the multiple PE/core software:
  1 Core
  1 PE :
  go to basic-mul under riscv-vp/sw and do 
  $ make
  $ make sim
  
  2 PE :
  go to basic-mul_2pe under riscv-vp/sw and do 
  $ make
  $ make sim
  
  4 PE :
  go to basic-mul_4PE under riscv-vp/sw and do 
  $ make
  $ make sim
  
  
  2 Cores :
  1 PE :
  go to basic-mu_2core_1pe under riscv-vp/sw and do 
  $ make
  $ make sim
  
  2 PE :
  go to basic-mul_2core_2pe under riscv-vp/sw and do 
  $ make
  $ make sim
  
  4 PE :
  go to basic-mul_2core_4pe under riscv-vp/sw and do 
  $ make
  $ make sim
  
  
  To test the two types, uncomment the part below no parallelism and comment out part below parallelism in both mul.h in vp/platform/<accelerator model> and main_mul.c in
  sw/<accelerator model>. then redo steps above
