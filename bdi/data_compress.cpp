// file to compress data

#include "data_compress.h"

using namespace std;
// change tracker to get order
unordered_map<char,int> lc_cmp;
unordered_map<int,char> lc_dcp;


void get_base_delta(vector<pair<string,Blocker*> > tracker,int& base,int& delta,string ss)
{

	for(pair<string,Blocker*> p : tracker)
	{
		if(p.second->code.compare(ss)==0)
		{
			base=p.second->base*2;
			delta=p.second->delta*2;
			break;
		}	
	}
	return;
}
string get_zero_out(int num)
{
	string res;
	for(int i=0;i<num;i++)
	{
		res+="0";
	}
	return res;
}
// get binary from hexa

vector<char> get_binary_hexa(string str)
{
	vector<char> res;

	for(char c : str)
	{
		int val=0;
		if(lc_cmp.find(c)!=lc_cmp.end())
		{
			val=lc_cmp[c];
		}
		else
		{
			val=c-'0';
		}
		//cout<<val<<"\t";
		vector<char> temp= get_binary(val,4);
		for(char p : temp)
			res.push_back(p);
	}

	return res;
}
// get binary corresponding to negative number

long long get_int(vector<char> binary,bool is_neg)
{

	int count=binary.size()-1;
	
	long long val=0;

	if(is_neg==true)
	{
		//cout<<"neg\n";
		val=-1*pow(2,count);
	}
	else
	{
		//cout<<"pos\n";
		val=(binary[0]-'0')*pow(2,count);
	}
	count--;


	//cout<<val<<"\n";
	for(int i=1;i<binary.size();i++)
	{
		val+=(binary[i]-'0')*pow(2,count);
		count--;
	}

	return val;
}
string decompress(string input,vector<pair<string,Blocker*> > tracker,float& bytes)
{
	// 1st 4 are base and the on basis of code next defines base length

	string output;
	int base=0,delta=0;
	get_base_delta(tracker,base,delta,input.substr(0,4));

	string base_str=input.substr(4,base);

	//cout<<"================== BAse str "<<base_str<<"\n";
	string decode=input.substr(base+4);

	bytes+=(0.5);
	//cout<<decode<<"\n";
	for(int i=0;i<decode.size();i+=delta+1)
	{
		if(decode[i]=='0')
		{
			output+=get_zero_out(base-delta)+decode.substr(i+1,delta);
		}
		else
		{
			//cout<<"Base decode\n";
				
			vector<char> binary_base = get_binary_hexa(base_str);

			for(char c : binary_base)
			{
				//cout<<c<<"\t";
			}
			//cout<<"\n";
			long long a=0,b=0;
			if(binary_base[0]=='1')
				a = get_int(binary_base,true);
			else
				a = get_int(binary_base,false);
		
			vector<char> binary_delta = get_binary_hexa(decode.substr(i+1,delta));
			
			
			if(binary_delta[0]=='1')
				b = get_int(binary_delta,true);
			else	
				b = get_int(binary_delta,false);

			//cout<<"Base : "<<a<<"\t"<<"Delta :"<<b<<" uuu "<<"\n";
			output+=convert_hexa(a+b,base);
		}
		
		bytes+=(0.125+delta/2);
	}

	return output;	
}
void init_blocks(vector<pair<string,Blocker*> >& tracker)
{
	//Blocker *ob = new Blocker("0000","zeros",1,0,1);
	//tracker.insert(make_pair("zeros",ob));	


	Blocker *ob1 = new Blocker("0010","base8d1",8,1,12);
	tracker.push_back(make_pair("base8d1",ob1));
	Blocker *ob2 = new Blocker("0011","base8d4",8,2,24);
	tracker.push_back(make_pair("base8d4",ob2));
	Blocker *ob3 = new Blocker("0100","base8d2",8,4,16);
	tracker.push_back(make_pair("base8d2",ob3));


	Blocker *ob4 = new Blocker("0101","base4d2",4,1,20);
	tracker.push_back(make_pair("base4d2",ob4));

	Blocker *ob5 = new Blocker("0110","base4d1",4,2,12);
	tracker.push_back(make_pair("base4d1",ob5));

	Blocker *ob6 = new Blocker("0111","base2d1",2,1,18);
	tracker.push_back(make_pair("base2d1",ob6));


	//Blocker *ob7 = new Blocker("1111","nocmpr",-1,-1,32);
	//tracker.push_back(make_pair("nocmpr",ob7));	

	//Blocker *ob8 = new Blocker("0001","rep",8,0,8);
	//tracker.push_back(make_pair("rep",ob));		

}

