#include "big_integer.h"
#include <algorithm>
#include <iostream>

const ui big_integer::kLogBase = 32;
const ull big_integer::kBase = (ull)1 << kLogBase;
const ui big_integer::maxNumber = (ui) 1e9;

big_integer::big_integer() {
    data.push_back(0);
    sign = false;
}

big_integer::big_integer(big_integer const &other) {
    sign = other.sign;
    data = other.data;
}

big_integer::big_integer(ui a) {
    sign = false;
    data.resize(0);
    if (a == 0) {
        data.push_back(0);
    } else {
        data.push_back(a);
    }
}

big_integer::big_integer(int a) {
    sign = false;
    data.resize(0);
    if (a > 0) {
        data.push_back((unsigned)a);
    } if (a == 0) {
        data.push_back(0);
    } if (a < 0) {
        //if (a == std::numeric_limits<int>::min()) {
        //    data.push_back((unsigned)(-a + 1));
        //    data.push_back((unsigned)1);
        //} else {
        data.push_back((unsigned)(-a));
        //}
        sign = true;
    }
}

big_integer::big_integer(std::vector<ui> const &obj, bool _sign) {
    data = obj;
    sign = _sign;
    if (obj.empty())
        data.push_back(0);
    if (obj.size() == 1 && obj[0] == 0)
        sign = false;
}


std::string big_integer::BASE_to_string(ui n, bool f) const {
    std::string s;
    while (n != 0) {
        s = char((n % 10) + '0') + s;
        n /= 10;
    }
    if (f)
        while (s.length() < 9) {
            s = "0" + s;
        }
    return s;
}

ui big_integer::getEmptyCell() const
{
    if (sign) {
        return (ui)(kBase - 1);
    }
    return 0;
}

std::string big_integer::to_string() const {
    std::string s = "";
    for (int i = 0; i < (int)data.size(); ++i) {
        if (i != (int)data.size() - 1)
            s = BASE_to_string(data[i], true) + s;
        else
            s = BASE_to_string(data[i], false) + s;
    }
    if (this->sign) {
        s = "-" + s;
    }
    if (s.empty()) {
        return "0";
    } else {
        return s;
    }
}

big_integer::big_integer(std::string const &str) {
    std::string s(str);
    if (s == "0") {
        return;
    }
    if (s[0] == '-') {
        sign = true;
        s.erase(s.begin());
    } else {
        sign = false;
    }
    while (s.length() % 9 != 0) {
        s = '0' + s;
    }
    long k = s.length() / 9;
    for (long i = 0; i < k; ++i) {
        data.push_back(0);
        for (int j = 8; j >= 0; --j) {
            data[i] += ((int((s[i * 9 + j])) - int('0')) * BASE_Degree[8 - j]);
        }
    }
    for (int i = 0; i < (int)data.size() / 2; ++i) {
        ui a = data[i];
        data[i] = data[(int)data.size() - i - 1];
        data[data.size() - i - 1] = a;
    }
}


big_integer &big_integer::operator=(big_integer const &other) {
    data = other.data;
    sign = other.sign;
    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    return big_integer(data, !sign);
}

void big_integer::make_right() {
    if (data.empty()) {
        data.push_back(0);
        return;
    }
    size_t i = data.size() - 1;
    while (i > 0 && data[i] == 0) i--;
    data.resize(i + 1);
}

big_integer& big_integer::operator*=(ui const &b) {
    big_integer res;
    res.sign = this->sign;
    unsigned c = 0;
    for (size_t i = 0; i < this->data.size(); ++i) {
        res.data[i] = this->data[i] * b + c;
        c = ((unsigned long long)this->data[i] * b + c) / big_integer::kBase;
    }
    res.make_right();
    return *this;
}

