#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vblink.h"

#define MAX_SIM_TIME 200000

int main(int argc, char** argv, char**env){
    Verilated::commandArgs(argc, argv);
    Verilated::traceEverOn(true);
    vluint64_t simTime = 0;
    Vblink* top = new Vblink;

    
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    top->trace(m_trace, 99);
    m_trace->open("waveform.vcd");
    while(simTime < MAX_SIM_TIME){
        top->clk ^= 1;
        top->eval();
        m_trace->dump(simTime);
        simTime++;
    }
    delete top;
    m_trace->close();
    exit(EXIT_SUCCESS);
}