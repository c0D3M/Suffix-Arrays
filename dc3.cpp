#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
string str;
bool isSame(int a1, int b1)
{
	if ((str[a1]==str[b1]) && (str[a1+1]==str[b1+1]) && (str[a1+2]==str[b1+2]))
		return true;
	else
		return false;
}
vector<int> DC3(string str, bool isAlpha=0)
{
	int l = str.size();
	int token = isAlpha?'$':'0';
	int offset = isAlpha?'a':'0';
	
	// Modiy string to append $
	if((l-1) % 3 == 2)
		str.append(2, token);
	else if( ((l-1) % 3 == 1) || ((l-1) % 3 == 0))
		str.append(3, token);
	 
	cout <<"Modified String is : "<< str<<"\n"; 
	// Step 1:
	// Generate SA12
	
	// Get 1,2 mod 3 position
	vector<int> pos;
	for (int i=0 ; i<str.size()-2; i++)
		if(i%3)
			pos.push_back(i);
	
	for (int i = 1; i<= 3; i++)
	{
		vector<queue<int>> bucket(27); // '$' occupy 0th index
		for (int j=0; j< pos.size(); j++)
			if(str[pos[j] + 3 -i]==token)
					bucket[0].push(pos[j]);
				else
					bucket[str[pos[j] + 3 -i]-offset + 1 ].push(pos[j]);  // +1 is done to accomodate '$' on index 0 bucket
		
		pos.clear();
		for (int i =0; i< 27; i++)
			if(bucket[i].size())
				while(!bucket[i].empty())
				{
					pos.push_back(bucket[i].front());
					bucket[i].pop();
				}
	}
	//Calculate Rank
	vector<int> rank(pos.size(), 1);
	int next_rank =1 ;
	vector<int> i2r(str.size()-2, -1); // Help in egenarting reverse mapping index i is which rank suffix
	i2r[pos[0]] = next_rank;
	for (int i=1; i< pos.size(); i++)
	{
		if(!isSame(pos[i],pos[i-1]))
			++next_rank;
		i2r[pos[i]] = next_rank;
		rank[i] = next_rank;
	}

	// pos is SA12
	if(next_rank!=pos.size())
	{
		string q(pos.size(), '0');
		// Create a string which has rank of [1 mod 3 || 2 mod 3]
		// For example [1,4,7,10]||[2, 5, 8, 11] , rank of these indices, form a string
		for (int i=0; i< pos.size(); i++)
		{
			int j =0;
			if((pos[i] % 3 )==2)// if its 2 mod 3 , it falls in 2nd half hence offset ;
				j = pos.size()/2;
			int index = j + (pos[i]/3);
			q[index] = '0' + rank[i];
		}
		vector<int> SA12 = DC3(q);
		int t[pos.size()];
		for (int i=0 ; i < pos.size()/2; i++)
		{
			t[i] = 1 +(3*i);
			t[i+(pos.size()/2)] = 2 +(3*i);
		}
		pos.clear();
		for (int i =1; i < SA12.size(); i++)
			pos.push_back(t[SA12[i]]);
		
	}
	
	// Step 2:
	// Generate SA0 using SA12
	vector<string> s0;
	map<string, int> m;
	for (int i=0; i< str.size()-2 ; i+=3)
	{
		string t; t += str[i];
		s0.push_back(t);
		s0.back() += i2r[i+1]+'0';
		m[s0.back()] = i;
	}
	vector<int> newPos;
	//Radix sort s0
	for (int i = 0; i < 2; ++i)
	{
		newPos.clear();
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
					newPos.push_back(bucket[i].front());
					bucket[i].pop();
				}
		vector<string> t;
		for (int i =0; i<newPos.size(); i++)
			t.push_back(s0[newPos[i]]);
		s0 = t;
	}
	//newPos is SA0
	if(isAlpha)
		for(int i =0 ; i< newPos.size(); ++i)
			newPos[i] = m[s0[i]];
	else
	// newPos stores index in SA0 string, to get index in actual string jyst multiply by 3 
	for(int i =0 ; i< newPos.size(); ++i)
		newPos[i] *= 3;

		
	// Step 3:
	// Merge SA12 & SA0 using below rules
	// Rule 1:
	vector<int> SA;
	int posIndex =0, newPosIndex =0;
	while(1)
	{
		int t_SA12Index = posIndex;
		int t_SA0Index = newPosIndex;
		{
			if(str[pos[t_SA12Index]] <  str[newPos[t_SA0Index]])
				{SA.push_back(pos[t_SA12Index]);++posIndex;}
			else if(str[pos[t_SA12Index]] >  str[newPos[t_SA0Index]])
				{SA.push_back(newPos[t_SA0Index]);++newPosIndex;}
			else // they are equal
			{
				if((pos[posIndex]%3) == 1) //B0 vs B1 case
				{
					// check the rank at b0+1 b1+1
					if(i2r [newPos[newPosIndex]+1]  < i2r[pos[posIndex]+1])
						{SA.push_back(newPos[t_SA0Index]);++newPosIndex;}
					else
						{SA.push_back(pos[t_SA12Index]);++posIndex;}
				}
				else if((pos[posIndex]%3) == 2) //B0 vs B2 case
				{
					// First check the letter at b0+1 b2+1
					if(str[pos[t_SA12Index]+1] <  str[newPos[t_SA0Index]+1])
						{SA.push_back(pos[t_SA12Index]);++posIndex;}
					else if(str[pos[t_SA12Index]+1] >  str[newPos[t_SA0Index]+1])
						{SA.push_back(newPos[t_SA0Index]);++newPosIndex;}
					else
					{
						if(i2r [newPos[newPosIndex]+2]  < i2r[pos[posIndex]+2])
							{SA.push_back(newPos[t_SA0Index]);++newPosIndex;}
						else
							{SA.push_back(pos[t_SA12Index]);++posIndex;}
					}
				}
			}
		}
		if(newPosIndex==newPos.size())
		{
			//Copy SA12 left over directly
			for(int i = posIndex; i< pos.size(); i++)
				SA.push_back(pos[i]);
			break;
		}
		else if(posIndex==pos.size())
		{
			//Copy SA0 left over directly
			for(int i = newPosIndex; i< newPos.size(); i++)
				SA.push_back(newPos[i]);
			break;
		}
	}
	return SA;
}
int main(int argc, char*argv[])
{
	cin >> str;
	str.append("$");
	DC3(str, 1);
	return 0;
}