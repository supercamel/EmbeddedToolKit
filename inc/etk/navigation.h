#ifndef ETK_NAVIGATION_H_INCLUDED
#define ETK_NAVIGATION_H_INCLUDED


#include <stdint.h>
#include <etk/vector.h>
#include <etk/conversions.h>

namespace etk
{

class Coordinate
{
public:
    Coordinate() { }
    Coordinate(float la, float ln);
    Coordinate(etk::Vector<2> v);
    Coordinate(etk::Vector<3> v);

    float bearing_to(Coordinate to);
    float distance_to(Coordinate b);
    float cross_track_distance(Coordinate from, Coordinate to);
    Coordinate destination_from_distance_bearing(float dist, float bearing);

    operator Vector<2>()
    {
        Vector<2> ret;
        ret.x() = lat*RAD_TO_DEG;
        ret.y() = lng*RAD_TO_DEG;
        return ret;
    }

    float get_latitude() {
        return lat*RAD_TO_DEG;
    }
    void set_latitude(float l) {
        lat = l*DEG_TO_RAD;
    }
    float get_longitude() {
        return lng*RAD_TO_DEG;
    }
    void set_longitude(float l) {
        lng = l*DEG_TO_RAD;
    }

protected:
    float lat = 0, lng = 0;
};


class Waypoint : public Coordinate
{
public:
    Waypoint() { }
    Waypoint(float la, float ln);
    Waypoint(float la, float ln, float a);
    Waypoint(etk::Vector<3> pos);

    operator Vector<3>()
    {
        Vector<3> ret;
        ret.x() = lat*RAD_TO_DEG;
        ret.y() = lng*RAD_TO_DEG;
        ret.z() = alt;
        return ret;
    }

    float get_altitude() {
        return alt;
    }
    void set_altitude(float a) {
        alt = a;
    }
protected:
    float alt = 0;
};

}


#endif


