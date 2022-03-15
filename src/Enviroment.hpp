#pragma once
#include "stdafx.hpp"
#include "ASTheader.h"
class Type;
class CompilerModule;

struct Enviroment {
public:
    typedef STD map<String, llvm::Value*> varMap;
    typedef STD out_of_range outofr;
private:
    varMap paramMap;
    STD vector<varMap> localVarMap = { {} };
    Type* self_context;
    llvm::Value* self;
    varMap functionMap;
    varMap staticMap;
public:
    void beginScope() {
        localVarMap.push_back({});
    }

    void endScope() {
        localVarMap.pop_back();
    }

    void setSelfContext(Type* ty, llvm::Value* self_param = 0) {
        self_context = ty;
        if (ty->isStatic()) return;
        self = self_param;
    }

    llvm::Value*& addParam(String name, llvm::Value* val) noexcept {
        return paramMap[name] = val;
    }

    llvm::Value*& addLocal(String name, llvm::Value* val) noexcept {
        return localVarMap.back()[name] = val;
    }

    llvm::Value* findVar(String name, CompilerModule* module);
};

#include "Types.hpp"
#include "CompilerModule.hpp"

llvm::Value* Enviroment::findVar(String name, CompilerModule* module) {
    for (long i = localVarMap.size() - 1; i >= 0; i--) {
        try {
            return localVarMap[i].at(name);
        } catch (outofr) {
            continue;
        }
    }

    try {
        return paramMap.at(name);
    } catch (outofr) {}

    try {
        if (self_context) {
            if (self_context->isStatic()) {
                return staticMap.at(self_context->getFullName() + "." + name);
            }
            if (self) {
                using namespace llvm;
                int idx = self_context->getFieldIndex(name);
                if (idx < 0) return nullptr;
                auto& c = module->getContext();
                auto& b = module->getBuilder();
                auto type = self_context->getLlvmType(c);
                return b.CreateLoad(type, b.CreateGEP(type, self, { ConstantInt::get(c, APInt(1, 0)), ConstantInt::get(c, APInt(32, idx)) }, "getelemptr"), "load");
            }
        }
    } catch (outofr) {}
    return nullptr;
}