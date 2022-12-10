# Knights-Tour-with-Multi-Threading
Given a NxN chess board, and a starting position i, j for a kinght, we find out in what path (if possible) can the knight tour all N^2 squares on the board without visiting a square twice.

We use Warnsdorff's Algorithm to find the tour of the board. In case the score of two squares are tied, we use a randomiser to choose where the knight should travel to.
Since we can find out the cases which do not have a solution without doing any traversal, we can know for sure that the given input has a solution. So, we put the algorithm in a while loop and restart everytime we reach a deadend. We multithread this while loop to have multiple threads searching for a solution at the same time. Even though the algorithm uses a randomiser, the time taken for N=50 stays around 0.01s. 
