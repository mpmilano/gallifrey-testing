import java.io.*;

public class Stdin2{
    public static void main(String[] args){
	System.err.println("java initialized");
	int poke_counter = 0;
	try {
	    while (System.in.read() == 0){
		System.in.read();
		++poke_counter;
		System.out.write(0);
		System.out.flush();
	    }
	}
	catch (IOException closed){
	}
    }
}
