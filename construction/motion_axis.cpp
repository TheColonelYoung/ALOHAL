#include "motion_axis.hpp"

Motion_axis::Motion_axis(double ratio, double max_position, double min_position, Motion_axis::Direction home_direction) : ratio(ratio), max_position(max_position), min_position(min_position), home_direction(home_direction){ }

Motion_axis::Direction Motion_axis::Flip_direction(Motion_axis::Direction direction){
    if (direction == Motion_axis::Direction::Forward){
        return Motion_axis::Direction::Reverse;
    } else {
        return Motion_axis::Direction::Forward;
    }
}
