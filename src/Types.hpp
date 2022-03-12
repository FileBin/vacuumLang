#pragma once

#include "stdafx.hpp"

struct Member;

namespace Constant {
    CEXPRSTR types[]{
        STR(void),
        STR(bool),
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
        STR(bigdbl),

        STR(char),

        STR(object),
    };
}

class Type : public IPrintable {
public:
    typedef Member* PMember;
    enum Enum {
        Class = -2,
        Unknown = -1,

        //integers
        Void = 0,
        Bool,
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
        BigDbl,

        //strings
        Char, //UTF-32 string character

        //Objects
        Object,
    };
protected:

    Enum type = Unknown;
    int byteWidth;

    STD vector<String> location;
    STD vector<PMember> members;
public:
    Type(Enum type_enum = Unknown) {
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
        case Class:
            byteWidth = calculateClassSize();
            break;
        default:
            byteWidth = -1; //Unknown
            break;
        }
    }

    Enum GetEnum() { return type; }

    String ToString() override {
        char_t buf[0x400];

        SPRINT(buf, ARRSIZE(buf), SPREF"{ type: %ls, byteWidth: %d })", getFullName().c_str(), byteWidth);

        return buf;
    }

    String getName() {
        return Constant::types[(size_t)type];
    }

    String getFullName() {
        String full_name = L"";
        for (const auto& name : location) {
            full_name += name + L".";
        }
        full_name += getName();
        return full_name;
    }

    String getLlvmName() {
        String llvm_name = L"";
        if (!location.empty()) {
            llvm_name = L"nsp_";
            for (const auto& name : location) {
                llvm_name += name + L"_";
            }
        }
        llvm_name += L"typ_" + getName();
        return llvm_name;
    }

    static bool tryParse(String str, Type*& t, bool strict = false) {
        using namespace Constant;
        if (!t) return false;
        Enum ty = Unknown;
        size_t len = 0;
        for (size_t i = 0; i < ARRSIZE(types); i++) {
            size_t _len = LEN(types[i]);
            if (strict && _len != str.length()) continue;
            if (len < _len && str.substr(0, _len) == types[i]) {
                len = _len;
                ty = (Enum)i;
            }
        }
        t = new Type(ty);
        return ty != Unknown;
    }

    int calculateClassSize();

    PMember getMember(String str);
};

struct Member {
public:
    enum Enum {
        Unknown = -1,
        Field = 0,
        Property,
        Function,
    };
protected:
    Enum mem_ty;
    Type ty, class_ty;
    String name;
public:
    Member(String name, Type ty, Type class_ty, Enum mem_ty) : name(name), ty(ty), class_ty(class_ty), mem_ty(mem_ty) {}

    Type GetType() {
        return ty;
    }

    Enum GetMemberType() {
        return mem_ty;
    }

    String GetName() {
        return name;
    }

    String GetFullName() {
        return class_ty.getFullName() + L"." + name;
    }

    virtual String GetLlvmName() { return L""; };
};

struct Field : public Member {
public:
    Field(String name, Type class_type, Type type)
        : Member(name, type, class_type, Member::Field) {}

    String GetLlvmName() override {
        return class_ty.getLlvmName() + L"_fld_" + name;
    }
};

struct Function : public Member {
private:
    String name;

    STD vector<Type> parameters;
public:
    Function(String name, Type class_type, Type type = Type::Void, STD vector<Type> args = {})
        : Member(name, type, class_type, Member::Function) {
        parameters = args;
    }

    String GetLlvmName() override {
        return class_ty.getLlvmName() + L"_fnc_" + name;
    }
};

struct Property : public Member {
private:
    ::Function f_get, f_set;
public:
    Property(String name, Type class_type, Type type)
        : Member(name, type, class_type, Member::Property), f_get(name + L"_get", class_type, type), f_set(name + L"_set", class_type, Type::Void, { type }) {
        ty = type;
        mem_ty = Member::Property;
    }

    String GetLlvmName() override {
        return class_ty.getLlvmName() + L"_prp_" + name;
    }
};

typedef ::Type ty;
ty::PMember ty::getMember(String str) {
    for (::Type::PMember& mem : members) {
        if(mem->GetName() == str) {
            return mem;
        }
    }
    return nullptr;
}

#include "Objects.hpp"