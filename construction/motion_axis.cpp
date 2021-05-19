#include "motion_axis.hpp"

Motion_axis::Motion_axis(double ratio, Motion_axis::Direction home_direction) : ratio(ratio), home_direction(home_direction){ }

Motion_axis::Direction Motion_axis::Flip_direction(Motion_axis::Direction direction){
    if (direction == Motion_axis::Direction::Forward){
        return Motion_axis::Direction::Reverse;
    } else {
        return Motion_axis::Direction::Forward;
    }
}
