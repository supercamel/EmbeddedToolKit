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
#include "vector.h"
#include "conversions.h"

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

    Coordinate(const Coordinate& c)
    {
        lat = c.lat;
        lng = c.lng;
    }

    Coordinate(const real_t la, const real_t ln)
    {
        lat = degrees_to_radians(la);
        lng = degrees_to_radians(ln);
    }
    /**
     * \brief Creates a Coordinate.
     * @arg v A two dimensional vector with lat as x and lng as y.
     */
    Coordinate(const etk::Vector<2>& v)
    {
        lat = degrees_to_radians(v.x());
        lng = degrees_to_radians(v.y());
    }
    /**
     * \brief Creates a Coordinate.
     * @arg v A three dimensional vector with lat as x and lng as y. The z dimension is ignored.
     */
    Coordinate(const etk::Vector<3>& v)
    {
        lat = degrees_to_radians(v.x());
        lng = degrees_to_radians(v.y());
    }

    /**
     * \brief Calculates the bearing to a coordinate.
     */
    real_t bearing_to(const Coordinate& to) const
    {
        real_t dLon = to.lng - lng;
        real_t y = sin(dLon) * cos(to.lat);
        real_t x = cos(lat)*sin(to.lat) -
                   sin(lat)*cos(to.lat)*cos(dLon);
        return radians_to_degrees(atan2(y, x));
    }

    /**
     * \brief Calculates the distance to a coordinate.
     * @return Distance to a coordinate in meters.
     */
    real_t distance_to(const Coordinate& b) const
    {
        return acos(sin(lat)*sin(b.lat) + cos(lat)*cos(b.lat)*cos(b.lng-lng)) * R;
    }

    /**
     * \brief Calculates cross track distance (how far off course you are).
     * \image html http://www.firetailuav.com/img/xtrack.png
     * @return Cross track distance in meters.
     */
    real_t cross_track_distance(const Coordinate& from, const Coordinate& to) const
    {
        real_t d13, brng13, brng12;
        d13 = from.distance_to(*this);
        brng13 = degrees_to_radians(from.bearing_to(*this));
        brng12 = degrees_to_radians(from.bearing_to(to));
        return asin(sin(d13/R)*sin(brng13-brng12)) * R;
    }


    /**
     * \brief Given a distance and a bearing, this function calculates the destination coordinate.
     * @arg dist Distance in meters.
     * @arg bearing Direction to destination.
     * @return Coordinate that represents the destination.
     */
    Coordinate destination_from_distance_bearing(const real_t dist, const real_t bearing) const
    {
        real_t brng = degrees_to_radians(bearing);
        Coordinate dest;
        dest.lat = asin(sin(lat)*cos(dist/R) +
                        cos(lat)*sin(dist/R)*cos(brng));
        dest.lng = lng + atan2f(sin(brng)*sin(dist/R)*cos(lat),
                                cos(dist/R)-sin(lat)*sin(dest.lat));

        /*
        lat2: =ASIN(SIN(lat1)*COS(d/R) + COS(lat1)*SIN(d/R)*COS(brng))
        lon2: =lon1 + ATAN2(COS(d/R)-SIN(lat1)*SIN(lat2), SIN(brng)*SIN(d/R)*COS(lat1))
        */
        return dest;
    }


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
    real_t get_lat() const {
        return radians_to_degrees(lat);
    }

    /**
     * \brief Sets latitude
     */
    void set_lat(const real_t l) {
        lat = degrees_to_radians(l);
    }

    /**
     * \brief Returns longitude
     */
    real_t get_lng() const {
        return radians_to_degrees(lng);
    }

    /**
     * \brief Sets longitude.
     */
    void set_lng(const real_t l) {
        lng = degrees_to_radians(l);
    }

    real_t get_lat_rad() const {
        return lat;
    }

    real_t get_lng_rad() const {
        return lng;
    }

    void set_lat_rad(const real_t l) {
        lat = l;
    }

    void set_lng_rad(const real_t l) {
        lng = l;
    }

protected:
    real_t lat = 0, lng = 0;
    static constexpr real_t R = 6371000.0;
};


/**
 * \class Waypoint
 * \brief A Waypoint IS_A coordinate, but it has an extra dimension to store altitude. It provides get and set methods for altitude, and it can be cast to a 3 dimensional etk::Vector.
 */

class Waypoint : public Coordinate
{
public:
    Waypoint() { }
    Waypoint(const real_t la, const real_t ln)
    {
        lat = degrees_to_radians(la);
        lng = degrees_to_radians(ln);
    }

    Waypoint(const real_t la, const real_t ln, const real_t a)
    {
        lat = degrees_to_radians(la);
        lng = degrees_to_radians(ln);
        alt = a;
    }

    Waypoint(const etk::Vector<3>& pos)
    {
        lat = degrees_to_radians(pos.x());
        lng = degrees_to_radians(pos.y());
        alt = pos.z();
    }

    Waypoint(const Coordinate& c)
    {
        lat = c.get_lat_rad();
        lng = c.get_lng_rad();
    }

    operator Vector<3>()
    {
        Vector<3> ret;
        ret.x() = radians_to_degrees(lat);
        ret.y() = radians_to_degrees(lng);
        ret.z() = alt;
        return ret;
    }

    real_t get_alt() const
    {
        return alt;
    }
    void set_alt(const real_t a)
    {
        alt = a;
    }

    Waypoint& operator=(const Coordinate& c)
    {
        lat = c.get_lat_rad();
        lng = c.get_lng_rad();
        return *this;
    }

protected:
    real_t alt = 0;
};


/**
 * \class RelativePointFactory
 * \brief Relative coordinates are points that are defined by an x and y distance from the origin.
 * Units are meters.
 */

class RelativePointFactory
{
public:
    RelativePointFactory(Coordinate origin) : origin(origin)
    {
    }

    Coordinate make_coord(real_t x, real_t y) const
    {
        Vector<2> v(y, x);
        real_t dist = v.magnitude();
        real_t bearing = radians_to_degrees(v.theta());

        return origin.destination_from_distance_bearing(dist, bearing);
    }

    Waypoint make_waypoint(real_t x, real_t y, real_t alt) const
    {
        Waypoint wp = make_coord(y, x);
        wp.set_alt(alt);
        return wp;
    }

private:
    Coordinate origin;
};


}


#endif
