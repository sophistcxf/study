# 在不同平台下，c/c++编出的库的名字是不同的，要符合平台的规范
# 否则执行时报找不到
# 如 mac 上是 libXXXX.dylib, Linux 上是 libXXXX.so

ndk="/Users/xiangfei.cui/workspace/android-ndk-r21e/"

include_dir="$ndk""toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/"
include_abi_dir="$ndk""toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/arm-linux-androideabi/"

# 下面的命令会生成native方法的头文件，即HelloNative.h
# java -h . HelloNative.java

kernel_name=`uname -s`
if [ $kernel_name == "Darwin" ]; then
    gcc -I./ -I ${include_dir} -I ${include_abi_dir} -fPIC -shared -o libHelloNative.dylib HelloNative.c
elif [ $kernel_name == "Linux" ]; then
    gcc -I./ -fPIC -shared -o libHelloNative.o HelloNative.c
fi

javac HelloNative.java
java HelloNative
