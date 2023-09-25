#include <stdio.h>
#include <stdlib.h>
#include "..\include\stack.h"

#define CHECK_ERROR(error)   \
do                           \
{                            \
    if ((error) != NO_ERROR) \
    {                        \
        stackDtor(&stk);     \
        abort();             \
    }                        \
} while (0);               




int main()
{
    StackError error = NO_ERROR;
    Stack stk = {};
    setLogFile("logs.txt");
    error = stackInit(&stk, 16);
    CHECK_ERROR(error)

    error = setLogFile("logs.txt");
    CHECK_ERROR(error)

    stackDump(&stk);

    for (int i = 0; i < 100; i++)
    {
        error = stackPush(&stk, i*i);
        CHECK_ERROR(error)   
        printf("hash: %llu\n", calculateStackHash(&stk));
    }

    for (int i = 0; i < 90; i++)
    {
        int a = 0;
        error = stackPop(&stk, &a);
        CHECK_ERROR(error)
    }
    stackDump(&stk);

    error = stackDtor(&stk);
    CHECK_ERROR(error);
}
