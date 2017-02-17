LCP problem can be converted to RMQ problem as following
* Do Euler Tour and stores the vertex  in array E
* Store height of each vertex during Euler tour in H, Note that consecutive entry in this array will differe by +/- 1
* Store occurence of each vertex in another array R
Problem boils down to finding RMQ in H, which can be done in multiple ways.
* Using Table lookup <O(n^2), O(1)>
* Using Sparse Table (ST) <O(n log(n)), O(1)>
* <O(n), O(1)> if we consider block of size log n /2 , 2n/log n blocks and generalize H array , whose consecutive elements differs 
by +-1 1 , because either the euler traversal will go down or up i.e. +1 or -1.

H = [1,2,3,2,4]
H`= [+1,+1,+1,-1,+1]
H``= [1,1,1,0,1]



RMQ to LCA
RMQ problem can be converted to generalized RMQ problem by first converting it to LCA problem and then can be solved using above way.
We use cartesian tree to convert RMQ to LCA

What is Cartesian Tree
Satisfy
* min-Heap or Max Heap property
* an inorder travesal yield the orignal array.




Reference
1. https://www.topcoder.com/community/data-science/data-science-tutorials/range-minimum-query-and-lowest-common-ancestor/
2. 
