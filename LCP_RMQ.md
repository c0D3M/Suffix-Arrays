LCP problem can be converted to RMQ problem as following
* Do Euler Tour and stores the vertex  in array E
* Store height of each vertex during Euler tour in H, Note that consecutive entry in this array will differe by +/- 1
* Store occurence of each vertex in another array R
Problem boils down to finding RMQ in H, which can be done using Sparse Table (ST) <O(n log(n)), O(1)>
can get better if we consider block of size log n /2 , 2n/log n blocks

Consecutive elements in H array will differ at most by +-1.
Understand this??

RMQ to LCA
RMQ problem can be converted to LCA problem and then can be solved using above way.
We use cartesian tree to convert RMQ to LCA

What is Cartesian Tree
Satisfy
* min-Heap or Max Heap property
* an inorder travesal yield the orignal array.




Reference
1. https://www.topcoder.com/community/data-science/data-science-tutorials/range-minimum-query-and-lowest-common-ancestor/
2. 
