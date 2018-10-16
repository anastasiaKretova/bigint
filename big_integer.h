#pragma once
#include <functional>
#include <string>
#include <vector>

typedef unsigned int ui;
typedef unsigned long long ull;

class big_integer {

private:
    void make_right();
    void shiftCells(int rhs);
    ui getEmptyCell() const;

public:
    big_integer();
    big_integer(big_integer const &other);
    big_integer(std::vector<ui> const &obj, bool _sign);
    big_integer(ui a);
    big_integer(int a);
    explicit big_integer(std::string const &str);
    //~big_integer();


    big_integer &operator=(big_integer const &other);
    //big_integer &operator*=(ui const &rhs);
    big_integer &operator-=(big_integer const &rhs);
    big_integer &operator+=(big_integer const &rhs);
    big_integer &operator*=(big_integer const &rhs);
    big_integer &operator/=(big_integer const &rhs);
    big_integer &operator%=(big_integer const &rhs);
    big_integer &apply_bit_operation(big_integer const &rhs, const std::function<ui(ui, ui)> func);
    big_integer &operator&=(big_integer const &rhs);
    big_integer &operator|=(big_integer const &rhs);
    big_integer &operator^=(big_integer const &rhs);
    big_integer &operator<<=(int rhs);
    big_integer &operator>>=(int rhs);
    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;
    big_integer &operator++();
    const big_integer operator++(int);
    big_integer &operator--();
    const big_integer operator--(int);

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

    std::string to_string() const;
    friend big_integer sMul(big_integer &a, ui const &b);
    friend std::pair<big_integer, ui> sDiv(big_integer const &a, ui const &b);
    friend void myDiv(big_integer const &a, big_integer &b, big_integer &res, big_integer &cur);
    friend void myt(big_integer &a);
    //friend big_integer operator*(big_integer const &a, ui const &b);

private:
    std::vector<ui> data;
    bool sign;

    static const ui kLogBase;
    static const ull kBase;
};

big_integer operator+(big_integer a, big_integer const &b);
big_integer operator-(big_integer a, big_integer const &b);
big_integer operator*(big_integer a, big_integer const &b);
big_integer operator/(big_integer a, big_integer const &b);
big_integer operator%(big_integer a, big_integer const &b);
big_integer operator&(big_integer a, big_integer const &b);
big_integer operator|(big_integer a, big_integer const &b);
big_integer operator^(big_integer a, big_integer const &b);
big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const &a, big_integer const &b);
bool operator!=(big_integer const &a, big_integer const &b);
bool operator<(big_integer const  &a, big_integer const &b);
bool operator>(big_integer const  &a, big_integer const &b);
bool operator<=(big_integer const &a, big_integer const &b);
bool operator>=(big_integer const &a, big_integer const &b);

std::string to_string(big_integer const &a);
std::ostream &operator<<(std::ostream &s, big_integer const &a);
std::istream &operator>>(std::istream &s, big_integer &a);
big_integer sMul(big_integer &a, ui const &b);
std::pair<big_integer, ui> sDiv(big_integer const &a, ui const &b);
void myDiv(big_integer const &a, big_integer &b, big_integer &res, big_integer &cur);
void myt(big_integer &a);

//big_integer operator*(big_integer const &a, ui const &b);
