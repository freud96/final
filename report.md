Written by Freud L. Lewis Piercius, student ID: 110061422

Final Project report

**Introduction**
    In mathematics field, more precisely linear algebra, matrix multiplication is an operation in which two matrices are involved to produce a third. This binary operation is widely popular. 
	For the final project, we design a matrix multiplication accelerator on RISCV-VP and synthetized it with HLS. The acccelerator can calculate the product of any matrix A m*16 and B 16*n. 
    
    
 **Hardware structure on RISCV-VP**
    On the RISCV-VP, the hardware structure can look like (this link) on the multicore. where the DMA controller is to transfer data from global memories to the accelerator; 
    the RISCV CPU core is where the software is running. and the accelerator hardware is where all the computations are executed. 
   
    **Accelerator**
      In this project. multiple number of PEs were considered. 1 single PE, 2 and 4. the structure of an individual PE can be shown as follow https://github.com/freud96/final/blob/main/Structure.JPG where the two data "a" and "b" from memory represent entry of matrix A at (i,j) and matrix B at(j, i) respectively.
      We implemented two types to analyse simulating times elapsed: 1- compute the multiplication of 2 two by vector 1*16 and 16* 1; 2- Compute 2 different multiplication operations of vectors 1*8 and 8*1 to exploit a single core 
      paralellism, and add them together to get the final result. 
      
    **Software**
      To feed the data to the accelerator, DMA interface is used to transfer data from global memories to the accelerator, also TLM's generic payload is also used as to when to read/write data from/to tlm port. 
	We used differents algorithm to send data to accelerator, for this project only square matrices were considered.
      On 1 core CPU:
      For type 1: The software send 1 data from each matrix per iteration.
        For 1 PE: number of iterations: 16*16*16 = 4096
        For 2 PEs: number of iterations: 8*16*16 = 2048
        For 4 PEs: number of iterations: 4*16*16 = 1024
      For type 2: The software sends 2 data from each matrix per iteration.
        For 1 PE: number of iterations: 16*16*8 = 2048
        For 2 PEs: number of iterations: 8*16*8 = 1024
        For 4 PEs: number of iterations: 4*16*8 = 512
      On 2 cores CPU:
      For type 1: The software send 1 data from each matrix per iteration.
        For 1 PE: number of iterations: 8*16*16 = 2048 on each core
        For 2 PEs: number of iterations: 4*16*16 = 1024 on each core
        For 4 PEs: number of iterations: 2*16*16 = 512 on each core
      For type 2: The software sends 2 data from each matrix per iteration.
        For 1 PE: number of iterations: 8*16*8 = 1024 on each core
        For 2 PEs: number of iterations: 4*16*8 = 512 on each core 
        For 4 PEs: number of iterations: 2*16*8 = 256 on each core
    
    
 **RISCV-VP CPU**
    CPU with one core was use for both types and 2 cores were studied for type 2.  
   
 
**RISCV-VP result**
    As mentioned above, we studied 2 different types of accelerator design, the table below show the result of simulation on RISCV virtual platform. 
    
    	No internal Parallelism 1 Core		internal Parallelism 1 Core		No internal Parallelism 2 Cores					internal Parallelism 2 Core				
	sim times		num ins		sim times		num ins		sim times		num ins core 0	num ins core 1		sim times		num ins core 0	num ins core 1
1PE	19176470 ns		529625		15694870 ns		441561		17343930 ns		546446		694595		 	13821350 ns		430938		579087
2PE	18673750 ns		518083		15435790 ns		436093		16816870 ns		529662		677595			13540020 ns		422140		570073
4PE	18421310 ns		512026		15308330 ns		433440		16553460 ns		521272		669097			13378730 ns		416979		564578


	As the table shows, the simulation time for 2 cores and 4 PEs is the shortest. When taking closer looks and according to what was explained in software part, total number of iterations are same for
		a) 1 core's type 1's 2 PEs 1 core's type 2's 1 PE, 2 core's type 1's 1 PE (2048); 
		b) 1 core's type 1's 4 PEs, 1 core's type 2's 2 PE, 2 cores' type 1's 2 PE, 2 cores' type 2's 2 PEs (1024);
		c) 1 Core's type 2's 1 PE, 2 cores' type 1's 4 PEs, 2 cores' type 2's 2 PEs (512)
	Starting from these lines, one would naively base on number of iteration to estimate the timing. However, multithreading programming makes a program faster and the more PEs used, the more core used the faster the program is.
	One major difference in all these types remained in the data transferred per iteration. For instance for 2 PEs in type 1, 4 different datas are sent while in type two the number of data sent is doubled.
	Hence to reduce memory bottleneck and avoid data transfer from global memory to accelerator, the more data sent per iteration, the better; however need to also care about the DMA, depending on the bandwitdh. Also,
	we did not get to that but we assume that 2 DMA will be useful when dealing with multiple core and multiple PEs.  
	with 4 PEs and 2 cores the program went from 19176470 ns(original, 1 core 1 single PE) to 13378730 ns.    


**Synthesis result**
   we also synthesized the accelerator with HLS
	For this part, we try to use as sc_int<8> for the input from software to hardware since the range of the matrices' values is small and sc_int<32> for the output. We synthesized both type enlightened in previous paragraph
	to evaluate the number of cycles and area. however for different numbers of PEs, stratus does not give total area as the instantion was made in System.cpp. For the PE we use module parallelism and pipelining hence only type 2
	fits in the requirements of the project since we split 16 entries to 8.  

			1PE		2PEs		4PEs
SIM BASIC			
latency t		8960		8576		8384
latency a		36		34		33
run time		81950ns		81950ns		81950 ns
area acc		1267.5		
area mul0		682.4		
area mul1		685.2		
total			2635.1		
				
				
SIM DPA				
latency t		8704		8448		8320
latency a		35		34		33
run time		81940 ns	81940 ns	81940 ns

	Number of cycles greatly is reduced as the number of PEs increases, we also believe that if we could transport 16-bit data through cynwp2p port instead of 2 8-bit per PE, number of cycle would be further reduced.
	As the number of PEs increases the area of the system also increases. 





    




