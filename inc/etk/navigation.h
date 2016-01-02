/*
    Embedded Tool Kit
    Copyright (C) 2015 Samuel Cowen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ETK_NAVIGATION_H_INCLUDED
#define ETK_NAVIGATION_H_INCLUDED


#include <stdint.h>
#include <etk/vector.h>
#include <etk/conversions.h>

namespace etk
{

/**
 * \class Coordinate
 * \brief Represents latitude and longitude coordinates. Units are in degrees unless stated otherwise.
 * All navigation functions use great circle mathematics and are, in theory, suitable for long distances and navigation in polar regions.
 */
class Coordinate
{
public:
    Coordinate() { }
    /**
     * \brief Creates a Coordinate.
     * @arg la latitude
     * @arg ln longitude
     */
    Coordinate(float la, float ln);
    /**
     * \brief Creates a Coordinate.
     * @arg v A two dimensional vector with lat as x and lng as y.
     */
    Coordinate(etk::Vector<2> v);
    /**
     * \brief Creates a Coordinate.
     * @arg v A three dimensional vector with lat as x and lng as y. The z dimension is ignored.
     */
    Coordinate(etk::Vector<3> v);

	/**
	 * \brief Calculates the bearing to a coordinate.
	 */
    float bearing_to(Coordinate to);

    /**
     * \brief Calculates the distance to a coordinate.
     * @return Distance to a coordinate in meters.
     */
    float distance_to(Coordinate b);

    /**
     * \brief Calculates cross track distance (how far off course you are).
     * \image html http://www.firetailuav.com/img/xtrack.png
     * @return Cross track distance in meters.
     */
    float cross_track_distance(Coordinate from, Coordinate to);

    /**
     * \brief Given a distance and a bearing, this function calculates the destination coordinate.
     * @arg dist Distance in meters.
     * @arg bearing Direction to destination.
     * @return Coordinate that represents the destination.
     */
    Coordinate destination_from_distance_bearing(float dist, float bearing);

	/**
     * \brief Coordinate can be seamlessly cast to a two dimensional vector.
     *@code
     etk::Vector<2> vec;
     etk::Coordinate coord(5, 10);
     vec = coord;
     @endcode
     * This makes it easy to perform more advanced vector mathematics on a coordinate.
     */
    operator Vector<2>()
    {
        Vector<2> ret;
        ret.x() = radians_to_degrees(lat);
        ret.y() = radians_to_degrees(lng);
        return ret;
    }

	/**
	 * \brief Returns latitude.
	 */
    float get_lat() {
        return radians_to_degrees(lat);
    }

    /**
     * \brief Sets latitude
     */
    void set_lat(float l) {
        lat = degrees_to_radians(l);
    }

    /**
     * \brief Returns longitude
     */
    float get_lng() {
        return radians_to_degrees(lng);
    }

    /**
     * \brief Sets longitude.
     */
    void set_lng(float l) {
        lng = degrees_to_radians(l);
    }

protected:
    float lat = 0, lng = 0;
};


/**
 * \class Waypoint
 * \brief A Waypoint IS_A coordinate, but it has an extra dimension to store altitude. It provides get and set methods for altitude, and it can be cast to a 3 dimensional etk::Vector.
 */

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
        ret.x() = radians_to_degrees(lat);
        ret.y() = radians_to_degrees(lng);
        ret.z() = alt;
        return ret;
    }

    float get_alt() {
        return alt;
    }
    void set_alt(float a) {
        alt = a;
    }
protected:
    float alt = 0;
};

}


#endif


