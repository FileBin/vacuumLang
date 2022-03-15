#pragma once

#include "stdafx.hpp"
#include "Location.hpp"
#include "ASTheader.h"

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

enum class StdType {
    Unknown = -1,
    Unsigned,
    Signed,
    Float,
};

class Type : public IPrintable {
public:
    typedef Member* PMember;
    enum Enum {
        Pointer = -3,
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

    Location location;
    STD vector<PMember> members;

    bool isProto = false;

    virtual int getClassSize() = 0;
    virtual llvm::Type* getClassLlvmType();
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

    bool hasDefinition() {
        return !isProto;
    }

    Metadata* getMeta() {
        return pmeta;
    }

    StdType getStdType() noexcept {
        switch (type)
        {
        case Byte:
        case UShort:
        case UInt:
        case UNum:
        case Char:
            return StdType::Unsigned;
        case SByte:
        case Short:
        case Int:
        case Num:
            return StdType::Signed;
        case Flt:
        case Dbl:
            return StdType::Float;
        default:
            return StdType::Unknown;
        }
    }

    int getByteSize() {
        if (type == Type::Class) return 0;
        if (isProto) logError("Type " + getName() + " is not defined!");
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

        case Pointer:
            return 8; //64-bit pointer

        case Class:
            return getClassSize();
        default:
            logError("Can't calculate class size!");
        }
        return -1;
    }

    Enum getEnum() { return type; }

    String toString() override {
        char_t buf[0x400] = { 0 };

        SPRINT(buf, ARRSIZE(buf), SPREF "{ type: " STRPARAM ", byteSize: %d }", getFullName().c_str(), getByteSize());

        return buf;
    }

    virtual String getName() = 0;

    String getFullName() {
        return location.getName() + getName();
    }

    String getLlvmName() {
        return location.getLlvmName() + "typ_" + getName();
    }

    void createDefinition(Type* super_ty, STD vector<PMember> _members = {}) {
        //if(super_ty->isProto) logError("Type " + super_ty->toString() + " is not defined!");
        super_type = super_ty;
        members = _members;
        isProto = false;
    }

    llvm::Type* getLlvmType(llvm::LLVMContext& context) {
        switch (type)
        {
        case Object:
        case Void:
            return llvm::Type::getVoidTy(context);
        case Bool:
            return llvm::Type::getInt1Ty(context);
        case SByte:
        case Byte:
            return llvm::Type::getInt8Ty(context);
        case Short:
        case UShort:
            return llvm::Type::getInt16Ty(context);
        case Int:
        case UInt:
            return llvm::Type::getInt32Ty(context);
        case Num:
        case UNum:
            return llvm::Type::getInt64Ty(context);

        case Flt:
            return llvm::Type::getFloatTy(context);
        case Dbl:
            return llvm::Type::getDoubleTy(context);

        case Class:
            return getClassLlvmType();

        default:
            logError("Unknown Type " + getFullName() + "!");
            return 0;
        }
    }

    static bool isStd(Type* ty) {
        return ty->type >= 0;
    }

    static String getName(Enum en) {
        if (en > 0) {
            return Constant::types[(size_t)en];
        }
        logError("Fuck yoy leatherman");
        return "";
    }

    static Type* getInstance(Metadata* pmeta, String name, Location location = {});
    static Type* getInstance(Metadata* pmeta, Enum en);
    static Type* createPrototype(Metadata* pmeta, String name, Location loc = {});

    PMember getMember(String name);
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
    bool isStatic, isVirtual, isAbstract;
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

struct Variable {
protected:
    String name;
    Type* t;
public:
    Variable(Type* type, String name) : name(name), t(type) {}

    String getName() {
        return name;
    }

    Type* getType() {
        return t;
    }
};

struct Field : public Member {
public:
    Field(String name, PType class_type, PType type)
        : Member(name, type, class_type, Member::Field) {}

    String getLlvmName() override {
        return L"fld_" + name;
    }
};

struct Function : public Member {
private:
    String name;
    TokenBufferStream body;
    STD vector<Variable*> parameters;
public:
    Function(String name, PType class_type, PType ret_type, STD vector<Variable*> args = {})
        : Member(name, ret_type, class_type, Member::Function) {
        parameters = args;
    }

    void setBody(TokenBufferStream& stream) {
        body = stream;
    }

    TokenBufferStream& getBody() {
        return body;
    }

    String getLlvmName() override {
        return class_ty->getLlvmName() + L"_fnc_" + name;
    }
};

struct Property : public Member {
private:
    ::Function f_get, f_set;
    ::Field* variable = nullptr;
public:
    Property(String name, PType class_type, PType type, bool need_variable = false)
        : Member(name, type, class_type, Member::Property),
        f_get(name + "_get", class_type, type),
        f_set(name + "_set", class_type, Type::getInstance(class_ty->getMeta(), Type::Void),
            { new Variable(type, "v") }) {
        if (need_variable) {
            variable = new ::Field(name + "_var", class_type, type);
        }
        ty = type;
        mem_ty = Member::Property;
    }

    String getLlvmName() override {
        return L"prp_" + name;
    }
};

typedef ::Type ty;
ty::PMember ty::getMember(String name) {
    for (::Type::PMember& mem : members) {
        if (mem->getName() == name) {
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

#include "ClassFactory.hpp"

Type* Type::createPrototype(Metadata* m, String name, Location loc) {
    return Objects::ClassFactory::createProto(m, loc, name);
}

Type* Type::getInstance(Metadata* pmeta, Type::Enum ty) {
    using namespace Objects;
    if (ty == Type::Void) return createPrototype(pmeta, "void");
    return getInstance(pmeta, getName(ty));
}

Type* Type::getInstance(Metadata* pmeta, String name, Location location) {
    auto ptr = pmeta->classTree.find<String>(location.getName() + name)->data;
    if (ptr == nullptr)
        ptr = createPrototype(pmeta, name, location);
    return ptr;
}

Location& Location::operator+=(Type* ty) {
    data.push_back({ ty->getName(), Location::Node::Class });
    return *this;
}