big_integer& big_integer::operator+=(big_integer const &rhs) {
    if (sign != rhs.sign) {
        if (sign) {
            *this = (rhs - -*this);
            return *this;
        } else {
            return (*this -= -rhs);
        }
    }
    size_t len = std::max(data.size(), rhs.data.size()) + 1;
    size_t oldSize = data.size();
    data.resize(len);
    std::fill(data.begin() + oldSize, data.begin() + len, 0);
    unsigned long c = 0;
    for (size_t i = 0; i < std::min(len, rhs.data.size()); ++i) {
        data[i] = data[i] + rhs.data[i] + c;
        c = (data[i] + (unsigned long long)rhs.data[i]) / kBase;
    }
    data[std::min(len, rhs.data.size())] += c;
    make_right();
    return *this;
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
	if (*this == rhs) {
		data.resize(0);
		data.push_back(0);
		sign = false;
		return *this;
	}
	std::cout << this->data.size() << std::endl;
	if (sign != rhs.sign) {
		return *this += -rhs;
	}
	if (!sign && *this < rhs || sign && *this > rhs) {
		return *this = -(rhs - *this);
	}
	long long temp = 0;
	for (size_t i = 0; i < rhs.data.size() || temp; ++i) {
		long long cur = data[i];
		cur -= temp;
		if (i < rhs.data.size()) {
			cur -= rhs.data[i];
		}
		if (cur < 0)
			temp = 1;
		else
			temp = 0;
		if (temp)
			cur += big_integer::maxNumber;
		data[i] = cur;
	}
	make_right();
	return *this;
}


big_integer &big_integer::operator*=(big_integer const &rhs) {
    big_integer res;
    res.data.resize(data.size() + rhs.data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        for (int j = 0, c = 0; j < rhs.data.size() || c; j++) {
            ui temp = res.data[i + j] + data[i] * 1ll * (j < rhs.data.size() ? rhs.data[j] : 0) + c;
            res.data[i + j] = (temp % kBase);
            c = temp / kBase;
        }
    }
    res.sign = sign ^ rhs.sign;
    res.make_right();
    *this = res;
    return  *this;
}


std::pair<big_integer, ui> sDiv(big_integer const & a, ui const & b) {
    //неправильная херня__(fixed)
    big_integer res;
    res = a;
    long long cur = 0;
    long long temp = 0;
    for (int i = a.data.size() - 1; i >= 0; i--) {
        temp = cur * big_integer::kBase + res.data[i];
        res.data[i] = temp / b;
        cur = temp - res.data[i] * b;
    }
    res.make_right();
    return { res, cur };
}

