# FTP Server/Client

*You can download full PDF documentation in persian from [here](../Asset/ftp.pdf)
and read it online from here[here](../Asset/ftp.md). I do recommend take a look.*

In this subproject (which actually was a full project) we implemented an FTP server and client.
The server is able to handle multiple clients at the same time.
The client is able to connect to the server and download files from it.

Server handles multiple clients at the same time.
It uses `select` system call to handle multiple clients at the same time.

Client and server communicate in two different tcp connections.
First connection is command channel which only commands are passed
and second connection is data channel which is used to transfer files and respond to commands with large data.

Upon every new connection, server creates new instance of user and save it into list of new users which are unknown.
The first stage fore user is to enter username and after entering username user enters second stage which is password.
After this stage if username and password are correct,
user enters third stage which is logged in stage and can send other commands.
