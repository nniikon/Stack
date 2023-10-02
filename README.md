# Stack Algorithm

This is a simple C program that implements a stack data structure with added security features, including canary protection and hash protection. The program provides all the basic stack functions.

## Table of Contents

- [Introduction](#introduction)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Functions](#functions)
  - [stackDump](#stackdump)
  - [stackInit](#stackinit)
  - [stackPush](#stackpush)
  - [stackPop](#stackpop)
  - [stackDtor](#stackdtor)
  - [setLogFile](#setlogfile)
- [Settings](#settings)
- [Error Codes](#error-codes)
- [Examples](#examples)

## Introduction

This C program provides a stack data structure with built-in protection mechanisms, such as canary protection and hash protection. These mechanisms help detect and handle errors that may occur during stack operations, enhancing the security and reliability of the stack. In addition, an html dump is generated for each error detected.

## Getting Started

To use this stack algorithm in your C project, follow these steps:

1. Clone the repository or download the source code.

2. Include the `stack.h` header file in your project:

   ```c
   #include "stack.h"
   ```

3. Implement the functions according to your requirements.

4. Compile your project, ensuring that you link it with the `stack.cpp` source file.

## Usage

Here's a brief overview of how to use the provided functions in your C program:

1. Initialize a stack using `stackInit`:

   ```c
   Stack myStack;
   StackError initError = stackInit(&myStack);
   if (initError != NO_ERROR) {
       // Handle initialization error
   }
   ```

2. Push elements onto the stack using `stackPush`:

   ```c
   elem_t element = 42;  // Replace with your element
   StackError pushError = stackPush(&myStack, element);
   if (pushError != NO_ERROR) {
       // Handle push error
   }
   ```

3. Pop elements from the stack using `stackPop`:

   ```c
   elem_t poppedElement;
   StackError popError = stackPop(&myStack, &poppedElement);
   if (popError != NO_ERROR) {
       // Handle pop error
   }
   ```

4. Perform any other necessary stack operations.

5. When done, release the stack memory using `stackDtor`:

   ```c
   StackError dtorError = stackDtor(&myStack);
   if (dtorError != NO_ERROR) {
       // Handle destructor error
   }
   ```

## Functions

### stackDump()

```c
#define stackDump(stk, stackError) stackDump_internal((stk), (stackError), __FILE__, __LINE__, __FUNCTION__)
```

- Description: dumps all relevant stack information along with the given error to the log file (stderr by default).
- Parameters:
  - `stk` - The stack struct.
  - `stackError` - The error code.
- Note: you don't need to call `stackDump()` on error, it's done automatically.

### stackInit()

```c
#define stackInit(stk) stackInit_internal((stk), StackInitInfo{__FILE__, #stk, __FUNCTION__, __LINE__})
```

- Description: initializes a stack structure.
- Parameters:
  - `stk` - Stack struct.
- Returns: Error code.

### stackPush()

```c
StackError stackPush(Stack* stk, const elem_t elem);
```

- Description: Puts another element into the stack, allocating more memory if needed.
- Parameters:
  - `stk` - The stack.
  - `elem` - The element to push.
- Returns: Error code.

### stackPop()

```c
StackError stackPop(Stack* stk, elem_t* elem);
```

- Description: Puts another element into the stack, allocating more memory if needed.
- Parameters:
  - `stk` - The stack.
  - `elem` - Pointer to the variable where the popped element will be stored.
- Returns: Error code.

### stackDtor()

```c
StackError stackDtor(Stack* stk);
```

- Description: Destructor for stack structure.
- Parameters:
  - `stk` - Stack struct.
- Returns: Error code.

### setLogFile()

```c
StackError setLogFile(const char* fileName);
```

- Description: Automatically creates a log file with the specified name.
- Parameters:
  - `fileName` - The name of the log file.
- Note: Don't forget to call `stackDtor` when you're done to close the file.

## Settings

This section describes the configurable settings and constants in the code:

### Release Mode

- `RELEASE` mode optimizes the code for performance.
- In this mode, unused hash values are not filled with poison.
- Validation is performed only on essential parameters.

### Canary Protection

- `CANARY_PROTECT` enhances security by using canary values to protect the stack and data.
- While enabling canary protection, keep in mind that it slightly increases the execution time of every stack operation.
- It may also require a slightly larger amount of memory due to the additional canary values.

### Hash Protection

- `HASH_PROTECT` strengthens security using a hash function to protect the stack and data.
- Enabling hash protection significantly increases the execution time of every stack operation.

### Data Types

- `elem_t` is the element type used in the stack.
- `canary_t` is the canary type used for data protection.

### Format Specifiers

- `ELEM_FORMAT` defines the format specifier for printing elements using `printf()`.
- `CANARY_FORMAT` defines the format specifier for printing canary values.

### Poison Value

- `POISON` is the value used to mark uninitialized elements in the stack.

### Canary Value

- `CANARY_VALUE` is the canary value used to protect data integrity.

### Default Stack Capacity

- `STACK_SIZE_DEFAULT` defines the default minimum capacity that the stack can have.

### Stack Capacity Multiplier

- `STACK_CAPACITY_MULTIPLIER` specifies the multiplier used to increase the capacity of the stack when needed.


## Error Codes

The functions in this stack algorithm may return the following error codes:

- `NO_ERROR` - no error occurred.
- `DATA_NULL_ERROR` - data is NULL, indicating an uninitialized stack.
- `ELEM_NULL_ERROR` - elem is NULL, which is unexpected.
- `STRUCT_NULL_ERROR` - struct is NULL, indicating an uninitialized stack structure.
- `NEGATIVE_SIZE_ERROR` - size cannot be negative.
- `NEGATIVE_CAPACITY_ERROR` - capacity cannot be negative.
- `SIZE_CAPACITY_ERROR` - size should not exceed capacity.
- `MEMORY_ALLOCATION_ERROR` - error during memory allocation (malloc or realloc).
- `POP_OUT_OF_RANGE_ERROR` - attempted pop operation on an empty stack.
- `OPENING_FILE_ERROR` - failed to open a file.
- `DEAD_STRUCT_CANARY_ERROR` - struct canary value indicates a possible stack attack.
- `DEAD_DATA_CANARY_ERROR` - data canary value indicates a possible stack attack.
- `UNREGISTERED_STRUCT_ACCESS_ERROR` - struct hash mismatch due to unauthorized data manipulation.
- `UNREGISTERED_DATA_ACCESS_ERROR` - data hash mismatch due to unauthorized data manipulation.

## Examples

You can find usage examples and additional information in the provided code files and documentation. (work in progress)
