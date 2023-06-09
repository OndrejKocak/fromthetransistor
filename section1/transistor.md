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
- NOT
- NAND
- AND
- NOR
- OR
- XNOR
- NOR