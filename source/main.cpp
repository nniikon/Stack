#include <stdio.h>
#include <stdlib.h>
#include "..\include\stack.h"


int main()
{
    Stack stk = {};
    stackInit(&stk, 16);

    for (int i = 0; i < 100; i++) 
    {
        StackError error = stackPush(&stk, i*i);
        if (error != NO_ERROR)
        {
            return error;
        }
    }
    // for (int i = 0; i < 99; i++)
    // {
    //     elem_t a = 0;
    //     StackError error = stackPop(&stk, &a);
    //     printf(": %lg\n", a);
    //     if (error != NO_ERROR)
    //     {
    //         return error;
    //     }
    // }
    DUMP_THE_ERROR(&stk, MEMORY_ALLOCATION_ERROR, stderr);
    stackDtor(&stk);
}