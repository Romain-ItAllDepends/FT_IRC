# FT_IRC

FT_IRC is an IRC server written in C++98 from scratch. It implements the core IRC protocol, handles multiple simultaneous clients using non-blocking sockets and poll(), and supports channels, operators, invitations, authentication and channel modes.

---

## Some ressources

- [Building a Small IRC Server - Medium](https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9)
- [A Comprehensive Guide to ft_irc](https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/)
- [Example ft_irc implementation](https://github.com/marineks/Ft_irc) (His README give many important informations)

## How it works

[Capture vidéo du 2026-08-03 17-40-35.webm](https://github.com/user-attachments/assets/91b89ebe-ac3e-47f1-9be5-a0d03df8a43d)

## Features

- Non-blocking TCP server
- Multiple simultaneous clients
- IRC authentication (PASS, NICK, USER)
- Channels
- Private messages
- Channel operators
- INVITE mode
- TOPIC
- KICK
- Password protected channels
- User limit

## Challenges

Implementing an IRC server required solving several networking problems:

- managing multiple clients with poll()
- handling partial TCP packets
- parsing IRC messages correctly
- synchronizing channel state
- managing operator permissions
- implementing IRC modes without blocking

## References

- RFC 1459
- RFC 2812
- man7.org
- The Open Group

---

## What I learned

This project gave me practical experience with low-level network programming in C++98. Building an IRC server from scratch helped me better understand TCP communication, non-blocking sockets, event-driven programming with poll(), and the challenges of implementing a real-world protocol.

Working in a team also taught me how to split responsibilities, collaborate through Git, review each other's code, and integrate features into a shared codebase.

<details>
<summary>Functions used</summary>

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

</details>
