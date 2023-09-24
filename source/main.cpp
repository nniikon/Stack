#include <stdio.h>
#include <stdlib.h>
#include "..\include\stack.h"


int main()
{

    StackError error = NO_ERROR;
    Stack stk = {};
    setLogFile("logs.txt");
    error = stackInit(&stk, 16);
    (*(canary_t*)(stk.data + stk.capacity))--;
    error = setLogFile("logs.txt");
    stackDump(&stk);

    for (int i = 0; i < 100; i++)
    {
        error = stackPush(&stk, i*i);
    }
    stackDump(&stk);

    //stackDtor(&stk);
}
