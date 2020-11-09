#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "vm_interface.h"

#include "opcodes.h"

uint64_t const_pull[CONST_PULL_SIZE]; 
uint64_t stack[STACK_SIZE] = {};

uint64_t sp;

uint64_t GetSP() {
    return sp;
}

void MoveSP(int i) {
    if(i >= 0) {
	sp += (uint64_t) i;
    }
    else {
	i = -i;
	assert(sp >= (uint64_t) i);
	sp -= (uint64_t) i;
    }
}

// TODO - dynamic frame
#define FRAME_SIZE 10

#define LOG_ON
#define ASM

// Programm counter
uint8_t pc = 0;

// Constant pull for method
// TODO Get Const pool dynamicly

// Method frame for method
uint64_t frame[FRAME_SIZE];
// TODO Move to local variable
register uint64_t* curr_frame asm ("t1");
// Frame pointer
uint8_t fp = 0;

void Init() {
    // 1. Clean intruction stack
    sp = 0;
    memset(stack, 0, STACK_SIZE * sizeof(uint64_t));

    // 2. TODO - fill const pull
    memset(const_pull, 0, CONST_PULL_SIZE * sizeof(uint64_t));

    // 3. Clean Frame
    // TODO - support dynamic frames 
    curr_frame = frame;

    // 4. Initialize pc
    pc = 0;
};


// Execute bytecode sequence
void Execute(uint8_t* bc) {
    // TODO Stack owerflow
    // TODO Garbage collection
    while (1) {
        uint8_t opcode = bc[pc];

        switch (opcode) {
            case(iconst_0) : {
                // Operand Stack:
                // ... ->
                // ..., <i>
#ifdef ASM
                uint64_t tmp_0;
                uint64_t tmp_1;
                asm ("addi %0, %1, 1" :"=r"(sp) :"r"(sp));
                asm ("lw  %0, 0(%1)" :"=r"(tmp_0) :"r"((uint64_t)const_pull));
                asm ("add %0, %1, %2" :"=r"(tmp_1) :"r"(sp * 8), "r"((uint64_t)stack));
                asm ("sw  %0, 0(%1)" :  :"r"(tmp_0), "r"(tmp_1));
#else
                // C-code:
                ++sp;
                stack[sp] = const_pull[0];
#endif
                break;
            }

	    case(iadd) : {
#ifdef ASM
		uint64_t a1, a2;
		uint64_t p1, p2;
		asm ("add %0, %1, %2" :"=r"(p2) :"r"(sp * 8), "r"((uint64_t)stack));
		asm ("lw  %0, 0(%1)" :"=r"(a2) :"r"(p2));
		asm ("addi %0, %1, -1" :"=r"(sp) :"r"(sp));
		asm ("add %0, %1, %2" :"=r"(p1) :"r"(sp * 8), "r"((uint64_t)stack));
		asm ("lw  %0, 0(%1)" :"=r"(a1) :"r"(p1));
		asm ("addw %0, %1, %2" :"=r"(a1) :"r"(a1), "r"(a2));
		asm ("sw %0, 0(%1)" :  :"r"(a1), "r"(p1));
#else
		--sp;
		stack[sp] = (stack[sp] + stack[sp + 1]) % UINT32_MAX;
#endif
		break;
	    }

            case(return_) : {
                // TODO support frame removing and return from methods
                if (fp == 0) {
                    return;
                } else {
                    assert(fp < FRAME_SIZE);
                    --fp;
                }
                break;
            }
            default:
#ifdef LOG_ON
                printf("Unsupported instruction with bc = %d at pc = %d \n", opcode, pc);
#endif
                break;
        }
        ++pc;
    }
};



