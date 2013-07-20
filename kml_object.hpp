/**

 */

#ifndef _HPP_KML_OBJECT
#define _HPP_KML_OBJECT

#include "kml_base.hpp"
#include <memory>
#include <string>
#include "xml_node.hpp"

namespace cppxml {

  class kml_object : public kml_base {
    typedef kml_base super;

  public:
    
    typedef std::shared_ptr<kml_object> pointer;

    // Doesn't make much sense to have a name or description at this level
    // (It's the only exception I can think of off the top of my head)
    kml_object() : super()
    {
    }

    kml_object(const kml_object &copy) : super(copy)
    {
    }

    virtual ~kml_object()
    {
    }

    xml_node::pointer to_xml() override
    {
      xml_node::pointer retval = std::make_shared<xml_node>(Kml, "", KmlnsUrl);
      // Top level document node also needs it added to the list
      retval->add_namespace("", KmlnsUrl); // Default namespace
      add_xml_children(retval);
      return retval;
    }

    const vector get_children() const
    {
      return children;
    }

  };

};

#endif
