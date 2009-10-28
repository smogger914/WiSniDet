public class JavaTest{

	private native int addOne(int n);

	static
  {
		System.loadLibrary("JavaTest");
	}

	public static void main(String[] args)
	{
		//
		JavaTest o = new JavaTest();
		
		System.out.println(o.addOne(11));
		//System.out.println("hello world");

	}
}
