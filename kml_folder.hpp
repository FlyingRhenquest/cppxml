/**
 * Folder's just a container for other KML thingies.
 */

#ifndef _HPP_KML_FOLDER
#define _HPP_KML_FOLDER

#include "kml_base.hpp"
#include <memory>
#include <string>
#include "xml_node.hpp"

namespace cppxml {

  class kml_folder : public kml_base {
  public:
    typedef std::shared_ptr<kml_folder> pointer;

  private:
    typedef kml_base super;
    const std::string Folder = "Folder";
    
  public:
    kml_folder(std::string name = "", std::string description = "") : super(name, description)
    {
    }

    kml_folder(const kml_folder &copy) : super(copy)
    {
    }

    ~kml_folder()
    {
    }

    xml_node::pointer to_xml() override
    {
      xml_node::pointer retval = std::make_shared<xml_node>(Folder, Kmlns, KmlnsUrl);
      add_name_and_description(retval);
      add_xml_children(retval);
      return retval;
    }
  };

}

#endif
