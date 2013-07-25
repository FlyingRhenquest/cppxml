/**
 * A kml timespan node. Takes a start and end time. If either isn't set,
 * that node isn't rendered.
 */

#ifndef _HPP_KML_TIMESPAN
#define _HPP_KML_TIMESPAN

#include "kml_base.hpp"
#include <memory>
#include <string>
#include "string_converts.hpp"
#include <sys/time.h>
#include "xml_node.hpp"

namespace cppxml {

  class kml_timespan : public kml_base {

    typedef kml_base super;
    const std::string Timespan = "TimeSpan";
    const std::string Begin = "begin";
    const std::string End = "end";

    double start_time;
    double end_time;

    cppxml::xml_node::pointer get_time_node(std::string node_name, double at_time)
    {
      timeval tv;
      tv.tv_sec = at_time;
      tv.tv_usec = (at_time - trunc(at_time)) * 1000000;
      xml_node::pointer retval = get_node(node_name, fr::time::to_string<timeval>()(tv));
      return retval;
    }

  public:
    typedef std::shared_ptr<kml_timespan> pointer;
   
    kml_timespan() : super(), start_time(0.0), end_time(0.0)
    {
    }

    kml_timespan(const kml_timespan &copy) : super(copy), start_time(copy.start_time), end_time(copy.end_time)
    {
    }

    ~kml_timespan()
    {
    }

    void start(double s)
    {
      start_time = s;
    }

    void end(double e)
    {
      end_time = e;
    }

    xml_node::pointer to_xml() override
    {
      xml_node::pointer retval = std::make_shared<xml_node>(Timespan, Kmlns, KmlnsUrl);
      if (start_time > 0.0) {
	retval->add_child(get_time_node(Begin, start_time));
      }
      if (end_time > 0.0) {
	retval->add_child(get_time_node(End, end_time));
      }
      return retval;
    }

  };

}

#endif
