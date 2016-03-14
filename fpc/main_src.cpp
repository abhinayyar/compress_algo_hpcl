#include<iostream>
#include<vector>

#include "parser/file_reader.h"
#include "data.h"

using namespace std;
#define FLIT_SIZE 32


int main(int argc,char *argv[])
{
	vector<pair<string,float> >tracker;

	if(argc <=1)
	{
		cout<<"Please enter data trace file \n";
		return 0;
	}

	/* open file */

	int file_size=open_file(argv[1]);
 	initialize(tracker);

	float total_flits_c=0,total_flits_d=0;	
	float total_bytes_c=0,total_bytes_d=0;	
	for(int i=0;i<file_size;i++)
	{
		float bytes_c=0,bytes_d=0;
		string input_to_process,output_to_get,decode_input;	
		int data_size=0;
		input_to_process=get_file_input(i,data_size);

		transform(input_to_process.begin(),input_to_process.end(),input_to_process.begin(),::toupper);
		bytes_c=input_to_process.size()/2;
		output_to_get=compress(input_to_process,tracker,bytes_d);
			
		
		//cout<<"Inp \n"<<input_to_process<<"\n";
	
		cout<<"\n";		
		cout<<"Out \n"<<output_to_get<<"\n";		

		decode_input=decode(output_to_get,tracker,bytes_d);
		

		
		
		cout<<"\n";
		cout<<"\n";
		cout<<input_to_process<<"\n";
		cout<<"\n";
		cout<<"--------------------------------------\n";
		cout<<decode_input<<"\n";
	
		if(input_to_process.compare(decode_input)==0)
		{
			cout<<"  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \t Match ..\n";	
		}
		else
		{
			
			cout<<" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \t Un - Match ..\n";	
		}

		float flit_c=bytes_c/FLIT_SIZE;
		float flit_d=bytes_d/FLIT_SIZE;

		total_bytes_c+=bytes_c;
		total_bytes_d+=bytes_d;

		total_flits_c+=flit_c;
		total_flits_d+=flit_d;

		cout<<"Flit original : \t"<<flit_c<<"Flit compressed : \t"<<flit_d<<"\n";
		cout<<"\n";
		cout<<"\n";
		cout<<"===============================================================================\n";
		cout<<"\n";
		
		cout<<"\n";			
	}

	cout<<" Total Bytes original \t"<<total_bytes_c<<" Total bytes compressed \t"<<total_bytes_d<<"\n";
	cout<<" Total Flits original \t"<<total_flits_c<<" Total Flits compressed \t"<<total_flits_d<<"\n";

	return 0;
}
