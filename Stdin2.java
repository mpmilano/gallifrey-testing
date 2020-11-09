import java.io.*;

public class Stdin2{
    public static void main(String[] args) throws Throwable {
	System.err.println("java initialized!");
	int poke_counter = 0;
	Runtime.getRuntime().addShutdownHook(new Thread(){
		@Override
		public void run(){
		    System.err.println("Java exit");
		}
	    });
	try {
	    int sleep_amnt = 40;
	    while (System.in.read() < 2){
		++poke_counter;
		Thread.sleep(sleep_amnt);
		//++sleep_amnt;
		System.out.write(0);
		System.out.flush();
	    }
	}
	catch (IOException closed){
	}
	finally {
	    System.err.println("ready for clean exit, poked " + poke_counter + " times");
	}
	if (poke_counter > 10)
	    Thread.sleep(100000000);
    }
}
