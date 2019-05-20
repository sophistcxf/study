//
//  mylib.cpp
//  mylib
//
//  Created by xiangfei.cui on 2019/5/20.
//  Copyright © 2019 cxxxxf. All rights reserved.
//

#include <iostream>
#include "mylib.hpp"
#include "mylibPriv.hpp"

void mylib::HelloWorld(const char * s)
{
    mylibPriv *theObj = new mylibPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void mylibPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

