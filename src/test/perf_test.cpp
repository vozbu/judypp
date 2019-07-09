/*
 * judypp - aC++ bindings for the Judy library.
 * Copyright (C) 2012 Valeriy Bykov <valery.bickov@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Judy:   http://judy.sourceforge.net/
 * judypp: https://github.com/vozbu/judypp
 */

/*
 * This test tests insertion into different containers and, which more important,
 * test for inserted values.
 */

#include <chrono>
#include <stdio.h>
#include <iostream>
#include <string.h>

#include <judypp/set.hpp>
#include <set>
#include <unordered_set>
#include <vector>
#ifdef HAVE_GOOGLE_SPARSE_HASH
#   include <sparsehash/dense_hash_set>
#endif

template <class F>
std::chrono::microseconds benchmark(F f)
{
    const auto start = std::chrono::high_resolution_clock::now();
    f();
    const auto stop = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
}

// Abstract set interface
// template <typename T>
// struct Set
// {
//      void insert(T);
//      // we never test index more than maximum inserted (for maximum BitSet performance)
//      bool test(T) const;
// };

template <typename T>
class BitSet
{
    uint8_t* m_arr = nullptr;
    int m_size = 0;

public:
    BitSet() = default;
    BitSet(const BitSet& r) : m_size(r.m_size)
    {
        if (0 != m_size)
        {
            m_arr = (uint8_t*)malloc(m_size);
            memcpy(m_arr, r.m_arr, m_size);
        }
    }
    BitSet& operator= (const BitSet& r) = delete;
    ~BitSet() { free(m_arr); }

    void insert(T bit)
    {
        int byte = bit >> 3;
        if (byte >= m_size)
        {
            int new_size = (byte + 1) * 2;
            m_arr = (uint8_t*)realloc(m_arr, new_size);
            memset(m_arr + m_size, 0, new_size - m_size);
        }
        m_arr[byte] |= (uint8_t)(1 << (bit & 7));
    }

    bool test(T bit) const
    {
        return (m_arr[bit >> 2] >> (bit & 7)) & 1;
    }
};

template <typename T>
class StdSet
{
    std::set<T> m_set;
public:
    void insert(T bit) { m_set.insert(bit); }
    bool test(T bit) const { return m_set.end() != m_set.find(bit); }
};

template <typename T>
class StdUnorderedSet
{
    std::unordered_set<T> m_set;
public:
    void insert(T bit) { m_set.insert(bit); }
    bool test(T bit) const { return m_set.end() != m_set.find(bit); }
};

#ifdef HAVE_GOOGLE_SPARSE_HASH
template <typename T>
class DenseHashSet
{
    google::dense_hash_set<T> m_set;
public:
    DenseHashSet() { m_set.set_empty_key(0); }
    void insert(T bit) { m_set.insert(bit); }
    bool test(T bit) const { return m_set.end() != m_set.find(bit); }
};
#endif

template <typename T>
class VectorBoolSet
{
    std::vector<bool> m_set;
public:
    void insert(T bit)
    {
        if (static_cast<int>(m_set.size()) <= bit)
            m_set.resize(bit + 1);
        m_set[bit] = true;
    }
    bool test(T bit) const { return m_set[bit]; }
};

volatile bool b;

template <template <typename T> class Cont, typename T>
void test_LimitedNumbers(const char* name, T count, T start, T step)
{
    Cont<T> t;
    std::cout << name << " " << count << " filled in " << benchmark([&] ()
            {
                for (T i = count; i > 0; --i)
                {
                    t.insert(start);
                    start += step;
                }
            }).count() << " mcs\n";

    std::cout << name << " " << count << " all numbers checked in " << benchmark([&] ()
            {
                for (T i = start - step; i > 0; --i)
                    b = t.test(i);
            }).count() << " mcs\n";

    Cont<T>* p = nullptr;

    std::cout << name << " " << count << " copied in " << benchmark([&] ()
            {
                p = new Cont<T>(t);
            }).count() << " mcs\n";

    std::cout << name << " " << count << " destroyed in " << benchmark([&] ()
            {
                delete p;
            }).count() << " mcs\n";
}

template <typename T>
void test_allLimitedNumers(const char* type, T count, T start, T step)
{
    std::cout << "\n===== " << type << "(" << count << ", " << start << ", " << step << ") =====\n";
    std::cout <<"\n";
    test_LimitedNumbers<BitSet, T>("bitset", count, start, step);
    std::cout <<"\n";
    test_LimitedNumbers<judypp::Set, T>("judypp::set", count, start, step);
    std::cout <<"\n";
    test_LimitedNumbers<StdSet, T>("std::set", count, start, step);
    std::cout <<"\n";
    test_LimitedNumbers<StdUnorderedSet, T>("std::unordered_set", count, start, step);
    std::cout <<"\n";
#ifdef HAVE_GOOGLE_SPARSE_HASH
    test_LimitedNumbers<DenseHashSet, T>("google::dense_hash_set", count, start, step);
    std::cout <<"\n";
#endif
    test_LimitedNumbers<VectorBoolSet, T>("std::vector<bool>", count, start, step);
    std::cout <<"\n";
}

int main()
{
    test_allLimitedNumers<int64_t>("int64_t", 1000, 10, 1);
    test_allLimitedNumers<int64_t>("int64_t", 1000, 10, 3);
    test_allLimitedNumers<int64_t>("int64_t", 10000, 10, 1);
    test_allLimitedNumers<int64_t>("int64_t", 10000, 10, 3);
    test_allLimitedNumers<int64_t>("int64_t", 100000, 10, 1);
    test_allLimitedNumers<int64_t>("int64_t", 100000, 10, 3);

    test_allLimitedNumers<int32_t>("int32_t", 1000, 10, 1);
    test_allLimitedNumers<int32_t>("int32_t", 1000, 10, 3);
    test_allLimitedNumers<int32_t>("int32_t", 10000, 10, 1);
    test_allLimitedNumers<int32_t>("int32_t", 10000, 10, 3);
    test_allLimitedNumers<int32_t>("int32_t", 100000, 10, 1);
    test_allLimitedNumers<int32_t>("int32_t", 100000, 10, 3);

    test_allLimitedNumers<int16_t>("int16_t", 1000, 10, 1);
    test_allLimitedNumers<int16_t>("int16_t", 1000, 10, 3);
    test_allLimitedNumers<int16_t>("int16_t", 10000, 10, 1);
    test_allLimitedNumers<int16_t>("int16_t", 10000, 10, 3);

    return 0;
}
