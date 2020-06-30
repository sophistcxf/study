#include "HelloNative.h"
#include <stdio.h>
  
JNIEXPORT void JNICALL Java_HelloNative_sayHello(JNIEnv* env, jclass c)
{
    printf("Hello，JNI");  
}
