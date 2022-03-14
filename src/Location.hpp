#pragma once
#include "stdafx.hpp"

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
};
