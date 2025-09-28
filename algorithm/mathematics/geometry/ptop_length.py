import math

pi = 3.14159265358979323846264338327950288

def ptop_length(x1, y1, x2, y2):
    '''
    '''
    dx = x2 - x1
    dy = y2 - y1
    sx = math.cos((y1+y2)*0.01745329252/2)
    return math.sqrt(dx * dx * sx + dy * dy) * 111195.0

def ptop_length3D(lon1, lat1, alt1, lon2, lat2, alt2):
    pi = 3.14159265358979323846264338327950288
    def degree2radian(v):
        return v * pi / 180.0
    f = 0.003352813177897
    re = 6378137
    rad2deg = 180.0 / pi
    rm = re * (1 - 2*f + 3*f*((math.sin(degree2radian(lat1)) * math.sin(degree2radian(lat1)))));
    rn = re * (1 + f*((math.sin(degree2radian(lat1)) * math.sin(degree2radian(lat1)))));
    de = (lon2 - lon1) / rad2deg * (rn + alt1) * math.cos(degree2radian((lat1)));
    dn = (lat2 - lat1) / rad2deg * (rm + alt1);
    return math.sqrt(de * de + dn * dn)

if __name__ == '__main__':
    x1 = 120.058791
    y1 = 30.295669
    z1 = 9.01498
    x2 = 120.058779
    y2 = 30.29567
    z2 = 9.469387
    print ptop_length(x1, y1, x2, y2)
    print ptop_length3D(x1, y1, z1, x2, y2, z2)
