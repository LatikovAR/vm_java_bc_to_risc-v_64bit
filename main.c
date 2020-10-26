#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#ifndef NO_FREERTOS
#include <devices.h>
#include <task.h>
#endif

#include "opcodes.h"
#include "test.h"

void vTask() {
    printf(" Task created\n");
    while (1) {
        RunTest(ONE_TEST, iadd);
#ifndef NO_FREERTOS
        vTaskDelay(2000 / portTICK_RATE_MS);
#endif
    }
}


int main()
{

    // Get char - to do not start programm immediatly 
    //getchar();
    printf("%u %u\n", RAND_MAX, INT32_MAX);

    puts("Interpreter executed\n");

    // Call default test case
    getchar();

#ifndef NO_FREERTOS
    xTaskCreate(vTask, "vTask", 20480, NULL, 3, NULL);

    while(1);
#else
    vTask();
#endif

    return 0;
}


