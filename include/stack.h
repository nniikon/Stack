#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


// SETTINGS

//#define RELEASE

typedef double elem_t;
#define ELEM_FORMAT "%lg"
static const elem_t POISON = 5.2365478;

static const int STACK_SIZE_DEFAULT = 16;
static const float CAPACITY_MULTIPLIER = 2.0;

static FILE* stkerr = stderr;


/// @brief Stack errors.
enum StackError
{
    NO_ERROR,                ///< No error.
    DATA_NULL_ERROR,         ///< Data is NULL.
    ELEM_NULL_ERROR,         ///< Elem is NULL.
    STRUCT_NULL_ERROR,       ///< Struct is NULL.  
    NEGATIVE_SIZE_ERROR,     ///< Negative size.
    NEGATIVE_CAPACITY_ERROR, ///< Negative capacity.
    SIZE_CAPACITY_ERROR,     ///< Size is greater than capacity.
    MEMORY_ALLOCATION_ERROR, ///< Memory allocation error.
    POP_OUT_OF_RANGE_ERROR,  ///< Pop was used at size zero. 
    OPENING_FILE_ERROR,      ///< Failed opening a file.
};


/// @brief Basic stack struct.
struct Stack
{
    elem_t* data; ///< Data array.
    int size;     ///< Current stack index.
    int capacity; ///< Current max size of the stack.
};


#ifndef RELEASE
    #define STACK_DUMP(stk, stackError, file) stackDump((stk), (stackError), (file), __FILE__, __LINE__, __FUNCTION__)
    
    /**
     * Verifies the stack structure and returns any errors.
     * 
     * @param[in]  stk  The stack structure to be checked.
     * @param[out] file The file where error information should be put.
     */
    #define CHECK_DUMP_AND_RETURN_ERROR(stk, file)             \
    do                                                         \
    {                                                          \
        if (checkStackError(stk) != NO_ERROR)                  \
        {                                                      \
            STACK_DUMP((stk), checkStackError((stk)), (file)); \
            return checkStackError((stk));                     \
        }                                                      \
    } while (0) 

    /**
     * Dumps the stack error and returns any errors.
     * 
     * @param[in]  stk   The stack structure to be dumped.
     * @param[in]  error The error to be dumped.
     * @param[out] file  The file where error information should be put.
     */
    #define DUMP_AND_RETURN_ERROR(stk, error, file)            \
    do                                                         \
    {                                                          \
        if ((error) != NO_ERROR)                               \
        {                                                      \
            STACK_DUMP((stk), (error), (file));                \
            return (error);                                    \
        }                                                      \
    } while (0) 

#else
    #define CHECK_DUMP_AND_RETURN_ERROR ;
    #define DUMP_AND_RETURN_ERROR       ;
    #define STACK_DUMP                  ;
#endif                                          


/**
 * @brief Initializes a stack structure.
 * 
 * @param[out] stk  Stack struct.
 * @param[in]  size Stack size by default.
 * 
 * @return Error code.
 * 
 * @note Don't forget to call `stackDtor` when you're done.
 */
StackError stackInit(Stack* stk, size_t capacity);


/**
 * @brief Initializes a stack structure.
 * 
 * @param[out] stk Stack struct.
 * 
 * @return Error code.
 * 
 * @note Don't forget to call `stackDtor` when you're done.
 */
StackError stackInit(Stack* stk);

/**
 * @brief Puts another element into the stack, allocating more memory if needed.
 * 
 * @param[in] elem  The element
 * @param[out] stk  The stack
 * 
 * @return Error code.
*/
StackError stackPush(Stack* stk, const elem_t elem);

/**
 * @brief Puts another element into the stack, allocating more memory if needed.
 * 
 * @param[in] The element
 * @param[out] The stack
 * 
 * @return Error code.
*/
StackError stackPop(Stack* stk, elem_t* elem);


/**
 * @brief Destructor for stack structure.
 * 
 * @param stk[in] Stack struct
 * 
 * @return Error code.
*/
StackError stackDtor(Stack* stk);


StackError checkStackError(Stack *stk);


void stackDump(const Stack* stk, const StackError err, FILE* file, 
               const char* fileName, const size_t line, const char* funcName);


StackError setLogFile(const char* fileName);

#endif