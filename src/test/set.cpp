#include <judypp/set.hpp>
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(judypp)

typedef boost::mpl::list<int, long, unsigned int, unsigned long, void*, int*> set_types_t;
BOOST_AUTO_TEST_CASE_TEMPLATE(test_set, T, set_types_t)
{
    judypp::Set<T> js;
    BOOST_CHECK_EQUAL(true, js.empty());
    BOOST_CHECK_EQUAL(0u, js.size());
    BOOST_CHECK_EQUAL(false, js.test(0));

    BOOST_CHECK_EQUAL(true, js.set(0));
    BOOST_CHECK_EQUAL(false, js.empty());
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_CHECK_EQUAL(true, js.test(0));

    BOOST_CHECK_EQUAL(false, js.set(0));
    BOOST_CHECK_EQUAL(false, js.empty());
    BOOST_CHECK_EQUAL(1u, js.size());
    BOOST_CHECK_EQUAL(true, js.test(0));

    BOOST_CHECK_EQUAL(true, js.set(T(100500)));
    BOOST_CHECK_EQUAL(false, js.empty());
    BOOST_CHECK_EQUAL(2u, js.size());
    BOOST_CHECK_EQUAL(true, js.test(0));
    BOOST_CHECK_EQUAL(true, js.test(T(100500)));

    BOOST_REQUIRE_NO_THROW(js.clear());
    BOOST_CHECK_EQUAL(true, js.empty());
    BOOST_CHECK_EQUAL(0u, js.size());
    BOOST_CHECK_EQUAL(false, js.test(0));
    BOOST_CHECK_EQUAL(false, js.test(T(100500)));

    BOOST_CHECK_EQUAL(true, js.set(T(1)));
    BOOST_CHECK_EQUAL(true, js.set(T(2)));
    BOOST_CHECK_EQUAL(true, js.set(T(100)));
    BOOST_CHECK_EQUAL(true, js.set(T(-300)));
    BOOST_CHECK_EQUAL(true, js.set(T(240)));
    BOOST_CHECK_EQUAL(true, js.set(T(65536)));
    BOOST_CHECK_EQUAL(false, js.set(T(100)));
    BOOST_CHECK_EQUAL(false, js.set(T(2)));
    BOOST_CHECK_EQUAL(false, js.set(T(1)));
    BOOST_CHECK_EQUAL(false, js.set(T(-300)));
    BOOST_CHECK_EQUAL(true, js.set(T(16)));
    BOOST_CHECK_EQUAL(false, js.empty());
    BOOST_CHECK_EQUAL(7u, js.size());
    BOOST_CHECK_EQUAL(true, js.test(T(100)));
    BOOST_CHECK_EQUAL(true, js.erase(T(100)));
    BOOST_CHECK_EQUAL(false, js.test(T(100)));
    BOOST_CHECK_EQUAL(false, js.erase(T(100)));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_set_iters, T, set_types_t)
{
    judypp::Set<T> js;
    BOOST_CHECK(js.begin() == js.end());

    js.set(T(0));
    auto it = js.begin();
    BOOST_CHECK(it != js.end());
    BOOST_CHECK_EQUAL(*it, T(0));
    ++it;
    BOOST_CHECK(it == js.end());

    js.set(T(10));
    js.set(T(29));
    it = js.begin();
    BOOST_CHECK(it != js.end());
    BOOST_CHECK_EQUAL(*it, T(0));
    it++;
    BOOST_CHECK_EQUAL(*it, T(10));
    ++it;
    BOOST_CHECK_EQUAL(*it, T(29));
    it--;
    BOOST_CHECK_EQUAL(*it, T(10));
    it--;
    BOOST_CHECK_EQUAL(*it, T(0));
    it++; it++; it++;
    BOOST_CHECK(it == js.end());

    js.erase(js.begin());
    BOOST_CHECK_EQUAL(*js.begin(), T(10));
}

BOOST_AUTO_TEST_SUITE_END()
