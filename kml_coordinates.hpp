/**
 * A coordinates tag. A lot of things in KML use coordinates, so it's
 * easier to define the tag outside of any specific object. Typically
 * the user ought not to have to manipulate this object himself. It's
 * more for the other objects to use.
 */

#ifndef _HPP_KML_COORDINATES
#define _HPP_KML_COORDINATES

#include "coordinates.hpp"
#include "kml_base.hpp"
#include <memory>
#include <string>
#include "string_converts.hpp"
#include <vector>
#include "xml_node.hpp"

namespace cppxml {

  class kml_coordinates : public kml_base {
  public:
    typedef std::shared_ptr<kml_coordinates> pointer;
    typedef std::vector<fr::coordinates::lat_long> latlong_vector;
  private:
    const std::string Coordinates = "coordinates";
    latlong_vector all_latlongs;
    int precision;
  public:
    kml_coordinates(int precision = 9) : kml_base(), precision(precision)
    {
    }

    kml_coordinates(const kml_coordinates &copy) : all_latlongs(copy.all_latlongs), precision(copy.precision)
    {
    }

    ~kml_coordinates()
    {
    }

    // A little template fuckery to allow anything that can be converted
    // to a latlong to be used as a coordinate. Latlongs can be converted
    // to themselves, so this'll work for those and ECEFs with a default
    // ellipsoid. ECIs need a timestamp and should therefore be converted 
    // in advance.

    template <typename coordinate_type>

    void add(coordinate_type &coordinate)
    {
      fr::coordinates::lat_long incoming_coordinate = fr::coordinates::converter<fr::coordinates::lat_long>()(coordinate);
      all_latlongs.push_back(incoming_coordinate);
    }

    void clear()
    {
      all_latlongs.clear();
    }

    cppxml::xml_node::pointer to_xml()
    {
      cppxml::xml_node::pointer retval = std::make_shared<cppxml::xml_node>(Coordinates, Kmlns, KmlnsUrl);
      std::string coordinate_string("");
      if (all_latlongs.size() > 0) {
	for (const fr::coordinates::lat_long &point : all_latlongs) {
	  coordinate_string.append("\n");
	  // KML coordinates are in long/lat.
	  coordinate_string.append(fr::time::to_string<double>(precision)(point.get_long()));
	  coordinate_string.append(",");
	  coordinate_string.append(fr::time::to_string<double>(precision)(point.get_lat()));
	  coordinate_string.append(",");
	  coordinate_string.append(fr::time::to_string<double>(precision)(point.get_alt()));
	}
	coordinate_string.append("\n");
      }
      retval->set_text(coordinate_string);      

      return retval;
    }

  };

}

#endif
