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

#include "co_routine.h"

#if __cplusplus >= 201103
#include <functional>
#endif

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

/// Extension apis to co_routine.h
int co_getlasterror();
int co_setlasterror(int);
void co_sleepfor(int/**ms*/);

void co_self_set_arg_destroy(void(*)(void*));
void co_self_set_spec_destroy(pthread_key_t, void(*)(void*));

/// the sharestack, i have disable it in libcow.
inline int co_create2(stCoRoutine_t **co, int stack_size, void *(*routine)(void*), void* arg)
{
    stCoRoutineAttr_t attr;
    if (stack_size > 0)
    {
        if (stack_size <= 14)
            stack_size = 1 << (7+stack_size);
        attr.stack_size = stack_size;
    }
    return co_create(co, &attr, routine, arg);
}

#if __cplusplus >= 201103
    inline void co_eventloop_once( stCoEpoll_t *ctx)
    {
        co_eventloop(ctx, [](void*){return -1;}, 0);
    }
    
    inline int co_create(stCoRoutine_t **co, int stack_size, std::function<void*()> functor)
    {
        auto* tmp = new std::function<void*()>;
        *tmp = std::move(functor);
        return ::co_create2(co, stack_size, 
            [](void* pfunctor)->void*{
                auto& other = *(std::function<void*()>*)pfunctor;
                std::function<void*()> functor = std::move(other);
                co_self_set_arg_destroy([](void* arg){
                    delete (std::function<void*()>*)arg;
                });
                return functor();
            }, (void*)tmp);
    }
    
    inline int co_create(stCoRoutine_t **co, int stack_size, std::function<void*(void*)> functor, void* arg, void(*destroy)(void*) = 0)
    {
        struct paras_t {
            std::function<void*(void*)> functor;
            void* arg = 0;
            void(*destroy)(void*) = 0;
        } ;
        auto* paras = new paras_t();
        paras->functor = std::move(functor);
        paras->arg = arg;
        paras->destroy = destroy;
        return ::co_create2(co, stack_size, 
            [](void* paras)->void*{
                paras_t* paras2 = (paras_t*)paras;
                std::function<void*(void*)> functor = std::move(paras2->functor);
                co_self_set_arg_destroy([](void* paras){
                    paras_t* paras2 = (paras_t*)paras;
                    if (paras2->destroy)
                        paras2->destroy(paras2->arg);
                    delete paras2;
                });
                return functor(paras2->arg);
            }, (void*)paras);
    }
#endif


#endif //_co_apis_h_
