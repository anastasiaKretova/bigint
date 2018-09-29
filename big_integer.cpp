#include "big_integer.h"
#include <algorithm>

const ui big_integer::kLogBase = 32;
const ull big_integer::kBase = (ull)1 << kLogBase;

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
        data.push_back((unsigned)(-a));
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
    if (sign != rhs.sign) {
        return *this += -rhs;
    }
    if (!sign && *this < rhs || sign && *this > rhs) {
        return *this = -(rhs - *this);
    }
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] > rhs.data[i]) {
            data[i] -= rhs.data[i];
        } else {
            size_t p = i + 1;
            while (data[p] == 0) {
                data[p++] += kBase - 1;
            }
            data[p] -= 1;
            data[i] -= rhs.data[i];
        }
    }
    make_right();
    return *this;
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
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
}

std::pair<big_integer, ui> sDiv(big_integer const & a, ui const & b)
//std::pair<big_integer, ui> sDiv(big_integer const &a, unsigned int const &b) 
{
    big_integer res;
    res.sign = a.sign;
    unsigned long long cur = 0;
    size_t temp = 0;
    for (size_t i = a.data.size() - 1; --i; i >= 0) {
        temp = cur * big_integer::kBase + a.data[i];
        res.data[i] = temp / b;
        cur *= big_integer::kBase;
        cur += i;
        res.data.push_back((unsigned)(cur / b));
        cur -= res.data.back() * b;
    }
    res.make_right();
    return { res, cur };
}

void myDiv(big_integer const &a, big_integer &b, big_integer &res, big_integer &cur) {
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
    big_integer u(a.data, a.sign);
    ull n = a.data.size(), m = u.data.size() - b.data.size();
    ull scale = big_integer::kBase / (b.data[n - 1] + 1); //нормализация
    if (scale > 1) {   //нормализация
        u *= scale;
        b *= scale;
    }
    ull uJ, vJ, i;                  //vJ - тек сдвиг B относ U(при вычитании), инд очередной цифры частного
    long temp1, temp2, temp = 0;   //uJ - тек цифра U
    ull qGuess, r;                 //догадка для частного и остаток
    long borrow, carry; //переносы
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
        //U - B * qGuess начиная с vJ+i позиции
        carry = 0, borrow = 0;
        ui *uShift = &u.data[vJ];
        for (i = 0; i < n; ++i) {
            temp1 = b.data[i] * qGuess + carry;
            carry = temp1 / big_integer::kBase;
            temp1 -= carry * big_integer::kBase;
            temp2 = uShift[i] - temp1 + borrow;
            if (temp2 < 0) {
                uShift[i] = temp2 + big_integer::kBase;
                borrow = -1;
            } else {
                uShift[i] = temp2;
                borrow = 0;
            }
        }
        temp2 = uShift[i] - carry + borrow;
        if (temp2 < 0) {
            uShift[i] = temp2 + big_integer::kBase;
            borrow = -1;
        } else {
            uShift[i] = temp2;
            borrow = 0;
        }
        if (borrow == 0) res.data[vJ] = qGuess;
        else { //qGuess на 1 больше нужного
            res.data[vJ] = qGuess - 1;
            carry = 0;
            for (i = 0; i < n; ++i) {
                temp = uShift[i] + b.data[i] + carry;
                if ((unsigned)temp >= big_integer::kBase) {
                    uShift[i] = temp - big_integer::kBase;
                    carry = 1;
                } else {
                    uShift[i] = temp;
                    carry = 0;
                }
            }
            uShift[i] = uShift[i] + carry - big_integer::kBase;
        }
        u.make_right();
    }
    res.make_right();
    if (scale > 1) {
        std::pair<big_integer, ui> p = sDiv(b, scale);
        b = p.first;
        p = sDiv(u, scale);
        cur = p.first;
    } else cur = u;
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    big_integer b = rhs, res, cur;
    myDiv(*this, b, res, cur);
    *this = res;
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    big_integer b = rhs, res, cur;
    myDiv(*this, b, res, cur);
    *this = cur;
    return *this;
}

big_integer &big_integer::apply_bit_operation(big_integer const &rhs, const std::function<ui(ui, ui)> func) {
    size_t len = std::max(data.size(), rhs.data.size());
    ui emptyCell = getEmptyCell();
    size_t oldSize = data.size();
    data.resize(len);
    while (oldSize < len) {
        data[oldSize++] = emptyCell;
    }
    for (size_t i = 0; i < len; ++i) {
        data[i] = func(i < data.size() ? data[i] : this->getEmptyCell(),
                       i < rhs.data.size() ? rhs.data[i] : rhs.getEmptyCell());
    }
    make_right();
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

big_integer big_integer::operator~() const {
    big_integer r = *this;
    if (data.empty()) {
        r.data.push_back(0);
    }
    for (size_t i = 0; i < r.data.size(); ++i) {
        r.data[i] = ~r.data[i];
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
