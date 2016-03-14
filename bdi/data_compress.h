
#ifndef __DATA_COMPRESS_H__
#define __DATA_COMPRESS_H__


#include<iostream>
#include<vector>
#include<unordered_map>
#include "blocker.h"
#include<cmath>
#include<algorithm>

using namespace std;


void init_blocks(vector<pair<string,Blocker*> >& tracker);
void lookup_hexa();
string compress(string packet,int data_size,vector<pair<string,Blocker*> >& tracker,int& used_base,int& used_delta,string& base_str);
bool check_base(vector<string>& part_output,int base,int delta,string& base_str);
bool convert_base(string src,string dst,int base,int delta,string& processed);
bool check_zero(string packet,int data_size,vector<string>& part_output,int base,int delta);
bool zero_diff(string src,string dst,int delta,string& p_input);
long long convert_int(string s);
string convert_hexa(long long num,int delta);
vector<char> get_binary_c(vector<char> b);
vector<char> get_binary(long long num,int limit);
long long get_two_c(long long num,int delta);
long long get_int(vector<char> binary,bool is_neg);
string decompress(string input,vector<pair<string,Blocker*> > tracker,float& bytes_d);
vector<char> get_binary_hexa(string str);

#endif

