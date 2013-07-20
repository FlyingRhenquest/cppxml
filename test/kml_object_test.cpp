/**
 * Makes sure kml_object works
 */

#include "cppunit"
#include "kml_object.hpp"
#include <memory>
#include <string>

class kml_object_test : public test_class
{
  CPPUNIT_TEST_SUITE(kml_object_test);
  CPPUNIT_TEST(basic_test);
  CPPUNIT_TEST_SUITE_END();

 public:
  void basic_test()
  {
    cppxml::kml_object::pointer test_obj = std::make_shared<cppxml::kml_object>();
    std::string retval = test_obj->to_xml()->to_string();
    std::string expected("<kml xmlns=\"http://www.opengis.net/kml/2.2\"/>\n");
    CPPUNIT_ASSERT(expected == retval);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(kml_object_test);