void myDiv(big_integer const &a, big_integer &b, big_integer &res, big_integer &cur) {

    // херня со знаками
    if (a.sign && b.sign) {
        big_integer d = -b;
        myDiv(-a, d, res, cur);
        return;
    }
    if (a.sign) {
        myDiv(-a, b, res, cur);
        if (res == 0) cur.sign = true;
        else res.sign = true;
        return;
    }
    if (b.sign) {
        big_integer d = -b;
        myDiv(a, d, res, cur);
        if (res == 0) cur.sign = true;
        else res.sign = true;
        return;
    }

    //частные случаи

    if (a.data.size() < b.data.size()) {
        res = 0;
        cur = a;
        return;
    }
    if (b.data.size() == 1) {
        std::pair<big_integer, ui> p = sDiv(a, b.data[0]);
        res = p.first;
        cur.data[0] = p.second;
        return;
    }

    // объявление переменных
    big_integer u(a.data, a.sign);
    u.data.resize(a.data.size() + 1);
    u.data[a.data.size()] = 0;


    long n = b.data.size(), m = u.data.size() - b.data.size(); //косяк(fixed)
    long uJ, vJ, i;                  //vJ - тек сдвиг B относ U(при вычитании), инд очередной цифры частного
    long temp1, temp2, temp = 0;   //uJ - тек цифра U
    long qGuess, r;                 //догадка для частного и остаток
    long borrow, carry; //переносы


    long scale = big_integer::kBase / (b.data[n - 1] + 1); //нормализация
    if (scale > 1) {   //нормализация
        u *= scale;
        b *= scale;
    }

    //пошла жара
    for (vJ = m, uJ = n + vJ; vJ >= 0; --vJ, --uJ) {

        qGuess = (u.data[uJ] * big_integer::kBase + u.data[uJ - 1]) / b.data[n - 1];
        r = (u.data[uJ] * big_integer::kBase + u.data[uJ - 1]) % b.data[n - 1];

        while (r < big_integer::kBase) {

            temp2 = b.data[n - 2] * qGuess;
            temp1 = r * big_integer::kBase + u.data[uJ - 2];

            if ((temp2 > temp1) || (qGuess == big_integer::kBase)) {

                --qGuess;                 //не работает, qGuess уменьшим
                r += b.data[n - 1];       //новый остаток

            } else break;     //qGuess правильное или на единицу больше
        }
        //так ну тут сложно было облажаться, поехали дальше
        //U - B * qGuess начиная с vJ+i позиции
        carry = 0, borrow = 0;
        ui *uShift = &u.data[vJ];
        for (i = 0; i < n; i++) {

            temp1 = b.data[i] * qGuess + carry;
            carry = temp1 / big_integer::kBase;
            temp1 -= carry * big_integer::kBase;

            temp2 = u.data[vJ + i] - temp1 + borrow;
            if (temp2 < 0) {
                u.data[i + vJ] = temp2 + big_integer::kBase;
                borrow = -1;
            }
            else {
                u.data[i + vJ] = temp2;
                borrow = 0;
            }
        }
        temp2 = u.data[i + vJ] - carry + borrow;
        if (temp2 < 0) {
            u.data[i + vJ] = temp2 + big_integer::kBase;
            borrow = -1;
        }
        else {
            u.data[i + vJ] = temp2;
            borrow = 0;
        }
        if (borrow == 0) res.data[vJ] = qGuess;
        else { //qGuess на 1 больше нужного
            res.data[vJ] = qGuess - 1;
            carry = 0;
            for (i = 0; i < n; ++i) {
                temp = u.data[i + vJ] + b.data[i] + carry;
                if (temp >= big_integer::kBase) {
                    u.data[i + vJ] = temp - big_integer::kBase;
                    carry = 1;
                }
                else {
                    u.data[i + vJ] = temp;
                    carry = 0;
                }
            }
            u.data[i + vJ] = u.data[i + vJ] + carry - big_integer::kBase;
        }
        u.make_right();
        //u.data.resize(u.data.size() + 1);

    }
    res.make_right();
    //res.data.resize(res.data.size() + 1);
    if (scale > 1) {
        std::pair<big_integer, ui> p = sDiv(b, scale);
        b = p.first;
        p = sDiv(u, scale);
        cur = p.first;
    }
    else cur = u;
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    big_integer b = rhs, res, cur;
    myDiv(*this, b, res, cur);
    *this = res;
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    *this = *this - (*this / rhs) * rhs;
    return *this;
}

big_integer &big_integer::apply_bit_operation(big_integer const &rhs, const std::function<ui(ui, ui)> func) {
    size_t len = std::max(data.size(), rhs.data.size());
    if (sign) {
        sign = false;
        *this = myt(*this) + 1;
    }
    big_integer c_rhs = rhs;
    if (c_rhs.sign) {
        c_rhs.sign = false;
        c_rhs = myt(c_rhs) + 1;
    }
    ui emptyCell = getEmptyCell();
    size_t oldSize = data.size();
    data.resize(len);
    while (oldSize < len) {
        data[oldSize++] = emptyCell;
    }
    for (size_t i = 0; i < len; ++i) {
        data[i] = func(i < data.size() ? data[i] : this->getEmptyCell(),
                       i < c_rhs.data.size() ? c_rhs.data[i] : c_rhs.getEmptyCell());
    }
    make_right();
    if (!data.empty() && (data.back() & ((ui) 1 << (kLogBase - 1)))) {
        *this = myt(*this) + 1;
        sign = true;
    }
    return *this;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    return apply_bit_operation(rhs, [](ui a, ui b) { return a & b; });
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    return apply_bit_operation(rhs, [](ui a, ui b) { return a | b; });
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    return apply_bit_operation(rhs, [](ui a, ui b) { return a ^ b; });
}

