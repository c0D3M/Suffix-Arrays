#include <bits/stdc++.h>
#include <algorithm>

using namespace std;
#define MAX_SIZE 63 /* {$,0-9,A-Z,a-z}*/

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

class SuffixArray
{
    string str;
    const char * c_str;
    int len;
    vector<int> order;
    vector<int> clas;
    vector<int> lcp;
    string bwt;

    void buildSuffixArray();
    int findRange(char c);
    vector<int> SortDouble(int l,  vector<int> &order,  vector<int> &clas);
    vector<int> ComputeClass( vector<int> &order,  vector<int> &clas, int l);
    void STFromSA(void);
    void display_suffix_tree_edge(struct SuffixTreeNode* s);
    struct SuffixTreeNode * CreateNewLeaf(struct SuffixTreeNode * n, int suffix);
    struct SuffixTreeNode * BreakEdge(struct SuffixTreeNode * node, int start, int offset);
public:
    SuffixArray(string s);
    vector<int> getSuffixArray();
    vector<int> getLCPArray();
    string getBWT();
    void printSuffixs();
    void printSuffixTree();
    void searchPattern(string pattern, int start, int end);
};
SuffixArray::SuffixArray(string str)
{

	//Append $ if not present
	if(str.at(str.size()-1)!='$')
	    str.append("$");
	this->str = str;
	len = str.size();
	this->c_str = this->str.c_str();
	order.resize(len);
	clas.resize(len);
	lcp.resize(len);
	bwt.resize(len);
}
int SuffixArray::findRange(char c)
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
vector<int> SuffixArray::SortDouble(int l,  vector<int> &order,  vector<int> &clas)
{
    vector<int>newOrder;
    newOrder.resize(len);
    vector<int>count(len, 0);

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
vector<int> SuffixArray::ComputeClass(vector<int> &order,  vector<int> &clas, int l)
{
    vector<int>newClass(len) ;
    newClass[order[0]] = 0;
    for (int i =1; i<=len-1;i++)
    {
        // Compare first half
        if(clas[order[i]]!= clas[order[i-1]])
            newClass[order[i]] = newClass[order[i-1]] + 1;
        else
        {
            // Comapre second half
            if(clas[(order[i]+l)%len]  !=  clas[(order[i-1]+l)%len])
                newClass[order[i]] = newClass[order[i-1]] + 1;
            else
                newClass[order[i]] = newClass[order[i-1]] ;
        }
    }
    return newClass;
}
void SuffixArray::buildSuffixArray()
{
    // Initialization phase:
    // First count number of occurrence of each different literals.
    int count[MAX_SIZE] ={0};

    for (int i = 0; i < len; i++)
    	if (c_str[i] == '$')
    		count[0] = 1;        // `$
    	else
    		count[findRange(c_str[i])]++;

    for (int i=1; i<MAX_SIZE; i++)
            count [i] += count [i-1];
    for (int j = len - 1; j >= 0; j--)
    {
        int c = c_str[j] == '$' ? --count[0] : --count[findRange(c_str[j])];
        order[c] = j;
    }
    clas[order[0]] = 0;
    for (int i = 1; i < len; i++)
        if (c_str[order[i - 1]] != c_str[order[i]])
            clas[order[i]] = clas[order[i - 1]] + 1;
        else
            clas[order[i]] = clas[order[i - 1]];
    // Generalization Phase , here we sort suffix of double length until < |S|
    int l = 1;
    while (l < len)
    {
        order = SortDouble(l, order, clas);
        clas = ComputeClass(order, clas, l);
        l = 2 * l;
    }
}
vector<int> SuffixArray::getSuffixArray()
{
    buildSuffixArray();
    return order;
}
void SuffixArray::printSuffixs()
{
    buildSuffixArray();
    for (int i = 0; i < len; i++) // start with i=1 to skip $
        printf("%.*s\n", len - order[i] + 1, (c_str + order[i])); //To print $ as well , start i as 0
}
struct SuffixTreeNode * SuffixArray::CreateNewLeaf(struct SuffixTreeNode * n, int suffix)
{
    struct SuffixTreeNode * l = new struct SuffixTreeNode;
    l->parent = n;
    l->depth = len-suffix;
    l->start = suffix+n->depth;
    l->end = len-1;
    n->children[c_str[l->start]] = l;
    return l;
}
struct SuffixTreeNode * SuffixArray::BreakEdge(struct SuffixTreeNode * node, int start, int offset)
{
    char startChar = c_str[start];
    char midChar = c_str[start + offset];

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
void SuffixArray::display_suffix_tree_edge(struct SuffixTreeNode* s)
{
#if 1
    std::stack<struct SuffixTreeNode*> st;
    st.push(s);
    while(!st.empty())
    {
        struct SuffixTreeNode* i = st.top();
        st.pop();
        printf("%d %d\n", i->start, i->end+1);
        std::map<char, struct SuffixTreeNode* >::reverse_iterator it;
        for (it = i->children.rbegin(); it != i->children.rend(); it++)
        st.push(it->second);
    }
#else //Recursive procedure
    printf("%.*s\n", s->end-s->start+1, c_str+s->start);
    for (auto &i:s->children)
        display_suffix_tree_edge(i.second);
#endif
}
void SuffixArray::STFromSA(void)
{
    struct SuffixTreeNode * root = new struct SuffixTreeNode;
    int lcpPrev = 0;
    struct SuffixTreeNode * curNode = root;
    for (int i = 0; i<= len -1; i++)
    {
        int suffix = order[i];
        while(curNode->depth > lcpPrev)
            curNode = curNode->parent;
        if(curNode->depth == lcpPrev)
            curNode = CreateNewLeaf(curNode, suffix);
        else
        {
            int edgeStart = order[i-1] + curNode->depth;
            int offset = lcpPrev - curNode->depth;
            struct SuffixTreeNode * midNode = BreakEdge(curNode, edgeStart, offset);
            curNode = CreateNewLeaf(midNode, suffix);
        }
        if(i<len-1)
            lcpPrev = lcp[i];
    }
    for (auto &i:root->children)
        display_suffix_tree_edge(i.second);
}
vector<int> SuffixArray::getLCPArray()
{
    int rank[len];
    for (int i = 0; i < len; i++)
        rank[order[i]] = i;

    int lastLCP = 0;
    for (int i = 0; i < len; i++)
    {

        if (rank[i] == len - 1)
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
        int nextSuffix = order[rank[i] + 1];

        int matchIndex = lastLCP;
        while ((currentSuffix + matchIndex < len - 1)
                && (nextSuffix + matchIndex < len - 1)
                && (c_str[currentSuffix + matchIndex]
                        == c_str[nextSuffix + matchIndex]))
        {
            matchIndex++;
            lastLCP++;
        }
        lcp[rank[i]] = lastLCP;
        if (lastLCP > 0)
            lastLCP--;
    }
    return lcp;
}
string SuffixArray::getBWT()
{
    for (int i = 0; i < len; i++)
    {
        int p = (order[i] - 1 + len) % len;
        bwt.push_back(c_str[p]);
    }
    return bwt;
}
void SuffixArray::printSuffixTree(void)
{
    STFromSA();
}
void SuffixArray::searchPattern(string pattern, int start, int end)
{
    int mid = (start+end)/2;
    if (start != end)
    {
        if (start < mid)
            searchPattern(pattern, start, mid - 1 );
        searchPattern(pattern, mid + 1, end);
    }
    if (((len - order[mid]) >= pattern.size())
            && c_str[order[mid]] == pattern.at(0))
    {
        if (!strncmp(c_str + order[mid], pattern.c_str(), pattern.size()))
            std::cout << order[mid] << " ";
    }

}
int main(int argc, char *argv[])
{
    string str;
    cin >> str;
    SuffixArray s(str);
    s.getSuffixArray();
    s.getLCPArray();
    cout<<str<<endl;
    s.printSuffixTree();

#if 0
    //s.printSuffixs();
    int n;
    cin >> n;
    for (int i=0; i< n ; i++)
    {
        string pattern;
        cin >> pattern;
        s.searchPattern(pattern, 0, str.size());
    }
#endif
}
