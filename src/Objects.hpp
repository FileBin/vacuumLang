#pragma once
#include "stdafx.hpp"
#include "Functions.hpp"
#include "Types.hpp"

namespace Objects {

    class ClassBase : public Type {
    protected:
        String name;
        ClassBase(Metadata* pmeta, Type* super_type, STD vector<String> location, String name, STD vector<PMember> pmembers);
    public:
        static Type* createProto(Metadata* pmeta, STD vector<String> location, String name){
            auto proto = new ClassBase(pmeta, nullptr, location, name, {});
            proto->isProto = true;
            return proto;
        }
        virtual String getName() override {
            return name;
        }

        int getClassSize() override;
    };

    class Object : public ClassBase {
    public:
        Object(Metadata* pmeta, String name = "Object");
        Object(Metadata* pmeta, STD vector<String> location, String name = "Object", STD vector<PMember> pmembers = {});

        virtual String getName() override {
            return name;
        }

        static String getClassName() {
            return "Object";
        }

        static STD vector<PMember> getMembers(Metadata* pmeta) {
            Type* super = getInstance(pmeta, getClassName());
            return {
            new Function("hash", super, getInstance(pmeta, Type::UNum)),
            new Function("equals", super, getInstance(pmeta, Type::Bool)),
            new Function("clone", super, getInstance(pmeta, Type::Object)),
            // new Function("toString", this, new StringO()),
            // new Function("getType", this, new TypeO())
            };
        }
    };

    class ValueType : public Object {
    public:
        ValueType(Metadata* pmeta) : Object(pmeta, "ValueType") {
           // super_type = new ValueType();
        }
        ValueType(Metadata* pmeta, String name) : Object(pmeta, name) {
           // super_type = new ValueType();
        }
    };

    class StringO : public Object {
    public:
        StringO(Metadata* pmeta) : Object(pmeta, "String") {}
    };

    class TypeO : public Object {
    public:
        TypeO(Metadata* pmeta) : Object(pmeta, "Type") {}
    };

    ClassBase::ClassBase(Metadata* pmeta, Type* super_ty, STD vector<String> loc, String str, STD vector<PMember> pmembers) : Type(pmeta, Type::Class) {
        location = loc;
        members = pmembers;
        name = str;
        super_type = super_ty;
    }

    Object::Object(Metadata* pmeta, String name)
        : ClassBase(pmeta, getInstance(pmeta, Type::Object), {}, name, Object::getMembers(pmeta)) {}

    Object::Object(Metadata* pmeta, STD vector<String> location, String name, STD vector<PMember> pmembers)
        : ClassBase(pmeta, getInstance(pmeta, Type::Object), location, name, Object::getMembers(pmeta)) {}
}

#include "MetadataGenerator.hpp"

int Objects::ClassBase::getClassSize() {
    int size = 0;
    if(super_type)
        size = super_type->getByteSize();
    for(PMember &mem : members) {
        if(mem->getMemberType() == Member::Field) {
            ::Type* ty = mem->getType();
            if(mem->isRef) {
                size += 8; //ref_size is 64bit
            } else {
                if(ty == static_cast<::Type*>(this)) LogError("Cycle definition is not allowed (pls use ref)!");
                size_t mem_s = ty->getByteSize();
                if(mem_s == 0) LogError("Intancing of abstract class is not allowed");
                size += mem_s;
            }
        }
    }
    return size;
}
