#ifndef SKANDAR_COMMON_HPP
#define SKANDAR_COMMON_HPP
#include <stdio.h>
#include <string>


#define SKANDAR_DEBUG 0

#if SKANDAR_DEBUG
    #define PRINT_ASSERT_ERR(condition)    \
        fprintf(stderr,                    \
            "%s(%ld): assertion failed\n"  \
            "%s\n",                        \
            __FILE__, __LINE__, #condition \
        )

    #define ASSERT(condition)            \
        if(!(condition)) {               \
            PRINT_ASSERT_ERR(condition); \
            exit(1);                     \
        }

    #define ASSERTM(condition, format, ...)       \
        if(!(condition)) {                        \
            PRINT_ASSERT_ERR(condition);          \
            fprintf(stderr, format, __VA_ARGS__); \
            exit(1);                              \
        }
#else
    #define PRINT_ASSERT_ERR(condition)
    #define ASSERT(condition)
    #define ASSERTM(condition, format, ...)
#endif

typedef char                int8;
typedef unsigned char      uint8;
typedef unsigned long long uint64;
typedef long long          int64;
typedef unsigned long      ulong;

#endif // SKANDAR_COMMON_HPP