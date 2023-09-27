#include "..\include\stack.h"


static FILE* stkerr = stderr;


/**
 * @brief Check stack class for errors.
 * 
 * @param[in] stk Stack struct.
 * 
 * @return Error code.
 */
static StackError checkStackError(Stack *stk);


// TODO: move to .cpp
#ifndef RELEASE
    #define STACK_DUMP(stk, stackError) stackDump_internal((stk), (stackError), __FILE__, __LINE__, __FUNCTION__)
    /**
     * Verifies the stack structure and returns any errors.
     * 
     * @param[in]  stk  The stack structure to be checked.
     */
    #define CHECK_DUMP_AND_RETURN_ERROR(stk)                     \
    do                                                           \
    {                                                            \
        StackError defineError = checkStackError(stk);           \
        if (defineError != NO_ERROR)                             \
        {                                                        \
            STACK_DUMP((stk), defineError);                       \
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
            STACK_DUMP((stk), (error));                         \
            return (error);                                    \
        }                                                      \
    } while (0) 

#else
    #define CHECK_DUMP_AND_RETURN_ERROR ;
    #define DUMP_AND_RETURN_ERROR       ;
    #define STACK_DUMP                  ;
#endif                                          

#ifdef HASH_PROTECT

    #define CHECK_HASH_RETURN_ERROR(stk)                           \
    do                                                             \
    {                                                              \
        if ((stk)->hash != calculateStackHash((stk)))              \
        {                                                          \
            DUMP_AND_RETURN_ERROR(stk, UNREGISTERED_ACCESS_ERROR); \
        }                                                          \
    } while (0);
    
    #define UPDATE_HASH(stk)                                  \
    do                                                        \
    {                                                         \
        (stk)->hash = calculateStackHash(stk);                \
    } while (0);                                              
    

#else
    #define CHECK_HASH_RETURN_ERROR(stk) ;
    #define UPDATE_HASH(stk) ;
#endif


static StackError checkStackError(Stack *stk)
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
    if (stk->data == NULL) return MEMORY_ALLOCATION_ERROR;

    // Move the data pointer to the real data.
    stk->data = (elem_t*)((size_t)stk->data + sizeof(canary_t));
    ((canary_t*)(stk->data + stk->capacity))[0] = CANARY_VALUE;
    #else
    stk->data = (elem_t*)malloc(capacity * sizeof(elem_t));
    if (stk->data == NULL) return MEMORY_ALLOCATION_ERROR;
    #endif

    CHECK_DUMP_AND_RETURN_ERROR(stk);
    if (stk->data == NULL) return DATA_NULL_ERROR;

    #ifdef HASH_PROTECT
    stk->hash = calculateStackHash(stk);
    #endif

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
    CHECK_HASH_RETURN_ERROR(stk);
    CHECK_DUMP_AND_RETURN_ERROR(stk);
    if (stk == NULL) return DATA_NULL_ERROR;


    if (stk->size >= stk->capacity)
    {
        StackError error = increaseCapacity(stk, STACK_CAPACITY_MULTIPLIER);
        DUMP_AND_RETURN_ERROR(stk, error);
    }

    stk->data[stk->size++] = elem;

    UPDATE_HASH(stk);
    return NO_ERROR;
}


StackError stackPop(Stack* stk, elem_t* elem)
{
    CHECK_HASH_RETURN_ERROR(stk);
    CHECK_DUMP_AND_RETURN_ERROR(stk);
    if (elem == NULL)      return ELEM_NULL_ERROR;
    if (stk->data == NULL) return DATA_NULL_ERROR;
    // If the size is STACK_CAPACITY_MULTIPLIER^2 smaller, than the capacity,...
    if (stk->size <= (float)stk->capacity / (STACK_CAPACITY_MULTIPLIER * STACK_CAPACITY_MULTIPLIER))
        increaseCapacity(stk, 1.0/STACK_CAPACITY_MULTIPLIER); //... decrease the capacity.

    if (stk->size <= 0)
    {
        STACK_DUMP(stk, POP_OUT_OF_RANGE_ERROR);
        return POP_OUT_OF_RANGE_ERROR;
    }
    #ifndef RELEASE
    stk->data[stk->size] = POISON;
    #endif
    *elem = stk->data[--stk->size];
    
    UPDATE_HASH(stk);
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


void stackDump_internal(const Stack* stk, const StackError err,
               const char* fileName, const size_t line, const char* funcName)
{
    fprintf(stkerr, "----------------------------------------------------------------");
    fprintf(stkerr, "\nSTACK[%p] called from %s(%d) function %s():\n", stk, fileName, line, funcName);
    fprintf(stkerr, "\t\t\t  ERROR CODE: %d\n", err);

    #ifdef CANARY_PROTECT
    fprintf(stkerr, "\t *leftCanary:" CANARY_FORMAT "\n", stk->leftCanary);
    #endif

    fprintf(stkerr, "\t *size = %d      \n", stk->size);
    fprintf(stkerr, "\t *capacity = %d  \n", stk->capacity);
    fprintf(stkerr, "\t *data[%p]:      \n", stk->data);



    #ifdef CANARY_PROTECT
    fprintf(stkerr, "\t\t leftCanary:" CANARY_FORMAT "\n", *(canary_t*)((size_t)stk->data - sizeof(canary_t)));
    #endif
    if (err != NEGATIVE_CAPACITY_ERROR)
    {
        for (int i = 0; i < stk->capacity; i++)
        {
            // <(int)log10(stk->capacity) + 1> is the amount of digits in a number.
            if (i == stk->size)              fprintf(stkerr, "\t\t> ");
            else if (stk->data[i] == POISON) fprintf(stkerr, "\t\tO ");
            else                             fprintf(stkerr, "\t\t@ ");

            fprintf(stkerr, "data[%.*d] = ", (int)log10(stk->capacity) + 1, i);
            if (stk->data[i] == POISON) fprintf(stkerr, "POISON");
            else                        fprintf(stkerr, ELEM_FORMAT, stk->data[i]);

            if (i == stk->size)  fprintf(stkerr, " <\n");
            else                 fprintf(stkerr, "  \n");
        }
        #ifdef CANARY_PROTECT
        fprintf(stkerr, "\t\t RightCanary:" CANARY_FORMAT "\n", *(canary_t*)(stk->data + stk->capacity));
        #endif
   }
    #ifdef CANARY_PROTECT
    fprintf(stkerr, "\t *RightCanary:" CANARY_FORMAT "\n", stk->rightCanary);
    #endif
}


static unsigned long long calculateHash(const char* dataStart, const size_t size)
{
    assert(dataStart);
    unsigned long long hash = +79653421411ull; 
    for (size_t i = 0; i < size; i++)
    {
        hash += int(dataStart[i]) * i;
    }
    return hash;
}


unsigned long long calculateStackHash(const Stack* stk)
{
    unsigned long long hash = 0ull;
    hash += calculateHash((const char*)stk->data, stk->size * sizeof(elem_t));
    size_t stackSize = sizeof(&(stk->data)) + sizeof(stk->size) + sizeof(stk->capacity);
    hash += calculateHash((const char*)(&(stk->data)), stackSize);

    return hash;
}


StackError setLogFile(const char* fileName)
{
    FILE* file = fopen(fileName, "w");
    if (file == NULL)
        return OPENING_FILE_ERROR;

    stkerr = file;

    return NO_ERROR;
}
