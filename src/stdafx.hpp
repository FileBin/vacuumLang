#pragma once
#include "config.h"

#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <memory>
#include <cstddef>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <sstream>

#define ARRSIZE(o) sizeof(o)/sizeof(o[0])

#ifdef USE_WSTR
    #define FILE_STREAM STD wifstream
    #define LEN(s) wcslen(s)
    #define PRINT(fmt, a...) wprintf(fmt, a)
    #define SPRINT(buf, size, fmt, a...) swprintf(buf, size, fmt, a)
    #define STRPARAM "%ls"
    #define SPREF L""
    typedef wchar_t char_t;
#else
    #define FILE_STREAM STD ifstream
    #define LEN(s) strlen(s)
    #define PRINT(fmt, a...) printf(fmt, a)
    #define SPRINT(buf, size, fmt, a...) sprintf(buf, size, fmt, a)
    #define STRPARAM "%s"
    #define SPREF 
    #define STR(s) #s
    typedef char char_t;
#endif

#define STR(s) SPREF #s

#define TSTR char_t*
#define CSTR char*
#define CTSTR const TSTR
#define CCSTR const CSTR
#define CEXPRSTR constexpr char_t const*
#define CEXPRCSTR constexpr char const*


#define interface struct
#define STD ::std::

#include "Util.hpp"
#include "Functions.hpp"