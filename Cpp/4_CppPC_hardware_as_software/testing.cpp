#include "OVERTURE/overture.cpp"
#include "OVERTURE/overtureInstructionSet.hpp"

uint8_t fib[255] = {
    0b00000000, // [0] DIRECT 0 → REG0=0
    0b10000001, // [1] COPY REG0→REG1 (a=0)
    0b10000010, // [2] COPY REG0→REG2 (b=0)
    0b00000001, // [3] DIRECT 1 → REG0=1
    0b10000010, // [4] COPY REG0→REG2 (b=1)

    // loop: PC=5
    0b10001111, // [5] COPY REG1→OUTPUT (print a)
    0b01000100, // [6] CALCULATE ADD → REG3 = a+b
    0b10011100, // [7] COPY REG3→REG4 (tmp = a+b)
    0b10010001, // [8] COPY REG2→REG1 (a = b)
    0b10100010, // [9] COPY REG4→REG2 (b = tmp)

    0b00000101, // [10] DIRECT 5 → REG0=5
    0b11111100, // [11] CONDITION Always → jump zu REG0

    // Rest mit 0 auffüllen
};


uint8_t fibAssembly[255] = {
    DIRECT(0),          // [0]  REG0 = 0
    COPY(R0, R1),       // [1]  REG1 = 0  (a)
    COPY(R0, R2),       // [2]  REG2 = 0  (b)
    DIRECT(1),          // [3]  REG0 = 1
    COPY(R0, R2),       // [4]  REG2 = 1  (b)

    // loop: PC=5
    COPY(R1, OUT),      // [5]  OUTPUT = REG1
    CALC_ADD,           // [6]  REG3 = REG1 + REG2
    COPY(R3, R4),       // [7]  REG4 = REG3 (tmp)
    COPY(R2, R1),       // [8]  REG1 = REG2
    COPY(R4, R2),       // [9]  REG2 = REG4

    GOTO(5),            // [10-11] springe zu PC=5
};

int main(){
    uint8_t Input[255]  = {};


    OverturePC pc(Input, fib);
    pc.run();
    return 0;
}