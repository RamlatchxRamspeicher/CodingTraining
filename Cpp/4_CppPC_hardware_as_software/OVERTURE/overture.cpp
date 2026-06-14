#include "../storage.cpp"
#include <cstring>
#include <iostream>
//this is too fast so to see output 
#include <thread>
#include <chrono>
#define SLEEP std::this_thread::sleep_for(std::chrono::seconds(1))
//0: OR, 1: NAND, 2: NOR, 3: AND like in Turing Complete
BYTE LOGICUNIT(BYTE instruction, BYTE a, BYTE b){
    return MUX8(instruction.b1,
                MUX8(instruction.b2,
                    OR_8(a,b),
                    NOT_8(OR_8(a,b))),
                MUX8(instruction.b2,
                    OR_8(NOT_8(a),NOT_8(b)),
                    NOT_8(OR_8(NOT_8(a),NOT_8(b))))
                
    );
}

/*  0: OR       instruction(64|0x40)
    1: NAND     instruction(65|0x41)
    2: NOR      instruction(66|0x42)
    3: AND      instruction(67|0x43)
    4: ADD      instruction(68|0x44)
    5: SUB      instruction(69|0x45)
    6: XOR      instruction(70|0x46)

*/
BYTE overtureALU(BYTE instruction, BYTE a, BYTE b){
    uint8_t inst = bitDecoder3(instruction.b1,instruction.b2,instruction.b4);
    switch (inst){
    case 0: return   OR_8(a,b);
    case 1: return NAND_8(a,b);
    case 2: return  NOR_8(a,b);
    case 3: return  AND_8(a,b);
    case 4: return  ADD8(a,b);
    case 5: return  SUB8(a,b);
    case 6: return  XOR_8(a,b);
    default:return  BYTE{ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO};}
}

enum OPCODE{
    DIRECT,
    CALCULATE,
    COPY,
    CONDITION,
    none
};

OPCODE OPCDECODE(BYTE instruction){
    uint8_t OPC = bitDecoder3(instruction.b64,instruction.b128,ZERO);
    switch(OPC){
    case 0: return DIRECT;       
    case 1: return CALCULATE;   
    case 2: return COPY;        
    case 3: return CONDITION;
    default:return none;       }
}

/*  0: Never    instruction(184|0xb8)
    1: x==0     instruction(185|0xb9)
    2: x<0      instruction(186|0xba)
    3: x<=0     instruction(187|0xbb)
    4: Always   instruction(188|0xbc)
    5: x!=0     instruction(189|0xbd)
    6: x>=0     instruction(190|0xbe)
    7: x>0      instruction(191|0xbf)
*/
bool JUMPCONDITION(BYTE instruction, BYTE v){
    return  XOR(instruction.b4,
                OR( AND(instruction.b2,
                        v.b128),
                    AND(instruction.b1,
                        NOR(OR3(v.b1,
                                v.b2,
                                v.b4),
                            OR3(v.b8,
                                v.b16,
                                OR3(v.b32,
                                    v.b64,
                                    v.b128)
                                )
                            )
                        )
                    )
                );
}


class INPUT{
    uint8_t idx = 0;
    public:
        uint8_t* inStream = nullptr; 
        INPUT() = default;
        INPUT(uint8_t* in): inStream(in){}
        BYTE operator()(){return BYTE8(inStream[idx++]);}
};
class OUTPUT{
    public:
        void operator()(BYTE out){std::cout << "Out: " << (int)uINT8(out)<<std::endl;SLEEP;}
};
class BUS{
    BYTE adressLane = NULLBYTE;
    BYTE IOLane = NULLBYTE;
    COUNTER& counter_;
    REGISTER &REG0, &REG1, &REG2, &REG3, &REG4, &REG5, &REG6;
    INPUT& in;
    OUTPUT& out;
    public:
        BUS(COUNTER& c, REGISTER& r0, REGISTER& r1, REGISTER& r2, REGISTER& r3, REGISTER& r4, REGISTER& r5, REGISTER& r6, INPUT& i, OUTPUT& o):
            counter_(c), REG0(r0), REG1(r1), REG2(r2), REG3(r3), REG4(r4), REG5(r5), REG6(r6), in(i), out(o) {}
        void pushInstruction(uint8_t instruction){
            adressLane=BYTE8(instruction);
            OPCODE op = OPCDECODE(adressLane);
            uint8_t readFrom = bitDecoder3s(adressLane.b8,adressLane.b16,adressLane.b32,op!=COPY);
            uint8_t writeTo = bitDecoder3s(adressLane.b1,adressLane.b2,adressLane.b4,op!=COPY);
            switch (op){
                case DIRECT:
                    IOLane = adressLane;
                    REG0(0, true, IOLane);
                    break;
                case CALCULATE:
                    IOLane = overtureALU(adressLane,REG1(),REG2());
                    REG3(3, true, IOLane);
                    break;
                case COPY:
                    switch (readFrom){
                        case 0: IOLane = REG0(); break; 
                        case 1: IOLane = REG1(); break;
                        case 2: IOLane = REG2(); break;
                        case 3: IOLane = REG3(); break;
                        case 4: IOLane = REG4(); break;
                        case 5: IOLane = REG5(); break;
                        case 6: IOLane = REG6(); break;
                        case 7: IOLane = in(); break;
                        default: break;}

                    switch (writeTo){
                        case 0: REG0(writeTo, true, IOLane); break; 
                        case 1: REG1(writeTo, true, IOLane); break;
                        case 2: REG2(writeTo, true, IOLane); break;
                        case 3: REG3(writeTo, true, IOLane); break;
                        case 4: REG4(writeTo, true, IOLane); break;
                        case 5: REG5(writeTo, true, IOLane); break;
                        case 6: REG6(writeTo, true, IOLane); break;
                        case 7: out(IOLane); break;
                        default: break;}
                    break;
                case CONDITION:
                        counter_(JUMPCONDITION(adressLane,REG3()),REG0());break;        
                default: break;
                }
        }
};
/*
bit 0 to 2 determines IOBus recieve aka wich register or output get active
*/
class OverturePC{
    INPUT in;
    REGISTER REG0{0};
    REGISTER REG1{1};
    REGISTER REG2{2};
    REGISTER REG3{3};
    REGISTER REG4{4};
    REGISTER REG5{5};
    REGISTER REG6{6};
    OUTPUT out;
    COUNTER counter_;
    BUS bus{counter_,REG0,REG1,REG2,REG3,REG4,REG5,REG6,in,out};
    Program program{counter_,nullptr};
    public:
    OverturePC(uint8_t* inputStream, uint8_t* programData)
        : in(inputStream){program.prog = programData;}

    void run() {
        auto it = program.begin();
        while (!it.done) {
            uint8_t instruction = *it;
            ++it;
            if (instruction == 0xFF) break; // HALT
            bus.pushInstruction(instruction);
        }
    }
};

/*
void processTick(){
        auto it = program.begin();

        while (it != program.end()) {
            uint8_t opcode = *it;   // liest prog[PC], kein Tick
            ++it;                    // PC += 1

            if (opcode == 0xA0) {
                uint8_t target = *it;  // Zieladresse steht im nächsten Byte
                ++it;
                it.jump(BYTE8(target)); // PC = target, kein Inkrement
    }

    // HALT-Bedingung manuell:
    if (opcode == 0xFF) { it.done = true; }
}
    }
    public:
        OverturePC(uint8_t code[255], uint8_t* psydoInput){
            program.prog = code;
            in.inStream = psydoInput;
        };*/