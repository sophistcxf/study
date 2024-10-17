#import "Arithmetic.h"
#include "Integer.h"

@implementation Integer(Arithmetic)

- (id) add: (Integer*)subtrahend
{
    self.integer = self.integer += subtrahend.integer;
    return self;
}

@end