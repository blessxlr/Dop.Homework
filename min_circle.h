#pragma once

#include <vector>

constexpr double EPS = 1e-9;

struct Point2D {
    double x, y;

    Point2D();
    Point2D(double x_, double y_);

    bool operator==(const Point2D& other) const;
    bool operator!=(const Point2D& other) const;
};

struct LineSegment {
    Point2D start;
    Point2D end;
};

struct Circle {
    Point2D center;
    double radius;

    bool isValid() const;
    bool contains(const Point2D& point) const;
    bool contains(const LineSegment& segment) const;
    bool containsEntireSegment(const LineSegment& segment) const;
};

Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment>& segments);

double distanceSquared(const Point2D& p1, const Point2D& p2);
double distance(const Point2D& p1, const Point2D& p2);
Point2D findMidPoint(const LineSegment& segment);
Circle circleFromDiameter(const Point2D& p1, const Point2D& p2);
Circle circleFrom3Points(const Point2D& p1, const Point2D& p2, const Point2D& p3);
std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment>& segments);
Circle minCircle(std::vector<Point2D> points);
Circle minCircleHelper(std::vector<Point2D>& points, std::vector<Point2D> boundary, size_t n);
