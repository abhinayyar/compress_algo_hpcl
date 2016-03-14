#include<iostream>
#include<vector>

#include "parser/file_reader.h"
#include "data_compress.h"
//#include "blocker.h"

using namespace std;
#define FLIT_SIZE 32


int main(int argc,char *argv[])
{
	vector<pair<string,Blocker*> >tracker;

	int used_base=-1,used_delta=-1;
	if(argc <=1)
	{
		cout<<"Please enter data trace file \n";
		return 0;
	}

	/* open file */

	int file_size=open_file(argv[1]);
 	init_blocks(tracker);
	lookup_hexa();

	float total_flits_c=0,total_flits_d=0;	
	float total_bytes_c=0,total_bytes_d=0;	
	for(int i=0;i<file_size;i++)
	{
		float bytes_c=0,bytes_d=0;
		string input_to_process,output_to_get,decode_input;	
		string base_str;
		int data_size=0;
		input_to_process=get_file_input(i,data_size);

		transform(input_to_process.begin(),input_to_process.end(),input_to_process.begin(),::toupper);
		

		//cout<<input_to_process<<"\n";		
		
		//cout<<convert_hexa(-12,2)<<"\n";
		//vector<char> binary =  get_binary_hexa("FB");

		//cout<<get_int_neg(binary)<<"\n";

		//for(char c : binary)
		//	cout<<c<<"\n";
		//cout<<"\n";
	
		//cout<<convert_int("FFFFFFFF")<<"\n";
		bytes_c=input_to_process.size()/2;
		output_to_get=compress(input_to_process,data_size,tracker,used_base,used_delta,base_str);

		
		cout<<"Used Base : "<<used_base<<"Used delts : "<<used_delta<<"\t"<<base_str<<"\n";
		
		/*
		if(used_base!=-1 && used_delta!=-1)
		{
			cout<<"Input :\n";
			for(int i=0;i<input_to_process.size();i+=used_base)
				cout<<input_to_process.substr(i,used_base)<<"\t";

			cout<<"\n";
			cout<<"Output : \n";
			
			cout<<"code " <<output_to_get.substr(0,4)<<"\t"<<"base "<<output_to_get.substr(4,used_base)<<"\n";		
			for(int i=4+used_base;i<output_to_get.size();i+=used_delta+1)
				cout<<output_to_get.substr(i,used_delta+1)<<"\t";	
		}
		else
		{
			cout<<"Uncompressed packet\n";
		}
		cout<<"\n";

		*/
		
		cout<<"Out \n"<<output_to_get<<"\n";		

		if(output_to_get.substr(0,4).compare("1111")==0)
		{
			decode_input=output_to_get.substr(4);	
			bytes_d+=(0.5+bytes_c);
		}
		else if(output_to_get.substr(0,4).compare("0000")==0)
		{
			decode_input= input_to_process;	
		}
		else
		{
			decode_input=decompress(output_to_get,tracker,bytes_d);
			bytes_d+=(used_base/2);
		
		}

		
		if(output_to_get.substr(0,4).compare("0000")==0)
		{
			bytes_d-=(used_base/2);
		}
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
	}

	cout<<" Total Bytes original \t"<<total_bytes_c<<" Total bytes compressed \t"<<total_bytes_d<<"\n";
	cout<<" Total Flits original \t"<<total_flits_c<<" Total Flits compressed \t"<<total_flits_d<<"\n";

	return 0;
}
