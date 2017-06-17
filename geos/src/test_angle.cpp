/*************************************************************************
  > File Name: test_angle.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun 19 Jun 2016 08:08:29 AM PDT
 ************************************************************************/

#include <iostream>
using namespace std;

#include "geos/algorithm/Angle.h"

namespace ga=geos::algorithm;

int main()
{
  cout << ga::Angle::toDegrees(ga::Angle::PI_TIMES_2) << endl;
  cout << ga::Angle::toDegrees(ga::Angle::PI_TIMES_2/2) << endl;

  cout << ga::Angle::getTurn(ga::Angle::PI_TIMES_2/2, 0) << endl;
  cout << ga::Angle::getTurn(0, ga::Angle::PI_TIMES_2/2) << endl;
  cout << ga::Angle::getTurn(ga::Angle::PI_TIMES_2, ga::Angle::PI_TIMES_2) << endl;
  return 0;
}
