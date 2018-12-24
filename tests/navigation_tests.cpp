#include "navigation_tests.h"
#include "etk/navigation.h"

#include <iostream>
using namespace std;

using namespace etk;

bool navigation_test(std::string& subtest)
{
    subtest = "Coordinate constructors";

    Coordinate coord(5.0f, 6.0f);
    if(compare(coord.get_lat(), 5.0f, 0.00001f) == false) {
        return false;
    }
    if(compare(coord.get_lng(), 6.0f, 0.00001f) == false) {
        return false;
    }

    subtest = "Coordinate from vector";
    Vector<2> v(3.6, -146.3);
    coord = Coordinate(v);
    if(compare(coord.get_lat(), 3.6f, 0.00001f) == false) {
        return false;
    }
    if(compare(coord.get_lng(), -146.3f, 0.00001f) == false) {
        return false;
    }

    subtest = "Coordinate from 3d vector";
    Vector<3> v3(3.6, -146.3, 90);
    coord = Coordinate(v3);
    if(compare(coord.get_lat(), 3.6f, 0.0001f) == false) {
        return false;
    }
    if(compare(coord.get_lng(), -146.3f, 0.00001f) == false) {
        return false;
    }

    subtest = "Coordinate to vector conversion";
    coord = Coordinate(35.63f, -147.63f);
    v = coord;
    if(compare(v.x(), 35.63f, 0.0001f) == false) {
        return false;
    }
    if(compare(v.y(), -147.63f, 0.000001f) == false) {
        return false;
    }

    subtest = "Bearing to";
    coord = Coordinate(35.0f, 53.0f);

    if(compare(coord.bearing_to(Coordinate(35, 54)), 89.7132f, 0.00001f) == false) {
        return false;
    }

    subtest = "Distance to";
    coord = Coordinate(22.755920f, 3.515625f);
    if(compare(coord.distance_to(Coordinate(21.779905, 7.382812f)), 412458.0f, 0.5f) == false) {
        return false;
    }

    subtest = "Distance to";
    coord = Coordinate(20.303318f, 23.026157f);
    if(compare(coord.distance_to(Coordinate(20.303307f, 23.026180f)), 2.66f, 0.1f) == false) {
        return false;
    }

    /*
    	subtest = "Cross track distance";
    	real_t xtk = cross_track_distance(Coordinate from, Coordinate to);
    */

    auto rpf = RelativePointFactory({-37.54, 147.58});
    auto p = rpf.make_coord(-100, 350);

    return true;
}
