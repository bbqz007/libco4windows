# libco4windows
porting tencent/libco to windows with wepoll.
short name is libcow.

https://github.com/bbqz007/libco4window

# summary
this is a experiment work. and it is buggy as libco codes. 

can you see a piece fo codes from big software corp.
* env,coctx, only have init(), but not uninit()
* the epoll resources never free by calling FreeEpoll()
* all the codes never call the co_release()
* they who wrote the tencent/libco never thought about how to make everything correctly release and quit.
* like a joke.

at first, i want to port this libco to windows for the compiler which can not use c++20. but during my porting it, i found it is buggy so much. now i just port it to the ability to run on windows. 

and i also found, on windows there is no choice than Fiber api because of the TEB, if the coro implement is base on stackful. the Fiber eat 2m vm at least. thus on windows, stackless std::cocoroutine maybe a better choice.

i have no idea that, if program under the console:subsys, it can run fine only using simple assembly switch. thus i port it to find whether using a user32.dll, then make a decition to use Fiber or not. the Native Fiber preforms so bad you can not believe it.

the tencent/libco is a eventloop framework implemented using stackful coroutines. each coro is equals to a connection object which can be event handler to epoll. each coro has a 4k object and 128k by default private stack. 

the purpose of tencent/libco, maybe hook read api to make the blocking io pragramming codes can performs async on one thread one eventloop. this can reduce much work to change eventloop framework for many third software in your projet. out of this usage, it seem to not be a coro library what you consider. 

in thery use cases, the coro is not a small piece task. the coro is actually an object though all the program life. so they never care how and when to release, and the a lot of coro creating and releasing. even that they seems never do release.
