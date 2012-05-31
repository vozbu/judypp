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

#ifndef __JUDYPP_SET_ITER_HPP__
#define __JUDYPP_SET_ITER_HPP__

#include <Judy.h>
#include <iterator>

namespace judypp
{
    class set_const_iterator_base
    {
    protected:
        Pcvoid_t m_Array;
        Word_t m_Index;
        bool m_End;

    public:
        // like from begin() or end()
        set_const_iterator_base(Pcvoid_t aArray) : m_Array(aArray)
        {
            if (NULL != m_Array)
            {
                m_Index = 0;
                // should not return 0 if m_Array is not NULL
                Judy1First(m_Array, &m_Index, PJE0);
                m_End = false;
            }
            else
            {
                m_End = true;
            }
        }

        // like from find(key) - search for concrete element
        set_const_iterator_base(Pcvoid_t aArray, Word_t aIndex) : m_Array(aArray)
        {
            if (NULL != m_Array)
            {
                m_Index = aIndex;
                m_End = 0 == Judy1First(m_Array, &m_Index, PJE0) || m_Index != aIndex;
            }
            else
            {
                m_End = true;
            }
        }

        bool operator == (const set_const_iterator_base& r) const
        {
            if (m_End && r.m_End)
                return true;
            else if (m_End != r.m_End)
                return false;
            else
                return m_Array == r.m_Array && m_Index == r.m_Index;
        }

        void operator++ ()
        {
            if (0 == Judy1Next(m_Array, &m_Index, PJE0))
                m_End = true;
        }

        void operator-- ()
        {
            if (0 == Judy1Prev(m_Array, &m_Index, PJE0))
                m_End = true;
        }
    };

    inline bool operator != (const set_const_iterator_base& l, const set_const_iterator_base& r)
    {
        return !(l == r);
    }

    template <typename Key>
    class set_const_iterator : public set_const_iterator_base
    {
        typedef set_const_iterator_base _Mytype;
        typedef set_const_iterator_base _Mybase;

    public:
        typedef std::iterator<std::bidirectional_iterator_tag, const Key, ptrdiff_t, void, Key> __iter_t;
        typedef typename __iter_t::value_type           value_type;
        typedef typename __iter_t::difference_type      difference_type;
        typedef typename __iter_t::pointer              pointer;
        typedef typename __iter_t::reference            reference;
        typedef typename __iter_t::iterator_category    iterator_category;

        set_const_iterator(Pcvoid_t aArray = NULL) : _Mybase(aArray) {}
        set_const_iterator(Pcvoid_t aArray, Key aKey) : _Mybase(aArray, (Word_t)aKey) {}

        _Mytype& operator++ ()
        {
            _Mybase::operator++();
            return *this;
        }

        _Mytype operator++ (int)
        {
            _Mytype tmp = *this;
            ++*this;
            return tmp;
        }

        _Mytype& operator-- ()
        {
            _Mybase::operator--();
            return *this;
        }

        _Mytype operator-- (int)
        {
            _Mytype tmp = *this;
            --*this;
            return tmp;
        }

        reference operator* () const
        {
            return (reference)m_Index;
        }
    };
}// judypp

#endif
