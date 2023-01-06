# Web Server

Here we implemented a simple http web server that can server multiple file formats.

By given port we start server on address 127.0.0.1 and listening to the port and wait for a request.
When we get a request we parse the request and check if the file exists.
If the file exists we send the file to the client.
If the file doesn't exist we send a 404 error to the client.

First we extract the request path and check weather the file exists or not.
If given path is `/` we send the `index.html` file to the client. Else we send the file that is requested.

Then we extract the file extension. This is done because we need to send the correct content type to the client.
For example if the file is a `html` file we need to send the content type `text/html` to the client.

After that we send the response to the client. The response is a `HTTP/1.1 200 OK` response.

Finally by calculating content length we send the file to the client by reading the file and send it to request
file descriptor and close connection.
