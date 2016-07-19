/*
    Embedded Tool Kit
    Copyright (C) 2015 Samuel Cowen

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
*/

#ifndef ETK_NAVIGATION_H_INCLUDED
#define ETK_NAVIGATION_H_INCLUDED


#include "types.h"
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

    Coordinate(const Coordinate& c);


    Coordinate(real_t la, real_t ln);
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
    real_t bearing_to(Coordinate to);

    /**
     * \brief Calculates the distance to a coordinate.
     * @return Distance to a coordinate in meters.
     */
    real_t distance_to(Coordinate b);

    /**
     * \brief Calculates cross track distance (how far off course you are).
     * \image html http://www.firetailuav.com/img/xtrack.png
     * @return Cross track distance in meters.
     */
    real_t cross_track_distance(Coordinate from, Coordinate to);

    /**
     * \brief Given a distance and a bearing, this function calculates the destination coordinate.
     * @arg dist Distance in meters.
     * @arg bearing Direction to destination.
     * @return Coordinate that represents the destination.
     */
    Coordinate destination_from_distance_bearing(real_t dist, real_t bearing);

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
    real_t get_lat() {
        return radians_to_degrees(lat);
    }

    /**
     * \brief Sets latitude
     */
    void set_lat(real_t l) {
        lat = degrees_to_radians(l);
    }

    /**
     * \brief Returns longitude
     */
    real_t get_lng() {
        return radians_to_degrees(lng);
    }

    /**
     * \brief Sets longitude.
     */
    void set_lng(real_t l) {
        lng = degrees_to_radians(l);
    }

protected:
    real_t lat = 0, lng = 0;
};


/**
 * \class Waypoint
 * \brief A Waypoint IS_A coordinate, but it has an extra dimension to store altitude. It provides get and set methods for altitude, and it can be cast to a 3 dimensional etk::Vector.
 */

class Waypoint : public Coordinate
{
public:
    Waypoint() { }
    Waypoint(real_t la, real_t ln);
    Waypoint(real_t la, real_t ln, real_t a);
    Waypoint(etk::Vector<3> pos);

    operator Vector<3>()
    {
        Vector<3> ret;
        ret.x() = radians_to_degrees(lat);
        ret.y() = radians_to_degrees(lng);
        ret.z() = alt;
        return ret;
    }

    real_t get_alt() {
        return alt;
    }
    void set_alt(real_t a) {
        alt = a;
    }
protected:
    real_t alt = 0;
};

}


#endif
