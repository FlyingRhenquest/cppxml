/**
 * The document wrapper. You can add other kml objects to it. Since
 * the document is also a kml object, you can also add a kml document
 * to a kml document. Don't do that :-P
 *
 */

#ifndef _HPP_KML_DOCUMENT
#define _HPP_KML_DOCUMENT

#include "kml_base.hpp"
#include <memory>

namespace cppxml {

  class kml_document : public kml_base {
    typedef kml_base super;
    const std::string Document = "Document";
    
  public:

    typedef std::shared_ptr<kml_document> pointer;

    kml_document(std::string name = "", std::string description = "") : super(name, description)
    {
    }

    kml_document(const kml_document &copy) : super(copy)
    {
    }

    ~kml_document()
    {
    }

    xml_node::pointer to_xml()
    {
      xml_node::pointer retval = std::make_shared<xml_node>(Document, Kmlns, KmlnsUrl);
      add_name_and_description(retval);
      add_xml_children(retval);
      return retval;
    }

  };

}

#endif
