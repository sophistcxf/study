//
//  mylibPriv.hpp
//  mylib
//
//  Created by xiangfei.cui on 2019/5/20.
//  Copyright © 2019 cxxxxf. All rights reserved.
//

/* The classes below are not exported */
#pragma GCC visibility push(hidden)

class mylibPriv
{
    public:
    void HelloWorldPriv(const char *);
};

#pragma GCC visibility pop
