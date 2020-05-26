public class HelloNative
{
    static
    {
        System.loadLibrary("HelloNative");
    }
      
    public static native void sayHello();
      
    @SuppressWarnings("static-access")
    public static void main(String[] args)
    {
        new HelloNative().sayHello();
    }
}
