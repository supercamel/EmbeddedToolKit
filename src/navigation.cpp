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

/*
	Many of these functions have been borrowed from
	ref: http://www.movable-type.co.uk/scripts/latlong.html
*/

#include <etk/navigation.h>
#include <etk/conversions.h>

namespace etk
{

Coordinate::Coordinate(float la, float ln)
{
    lat = la*DEG_TO_RAD;
    lng = ln*DEG_TO_RAD;
}

Coordinate::Coordinate(etk::Vector<2> v)
{
    lat = v.x()*DEG_TO_RAD;
    lng = v.y()*DEG_TO_RAD;
}

Coordinate::Coordinate(etk::Vector<3> v)
{
    lat = v.x()*DEG_TO_RAD;
    lng = v.y()*DEG_TO_RAD;
}

float Coordinate::bearing_to(Coordinate to)
{
    float dLon = to.lng - lng;
    float y = sin(dLon) * cos(to.lat);
    float x = cos(lat)*sin(to.lat) -
              sin(lat)*cos(to.lat)*cos(dLon);
    return RAD_TO_DEG*(atan2(y, x));
}

float Coordinate::distance_to(Coordinate b)
{
    return acos(sin(lat)*sin(b.lat) + cos(lat)*cos(b.lat)*cos(b.lng-lng)) * 6371000.0f;
}

float Coordinate::cross_track_distance(Coordinate from, Coordinate to)
{
    float d13, brng13, brng12;
    d13 = from.distance_to(*this);
    brng13 = DEG_TO_RAD*(from.bearing_to(*this));
    brng12 = DEG_TO_RAD*(from.bearing_to(to));
    int R = 6371000.0f; //radius of the world in meters
    return asin(sin(d13/R)*sin(brng13-brng12)) * R;
}

Coordinate Coordinate::destination_from_distance_bearing(float d, float bearing)
{
    float R = 6371000.0f; //radius of the world in meters
    float brng = bearing * DEG_TO_RAD;
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


Waypoint::Waypoint(float la, float ln)
{
    lat = la*DEG_TO_RAD;
    lng = ln*DEG_TO_RAD;
}

Waypoint::Waypoint(float la, float ln, float a)
{
    lat = la*DEG_TO_RAD;
    lng = ln*DEG_TO_RAD;
    alt = a;
}

Waypoint::Waypoint(etk::Vector<3> pos)
{
    lat = pos.x()*DEG_TO_RAD;
    lng = pos.y()*DEG_TO_RAD;
    alt = pos.z();
}


}


