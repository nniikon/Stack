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
static const elem_t POISON = __INT_MAX__;
#define CANARY_FORMAT "%llx"
static const canary_t CANARY_VALUE = 0xBAADF00D;

static const int STACK_SIZE_DEFAULT = 16;
static const float STACK_CAPACITY_MULTIPLIER = 2.0;

#define ERROR_NAME(func)\
        func(NO_ERROR)\
        func(DATA_NULL_ERROR)\
        func(ELEM_NULL_ERROR)\
        func(STRUCT_NULL_ERROR)\
        func(NEGATIVE_SIZE_ERROR)\
        func(NEGATIVE_CAPACITY_ERROR)\
        func(SIZE_CAPACITY_ERROR)\
        func(MEMORY_ALLOCATION_ERROR)\
        func(POP_OUT_OF_RANGE_ERROR)\
        func(OPENING_FILE_ERROR)\
        func(DEAD_STRUCT_CANARY_ERROR)\
        func(DEAD_DATA_CANARY_ERROR)\
        func(UNREGISTERED_STRUCT_ACCESS_ERROR)\
        func(UNREGISTERED_DATA_ACCESS_ERROR)\

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

//           NO_ERROR,                         ///< No error occurred.
//           DATA_NULL_ERROR,                  ///< Data is NULL, indicating an uninitialized stack.
//           ELEM_NULL_ERROR,                  ///< Elem is NULL, which is unexpected.
//           STRUCT_NULL_ERROR,                ///< Struct is NULL, indicating an uninitialized stack structure.
//           NEGATIVE_SIZE_ERROR,              ///< Size cannot be negative.
//           NEGATIVE_CAPACITY_ERROR,          ///< Capacity cannot be negative.
//           SIZE_CAPACITY_ERROR,              ///< Size should not exceed capacity.
//           MEMORY_ALLOCATION_ERROR,          ///< Error during memory allocation (malloc or realloc).
//           POP_OUT_OF_RANGE_ERROR,           ///< Attempted pop operation on an empty stack.
//           OPENING_FILE_ERROR,               ///< Failed to open a file.
//           DEAD_STRUCT_CANARY_ERROR,         ///< Struct canary value indicates a possible stack attack.
//           DEAD_DATA_CANARY_ERROR,           ///< Data canary value indicates a possible stack attack.
//           UNREGISTERED_STRUCT_ACCESS_ERROR, ///< Struct hash mismatch due to unauthorized data manipulation.
//           UNREGISTERED_DATA_ACCESS_ERROR,   ///< Data hash mismatch due to unauthorized data manipulation.


enum StackError
{
    ERROR_NAME(GENERATE_ENUM)
};


struct StackInitInfo
{
    const char* fileName;
    const char* varName;
    const char* funcName;
    int lineNum;
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
    StackInitInfo info; ///< Stack initialization info.

    #ifdef HASH_PROTECT
    unsigned long long dataHash;
    unsigned long long structHash;
    #endif

    #ifdef CANARY_PROTECT
    canary_t rightCanary;
    #endif
};



#define stackInit(stk) stackInit_internal((stk), StackInitInfo{__FILE__, #stk, __FUNCTION__, __LINE__})


/**
 * @brief Dump all relevant stack information with the given error to the log file (stderr by default).
 * 
 * @param[in] stk The stack struct.
 * @param[in] StackError The error code.
 * 
 * @note You can change the log file using `setLogFile("FILE_NAME")`.
 */
#define stackDump(stk, stackError) stackDump_internal((stk), (stackError), __FILE__, __LINE__, __FUNCTION__)

void stackDump_internal(const Stack* stk, const StackError err,
               const char* fileName, const size_t line, const char* funcName);

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
StackError stackInit_internal(Stack* stk, size_t capacity, StackInitInfo info);


/**
 * @brief Initializes a stack structure.
 * 
 * @param[out] stk Stack struct.
 * 
 * @return Error code.
 * 
 * @note Don't forget to call `stackDtor` when you're done.
 */
StackError stackInit_internal(Stack* stk, StackInitInfo info);


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


StackError setLogFile(const char* fileName);

#endif