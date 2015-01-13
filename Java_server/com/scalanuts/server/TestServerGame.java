package com.scalanuts.server;
import java.io.*;
import java.net.*;

public class TestServerGame {
   public static void main(String h[]) {
      try {
            ServerSocket ss=new ServerSocket(6669);
            Socket s=ss.accept();
            System.out.println("Client Accepted");
            BufferedReader br=new BufferedReader(new InputStreamReader(s.getInputStream()));
            System.out.println(br.readLine());
            PrintWriter wr=new PrintWriter(new OutputStreamWriter(s.getOutputStream()),true);
            wr.println("O Da Detka");
      } catch(Exception e){
            System.out.println(e);
      }
   }
}