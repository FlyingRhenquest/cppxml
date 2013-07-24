/**
 * Make sure kml_coordinates work
 */

#include "coordinates.hpp"
#include "cppunit"
#include "kml_coordinates.hpp"
#include <memory>
#include <string>
#include "xml_node.hpp"

class kml_coordinates_test : public test_class {
  CPPUNIT_TEST_SUITE(kml_coordinates_test);
  CPPUNIT_TEST(basic_test);
  CPPUNIT_TEST_SUITE_END();

public:

  void basic_test()
  {
    fr::coordinates::lat_long ll(1,2,3);
    cppxml::kml_coordinates::pointer testval = std::make_shared<cppxml::kml_coordinates>(0);
    testval->add(ll);
    std::string returned = testval->to_xml()->to_string();
    std::string expected("<coordinates>\n2,1,3\n</coordinates>\n");
    CPPUNIT_ASSERT(expected == returned);
    // ECEF coordinates for Denver
    fr::coordinates::ecef ef(-1260484.206487,4747249.668167,4057711.884932);
    testval->add(ef);
    std::string expected_2("<coordinates>\n2,1,3\n104.87,39.75,1609.34\n</coordinates>\n");
    returned = testval->to_xml()->to_string();
    CPPUNIT_ASSERT(expected_2 == returned);
    testval->clear();
    returned = testval->to_xml()->to_string();
    CPPUNIT_ASSERT("<coordinates/>\n" == returned);
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(kml_coordinates_test);
