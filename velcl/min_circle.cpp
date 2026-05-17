#include "min_circle.h"

bool Circle::isValid() const
{
	if (radius >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Circle::contains(const Point2D& point) const
{
	if (!isValid())
	{
		return false;
	}
	if (distance(point,center) <= radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Circle::contains(const LineSegment& segment) const
{
	if (contains(segment.start) && contains(segment.end))
	{
		return true;
	}
	else
	{
		return false;
	}
}

double distanceSquared(const Point2D& p1, const Point2D& p2)
{
	return pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2);
}

double distance(const Point2D& p1, const Point2D& p2)
{
	return sqrt(distanceSquared(p1,p2));
}

Circle circleFromDiameter(const Point2D& p1, const Point2D& p2)
{
	double r = distance(p1,p2) / 2;
	Point2D centr = findMidPoint({p1, p2});
	return Circle{centr,r};
}

Point2D findMidPoint(const LineSegment& segment)
{
	double dx = (segment.start.x + segment.end.x) / 2;
	double dy = (segment.start.y + segment.end.y) / 2;
	return Point2D{dx, dy};
}

Circle circleFrom3Points(const Point2D& p1, const Point2D& p2, const Point2D& p3)
{
	double A = p2.x - p1.x;
	double B = p2.y - p1.y;
	double C = p3.x - p1.x;
	double D = p3.y - p1.y;
	double E = A * (p1.x + p2.x) + B * (p1.y + p2.y);
	double F = C * (p1.x + p3.x) + D * (p1.y + p3.y);
	double G = 2 * (A * (p3.y - p2.y) - B * (p3.x - p2.x));
	if (G == 0)
	{
		return Circle{{0,0},0};
	}
	else
	{
		double C_x = (D * E - B * F) / G;
		double C_y = (A * F - C * E) / G;
		double r = distance({C_x,C_y},p1);
		return Circle{{C_x, C_y}, r};
	}
}

std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment>& segments)
{
	std::vector<Point2D> vec;
	for (size_t i = 0; i < segments.size(); ++i)
	{
		vec.push_back(segments[i].start);
		vec.push_back(segments[i].end);
		vec.push_back(findMidPoint(segments[i]));
	}
	return vec;
}

Circle minCircleHelper(std::vector<Point2D>& points, std::vector<Point2D> boundary, size_t n)
{
	if (boundary.size() == 3)
	{
		return circleFrom3Points(boundary[0], boundary[1], boundary[2]);
	}
	if (n == 0)
	{
		switch (boundary.size())
		{
			case 2:
				return circleFromDiameter(boundary[0], boundary[1]);
			case 1:
				return Circle{boundary[0], 0};
			default:
				return Circle{{0, 0}, 0};
		}
	}
	Circle circle = minCircleHelper(points, boundary, n-1);
	if (circle.contains(points[n-1]))
	{
		return circle;
	}
	boundary.push_back(points[n-1]);
	return minCircleHelper(points, boundary, n-1);
}

Circle minCircle(std::vector<Point2D> points)
{
	return minCircleHelper(points, {}, points.size());
}

Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment>& segments)
{
	std::vector<Point2D> points = extractPointsFromSegments(segments);
	return minCircle(points);
}
