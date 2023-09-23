#include <stdio.h>
#include <stdlib.h>
#include "..\include\stack.h"


int main()
{
    StackError error = NO_ERROR;

    Stack stk = {};
    stackInit(&stk, 16);

    error = setLogFile("log.txt");
    DUMP_AND_RETURN_ERROR(&stk, error, stkerr);

    for (int i = 0; i < 100; i++) 
    {
        error = stackPush(&stk, i*i);
    }
    DUMP_AND_RETURN_ERROR(&stk, NEGATIVE_SIZE_ERROR, stkerr);
    stackDtor(&stk);
}