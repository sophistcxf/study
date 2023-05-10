public class HelloNative
{
    static
    {
        System.loadLibrary("HelloNative");
    }

    private native long sumIntegers(int first, int second);
    private native String sayHelloToMe(String name, boolean isFemale);
      
    @SuppressWarnings("static-access")
    public static void main(String[] args)
    {
        HelloNative obj = new HelloNative();
        obj.sumIntegers(10, 11);
        obj.sayHelloToMe("cxxxxf", false);
    }
}
