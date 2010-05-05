/* \file WiSniServer.java
 * \brief Handles receiving packets from the clients and updates the linked list with infomation
 * \ingroup javaserver
 */
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
         * \fn public void run()
         * \ingroup javaserver
         * \brief the threads run() method receives datagram packets and adds them to the linked list
         */
        public void run(){
            try{
 			aSocket = new DatagramSocket(5316);
			byte[] buffer = new byte[1000];

			while (true){
				DatagramPacket request = new DatagramPacket(buffer,buffer.length);
				aSocket.receive(request);
                                addClientToList(request);
				
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
         * \fn  public static void addClientToList(DatagramPacket d)
         * \ingroup javaserver
         * \brief adds a packet to a linked list
         */
        public static void addClientToList(DatagramPacket d){
         
            clientList.add(d);
        }
        /*
         * \fn public static LinkedList getClientList()
         * \ingroup javaserver
         * \brief returns a linkedlist of the datagram packets
         */
        public static LinkedList getClientList(){
            return clientList;
        }
        /*
         * \fn WiSniServer()
         * \brief constructor for the server
         */
        WiSniServer(){
                
		aSocket = null;
		
	}
}


