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

#include <judypp/map.hpp>
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(judypp)

template <typename K, typename V>
struct pair
{
    typedef K KeyT;
    typedef V ValT;
};

typedef boost::mpl::list<
    pair<int, int>,
    pair<long, unsigned int>,
    pair<unsigned long, char>,
    pair<void*, int*>,
    pair<char, judypp::Map<int*, int>*>
> map_types_t;

const void* np = NULL;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_map_size_empty_clear, TPair, map_types_t)
{
    // Проверяет, что если size == 0, то empty == true и наоборот, и что clear очищает мапку
    typedef typename TPair::KeyT KeyT;
    typedef typename TPair::ValT ValT;
    judypp::Map<KeyT, ValT> js;

    BOOST_CHECK_EQUAL(true, js.empty());
    BOOST_CHECK_EQUAL(0u, js.size());

    js.put(KeyT(0));
    BOOST_CHECK_EQUAL(false, js.empty());
    BOOST_CHECK_EQUAL(1u, js.size());

    js.put(KeyT(1));
    BOOST_CHECK_EQUAL(false, js.empty());
    BOOST_CHECK_EQUAL(2u, js.size());

    js.clear();
    BOOST_CHECK_EQUAL(true, js.empty());
    BOOST_CHECK_EQUAL(0u, js.size());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_map, TPair, map_types_t)
{
    typedef typename TPair::KeyT KeyT;
    typedef typename TPair::ValT ValT;
    typedef judypp::Map<KeyT, ValT> map_t;
    map_t js;

    BOOST_CHECK_EQUAL(0u, js.size());

    BOOST_CHECK_EQUAL(np, js.get(KeyT(0)));
    BOOST_CHECK_EQUAL(np, js.get(KeyT(1)));

    js.put(KeyT(0)) = ValT(15);
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_REQUIRE_NE (np, js.get(KeyT(0)));
    BOOST_CHECK_EQUAL(np, js.get(KeyT(1)));
    BOOST_CHECK_EQUAL(ValT(15), *js.get(KeyT(0)));

    js.put(KeyT(0)) = ValT(18);
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_REQUIRE_NE (np, js.get(KeyT(0)));
    BOOST_CHECK_EQUAL(np, js.get(KeyT(1)));
    BOOST_CHECK_EQUAL(ValT(18), *js.get(KeyT(0)));

    js.put(KeyT(1)) = ValT(23);
    BOOST_CHECK_EQUAL(2u, js.size());
    BOOST_REQUIRE_NE (np, js.get(KeyT(0)));
    BOOST_REQUIRE_NE (np, js.get(KeyT(1)));
    BOOST_CHECK_EQUAL(ValT(18), *js.get(KeyT(0)));
    BOOST_CHECK_EQUAL(ValT(23), *js.get(KeyT(1)));

    *js.get(KeyT(1)) = ValT(12);
    BOOST_CHECK_EQUAL(ValT(12), *js.get(KeyT(1)));

    BOOST_CHECK_EQUAL(false, js.del(KeyT(3)));
    BOOST_CHECK_EQUAL(2u, js.size());

    BOOST_CHECK_EQUAL(true, js.del(KeyT(0)));
    BOOST_CHECK_EQUAL(np, js.get(KeyT(0)));
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_REQUIRE_NE (np, js.get(KeyT(1)));
    BOOST_CHECK_EQUAL(ValT(12), *js.get(KeyT(1)));

    BOOST_CHECK_EQUAL(false, js.del(KeyT(0)));
    BOOST_CHECK_EQUAL(1u, js.size());

    BOOST_CHECK_EQUAL(true, js.del(KeyT(1)));
    BOOST_CHECK_EQUAL(0u, js.size());
    BOOST_CHECK_EQUAL(np, js.get(KeyT(1)));

    // clear
    js.put(KeyT(0));
    js.put(KeyT(1));
    BOOST_CHECK_EQUAL(2u, js.size());
    BOOST_REQUIRE_NE (np, js.get(KeyT(0)));
    BOOST_REQUIRE_NE (np, js.get(KeyT(1)));

    js.clear();
    BOOST_CHECK_EQUAL(0u, js.size());
    BOOST_CHECK_EQUAL(np, js.get(KeyT(0)));
    BOOST_CHECK_EQUAL(np, js.get(KeyT(1)));

    //std::map interface
    BOOST_CHECK_EQUAL(true, js.insert(std::make_pair(KeyT(3), ValT(1))));
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_REQUIRE_NE (np, js.get(KeyT(3)));
    BOOST_CHECK_EQUAL(ValT(1), *js.get(KeyT(3)));

    BOOST_CHECK_EQUAL(false, js.insert(std::make_pair(KeyT(3), ValT(5))));
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_REQUIRE_NE (np, js.get(KeyT(3)));
    BOOST_CHECK_EQUAL(ValT(1), *js.get(KeyT(3)));

    js[KeyT(10)] = ValT(3);
    BOOST_CHECK_EQUAL(2u, js.size());
    BOOST_REQUIRE_NE (np, js.get(KeyT(3)));
    BOOST_CHECK_EQUAL(ValT(1), *js.get(KeyT(3)));
    BOOST_REQUIRE_NE (np, js.get(KeyT(10)));
    BOOST_CHECK_EQUAL(ValT(3), *js.get(KeyT(10)));

    BOOST_CHECK_EQUAL(1u, js.erase(KeyT(3)));
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_CHECK_EQUAL(0u, js.erase(KeyT(3)));
}

BOOST_AUTO_TEST_SUITE_END()
