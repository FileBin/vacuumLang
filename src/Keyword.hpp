#pragma once
#include "stdafx.hpp"

namespace Constant {
    CEXPRCSTR keywords[] = {
        "native", //import c function
        "import", //import namespace
        "entrypoint", //begin of the code

        //OOP
        "namespace",
        "class",
        "self",
        "super",
        "public",
        "private",
        "protected",
        "friend",
        "interface",
        "constuctor",
        "destuctor",
        "var",

        "as",
        "is",

        "null",

        //modifiers
        "const",
        "static",

        "ref",

        "sizeof",
        "addrof",
        "typeof",

        "unroll",

        "native_pointer",
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

        Constructor,
        Destructor,

        As,
        Is,

        Null,

        Const,
        Static,

        Ref,

        Sizeof,
        Addrof,
        Typeof,
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
            size_t klen = strlen(keywords[i]);
            if(strict && str.length() != klen) continue;
            if(len < klen && str.substr(0, klen) == String(keywords[i])){
                len = klen;
                ty = (Enum)i;
            }
        }
        kw = new Keyword(ty);
        return ty != Unknown;
    }
};