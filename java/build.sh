kernel_name=`uname -s`
if [ $kernel_name == "Darwin" ]; then
    gcc -I./ -I"/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/JavaVM.framework//Versions/A/Headers/" -fPIC -shared -o libHelloNative.dylib HelloNative.c
elif [ $kernel_name == "Linux" ]; then
    gcc -I./ -I"/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/JavaVM.framework//Versions/A/Headers/" -fPIC -shared -o libHelloNative.dylib HelloNative.c
fi

javac HelloNative.java
java HelloNative
