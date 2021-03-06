/**
 * Test for xml node
 */

#include <cppunit/extensions/HelperMacros.h>
#include "xml_node.hpp"

class xml_node_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(xml_node_test);
  CPPUNIT_TEST(test_basic_functionality);
  CPPUNIT_TEST(test_to_string);
  CPPUNIT_TEST(test_set_text);
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

  void test_to_string()
  {
    cppxml::xml_node foo("foo", "n", "http://fake_domain/fake_ns");
    foo.add_namespace("n", "http://fake_domain/fake_ns");
    cppxml::xml_node bar("bar", "n", "http://fake_domain/fake_ns");
    cppxml::xml_node baz("baz", "n", "http://fake_domain/fake_ns");
    bar.add_child(std::make_shared<cppxml::xml_node>(baz));
    foo.add_child(std::make_shared<cppxml::xml_node>(bar));

    std::string tos = foo.to_string();
    std::string expected("<n:foo xmlns:n=\"http://fake_domain/fake_ns\">\n   <n:bar>\n      <n:baz/>\n   </n:bar>\n</n:foo>\n");
    CPPUNIT_ASSERT(tos == expected);
  }

  void test_set_text()
  {
    cppxml::xml_node foo("foo", "", "");
    foo.set_text("bar");
    std::string expected("<foo>bar</foo>\n");
    CPPUNIT_ASSERT(expected == foo.to_string());
  }
  
};

CPPUNIT_TEST_SUITE_REGISTRATION(xml_node_test);
