#pragma once

#include "stdafx.hpp"

struct Member;

namespace Constant {
    CEXPRCSTR types[]{
        "void",
        "bool",
        "sbyte",
        "byte",
        "sht", //aka. 'short'
        "usht",
        "int",
        "uint",
        "num", //aka. 'long'
        "unum",

        "flt",
        "dbl",
        "bigdbl",

        "char",

        "object",
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
    Type* super_type;
    Type* interfaces;
    Enum type = Unknown;

    STD vector<String> location;
    STD vector<PMember> members;

    virtual int getClassSize() { return 0; }
public:
    Type(Enum type_enum = Unknown) {
        type = type_enum;
    }

    int getByteSize() {
        switch (type) {
        case Object:
        case Void:
            return 0; //abstract objects

        case Byte:
        case SByte:
            return 1; //8bit

        case Short:
        case UShort:
            return 2; //16bit

        case Int:
        case UInt:
        case Flt:
        case Char:
            return 4; //32bit

        case Num:
        case UNum:
        case Dbl:
            return 8; //64bit
        case Class:
            return getClassSize();
        default:
            return -1;
        }
    }

    Enum GetEnum() { return type; }

    String ToString() override {
        char_t buf[0x400];

        SPRINT(buf, ARRSIZE(buf), SPREF"{ type: %ls, byteSize: %d })", getFullName().c_str(), getByteSize());

        return buf;
    }

    virtual String getName() {
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
            size_t _len = strlen(types[i]);
            if (strict && _len != str.length()) continue;
            if (len < _len && str.substr(0, _len) == String(types[i])) {
                len = _len;
                ty = (Enum)i;
            }
        }
        t = new Type(ty);
        return ty != Unknown;
    }

    static Type createType(String name, STD vector<String> loc = {}, STD vector<PMember> members = {});
    static Type createType(Type super_ty, String name, STD vector<String> loc = {}, STD vector<PMember> members = {});
    static Type createPrototype(String name, STD vector<String> loc = {});

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
    bool isStatic, isRef, isVirtual, isAbstract;
protected:
    Enum mem_ty;
    Type ty, class_ty;
    String name;
public:
    Member(String name, Type ty, Type class_ty, Enum mem_ty) : name(name), ty(ty), class_ty(class_ty), mem_ty(mem_ty) {}

    Type getType() {
        return ty;
    }

    Enum getMemberType() {
        return mem_ty;
    }

    String getName() {
        return name;
    }

    String getFullName() {
        return class_ty.getFullName() + L"." + name;
    }

    virtual String getLlvmName() = 0;
};

struct Field : public Member {
public:
    Field(String name, Type class_type, Type type)
        : Member(name, type, class_type, Member::Field) {}

    String getLlvmName() override {
        return class_ty.getLlvmName() + L"_fld_" + name;
    }
};

struct Function : public Member {
private:
    String name;

    STD vector<Type> parameters;
public:
    Function(String name, Type class_type, Type ret_type = Type::Void, STD vector<Type> args = {})
        : Member(name, ret_type, class_type, Member::Function) {
        parameters = args;
    }

    String getLlvmName() override {
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

    String getLlvmName() override {
        return class_ty.getLlvmName() + L"_prp_" + name;
    }
};

typedef ::Type ty;
ty::PMember ty::getMember(String str) {
    for (::Type::PMember& mem : members) {
        if (mem->getName() == str) {
            return mem;
        }
    }
    return nullptr;
}

bool operator==(::Type a, ::Type b) {
    return a.getFullName() == b.getFullName();
}

#include "Objects.hpp"

Type Type::createType(String name, STD vector<String> loc, STD vector<PMember> members = {}) {
    return createType(Objects::Object(), name, loc, members);
}

Type Type::createType(Type super_ty, String name, STD vector<String> loc, STD vector<PMember> members = {}) {
    using Objects::Object;
    Object obj = Object(loc, name, members);
    obj.super_type = new Type(super_ty);
    return obj;
}