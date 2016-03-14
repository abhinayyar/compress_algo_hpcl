// functions to read data from the file

#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>

#define DATA 3
#define DATA_SIZE 2
#define SINGLE_DATA_SIZE 8
using namespace std;

ifstream ifile;
vector<vector<string> > input_data;


vector<string> generate_test_data(string cont_data);


int open_file(string file_name)
{
	

	
	ifile.open(file_name);
	string time,sim_id,data_size,data;
	
	while(ifile >> time >> sim_id >> data_size >> data)
	{
		vector<string> data_line;
		data_line.push_back(time);
		data_line.push_back(sim_id);
		data_line.push_back(data_size);
		data_line.push_back(data);
		input_data.push_back(data_line);	
	}
	return input_data.size();
}

string get_file_input(int index,int& data_size)
{
	// actual data	

	if(input_data[index][DATA_SIZE].empty()==false)
	data_size=stoi(input_data[index][DATA_SIZE]);

	return input_data[index][DATA];
}
	
