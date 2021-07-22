### Dining-Philosopher
This is a c program using multithreading and semaphores to solve classic dining philosophers problem.

## To Compile in Linux:
``` 
gcc -pthread dining.c 
./a.out
```

Note: pthread functionality is used for threading in this program which is natively available in linux OS.

Additional features:

1. When the number of philosophers exceed the number of seats they will be served on First Come First Serve basis in groups of assigned number of seats.