void lookup_hexa()
{
	lc_cmp.insert(make_pair('A',10));
	lc_cmp.insert(make_pair('B',11));
	lc_cmp.insert(make_pair('C',12));
	lc_cmp.insert(make_pair('D',13));
	lc_cmp.insert(make_pair('E',14));
	lc_cmp.insert(make_pair('F',15));
	lc_dcp.insert(make_pair(10,'A'));
	lc_dcp.insert(make_pair(11,'B'));
	lc_dcp.insert(make_pair(12,'C'));
	lc_dcp.insert(make_pair(13,'D'));
	lc_dcp.insert(make_pair(14,'E'));
	lc_dcp.insert(make_pair(15,'F'));
}
string form_output(vector<string> part_output)
{
	string res;

	for(string s : part_output)
	{
		res+=s;
	}
	return res;
}
// full packet compress function
string compress(string packet,int data_size,vector<pair<string,Blocker*> >& tracker,int& used_base,int& used_delta,string& base_str)
{

	string output;
	//cout<<__FUNCTION__<<"\n";	
	for(pair<string,Blocker*> p : tracker)
	{
		vector<string> part_output;
		
		//cout<<p.second->base<<p.second->delta<<"\n";	
		bool all_zero=check_zero(packet,data_size,part_output,p.second->base*2,p.second->delta*2);
		if(all_zero==true)
		{
			return output.assign("0000"+form_output(part_output));
		}
		used_base=p.second->base*2;
		used_delta=p.second->delta*2;

		if(check_base(part_output,p.second->base*2,p.second->delta*2,base_str))
		{
			//cout<<"True now\n";
			return p.second->code+base_str+form_output(part_output);
		}	
		
	}
	used_base=-1;
	used_delta=-1;
	return output.assign("1111"+packet);	
}

bool check_base(vector<string>& part_output,int base,int delta,string& base_str)
{
	bool is_base_set=false;
	string processed;

		
	//cout<<__FUNCTION__<<"\n";	
	for(int i=0;i<part_output.size();i++)
	{
		string cur=part_output[i];
		if(cur.front()=='#')
		{
			cur.erase(cur.begin());
			if(is_base_set==false)
			{
				base_str.assign(cur);
				//cout<<"Picked base :  "<<base_str<<"\n";
				is_base_set=true;
			}
			
			if(convert_base(cur,base_str,base,delta,processed)==false)
			{
				return false;
			}
			part_output[i].assign("1"+processed);	
		}
	}

	return true;
}

