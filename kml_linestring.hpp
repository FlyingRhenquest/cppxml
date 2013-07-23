/**
 * A linestring. This is a collection of points that make a line. You
 * can use extrude if you want the line to grow up from the ground, and
 * tessellate if you want it to be a dashed line instead of a solid line.
 */

#include "coordinates.hpp"
#include "kml_base.hpp"
#include "kml_coordinates.hpp"
#include <memory>
#include <string>
#include "xml_node.hpp"

#ifndef _HPP_KML_LINESTRING
#define _HPP_KML_LINESTRING

namespace cppxml {

  class kml_linestring : public kml_base {

  public:
    typedef std::shared_ptr<kml_linestring> pointer;
  private:
    typedef kml_base super;

    const std::string Linestring = "LineString";
    const std::string Tessellate = "Tessellate";
    bool tessellate;
    bool extrude;
    altitude_mode alt_mode;
    kml_coordinates::pointer coordinates;

  public:
    

    kml_linestring(altitude_mode alt_mode = relativeToGround, bool extrude = true, bool tessellate = false) : super(), tessellate(tessellate), extrude(extrude), alt_mode(alt_mode), coordinates(std::make_shared<kml_coordinates>())
    {
    }

    kml_linestring(const kml_linestring &copy) : super(copy), tessellate(copy.tessellate), extrude(copy.extrude), alt_mode(copy.alt_mode), coordinates(copy.coordinates)
    {
    }

    ~kml_linestring()
    {
    }

    void clear()
    {
      coordinates->clear();
    }

    template <typename coordinate_type>
    void add(coordinate_type c)
    {
      coordinates->add(c);
    }

    xml_node::pointer to_xml()
    {
      xml_node::pointer retval = get_node(Linestring, "");
      add_altitude_mode(retval, alt_mode);
      std::string tessellate_str = tessellate ? std::string("1") : std::string("0");
      xml_node::pointer tessellate_node = get_node(Tessellate, tessellate_str);
      retval->add_child(tessellate_node);
      std::string extrude_str = extrude ? std::string("1") : std::string("0");
      xml_node::pointer extrude_node = get_node(Extrude, extrude_str);
      retval->add_child(extrude_node);
      retval->add_child(coordinates->to_xml());
      return retval;
    }

  };

}

#endif
