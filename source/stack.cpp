#include "..\include\stack.h"


static FILE* stkerr = stderr;


// TODO: move to .cpp
#ifndef RELEASE
    #define STACK_DUMP(stk, stackError) stackDump((stk), (stackError), stkerr, __FILE__, __LINE__, __FUNCTION__)
    
    /**
     * Verifies the stack structure and returns any errors.
     * 
     * @param[in]  stk  The stack structure to be checked.
     */
    #define CHECK_DUMP_AND_RETURN_ERROR(stk)                     \
    do                                                           \
    {                                                            \
        if (checkStackError(stk) != NO_ERROR)                    \
        {                                                        \
            STACK_DUMP((stk), checkStackError((stk)));           \
            return checkStackError((stk));                       \
        }                                                        \
    } while (0) 

    /**
     * Dumps the stack error and returns any errors.
     * 
     * @param[in]  stk   The stack structure to be dumped.
     * @param[in]  error The error to be dumped.
     */
    #define DUMP_AND_RETURN_ERROR(stk, error)                  \
    do                                                         \
    {                                                          \
        if ((error) != NO_ERROR)                               \
        {                                                      \
            STACK_DUMP((stk), (error));                        \
            return (error);                                    \
        }                                                      \
    } while (0) 

#else
    #define CHECK_DUMP_AND_RETURN_ERROR ;
    #define DUMP_AND_RETURN_ERROR       ;
    #define STACK_DUMP                  ;
#endif                                          


void stackDump(Stack* stk)
{
    STACK_DUMP(stk, NO_ERROR);
}



StackError checkStackError(Stack *stk)
{
#ifdef CANARY_PROTECT
    if (stk->leftCanary  != CANARY_VALUE) return DEAD_CANARY_ERROR;
    if (stk->rightCanary != CANARY_VALUE) return DEAD_CANARY_ERROR;

    if (*(canary_t*)((size_t)stk->data - sizeof(canary_t)) != CANARY_VALUE) return DEAD_CANARY_ERROR;
    if (*(canary_t*)(stk->data + stk->capacity) != CANARY_VALUE)            return DEAD_CANARY_ERROR;
#endif
    if (stk->capacity < 0)                return NEGATIVE_CAPACITY_ERROR;
    if (stk->size     < 0)                return NEGATIVE_SIZE_ERROR;
    if (stk->data == NULL)                return DATA_NULL_ERROR;
    if (stk       == NULL)                return STRUCT_NULL_ERROR;
    if (stk->size > stk->capacity)        return SIZE_CAPACITY_ERROR;
    else      /*POST IRONIYA*/            return NO_ERROR;
}


static StackError increaseCapacity(Stack* stk, const float coef)
{
    CHECK_DUMP_AND_RETURN_ERROR(stk);
    if (stk == NULL)       return STRUCT_NULL_ERROR;
    if (stk->data == NULL) return DATA_NULL_ERROR;


    stk->capacity = (int)((float)stk->capacity * coef);

    #ifdef CANARY_PROTECT
    // Move the data to the originally allocated place.
    stk->data = (elem_t*)(size_t(stk->data) - sizeof(canary_t));
    stk->data = (elem_t*)realloc(stk->data, stk->capacity * sizeof(elem_t) + 2 * sizeof(canary_t));
    ((canary_t*)stk->data)[0] = CANARY_VALUE;

    // Move the data pointer to the real data.
    stk->data = (elem_t*)((size_t)stk->data + sizeof(canary_t));
    ((canary_t*)(stk->data + stk->capacity))[0] = CANARY_VALUE;
    #else
    stk->data = (elem_t*)realloc(stk->data, sizeof(elem_t) * stk->capacity);
    #endif


    CHECK_DUMP_AND_RETURN_ERROR(stk);
    if (stk->data == NULL) return MEMORY_ALLOCATION_ERROR;


    #ifndef RELEASE

    for (int i = (int)((float)stk->capacity / coef); i < stk->capacity; i++)
    {
        stk->data[i] = POISON;
    }

    #endif

    CHECK_DUMP_AND_RETURN_ERROR(stk);

    return NO_ERROR;

}


StackError stackInit(Stack* stk, size_t capacity)
{
    if (stk == NULL)  return STRUCT_NULL_ERROR;

    stk->capacity = capacity;
    stk->size = 0;

    #ifdef CANARY_PROTECT
    stk->leftCanary  = CANARY_VALUE;
    stk->rightCanary = CANARY_VALUE;
    #endif

    #ifdef CANARY_PROTECT
    stk->data = (elem_t*)malloc(capacity * sizeof(elem_t) + 2 * sizeof(canary_t));
    ((canary_t*)stk->data)[0] = CANARY_VALUE;

    // Move the data pointer to the real data.
    stk->data = (elem_t*)((size_t)stk->data + sizeof(canary_t));
    ((canary_t*)(stk->data + stk->capacity))[0] = CANARY_VALUE;
    #else
    stk->data = (elem_t*)malloc(capacity * sizeof(elem_t));
    #endif

    CHECK_DUMP_AND_RETURN_ERROR(stk);
    if (stk->data == NULL) return DATA_NULL_ERROR;


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
    CHECK_DUMP_AND_RETURN_ERROR(stk);
    if (stk == NULL) return DATA_NULL_ERROR;


    if (stk->size >= stk->capacity)
    {
        StackError error = increaseCapacity(stk, CAPACITY_MULTIPLIER);
        DUMP_AND_RETURN_ERROR(stk, error);
    }

    stk->data[stk->size++] = elem;


    return NO_ERROR;
}


StackError stackPop(Stack* stk, elem_t* elem)
{
    CHECK_DUMP_AND_RETURN_ERROR(stk);
    if (elem == NULL)      return ELEM_NULL_ERROR;
    if (stk->data == NULL) return DATA_NULL_ERROR;


    // If the size is CAPACITY_MULTIPLIER^2 smaller, than the capacity,...
    if (stk->size <= (float)stk->capacity / (CAPACITY_MULTIPLIER * CAPACITY_MULTIPLIER))
        increaseCapacity(stk, 1.0/CAPACITY_MULTIPLIER); //... decrease the capacity.

    if (stk->size <= 0)
    {
        STACK_DUMP(stk, POP_OUT_OF_RANGE_ERROR);
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
        STACK_DUMP(stk, error);
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

    fprintf(file, "\t *size = %d      \n", stk->size);
    fprintf(file, "\t *capacity = %d  \n", stk->capacity);
    fprintf(file, "\t *data[%p]:      \n", stk->data);
    #ifdef CANARY_PROTECT
    fprintf(file, "\t leftCanary:" CANARY_FORMAT "\n", *(canary_t*)((size_t)stk->data - sizeof(canary_t)));
    #endif
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
        #ifdef CANARY_PROTECT
        fprintf(file, "\t RightCanary:" CANARY_FORMAT "\n", *(canary_t*)(stk->data + stk->capacity));
        #endif
   }
}


StackError setLogFile(const char* fileName)
{
    FILE* file = fopen(fileName, "w");
    if (file == NULL)
        return OPENING_FILE_ERROR;

    stkerr = file;

    return NO_ERROR;
}
