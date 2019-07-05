#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cstring>
#define MAX_SIZE 63 /* {$,0-9,A-Z,a-z}*/
int findRange(char c)
{
	int ret =0;
	if(c>='a'&& c<='z')
		ret = 36 + (c-'a');
	else if(c>='A' && c<='Z')
		ret = 11 + (c-'A');
	else
		ret = 1+(c-'0');
	ret++;
	return ret;
}
int * SortDouble(const char * string, int l,  int *order,  int * clas)
{
	int len = strlen(string);
	int *newOrder = (int*)malloc(sizeof(int)*len);
	int *count = (int*)malloc(sizeof(int)*len);
	memset(count, 0, sizeof(int)*len);

	for (int i=0; i<= len -1; i++)
		count[clas[i]]++;

	for (int i=1; i<= len -1; i++)
		count[i] += count[i-1];

	for (int i= len -1; i>=0; i--)
	{
		int before = (order[i]-l+len)%len;
		//Get class index for this
		int c = clas[before];
		count [c] = count[c] -1;//Decrement that count;
		newOrder[count[c]] = before;
	}
	return newOrder;
}
int * ComputeClass( int *order,  int *clas, int l, int str_len)
{
	int * newClass = (int *)malloc(str_len *sizeof(int));
	newClass[order[0]] = 0;
	for (int i =1; i<=str_len-1;i++)
	{
		// Compare first half
		if(clas[order[i]]!= clas[order[i-1]])
			newClass[order[i]] = newClass[order[i-1]] + 1;
		else
		{
			// Comapre second half
			if(clas[(order[i]+l)%str_len]  !=  clas[(order[i-1]+l)%str_len])
				newClass[order[i]] = newClass[order[i-1]] + 1;
			else
				newClass[order[i]] = newClass[order[i-1]] ;
		}
	}
	return newClass;
}
#include <map>
struct SuffixTreeNode
{
	SuffixTreeNode()
	{
		parent = NULL;
		depth = 0;
		start -1;
		end =-1;
	};
	struct SuffixTreeNode *parent;
	std::map<char, struct SuffixTreeNode*> children;
	int depth;
	int start;
	int end;
};
struct SuffixTreeNode * CreateNewLeaf(struct SuffixTreeNode * n, const char * S, int suffix)
{
	struct SuffixTreeNode * l = new struct SuffixTreeNode;
	l->parent = n;
	l->depth = strlen(S)-suffix;
	l->start = suffix+n->depth;
	l->end = strlen(S)-1;
	n->children[S[l->start]] = l;
	return l;
}
struct SuffixTreeNode * BreakEdge(struct SuffixTreeNode * node, const char * S, int start, int offset)
{
	char startChar = S[start];
	char midChar = S[start + offset];
	
	struct SuffixTreeNode * midNode = new SuffixTreeNode;
	midNode->parent = node;
	midNode->depth = node->depth + offset;
	midNode->start = start;
	midNode->end = start+offset -1;
	midNode->children[midChar] = node->children[startChar];
	
	node->children.find(startChar)->second->parent = midNode;
	node->children.find(startChar)->second->start += offset;
	node->children[startChar] = midNode;
	return midNode;
}
void display_suffix_tree_edge(const char *S, struct SuffixTreeNode* s)
{
	printf("%.*s\n", s->end-s->start+1, S+s->start);
	for (auto &i:s->children)
		display_suffix_tree_edge(S, i.second);
}
void STFromSA(const char * S,int * order, int * lcpArray)
{
	struct SuffixTreeNode * root = new struct SuffixTreeNode;
	int lcpPrev = 0;
	struct SuffixTreeNode * curNode = root;
	for (int i = 0; i<= strlen(S) -1; i++)
	{
		int suffix = order[i];
		while(curNode->depth > lcpPrev)
			curNode = curNode->parent;
		if(curNode->depth == lcpPrev)
			curNode = CreateNewLeaf(curNode, S, suffix);
		else
		{
			int edgeStart = order[i-1] + curNode->depth;
			int offset = lcpPrev - curNode->depth;
			struct SuffixTreeNode * midNode = BreakEdge(curNode, S, edgeStart, offset);
			curNode = CreateNewLeaf(midNode, S, suffix);
		}
		if(i<strlen(S)-1)
			lcpPrev = lcpArray[i];
		#if 0
		printf("------- i=%d\n", i);
		for (auto &i:root->children)
			display_suffix_tree_edge(S, i.second);
		printf("------- ----------\n");
		#endif
	}
	for (auto &i:root->children)
		display_suffix_tree_edge(S, i.second);
}	

