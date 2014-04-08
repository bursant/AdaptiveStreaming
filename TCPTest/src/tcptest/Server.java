package tcptest;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

	public static void main(String[] args) throws Exception {
		char buffer[] = new char[100000];
		ServerSocket serverSocket = new ServerSocket(5555);
		Socket connectionSocket = serverSocket.accept();
		BufferedReader inFromClient = new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));
		double start = System.currentTimeMillis();
		while(inFromClient.read(buffer) != -1){}
		double end = System.currentTimeMillis();
		System.out.println(end-start);
		serverSocket.close();
	}

}