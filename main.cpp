#include "min_circle.h"
#include <iostream>
#include <vector>

int main() {
    std::vector<LineSegment> empty;
    Circle c1 = MinimumEnclosingCircleForSegments(empty);
    std::cout << "Test 1 (Empty): " << (c1.isValid() ? "Valid" : "Invalid") 
              << " (Radius: " << c1.radius << ")\n";

    std::vector<LineSegment> single = { {{0, 0}, {4, 0}} };
    Circle c2 = MinimumEnclosingCircleForSegments(single);
    std::cout << "Test 2 (Single segment): Center=(" << c2.center.x << "," << c2.center.y 
              << "), Radius=" << c2.radius << "\n";

    std::vector<LineSegment> two = { 
        {{0, 0}, {4, 0}}, 
        {{2, 0}, {2, 3}} 
    };
    Circle c3 = MinimumEnclosingCircleForSegments(two);
    std::cout << "Test 3 (L-shape): Center=(" << c3.center.x << "," << c3.center.y 
              << "), Radius=" << c3.radius << "\n";

    auto points = extractPointsFromSegments(two);
    bool all_inside = true;
    for (const auto& p : points) {
        if (!c3.contains(p)) {
            all_inside = false;
            std::cout << "Point (" << p.x << "," << p.y << ") is outside!\n";
        }
    }
    std::cout << "All points inside: " << (all_inside ? "Yes" : "No") << "\n";

    std::vector<LineSegment> triangle = {
        {{0, 0}, {6, 0}},
        {{6, 0}, {3, 5}},
        {{3, 5}, {0, 0}}
    };
    Circle c4 = MinimumEnclosingCircleForSegments(triangle);
    std::cout << "Test 4 (Triangle): Center=(" << c4.center.x << "," << c4.center.y 
              << "), Radius=" << c4.radius << "\n";

    return 0;
}
