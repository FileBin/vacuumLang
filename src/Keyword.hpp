#pragma once
#include "stdafx.hpp"

namespace Constant {
    constexpr char_t const* keywords[] = {
        STR(native), //import c function
        STR(import), //import namespace
        STR(entrypoint), //begin of the code

        //OOP
        STR(namespace),
        STR(class),
        STR(self),
        STR(super),
        STR(public),
        STR(private),
        STR(protected),
        STR(friend),
        STR(interface),
        STR(var),

        STR(as),
        STR(is),

        STR(null),

        //modifiers
        STR(const),
        STR(static),

        STR(ref),

        STR(sizeof),
        STR(addrof),

        STR(unroll),

        STR(native_pointer),
    };
}

class Keyword : public IPrintable {
public:
    enum Enum {
        Unknown = -1,
        Native = 0,
        Import,
        Entrypoint,

        Namespace,
        Class,
        Self,
        Super,
        Public,
        Private,
        Protected,
        Friend,
        Interface,
        Var,

        As,
        Is,

        Null,

        Const,
        Static,

        Ref,

        Sizeof,
        Addrof,
        Unroll,

        NativePointer,
    } ty;

    bool isModifier(){
        switch(ty) {
            case Static:
            case Const:
            case Ref:
            case Public:
            case Private:
            case Protected:
            case Native:
            return true;

            default:
            return false;
        }
    }

    bool isDefinition() {
        switch (ty) {
            case Class:
            case Interface:
            case Var:
            return true;
            
            default:
            return false;
        }
    }

    bool isVisiblityModifier(){
        switch(ty) {
            case Public:
            case Private:
            case Protected:
            return true;

            default:
            return false;
        }
    }

    Keyword(Enum en = Unknown) {
        ty = en;
    }

    String ToString() {
        return Constant::keywords[(size_t)ty];
    }

    static bool TryParse(String str, Keyword* &kw, bool strict = false) {
        using namespace Constant;
        Enum ty = Unknown;
        size_t len = 0;
        for(size_t i=0; i<ARRSIZE(keywords); i++) {
            size_t klen = LEN(keywords[i]);
            if(strict && str.length() != klen) continue;
            if(len < klen && str.substr(0, klen) == keywords[i]){
                len = klen;
                ty = (Enum)i;
            }
        }
        kw = new Keyword(ty);
        return ty != Unknown;
    }
};