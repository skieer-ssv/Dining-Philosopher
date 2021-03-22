# Dining-Philosopher
This is a c program using multithreading and semaphores to solve classic dining philosophers problem.

Note: pthread functionality is used for threading in this program which is natively available in linux.

Additional features:
1. Introduced a three course meals for the philosophers
2. When the number of philosophers exceed the number of seats they will be served on First Come First Serve basis in groups of assigned number of seats
