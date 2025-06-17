# My-HTTPS-Server
My own HTTP/S Server built from scratch using low level system sockets and OpenSSL. Written in C/C++.

### Features
* Uses system sockets, only required system libraries are used, no additional I/O abstractions.
* Uses C like data structres where efficiency is important (doesn't mean it's fast).
* Custom HTTP parsing, no libraries.
* Listens for both HTTP and HTTPS connections on different ports.
* Automatically redirects from HTTP to HTTPS.
* Serves files in a `site/` directory.


### Why build this?
This project is purely educational. It's by no means ready for production. It's definitely slower and much less secure than any mainstream webserver. I wrote this to get a good grasp of how web server, http, tls, sockets etc. work. As such, this project was definitely successful.
