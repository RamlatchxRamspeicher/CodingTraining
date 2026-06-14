#include "dataflow.cpp"
struct AdderResult{bool sum; bool carry;};

AdderResult halfAdder(BIT a, BIT b){
    return AdderResult{XOR(a,b),AND(a,b)};
}
AdderResult fullAdder(BIT a, BIT b, bool c){
    return AdderResult{  XOR(XOR(a,b),c),
                         OR(AND(a,b),AND(XOR(a,b),c))};
}

BYTE ADD8(BYTE a, BYTE b, BIT carry = ZERO){
    BYTE result;
    AdderResult r = fullAdder(  a.b1,   b.b1,   carry); result.b1   = r.sum;
                r = fullAdder(  a.b2,   b.b2, r.carry); result.b2   = r.sum;
                r = fullAdder(  a.b4,   b.b4, r.carry); result.b4   = r.sum;
                r = fullAdder(  a.b8,   b.b8, r.carry); result.b8   = r.sum;
                r = fullAdder( a.b16,  b.b16, r.carry); result.b16  = r.sum;
                r = fullAdder( a.b32,  b.b32, r.carry); result.b32  = r.sum;
                r = fullAdder( a.b64,  b.b64, r.carry); result.b64  = r.sum;
                r = fullAdder(a.b128, b.b128, r.carry); result.b128 = r.sum;
    return result;
}

BYTE NEGATE8(BYTE a){return ADD8(a,BYTE{0,0,0,0,0,0,0,1});}

BYTE SUB8(BYTE a, BYTE b){return ADD8(a,NEGATE8(b));}

BYTE SHIFTL(BYTE a, BYTE x){
    uint8_t ammount = bitDecoder3(x.b1,x.b2,x.b4);
    switch (ammount) {
    case 0: return a;
    case 1: return BYTE{a.b64,a.b32,a.b16,a.b8,a.b4,a.b2,a.b1, ZERO};
    case 2: return BYTE{a.b32,a.b16,a.b8 ,a.b4,a.b2,a.b1,ZERO , ZERO};
    case 3: return BYTE{a.b16,a.b8 ,a.b4 ,a.b2,a.b1,ZERO ,ZERO , ZERO};
    case 4: return BYTE{a.b8 ,a.b4 ,a.b2 ,a.b1,ZERO ,ZERO ,ZERO , ZERO};
    case 5: return BYTE{a.b4 ,a.b2 ,a.b1 ,ZERO ,ZERO ,ZERO ,ZERO , ZERO};
    case 6: return BYTE{a.b2 ,a.b1 ,ZERO  ,ZERO ,ZERO ,ZERO ,ZERO , ZERO};
    case 7: return BYTE{a.b1 ,ZERO  ,ZERO  ,ZERO ,ZERO ,ZERO ,ZERO , ZERO};
        /* code */
    default: return NULLBYTE;
    }
}

BYTE SHIFTR(BYTE a, BYTE x){
    uint8_t ammount = bitDecoder3(x.b1,x.b2,x.b4);
    switch (ammount) {
    case 0: return a;
    case 1: return BYTE{ZERO, a.b128,a.b64,a.b32,a.b16,a.b8,a.b4,a.b2};
    case 2: return BYTE{ZERO, ZERO, a.b128,a.b64,a.b32,a.b16,a.b8,a.b4};
    case 3: return BYTE{ZERO, ZERO, ZERO, a.b128,a.b64,a.b32,a.b16,a.b8};
    case 4: return BYTE{ZERO, ZERO, ZERO, ZERO, a.b128,a.b64,a.b32,a.b16};
    case 5: return BYTE{ZERO, ZERO, ZERO, ZERO, ZERO, a.b128,a.b64, a.b32};
    case 6: return BYTE{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, a.b128,  a.b64};
    case 7: return BYTE{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO,    a.b128};
        /* code */
    default: return NULLBYTE;
    }
}
BYTE NIBBLEN(fourBit a, fourBit b){
    BYTE result = {ZERO, ZERO, ZERO, ZERO, a.b8, a.b4, a.b2, a.b1};
    result = MUX8(  b.b8,
                    MUX8(   b.b4,
                            MUX8(   b.b2,
                                    MUX8(   b.b1,
                                            {0,0,0,0,0,0,0,0},
                                            result),
                                    ADD8(   MUX8(   b.b1,
                                                    {0,0,0,0,0,0,0,0},
                                                    result),
                                            SHIFTL( result,
                                                    {0,0,0,0,0,0,0,1}))),
                            ADD8(   MUX8(   b.b2,
                                            MUX8(   b.b1,
                                                    {0,0,0,0,0,0,0,0},
                                                    result),
                                            ADD8(   MUX8(   b.b1,
                                                            {0,0,0,0,0,0,0,0},
                                                            result),
                                                    SHIFTL( result,
                                                            {0,0,0,0,0,0,0,1}))),
                                    SHIFTL( result,
                                            {0,0,0,0,0,0,1,0}))),
                    ADD8(   MUX8(   b.b4,
                                    MUX8(   b.b2,
                                            MUX8(   b.b1,
                                                    {0,0,0,0,0,0,0,0},
                                                    result),
                                            ADD8(   MUX8(   b.b1,
                                                            {0,0,0,0,0,0,0,0},
                                                            result),
                                                    SHIFTL( result,
                                                            {0,0,0,0,0,0,0,1}))),
                                    ADD8(   MUX8(   b.b2,
                                                    MUX8(   b.b1,
                                                            {0,0,0,0,0,0,0,0},
                                                            result),
                                                    ADD8(   MUX8(   b.b1,
                                                                    {0,0,0,0,0,0,0,0},
                                                                    result),
                                                            SHIFTL( result,
                                                                    {0,0,0,0,0,0,0,1}))),
                                            SHIFTL( result,
                                                    {0,0,0,0,0,0,1,0}))),
                            SHIFTL( result,
                                    {0,0,0,0,0,0,1,1})));
    return result;
}