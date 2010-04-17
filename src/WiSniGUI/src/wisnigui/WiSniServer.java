
package wisnigui;

import java.net.*;
import java.io.*;
import java.util.LinkedList;
/*
 * main server class
 */
public class WiSniServer implements Runnable{
        //variables
DatagramSocket aSocket;
        static LinkedList clientList = new LinkedList();

        /*
         * thread stuff for server
         */
        public void run(){
            try{
 			aSocket = new DatagramSocket(5316);
			byte[] buffer = new byte[1000];

			while (true){
				DatagramPacket request = new DatagramPacket(buffer,buffer.length);
				aSocket.receive(request);
                                //System.out.println("got one");
                                //System.out.println(request.getAddress());
                                //System.out.println(request.getLength());
                                addClientToList(request);
				//DatagramPacket reply = new DatagramPacket(d.getData(), d
				//.getLength(), request.getAddress(), request.getPort());
				//aSocket.send(reply);
			}
		}
		catch (SocketException e){
			System.out.println("Socket: " + e.getMessage());
		}
		catch (IOException e){
			System.out.println("IO: " + e.getMessage());
		}
		finally{
			if (aSocket != null)
				aSocket.close();
		}
        }
        /*
         *adds client IP to the list of clients
         */
        public static void addClientToList(DatagramPacket d){
           // System.out.println(d);
            //String temp;
            //temp = d.getHostName();
            clientList.add(d);
        }
        /*
         * returns the LinkedList of clients
         */
        public static LinkedList getClientList(){
            return clientList;
        }
        
	
        /*
         * the actauly server work
         * waits for a Datagram and adds its sender to the list
         */
        WiSniServer(){
                //System.out.println("starting server");
		aSocket = null;
		
	}
}


