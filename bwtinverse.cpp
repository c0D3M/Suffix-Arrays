#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

string InverseBWT(const string& bwt)
{
    std::string text;
    text.resize(bwt.size());

    // write your code here
    text[0] = '$'; // This will always be first characters
    size_t pos = 1;
    vector<int> front_count;
    front_count.push_back(1); // count of `$`

    size_t t = std::count(bwt.begin(), bwt.end(), 'A');
    std::fill(text.begin() + pos, text.begin() + pos + t, 'A');
    pos += t;

    t = std::count(bwt.begin(), bwt.end(), 'C');
    std::fill(text.begin() + pos, text.begin() + pos + t, 'C');
    pos += t;

    t = std::count(bwt.begin(), bwt.end(), 'G');
    std::fill(text.begin() + pos, text.begin() + pos + t, 'G');
    pos += t;

    t = std::count(bwt.begin(), bwt.end(), 'T');
    std::fill(text.begin() + pos, text.begin() + pos + t, 'T');
    pos += t;

    // For first column we store the index of each string, BWT= TTCCTAACG$A , first column = $AAACCCGTTT
    // first = $= [0] , A=[0,1,2] , C=[3,4,5] G= [6], T=[7,8,9]
    std::map<char, vector<int> > first;
    for (int i = 0; i < bwt.size(); i++)
        first[text[i]].push_back(i);

    //Store count of each character in last column like TTCCTAACG$A is stored as [0,1,0,1,2,0,1,2,0,0,2]
    std::map<char, int> m;
    vector<int> last;
    m['A'] = 0;
    m['C'] = 0;
    m['G'] = 0;
    m['T'] = 0;
    m['$'] = 0;
    for (int i = 0; i < bwt.size(); i++)
        last.push_back(m[bwt[i]]++);

    int index = 0;
    std::string res;
    res.resize(bwt.size());
    int store = bwt.size();
    res[--store] = '$';
    while (bwt[index] != '$')
    {
        res[--store] = (bwt[index]);
        int count = last[index];
        index = first[bwt[index]][count];
    }
    return res;
}

int main()
{
    string bwt;
    cin >> bwt;
    cout << InverseBWT(bwt) << endl;
    return 0;
}
