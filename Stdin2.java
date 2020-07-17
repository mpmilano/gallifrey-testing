import java.io.*;

public class Stdin2{
    public static void main(String[] args) throws Throwable {
	System.err.println("java initialized");
	int poke_counter = 0;
	try {
	    int sleep_amnt = 40;
	    while (System.in.read() == 0){
		System.in.read();
		++poke_counter;
		Thread.sleep(sleep_amnt);
		//++sleep_amnt;
		System.out.write(0);
		System.out.flush();
	    }
	}
	catch (IOException closed){
	}
    }
}
