/**
 * Tests functionality of the node engine.
 *
 * Copyright 2013 Bruce Ide
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include <cppunit/extensions/HelperMacros.h>
#include <boost/bind.hpp>
#include "node_engine.hpp"
#include <string>
#include <sstream>

class node_engine_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(node_engine_test);
  CPPUNIT_TEST(test_basic);
  CPPUNIT_TEST(test_less_basic);
  CPPUNIT_TEST_SUITE_END();

public:

  void test_basic()
  {
    std::stringstream test_xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<foo bar=\"baz\">bait</foo>");
    cppxml::node_engine *engine = nullptr;
    try {
      engine = new cppxml::node_engine(dynamic_cast<std::istream &>(test_xml));
    } catch (std::string err) {
      CPPUNIT_FAIL(err);
    }
    cppxml::node_pointer document = nullptr;
    try {
      document = engine->get_document();
    } catch (std::string err) {
      CPPUNIT_FAIL(err);
    }
    CPPUNIT_ASSERT(document->get_name() == "foo");
    CPPUNIT_ASSERT(document->get_text() == "bait");
    CPPUNIT_ASSERT(document->find_attribute("bar") != nullptr && (*document->find_attribute("bar")) == "baz");
    delete engine;
    // I'm assuming document gets reset when it goes out of scope, since
    // the engine is no longer holding any pointers to it.
  }

  // Read from a file, verify namespaces
  void test_less_basic()
  {
    std::string filename("xmbibble.xml");
    cppxml::node_engine *e = nullptr;
    try {
      e = new cppxml::node_engine(filename);
    } catch (std::string err) {
      CPPUNIT_FAIL(err);
    }
    cppxml::node_pointer document = nullptr;
    try {
      document = e->get_document();
    } catch (std::string err) {
      CPPUNIT_FAIL(err);
    }
    cppxml::node_pointer bar = document->find_child("bar");
    CPPUNIT_ASSERT(bar.get() != nullptr);
    // There are some extra crs in the text, since my XML is sloppy
    CPPUNIT_ASSERT(bar->get_text().find("Text from Bar") != std::string::npos);
    CPPUNIT_ASSERT(bar->get_name() == "bar");
    CPPUNIT_ASSERT(bar->get_namespace() == "n");
    CPPUNIT_ASSERT(bar->get_namespace_uri() == "http://fake_domain/test");
    
    cppxml::node_pointer baz = bar->find_child("baz");
    CPPUNIT_ASSERT(baz.get() != nullptr);
    CPPUNIT_ASSERT(baz->find_attribute("bait").get() != nullptr);
    CPPUNIT_ASSERT((*(baz->find_attribute("bait").get())) == "quux");
    delete e;
  }
  
};

CPPUNIT_TEST_SUITE_REGISTRATION(node_engine_test);
