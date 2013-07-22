/**
 * Placemark. Really it's just another container-thing. Maybe I should
 * refactor document, folder and more into kml-base...
 */

#ifndef _HPP_KML_PLACEMARK
#define _HPP_KML_PLACEMARK

#include "kml_base.hpp"
#include <memory>
#include <string>
#include "xml_node.hpp"

namespace cppxml {

  class kml_placemark : public kml_base {
  public:
    typedef std::shared_ptr<kml_placemark> pointer;
    
  private:
    typedef kml_base super;
    const std::string Placemark = "Placemark";

  public:
    kml_placemark(std::string name = "", std::string description = "") : super(name, description)
    {
    }

    kml_placemark(const kml_placemark &copy) : super(copy)
    {
    }

    xml_node::pointer to_xml() override
    {
      xml_node::pointer retval = std::make_shared<xml_node>(Placemark, Kmlns, KmlnsUrl);
      add_name_and_description(retval);
      add_xml_children(retval);
      return retval;
    }

  };

}

#endif
