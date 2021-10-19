#ifndef HV_SOCKET_H_
#define HV_SOCKET_H_

#include "hexport.h"
#include "hplatform.h"

#ifdef ENABLE_UDS
    #include <sys/un.h> // import struct sockaddr_un
#endif

#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif

#define LOCALHOST   "127.0.0.1"
#define ANYADDR     "0.0.0.0"

BEGIN_EXTERN_C

HV_INLINE int socket_errno() {
#ifdef OS_WIN
    return WSAGetLastError();
#else
    return errno;
#endif
}
HV_EXPORT const char* socket_strerror(int err);

#ifdef OS_WIN

typedef int socklen_t;

HV_INLINE int blocking(int sockfd) {
    unsigned long nb = 0;
    return ioctlsocket(sockfd, FIONBIO, &nb);
}
HV_INLINE int nonblocking(int sockfd) {
    unsigned long nb = 1;
    return ioctlsocket(sockfd, FIONBIO, &nb);
}

#ifndef poll
#define poll        WSAPoll
#endif

#undef  EAGAIN
#define EAGAIN      WSAEWOULDBLOCK

#undef  EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS

#undef  ENOTSOCK
#define ENOTSOCK    WSAENOTSOCK

#undef  EMSGSIZE
#define EMSGSIZE    WSAEMSGSIZE

#else

#define blocking(s)     fcntl(s, F_SETFL, fcntl(s, F_GETFL) & ~O_NONBLOCK)
#define nonblocking(s)  fcntl(s, F_SETFL, fcntl(s, F_GETFL) |  O_NONBLOCK)

typedef int         SOCKET;
#define INVALID_SOCKET  -1
#define closesocket close

#endif

//-----------------------------sockaddr_u----------------------------------------------
typedef union {
    struct sockaddr     sa;
    struct sockaddr_in  sin;
    struct sockaddr_in6 sin6;
#ifdef ENABLE_UDS
    struct sockaddr_un  sun;
#endif
} sockaddr_u;

// @param host: domain or ip
// @retval 0:succeed
HV_EXPORT int Resolver(const char* host, sockaddr_u* addr);

HV_EXPORT const char* sockaddr_ip(sockaddr_u* addr, char *ip, int len);
HV_EXPORT uint16_t sockaddr_port(sockaddr_u* addr);
HV_EXPORT int sockaddr_set_ip(sockaddr_u* addr, const char* host);
HV_EXPORT void sockaddr_set_port(sockaddr_u* addr, int port);
HV_EXPORT int sockaddr_set_ipport(sockaddr_u* addr, const char* host, int port);
HV_EXPORT socklen_t sockaddr_len(sockaddr_u* addr);
HV_EXPORT const char* sockaddr_str(sockaddr_u* addr, char* buf, int len);

//#define INET_ADDRSTRLEN   16
//#define INET6_ADDRSTRLEN  46
#ifdef ENABLE_UDS
#define SOCKADDR_STRLEN     sizeof(((struct sockaddr_un*)(NULL))->sun_path)
HV_INLINE void sockaddr_set_path(sockaddr_u* addr, const char* path) {
    addr->sa.sa_family = AF_UNIX;
    strncpy(addr->sun.sun_path, path, sizeof(addr->sun.sun_path));
}
#else
#define SOCKADDR_STRLEN     64 // ipv4:port | [ipv6]:port
#endif

HV_INLINE void sockaddr_print(sockaddr_u* addr) {
    char buf[SOCKADDR_STRLEN] = {0};
    sockaddr_str(addr, buf, sizeof(buf));
    puts(buf);
}

#define SOCKADDR_LEN(addr)      sockaddr_len((sockaddr_u*)addr)
#define SOCKADDR_STR(addr, buf) sockaddr_str((sockaddr_u*)addr, buf, sizeof(buf))
#define SOCKADDR_PRINT(addr)    sockaddr_print((sockaddr_u*)addr)
//=====================================================================================

// socket -> setsockopt -> bind
// @param type: SOCK_STREAM(tcp) SOCK_DGRAM(udp)
// @return sockfd
HV_EXPORT int Bind(int port, const char* host DEFAULT(ANYADDR), int type DEFAULT(SOCK_STREAM));

// Bind -> listen
// @return listenfd
HV_EXPORT int Listen(int port, const char* host DEFAULT(ANYADDR));

// @return connfd
// Resolver -> socket -> nonblocking -> connect
HV_EXPORT int Connect(const char* host, int port, int nonblock DEFAULT(0));
// Connect(host, port, 1)
HV_EXPORT int ConnectNonblock(const char* host, int port);
// Connect(host, port, 1) -> select -> blocking
#define DEFAULT_CONNECT_TIMEOUT 5000 // ms
HV_EXPORT int ConnectTimeout(const char* host, int port, int ms DEFAULT(DEFAULT_CONNECT_TIMEOUT));

#ifdef ENABLE_UDS
HV_EXPORT int BindUnix(const char* path, int type DEFAULT(SOCK_STREAM));
HV_EXPORT int ListenUnix(const char* path);
HV_EXPORT int ConnectUnix(const char* path, int nonblock DEFAULT(0));
HV_EXPORT int ConnectUnixNonblock(const char* path);
HV_EXPORT int ConnectUnixTimeout(const char* path, int ms DEFAULT(DEFAULT_CONNECT_TIMEOUT));
#endif

// Just implement Socketpair(AF_INET, SOCK_STREAM, 0, sv);
HV_EXPORT int Socketpair(int family, int type, int protocol, int sv[2]);

HV_INLINE int tcp_nodelay(int sockfd, int on DEFAULT(1)) {
    return setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (const char*)&on, sizeof(int));
}

HV_INLINE int tcp_nopush(int sockfd, int on DEFAULT(1)) {
#ifdef TCP_NOPUSH
    return setsockopt(sockfd, IPPROTO_TCP, TCP_NOPUSH, (const char*)&on, sizeof(int));
#elif defined(TCP_CORK)
    return setsockopt(sockfd, IPPROTO_TCP, TCP_CORK, (const char*)&on, sizeof(int));
#else
    return -10;
#endif
}

HV_INLINE int tcp_keepalive(int sockfd, int on DEFAULT(1), int delay DEFAULT(60)) {
    if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&on, sizeof(int)) != 0) {
        return socket_errno();
    }

#ifdef TCP_KEEPALIVE
    return setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPALIVE, (const char*)&delay, sizeof(int));
#elif defined(TCP_KEEPIDLE)
    // TCP_KEEPIDLE     => tcp_keepalive_time
    // TCP_KEEPCNT      => tcp_keepalive_probes
    // TCP_KEEPINTVL    => tcp_keepalive_intvl
    return setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, (const char*)&delay, sizeof(int));
#else
    return 0;
#endif
}

HV_INLINE int udp_broadcast(int sockfd, int on DEFAULT(1)) {
    return setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof(int));
}

// send timeout
HV_INLINE int so_sndtimeo(int sockfd, int timeout) {
#ifdef OS_WIN
    return setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(int));
#else
    struct timeval tv = {timeout/1000, (timeout%1000)*1000};
    return setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
#endif
}

// recv timeout
HV_INLINE int so_rcvtimeo(int sockfd, int timeout) {
#ifdef OS_WIN
    return setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(int));
#else
    struct timeval tv = {timeout/1000, (timeout%1000)*1000};
    return setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
#endif
}

END_EXTERN_C

#endif // HV_SOCKET_H_
