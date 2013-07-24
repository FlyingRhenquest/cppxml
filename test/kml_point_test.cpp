/**
 * Verify kml_point works
 */

#include "coordinates.hpp"
#include "cppunit"
#include "kml_point.hpp"
#include <memory>
#include <string>
#include "xml_node.hpp"

class kml_point_test : public test_class {
  CPPUNIT_TEST_SUITE(kml_point_test);
  CPPUNIT_TEST(basic_point_test);
  CPPUNIT_TEST_SUITE_END();

public:

  void basic_point_test()
  {
    cppxml::kml_point::pointer point = std::make_shared<cppxml::kml_point>("", true);
    fr::coordinates::lat_long ll(1,2,3);
    point->set_point(ll);
    std::string point_string = point->to_xml()->to_string();
    std::string expected("<Point>\n   <altitudeMode>clampToGround</altitudeMode>\n   <extrude>1</extrude>\n   <coordinates>\n2,1,3\n</coordinates>\n</Point>\n");
     CPPUNIT_ASSERT(point_string == expected);
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(kml_point_test);
