#include "..\include\stack.h"


const int STACK_SIZE_DEFAULT = 16;
const float CAPACITY_MULTIPLIER = 2.0;

StackError checkStackError(Stack *stk)
{
    if (stk->capacity < 0)         return NEGATIVE_CAPACITY_ERROR;
    if (stk->size     < 0)         return NEGATIVE_SIZE_ERROR;
    if (stk->data == NULL)         return DATA_NULL_ERROR;
    if (stk       == NULL)         return STRUCT_NULL_ERROR;
    if (stk->size > stk->capacity) return SIZE_CAPACITY_ERROR;
    else      /*POST IRONIYA*/     return NO_ERROR;
}


static StackError increaseCapacity(Stack* stk, const float coef)
{
    if (stk == NULL)       return STRUCT_NULL_ERROR;
    if (stk->data == NULL) return DATA_NULL_ERROR;

    CHECK_AND_DUMP(stk, stderr);

    stk->capacity = (int)((float)stk->capacity * coef);
    stk->data = (elem_t*)realloc(stk->data, sizeof(elem_t) * stk->capacity);

    #ifndef RELEASE

    for (int i = (int)((float)stk->capacity / coef); i < stk->capacity; i++)
    {
        stk->data[i] = POISON;
    }

    #endif

    CHECK_AND_DUMP(stk, stderr);

    return NO_ERROR;

}


StackError stackInit(Stack* stk, size_t capacity)
{
    if (stk == NULL)  return STRUCT_NULL_ERROR;

    stk->data = (elem_t*)malloc(capacity * sizeof(elem_t));

    CHECK_AND_DUMP(stk, stderr);
    if (stk->data == NULL) return DATA_NULL_ERROR;


    stk->capacity = capacity;
    stk->size = 0;


    #ifndef RELEASE
    for (int i = 0; i < stk->capacity; i++)
        stk->data[i] = POISON;
    #endif

    return NO_ERROR;
}

StackError stackInit(Stack* stk)
{
    return stackInit(stk, STACK_SIZE_DEFAULT);
}


StackError stackPush(Stack* stk, const elem_t elem)
{
    CHECK_AND_DUMP(stk, stderr);
    if (stk == NULL) return DATA_NULL_ERROR;


    if (stk->size >= stk->capacity) 
    {
        StackError error = increaseCapacity(stk, CAPACITY_MULTIPLIER);
        DUMP_THE_ERROR(stk, error, stderr);
    }

    stk->data[stk->size++] = elem;


    return NO_ERROR;
}

StackError stackPop(Stack* stk, elem_t* elem)
{
    CHECK_AND_DUMP(stk, stderr);
    if (elem == NULL)      return ELEM_NULL_ERROR;
    if (stk->data == NULL) return DATA_NULL_ERROR;


    // If the size is CAPACITY_MULTIPLIER^2 smaller, than the capacity,...
    if (stk->size <= (float)stk->capacity / (CAPACITY_MULTIPLIER * CAPACITY_MULTIPLIER))
        increaseCapacity(stk, 1.0/CAPACITY_MULTIPLIER); //... decrease the capacity.

    if (stk->size <= 0)
    {
        STACK_DUMP(stk, POP_OUT_OF_RANGE_ERROR, stderr);
        return POP_OUT_OF_RANGE_ERROR;
    }

    *elem = stk->data[--stk->size];


    return NO_ERROR;
}

StackError stackDtor(Stack* stk)
{
    #ifdef RELEASE

    if (stk       == NULL) return STRUCT_NULL_ERROR;
    if (stk->data == NULL) return   DATA_NULL_ERROR;

    free(stk->data);

    return NO_ERROR;

    #else

    StackError error = checkStackError(stk);
    if (error != NO_ERROR)
    {
        STACK_DUMP(stk, error, stderr);
        free(stk->data);
        return error;
    }

    for (int i = 0; i < stk->capacity; i++)
    {
        stk->data[i] = POISON;
    }
    
    return error;

    #endif
}


void stackDump(const Stack* stk, const StackError err, FILE* file, 
               const char* fileName, const size_t line, const char* funcName)
{
    fprintf(file, "----------------------------------------------------------------");
    fprintf(file, "\nSTACK[%p] called from %s(%d) function %s():\n", stk, fileName, line, funcName);
    fprintf(file, "\t\t\t  ERROR CODE: %d\n", err);

    fprintf(file, "\t *size = %d     \n", stk->size);
    fprintf(file, "\t *capacity = %d \n", stk->capacity);
    fprintf(file, "\t *data[%p]:     \n", stk->data);

    if (err != NEGATIVE_CAPACITY_ERROR)
    {
        for (int i = 0; i < stk->capacity; i++)
        {
            // <(int)log10(stk->capacity) + 1> is the amount of digits in a number.
            if (i == stk->size)              fprintf(file, "\t\t> ");
            else if (stk->data[i] == POISON) fprintf(file, "\t\tO ");
            else                             fprintf(file, "\t\t@ ");

            fprintf(file, "data[%.*d] = ", (int)log10(stk->capacity) + 1, i);
            if (stk->data[i] == POISON) fprintf(file, "POISON");
            else                        fprintf(file, ELEM_FORMAT, stk->data[i]);

            if (i == stk->size)  fprintf(file, " <\n");
            else                 fprintf(file, "  \n");
        }
    }
}
