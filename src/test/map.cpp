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
    pair<unsigned long, char>
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

    js.put(0);
    BOOST_CHECK_EQUAL(false, js.empty());
    BOOST_CHECK_EQUAL(1u, js.size());

    js.put(1);
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

    BOOST_CHECK_EQUAL(np, js.get(0));
    BOOST_CHECK_EQUAL(np, js.get(1));

    js.put(0) = 15;
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_REQUIRE_NE (np, js.get(0));
    BOOST_CHECK_EQUAL(np, js.get(1));
    BOOST_CHECK_EQUAL(ValT(15), *js.get(0));

    js.put(0) = 18;
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_REQUIRE_NE (np, js.get(0));
    BOOST_CHECK_EQUAL(np, js.get(1));
    BOOST_CHECK_EQUAL(ValT(18), *js.get(0));

    js.put(1) = 23;
    BOOST_CHECK_EQUAL(2u, js.size());
    BOOST_REQUIRE_NE (np, js.get(0));
    BOOST_REQUIRE_NE (np, js.get(1));
    BOOST_CHECK_EQUAL(ValT(18), *js.get(0));
    BOOST_CHECK_EQUAL(ValT(23), *js.get(1));

    *js.get(1) = 12;
    BOOST_CHECK_EQUAL(ValT(12), *js.get(1));

    BOOST_CHECK_EQUAL(false, js.del(3));
    BOOST_CHECK_EQUAL(2u, js.size());

    BOOST_CHECK_EQUAL(true, js.del(0));
    BOOST_CHECK_EQUAL(np, js.get(0));
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_REQUIRE_NE (np, js.get(1));
    BOOST_CHECK_EQUAL(ValT(12), *js.get(1));

    BOOST_CHECK_EQUAL(false, js.del(0));
    BOOST_CHECK_EQUAL(1u, js.size());

    BOOST_CHECK_EQUAL(true, js.del(1));
    BOOST_CHECK_EQUAL(0u, js.size());
    BOOST_CHECK_EQUAL(np, js.get(1));

    // clear
    js.put(0);
    js.put(1);
    BOOST_CHECK_EQUAL(2u, js.size());
    BOOST_REQUIRE_NE (np, js.get(0));
    BOOST_REQUIRE_NE (np, js.get(1));

    js.clear();
    BOOST_CHECK_EQUAL(0u, js.size());
    BOOST_CHECK_EQUAL(np, js.get(0));
    BOOST_CHECK_EQUAL(np, js.get(1));

    //std::map interface
    BOOST_CHECK_EQUAL(true, js.insert(std::make_pair(KeyT(3), ValT(1))));
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_REQUIRE_NE (np, js.get(3));
    BOOST_CHECK_EQUAL(ValT(1), *js.get(3));

    BOOST_CHECK_EQUAL(false, js.insert(std::make_pair(KeyT(3), ValT(5))));
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_REQUIRE_NE (np, js.get(3));
    BOOST_CHECK_EQUAL(ValT(1), *js.get(3));

    js[10] = 3;
    BOOST_CHECK_EQUAL(2u, js.size());
    BOOST_REQUIRE_NE (np, js.get(3));
    BOOST_CHECK_EQUAL(ValT(1), *js.get(3));
    BOOST_REQUIRE_NE (np, js.get(10));
    BOOST_CHECK_EQUAL(ValT(3), *js.get(10));

    BOOST_CHECK_EQUAL(1u, js.erase(3));
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_CHECK_EQUAL(0u, js.erase(3));
}

BOOST_AUTO_TEST_SUITE_END()
