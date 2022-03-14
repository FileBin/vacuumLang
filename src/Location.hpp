#pragma once
#include "stdafx.hpp"

struct Type;

struct Location : IPrintable {
public:
    struct Node {
        String name;
        enum Enum { Class, Namespace } type;

        String getLlvmName() {
            switch (type)
            {
            case Class:
                return "cls_" + name;
            case Namespace:
                return "nsp_" + name;
            }
        }
    };
private:
    friend Location operator+(Location, Type*);
    friend Location operator+(Location, String);

    STD vector<Node> data;
public:
    String toString() override {
        if (data.empty()) return "";
        auto it = data.begin();
        String full_name = it->name;
        for (;it != data.end();it++) {
            full_name += "." + it->name;
        }
        return full_name;
    }

    String getName() {
        String full_name = "";
        for (auto it = data.begin();it != data.end();it++) {
            full_name += it->name + ".";
        }
        return full_name;
    }

    String getLlvmName() {
        if (data.empty()) return "";
        auto it = data.begin();
        String llvm_name = it->getLlvmName();
        for (;it != data.end();it++) {
            llvm_name += "_" + it->getLlvmName();
        }
        return llvm_name;
    }

    Location& operator+=(String nsp) {
        data.push_back({ nsp, Location::Node::Namespace });
        return *this;
    }

    Location& operator+=(Type* ty) {
        data.push_back({ ty->getName(), Location::Node::Class });
        return *this;
    }
};

Location operator+(Location a, Type* b);
Location operator+(Location a, String nsp);

#include "Types.hpp"

Location operator+(Location a, Type* b) {
    Location loc = a;
    loc += b;
    return loc;
}

Location operator+(Location a, String nsp) {
    Location loc = a;
    loc += nsp;
    return loc;
}
