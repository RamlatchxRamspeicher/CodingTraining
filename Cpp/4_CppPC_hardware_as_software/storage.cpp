#include "arithmetic.cpp"
#include <vector>

class tickDELAY{
    public:
        BIT a = ZERO;
        BIT tick(BIT i){BIT tmp = a; a = i; return tmp;}
        BIT operator()(BIT i){return tick(i);}
};
class byteDELAY{
    BYTE a = {ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO};
    BYTE tick(BYTE i){BYTE tmp = a; a = i; return tmp;}
    public:
        BYTE operator()(BYTE i){return tick(i);}
};
class CLOCK{
    BIT state = ZERO;
    public:
        BIT operator()(){state=!state; return state;}
};
class bitStorage{
    tickDELAY storage;
    BIT act(bool s, BIT v){
        return storage((!s/*SWITCH(v,s)==NULL*/) ? SWITCH(storage.a,NOT(s)): v);
    }
    public:
        BIT operator()(bool s, BIT v){return act(s,v);}
        //schnittstelle fuer program counter nicht in realer hardware da keine leitung vorhanden
        BIT operator[](BIT _)const{return storage.a;}
};
class byteStorage{
    bitStorage s1,s2,s4,s8,s16,s32,s64,s128;
    BYTE act(bool s, BYTE v){ return BYTE{
        s1(s,v.b1),
        s2(s,v.b2),
        s4(s,v.b4),
        s8(s,v.b8),
        s16(s,v.b16),
        s32(s,v.b32),
        s64(s,v.b64),
        s128(s,v.b128)};
    }
    //schnittstelle fuer program counter nicht in realer hardware da keine leitung vorhanden
    BYTE peek()const{return BYTE{s128[NULL], s64[NULL], s32[NULL], s16[NULL], s8[NULL], s4[NULL], s2[NULL], s1[NULL]};}
    public:
        BYTE operator()(bool s, BYTE v){return act(s,v);}
        BYTE operator()()const{return peek();}
};
class COUNTER{
    byteStorage current;
    BYTE next(bool s, BYTE v){
        return current(true, MUX8(s,ADD8(current(),BYTE{}, true),v)); //always overwrite: get what is in storage incremented by one or value
    }
    public:
        //byte operator()(bool s, byte v){return next(s,v);}
        //byte peek() const {return current();} //schnittstelle fuer program counter nicht in realer hardware da keine leitung vorhanden
        BYTE operator()(bool s, BYTE v){
            BYTE result = next(s,v);
            return result;
        }
        BYTE peek() const {
            BYTE p = current();
            return p;
        }
};

class REGISTER{ // OVERTURE
    byteStorage storage;
    uint8_t ID;
    BYTE adress(uint8_t on, bool overwrite, BYTE busData){
        return (ID==on) ? storage(overwrite, busData) : NULLBYTE;
    }
    BYTE get(){return storage();} //use as permanent on for REG0 to Counter; REG1, REG2 to ALU; REG3 to JUMPCONDITION
    public:
        BYTE operator()(uint8_t on, bool overwrite, BYTE busData){return adress(on, overwrite, busData);}
        BYTE operator()(){return get();}
        REGISTER(uint8_t id): ID(id){}
};

class REG_16{
    byteStorage storage[2];
    protected:
        uint8_t ID;
        SHORT adress(uint8_t on, bool overwrite, SHORT busData){
            return (ID==on) ? SHORT{storage[1](overwrite, busData.upper),storage[0](overwrite, busData.lower)} : NULLSHORT;
        }
        SHORT get(){return SHORT{storage[1](),storage[0]()};} //use as permanent on
    public:
        SHORT operator()(uint8_t on, bool overwrite, SHORT busData){return adress(on, overwrite, busData);}
        SHORT operator()(){return get();}
        REG_16(uint8_t id=0){ID=id;}
};

//program of up to 256 instructions
class Program{
    COUNTER& counter_;
    public:
        uint8_t* prog;
        Program(COUNTER& counter, uint8_t* p) : counter_(counter), prog(p) {}

        struct Iterator {
            const uint8_t* prog;
            COUNTER& counter;
            bool done;

            uint8_t operator*() const {
                return prog[uINT8(counter.peek())];
            }

            Iterator& operator++() {
                counter(false, BYTE{});
                return *this;
            }

            void jump(BYTE addr) {
                counter(true, addr);
            }

            bool operator!=(const Iterator&) const { return !done; }
        };

        Iterator begin() { return {prog, counter_, false}; }
        Iterator end()   { return {prog, counter_, true};  }
};
