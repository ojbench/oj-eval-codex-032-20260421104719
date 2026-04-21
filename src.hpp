// Implementation of a minimal Python-like list supporting nested lists and ints
#ifndef PYLIST_H
#define PYLIST_H

#include <iostream>
#include <vector>
#include <memory>
#include <cstddef>

class pylist {
private:
    struct ListData {
        std::vector<pylist> elems;
    };

    enum class Type { INT, LIST } type_;
    int iv_ = 0;
    std::shared_ptr<ListData> list_;

    // Ensure this object is a list with shared storage
    void ensure_list() {
        if (type_ != Type::LIST) {
            type_ = Type::LIST;
            list_ = std::make_shared<ListData>();
        } else if (!list_) {
            list_ = std::make_shared<ListData>();
        }
    }

public:
    // Default constructs an empty list
    pylist() : type_(Type::LIST), iv_(0), list_(std::make_shared<ListData>()) {}

    // Construct from int
    pylist(int v) : type_(Type::INT), iv_(v), list_(nullptr) {}

    // Assign from int
    pylist &operator=(int v) {
        type_ = Type::INT;
        iv_ = v;
        list_.reset();
        return *this;
    }

    // Append int
    void append(int v) {
        ensure_list();
        list_->elems.emplace_back(v);
    }

    // Append pylist
    void append(const pylist &x) {
        ensure_list();
        list_->elems.emplace_back(x);
    }

    // Pop back and return the element
    pylist pop() {
        ensure_list();
        // Assuming valid usage: pop is called when not empty
        pylist tmp = list_->elems.back();
        list_->elems.pop_back();
        return tmp;
    }

    // Indexing (non-const)
    pylist &operator[](std::size_t i) {
        ensure_list();
        return list_->elems[i];
    }

    // Indexing (const)
    const pylist &operator[](std::size_t i) const {
        // For const context, avoid modifying structure
        return list_->elems[i];
    }

    // Implicit conversion to int for arithmetic and comparisons
    operator int() const {
        return iv_;
    }

    // Stream output
    friend std::ostream &operator<<(std::ostream &os, const pylist &ls) {
        if (ls.type_ == Type::INT) {
            os << ls.iv_;
            return os;
        }
        os << '[';
        const auto &v = ls.list_->elems;
        for (std::size_t i = 0; i < v.size(); ++i) {
            if (i) os << ", ";
            os << v[i];
        }
        os << ']';
        return os;
    }
};

#endif // PYLIST_H
