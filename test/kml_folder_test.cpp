/**
 * Quick test to make sure we can get a folder
 */

#include "coordinates.hpp"
#include "cppunit"
#include "kml_folder.hpp"
#include "xml_node.hpp"
#include "kml_point.hpp"

class kml_folder_test : public test_class
{
  CPPUNIT_TEST_SUITE(kml_folder_test);
  CPPUNIT_TEST(basic_folder_test);
  CPPUNIT_TEST_SUITE_END();

public:
  void basic_folder_test()
  {
    cppxml::kml_folder::pointer test_folder = std::make_shared<cppxml::kml_folder>("Test Folder", "A basic test folder");
    cppxml::kml_point::pointer point = std::make_shared<cppxml::kml_point>("", true);
    fr::coordinates::lat_long ll(1,2,3);
    point->set_point(ll);
    test_folder->add_child(point);
    std::string expected("<Folder>\n   <name>Test Folder</name>\n   <description>A basic test folder</description>\n   <Point altitudeMode=\"clampToGround\" extrude=\"1\">\n      <coordinates>\n2,1,3\n</coordinates>\n   </Point>\n</Folder>\n");
    std::string actual = test_folder->to_xml()->to_string();

    CPPUNIT_ASSERT(expected == actual);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(kml_folder_test);
