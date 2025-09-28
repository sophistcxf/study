/*************************************************************************
  > File Name: main.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 16 Aug 2017 05:40:24 PM CST
 ************************************************************************/

#include <iostream>
#include <fstream>
using namespace std;

#include <boost/algorithm/string.hpp>

#include "kmeans.h"

class Point
{
public:
	Point(float tx, float ty, unsigned i) : x(tx), y(ty), index(i) {}
	float x;
	float y;
	unsigned extra;
	unsigned index;
};

class CoorsGetor
{
public:
	std::vector<double> operator() (const Point* pt)
	{
		std::vector<double> v;
		v.push_back(pt->x*100);
		v.push_back(pt->y);
		std::vector<double> t;
		t.swap(v);
		return t;
	}
};

int main()
{
	KMeans<Point, CoorsGetor> kmeans;
	std::vector<Point*> pts;
	//float samples[][2] = {{1.0, 2.0}, {1.0, 0.0}, {4.0, 2.0}, {1.0, 4.0}, {4.0, 4.0}, {4.0, 0.0}};
	//float samples[][2] = {{1.0, 2.0}, {1.0, 0.0}, {4.0, 2.0}};
	std::vector<std::vector<double> > samples;
	std::ifstream ifs("text.txt", std::ifstream::in);
	std::string line;
	while (getline(ifs, line))
	{
		std::vector<std::string> fds;
		boost::split(fds, line, boost::is_any_of(" \t"));
		std::vector<double> coors;
		for (unsigned i = 0; i < fds.size(); ++i)
		{
			coors.push_back(atof(fds[i].c_str()));
		}
		samples.push_back(coors);
	}
	for (unsigned i = 0; i < samples.size(); ++i)
	{
		Point* pt = new Point(samples[i][0], samples[i][1], i);
		pts.push_back(pt);
	}
	kmeans.cluster(pts, 2);
	std::vector<unsigned> labels = kmeans.labels();
	for (unsigned i = 0; i < labels.size(); ++i)
	{
		std::cout << labels[i] << ",";
	}
	std::cout << endl;
	std::cout << kmeans.inertia() << endl;

	std::vector<std::vector<double> > centers = kmeans.centers();
	for (unsigned i = 0; i < centers.size(); ++i)
	{
		for (unsigned j = 0; j < centers[i].size(); ++j)
		{
			std::cout << centers[i][j];
			if (j != centers[i].size())
				std::cout << "\t";
		}
		std::cout << std::endl;
	}
	return 0;
}
