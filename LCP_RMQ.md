LCP problem can be converted to RMQ problem as following
* Do Euler Tour and stores the vertex  in array E
* Store level of each vertex during Euler tour in H, Note that consecutive entry in this array will differe by +/- 1 [ because the path will go down or up and this will only change level either by +1 or -1]
* Store occurence of each vertex in another array R
How to find LCA
* First check in array R when these nodes occured during traversal this will be A and B.
* Next you get traversal number , check in array L , the shallow node (the minium level ) going from A to B.
* This will give travesal number , map that in E array. Because if L stores level of that node , E store node number.

Problem boils down to finding RMQ in H, which can be done in multiple ways.
* Using Table lookup <O(n^2), O(1)>
* Using Sparse Table (ST) <O(n log(n)), O(1)>
* <O(n), O(1)> 

Divide array L in block of size log (n) /2. Their will be 2n/log n such blocks  [ n/[log(n)/2] = 2n/log(n)
Convert H array in to H` and further -1 -> 0 and +1 ->1

L = [1,2,3,2,4]
L`= [+1,+1,+1,-1,+1]
L``= [1,1,1,0,1]

Now the trick is to precompute minimum in each block.
For example suppose a block is  00011 , this correspond to a subset of Euler tour i.e. 3 up (where level differe by previous by -1) and 2 down (where level increases).
All possible combination of 00011 between every pair is
s= start index
e= end index 
min = index of minimumum between that pair 
blocksize =5
in-case of tie take right one.

|0|1|2|3|4|
|---|---|---|---|---|
|0|0|0|1|1|

##### P
|s|e|min|
|---|---|---|
|0|0|0|
|0|1|1|
|0|2|2|
|0|3|2|
|0|4|2|
|1|1|1|
|1|2|2|
|1|3|2|
|1|4|2|
|2|2|2|
|2|3|2|
|2|4|2|
|3|3|3|
|3|4|4|
|4|4|4|

so like this store all possible combination for each pattern in H``.

__Storage__:
Number of block is  2n/log(n)
Each block has n*(n+1)/2 combination
So create a 1-D array __T__ of size 2n/log(n) * [blocksize*(blocksize+1)/2] 
and this array can be index using decimal equivalent of block 
for example 00011 is 3 , and we know in each block  there are 15 combination for each decimal equivalent of block.
So in above example array __P__ is stored from location __45__ (decimal equivalent * blocksize i.e. 3*15)
Note even table __P__ shown as array , we will store this also as 1D array.
 So now suppose  you have query what is minimum at 3rd block 2nd index.
 1.Calculate decimal equivalent of 3rd block and reach to index in __T__
 2.Lookup [2,4] , and that gives you minimum.
 
 


RMQ to LCA
RMQ problem can be converted to generalized RMQ problem by first converting it to LCA problem and then can be solved using above way.
We use cartesian tree to convert RMQ to LCA

What is Cartesian Tree
 A tree that can be build from an array which satisfy
* min-Heap or Max Heap property
* an inorder travesal yield the orignal array.



Reference
1. https://www.topcoder.com/community/data-science/data-science-tutorials/range-minimum-query-and-lowest-common-ancestor/
2. 
