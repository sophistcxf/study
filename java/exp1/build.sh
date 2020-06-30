# 在不同平台下，c/c++编出的库的名字是不同的，要符合平台的规范
# 否则执行时报找不到
# 如 mac 上是 libXXXX.dylib, Linux 上是 libXXXX.so

kernel_name=`uname -s`
if [ $kernel_name == "Darwin" ]; then
    gcc -I./ -I"/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/JavaVM.framework//Versions/A/Headers/" -fPIC -shared -o libHelloNative.dylib HelloNative.c
elif [ $kernel_name == "Linux" ]; then
    gcc -I./ -I"/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/JavaVM.framework//Versions/A/Headers/" -fPIC -shared -o libHelloNative.dylib HelloNative.c
fi

javac HelloNative.java
java HelloNative
