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
    llvm::Type* llvm_type = nullptr;

    Location location;
    STD vector<PMember> members;

    bool is_proto = false;
    bool is_static = false;
    bool is_sealed = false;
    bool is_abstract = false;

    virtual int getClassSize() = 0;
    virtual llvm::Type* getClassLlvmType(llvm::LLVMContext& context) = 0;
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
        is_proto = other.is_proto;
    }

    bool hasDefinition() {
        return !is_proto;
    }

    bool isStatic() { return is_static; }
    bool isAbstact() { return is_abstract; }
    bool isSealed() { return is_sealed; }

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
        //FIXME remove this line when vacuum code will work
        if (type == Type::Class) return 0;
        if (is_proto) logError("Type " + getName() + " is not defined!");
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
            logError("Can't calculate class size!");
        }
        return -1;
    }

    Enum getEnum() { return type; }

    virtual String getName() = 0;

    String getFullName() {
        return location.getName() + getName();
    }

    STD string getLlvmName() {
        return location.getLlvmName() + "typ_" + ToStdString(getName());
    }

    void createDefinition(Type* super_ty, STD vector<PMember> _members = {}) {
        //FIXME uncomment line below when vacuum code will work
        //if(super_ty->is_proto) logError("Type " + super_ty->toString() + " is not defined!");
        super_type = super_ty;
        members = _members;
        is_proto = false;
    }

    llvm::Type* getLlvmType(llvm::LLVMContext& context) {
        using T = llvm::Type;
        if(llvm_type) return llvm_type;
        switch (type)
        {
        case Object:
        case Void:
            return llvm_type = T::getVoidTy(context);
        case Bool:
            return llvm_type = T::getInt1Ty(context);
        case SByte:
        case Byte:
            return llvm_type = T::getInt8Ty(context);
        case Short:
        case UShort:
            return llvm_type = T::getInt16Ty(context);
        case Int:
        case UInt:
            return llvm_type = T::getInt32Ty(context);
        case Num:
        case UNum:
            return llvm_type = T::getInt64Ty(context);

        case Flt:
            return llvm_type = T::getFloatTy(context);
        case Dbl:
            return llvm_type = T::getDoubleTy(context);

        case Class:
            return llvm_type = getClassLlvmType(context);

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

    void addModifier(Keyword kw) {
        if (kw.isModifier()) logError("Unknown keyword");
        if (!is_abstract) {
            if (kw.ty == Keyword::Static) {
                is_static = true;
                return;
            }
        }
    }

    String toString() override {
        char_t buf[0x400] = { 0 };

        SPRINT(buf, ARRSIZE(buf), SPREF "{ type: " STRPARAM ", byteSize: %d }", getFullName().c_str(), getByteSize());

        return buf;
    }

    static Type* getInstance(Metadata* pmeta, String name, Location location = {});
    static Type* getInstance(Metadata* pmeta, Enum en);
    static Type* createPrototype(Metadata* pmeta, String name, Location loc = {});

    PMember getMember(String name);
    int getMemberIndex(String name);
    int getFieldIndex(String name);
    STD vector<PMember> getMembers() { return members; }
};

struct Member {
public:
    typedef Type* PType;
    enum Enum {
        Unknown = -1,
        Field = 0,
        Property,
        Function,
    } mem_ty;
    bool isStatic, isVirtual, isAbstract, isRef, isNativeP;
protected:
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

    virtual STD string getLlvmName() = 0;
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

    STD string getLlvmName() override {
        return ToStdString(name);
    }
};

#include "TokenBufferStream.hpp"

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

    STD string getLlvmName() override {
        return class_ty->getLlvmName() + "_fnc_" + ToStdString(name);
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

    STD string getLlvmName() override {
        return "prp_" + ToStdString(name);
    }
};

typedef ::Type ty;
ty::PMember ty::getMember(String name) {
    int idx;
    if((idx = getMemberIndex(name)) >= 0)
        return members[idx];
    return nullptr;
}

int ty::getMemberIndex(String name) {
    size_t n = members.size();
    for (int i = 0;i < n;i++) {
        if (members[i]->getName() == name) {
            return i;
        }
    }
    return -1;
}

int ty::getFieldIndex(String name) {
    size_t n = members.size();
    int idx = 0;
    for (auto mem : members) {
        if(mem->mem_ty != Member::Field)
            continue;
        if (mem->getName() == name) {
            return idx;
        }
        idx++;
    }
    return -1;
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