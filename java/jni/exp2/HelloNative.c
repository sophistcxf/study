#include <stdio.h>
#include <string>
#include "HelloNative.h"

JNIEXPORT jlong JNICALL Java_HelloNative_sumIntegers(JNIEnv * env, jobject, jint first, jint second) {
    printf("C++: The numbers received are : %d and %d\n", first, second);
    return (long)first + (long)second;
}

JNIEXPORT jstring JNICALL Java_HelloNative_sayHelloToMe(JNIEnv * env, jobject thisObject, jstring name, jboolean isFemale) {
    const char* nameCharPointer = env->GetStringUTFChars(name, NULL);
    std::string title;
    if(isFemale) {
        title = "Ms. ";
    }
    else {
        title = "Mr. ";
    }

    std::string fullName = title + nameCharPointer;
    return env->NewStringUTF(fullName.c_str());
}
