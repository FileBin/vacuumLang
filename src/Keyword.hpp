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

        As,
        Is,

        Null,

        Const,
        Static,

        Ref,

        Sizeof,
        Addrof,
        Unroll,
    } ty;

    Keyword(Enum en = Unknown) {
        ty = en;
    }

    String ToString() {
        return Constant::keywords[(size_t)ty];
    }

    static bool TryParse(String str, Keyword* &kw) {
        using namespace Constant;
        Enum ty = Unknown;
        size_t len = 0;
        for(size_t i=0; i<ARRSIZE(keywords); i++){
            if(len < LEN(keywords[i]) && str.substr(0, LEN(keywords[i])) == keywords[i]){
                len = LEN(keywords[i]);
                ty = (Enum)i;
            }
        }
        kw = new Keyword(ty);
        return ty != Unknown;
    }
};