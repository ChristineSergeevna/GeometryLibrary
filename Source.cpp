#include "Header.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <cmath>

Points Line::intersect(Line &line) 
{
	Points result;
	Vector r1 = end - start;
	Vector r2 = line.end - line.start;
	long double d = r1 % r2;
	long double t1 = ((line.start - start) % r2) / d;
	long double t2 = ((line.start - start) % r1) / d;
	if ((abs(d) < eps) || (abs(t1) > 1 + eps) || (abs(t2) > 1 + eps)) 
		return result;
	result.push_back(start + r1 * t1);
	return result;
}

Points Line::intersect(Polyline &polyline) 
{
	Points result;
	for (size_t i = 1; i < polyline.points.size(); i++) 
	{
		Points a = intersect(Line(polyline.points[i - 1], polyline.points[i]));
		if (a.size()) 
			result.push_back(a[0]);
	}
	return result;
}

Points Line::intersect(Circle &circle) 
{
	Points result;
	Vector d1 = end - circle.center; 
	Vector d2 = end - start; 
	
	long double  a = d2 ^ d2;
	long double  b = 2. * (d1 ^ d2);
	long double  c = (d1 ^ d1) - circle.radius * circle.radius;

	long double  d = b * b - 4 * a * c;
	if (d < eps) 
		return result;
	long double  t1 = (-b - sqrt(d)) / 2 / a;
	long double  t2 = (-b + sqrt(d)) / 2 / a;
	if (t1 < eps && t1 > -1. - eps) 
		result.push_back(end + (d2 * t1));
	if (t2 < eps && t2 > -1. - eps) 
		result.push_back(end + (d2 * t2));
	return result;
}

Points Polyline::intersect(Polyline &polyline) 
{
	Points result;
	for (size_t i = 1; i < polyline.points.size(); i++) 
	{
		Points a = intersect(Line(polyline.points[i - 1], polyline.points[i]));
		if (a.size() && (find(result.begin(), result.end(), a[0]) == result.end()))
			result.push_back(a[0]);
	}
	return result;
}

Points Polyline::intersect(Circle &circle) 
{
	Points result;
	for (size_t i = 1; i < points.size(); i++) 
	{
		Points a = Line(points[i - 1], points[i]).intersect(circle);
		for (size_t i = 0; i < a.size(); i++) 
		{
			bool fail = false;
			for (size_t j = 0; j < result.size(); j++)
				fail = (a[i] == result[j]) || fail;
			if (!fail) 
				result.push_back(a[i]);
		}
	}
	return result;
}

Points Circle::intersect(Circle &circle) 
{
	Points result;
	long double d = (center - circle.center).length();
	if ((d > radius + circle.radius + eps) || (d < abs(radius - circle.radius) + eps) || (abs(d) < eps && abs(radius - circle.radius) < eps)) 
		return result; 
	long double a = (pow(d, 2) + pow(radius, 2) - pow(circle.radius, 2)) / (2. * d);
	long double h = sqrt(pow(radius, 2) - pow(a, 2));
	Point p = center + (circle.center - center) * (a / d);
	result.push_back(Point(p.x + h * (circle.center.y - center.y) / d, p.y - h * (circle.center.x - center.x) / d));
	if (abs(a - radius) > eps) 
		result.push_back(Point(p.x - h * (circle.center.y - center.y) / d, p.y + h * (circle.center.x - center.x) / d));
	return result;
}

Points reedAns(ifstream& file, int n)
{
	Points points;
	while (n)
	{
		long double  x, y;
		file >> x >> y;
		points.push_back(Point(x, y));
		n--;
	}
	return points;
}

bool comp(Points& p1, Points& p2)
{
	if (p1.size() != p2.size()) return false;
	for (int i = 0; i < p1.size(); i++)
		if (!(p1[i] == p2[i])) return false;
	return true;
}

void print(Points& p, ifstream &ans) 
{
	for (int i = 0; i < p.size(); i++)
		printf("x = %F y = %F\n", p[i].x, p[i].y);
	if (comp(p, reedAns(ans, p.size())))
		cout << "ok\n" << endl;
	else cout << "fail\n" << endl;
}

void testing()
{
	ifstream tests("tests.txt");
	ifstream ans("ans.txt");

	Points points;
	int n;
	tests >> n;

	for (int i = 0; i < n; i++)
	{
		long double  x, y, x1, y1, r, _x, _y, _x1, _y1, _r;
		tests >> x >> y >> x1 >> y1 >> _x >> _y >> _x1 >> _y1;
		Base *line1 = new Line(Point(x, y), Point(x1, y1));
		Base *line2 = new Line(Point(_x, _y), Point(_x1, _y1));
		tests >> x >> y >> r >> _x >> _y >> _r;
		Base *circle1 = new Circle(Point(x, y), r);
		Base *circle2 = new Circle(Point(_x, _y), _r);
		for (int j = 0; j < 4; j++)
		{
			tests >> x >> y;
			points.push_back(Point(x, y));
		}
		Base *polyline1 = new Polyline(points);

		for (int j = 0; j < 4; j++)
		{
			tests >> x >> y;
			points.push_back(Point(x, y));
		}
		Base *polyline2 = new Polyline(points);

		cout << "test" << i + 1 << "\nline1 and polyline1 intersection: " << endl;
		print(line1->intersect(*polyline1), ans);
		cout << "polyline1 and circle1 intersection: " << endl;
		print(polyline1->intersect(*circle1), ans);
		cout << "circle1 and line1 intersection: " << endl;
		print(circle1->intersect(*line1), ans);
		cout << "line1 and line2 intersection: " << endl;
		print(line1->intersect(*line2), ans);
		cout << "circle1 and circle2 intersection: " << endl;
		print(circle1->intersect(*circle2), ans);
		cout << "polyline1 and polyline2 intersection: " << endl;
		print(polyline1->intersect(*polyline2), ans);
	}
	system("pause");
}

int main()
{
	testing();
	return 0;
}