bool convert_base(string src,string dst,int base,int delta,string& processed)
{
	
	//cout<<__FUNCTION__<<"\n";	
	
	//cout<<"1st "<<src<<"\t"<<dst<<"\n";
	// dst here imply that its base string
	string src_delta=src.substr(0,src.size()-delta);
	string dst_delta=dst.substr(0,dst.size()-delta);

	if(src_delta.compare(dst_delta)!=0)
	{
		return false;
	}

	//cout<<"2nd "<<src_delta<<"\t"<<dst_delta<<"\n";	

	//cout<<"base "<<dst<<"\n";
	vector<char> binary_base = get_binary_hexa(dst);

	long long a=0,b=0;
	if(binary_base[0]=='1')
		a = get_int(binary_base,true);
	else
		a = get_int(binary_base,false);
	
	//cout<<"Delta "<<src.substr(src.size()-delta)<<"\n";	
	vector<char> binary_delta = get_binary_hexa(src.substr(src.size()-delta));
			
			
	if(binary_delta[0]=='1')
		b = get_int(binary_delta,true);
	else	
		b = get_int(binary_delta,false);

	//cout<<"Compress base delta\n";
	//cout<<a<<"\t"<<b<<" encode uuu "<<"\n";


	long long two_c=a*-1;

	//cout<<"tw_c"<<two_c<<"\n";
	//cout<<"base "<<b<<"src "<<a<<"\n";		
	processed.assign(convert_hexa(b+two_c,delta));

	//cout<<"base c "<<processed<<"\n";
	return true;
	
}
//function which try to compress it with zero base
bool check_zero(string packet,int data_size,vector<string>& part_output,int base,int delta)
{
	bool all_zero=true;

	// base = base * 2
	string zero;
		
	//cout<<__FUNCTION__<<"\n";	
	for(int i=0;i<base;i++)
	{
		zero+="0";
	}
		
	for(int i=0;i<packet.size();i+=base)
	{
		string processed_input;
		string sub=packet.substr(i,base);
		if(zero_diff(sub,zero,delta,processed_input)==false)
		{
			part_output.push_back("#"+sub);
			all_zero=false;
		}
		else
		{
			part_output.push_back(processed_input);
		}
		//cout<<sub<<"\t"<<part_output.back()<<"\n";
					
	}
	return all_zero;		
}
//function that compares whether our string with zero and a particular base
bool zero_diff(string src,string dst,int delta,string& p_input)
{
	// dst is our base in this case

	string src_delta=src.substr(0,src.size()-delta);
	string dst_delta=dst.substr(0,dst.size()-delta);
	

	////cout<<__FUNCTION__<<"\n";	

	////cout<<src_delta<<"\t"<<dst_delta<<"\n";	
	if(src_delta.compare(dst_delta)!=0)
	{
		return false;
	}

	p_input.assign("0"+src.substr(src.size()-delta));
	return true;
}

long long convert_int(string s)
{
	//cout<<" int st "<<s<<"\n";
	long long output=0;
	int size=s.size()-1; 
	// string to int ( string by def is hexadecimal )
	for(char p: s) 
	{

		if(lc_cmp.find(p)!=lc_cmp.end())
		{
			output+=lc_cmp[p]*pow(16,size);	
		}
		else
		{
			output+=(p-'0')*pow(16,size);
		}
		size--;		
	}
		
	return output;
}

string convert_hexa(long long num,int delta)
{
	
	// convert int to hexa and return output in form of string
	int limit=delta*4;
	bool is_neg=false;

	string output;

	if(num<0)
	is_neg=true;
	
	vector<char> binary = get_binary(abs(num),limit);
		
	vector<char> two_c;
	if(is_neg==true)
	two_c=get_binary_c(binary);
	else
	two_c=binary;
	

	for(int i=0;i<two_c.size();i+=4)
	{
		int val=0;
		int count=3;
		for(int j=i;j<i+4;j++)
		{
			val+=(two_c[j]-'0')*pow(2,count);
			count--;
		}

		if(lc_dcp.find(val)!=lc_dcp.end())
		{
			string s;
			s.push_back(lc_dcp[val]);
			output+=s;
		}
		else
		{
			output+=to_string(val);
		}

	}
	//cout<<"hex out "<<output<<"\n";
	return output;	
}
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
vector<char> get_binary(long long num,int limit)
{
	vector<char> res;

	while(limit>0)
	{
		res.insert(res.begin(),num%2+'0');
		num=num/2;
		limit--;
	}
	return res;
}
long long get_two_c(long long num,int delta)
{
	long long output=0;
	while(num>0)
	{
		int val =(num & 1);
		if(val!=1)
		output = output | 1;	
		num>>=1;
		if(num>0)
		output<<=1;
	}

	return output;	
}
