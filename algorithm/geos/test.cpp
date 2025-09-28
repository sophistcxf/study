#include <iostream>
#include <iomanip>
#include <math.h>

#define MAP_DIST_FOR_REAL_DIST 9

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
};

 void calc_rect(const Point& center, double rect_dist, Point& min_pos, Point& max_pos)
{
    int32_t boundaryDis = 0;
    int32_t rate = 0;

    boundaryDis = int32_t(rect_dist* MAP_DIST_FOR_REAL_DIST) * 10;
    rate = (int32_t)(boundaryDis / cos((double)center.lat_double() * 0.01745329252f));

    min_pos.lon = (center.lon - rate);
    min_pos.lat = (center.lat - boundaryDis);

    max_pos.lon = (center.lon + rate );
    max_pos.lat = (center.lat + boundaryDis);
}

void test1()
{
    Point center(1089275274, 343013357);
    Point min_pos, max_pos;
    calc_rect(center, 50, min_pos, max_pos);
    std::cout << std::setprecision(10)
              << center.lon_double() << "," << center.lat_double() << std::endl
              << min_pos.lon_double() << "," << min_pos.lat_double() << std::endl
              << max_pos.lon_double() << "," << max_pos.lat_double() << std::endl;
}

/*!
 * 计算 start_point -> end_point 的方向，正东逆时针
 */
float calc_azi_of_line(const Point& start_point, const Point& end_point) {
    float angle = 0.0f;                                /* 线的角度 返回值 */
    double fac = 1.0f;                                /* 距离表对应的值 */

    fac = cos(start_point.lat_double() / 180.0 * S_PI);

    if ((fac > 0.0000001) || (fac < -0.0000001)) {
        double k = 0.0f; /* 斜率 */
        if ((end_point.lon - start_point.lon) == 0) {
            /* 线段起点和终点在同一经度上 */
            /* 判断起点和终点的相对位置 */
            if (start_point.lat == end_point.lat) {
                /* 终点纬度 = 起点纬度 线的角度为0°*/
                angle = 0.0f;
            } else if (start_point.lat < end_point.lat) {
                /* 终点纬度 > 起点纬度 线的角度为90°*/
                angle = 90.0f;
            } else {
                /* 终点纬度 < 起点纬度 线的角度为270°*/
                angle = 270.0f;
            }
        } else if ((end_point.lon - start_point.lon) > 0) {
            /* 线段终点经度 > 起点经度*/
            k = ((double)end_point.lat - (double)start_point.lat)
                / (((double)end_point.lon - (double)start_point.lon) * fac);

            angle = (float)(180.0 * atan(k) / S_PI);
        } else {
            /* 线段终点经度 < 起点经度*/
            k = ((double)end_point.lat - (double)start_point.lat)
                / (((double)end_point.lon - (double)start_point.lon) * fac);

            angle = (float)((180.0f * atan(k)) / S_PI + 180.0);
        }
    }
    return angle;
}

void test2()
{
    Point pair90[] = {Point(1089268155, 343002569), Point(1089268155,343016541)};
    std::cout << calc_azi_of_line(pair90[0], pair90[1]) << std::endl;
    Point pair45[] = {Point(1089268155, 343002569), Point(1089272998,343006877)}; 
    std::cout << calc_azi_of_line(pair45[0], pair45[1]) << std::endl;
}

int main(int argc, char* argv[])
{
    test2();
    return 0;
}
