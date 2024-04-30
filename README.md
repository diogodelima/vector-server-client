# vector-server-client
Work carried out for the Operating Systems subject.

This project involves a client sending a vector with high dimensions to a server via sockets. The server then filters the vector and sends the filtered result back to the client. The server is designed to handle multiple connections from different clients concurrently using threads, enabling concurrent processing.

Additionally, the server has the capability to partition the filtering task into subvectors and perform the filtering using multiple threads or processes. This behavior is configurable at command.
