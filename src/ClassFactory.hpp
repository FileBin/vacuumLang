#pragma once
#include "stdafx.hpp"
#include "Functions.hpp"
#include "Types.hpp"
#include "Metadata.hpp"

namespace Objects {

    class ClassFactory : public Type {
    protected:
        String name;
        ClassFactory(Metadata* pmeta, Type* super_type, Location location, String name, STD vector<PMember> pmembers);
    public:
        static Type* createProto(Metadata* pmeta, Location location, String name) {
            auto proto = new ClassFactory(pmeta, nullptr, location, name, {});
            proto->is_proto = true;
            return proto;
        }

        virtual String getName() override { return name; }
        virtual llvm::Type* getClassLlvmType() override { return 0; }

        int getClassSize() override;
    };

    ClassFactory::ClassFactory(Metadata* pmeta, Type* super_ty, Location loc, String str, STD vector<PMember> pmembers)
        : Type(pmeta, Type::Class) {
        location = loc;
        members = pmembers;
        name = str;
        super_type = super_ty;
    }
}

#include "Metadata.hpp"

int Objects::ClassFactory::getClassSize() {
    int size = 0;
    if (super_type)
        size = super_type->getByteSize();
    for (PMember& mem : members) {
        if (mem->getMemberType() == Member::Field) {
            ::Type* ty = mem->getType();
            if (ty == static_cast<::Type*>(this)) logError("Cycle definition is not allowed (pls use ref)!");
            size_t mem_s = ty->getByteSize();
            if (mem_s == 0) logError("Intancing of abstract class is not allowed");
            size += mem_s;
        }
    }
    return size;
}
