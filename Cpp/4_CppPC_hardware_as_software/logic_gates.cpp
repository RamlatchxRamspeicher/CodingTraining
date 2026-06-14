#include "dataTypes.cpp"

//binary
BIT NAND   (BIT a, BIT b){   return                                     !(a&&b)  ;}
BIT NOT    (       BIT a){   return NAND(                a,                  a)  ;}
BIT ONE    =                          NAND(         NOT(ZERO),                ZERO)  ; // fancy way to say true
BIT AND    (BIT a, BIT b){   return  NOT(                  NAND(        a,  b))  ;}
BIT OR     (BIT a, BIT b){   return NAND(           NOT(a),NOT(             b))  ;}
BIT NOR    (BIT a, BIT b){   return  NOT(                  OR(          a,  b))  ;}
BIT XOR    (BIT a, BIT b){   return NAND(NAND(a,NAND(a,b)),NAND(NAND(a,b),  b))  ;}
BIT XNOR   (BIT a, BIT b){   return  NOT(                  XOR(         a,  b))  ;}
//tertiary
BIT AND3(BIT a, BIT b, BIT c){return AND(a,AND(b,c));}
BIT OR3 (BIT a, BIT b, BIT c){return  OR(a, OR(b,c));}

//8BIT
BYTE NOT_8(BYTE a){
    a.b128 = NOT(a.b128);
    a.b64  = NOT( a.b64);
    a.b32  = NOT( a.b32);
    a.b16  = NOT( a.b16);
    a.b8   = NOT(  a.b8);
    a.b4   = NOT(  a.b4);
    a.b2   = NOT(  a.b2);
    a.b1   = NOT(  a.b1);
    return a;
}
BYTE OR_8(BYTE a, BYTE b){
    BYTE result;
    result.b1   = OR(a.b1  ,   b.b1);
    result.b2   = OR(a.b2  ,   b.b2);
    result.b4   = OR(a.b4  ,   b.b4);
    result.b8   = OR(a.b8  ,   b.b8);
    result.b16  = OR(a.b16 ,  b.b16);
    result.b32  = OR(a.b32 ,  b.b32);
    result.b64  = OR(a.b64 ,  b.b64);
    result.b128 = OR(a.b128, b.b128);
    return result;
}
BYTE NAND_8(BYTE a, BYTE b){
    BYTE result;
    result.b1   = NAND(a.b1  ,   b.b1);
    result.b2   = NAND(a.b2  ,   b.b2);
    result.b4   = NAND(a.b4  ,   b.b4);
    result.b8   = NAND(a.b8  ,   b.b8);
    result.b16  = NAND(a.b16 ,  b.b16);
    result.b32  = NAND(a.b32 ,  b.b32);
    result.b64  = NAND(a.b64 ,  b.b64);
    result.b128 = NAND(a.b128, b.b128);
    return result;
}
BYTE AND_8(BYTE a, BYTE b){
    BYTE result;
    result.b1   = AND(a.b1  ,   b.b1);
    result.b2   = AND(a.b2  ,   b.b2);
    result.b4   = AND(a.b4  ,   b.b4);
    result.b8   = AND(a.b8  ,   b.b8);
    result.b16  = AND(a.b16 ,  b.b16);
    result.b32  = AND(a.b32 ,  b.b32);
    result.b64  = AND(a.b64 ,  b.b64);
    result.b128 = AND(a.b128, b.b128);
    return result;
}

BYTE NOR_8(BYTE a, BYTE b){
    BYTE result;
    result.b1   = NOR(a.b1  ,   b.b1);
    result.b2   = NOR(a.b2  ,   b.b2);
    result.b4   = NOR(a.b4  ,   b.b4);
    result.b8   = NOR(a.b8  ,   b.b8);
    result.b16  = NOR(a.b16 ,  b.b16);
    result.b32  = NOR(a.b32 ,  b.b32);
    result.b64  = NOR(a.b64 ,  b.b64);
    result.b128 = NOR(a.b128, b.b128);
    return result;
}
BYTE XOR_8(BYTE a, BYTE b){
    BYTE result;
    result.b1   = XOR(a.b1  ,   b.b1);
    result.b2   = XOR(a.b2  ,   b.b2);
    result.b4   = XOR(a.b4  ,   b.b4);
    result.b8   = XOR(a.b8  ,   b.b8);
    result.b16  = XOR(a.b16 ,  b.b16);
    result.b32  = XOR(a.b32 ,  b.b32);
    result.b64  = XOR(a.b64 ,  b.b64);
    result.b128 = XOR(a.b128, b.b128);
    return result;
}
BYTE XNOR_8(BYTE a, BYTE b){
    BYTE result;
    result.b1   = XNOR(a.b1  ,   b.b1);
    result.b2   = XNOR(a.b2  ,   b.b2);
    result.b4   = XNOR(a.b4  ,   b.b4);
    result.b8   = XNOR(a.b8  ,   b.b8);
    result.b16  = XNOR(a.b16 ,  b.b16);
    result.b32  = XNOR(a.b32 ,  b.b32);
    result.b64  = XNOR(a.b64 ,  b.b64);
    result.b128 = XNOR(a.b128, b.b128);
    return result;
}
BYTE AND3_8(BYTE a, BYTE b, BYTE c){
    return AND_8(a,AND_8(b,c));
}
BYTE OR3_8(BYTE a, BYTE b, BYTE c){
    return OR_8(a,OR_8(b,c));
}