# Sockets in Network Communication

This section contains examples related to the use of sockets for network communication in C. Below is a list of the source files with links to their content.

| File Name             | Description | Link |
|-----------------------|-------------|------|
| rotn_server.c         | A concurrent TCP server on port 5001 that sends back each received line obfuscated with ROT3 | [View](https://github.com/alainlebret/os/blob/master/exemplier/7_sockets/src/rotn_server.c) |
| test_sockets.c        | Opens 8 sockets (Unix/Internet, stream/datagram/raw) and waits for Ctrl-C, to observe them with `lsof -p <PID>`; raw sockets need root | [View](https://github.com/alainlebret/os/blob/master/exemplier/7_sockets/src/test_sockets.c) |
| testbind1.c           | Demonstrates creating and binding a TCP/IP stream socket | [View](https://github.com/alainlebret/os/blob/master/exemplier/7_sockets/src/testbind1.c) |
| testbind2.c           | Demonstrates creating and binding a TCP/IP stream socket to a specific host (resolved with `getaddrinfo()`, port 5001) | [View](https://github.com/alainlebret/os/blob/master/exemplier/7_sockets/src/testbind2.c) |
| resolve_name.c        | Demonstrates resolving a hostname to its IP address(es) using `getaddrinfo()` (formerly `testgethostbyname.c`: `gethostbyname()` is obsolete) | [View](https://github.com/alainlebret/os/blob/master/exemplier/7_sockets/src/resolve_name.c) |
| echo_server_iterative.c | Running example: iterative TCP echo server on port 5001 (course, chapter « Sockets ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/7_sockets/src/echo_server_iterative.c) |
| echo_server_concurrent.c | Running example: concurrent TCP echo server (one child per client, children reaped on SIGCHLD) (course, chapter « Sockets ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/7_sockets/src/echo_server_concurrent.c) |
| echo_server_poll.c | TCP echo server serving all its clients in a single process with `poll()` (course, chapter « Sockets ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/7_sockets/src/echo_server_poll.c) |
| echo_client.c | Running example: TCP client of the echo server, reads each echo up to the end of line (course, chapter « Sockets ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/7_sockets/src/echo_client.c) |
| udp_echo_server.c | Running example: UDP echo receiver on port 5001 (course, chapter « Sockets ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/7_sockets/src/udp_echo_server.c) |
| udp_echo_client.c | Running example: UDP sender that sends a datagram and displays its echo (course, chapter « Sockets ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/7_sockets/src/udp_echo_client.c) |
