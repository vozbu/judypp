#ifndef __JUDYPP_SET_HPP__
#define __JUDYPP_SET_HPP__

#include <boost/noncopyable.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <Judy.h>
#include <judypp/set_iter.hpp>

namespace judypp
{
    //! Key must be an integral type with sizeof(Key) <= sizeof(Word_t)
    template <typename Key>
    class Set : boost::noncopyable
    {
        Pvoid_t m_Array;

    public:
        BOOST_STATIC_ASSERT(sizeof(Key) <= sizeof(Word_t));
        BOOST_STATIC_ASSERT(boost::is_integral<Key>::value || boost::is_pointer<Key>::value);

        typedef Key key_type;
        typedef Key value_type;

        typedef set_const_iterator<Key> const_iterator;

        Set() : m_Array(NULL) {}
        ~Set() { clear(); }

        //! returns true if new bit is set in result of call, otherwise returns false
        bool set(key_type key) { return Judy1Set(&m_Array, (Word_t)key, PJE0); }

        //! returns true if bit is unset in result of call, otherwise returns false
        bool unset(key_type key) { return Judy1Unset(&m_Array, (Word_t)key, PJE0); }

        bool test(key_type key) const { return Judy1Test(m_Array, (Word_t)key, PJE0); }

        size_t size() const { return Judy1Count(m_Array, 0, -1, PJE0); }

        bool empty() const { return 0 == size(); }

        void clear() { Judy1FreeArray(&m_Array, PJE0); }

        // --- std::set interface ---

        bool insert(const value_type& v) { return set(v); }

        //! return count of erased keys (0 or 1)
        size_t erase(const key_type& k) { return unset(k); }
        void erase(const_iterator p) { erase(*p); }

        const_iterator begin() const { return const_iterator(m_Array); }
        const_iterator end()   const { return const_iterator(); }
    };
}// judypp

#endif
