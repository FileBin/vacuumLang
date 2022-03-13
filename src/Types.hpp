#pragma once

#include "stdafx.hpp"
struct Member;
struct Metadata;

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

        //strings
        Char, //UTF-32 string character

        //Objects
        Object,
    };
protected:
    friend struct Metadata;
    Metadata* pmeta;
    Type* super_type = nullptr;
    STD vector<Type*> interfaces;
    Enum type = Unknown;

    STD vector<String> location;
    STD vector<PMember> members;

    bool isProto = false;

    virtual int getClassSize() = 0;
public:
    Type(Metadata* pmeta, Enum type_enum = Unknown) : pmeta(pmeta) {
        interfaces = {};
        location = {};
        members = {};

        type = type_enum;
    }

    Type(const Type& other) {
        super_type = other.super_type;
        interfaces = other.interfaces;
        type = other.type;
        location = other.location;
        members = other.members;
        isProto = other.isProto;
    }

    int getByteSize() {
        if (isProto) LogError(String("Type ") + getName() + String(" is not defined!"));
        switch (type) {
        case Object:
        case Void:
            return 0; //abstract objects

        case Bool:
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
        char_t buf[0x400] = { 0 };

        SPRINT(buf, ARRSIZE(buf), SPREF"{ type: %ls, byteSize: %d }", getFullName().c_str(), getByteSize());

        return buf;
    }

    virtual String getName() = 0;

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

    static String getEnumTypeName(Enum en) {
        switch (en)
        {
        case Bool: return "Bool";
        case SByte: return "SByte";
        case Byte: return "Byte";
        case Short: return "Short";
        case UShort: return "UShort";
        case Int: return "Int";
        case UInt: return "UInt";
        case Num: return "Num";
        case UNum: return "UNum";
        case Flt: return "Flt";
        case Dbl: return "Dbl";
        case Char: return "Char";
        case Object: return "Object";

        default:
            LogError("Unknown TypeName!");
        }
        return "";
    }

    static bool tryParse(String str, Type*& t, bool strict = false);
    Type* getInstance(String str);
    Type* getInstance(Enum en);

    void createDefinition(Type* super_ty, STD vector<PMember> _members = {}) {
        super_type = super_ty;
        members = _members;
        isProto = false;
    }
    static Type* getInstance(Metadata* pmeta, String str);
    static Type* getInstance(Metadata* pmeta, Enum en);
    static Type* createInstance(Metadata* pmeta, Enum en);
    static Type* createType(Metadata* pmeta, String name, STD vector<String> loc = {}, STD vector<PMember> members = {});
    static Type* createType(Metadata* pmeta, Type* super_ty, String name, STD vector<String> loc = {}, STD vector<PMember> members = {});
    static Type* createPrototype(Metadata* pmeta, String name, STD vector<String> loc = {});

    PMember getMember(String str);
};

struct Member {
public:
    typedef Type* PType;
    enum Enum {
        Unknown = -1,
        Field = 0,
        Property,
        Function,
    };
    bool isStatic, isRef, isVirtual, isAbstract;
protected:
    Enum mem_ty;
    PType ty, class_ty;
    String name;
public:
    Member(String name, PType ty, PType class_ty, Enum mem_ty) : name(name), ty(ty), class_ty(class_ty), mem_ty(mem_ty) {}

    PType getType() {
        return ty;
    }

    Enum getMemberType() {
        return mem_ty;
    }

    String getName() {
        return name;
    }

    String getFullName() {
        return class_ty->getFullName() + L"." + name;
    }

    virtual String getLlvmName() = 0;
};

struct Field : public Member {
public:
    Field(String name, PType class_type, PType type)
        : Member(name, type, class_type, Member::Field) {}

    String getLlvmName() override {
        return class_ty->getLlvmName() + L"_fld_" + name;
    }
};

struct Function : public Member {
private:
    String name;

    STD vector<Type*> parameters;
public:
    Function(String name, PType class_type, PType ret_type, STD vector<PType> args = {})
        : Member(name, ret_type, class_type, Member::Function) {
        parameters = args;
    }

    String getLlvmName() override {
        return class_ty->getLlvmName() + L"_fnc_" + name;
    }
};

struct Property : public Member {
private:
    ::Function f_get, f_set;
public:
    Property(String name, PType class_type, PType type)
        : Member(name, type, class_type, Member::Property), f_get(name + L"_get", class_type, type), f_set(name + L"_set", class_type, class_type->getInstance(Type::Void), { type }) {
        ty = type;
        mem_ty = Member::Property;
    }

    String getLlvmName() override {
        return class_ty->getLlvmName() + L"_prp_" + name;
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

bool operator==(::Type& a, ::Type& b) {
    return a.getFullName() == b.getFullName();
}

bool operator==(::Type& a, String b) {
    return a.getFullName() == b;
}

#include "Objects.hpp"

bool Type::tryParse(String str, Type*& t, bool strict) {
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
    t = createInstance(nullptr, ty);
    return ty != Unknown;
}

Type* Type::createType(Metadata* pmeta, String name, STD vector<String> loc, STD vector<PMember> members) {
    return createType(pmeta, new Objects::Object(pmeta), name, loc, members);
}

Type* Type::createType(Metadata* pmeta, Type* super_ty, String name, STD vector<String> loc, STD vector<PMember> members) {
    using Objects::Object;
    Object* obj = new Object(pmeta, loc, name, members);
    obj->super_type = super_ty;
    return obj;
}

Type* Type::createInstance(Metadata* pmeta, Type::Enum ty) {
    using namespace Objects;
    if (ty == Type::Object || ty == Type::Void) {
        return new Objects::Object(pmeta);
    }
    return new ValueType(pmeta, getEnumTypeName(ty));
}

Type* Type::createPrototype(Metadata* m, String name, STD vector<String> loc) {
    return Objects::ClassBase::createProto(m, loc, name);
}

Type* Type::getInstance(Type::Enum ty) {
    return getInstance(pmeta, ty);
}

Type* Type::getInstance(String str) {
    return getInstance(pmeta, str);
}

Type* Type::getInstance(Metadata* pmeta, Type::Enum ty) {
    using namespace Objects;
    if (ty == Type::Object || ty == Type::Void) {
        return getInstance(pmeta, "Object");
    }
    return getInstance(pmeta, getEnumTypeName(ty));
}

Type* Type::getInstance(Metadata* pmeta, String str) {
    return pmeta->ClassTree.find<String>(str)->data;
}