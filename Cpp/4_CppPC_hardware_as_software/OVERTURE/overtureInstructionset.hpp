#pragma once
#include <cstdint>
 
// ─── Register IDs ────────────────────────────────────────────────────────────
#define R0  0
#define R1  1
#define R2  2
#define R3  3
#define R4  4
#define R5  5
#define R6  6
#define IN  7   // Input  (readFrom only)
#define OUT 7   // Output (writeTo only)
 
// ─── DIRECT: lädt Immediate-Wert (0-63) in REG0 ─────────────────────────────
// Opcode: 00xxxxxx  (obere 2 Bits = 00)
#define DIRECT(imm)         ((uint8_t)((imm) & 0x3F))
 
// ─── CALCULATE: ALU-Operation auf REG1, REG2 → REG3 ─────────────────────────
// Opcode: 01000xxx  (obere 2 Bits = 01, Bits 3-7 = ALU-Op)
#define _ALU_OR             0
#define _ALU_NAND           1
#define _ALU_NOR            2
#define _ALU_AND            3
#define _ALU_ADD            4
#define _ALU_SUB            5
#define _ALU_XOR            6
 
#define CALCULATE(op)       ((uint8_t)(0x40 | ((op) & 0x07)))
#define CALC_OR             CALCULATE(_ALU_OR)
#define CALC_NAND           CALCULATE(_ALU_NAND)
#define CALC_NOR            CALCULATE(_ALU_NOR)
#define CALC_AND            CALCULATE(_ALU_AND)
#define CALC_ADD            CALCULATE(_ALU_ADD)
#define CALC_SUB            CALCULATE(_ALU_SUB)
#define CALC_XOR            CALCULATE(_ALU_XOR)
 
// ─── COPY: kopiert von Register/Input nach Register/Output ───────────────────
// Opcode: 10rrrwww  (r=readFrom, w=writeTo)
#define COPY(from, to)      ((uint8_t)(0x80 | (((from) & 0x07) << 3) | ((to) & 0x07)))
 
// ─── CONDITION: bedingter Sprung zu Adresse in REG0 ──────────────────────────
// Opcode: 11000ccc  (c=Bedingung, prüft REG3)
#define _COND_NEVER         0   // nie springen
#define _COND_EQ_ZERO       1   // REG3 == 0
#define _COND_LT_ZERO       2   // REG3 <  0
#define _COND_LE_ZERO       3   // REG3 <= 0
#define _COND_ALWAYS        4   // immer springen
#define _COND_NE_ZERO       5   // REG3 != 0
#define _COND_GE_ZERO       6   // REG3 >= 0
#define _COND_GT_ZERO       7   // REG3 >  0
 
#define CONDITION(cond)     ((uint8_t)(0xB8 | ((cond) & 0x07)))
#define JMP_NEVER           CONDITION(_COND_NEVER)
#define JMP_IF_EQ_ZERO      CONDITION(_COND_EQ_ZERO)
#define JMP_IF_LT_ZERO      CONDITION(_COND_LT_ZERO)
#define JMP_IF_LE_ZERO      CONDITION(_COND_LE_ZERO)
#define JMP_ALWAYS          CONDITION(_COND_ALWAYS)
#define JMP_IF_NE_ZERO      CONDITION(_COND_NE_ZERO)
#define JMP_IF_GE_ZERO      CONDITION(_COND_GE_ZERO)
#define JMP_IF_GT_ZERO      CONDITION(_COND_GT_ZERO)
 
// ─── Sprung zu Label-Adresse (2 Instruktionen) ───────────────────────────────
// Lädt Adresse in REG0 und springt
#define JMP_TO(addr, cond)  DIRECT(addr), CONDITION(cond)
#define GOTO(addr)          JMP_TO(addr, _COND_ALWAYS)