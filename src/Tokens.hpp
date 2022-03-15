#pragma once
#include "stdafx.hpp"

class Keyword;
class Operator;

struct Token : IPrintable {
    enum Enum {
        Unknown = 0x7fffffff,

        End = -1, // EOF
        CmdEnd = -2, // ';'
        Comma = -3, // ','
        Colon = -4, //':'
        BraceOpen = -5, //'{'
        BraceClose = -6, //'}'

        Keyword = 0,
        Identifier,
        Number,
        String,
        Operator,
    };

    Enum ty;

    void* data = 0;
    size_t data_size = 0;

    Token(Enum e = Unknown) { ty = e; }

    Token(const Token& other) {
        copy(other);
    }

    bool isKeyword(::Keyword k);
    bool isOperator(::Operator op);

    ::String toString() override {
        static constexpr auto buf_size = 0x400;
        char_t buf[buf_size];
        auto tokenStr = toString(ty);
        if (data_size > 0) {
            IPrintable* ip = reinterpret_cast<IPrintable*>(data);
            swprintf(buf, buf_size, tokenStr.c_str(), ip->toString().c_str());
        } else return tokenStr;
        return buf;
    }

    template<typename T>
    bool TryGetData(T& d) {
        T* p = reinterpret_cast<T*>(data);
        if (p) {
            d = *p;
            return true;
        }
        return false;
    }

    template<typename T>
    T* getData() {
        return reinterpret_cast<T*>(data);
    }

    template<typename T>
    bool setData(T* d) {
        data_size = sizeof(T);
        data = d;
        return true;
    }

    Token operator=(const Token& other) {
        copy(other);
        return *this;
    }
private:
    ::String toString(Enum en) {
        switch (en)
        {
        case Unknown:
            return "{ Unknown }";
        case End:
            return "{ End }";
        case CmdEnd:
            return "{ CmdEnd }";
        case BraceOpen:
            return "{ BraceOpen }";
        case BraceClose:
            return "{ BraceClose }";
            // commands
        case Keyword:
            return "{ Keyword: " STRPARAM " }";
        case String:
            return "{ String, value: \"" STRPARAM "\" }";
            // primary
        case Identifier:
            return "{ Identifier, string: \"" STRPARAM "\" }";
        case Number:
            return "{ Number, value: " STRPARAM " }";
        case Operator:
            return "{ Operator, type: " STRPARAM " }";

        case Comma:
            return "{ Comma }";

        case Colon:
            return "{ Colon }";

        default:
            return "{ Unknown(unformal) }";
            break;
        }
    }

    void copy(const Token& other) {
        ty = other.ty;
        if (other.data) {
            if (data)
                free(data);
            data_size = other.data_size;
            data = malloc(other.data_size);
            memcpy(data, other.data, data_size);
        }
    }
};

#include "Keyword.hpp"
#include "Operators.hpp"

bool Token::isKeyword(::Keyword kw) {
    if (ty == Keyword)
        return getData<::Keyword>()->ty == kw.ty;
    return false;
}

bool Token::isOperator(::Operator op) {
    if (ty == Operator)
        return getData<::Operator>()->ty == op.ty;
    return false;
}