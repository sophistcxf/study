/*************************************************************************
  > File Name: van.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun 24 Jul 2016 01:55:07 PM CST
 ************************************************************************/

#include <stdio.h>
#include "car.h"

static void run()
{
  printf("I am Van, running...\n");
}

static void stop()
{
  printf("I am Van, stopped.\n");
}

struct Car module = { &run, &stop };

struct Car* get_module()
{
  return &module;
}

