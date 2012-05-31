/*
 * judypp - C++ bindings for the Judy library.
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

#ifndef __JUDYPP_MAP_HPP__
#define __JUDYPP_MAP_HPP__

#include <boost/noncopyable.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <Judy.h>
#include <utility>

namespace judypp
{
    //! Key must be POD, T must be POD
    template <typename Key, typename T>
    class Map : boost::noncopyable
    {
        Pvoid_t m_Array;

    public:
        BOOST_STATIC_ASSERT(sizeof(Key) <= sizeof(Word_t));
        BOOST_STATIC_ASSERT(sizeof(T) <= sizeof(Word_t));
        BOOST_STATIC_ASSERT(boost::is_integral<Key>::value || boost::is_pointer<Key>::value);
        BOOST_STATIC_ASSERT(boost::is_integral<T>::value || boost::is_pointer<T>::value);

        typedef Key key_type;
        typedef T mapped_type;
        typedef std::pair<const Key, T> value_type;

        Map() : m_Array(NULL) {}
        ~Map() { clear(); }

        // own interface
        //! inserts value by key or searches for existing. \return reference to it
        mapped_type& put(key_type key) { return *reinterpret_cast<mapped_type*>(JudyLIns(&m_Array, (Word_t)key, PJE0)); }

        //! searches for the element by key. \return pointer to it or NULL
        const mapped_type* get(key_type key) const { return reinterpret_cast<mapped_type*>(JudyLGet(m_Array, (Word_t)key, PJE0)); }
        mapped_type* get(key_type key) { return const_cast<mapped_type*>(const_cast<const Map<Key,T>*>(this)->get(key)); }

        bool del(key_type key) { return JudyLDel(&m_Array, (Word_t)key, PJE0); }

        size_t size() const { return JudyLCount(m_Array, 0, -1, PJE0); }

        bool empty() const { return 0 == size(); }

        void clear() { JudyLFreeArray(&m_Array, PJE0); }

        // std::map interface
        //! return true if new the key is inserted, false if key is already in (value was not changed)
        bool insert(const value_type& v)
        {
            mapped_type* m = get(v.first);
            if (NULL == m)
            {
                put(v.first) = v.second;
                return true;
            }
            return false;
        }

        //! gets or creates value by the key
        T& operator[] (const key_type& k) { return put(k); }

        //! return count of erased keys (0 or 1)
        size_t erase(const key_type& k) { return del(k); }
    };
}// judypp

#endif
