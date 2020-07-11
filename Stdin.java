import java.io.*;

public class Stdin{
    public static void main(String[] args){
	System.out.println("java initialized");
	int poke_counter = 0;
	try {
	    while (System.in.read() == 0){
		System.in.read();
		++poke_counter;
	    }
	}
	catch (IOException closed){
	}
	System.out.println("Stream done, poked " + poke_counter + " times");
    }
}
