import java.io.*;
import java.net.*;
import java.util.concurrent.atomic.AtomicInteger;

public class EchoServer {
    public static void main(String[] args) throws IOException {
        int port = 12345;
        AtomicInteger clientCount = new AtomicInteger(0);

        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("Echo Server is running on port " + port);

            while (true) {
                Socket clientSocket = serverSocket.accept();

                // Assign a unique ID for each new client
                int clientId = clientCount.incrementAndGet();
                String clientInfo = "Client-" + clientId + " (" + clientSocket.getInetAddress().getHostAddress() + ":" + clientSocket.getPort() + ")";

                System.out.println(clientInfo + " connected.");

                // Handle client in a new thread
                new Thread(() -> {
                    try (Socket sock = clientSocket;
                         BufferedReader in = new BufferedReader(new InputStreamReader(sock.getInputStream()));
                         PrintWriter out = new PrintWriter(sock.getOutputStream(), true)) {

                        String message;
                        while ((message = in.readLine()) != null) {
                            System.out.println("📨 Message from " + clientInfo + ": " + message);
                            out.println("Echo: " + message);
                        }

                    } catch (IOException e) {
                        System.out.println(clientInfo + " disconnected.");
                    }
                }).start();
            }
        }
    }
}
