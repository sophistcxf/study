/*************************************************************************
  > File Name: main.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun 24 Jul 2016 01:58:14 PM CST
 ************************************************************************/

#include <dlfcn.h>
#include <stdlib.h>
#include "car.h"

struct Car* car;

struct Car *register_module(char *module_name)
{
  struct Car *(*get_module)();
  void *handle;
  handle = dlopen(module_name, RTLD_LAZY);
  if (!handle) return NULL;
  get_module = (struct Car*(*)())dlsym(handle, "get_module");
  if (dlerror() != NULL) {
    dlclose(handle);
    return NULL;
  }
  dlclose(handle);
  return get_module();
}

int main(int argc, char *argv[])
{
  struct Car *car;
  if ((car = register_module("./van.so")) == NULL) return -1;
  car->run();
  car->stop();
  return 0;
}
