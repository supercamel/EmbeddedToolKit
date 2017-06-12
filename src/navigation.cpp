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

/*
	Many of these functions have been borrowed from
	ref: http://www.movable-type.co.uk/scripts/latlong.html
*/

#include <etk/navigation.h>
#include <etk/conversions.h>

namespace etk
{

Coordinate::Coordinate(real_t la, real_t ln)
{
    lat = degrees_to_radians(la);
    lng = degrees_to_radians(ln);
}

Coordinate::Coordinate(const Coordinate& c)
{
	lat = c.lat;
	lng = c.lng;
}

Coordinate::Coordinate(etk::Vector<2> v)
{
    lat = degrees_to_radians(v.x());
    lng = degrees_to_radians(v.y());
}

Coordinate::Coordinate(etk::Vector<3> v)
{
    lat = degrees_to_radians(v.x());
    lng = degrees_to_radians(v.y());
}

real_t Coordinate::bearing_to(Coordinate to) const
{
    real_t dLon = to.lng - lng;
    real_t y = sin(dLon) * cos(to.lat);
    real_t x = cos(lat)*sin(to.lat) -
              sin(lat)*cos(to.lat)*cos(dLon);
    return radians_to_degrees(atan2(y, x));
}

real_t Coordinate::distance_to(Coordinate b) const
{
    return acos(sin(lat)*sin(b.lat) + cos(lat)*cos(b.lat)*cos(b.lng-lng)) * real_t(6371000.0);
}

real_t Coordinate::cross_track_distance(Coordinate from, Coordinate to) const
{
    real_t d13, brng13, brng12;
    d13 = from.distance_to(*this);
    brng13 = degrees_to_radians(from.bearing_to(*this));
    brng12 = degrees_to_radians(from.bearing_to(to));
    real_t R = 6371000.0; //radius of the world in meters
    return asin(sin(d13/R)*sin(brng13-brng12)) * R;
}

Coordinate Coordinate::destination_from_distance_bearing(real_t d, real_t bearing) const
{
    real_t R = 6371000.0; //radius of the world in meters
    real_t brng = degrees_to_radians(bearing);
    Coordinate dest;
    dest.lat = asin(sin(lat)*cos(d/R) +
                    cos(lat)*sin(d/R)*cos(brng));
    dest.lng = lng + atan2f(sin(brng)*sin(d/R)*cos(lat),
                                cos(d/R)-sin(lat)*sin(dest.lat));

	/*
	lat2: =ASIN(SIN(lat1)*COS(d/R) + COS(lat1)*SIN(d/R)*COS(brng))
lon2: =lon1 + ATAN2(COS(d/R)-SIN(lat1)*SIN(lat2), SIN(brng)*SIN(d/R)*COS(lat1))
	*/
    return dest;
}


Waypoint::Waypoint(real_t la, real_t ln)
{
    lat = degrees_to_radians(la);
    lng = degrees_to_radians(ln);
}

Waypoint::Waypoint(real_t la, real_t ln, real_t a)
{
    lat = degrees_to_radians(la);
    lng = degrees_to_radians(ln);
    alt = a;
}

Waypoint::Waypoint(etk::Vector<3> pos)
{
    lat = degrees_to_radians(pos.x());
    lng = degrees_to_radians(pos.y());
    alt = pos.z();
}


}
