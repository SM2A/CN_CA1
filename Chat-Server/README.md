# Chat Client

*You can read full documentation from [here](../Asset/chat-server.md)*

This is a simple chat client that uses the chat server to send and receive messages.

This chat client uses TCP connection to communicate with the server. TCP connection is alive until
the client disconnects from the server with the `exit` command.

Packet format is as follows:

![Packet Format](../Asset/chat-packet.png)

![Example](../Asset/chat-server.png)

There are 3 commands provided by the client:
- `list`: List all the users in the chat room
- `send <username> <message>`: Send a message to a specific user
- `exit`: Disconnect from the server

You should provide a username as the first argument and server ip and port(IP:PORT) as the second argument when start the client.

After sending each command client will wait for the server to respond.

Client is capable of receiving messages from other users even when waiting for user input. This feature is implemented

client check for any  incoming messages. If there is any message client will print it.

