#include "data.h"


using namespace std;


#define DATA_SIZE 8

unordered_set<string> lc_cmp;

void initialize(vector<pair<string,float> >& tracker)
{

	tracker.push_back(make_pair("000",0));
	tracker.push_back(make_pair("001",1));
	tracker.push_back(make_pair("010",2));
	tracker.push_back(make_pair("011",4));
	tracker.push_back(make_pair("100",4));
	tracker.push_back(make_pair("101",4));
	tracker.push_back(make_pair("110",2));
	tracker.push_back(make_pair("111",8));

	lc_cmp.insert("8");
	lc_cmp.insert("9");
	lc_cmp.insert("A");
	lc_cmp.insert("B");
	lc_cmp.insert("C");
	lc_cmp.insert("D");
	lc_cmp.insert("E");
	lc_cmp.insert("F");
	
}
string compress(string input,vector<pair<string,float> > tracker,float& bytes)
{


	string output;
	for(int i=0;i<input.size();i+=DATA_SIZE)
	{
		string sub=input.substr(i,DATA_SIZE);
		output+=each_compress(sub,tracker,bytes);
	
	}

		
	return output;
}

// for each string compression

string each_compress(string input,vector<pair<string,float> > tracker,float& bytes)
{
	string processed;

	for(pair<string,float> p : tracker)
	{
		compare_pattern(input,p.first,processed,bytes);
		if(processed.empty()==false) return processed;
		
	}
	cout<<"Should not reach here..\n";
	return processed;	
}

string gen_zero(int n)
{
	string res;

	for(int i=0;i<n;i++)
	res+="0";

	return res;
}
string gen_f(int n)
{
	string res;

	for(int i=0;i<n;i++)
	res+="F";

	return res;
}
bool check_str(string input,int index,string& processed,float& bytes,int data_size)
{

	string sub=input.substr(0,data_size-index);
	string cmp=input.substr(data_size-index,1);
	if(sub.compare(gen_zero(data_size-index))==0 && cmp[0]-'0'<=7)
	{
		if(cmp[0]-'0'>=0 && cmp[0]-'0'<=7)
		{
			processed+=input.substr(data_size-index);
		}
		return true;	
	}
	else if(sub.compare(gen_f(data_size-index))==0 && cmp[0]-'0'>7)
	{
		string ls_str=input.substr(data_size-index,1);
		if(lc_cmp.find(ls_str)!=lc_cmp.end())
		{
			processed+=input.substr(data_size-index);
		}
		return true;	
	}

	return false;
	
}
void compare_pattern(string input,string pattern,string& processed,float& bytes)
{

	if(pattern.compare("000")==0)
	{	
		// case for zero run
		if(input.compare(gen_zero(DATA_SIZE))==0)
		processed.assign(pattern);
			
	}	
	else if(pattern.compare("001")==0)
	{
		// four bit sign extended

		if(check_str(input,1,processed,bytes,DATA_SIZE))
		processed=pattern+processed;	
	}	
	else if(pattern.compare("010")==0)
	{
		// 1 byte sign extended
			
		if(check_str(input,2,processed,bytes,DATA_SIZE))				
		processed=pattern+processed;	
		
	}	
	else if(pattern.compare("011")==0)
	{
		// 2 byte sign extended
		
		if(check_str(input,4,processed,bytes,DATA_SIZE))				
		processed=pattern+processed;	
	}
	else if(pattern.compare("100")==0)
	{
		// half word padded with zero half word

		string sub=input.substr(DATA_SIZE-4);
		
		if(sub.compare(gen_zero(DATA_SIZE-4))==0)
		{
			processed.assign(pattern);
			processed+=input.substr(0,DATA_SIZE-4);
		}	
	}
	else if(pattern.compare("101")==0)
	{

		// two half words each a byte sign extended

		string part_one=input.substr(0,DATA_SIZE-4);
		string part_two=input.substr(DATA_SIZE-4);

		string p_o,p_t;
		if(check_str(part_one,2,p_o,bytes,DATA_SIZE/2) && check_str(part_two,2,p_t,bytes,DATA_SIZE/2))
		{
			processed=pattern+p_o+p_t;	
		}
			
	}
	else if(pattern.compare("110")==0)
	{
		string rep;
		if(check_rep(input,rep))
		processed=pattern+rep;	
	}
	else if(pattern.compare("111")==0)
	{
		processed=pattern+input;		
	}
	return;
}


bool check_rep(string input,string& rep)
{
	unordered_set<string> tmp;

	tmp.insert(input.substr(0,2));

	for(int i=2;i<input.size();i+=2)
	{
		string sub=input.substr(i,2);

		if(tmp.find(sub)==tmp.end())
		return false;
	}

	rep.assign(input.substr(0,2));
	return true;
}
int get_pattern_len(string pattern,vector<pair<string,float> > tracker)
{
	for(pair<string,float> p : tracker)
	{
		if(p.first.compare(pattern)==0)
		return (int)p.second;
	}	
}
string decode(string input,vector<pair<string,float> > tracker,float& bytes_d)
{


	int i=0;
	int len=0;
	string output;
	while(i<input.size())
	{
		string pattern=input.substr(i,3);
		len=get_pattern_len(pattern,tracker);
		bytes_d+=(len/2);
		bytes_d+=(3/8);

		output+=form_output(pattern,input.substr(i+3,len));
		i+=(len+3);	
	}

	return output;					
}

void decode_each(string input,int index,string& output)
{

	if(input[0]-'0'>=0&& input[0]-'0'<=7)
	{
		output.assign(gen_zero(DATA_SIZE-index));
	}
	else
	{
		output.assign(gen_f(DATA_SIZE-index));
	}

}
string form_output(string pattern,string input)
{
	string output;

	if(pattern.compare("000")==0)
	{
		output.assign(gen_zero(DATA_SIZE));	
	}
	else if(pattern.compare("001")==0)
	{
		decode_each(input,1,output);
		output+=input;		
	}
	else if(pattern.compare("010")==0)
	{
			
		decode_each(input.substr(0,1),2,output);	
		output+=input;		
	}
	else if(pattern.compare("011")==0)
	{
			
		decode_each(input.substr(0,1),4,output);	
		output+=input;		
	}
	else if(pattern.compare("100")==0)
	{	
		output=input+gen_zero(4);		
	}
	else if(pattern.compare("101")==0)
	{
		string one=input.substr(0,2);
		string two=input.substr(2);

		string o_one,o_two;	
		decode_each(one.substr(0,1),6,o_one);	
		decode_each(two.substr(0,1),6,o_two);
		o_one+=one;
		o_two+=two;	
		output+=(o_one+o_two);		
	}
	else if(pattern.compare("110")==0)
	{
		for(int i=0;i<4;i++)
		{
			output+=input;
		}	
	}
	else
	{
		output.assign(input);
	}

	return output;	
}
