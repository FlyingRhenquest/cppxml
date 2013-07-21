/**
 * This is a point in kml. It can accept any coordinate from fr::coordinate.
 * The coordinate will be converted to a lat_long prior to rendering to
 * xml.
 */

#ifndef _HPP_KML_POINT
#define _HPP_KML_POINT

#include "coordinates.hpp"
#include "kml_base.hpp"
#include "kml_coordinates.hpp"
#include <memory>
#include <string>
#include "xml_node.hpp"

namespace cppxml {

  class kml_point : public kml_base {
  public:
    typedef std::shared_ptr<kml_point> pointer;
 
    enum altitude_mode {clampToGround, relativeToGround, absolute};

    const std::string Absolute = "absolute";
    const std::string AltitudeMode= "altitudeMode";
    const std::string ClampToGround = "clampToGround";
    const std::string Extrude = "extrude";
    const std::string Id = "id";
    const std::string Point = "Point";
    const std::string RelativeToGround = "relativeToGround";

  private:

    std::string id;
    fr::coordinates::lat_long point;
    bool extrude;
    altitude_mode alt_mode;

    void add_altitude_mode(xml_node::pointer to_this)
    {
      switch(alt_mode) {
      case absolute:
	to_this->set_attribute(AltitudeMode, Absolute);
	break;
      case clampToGround:
	to_this->set_attribute(AltitudeMode, ClampToGround);
	break;
      case relativeToGround:
	to_this->set_attribute(AltitudeMode, RelativeToGround);
	break;
      }
    }

  public:
    kml_point(std::string id = "", bool extrude = false, altitude_mode alt_mode = clampToGround) : id(id), extrude(extrude), alt_mode(alt_mode)
    {
    }

    kml_point(const kml_point &copy) : id(copy.id), point(copy.point), extrude(copy.extrude), alt_mode(copy.alt_mode)
    {
    }

    // Templatize set_point so I can set any coordinate point as my
    // point.
    template <typename coordinate_type>
    void set_point(const coordinate_type &coord)
    {
      point = fr::coordinates::converter<fr::coordinates::lat_long>()(coord);
    }

    xml_node::pointer to_xml()
    {
      xml_node::pointer retval = std::make_shared<xml_node>(Point, Kmlns, KmlnsUrl);
      if (id.length() > 0) {
	retval->set_attribute(Id, id);
      }
      add_altitude_mode(retval);
      retval->set_attribute(Extrude, extrude ? std::string("1") : std::string("0"));
      kml_coordinates coords;
      coords.add(point);
      retval->add_child(coords.to_xml());
      return retval;
    }

  };

}

#endif
