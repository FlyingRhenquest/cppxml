/**
 * Verify basic engine works
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
 */

#include <cppunit/extensions/HelperMacros.h>
#include <boost/bind.hpp>
#include "engine.hpp"
#include <string>
#include <sstream>

class engine_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(engine_test);
  CPPUNIT_TEST(test_start_basic_engine);
  CPPUNIT_TEST(test_namespace_handling);
  CPPUNIT_TEST_SUITE_END();

  // Helper function for thingies. Use boost::bind to bind the expected tag and
  // pointer to a boolean when you connect to the start and end tag functions
  // and it will say yea or nay. Make sure your boolean starts out as false,
  // because if your XML has more than one tag in it, I just want to verify
  // you saw your tag in there somewhere.

  static void got_expected_tag(const std::string &expected_tag, const std::string &actual_tag, bool *match)
  {
    if (expected_tag == actual_tag) {
      *match = true;
    }
  }

  static void got_expected_ns(const std::string &expected_ns, const std::string &expected_uri, const std::string &actual_ns, const std::string &actual_uri, bool *match)
  {
    if (expected_ns == actual_ns && expected_uri == actual_uri) {
      *match = true;
    }
  }

  static void got_expected_attribute(const std::string &expected_name, const std::string &expected_value, const cppxml::attribute_vector &attributes, bool *found)
  {
    for (cppxml::attribute_pair current : attributes) {
      if (current.first == expected_name && current.second == expected_value) {
	*found = true;
	break;
      }
    }
  }

public:

  // Make sure we get a start and end signal
  void test_start_basic_engine()
  {
    bool got_start_tag = false;
    bool got_end_tag = false;
    bool got_flibble = false; // Just to prove nothing up my sleeve...
    // We can leverage got_expected_tag for text, too
    bool got_bait = false;
    bool got_baz = false;

    try {
      std::stringstream test_xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<foo bar=\"baz\">bait</foo>");
      cppxml::engine xml_engine(dynamic_cast<std::istream &>(test_xml));
      xml_engine.start.connect(boost::bind(got_expected_tag, std::string("foo"), _1, &got_start_tag));
      xml_engine.start.connect(boost::bind(got_expected_tag, std::string("flibble"), _1, &got_flibble));
      xml_engine.end.connect(boost::bind(got_expected_tag, std::string("foo"), _1, &got_end_tag));
      xml_engine.text.connect(boost::bind(got_expected_tag, std::string("bait"), _1, &got_bait));
      xml_engine.start.connect(boost::bind(got_expected_attribute, std::string("bar"), std::string("baz"), _2, &got_baz));
      xml_engine.parse();
      CPPUNIT_ASSERT(got_start_tag);
      CPPUNIT_ASSERT(got_end_tag);
      CPPUNIT_ASSERT(got_bait);
      CPPUNIT_ASSERT(!got_flibble);
    } catch (std::string err) {
      CPPUNIT_FAIL(err);
    }
  }

  // Verify namespace stuff works

  void test_namespace_handling()
  {
    bool start_signal_found = false;
    bool end_signal_found = false;
    bool tag_found;
    std::stringstream test_xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<foo xmlns:n=\"http://fakeurl.com/test_ns\"><n:bar>bait</n:bar></foo>");
    cppxml::engine *xml_engine = nullptr;
    try {
      xml_engine = new cppxml::engine(dynamic_cast<std::istream &>(test_xml));
    } catch (std::string err) {
      CPPUNIT_FAIL(err);
    }
    CPPUNIT_ASSERT(xml_engine != nullptr);
    xml_engine->namespace_start.connect(boost::bind(got_expected_ns, std::string("n"), std::string("http://fakeurl.com/test_ns"), _1, _2, &start_signal_found));
    // Got_expected_tag will work for this too :-D
    xml_engine->namespace_end.connect(boost::bind(got_expected_tag, std::string("n"), _1, &end_signal_found));
    // Just to demonstrate how this affects your tags...
    xml_engine->start.connect(boost::bind(got_expected_tag, std::string("http://fakeurl.com/test_ns:bar"), _1, &tag_found));
    xml_engine->parse();
    CPPUNIT_ASSERT(start_signal_found);
    CPPUNIT_ASSERT(end_signal_found);
    CPPUNIT_ASSERT(tag_found);
    delete(xml_engine);
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(engine_test);
