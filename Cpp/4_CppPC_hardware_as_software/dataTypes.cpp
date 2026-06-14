#include <cstdint>
//types
typedef bool BIT;
//need this to start dont have actual transistor
BIT ZERO=0;
struct BYTE{BIT b128 = ZERO;
            BIT b64  = ZERO;
            BIT b32  = ZERO;
            BIT b16  = ZERO;
            BIT b8   = ZERO;
            BIT b4   = ZERO;
            BIT b2   = ZERO;
            BIT b1   = ZERO;};
struct fourBit{BIT b8;BIT b4;BIT b2;BIT b1;};
#define NULLBYTE BYTE{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
struct SHORT{BYTE upper; BYTE lower;};
#define NULLSHORT SHORT{NULLBYTE,NULLBYTE}
struct INT{BYTE B3; BYTE B2; BYTE B1; BYTE B0;};
#define NULLINT INT{NULLBYTE,NULLBYTE,NULLBYTE,NULLBYTE}


//conversions
BYTE BYTE8(uint8_t i){
    return BYTE{
        i>127,
        (i%128)>63,
        (i% 64)>31,
        (i% 32)>15,
        (i% 16)> 7,
        (i%  8)> 3,
        (i%  4)> 1,
        (i%  2)> 0
    };
}

uint8_t toUnsigned8(int8_t i){
    return (i>=0) ? i : i+128;
}
int8_t toSigned8(uint8_t i){
    return sINT8(BYTE8(i));
}
uint8_t uINT8(BYTE bits){
    return (uint8_t)128*bits.b128+64*bits.b64+32*bits.b32+16*bits.b16+8*bits.b8+4*bits.b4+2*bits.b2+1*bits.b1;
}
int8_t sINT8(BYTE bits){
    return (int8_t)-128*bits.b128+64*bits.b64+32*bits.b32+16*bits.b16+8*bits.b8+4*bits.b4+2*bits.b2+1*bits.b1;
}

SHORT SHORT16(uint16_t i){
    return SHORT{
        BYTE8((i >> 8)  & 0xFF),
        BYTE8( i        & 0xFF)
    };
}
uint16_t uSHORT(SHORT i){
    return (((uint16_t)uINT8(i.upper) << 8)+(uint16_t)uINT8(i.lower));
}

INT INT32(uint32_t i){
    return INT{
        BYTE8((i >> 24) & 0xFF),
        BYTE8((i >> 16) & 0xFF),
        BYTE8((i >> 8)  & 0xFF),
        BYTE8( i        & 0xFF)
    };
}
uint32_t uINT32(INT i){
    return (((uint32_t)uINT8(i.B3) << 24)+((uint32_t)uINT8(i.B2) << 16)+((uint32_t)uINT8(i.B1) << 8)+(uint32_t)uINT8(i.B0));
}