#pragma once
#include "stdafx.hpp"
#include "Types.hpp"

namespace Objects {

    class ClassBase : public Type {
    protected:
        ClassBase(String name, STD vector<Member> members);
    };

    class Object : public ClassBase {
    private:
        int byteWidth;
    public:
        Object();

        int GetByteWidth() { return byteWidth; }
    };

    class StringO : public Object {
    public:
        StringO() : Object() {}
    };

    inline Object::Object()
        : ClassBase(L"Object", {
            Function(L"toString", *this, StringO()),
            Function(L"hash", *this, Type::Num),
            Function(L"equals", *this, Type::Bool),
            Function(L"clone", *this, Type::Object),
            //TODO: impl Reflection Function(L"getType", *this, ) 
            }) {
        byteWidth = sizeof(Object);
    }
}