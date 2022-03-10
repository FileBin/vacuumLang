#pragma once
#include "stdafx.hpp"
class Object {
    private:
    int byteWidth;
    public:
    Object();

    int GetByteWidth() { return byteWidth; }
};

inline Object::Object(){
    byteWidth = sizeof(Object);
}