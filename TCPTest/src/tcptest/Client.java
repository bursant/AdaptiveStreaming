package tcptest;

import java.io.DataOutputStream;
import java.net.Socket;

public class Client {

	public static void main(String[] args) throws Exception {
		char buffer[] = new char[1000]; //char is 2 bytes
		char sign = 0;
		for(int i = 0; i < 1000; i++)
			buffer[i] = sign++;
		byte b[] = new String(buffer).getBytes(); 
		Socket clientSocket = new Socket("11.0.0.2", 5556);
		DataOutputStream outToServer = new DataOutputStream(clientSocket.getOutputStream());
		for(int i = 0; i < 524; i++)
			outToServer.write(b);
		
		clientSocket.close();
	}

}