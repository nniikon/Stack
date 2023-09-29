#include <stdio.h>
#include <stdlib.h>
#include "../include/stack.h"

#define CHECK_ERROR(error)         \
do                                 \
{                                  \
    if ((error) != NO_ERROR)       \
    {                              \
        printf("error!\n");        \
        stackDtor(&stk);           \
        return error;              \
    }                              \
} while (0)      




int main()
{    
    StackError error = NO_ERROR;
    Stack stk = {};

    error = setLogFile("logs.txt");
    CHECK_ERROR(error);

    error = stackInit(&stk, 16);
    CHECK_ERROR(error);
    stackDump(&stk, NO_ERROR);

    for (int i = 0; i < 120; i++)
    {        
        error = stackPush(&stk, i*i);
        CHECK_ERROR(error); 
    }

    for (int i = 0; i < 121; i++)
    {        
        elem_t a = 0;
        error = stackPop(&stk, &a);
        CHECK_ERROR(error); 
    }    

    stackDump(&stk, error);

    error = stackDtor(&stk);
    CHECK_ERROR(error);
}