#include <iostream>
#include <iomanip>
#include <cmath>

#define S_PI 3.14159265358979323846264338327950288

struct Point
{
public:
    Point(double x = 0, double y = 0) : lon(x), lat(y) {}
    double lon;
    double lat;
    double lat_double() const
    {
        return lat / 1e7;
    }
    double lon_double() const
    {
        return lon / 1e7;
    }
    bool operator == (const Point& pnt) const
    {
        return lon == pnt.lon && lat == pnt.lat;
    }
};

enum ProjectPosition
{
    PROJECTPOS_BEFORE_START = 0,
    PROJECTPOS_ONLINE,
    PROJECTPOS_AFTER_END
};

long long calcDP(const int x1, const int y1, const int x2, const int y2) {
    return ((long long)x1*x2 + (long long)y1*y2);
}

double calcProjPosRatio(const Point &startPoint, const Point &endPoint, const Point& target_pos, Point &resultPoint) {
    if (startPoint == endPoint) {
        resultPoint = startPoint;
        return (0.0);
    }

    int abX = endPoint.lon - startPoint.lon;        /* 线终点和起点经度差 */
    int abY = endPoint.lat - startPoint.lat;        /* 线终点和起点纬度差 */
    int acX = target_pos.lon - startPoint.lon;
    int acY = target_pos.lat - startPoint.lat;

    double t = std::cos(target_pos.lat_double()/180.0*S_PI);
    int abX_ = abX;
    abX = int(std::round(abX * t));
    acX = int(std::round(acX * t));

    long long f = calcDP(abX, abY, acX, acY);
    long long d = calcDP(abX, abY, abX, abY);

    if (0 == d) {
        resultPoint = startPoint;
        return (0.0);
    }

    double r = (double)f / d;

    resultPoint.lon = int((double)startPoint.lon + (r * (double)abX_) + 0.5);
    resultPoint.lat = int((double)startPoint.lat + (r * (double)abY) + 0.5);

    return r;
}

ProjectPosition calcProjPosOfLine(const Point& startPoint, const Point& endPoint, const Point& target_pos, Point& resultPoint) {
    double r = calcProjPosRatio(startPoint, endPoint, target_pos, resultPoint);
    ProjectPosition projPos;
    if (r < 0.0) {
        projPos = PROJECTPOS_BEFORE_START;
        resultPoint = startPoint;
    } else if (r > 1.0) {
        projPos = PROJECTPOS_AFTER_END;
        resultPoint = endPoint;
    } else {
        projPos = PROJECTPOS_ONLINE;
    }
    return projPos;
}


int main()
{
    Point start_point(1165397569, 399444272);
    Point end_point(1165401780, 399438411);
    Point target_point(1165427967, 399401963);
    Point result_point;
    ProjectPosition proj_pos = calcProjPosOfLine(start_point, end_point, target_point, result_point);
    std::cout << std::setprecision(10);
    std::cout << "start_point: " << start_point.lon_double() << "," << start_point.lat_double() << std::endl
              << "end_point: " << end_point.lon_double() << "," << end_point.lat_double() << std::endl
              << "target_point: " << target_point.lon_double() << "," << target_point.lat_double() << std::endl
              << "result_point: " << result_point.lon_double() << "," << result_point.lat_double() << std::endl
              << "proj position: " << proj_pos << std::endl;
    return 0;
}
