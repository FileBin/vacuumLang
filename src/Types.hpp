#pragma once

#include "stdafx.hpp"

namespace Constant {
    CEXPRSTR types[] {
        STR(void),
        STR(sbyte),
        STR(byte),
        STR(sht), //aka. short
        STR(usht),
        STR(int),
        STR(uint),
        STR(num), //aka. long
        STR(unum),

        STR(flt),
        STR(dbl),
        STR(longdbl),

        STR(char),

        STR(object),
    };
}

class Type : public IPrintable {
public:
    enum Enum {
        Unknown = -1,
        //integers
        Void = 0,
        SByte,
        Byte,
        Short,
        UShort,
        Int,
        UInt,
        Num,
        UNum,

        //floats
        Flt,
        Dbl,
        LongDbl,

        //strings
        Char, //utf-8 string character

        //Objects
        Object,
    };
protected:
    Enum type = Unknown;
    int byteWidth;
public:
    Type(Enum type_enum = Unknown){
        type = type_enum;
        switch (type_enum)
        {
        case Void:
            byteWidth = 0;
            break;
        case Byte:
        case SByte:
            byteWidth = 1; //8bit
            break;
        case Short:
        case UShort:
            byteWidth = 2; //16bit
            break;

        case Int:
        case UInt:
        case Flt:
        case Char:
            byteWidth = 4; //32bit
            break;

        case Num:
        case UNum:
        case Dbl:
            byteWidth = 8; //64bit
            break;
        case Object:
            //TODO: Calculate obj width from map
        default:
            byteWidth = -1; //Unknown
            break;
        }
    }

    Enum GetEnum() { return type; }

    String ToString() override {
        char_t buf[0x400];

        SPRINT(buf,ARRSIZE(buf), SPREF"{ type: %ls, byteWidth: %d })", Constant::types[(size_t)type], byteWidth);

        return buf;
    }

    static bool TryParse(String str, Type* &t, bool strict = false) {
        using namespace Constant;
        if(!t) return false;
        Enum ty = Unknown;
        size_t len = 0;
        for(size_t i=0; i<ARRSIZE(types); i++){
            size_t _len = LEN(types[i]);
            if(strict && _len != str.length()) continue;
            if(len < _len && str.substr(0, _len) == types[i]) {
                len = _len;
                ty = (Enum)i;
            }
        }
        t = new Type(ty);
        return ty != Unknown;
    }
};