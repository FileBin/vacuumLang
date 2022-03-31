#pragma once
#include <string>
#include "stdafx.hpp"
#include <codecvt>
#include <locale>
#ifdef WIN32
#include <Windows.h>
#endif

struct String;

String operator+(CCSTR, String);
String operator+(String, CCSTR);

typedef STD basic_string<char_t> std_string;

interface IPrintable {
    virtual String toString() = 0;
};

struct String : public std_string, public IPrintable {
    String() : std_string() {};
#ifdef USE_WSTR
    String(const wchar_t* wstr) : std_string(wstr) {}
#else
    String(const wchar_t* wstr);
#endif

    String(const char* cstr);

    String(const std_string& wstr) : std_string(wstr) {}
    String toString() override {
        return *this;
    }

    String operator=(char_t ch) {
        *this = String();
        at(0) = ch;
        return *this;
    }
};

template<typename T>
class Tree : IPrintable {
public:
    struct Node;
    typedef Node* PNode;
    struct Node : IPrintable {
    public:
        typedef STD vector<PNode> vec_t;
        typedef typename STD vector<PNode>::iterator it_t;
    private:
        friend class Tree;
        PNode parent = nullptr;
        vec_t children;
        it_t it;
    public:

        T* data;
        PNode addChild(T* d) {
            auto ptr = new Node();
            ptr->parent = this;
            ptr->data = d;
            ptr->it = children.insert(children.end(), ptr);
            return ptr;
        }

        bool removeChild(T* d) {
            for (auto it = children.begin(); it != children.end(); it++) {
                if ((*it)->data == d) {
                    children.erase(it);
                    return true;
                }
            }
            return false;
        }

        PNode getNext() {
            if (parent == nullptr)
                return children.front();

            int level = 0;
            PNode node = this;
            while (node->it + 1 == node->parent->children.end()) {
                level++;
                node = node->parent;
                if(node->parent == nullptr) break;
            }
            if (level == 0)
                return *(node->it + 1);

            if (node->parent == nullptr) {
                node = node->children.front();
            } else {
                node++;
            }
            for (int i = 0; i < level; i++) {
                if (node->children.empty()) return nullptr;
                node = node->children.front();
            }
            return node;
        }

        String toString() {
            return toString(0);
        }
        //min level 0, max level 128
        String toString(int level) {
            char buf[0x100];
            memset(&buf, ' ', sizeof(buf));
            buf[level * 2] = 0;
            String str = buf + data->toString() + "\n";
            for (PNode child : children) {
                str += child->toString(level + 1);
            }
            return str;
        }
    };

    PNode root;

    Tree(T* rootData = {}) {
        root = new Node();
        root->data = rootData;
    }

    template<typename R>
    PNode find(R data) noexcept {
        STD vector<PNode> l = { root }, buf;
        /* do {
             buf.clear();
             for (PNode node : l) {
                 if (*node->data == data) {
                     return node;
                 }
                 buf.insert(buf.begin(), node->children.begin(), node->children.end());
             }
             l = buf;
         } while (!buf.empty());*/

        for (PNode p = root; p != nullptr; p = p->getNext()) {
            if (*p->data == data)
                return p;
        }

        return nullptr;
    }

    String toString() {
        return String("{\n") + root->toString() + "}\n";
    }
};

String Utf8ToWstring(const std::string& str) {
#ifdef __linux__
    std::wstring_convert<std::codecvt_utf8<char_t>> utf8_conv;
    return utf8_conv.from_bytes(str);
#endif
#ifdef WIN32
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
#endif
}

std::string ToStdString(const String& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
    return utf8_conv.to_bytes(str);
}

::String::String(const char* cstr) {
    size_t size = strlen(cstr);
    resize(size);
    for (size_t pos = 0; pos < size; pos++) {
        at(pos) = cstr[pos];
    }
}

String operator+(CCSTR cstr, String s) {
    return String(cstr) + s;
}

String operator+(String s, CCSTR cstr) {
    return s + String(cstr);
}

String operator+(String s, STD string str) {
    return s + str.c_str();
}

String operator+(STD string s, String str) {
    return s.c_str() + str;
}