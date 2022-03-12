#pragma once

#include <cstdlib>
#include <string>
#include <cstring>
#include <memory>
#include <cstddef>
#include <string>
#include <map>
#include <stack>
#include <vector>

#define USE_WSTR

#define ARRSIZE(o) sizeof(o)/sizeof(o[0])

#ifdef USE_WSTR
    #define LEN(s) wcslen(s)
    #define SPRINT(buf, size, fmt, a...) swprintf(buf, size, fmt, a);
    #define STRPARAM
    #define SPREF L""
    #define STR(s) SPREF #s
    typedef wchar_t char_t;
#else
    #define LEN(s) strlen(s)
    #define STR(s) #s
    typedef char char_t;
#endif

#define TSTR char_t*
#define CTSTR const TSTR
#define CEXPRSTR constexpr char_t const*


#define interface struct
#define STD ::std::

#include "Util.hpp"