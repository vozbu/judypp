#include <judypp/set.hpp>
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(judypp)

typedef boost::mpl::list<int, long, unsigned int, unsigned long> set_types_t;
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

    BOOST_CHECK_EQUAL(true, js.set(100500));
    BOOST_CHECK_EQUAL(false, js.empty());
    BOOST_CHECK_EQUAL(2u, js.size());
    BOOST_CHECK_EQUAL(true, js.test(0));
    BOOST_CHECK_EQUAL(true, js.test(100500));

    BOOST_REQUIRE_NO_THROW(js.clear());
    BOOST_CHECK_EQUAL(true, js.empty());
    BOOST_CHECK_EQUAL(0u, js.size());
    BOOST_CHECK_EQUAL(false, js.test(0));
    BOOST_CHECK_EQUAL(false, js.test(100500));

    BOOST_CHECK_EQUAL(true, js.set(1));
    BOOST_CHECK_EQUAL(true, js.set(2));
    BOOST_CHECK_EQUAL(true, js.set(100));
    BOOST_CHECK_EQUAL(true, js.set(-300));
    BOOST_CHECK_EQUAL(true, js.set(240));
    BOOST_CHECK_EQUAL(true, js.set(65536));
    BOOST_CHECK_EQUAL(false, js.set(100));
    BOOST_CHECK_EQUAL(false, js.set(2));
    BOOST_CHECK_EQUAL(false, js.set(1));
    BOOST_CHECK_EQUAL(false, js.set(-300));
    BOOST_CHECK_EQUAL(true, js.set(16));
    BOOST_CHECK_EQUAL(false, js.empty());
    BOOST_CHECK_EQUAL(7u, js.size());
    BOOST_CHECK_EQUAL(true, js.test(100));
    BOOST_CHECK_EQUAL(true, js.erase(100));
    BOOST_CHECK_EQUAL(false, js.test(100));
    BOOST_CHECK_EQUAL(false, js.erase(100));
}

BOOST_AUTO_TEST_SUITE_END()
