import java.io.*;
import java.net.*;

public class EchoClient {
    public static void main(String[] args) throws IOException {
        String host = "127.0.0.1"; 
        int port = 12345;         

        try (
            Socket socket = new Socket(host, port);
            BufferedReader console = new BufferedReader(new InputStreamReader(System.in));
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true)
        ) {
            System.out.println("Connected to server. Type messages to send:");

            String userInput;
            while ((userInput = console.readLine()) != null) {
                out.println(userInput);              
                System.out.println("Server: " + in.readLine()); 
            }
        }
    }
}