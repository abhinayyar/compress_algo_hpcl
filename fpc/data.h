#ifndef __DATA_H__
#define __DATA_H__

#include<iostream>
#include<vector>
#include<unordered_map>
#include<utility>
#include<cstring>
#include<algorithm>
#include<cmath>
#include<unordered_set>

using namespace std;


string compress(string input,vector<pair<string,float> > tracker,float& bytes);
string each_compress(string input,vector<pair<string,float> > tracker,float& bytes);

void initialize(vector<pair<string,float> >& tracker);
string gen_zero(int n);
string gen_f(int n);
void compare_pattern(string input,string pattern,string& processed,float& bytes);
bool check_rep(string input,string& rep);
int get_pattern_len(string pattern,vector<pair<string,float> > tracker);
string decode(string input,vector<pair<string,float> > tracker,float& bytes_d);

void decode_each(string input,int index,string& output);
string form_output(string pattern,string input);
bool check_str(string input,int index,string& processed,float& bytes,int data_size);


#endif


