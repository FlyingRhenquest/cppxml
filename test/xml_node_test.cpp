/**
 * Test for xml node
 */

#include <cppunit/extensions/HelperMacros.h>
#include "xml_node.hpp"

class xml_node_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(xml_node_test);
  CPPUNIT_TEST(test_basic_functionality);
  CPPUNIT_TEST_SUITE_END();

public:

  void test_basic_functionality()
  {
    cppxml::xml_node foo;
    CPPUNIT_ASSERT(foo.get_name() == "");
    CPPUNIT_ASSERT(foo.get_text() == "");
    CPPUNIT_ASSERT(foo.get_namespace() == "");
    cppxml::xml_node bar("bar", "n", "http://fake_domain/fake_ns");
    CPPUNIT_ASSERT(bar.get_name() == "bar");
    CPPUNIT_ASSERT(bar.get_namespace() == "n");
    CPPUNIT_ASSERT(bar.get_namespace_uri() == "http://fake_domain/fake_ns");
    foo = bar;
    CPPUNIT_ASSERT(foo.get_name() == "bar");
    CPPUNIT_ASSERT(foo.get_namespace() == "n");
    CPPUNIT_ASSERT(foo.get_namespace_uri() == "http://fake_domain/fake_ns");
    foo.set_attribute("baz", "bait");
    CPPUNIT_ASSERT(foo.find_attribute("baz").get() != nullptr && (*foo.find_attribute("baz")) == "bait");
    foo.set_name("foo");
    CPPUNIT_ASSERT(foo.get_name() == "foo");
    foo.add_child(std::make_shared<cppxml::xml_node>(bar));
    CPPUNIT_ASSERT(foo.find_child("bar").get() != nullptr && foo.find_child("bar")->get_name() == "bar");
    cppxml::child_vector v = foo.find_children("bar");
    CPPUNIT_ASSERT(v.size() == 1);
    CPPUNIT_ASSERT(v[0]->get_name() == "bar");
  }
  
};

CPPUNIT_TEST_SUITE_REGISTRATION(xml_node_test);
