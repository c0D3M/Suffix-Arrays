#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

bool isSame(const string &str, int a1, int b1)
{
	if ((str[a1]==str[b1]) && (str[a1+1]==str[b1+1]) && (str[a1+2]==str[b1+2]))
		return true;
	else
		return false;
}
vector<int> DC3(string str, bool isAlpha=false)
{
	int l = str.size();
	int token = isAlpha?'$':'0';
	int offset = isAlpha?'a':'0';
	
	// Modiy string to append $
	str.append(3, token);
	 
	cout <<"Modified String is : "<< str<<"\n"; 
	// Step 1:
	// Generate SA12
	
	// Get 1,2 mod 3 position Suffix Array
	vector<int> SA12;
	for (int i=0 ; i<str.size()-2; i++)
		if(i%3)
			SA12.push_back(i);
	
	//Radix sort
	for (int i = 1; i<= 3; i++)
	{
		vector<queue<int>> bucket(27); // '$' occupy 0th index
		for (int j=0; j< SA12.size(); j++)
			if(str[SA12[j] + 3 -i]==token)
					bucket[0].push(SA12[j]);
				else
					bucket[str[SA12[j] + 3 -i]-offset + 1 ].push(SA12[j]);  // +1 is done to accomodate '$' on index 0 bucket
		
		SA12.clear();
		for (int i =0; i< 27; i++)
			if(bucket[i].size())
				while(!bucket[i].empty())
				{
					SA12.push_back(bucket[i].front());
					bucket[i].pop();
				}
	}
	//Calculate Rank
	vector<int> rank(SA12.size(), 1);
	int next_rank =1 ;
	map<int, int> i2r; // Help in regenarting reverse mapping index i is which rank suffix
	i2r[SA12[0]] = next_rank;
	for (int i=1; i< SA12.size(); i++)
	{
		if(!isSame(str, SA12[i],SA12[i-1]))
			++next_rank;
		i2r[SA12[i]] = next_rank;
		rank[i] = next_rank;
	}

	// Are all rank unique ? if not we have to recurvisely sort rank string
	if(next_rank!=SA12.size())
	{
		string q(SA12.size(), '0');
		// Create a string which has rank of [1 mod 3 || 2 mod 3]
		// For example [1,4,7,10]||[2, 5, 8, 11] , rank of these indices, form a string
		for (int i=0; i< SA12.size(); i++)
		{
			int j =0;
			if((SA12[i] % 3 )==2)// if its 2 mod 3 , it falls in 2nd half hence offset ;
			{
				j = SA12.size()/2;
				if(SA12.size()&1)//147||25 case
					++j;
			}
			int index = j + (SA12[i]/3);
			q[index] = '0' + rank[i];
		}
		vector<int> t_SA12 = DC3(q);//Recursively find SA 
		int t[SA12.size()];
		int i;
		int other_half = (SA12.size()/2);
		if(SA12.size()&1)//147||25 case
			++other_half;
		
		for (i=0 ; i < SA12.size()/2; i++)
		{
			t[i] = 1 +(3*i); // 1 mod n multiple 
			t[i+other_half] = 2 +(3*i); // 2 mod n multiple
		}
		if(SA12.size()&1)//147||25 case
			t[i] = 1 +(3*i);
		SA12.clear();
		for (int i =0; i < t_SA12.size(); i++)
			if(t_SA12[i]<q.size())
				SA12.push_back(t[t_SA12[i]]);
	}
	//i2r get updated after ranks are resolved.
	for (int i =0; i<SA12.size(); i++)
		i2r[SA12[i]] = i+1;
	//For sentiel data i.e. $ or 0 make their rank lowest
	for (int i= l ; i< str.size(); i++)
		i2r[i] = 0;
	
	
	// Step 2:
	// Generate SA0 using SA12
	vector<string> s0;
	map<string, int> m;
	for (int i=0; i< l; i+=3)
	{
		string t; t += str[i];
		s0.push_back(t);
		s0.back() += i2r[i+1]+'0';
		m[s0.back()] = i;
	}
	vector<int> SA0;
	//Radix sort s0
	for (int i = 0; i < 2; ++i)
	{
		SA0.clear();
		vector<queue<int>> bucket(27);
		//bucket.assign(bucket.capacity(), -1);
		
		if(isAlpha)
			offset= 'a';
		else
			offset = '0';
		if(isAlpha && (i==0))
			offset = '0';
		
		for (int j =0; j< s0.size(); ++j)
			if(s0[j][1-i]=='$')
				bucket[0].push(j);
			else
				bucket[s0[j][1-i]-offset+1].push(j);
		for (int i= 0; i< bucket.size(); i++)
			if(bucket[i].size())
				while(!bucket[i].empty())
				{
					SA0.push_back(bucket[i].front());
					bucket[i].pop();
				}
		vector<string> t;
		for (int i =0; i<SA0.size(); i++)
			t.push_back(s0[SA0[i]]);
		s0 = t;
	}
	for(int i =0 ; i< SA0.size(); ++i)
			SA0[i] = m[s0[i]];
			
	// Step 3:
	// Merge SA12 & SA0 using below rules
	vector<int> SA;
	int t_SA12Index =0, t_SA0Index =0;
	while(1)
	{
		{
			if(str[SA12[t_SA12Index]] <  str[SA0[t_SA0Index]])
				{SA.push_back(SA12[t_SA12Index]);++t_SA12Index;}
			else if(str[SA12[t_SA12Index]] >  str[SA0[t_SA0Index]])
				{SA.push_back(SA0[t_SA0Index]);++t_SA0Index;}
			else // they are equal
			{
				if((SA12[t_SA12Index]%3) == 1) //B0 vs B1 case
				{
					// check the rank at b0+1 b1+1 because index b0+1  & b1+1 falls in [1,2 mod n]
					if(i2r [SA0[t_SA0Index]+1]  < i2r[SA12[t_SA12Index]+1])
						{SA.push_back(SA0[t_SA0Index]);++t_SA0Index;}
					else
						{SA.push_back(SA12[t_SA12Index]);++t_SA12Index;}
				}
				else if((SA12[t_SA12Index]%3) == 2) //B0 vs B2 case
				{
					// b0+1 falls in [1,2 mod n] but b2+1 falls in 0 modn , hence first check the letter
					if(str[SA12[t_SA12Index]+1] <  str[SA0[t_SA0Index]+1])
						{SA.push_back(SA12[t_SA12Index]);++t_SA12Index;}
					else if(str[SA12[t_SA12Index]+1] >  str[SA0[t_SA0Index]+1])
						{SA.push_back(SA0[t_SA0Index]);++t_SA0Index;}
					else
					{
						// If still they are equal 
						// check the rank at b0+1 b1+1 because index b0+2(1)  & b2+2(1) falls in [1,2 mod n]
						if(i2r [SA0[t_SA0Index]+2]  < i2r[SA12[t_SA12Index]+2])
							{SA.push_back(SA0[t_SA0Index]);++t_SA0Index;}
						else
							{SA.push_back(SA12[t_SA12Index]);++t_SA12Index;}
					}
				}
			}
		}
		//One of the array will exhaust first, then copy all of the remaining
		if(t_SA0Index==SA0.size())
		{
			//Copy SA12 left over directly
			for(int i = t_SA12Index; i< SA12.size(); i++)
				SA.push_back(SA12[i]);
			break;
		}
		else if(t_SA12Index==SA12.size())
		{
			//Copy SA0 left over directly
			for(int i = t_SA0Index; i< SA0.size(); i++)
				SA.push_back(SA0[i]);
			break;
		}
	}
	return SA;
}
int main(int argc, char*argv[])
{
	string str;
	cin >> str;
	vector<int> SA = DC3(str, true);
	for(int i=0; i< SA.size(); i++)
		if(SA[i]< str.size())
		cout << SA[i]<<" : "<< str.substr(SA[i])<<"\n";
		//cout << SA[i]<<"\n";
	return 0;
}