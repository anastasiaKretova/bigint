#ifndef pragma_once
#define pragma_once

#include <vector>
#include <memory>
#include <cassert>
#include <cstring>

using std::shared_ptr;
using std::vector;

class my_vector {

    union data {
        unsigned int small;
        shared_ptr<vector<unsigned int>> big;
        data() {};
        ~data() {};

    };

    data data;
    size_t _size;
    bool is_small;

    void make_big();
    void make_unique();

public:

    my_vector();
    ~my_vector();
    my_vector(my_vector const &other);
    my_vector &operator=(my_vector const &other);

    size_t size() const;
    unsigned int back() const;
    void resize(size_t len);
    void clear();
    bool empty() const;

    void push_back(unsigned int x);
    void pop_back();

    unsigned int &operator[](size_t ind);
    unsigned int const &operator[](size_t ind) const;

};

#endif /* pragma_once */