void big_integer::shiftCells(int rhs) {
    if (rhs > 0) {
        size_t oldSize = data.size();
        data.resize(data.size() + rhs);
        for (size_t i = oldSize; i > 0; --i) {
            data[i + rhs - 1] = data[i - 1];
        }
        for (auto i = (size_t)rhs; i > 0; --i) {
            data[i - 1] = 0;
        }
    } else {
        for (auto i = (size_t)-rhs; i < data.size(); ++i) {
            data[i + rhs] = data[i];
        }
        for (size_t i = data.size(); i > data.size() + rhs; --i) {
            data[i - 1] = getEmptyCell();
        }
        make_right();
    }
}

big_integer &big_integer::operator<<=(int rhs) {
    if (rhs < 0) {
        return *this >>= -rhs;
    }
    int auxShift = rhs / kLogBase;
    if (auxShift) {
        shiftCells(auxShift);
    }
    auxShift = rhs - auxShift * kLogBase;
    if (auxShift) {
        data.push_back(getEmptyCell());
        for (size_t i = data.size(); i > 0; --i) {
            if (i != data.size()) {
                data[i] += data[i - 1] >> (kLogBase - auxShift);
            }
            data[i - 1] <<= auxShift;
        }
    }
    make_right();
    return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
    if (rhs < 0) {
        return *this <<= -rhs;
    }
    int auxShift = rhs / kLogBase;
    if (auxShift) {
        shiftCells(-auxShift);
    }
    auxShift = rhs - auxShift * kLogBase;
    if (auxShift) {
        ui cur = getEmptyCell();
        for (size_t i = 0; i < data.size(); ++i) {
            if (i != 0) {
                data[i - 1] += data[i] << (kLogBase - auxShift);
            }
            data[i] >>= auxShift;
        }
        data.back() += cur << (kLogBase - auxShift);
    }
    make_right();
    return *this;
}

big_integer myt(big_integer const &a) {
    big_integer aa = a;
    for (size_t i = 0; i < a.data.size(); ++i) {
        aa.data[i] = ~a.data[i];
    }
    return aa;
}

big_integer big_integer::operator~() const {
    big_integer r = *this;
    if (data.empty()) {
        r.data.push_back(0);
    }
    if (r.sign) {
        r.sign = false;
        r = myt(r) + 1;
    }
    for (size_t i = 0; i < r.data.size(); ++i) {
        r.data[i] = ~r.data[i];
    }
    if (!r.data.empty() && (r.data.back() & ((ui) 1 << (kLogBase - 1)))) {
        r = myt(r) + 1;
        r.sign = true;
    }
    r.make_right();
    return r;
}

big_integer &big_integer::operator++() {
    return *this += 1;
}

const big_integer big_integer::operator++(int) {
    big_integer r = *this;
    ++*this;
    return r;
}

big_integer &big_integer::operator--() {
    return *this -= 1;
}

const big_integer big_integer::operator--(int) {
    big_integer r = *this;
    --*this;
    return r;
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const &b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const &b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const &b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const &b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}


std::string to_string(big_integer const &a) {
    return a.to_string();
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << a.to_string();
}

std::istream &operator>>(std::istream &s, big_integer &a) {
    std::string aux;
    s >> aux;
    a = big_integer(aux);
    return s;
}

bool operator==(big_integer const & a, big_integer const & b) {
    return (!(a < b) && !(a > b));
}

bool operator!=(big_integer const & a, big_integer const & b) {
    return !(a == b);
}

bool operator<(big_integer const & a, big_integer const & b) {
    if (a.sign) {
        if (b.sign) return ((-b) < (-a));
        else return true;
    } else if (b.sign) return false;
    else {
        if ((int)a.data.size() != (int)b.data.size()) {
            return (int)a.data.size() < (int)b.data.size();
        } else {
            for (int i = (int)a.data.size() - 1; i >= 0; --i) {
                if (a.data[i] != b.data[i]) return a.data[i] < b.data[i];
            }
            return false;
        }
    }
}

bool operator>(big_integer const & a, big_integer const & b) {
    return (b < a);
}

bool operator<=(big_integer const & a, big_integer const & b) {
    return (!(a > b));
}

bool operator>=(big_integer const & a, big_integer const & b) {
    return (b <= a);
}
