#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <assert.h>
#include "vm_interface.h"
#include "opcodes.h"
#include "test.h"

int const TEST_COUNT = OPCODE_NUM;

// Execution type and number of iterations (or test ID)
void RunTest(ExecType type, int it) {
    switch (type) {
        case (ONE_TEST) : {
            Test(it);
#ifdef LOG_ON
            printf("test %d executed\r\n", it);
#endif
            break;
        }
        case (DEFAULT_SEQUENCE) : {
            for(int j = 0; j < it; ++j) {
                for(int i = 0; i < TEST_COUNT; ++i) {
                    Test(i);
                }
            }
#ifdef LOG_ON
            printf("defatult sequence - done\r\n");
#endif
            break;
        }
        case (RANDOM_SEQUENCE) : {
            time_t t = time(NULL);
            srand(t);
            for(int i = 0; i < TEST_COUNT * it; ++i) {
                int id = rand() % TEST_COUNT;
                Test(id);
            }
#ifdef LOG_ON
            printf("randomized testing - done\r\n");
#endif
            break;
        }
    }
}


void Test(int id) {
    time_t t = time(NULL);
    srand(t);

    switch(id) {
        case(iconst_0) : {
            uint8_t bytecodes[2] = {iconst_0, return_};
           
            // Initialize VM
            Init();
            uint64_t prev_sp = GetSP();
            // Fill zero const pull by random value
            uint8_t rand_value = (rand());
            const_pull[0] = rand_value;
#ifdef LOG_ON
            printf("Enter to iconst_0 test\r\n");
            printf("  rand = %d\r\n", rand_value);
            printf("  sp = %ld\r\n", prev_sp);
#endif

            // Execute bc
            Execute(bytecodes);
#ifdef LOG_ON
            printf("Exit from execution iconst_0 test\r\n");
            printf("  stack[sp] = %ld\r\n", stack[GetSP()]);
            printf("  sp = %ld\r\n", GetSP());
#endif
            assert(GetSP() == prev_sp + 1);
            assert(stack[GetSP()] == rand_value);
            // Create bytecode:
            break; 
        }

        case(iadd) : {
            uint8_t bytecodes[2] = {iadd, return_};
           
            // Initialize VM
            Init();
            uint64_t prev_sp = GetSP();
	    uint32_t rv1, rv2;
	    rv1 = rand() + rand();
	    rv2 = rand() + rand();
            stack[prev_sp] = rv1;
	    MoveSP(1);
	    prev_sp++;
	    assert(prev_sp == GetSP());
	    stack[prev_sp] = rv2; 
#ifdef LOG_ON
            printf("Enter to iadd test\r\n");
            printf("  rv1 = %u,  rv2 = %u\r\n", rv1, rv2);
            printf("  sp = %lu\r\n", prev_sp);
#endif

            // Execute bc
            Execute(bytecodes);
#ifdef LOG_ON
            printf("Exit from execution iadd test\r\n");
            printf("  stack[sp] = %lu\r\n", stack[GetSP()]);
            printf("  sp = %lu\r\n", GetSP());
#endif
            assert(GetSP() == prev_sp - 1);
            assert((uint32_t) stack[GetSP()] == rv1 + rv2);
            // Create bytecode:
            break; 
        }

    default:
#ifdef LOG_ON
        printf("unsupported opcode %d \r\n", id);
#endif
        break;
    }
}




