public class HelloNative
{
    static
    {
        System.loadLibrary("HelloNative");
    }
      
    // sayHello() 由 jni 提供
    public static native void sayHello();
      
    @SuppressWarnings("static-access")
    public static void main(String[] args)
    {
        new HelloNative().sayHello();
    }
}
