#include <stdio.h>
#include <stdlib.h>
#include "../include/stack.h"

#define CHECK_ERROR(error)         \
{                                  \
    if ((error) != NO_ERROR)       \
    {                              \
        printf("error!\n");        \
        stackDtor(&stk);           \
        return error;              \
    }                              \
}

int main()
{    
    
    StackError error = NO_ERROR;
    Stack stk = {};

    //error = setLogFile("logs.txt");
    //CHECK_ERROR(error);

    error = stackInit(&stk, 16);
    CHECK_ERROR(error);


    stackDump(&stk, NO_ERROR);

    error = stackPush(&stk, 1);
    CHECK_ERROR(error); 
    error = stackPush(&stk, 2);
    CHECK_ERROR(error);
    for (int i = 0; i < 10; i++)
    {        
        error = stackPush(&stk, i*i);
        CHECK_ERROR(error); 
    }

    stackDump(&stk, error);

    error = stackDtor(&stk);
    CHECK_ERROR(error);
}