#include <stdio.h>
#include <stdlib.h>
#include "..\include\stack.h"

#define CHECK_ERROR(error)         \
do                                 \
{                                  \
    if ((error) != NO_ERROR)       \
    {                              \
        printf("error!\n");        \
        stackDump(&stk, NO_ERROR); \
        stackDtor(&stk);           \
        assert(0);                 \
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

    for (int i = 0; i < 100; i++)
    {        
        error = stackPush(&stk, i*i);
        CHECK_ERROR(error); 
    }
    stackDump(&stk, NO_ERROR);
    stk.size = 0;
    for (int i = 0; i < 90; i++)
    {
        int a = 0;
        error = stackPop(&stk, &a);
        CHECK_ERROR(error);
    }
    stackDump(&stk, NO_ERROR);

    error = stackDtor(&stk);
    CHECK_ERROR(error);
}