void computeLCP(const char * string, int *order, int size)
{
    int rank[size], lcp[size] ;
    /*
     * Iterate Suffix array and store the rank for each suffix from that index
     * for example string
     * 0 1 2 3 4 5
     * b a n a n a
     * 5 3 1 0 4 2  -> Suffix Array
     * At index 0 of string suffix is banana  Rank= 3
     * At index 1 of string suffix is anana   Rank= 2
     * At index 2 of string suffix is nana    Rank= 5
     * At index 3 of string suffix is ana     Rank= 1
     * At index 4 of string suffix is na      Rank= 4
     * At index 5 of string suffix is a       Rank= 0
     *
     * So just iterate 0 to len -1 and Rank[Suffix[i]] = i
     */
    for(int i=0; i<size; i++)
        rank[order[i]] = i;

    int lastLCP = 0;
    for(int i=0; i<size; i++)
    {

        if(rank[i]==size-1)
        {
            //Last lexiographic suffix , its LCP will always be 0
            lcp[rank[i]] = 0;
            continue;
        }
        /*
         * Start with suffix at this index , find what will be the next suffix after it (in lex order)
         * For this rank[i]+1 and index in SA , you will get string index where next suffix begin.
         * Start matching AFTER lastLCP
         *
         */
        int currentSuffix = i;
        int nextSuffix = order[rank[i]+1];

        int matchIndex = lastLCP;
        while((currentSuffix+matchIndex < size-1) && (nextSuffix+matchIndex < size-1) && (string[currentSuffix+matchIndex]==string[nextSuffix+matchIndex]))
        {
            matchIndex++;
            lastLCP++;
        }
        lcp[rank[i]] = lastLCP;
        if(lastLCP>0)
            lastLCP--;
    }
#if 0
    for (int i=0; i<size; i++) //
            printf("%d ", lcp[i]);  //Prints LCP
    printf("\n");
#endif
	STFromSA(string, order, &lcp[0]);
}
int main(int argc, char *argv[])
{
	std::string inp;
	std::cin >> inp;
       const char * str = inp.c_str();	
       int len = inp.length();

	// Initialization phase:
	// First count number of occurence of each different literals.
	int count [MAX_SIZE] = {0};
	for (int i=0; i<len; i++)
		if(str[i]=='$')
			count [0] = 1;// `$
		else
			count [findRange(str[i]) ] ++;
	/* Now compute the sum
	 We need to see where the next literals ends when array is sorted , for that we need to know where the previous one ends
	 For example for 'ababaa$' , sorted suffix of len=1 would be $aaaabb
	 Counting sort gives
	 $ = 1
	 a = 4
	 b = 2
	 Now 'a' will ends at 5 because `$` ends at 1 + occurence of a count 2 = 4, similar b ends at 7 because a ends at 5 + occurence of b count 2 = 7

	 1 2 3 4 5 6 7
	 $ a a a a b b ->sorted
	 counting sort array after adding previou
	 $ = 1
	 a = 5
	 b = 7
	*/
	for (int i=1; i<MAX_SIZE; i++)
		count [i] += count [i-1];
	 int * order = ( int *) malloc(sizeof( int) * len);
	/* len = 7
	 * 0 1 2 3 4 5 6
	   a b a b a a $ -> unsorted

        j s c             0 1 2 3 4 5 6
	   |6|$|0|   order = [6, , , , , , ]
	   |5|a|4|   order = [6, , , ,5, , ]
	   |4|a|3|   order = [6, , ,4,5, , ]
	   |3|b|6|   order = [6, , ,4,5, ,3]
	   |2|a|2|   order = [6, ,2,4,5, ,3]
	   |1|b|5|   order = [6, ,2,4,5,5,3]
	   |0|a|1|   order = [6,0,2,4,5,1,3]
	 */
	for (int j = len -1; j>=0; j--)
	{
		int c = str[j]=='$'?--count[0]:--count[findRange(str[j])];
		order [c] = j;
	}
	// Order contains [6, 0, 2, 4, 5, 1, 3] Index of suffix start of length 1 in sorted order

	// Compute clas , this is basically different type of literals that exists for ababaa$ , suffx len 1 , literals are $, a, b
	 int * clas = ( int *) malloc(sizeof( int) * len);
	/*
	 *  j o s p             0 1 2 3 4 5 6                 j = loop, o = order[j], s = string[order[j]], p=previous string
	   |1|0|a|$|   clas	 = [1, , , , , ,0]
	   |2|2|a|a|   clas  = [1, ,1, , , ,0]
	   |3|4|a|a|   clas  = [1, ,1, ,1, ,0]
	   |4|5|a|a|   clas  = [1, ,1, ,1,1,0]
	   |5|1|b|a|   clas  = [1,2,1, ,1,1,0]
	   |6|3|b|b|   clas  = [1,2,1,2,1,1,0]
	 *
	 */
	clas[order[0]] = 0;
	for (int i=1; i<len; i++)
		if(str[order[i-1]]!= str[order[i]])
			clas[order[i]] = clas[order[i-1]]+ 1;
		else
			clas[order[i]] = clas[order[i-1]];

	// Generalistion Phase , here we sort suffix of double length until < |S|
	int l = 1;
	while(l < len)
	{
		int *newOrder = SortDouble(str, l, order, clas);
		int *newClass = ComputeClass(newOrder, clas, l, strlen(str));
		//Free Older order & class memory
		free(order);
		free(clas);
		order = newOrder;
		clas = newClass;
		l = 2*l;
	}
	computeLCP(str, order, len);
	#if 0
	//printf("\n");
	// newOrder is what is final output contains index of all suffix of a given string
	for (int i=0; i<len; i++) // start with i=1 to skip $
	{
		printf("%d ", order[i]);  //Prints Suffix index in string in lexographical order
		printf("%.*s\n", len-order[i]+1, (str+order[i])); //To print $ as well , start i as 0
		//printf("%.*s\n", len-order[i]+1, (argv[1]+order[i])); // Prints suffix in lexographical order
	}
	#endif
    //printf("\n");
	
}
