#include "min_circle.h"
#include <cmath>
#include <algorithm>
#include <random>

Point2D::Point2D() : x(0), y(0) {}

Point2D::Point2D(double x_, double y_) : x(x_), y(y_) {}

bool Point2D::operator==(const Point2D& other) const {
    return std::abs(x - other.x) <= EPS && std::abs(y - other.y) <= EPS;
}

bool Point2D::operator!=(const Point2D& other) const {
    return !(*this == other);
}

bool Circle::isValid() const {
    return radius >= 0;
}

bool Circle::contains(const Point2D& point) const {
    return distanceSquared(center, point) <= radius * radius + EPS;
}

bool Circle::contains(const LineSegment& segment) const {
    return contains(segment.start) && contains(segment.end) && containsEntireSegment(segment);
}

bool Circle::containsEntireSegment(const LineSegment& segment) const {
    if (!contains(segment.start) || !contains(segment.end))
        return false;

    double abx = segment.end.x - segment.start.x;
    double aby = segment.end.y - segment.start.y;
    double ab_len_sq = abx * abx + aby * aby;

    if (ab_len_sq < EPS)
        return true;

    double t = ((center.x - segment.start.x) * abx + (center.y - segment.start.y) * aby) / ab_len_sq;

    if (t >= 0.0 && t <= 1.0) {
        Point2D proj(segment.start.x + t * abx, segment.start.y + t * aby);
        if (!contains(proj))
            return false;
    }
    return true;
}

double distanceSquared(const Point2D& p1, const Point2D& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

double distance(const Point2D& p1, const Point2D& p2) {
    return std::sqrt(distanceSquared(p1, p2));
}

Point2D findMidPoint(const LineSegment& segment) {
    return Point2D(
        (segment.start.x + segment.end.x) * 0.5,
        (segment.start.y + segment.end.y) * 0.5
    );
}

Circle circleFromDiameter(const Point2D& p1, const Point2D& p2) {
    Circle c;
    c.center = Point2D((p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5);
    c.radius = distance(p1, p2) * 0.5;
    return c;
}

Circle circleFrom3Points(const Point2D& p1, const Point2D& p2, const Point2D& p3) {
    Point2D ab(p2.x - p1.x, p2.y - p1.y);
    Point2D ac(p3.x - p1.x, p3.y - p1.y);
    double crossVal = ab.x * ac.y - ab.y * ac.x;

    if (std::abs(crossVal) < EPS) {
        double d12 = distanceSquared(p1, p2);
        double d13 = distanceSquared(p1, p3);
        double d23 = distanceSquared(p2, p3);
        if (d12 >= d13 && d12 >= d23) return circleFromDiameter(p1, p2);
        if (d13 >= d12 && d13 >= d23) return circleFromDiameter(p1, p3);
        return circleFromDiameter(p2, p3);
    }

    double d = 2.0 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));
    double ux = ((p1.x * p1.x + p1.y * p1.y) * (p2.y - p3.y) +
                 (p2.x * p2.x + p2.y * p2.y) * (p3.y - p1.y) +
                 (p3.x * p3.x + p3.y * p3.y) * (p1.y - p2.y)) / d;
    double uy = ((p1.x * p1.x + p1.y * p1.y) * (p3.x - p2.x) +
                 (p2.x * p2.x + p2.y * p2.y) * (p1.x - p3.x) +
                 (p3.x * p3.x + p3.y * p3.y) * (p2.x - p1.x)) / d;

    Circle c;
    c.center = Point2D(ux, uy);
    c.radius = distance(c.center, p1);
    return c;
}

static bool getIntersection(const LineSegment& s1, const LineSegment& s2, Point2D& out) {
    Point2D r(s1.end.x - s1.start.x, s1.end.y - s1.start.y);
    Point2D s(s2.end.x - s2.start.x, s2.end.y - s2.start.y);
    double rxs = r.x * s.y - r.y * s.x;
    Point2D qmp(s2.start.x - s1.start.x, s2.start.y - s1.start.y);
    double qmp_x_r = qmp.x * r.y - qmp.y * r.x;

    if (std::abs(rxs) < EPS) {
        if (std::abs(qmp_x_r) < EPS) {
            double r_len_sq = r.x * r.x + r.y * r.y;
            double t0 = (qmp.x * r.x + qmp.y * r.y) / r_len_sq;
            double t1 = t0 + (s.x * r.x + s.y * r.y) / r_len_sq;
            double t_min = std::max(0.0, std::min(t0, t1));
            double t_max = std::min(1.0, std::max(t0, t1));
            if (t_min <= t_max + EPS) {
                out = Point2D(s1.start.x + t_min * r.x, s1.start.y + t_min * r.y);
                return true;
            }
        }
        return false;
    }

    double t = (qmp.x * s.y - qmp.y * s.x) / rxs;
    double u = (qmp.x * r.y - qmp.y * r.x) / rxs;

    if (t >= -EPS && t <= 1.0 + EPS && u >= -EPS && u <= 1.0 + EPS) {
        out = Point2D(s1.start.x + t * r.x, s1.start.y + t * r.y);
        return true;
    }
    return false;
}

std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment>& segments) {
    std::vector<Point2D> pts;

    for (const auto& seg : segments) {
        pts.push_back(seg.start);
        pts.push_back(seg.end);
        pts.push_back(findMidPoint(seg));
    }

    for (size_t i = 0; i < segments.size(); ++i) {
        for (size_t j = i + 1; j < segments.size(); ++j) {
            Point2D inter;
            if (getIntersection(segments[i], segments[j], inter)) {
                pts.push_back(inter);
            }
        }
    }

    return pts;
}

Circle minCircleHelper(std::vector<Point2D>& points, std::vector<Point2D> boundary, size_t n) {
    if (n == 0 || boundary.size() == 3) {
        if (boundary.empty()) return {{0, 0}, 0};
        if (boundary.size() == 1) return {boundary[0], 0};
        if (boundary.size() == 2) return circleFromDiameter(boundary[0], boundary[1]);
        return circleFrom3Points(boundary[0], boundary[1], boundary[2]);
    }

    size_t idx = n - 1;
    Point2D p = points[idx];

    Circle c = minCircleHelper(points, boundary, n - 1);

    if (c.contains(p))
        return c;

    boundary.push_back(p);
    return minCircleHelper(points, boundary, n - 1);
}

Circle minCircle(std::vector<Point2D> points) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(points.begin(), points.end(), g);

    std::vector<Point2D> uniq;
    for (const auto& p : points) {
        bool found = false;
        for (const auto& u : uniq) {
            if (u == p) { found = true; break; }
        }
        if (!found)
            uniq.push_back(p);
    }

    if (uniq.empty()) return {{0, 0}, -1};
    if (uniq.size() == 1) return {uniq[0], 0};

    return minCircleHelper(uniq, {}, uniq.size());
}

Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment>& segments) {
    if (segments.empty())
        return {{0, 0}, -1};

    if (segments.size() == 1)
        return circleFromDiameter(segments[0].start, segments[0].end);

    std::vector<Point2D> points = extractPointsFromSegments(segments);
    Circle c = minCircle(points);
    for (const auto& seg : segments) {
        if (!c.contains(seg)) {
            for (const auto& s : segments) {
                for (double t = 0.0; t <= 1.0; t += 0.25) {
                    points.push_back(Point2D(
                        s.start.x + t * (s.end.x - s.start.x),
                        s.start.y + t * (s.end.y - s.start.y)
                    ));
                }
            }
            c = minCircle(points);
            break;
        }
    }

    return c;
}
