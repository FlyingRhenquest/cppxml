/**
 * An object all KML objects can inherit from. A lot of KML objects
 * share the same tags and things, so you can store those here.
 */

#ifndef _HPP_KML_BASE
#define _HPP_KML_BASE

#include <memory>
#include <string>
#include <vector>
#include "xml_node.hpp"

namespace cppxml {

  enum altitude_mode {clampToGround, relativeToGround, absolute};

  class kml_base {
  public:
    typedef std::shared_ptr<kml_base> pointer;
    typedef std::vector<pointer> vector;
  protected:
    std::string name;
    std::string description;
    vector children;

    // This is a good place for common tag names too
    const std::string Name = "name";
    const std::string Description = "description";
    const std::string Kml = "kml";
    const std::string Kmlns = ""; // default namespace
    const std::string KmlnsUrl = "http://www.opengis.net/kml/2.2";

    xml_node::pointer get_node(const std::string &node_name, const std::string &node_text)
    {
      xml_node::pointer retval = std::make_shared<xml_node>(node_name, Kmlns, KmlnsUrl);
      retval->set_text(node_text);
      return retval;
    }

    void add_name_and_description(xml_node::pointer to_this)
    {
      if (name.length() > 0) {
	to_this->add_child(get_node(Name, name));
      }
      if (description.length() > 0) {
	to_this->add_child(get_node(Description, description));
      }
    }

    void add_xml_children(xml_node::pointer to_this)
    {
      for (pointer child : children) {
	to_this->add_child(child->to_xml());
      }
    }
    
  public:

    
    kml_base(std::string name = "", std::string description = "") : name(name), description(description)
    {
    }

    kml_base(const kml_base &copy) : name(copy.name), description(copy.description), children(copy.children)
    {
    }

    virtual ~kml_base()
    {
    }

    // Add, well, anything that implements to_xml really. Don't forget to
    // std::dynamic_pointer_cast them!

    void add_child(pointer child)
    {
      children.push_back(child);
    }

    virtual xml_node::pointer to_xml() = 0;
  };

}

#endif
