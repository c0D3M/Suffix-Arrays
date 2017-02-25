# Suffix Arrays
I am sharing my experince with various Suffix Array Construction Algorithms(SACA).

# Introduction
Few years back I stumbled upon this nice data structure Suffix Tree. I must admit its construction is a bit complex.
While studying more about it, I read about Suffix Array.
There are so many algorithm being developed considering vast application of Suffix Array in patter matching, compression, Genome Sequencing.These Algorithm can be categorised into broadly three category.

* Prefix Doubling  - Manaber -meyers/Larsson-Sadakane
* Recursive Algorithm - DC3, KA Algroithm, NZC Algorithm
* Induced Sorting - S Algorith/MF Algorithm

# Prefix Doubling Algorithm
Key idea is if you take cyclic rotation at each position of a given string and sort it and remove everything after $ from each string you will have a sorted suffix.
Lets see this with an example 
String = ababaa
Append $ at the end this will help in indetifying each uique suffix, for example try to make suffix tree for papa
it will be 
a
pa
apa
papa

a & apa share common prefix a , so suffix 'a' is invisible, to fix this we add $
now suffixs are
$
a$
pa$
apa$
papa$

So our input string is ababaa$
String starting at index.

index | string|index | string|index | string
------|-------|------|-------|------|-------
0| ababaa$       |6| $ababaa          |6| $
1| babaa$a       |5| a$ababa          |5| a$
2| abaa$ab       |4| aa$abab          |4| aa$
3| baa$aba       |2| abaa$ab          |2| abaa$ 
4| aa$abab       |0| ababaa$          |0| ababaa$
5| a$ababa       |3| baa$aba          |3| baa$
6| $ababaa       |1| babaa$a          |1| babaa$

So final suffix array would be [6, 5, 4, 2, 0, 3, 1]
Now how to convert this into an efficient algorithm.
We will first sort 1 characters and then at each iterator double up, so total iterations would be log(n) , in each iterator we will do 'n' cycle to calculate 'order' and 'rank'. SO overall complexity would be O(n log(n))

order: index of suffix in lexigoraphic order in this scan cycle. 
rank:  different type of string encoruted in this scan cycle.

Lets start with 1
ababaa$
Counting sort       Partial Sum 
$ -> 1               $ -> 1
a -> 4               a -> 5    
b -> 2               b -> 7
Scan from backward 
Each count[literal]-- 
order [count[literal]-- ] = pos

ababaa$

i  |str |Count|Order
---|----|-----|-----
6  |  $ |  0  |6, , , , , ,
5  |  a |  4  |6, , , ,5, ,
4  |  a |  3  |6, , ,4,5, , ,
3  |  b |  6  |6, , ,4,5, ,3
2  |  a |  2  |6, ,2,4,5, ,3
1  |  b |  5  |6, ,2,4,5,1,3
0  |  a |  1  |6,0,2,4,5,1,3 

We scan the inpur string from backward and we know based on couting sort that their is 1 $, 4 a and 2 b , so a is going to end  (count of $) + count of a i.e. at 5 , similarly b will end at 7
One more thing is stable sort i.e. if we encounter 2 a's order will be decided based on order of occurence in string.
So we have 4 a's at index 0,2,4,5 and in suffix array they come in same order.
So this is how we have calculated order when taken 1 at a time.

rank array : each distinguised elemnt occur in order 
rank = [0,1,1,1,1,2,2] because there are only 3 different elements i.e. $, a, b





