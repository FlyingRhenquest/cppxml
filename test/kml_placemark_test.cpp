/**
 * Veryify kml_placemark works
 */

#include "cppunit"
#include "kml_object.hpp"
#include "kml_placemark.hpp"
#include <memory>
#include <string>

class kml_placemark_test : public test_class
{
  CPPUNIT_TEST_SUITE(kml_placemark_test);
  CPPUNIT_TEST(basic_test);
  CPPUNIT_TEST_SUITE_END();

public:

  void basic_test()
  {
    cppxml::kml_placemark::pointer placemark = std::make_shared<cppxml::kml_placemark>("Foo", "Bar");
    std::string placemark_str = placemark->to_xml()->to_string();
    std::string expected("<Placemark>\n   <name>Foo</name>\n   <description>Bar</description>\n</Placemark>\n");
    CPPUNIT_ASSERT(expected == placemark_str);

  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(kml_placemark_test);
