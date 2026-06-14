#include "../logic_gates.cpp"
#include <cstdint>
class RS_FF{
    bool Q, R, S;
    void _w(){Q  = NOT(OR(R,NOT(OR(S,Q))));}
    bool _get(){return Q;}
    public:
        RS_FF()=default;
        bool operator()(bool r, bool s){
            R=r;S=s;
            if(XOR(R,S))_w();
            return _get();
        }
};

class BITCELL{ //Binary Cell
    RS_FF mem;
    public:
        bool operator()(bool input, bool rw, bool select){
            return(AND3(select,NOT(rw),mem(AND3(select,NOT(input),rw),AND3(select,input,rw))));
        }
};

class CELL_8{ //8bit words
    BITCELL mem[8];
    public:
        BYTE operator()(BYTE input, bool rw, bool select, bool bigEndian){
            if (bigEndian){
            return BYTE{
                mem[0](input.b1,rw,select),
                mem[1](input.b2,rw,select),
                mem[2](input.b4,rw,select),
                mem[3](input.b8,rw,select),
                mem[4](input.b16,rw,select),
                mem[5](input.b32,rw,select),
                mem[6](input.b64,rw,select),
                mem[7](input.b128,rw,select)
            };}
            return BYTE{
                mem[7](input.b1,rw,select),
                mem[6](input.b2,rw,select),
                mem[5](input.b4,rw,select),
                mem[4](input.b8,rw,select),
                mem[3](input.b16,rw,select),
                mem[2](input.b32,rw,select),
                mem[1](input.b64,rw,select),
                mem[0](input.b128,rw,select)
            };
        }
};
class BYTECELL{ // word size 8 see CELL8 for true to hardware
    BYTE mem;
    public:
        BYTE operator()(BYTE input, bool rw, bool select){
            if(NOT(select)) return NULLBYTE;
            if(AND(NOT(rw),select)) return mem;
            else mem = input;
            return NULLBYTE;
        }
};
class SHORTCELL{
    BYTE mem[2];
    public:
        SHORT operator()(SHORT input, bool rw, bool select){
            if(NOT(select)) {return NULLSHORT;}
            if(AND(NOT(rw),select)) return SHORT{mem[1],mem[0]};
            else {mem[0] = input.lower;mem[1] = input.upper;}
            return NULLSHORT;
        }
};
class INTCELL{
    BYTE mem[4];
    public:
        INT operator()(INT input, bool rw, bool select){
            if(NOT(select)) {return NULLINT;}
            if(AND(NOT(rw),select)) return INT{mem[3],mem[2],mem[1],mem[0]};
            else {mem[0] = input.B0;mem[1] = input.B1;mem[2] = input.B2;mem[3] = input.B3;}
            return NULLINT;
        }
};

class RAM_16{
    static const uint16_t size = 1024;
    SHORTCELL mem[size];
    public:
        void write(SHORT input, SHORT ADR){mem[uSHORT(ADR)](input,true,true);}
        SHORT read(SHORT ADR){return mem[uSHORT(ADR)](NULLSHORT,false,true);}
};

class RAMCONTROLER_32{
    bool occupied[1024];
    /* void write(INT* input, INT iSize, INT ADR){
        for(int i=0; i<uINT32(iSize); i++){
            mem[uINT32(ADR)+i](input[i],true,true);
        }
    } */
};
class RAM_32{
    static const uint16_t size = 1024;
    INTCELL mem[size];
    public:
        void write(INT input, INT ADR){mem[uINT32(ADR)](input,true,true);}
        INT read(INT ADR){return mem[uINT32(ADR)](NULLINT,false,true);}
};