Complete taxonomy of these algorithm can be found [here](http://www.cas.mcmaster.ca/~bill/best/algorithms/07Taxonomy.pdf)


# DC3 Algorithm
# S[Seward] Algorithm
# KA Algorithm / NZC Algorithm
# Burrows Wheelers Transform
   * BWT
      
      BWT are helpful in converting and inpur string containing **repeated subsequence into runs** and then run length encoding can be         used to allow compression.
      For a given input string, sort all cyclic rotation in lexigoraphical order and the last column give you BWT.
      For example
      ababaa$
      Refer the table above , BWT(ababaa$)   = aabb$aa
      Actually they are one previous character in suffix array.
      Trick is in English language any common word like **and** when suffix sorted its previous characters(in last column) would be           **a** and that would contribute to run.
   * Inverting
       Lets see if we have BWT can we recover original string.
       
       ##### Naive Way.
       
       As we know BWT is last column in sorted cyclic rotation.We also know that first column is sorted.
       
       $-----a1
       a1-----a2
       a2-----b1      
       a3-----b2
       a4-----$
       b1-----a3
       b2-----a4 
       combine them because we know what preceeds each sorted suffix and then sort
       
       a$       $a---a         a$a        $ab--a       a$ab       $aba--a       a$aba       $abab-a      a$abab      $ababaa
       aa       a$---a         aa$        a$a--a       aa$a       a$ab--a       aa$ab       a$aba-a      aa$aba      a$ababa
       ba       aa---b         baa        aa$--b       baa$       aa$a--b       baa$a       aa$ab-b      baa$ab      aa$abab
       ba Sort->ab---b Join -> bab Sort-->aba--b Join->baba Sort->abaa--b Join->babaa Sort->abaa$-bJoin->babaa$Sort->abaa$ab  
       $a       ab---$         $ab        aba--$       $aba       abab--$       $abab       ababa-$      $ababa      ababaa$ 
       ab       ba---a         aba        baa--a       abaa       baa$--a       abaa$       baa$a-a      abaa$a      baa$aba
       ab       ba---a         aba        bab--a       abab       baba--a       ababa       babaa-a      ababaa      babaa$a 
       
       Keep doing this and eventually we get the input string.
       But the problem is it take lot more memory.
       ##### First last property.
       Occurence of any symbol in first and last column matches.
       Trick is we know that BWT contains symbol one previous that of suffix array (which are in lexigraphical order).
       So start with $ (because we know for sure that this is last symbol in original string).
       Now BWT corresponding to this $ tells what lies before $ so that symbol preceed $ in original string, take that lookup i
       first column and find again the correspoding BWT symbol.
       Repeating this process we spell out original string.
       Take example above
       $ ->a1 -> a2-> b1->a3->b2-> a4->$
       So original string is ababaa$
       
   * Pattern Matching using LF mapping
      Next lets see how BWT is useful in doing pattern matching.
      Key is to sart searching for pattern in backward fashion because BWT stores what lies before any given symbol.
      For example to search **aba** in **ababaaa**
      start with ab**a** and look in first column.
      There are 4 places starts with **a** but only 2 has **last columns as b** (row 2 & 3)
      Selct those b's row and look which of them has last column as **a**ba , both b rows has,
      so **aba** occurs 2 times in original text.
      Problem with this approach is one has to linearly scan the whole last column range to find the symbol.
      This can be solved using Couting filter which store at each row , for each symbol , its count in last column.
      So now while lookin for last column look that the symbol transition range.
      For example 
      
      Fst|Last|$  |a  |b  
      ---|--- |---|---|---
      $  |a   |0  | 0 |0
      a  |a   |0  | 1 |0
      a  |b   |0  | 2 |0
      a  |b   |0  | 2 |1
      a  |$   |0  | 2 |2
      b  |a   |1  | 2 |2
      b  |a   |1  | 3 |2
      X  |X   | 1 | 4 |2
      
      For a in first column b changes from 0 to 2 , that means there are 2 b's in last column.
      Goto B column and then look for 'a' and it changes 2 to 3 that means their is 1 a before a so pattern aba occurs once in string.
           
   * FM-Index
     There are still 3 issues with above way of pattern matching.
     * Scanning in last column can be linear scan and in worst case can be O(n).
       Key idea to fix is to store a tally table for occurence of each symbol in last column.
       For example
       a  |b
       ---|---
       0  | 0
       1  | 0
       2  | 0
       2  | 1
       2  | 2
       3  | 2
       4  | 2
       This basically tells at each row how many **a** and **b** are seen, for example
       row = 3 , a=2 and b=1 i.e. 2 a's and 1 b's are seen.
       
       Now storing this kind of table also can take space, instead we store every 5th or so(checkpoint) entried and infer the requried          value from nearest checkpoint.
       This also solved 2nd issue of saving space.
     * Storing count array for each symbol take O(n) * |alphabet|
     * This kind of pattern matching didnt tell posiition at which the match occurs.
       Use Suffix Array but store suffix's at every other 'k' position with respect to n not suffix Array.
       This is important because storing every kth suffix wrt to T give us a constant time to k to find suffix array t didnt 
       available at a given row position.We will use LF mapping to find the row which has Suffix Array value present.
         
       SA(row)= SA(k) + number of LF mapping jumps.
       
       Memory Foot print of FM-Index
       First we store F and L rows,
       F is basically symbol in sorted occurence, just store start index for each symbol.
       L is basically BWT of N , so its size will be O(n)
       For checkpoint we save fraction of tally , lets say we wanted to **a** fraction, so total size will be 
       n * **a** * symbol_size
       
       For SA , since we are storing every kth suffix in T , total size would be
       n * **k**
       
       Reference:
       https://www.youtube.com/watch?v=kvVGj5V65io&t=824s
       https://www.cs.jhu.edu/~langmea/resources/lecture_notes/bwt_and_fm_index.pdf
       
     
   * Wavelet Tree
       During pattern matching we have to search  last column from 0 to start/end of index of first character.
       Like searching **aba** , we first goto first column and find range of a i.e.  1,4
       during in last column we have to find **b**
       rank(1, b) and rank(4, b) , we wanted to find number of **b** in last column from 0 to 1 and 0 to 4.
       We achieved this by using count array and some optimization in earlier section.
       Here we will see alternative approach using wavelet tree to answer these queries in O(1) time
        Broadly these are the steps.
        * Encode first half of string as 0 and second half as 1
        * Group each 0-encoded symbol as left subtree 
        * Group 1-encoded symbol as right subtree
        * Recursively do that.
        
        Lets see how we create Wavelet Tree of BWT **aabb$aa**
        {$,a} -> 0
        {b} -> 1
                                     aabb$aa
                                     0011000
                                     /
                                    /
                                  aa$aa
                                  11011
                                   
                                   aabb$aa
                                   0123456
         Suppose we need to find rank(4,a) , since a is encoded as 0 find rank(0, 4) which would be 2.
         At this level 1, a is encoded as 1 so find rank(1,2), which would be 2.
         Take right child path this will be all a i.e. 1111, do a rank query of rank(1,2) which is 2 hence we 
         rank(4,a) would be 2.
         http://alexbowe.com/wavelet-trees/
         https://github.com/alexbowe/wavelet-paper/blob/master/thesis.pdf
         
   * RRR Data structure.
          http://alexbowe.com/rrr/
          wavelet tree node when stored as RRR sequence can answer rank queries in O(1) time
          

# Practical Application
# Competitive Programming Problems
# Reference
# Suffix Tree using Suffix Array
# Suffix Tree using Ukkonen Algorithm
