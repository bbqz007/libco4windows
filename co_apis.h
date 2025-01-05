/*
* experiment and porting tencent/libco to windows using wepoll.
* modifications by bbqz007.

* Copyright (C) 2025, bbqz007, github.com/bbqz007. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/
#ifndef _co_apis_h_
#define _co_apis_h_
#include <winsock2.h>
#include <Ws2tcpip.h>

#ifdef ZPort
#define O_NONBLOCK 0x800
#define O_NDELAY        O_NONBLOCK
#define F_DUPFD                0        /* dup */
#define F_GETFD                1        /* get close_on_exec */
#define F_SETFD                2        /* set/clear close_on_exec */
#define F_GETFL                3        /* get file->f_flags */
#define F_SETFL                4        /* set file->f_flags */
#endif
typedef unsigned long int nfds_t;
#define bzero ZeroMemory
namespace libcow
{
    extern int socket(int domain, int type, int protocol);
    extern int fcntl(int fildes, int cmd, ...);
    extern int poll(struct pollfd fds[], nfds_t nfds, int timeout);
    extern int connect(int fd, const struct sockaddr *address, socklen_t address_len);
    extern int co_accept( int fd, struct sockaddr *addr, socklen_t *len );
    extern ssize_t send(int socket, const void *buffer, size_t length, int flags);
    extern ssize_t recv(int socket, void *buffer, size_t length, int flags);
    inline ssize_t read( int fd, void *buf, size_t nbyte )
    {
        return libcow::recv(fd, buf, nbyte, 0);
    }
    inline ssize_t write( int fd, const void *buf, size_t nbyte )
    {
        return libcow::send(fd, buf, nbyte, 0);
    }
    extern ssize_t await_recv(int socket, void *buffer, size_t length, int flags);
    inline ssize_t await_read( int fd, void *buf, size_t nbyte )
    {
        return libcow::await_recv(fd, buf, nbyte, 0);
    }
    extern int close(int fd);
    extern void co_disable_win32fiber_backend();
};

#endif //_co_apis_h_
