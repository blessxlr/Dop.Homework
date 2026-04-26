#include "min_circle.h"
#include <cmath>
#include <algorithm>
#include <random>

const double EPS = 1e-8;

double distanceSquared(const Point2D& p1, const Point2D& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

double distance(const Point2D& p1, const Point2D& p2) {
    return std::sqrt(distanceSquared(p1, p2));
}

Circle circleFromDiameter(const Point2D& p1, const Point2D& p2) {
    Circle c;
    c.center.x = (p1.x + p2.x) / 2.0;
    c.center.y = (p1.y + p2.y) / 2.0;
    c.radius = distance(p1, p2) / 2.0;
    return c;
}

Circle circleFrom3Points(const Point2D& p1, const Point2D& p2, const Point2D& p3) {
    double area2 = std::abs((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x));
    
    if (area2 < EPS) {
        double d12 = distanceSquared(p1, p2);
        double d23 = distanceSquared(p2, p3);
        double d31 = distanceSquared(p3, p1);
        
        if (d12 >= d23 && d12 >= d31) {
            return circleFromDiameter(p1, p2);
        } else if (d23 >= d12 && d23 >= d31) {
            return circleFromDiameter(p2, p3);
        } else {
            return circleFromDiameter(p3, p1);
        }
    }

    double ax = p1.x, ay = p1.y;
    double bx = p2.x, by = p2.y;
    double cx = p3.x, cy = p3.y;
    
    double d = 2.0 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
    
    double ux = ((ax * ax + ay * ay) * (by - cy) + 
                 (bx * bx + by * by) * (cy - ay) + 
                 (cx * cx + cy * cy) * (ay - by)) / d;
                 
    double uy = ((ax * ax + ay * ay) * (cx - bx) + 
                 (bx * bx + by * by) * (ax - cx) + 
                 (cx * cx + cy * cy) * (bx - ax)) / d;
    
    Circle c;
    c.center.x = ux;
    c.center.y = uy;
    c.radius = distance(c.center, p1);
    
    return c;
}

bool getIntersection(const LineSegment& s1, const LineSegment& s2, Point2D& intersection) {
    double x1 = s1.start.x, y1 = s1.start.y;
    double x2 = s1.end.x, y2 = s1.end.y;
    double x3 = s2.start.x, y3 = s2.start.y;
    double x4 = s2.end.x, y4 = s2.end.y;
    
    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    
    if (std::abs(denom) < EPS) return false;
    
    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;
    
    if (t >= -EPS && t <= 1.0 + EPS && u >= -EPS && u <= 1.0 + EPS) {
        intersection.x = x1 + t * (x2 - x1);
        intersection.y = y1 + t * (y2 - y1);
        return true;
    }
    
    return false;
}

Point2D findMidPoint(const LineSegment& segment) {
    Point2D mid;
    mid.x = (segment.start.x + segment.end.x) / 2.0;
    mid.y = (segment.start.y + segment.end.y) / 2.0;
    return mid;
}

std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment>& segments) {
    std::vector<Point2D> points;
    
    for (const auto& seg : segments) {
        points.push_back(seg.start);
        points.push_back(seg.end);
        points.push_back(findMidPoint(seg));
    }
    
 
    for (size_t i = 0; i < segments.size(); ++i) {
        for (size_t j = i + 1; j < segments.size(); ++j) {
            Point2D intersection;
            if (getIntersection(segments[i], segments[j], intersection)) {
                points.push_back(intersection);
            }
        }
    }
    
    return points;
}

Circle minCircleHelper(std::vector<Point2D>& points, std::vector<Point2D> boundary, size_t n) {
     if (n == 0 || boundary.size() == 3) {
        if (boundary.empty()) {
            return {{0, 0}, 0};
        } else if (boundary.size() == 1) {
            return {boundary[0], 0};
        } else if (boundary.size() == 2) {
            return circleFromDiameter(boundary[0], boundary[1]);
        } else {
            return circleFrom3Points(boundary[0], boundary[1], boundary[2]);
        }
    }
     static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, n - 1);
    size_t idx = dis(gen);
    
    Point2D p = points[idx];
    std::swap(points[idx], points[n - 1]);
     Circle c = minCircleHelper(points, boundary, n - 1);
 
    if (c.contains(p)) {
        return c;
    }
 
    boundary.push_back(p);
    return minCircleHelper(points, boundary, n - 1);
}

Circle minCircle(std::vector<Point2D> points) {
 
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::shuffle(points.begin(), points.end(), gen);
    
    std::vector<Point2D> boundary;
    return minCircleHelper(points, boundary, points.size());
}

Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment>& segments) {
    if (segments.empty()) {
        return {{0, 0}, -1};
    }
 
    if (segments.size() == 1) {
        return circleFromDiameter(segments[0].start, segments[0].end);
    }
 
    std::vector<Point2D> points = extractPointsFromSegments(segments);
    
     return minCircle(points);
}
