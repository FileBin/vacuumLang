#pragma once
#include "stdafx.hpp"
#include "Functions.hpp"
#include "Types.hpp"

namespace Objects {

    class ClassBase : public Type {
    protected:
        String name;
        ClassBase(STD vector<String> location, String name, STD vector<PMember> pmembers);
    public:
        String getName() override {
            return name;
        }

        int getClassSize() override;
    };

    class Object : public ClassBase {
    public:
        Object(String name = "Object", STD vector<PMember> pmembers = {});
        Object(STD vector<String> location, String name = "Object", STD vector<PMember> pmembers = {});
    };

    class ValueType : public Object {
    public:
        ValueType() : Object("ValueType") {}
    };

    class StringO : public Object {
    public:
        StringO() : Object() {}
    };

    class TypeO : public Object {
    public:
        TypeO() : Object() {}
    };

    ClassBase::ClassBase(STD vector<String> loc, String str, STD vector<PMember> pmembers) {
        location = loc;
        members = pmembers;
        type = Type::Class;
        name = str;
    }

    Object::Object(String name, STD vector<PMember> pmembers)
        : ClassBase({}, name, {
            new Function("toString", *this, StringO()),
            new Function("hash", *this, Type::UNum),
            new Function("equals", *this, Type::Bool),
            new Function("clone", *this, Type::Object),
            new Function("getType", *this, TypeO())
            }) {
        members.insert(members.end(), pmembers.begin(), pmembers.end());
    }

    Object::Object(STD vector<String> location, String name, STD vector<PMember> pmembers)
        : ClassBase(location, name, {
            new Function("toString", *this, StringO()),
            new Function("hash", *this, Type::UNum),
            new Function("equals", *this, Type::Bool),
            new Function("clone", *this, Type::Object),
            new Function("getType", *this, TypeO())
            }) {
        members.insert(members.end(), pmembers.begin(), pmembers.end());
    }
}

#include "MetadataGenerator.hpp"

int Objects::ClassBase::getClassSize() {
    int size = 0;
    for(PMember &mem : members){
        if(mem->getMemberType() == Member::Field) {
            ::Type ty = mem->getType();
            if(mem->isRef) {
                size += 8; //ref_size is 64bit
            } else {
                if(ty == static_cast<::Type>(*this)) LogError("Cycle definition is not allowed (pls use ref)!");
                size_t mem_s = ty.getByteSize();
                if(mem_s == 0) LogError("Intancing of abstract class is not allowed");
                size += mem_s;
            }
        }
    }
    return size;
}
