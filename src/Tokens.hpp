#pragma once
// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
#include "stdafx.hpp"

struct Token
{
    enum Enum
    {
        Unknown = 0x7fffffff,
        End = -1,
        CmdEnd = -2,
        Comma = -3,
        Colon = -4,

        // primary
        Keyword = -0xff,
        Type,
        Identifier,
        Number,
        String,
        Operator,
    };

    union {
        Enum ty;
        int val;
    };

    void* data = 0;
    size_t data_size = 0;

    Token(Enum e = Unknown) { ty = e; }

    Token(const Token &other) {
        copy(other);
    }

    ::String ToString(){
        static constexpr auto buf_size = 0x400*sizeof(wchar_t);
        //wchar_t* buf = (wchar_t*)malloc(buf_size);
        //memset(buf, 0, buf_size);
        wchar_t buf[0x400];
        const wchar_t* tokenStr = toString(ty);
        if(data_size > 0) {
            IPrintable *ip = reinterpret_cast<IPrintable*>(data);
            swprintf(buf, 0x400, tokenStr, ip->ToString().c_str());
        }
        else return tokenStr;
        return buf;
    }

    template<typename T>
    bool TryGetData(T& d){
        T* p = reinterpret_cast<T*>(data);
        if(p) {
            d = *p;
            return true;
        }
        return false;
    }

    template<typename T>
    T* GetData(){
        return reinterpret_cast<T*>(data);
    }

    template<typename T>
    bool SetData(T* d) {
        data_size = sizeof(T);
        data = d;
        return true;
    }

    Token operator=(const Token &other) {
        copy(other);
        return *this;
    }
private:
    const wchar_t* toString(Enum en){
        switch (en)
        {
        case Unknown:
            return L"{ Unknown }";
        case End:
            return L"{ End }";
        case CmdEnd:
            return L"{ CmdEnd }";
        // commands
        case Keyword:
            return L"{ Keyword: %ls }";
        case String:
            return L"{ String, value: \"%ls\" }";
        // primary
        case Type:
            return L"{ Type, type: %ls }";
        case Identifier:
            return L"{ Identifier, string: \"%ls\" }";
        case Number:
            return L"{ Number, value: %ls }";
        case Operator:
            return L"{ Operator, type: %ls }";

        case Comma:
            return L"{ Comma }";

        case Colon:
            return L"{ Colon }";
        
        default:
            return L"Unknown(unformal)";
            break;
        }
    }

    void copy(const Token& other) {
        val = other.val;
        if(other.data) {
            if(data)
                free(data);
            data_size = other.data_size;
            data = malloc(other.data_size);
            memcpy(data, other.data, data_size);
        }
    }
};