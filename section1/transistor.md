#### Section 1: Intro: Cheating our way past the transistor -- 0.5 weeks
- So about those transistors -- Course overview. Describe how FPGAs are buildable using transistors, and that ICs are just collections of transistors in a nice reliable package. Understand the LUTs and stuff. Talk briefly about the theory of transistors, but all projects must build on each other so we can’t build one.
- Emulation -- Building on real hardware limits the reach of this course. Using something like Verilator will allow anyone with a computer to play.

#### Bipolar transistor
- Transistor is device used for switching or amplification of electric signal.
- 3 terminals(Base, Emitter, Collector)
- Conduction between Emitter and Collector depends on the availability of charge carriers, which is controlled by a voltage applied to the Base

#### MOSFET
- Metal Oxid Semiconductor Field Effect Transistor
- 4 terminals(Souce(S), Gate(G), Drain(D), Body(B))
- Electrical variations happening in the channel width along with the flow of carriers (either holes or electrons)
- The charge carriers enter into the channel through the source terminal and exit via the drain.
-The width of the channel is controlled by the voltage applied to the Gate

- Depletion Mode
    - When there is no voltage across the gate terminal, the channel shows its maximum conductance. Whereas when the voltage across the gate terminal is either positive or negative, then the channel conductivity decreases.

- Enhancement Mode
    - When there is no voltage across the gate terminal, then the device does not conduct. When there is the maximum voltage across the gate terminal, then the device shows enhanced conductivity.

- P-channel MOSFET  a negative voltage(low voltage in enhacement mode) is necessary  to activate MOSFET(electrons will flow drain to source)(Hole carriers)
- N-channel MOSFET  a positive voltage(high voltage in enhacement mode) is necessary  to activate MOSFET(electrons will flow source to drain)(Electron carriers)
#### Logic gates
- is a device that performs a boolean function, a logical operation on one or more binary inputs
- the basic building block
- are built from transistors
- NOT
- NAND
- AND
- NOR
- OR
- XNOR
- NOR

#### Lookup table(LUT)
- is an actual table that generates an output based on the inputs
- customized truth table which is loaded with values that are relevant
- in fpga LUT acts like a memory

#### Integrated circuit(IC)
- large number of miniaturized transistors integrated together in small area in silicon
- high transistor density
#### FPGA
- type of IC
- array of programmable logic blocks(CLB control logic block)
- logic blocks and interconnects between gates can be programmed
- the boolean function is saved in the LUT within the CLB
- cannot set initial state, since all is based on voltages; so you always need to wait for signals to settle, which you need to account for in the design. 
- coarse grained elements: dedicated memory (block RAM), embedded processors, flash memory, floating point units, multipliers etc. basically anything that's already pre-fabed

#### CLB
- contains flip-flops, LUTs and multiplexers
- nterconnects can be routed to other CLBs, IO blocks or other coarse grained element
- LUT stores the functions output
- basically: LUT (has outputs) -> Multiplexer (inputs is LUT, single output, selectors are the inputs) -> DFF (to make the whole thing sequentially valid and have the reads driven by the CLK)
    - the DFF here is optional and only needed if you need to make a sequential circuit.
    - if a combinational is needed, then DFF is skipped
    - a MUX is used to select between DFF and single LUT-MUX output
