# Homework #4
Do not cheat and good luck!

It will be great (and useful for you) if you will document your work in this README and write a sort of documentation for your homework. We may give few extra credits for good documentation.


Part 1:

When we are executing application X through fork and execvp, ctrl-c will only terminate the child process.
Since I implemented a handler for SIGINT signal, when the parent process receives SIGINT, it will send the signal directy to its child through kill(). If there is no child process, the default handler will be executed and the parent proceess will terminate.

You may execute part1 by typing make, then ./part1. 

Part 2:

Besides the short description I put before each function in cse320_functions.h, one thing I want to mention is that I never call free() and fclose() in cse320_free() and cse320_fclose(). Since I want to avoid situation which the same address is being reused and creating duplicates inside the addr_in_use array or the files_in_use array. We know users must call cse320_clean() before exiting the application. Therefore, free() and fclose() are called in cse320_clean() to avoid memory leak.
In cse320_free() and cse320_fclose(), only ref_count is being handled, and exits if any error occurs.
 
//(Professor said that this is okay on Piazza)

In order to use part2 functions, you need to put #include "cse320_functions.h" in your file and compile cse320_functions.c with your own c file. Don't forget to put -static and -lpthread.

Part 3: 

User must first call cse320_settimer(int t). User can call this function again when they want to modify the time.
What this fuction does is calling setitimer(), so at each expiration, a SIGALRM signal is generated. 
In cse320_fork(), we set cse320_reap() as the handler for SIGALRM.

Therefore, when we call cse320_fork(), we will never have to worry about reaping children, since we will receive SIGALRM every t seconds. And cse320_reap() will be called since it is the handler for SIGALRM. We will be reaping children every t seconds. 

