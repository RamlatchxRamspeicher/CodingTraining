#include "logic_gates.cpp"
#ifndef NULLBYTE
    #define NULLBYTE byte{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
#endif
struct bitDecoded{bool zero; bool one;};

BIT SWITCH(BIT a, BIT s){return (s) ? AND(a,s): 0/*NULL*/;}

BYTE SWITCH8(bool s, BYTE a){
    return BYTE{
        SWITCH(a.b128, s),
        SWITCH(a.b64,  s),
        SWITCH(a.b32,  s),
        SWITCH(a.b16,  s),
        SWITCH(a.b8,   s),
        SWITCH(a.b4,   s),
        SWITCH(a.b2,   s),
        SWITCH(a.b1,   s)
    };
}

BYTE MUX8(bool s, BYTE a, BYTE b){
    return OR_8(SWITCH8(NOT(s),a),SWITCH8(s,b));
}

bitDecoded bitDecoder(BIT i){return bitDecoded{NOT(i),i};}

uint8_t bitDecoder3(BIT i1,BIT i2, BIT i3){
    bitDecoded b1= bitDecoder(i1);
    bitDecoded b2= bitDecoder(i2);
    bitDecoded b3= bitDecoder(i3);
    if(AND3(b1.zero,b2.zero,b3.zero)) return (uint8_t)0;
    if(AND3(b1.one ,b2.zero,b3.zero)) return (uint8_t)1;
    if(AND3(b1.zero,b2.one ,b3.zero)) return (uint8_t)2;
    if(AND3(b1.one ,b2.one ,b3.zero)) return (uint8_t)3;
    if(AND3(b1.zero,b2.zero,b3.one )) return (uint8_t)4;
    if(AND3(b1.one ,b2.zero,b3.one )) return (uint8_t)5;
    if(AND3(b1.zero,b2.one ,b3.one )) return (uint8_t)6;
    if(AND3(b1.one ,b2.one ,b3.one )) return (uint8_t)7;
    return 255/*NULL*/;
}
uint8_t bitDecoder3s(BIT i1, BIT i2, BIT i3, bool s){
    return (!s) ? bitDecoder3(i1,i2,i3) : 255 /*NULL*/;
}

