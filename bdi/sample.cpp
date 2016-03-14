#include<iostream>
#include<vector>
#include<stdlib.h>

using namespace std;


vector<char> get_binary_c(vector<char> b)
{
vector<char> res;
int carry=1;
	for(int i=b.size()-1;i>=0;i--)
	{
		if(b[i]=='0')
		{
			int val=(1^carry);
			res.insert(res.begin(),'0'+val);
			carry= 1 & carry;
		}
		else
		{
			int val2=(0^carry);
			res.insert(res.begin(),'0'+val2);
			carry = 0;
		}	
	}
	return res;
}


int main()
{
	//cout<<get_two_c(7)<<"\n";
	vector<char> input;
	input.push_back('0');
	input.push_back('1');
	input.push_back('1');
	input.push_back('1');

	vector<char> res=get_binary_c(input);

	for(char c : res)
	{
		cout<<c<<"\t";
	}
	cout<<"\n";
	return 0;
}
