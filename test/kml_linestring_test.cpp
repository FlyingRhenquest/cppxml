/**
 * Verify kml_linestring works
 */

#include "coordinates.hpp"
#include "cppunit"
#include "kml_linestring.hpp"
#include <memory>
#include <string>
#include "xml_node.hpp"

class kml_linestring_test : public test_class {

  CPPUNIT_TEST_SUITE(kml_linestring_test);
  CPPUNIT_TEST(basic_linestring_test);
  CPPUNIT_TEST_SUITE_END();
  
  public:

  void basic_linestring_test()
  {
    cppxml::kml_linestring::pointer ls = std::make_shared<cppxml::kml_linestring>();
    fr::coordinates::lat_long point1(2,1,4);
    fr::coordinates::lat_long point2(1,2,3);
    ls->add(point1);
    ls->add(point2);
    std::string actual = ls->to_xml()->to_string();
    std::string expected("<LineString>\n   <altitudeMode>relativeToGround</altitudeMode>\n   <Tessellate>0</Tessellate>\n   <extrude>1</extrude>\n   <coordinates>\n1,2,4\n2,1,3\n</coordinates>\n</LineString>\n");
    CPPUNIT_ASSERT(actual == expected);
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(kml_linestring_test);
