/**
 * Verify I can construct a document, add it to a kml object and output
 * the correct XML.
 */

#include "cppunit"
#include "kml_document.hpp"
#include "kml_object.hpp"
#include <memory>
#include <string>

class kml_document_test : public test_class
{
  CPPUNIT_TEST_SUITE(kml_document_test);
  CPPUNIT_TEST(basic_test);
  CPPUNIT_TEST(copy_test);
  CPPUNIT_TEST_SUITE_END();

public:
  void basic_test()
  {
    cppxml::kml_object::pointer obj = std::make_shared<cppxml::kml_object>();
    cppxml::kml_document::pointer doc = std::make_shared<cppxml::kml_document>("Test Document", "A random test document");
    obj->add_child(doc);
    std::string expected("<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n   <Document>\n      <name>Test Document</name>\n      <description>A random test document</description>\n   </Document>\n</kml>\n");
    CPPUNIT_ASSERT(expected == obj->to_xml()->to_string());
  }
  
  void copy_test()
  {
    cppxml::kml_object::pointer obj = std::make_shared<cppxml::kml_object>();
    cppxml::kml_document::pointer doc = std::make_shared<cppxml::kml_document>("Test Document", "A test document for the copy test");
    obj->add_child(doc);
    cppxml::kml_object copy(*(obj.get()));
    CPPUNIT_ASSERT(copy.get_children().size() == 1);
    // TODO: check copy xml output
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(kml_document_test);
