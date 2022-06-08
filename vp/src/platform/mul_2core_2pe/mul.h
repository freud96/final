#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
#include <cmath>
#include <iomanip>
using namespace sc_core;

#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#include "matrix.h"



class MUL : public sc_module{
    public:
        tlm_utils::simple_target_socket<MUL> tsock;
        
      
         //parallelism
        sc_fifo<signed char> i_a;
        sc_fifo<signed char> i_b;
        sc_fifo<signed char> i_a1;
        sc_fifo<signed char> i_b1;
        sc_fifo<int> o_result;
    
     //no parallel
       /*
        sc_fifo<signed char> i_a;
        sc_fifo<signed char> i_b;
        sc_fifo<int> o_result;
       
         */

        SC_HAS_PROCESS(MUL);
        MUL(sc_module_name n):
        sc_module(n), tsock("t_skt"), base_offset(0){
             
        tsock.register_b_transport(this, &MUL::blocking_transport);
        SC_THREAD(computation);
    }
       ~MUL() {}


      int val;
      
      
       
        unsigned int base_offset;
        








//no parallelism

/*
void computation(){

    wait(CLOCK_PERIOD, SC_NS);
    while(true)
    {
        val = 0;   
        for(int i = 0; i<16; i++)
        {
          signed char a =  i_a.read(); 
          signed char b = i_b.read();
          int prod = a * b;
          wait(CLOCK_PERIOD, SC_NS);
          val += prod;
          wait(CLOCK_PERIOD, SC_NS);
        }

o_result.write(val);
    }
}



void blocking_transport(tlm::tlm_generic_payload &payload,
                                     sc_core::sc_time &delay) {
 wait(delay);
    // unsigned char *mask_ptr = payload.get_byte_enable_ptr();
    // auto len = payload.get_data_length();
    tlm::tlm_command cmd = payload.get_command();
    sc_dt::uint64 addr = payload.get_address();
    unsigned char *data_ptr = payload.get_data_ptr();

    addr -= base_offset;
  
  word buffer;

  switch (cmd) {
  case tlm::TLM_READ_COMMAND:
    switch (addr) {
    case MUL_RESULT_ADDR:
       buffer.uint = o_result.read();
                
            
      break;
  
    default:
      std::cerr << "Error! MUL_compute::blocking_transport read: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
    }
    data_ptr[0] = buffer.uc[0];
    data_ptr[1] = buffer.uc[1];
    data_ptr[2] = buffer.uc[2];
    data_ptr[3] = buffer.uc[3];
   // data_ptr[4] = buffer.data_in[4];

    break;

  case tlm::TLM_WRITE_COMMAND:
    switch (addr) {
    case MUL_R_ADDR:
        i_a.write(data_ptr[0]);
        i_b.write(data_ptr[1]);

      break;
    default:
      std::cerr << "Error! MUL_compute::blocking_transport wirte: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
    }
    break;

  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }

  payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}


*/


//with parallelism row compuation
void computation(){

    wait(CLOCK_PERIOD, SC_NS);
    while(true)
    {
        val = 0;   
        for(int i = 0; i<8; i++)
        {
          signed char a =  i_a.read(); 
          signed char b = i_b.read();
          signed char c =  i_a1.read(); 
          signed char d = i_b1.read();
          int prod = a * b;
          int prod1 = c * d;
          wait(CLOCK_PERIOD, SC_NS);
          val += (prod + prod1);
          wait(CLOCK_PERIOD, SC_NS);
        }

o_result.write(val);
    }
}



void blocking_transport(tlm::tlm_generic_payload &payload,
                                     sc_core::sc_time &delay) {
 wait(delay);
    // unsigned char *mask_ptr = payload.get_byte_enable_ptr();
    // auto len = payload.get_data_length();
    tlm::tlm_command cmd = payload.get_command();
    sc_dt::uint64 addr = payload.get_address();
    unsigned char *data_ptr = payload.get_data_ptr();

    addr -= base_offset;
  
  word buffer;

  switch (cmd) {
  case tlm::TLM_READ_COMMAND:
    switch (addr) {
    case MUL_RESULT_ADDR:
       buffer.uint = o_result.read();
                
            
      break;
  
    default:
      std::cerr << "Error! MUL_compute::blocking_transport read: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
    }
    data_ptr[0] = buffer.uc[0];
    data_ptr[1] = buffer.uc[1];
    data_ptr[2] = buffer.uc[2];
    data_ptr[3] = buffer.uc[3];
   // data_ptr[4] = buffer.data_in[4];

    break;

  case tlm::TLM_WRITE_COMMAND:
    switch (addr) {
    case MUL_R_ADDR:
        i_a.write(data_ptr[0]);
        i_b.write(data_ptr[1]);
        i_a1.write(data_ptr[2]);
        i_b1.write(data_ptr[3]);

      break;
    default:
      std::cerr << "Error! MUL_compute::blocking_transport wirte: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
    }
    break;

  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }

  payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}


};

#endif