#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

using namespace std;
const long double eps = 1e-5;

typedef class _vector
{
public:
	long double  x, y;
	_vector() : x(0.), y(0.) {};
	_vector(long double  x, long double  y) : x(x), y(y) {};

	long double length() { return sqrt(x * x + y * y); }

	_vector operator-(const _vector a)
	{
		return _vector(x - a.x, y - a.y);
	}

	_vector operator+(const _vector a)
	{
		return _vector(x + a.x, y + a.y);
	}

	_vector operator*(const long double a)
	{
		return _vector(x * a, y * a);
	}

	/* скалярное произведение */
	double operator^(const _vector a)
	{
		return x * a.x + y * a.y;
	}
	
	double operator%(const _vector a) 
	{
		return x * a.y - y * a.x;
	} 

	bool operator==(const _vector a)
	{
		return (abs(x - a.x) < eps) && (abs(y - a.y) < eps);
	}
} Vector, Point;

typedef vector<Point> Points;
class Line;
class Polyline;
class Circle;

class Base
{
public:
	virtual Points intersect(Base&) = 0;
	virtual Points intersect(Line&) = 0;
	virtual Points intersect(Polyline&) = 0;
	virtual Points intersect(Circle&) = 0;
	virtual long double length() = 0;
};

class Line : public Base
{
public:
	Point start, end;

	Line() {};
	Line(Point start, Point end) : start(start), end(end) {};
	Points intersect(Line& l) override;
	Points intersect(Polyline& p) override;
	Points intersect(Circle& c) override;
	Points intersect(Base& a) override { return a.intersect(*this); }
	long double length() override { return (end - start).length(); }
};

class Polyline : public Base
{
public:
	Points points;

	Polyline() {};
	Polyline(Points& p)
	{
		for (Points::iterator it = p.begin(); it != p.end(); it++)
			points.push_back(*it);
	}
	Points intersect(Line& l) override { return l.intersect(*this); }
	Points intersect(Polyline& p) override;
	Points intersect(Circle& c) override;
	Points intersect(Base& a) override { return a.intersect(*this); }
	long double length() override
	{
		long double  s = 0;
		for (size_t i = 1; i < points.size(); i++)
			s += (points[i] - points[i - 1]).length();
		return s;
	}
};

class Circle : public Base
{
public:
	Point center;
	long double  radius;

	Circle() {};
	Circle(Point center, long double radius) : center(center), radius(radius) {};
	Points intersect(Line& l) override { return l.intersect(*this); }
	Points intersect(Polyline& p) override { return p.intersect(*this); }
	Points intersect(Circle& c) override;
	Points intersect(Base& a) override { return a.intersect(*this); }
	long double length() override { return 2. * M_PI * radius; }
};
