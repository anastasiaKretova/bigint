#include <iostream>
#include "vector.h"

my_vector::my_vector(): _size(0), is_small(true) {
    data.small = 0;
}

my_vector::~my_vector() {
    if (!is_small) {
        data.big.reset();
        data.big.~shared_ptr();
    }
}

void my_vector::make_big() {
    if (!is_small) {
        return;
    }
    is_small = false;
    unsigned int dat = data.small;
    vector<unsigned int> *copy = new vector<unsigned int>(_size);
    new(&data.big) std::shared_ptr<std::vector<unsigned int>>((copy));
    (*data.big.get())[0] = dat;
}

void my_vector::make_unique() {
    if (is_small || data.big.unique()) {
        return;
    }
    data.big = std::make_shared<std::vector<unsigned int>>(*data.big);
}

my_vector::my_vector(my_vector const &other) {
    _size = other._size;
    is_small = other.is_small;
    if (is_small) {
        data.small = other.data.small;
    } else {
        new(&data.big) std::shared_ptr<vector<unsigned int>>(other.data.big);
    }
}

my_vector& my_vector::operator=(my_vector const &other) {
    _size = other._size;
    if (!is_small) {
        data.big.reset();
    }
    is_small = other.is_small;
    if (is_small) {
        data.small = other.data.small;
    } else {
        new(&data.big) std::shared_ptr<vector<unsigned int>>(other.data.big);
    }
    return *this;
}

size_t my_vector::size() const {
    return _size;
}

unsigned int my_vector::back() const {
    assert(_size > 0);
    if (is_small) {
        return data.small;
    }
    return (*data.big.get())[_size - 1];
}

void my_vector::resize(size_t len) {
    _size = len;
    if (len == 1) {
        if (!is_small) {
            unsigned int dat = (*data.big.get())[0];
            is_small = true;
            make_unique();
            data.big.~shared_ptr();
            data.small = dat;
        }
        return;
    }
    if (!is_small) {
        data.big->resize(len);
    } else {
        make_big();
//        for (size_t i = 1; i < _size; ++i) {
//            (*data.big)[i] = 0;
//        }
    }
}

void my_vector::clear() {
    make_unique();
    if (!is_small) {
        data.big.reset();
        //data.big.~shared_ptr();
        is_small = true;
    }
    memset(&data.small, 0, sizeof(unsigned int));
    _size = 0;
}

bool my_vector::empty() const{
    return (_size == 0);
}

void my_vector::push_back(unsigned int x) {
    if (is_small && _size < 1) {
        data.small = x;
        ++_size;
        return;
    }
    make_unique();
    make_big();
    (*data.big.get()).push_back(x);
    ++_size;
}

void my_vector::pop_back() {
    assert(_size > 0);
    --_size;
    if (is_small){
        return;
    }
    make_unique();
    (*data.big.get()).pop_back();
    if (data.big->size() == 1) {
        unsigned int dat = data.big->back();
        //data.big.~shared_ptr();
        is_small = true;
        data.small = dat;
    }
}

unsigned int& my_vector::operator[](size_t ind) {
    assert(ind < _size);
    if (is_small) {
        return data.small;
    }
    make_unique();
    return (*data.big.get())[ind];
}

unsigned int const& my_vector::operator[](size_t ind) const {
    assert(ind < _size);
    if (is_small) {
        return data.small;
    }
    return (*data.big.get())[ind];
}