#include "min_circle.h"
#include <iostream>

int main() {
    LineSegment seg1 = {{1, 2}, {3, 4}};
    LineSegment seg2 = {{4, 3}, {2, 1}};
    std::vector<LineSegment> segs = {seg1, seg2};
    Circle result = MinimumEnclosingCircleForSegments(segs);

    std::cout << "Центр: (" << result.center.x << ", " << result.center.y << ")" << std::endl;
    std::cout << "Радиус: " << result.radius << std::endl;

    return 0;
}
