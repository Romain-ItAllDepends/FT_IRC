# FT_IRC Project

This project is implemented in **C++ 98** and covers various essential socket and system functions such as `socket`, `close`, `setsockopt`, and many more!

## Some ressources

	(https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9)
	(https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/)
	(https://github.com/marineks/Ft_irc)

## How that work

[Capture vidéo du 2026-08-03 17-40-35.webm](https://github.com/user-attachments/assets/91b89ebe-ac3e-47f1-9be5-a0d03df8a43d)


## Functions used

The project uses the following system calls and library functions (or their equivalents):

- `socket`
- `close`
- `setsockopt`
- `getsockname`
- `getprotobyname`
- `gethostbyname`
- `getaddrinfo`
- `freeaddrinfo`
- `bind`
- `connect`
- `listen`
- `accept`
- `htons`
- `htonl`
- `ntohs`
- `ntohl`
- `inet_addr`
- `inet_ntoa`
- `send`
- `recv`
- `signal`
- `sigaction`
- `lseek`
- `fstat`
- `fcntl`
- `poll`

---

## Function References

Below are the links for detailed documentation and usage examples for each of the key functions used in this project:

### Socket and Communication Functions
- **`socket`**: [GeeksforGeeks - Socket Programming in C++](https://www.geeksforgeeks.org/socket-programming-in-cpp/)
- **`close`**: For file descriptor (`fd`) handling.
- **`setsockopt`**: 
  - [The Open Group - setsockopt](https://pubs.opengroup.org/onlinepubs/009695399/functions/setsockopt.html)
  - [man7.org - setsockopt](https://man7.org/linux/man-pages/man2/setsockopt.2.html)
- **`getsockname`**: [The Open Group - getsockname](https://pubs.opengroup.org/onlinepubs/9699919799/functions/getsockname.html)
- **`getprotobyname`**: [manpages.debian.org - getprotobyname](https://manpages.debian.org/unstable/manpages-fr-dev/getprotobyname.3.fr.html)
- **`gethostbyname`**: [manpages.debian.org - gethostbyname](https://manpages.debian.org/bookworm/manpages-fr-dev/gethostbyname.3.fr.html)
- **`getaddrinfo`**: [man7.org - getaddrinfo](https://man7.org/linux/man-pages/man3/getaddrinfo.3.html)
- **`freeaddrinfo`**: [manpages.debian.org - freeaddrinfo](https://manpages.debian.org/testing/manpages-fr-dev/freeaddrinfo.3.fr.html)

### Socket Management Functions
- **`bind`**: [GeeksforGeeks - bind in C++ STL](https://www.geeksforgeeks.org/working-and-examples-of-bind-in-cpp-stl/)
- **`connect`**: [GeeksforGeeks - Socket Programming in C++](https://www.geeksforgeeks.org/socket-programming-in-cpp/)
- **`listen`**: [man7.org - listen](https://man7.org/linux/man-pages/man2/listen.2.html)
- **`accept`**: [GeeksforGeeks - Socket Programming in C++](https://www.geeksforgeeks.org/socket-programming-in-cpp/)

### Data Conversion and Byte Order Functions
- **`htons`**: [man7.org - htons](https://man7.org/linux/man-pages/man3/htons.3.html)
- **`htonl`**: [man7.org - htonl](https://man7.org/linux/man-pages/man3/htons.3.html)
- **`ntohs`**: [man7.org - ntohs](https://man7.org/linux/man-pages/man3/ntohs.3.html)
- **`ntohl`**: [GNU - Byte Order](https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.5/html_node/Byte-Order.html)
- **`inet_addr`**: [The Open Group - inet_addr](https://pubs.opengroup.org/onlinepubs/7990949875/functions/inet_addr.html)
- **`inet_ntoa`**: [man7.org - inet_ntoa](https://man7.org/linux/man-pages/man3/inet_ntoa.3.html)

### Data Transmission Functions
- **`send`**: [The Open Group - send](https://pubs.opengroup.org/onlinepubs/009604399/functions/send.html)
- **`recv`**: [The Open Group - recv](https://pubs.opengroup.org/onlinepubs/000095399/functions/recv.html)

### Signal Handling Functions
- **`signal`**: [GeeksforGeeks - Signal Handling in C++](https://www.geeksforgeeks.org/signal-handling-in-cpp/)
- **`sigaction`**: [man7.org - sigaction](https://man7.org/linux/man-pages/man2/sigaction.2.html)

### File Management and Control Functions
- **`lseek`**: [man7.org - lseek](https://man7.org/linux/man-pages/man2/lseek.2.html)
- **`fstat`**: [The Open Group - fstat](https://pubs.opengroup.org/onlinepubs/9699919799/functions/fstat.html)
- **`fcntl`**: [The Open Group - fcntl](https://pubs.opengroup.org/onlinepubs/9699969799/functions/fcntl.html)

### Event Polling
- **`poll`**: [man7.org - poll](https://man7.org/linux/man-pages/man2/poll.2.html)

---

## Conclusion

This project involves a wide range of system calls and socket programming techniques, primarily focusing on networking, file handling, and signal processing in C++98. The documentation provided here should help you understand the usage and implementation details of each function used.

For more information on C++ system programming, feel free to refer to the links above!

