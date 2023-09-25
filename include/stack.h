#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


// SETTINGS

//#define RELEASE
#define CANARY_PROTECT 
#define HASH_PROTECT

typedef int elem_t;
typedef unsigned long long canary_t;

#define ELEM_FORMAT "%d"
static const elem_t POISON = __INT_FAST32_MAX__;
#define CANARY_FORMAT "%llu"
static const canary_t CANARY_VALUE = 0xBAADF00D;

static const int STACK_SIZE_DEFAULT = 16;
static const float CAPACITY_MULTIPLIER = 2.0;


/// @brief Stack errors.
enum StackError
{
    NO_ERROR,                 ///< No error.
    DATA_NULL_ERROR,          ///< Data is NULL.
    ELEM_NULL_ERROR,          ///< Elem is NULL.
    STRUCT_NULL_ERROR,        ///< Struct is NULL.  
    NEGATIVE_SIZE_ERROR,      ///< Negative size.
    NEGATIVE_CAPACITY_ERROR,  ///< Negative capacity.
    SIZE_CAPACITY_ERROR,      ///< Size is greater than capacity.
    MEMORY_ALLOCATION_ERROR,  ///< Memory allocation error.
    POP_OUT_OF_RANGE_ERROR,   ///< Pop was used at size zero. 
    OPENING_FILE_ERROR,       ///< Failed opening a file.
    DEAD_CANARY_ERROR,        ///< Canary died. 
    UNREGISTRED_ACCESS_ERROR, ///< Hash changed.
};


/// 12 debug params
/// @brief Basic stack struct.
struct Stack
{
    #ifdef CANARY_PROTECT
    canary_t leftCanary;
    #endif
    
    elem_t* data; ///< Data array.
    int size;     ///< Current stack index.
    int capacity; ///< Current max size of the stack.

    #ifdef HASH_PROTECT
    unsigned long long hash;
    #endif

    #ifdef CANARY_PROTECT
    canary_t rightCanary;
    #endif
};

// TODO: DELETE
void stackDump(Stack* stk);


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
 * @param[in] stk Stack struct.
 * 
 * @return Error code.
*/
StackError stackDtor(Stack* stk);


/**
 * @brief Check stack class for errors.
 * 
 * @param[in] stk Stack struct.
 * 
 * @return Error code.
 */
StackError checkStackError(Stack *stk);


StackError setLogFile(const char* fileName);


unsigned long long calculateStackHash(const Stack* stk);

#endif