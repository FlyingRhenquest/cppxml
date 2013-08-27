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
    const std::string Visibility = "visibility";
    const std::string Open = "open";
    bool open;
    bool visibility;

    std::string bool_to(bool val)
    {
      return (val ? std::string("1") : std::string("0"));
    }

  public:
    kml_folder(std::string name = "", std::string description = "", bool open=false, bool visibility = true) : super(name, description), open(open), visibility(visibility)
    {
    }

    kml_folder(const kml_folder &copy) : super(copy), open(copy.open), visibility(copy.visibility)
    {
    }

    ~kml_folder()
    {
    }

    // Open or close the folder (Will need to reload the KML if it's already
    // loaded)
    void set_open(bool open)
    {
      this->open = open;
    }

    // Causes google earth to render or not render the element when it's loaded
    void set_visibility(bool visible)
    {
      visibility = visible;
    }

    xml_node::pointer to_xml() override
    {
      xml_node::pointer retval = std::make_shared<xml_node>(Folder, Kmlns, KmlnsUrl);
      
      xml_node::pointer visibility_node = std::make_shared<xml_node>(Visibility, "", "");
      visibility_node->set_text(bool_to(visibility));
      retval->add_child(visibility_node);
      xml_node::pointer open_node = std::make_shared<xml_node>(Open, "", "");
      open_node->set_text(bool_to(this->open));
      retval->add_child(open_node);
      add_name_and_description(retval);
      add_xml_children(retval);
      return retval;
    }

    
  };

}

#endif
