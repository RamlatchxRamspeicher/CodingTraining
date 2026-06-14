#include "storage.cpp"

class STACKREG_16: public REG_16{
    static uint8_t nextID;
    public:
        STACKREG_16():REG_16(nextID){nextID++;}
};

class STACK_16{
    uint8_t stackPointer=0;
    STACKREG_16 stack[256];
    public:
        void push(SHORT v){stack[stackPointer](stackPointer,true,v);stackPointer++;}
        SHORT pop(){return stack[stackPointer]();stackPointer--;}
};