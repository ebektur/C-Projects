#include "Blocks.h"
#include "Customer.h"
#include <sstream>




int main()
{
	ofstream outputfile;
	outputfile.open("output.txt");

	const string ITEMNOTFOUND = "no";
	//HashTable myhash();
	vector<Block> block_list; //used for constructing the stadium.
	vector<string> row_names;

	//for constructing blocks to push into block_list
	vector<string> block_names;



	ifstream myfile("inputs1.txt");
	stringstream ss;
	stringstream rows;
	stringstream inputfile;
	stringstream columns;
	string BlockNames;
	string RowNames;
	string s_column_number;
	int column_number;
	string BlockName;
	int lastSpace = -1;

	if (myfile.is_open())
	{
		getline(myfile, BlockNames); 
		ss << BlockNames;


		for (int i = 0; i < BlockNames.length(); i++)
		{
			if (BlockNames[i] == ' ')
			{
				BlockName = BlockNames.substr(lastSpace + 1, i - lastSpace - 1);
				block_names.push_back(BlockName);
				lastSpace = i;
			}

			else if (i == BlockNames.length() - 1)
			{
				BlockName = BlockNames.substr(lastSpace + 1, (i - lastSpace));
				block_names.push_back(BlockName);
			}
		}

		
		getline(myfile, RowNames);
		rows << RowNames;
		string RowName;
		int lastSpace2 = -1;
		int length = RowNames.length();

		for (int i = 0; i < RowNames.length(); i++)
		{
			if (RowNames[i] == ' ')
			{
				RowName = "";
				RowName = RowNames.substr(lastSpace2 + 1, i - lastSpace2 - 1);
				row_names.push_back(RowName);
				lastSpace2 = i;
			}

			else if (i == RowNames.length() - 1)
			{
				RowName = "";
				RowName = RowNames.substr(lastSpace2 + 1, (i - lastSpace2));
				row_names.push_back(RowName);
			}
		}

		getline(myfile, s_column_number);
		columns << s_column_number;
		column_number = stoi(s_column_number);
		int size = block_names.size();


		//for (int j = 0; j < size; j++)
		//{
		//	string line = std::move(block_names.front());
		//	block_names.erase(block_names.begin());

		//	Block newblock(line, column_number, row_names); //burda siciyor.
		//	block_list.push_back(newblock);
		//}

		for (int j = 0; j < block_names.size(); j++)
		{
			Block newblock(block_names[j], column_number, row_names);
			block_list.push_back(newblock);
		}

		
		mySortings Stadium(ITEMNOTFOUND, size+1, block_list); //constuctor

		//Stadium.print();


		HashTable Customer(ITEMNOTFOUND, 1);
		string line2;

		string funcs;
		string cust_name;
		string blockname;
		string row_name;
		int columnnum;
		//int cust_count;



		while (getline(myfile, line2)) //parse each word into func + cust + block_name + row_name + col_num
		{
			//++cust_count;
			inputfile.clear();
			inputfile << line2;
			inputfile >> funcs >> cust_name >> blockname >> row_name >> columnnum;


			if (funcs == "print")
			{
				Stadium.print(outputfile);
			}

			if (funcs == "reserve_seat")
			{
				Stadium.reserve_seat(blockname, row_name, cust_name, columnnum, outputfile);
				//Stadium.print();
			}

			if (funcs == "get_seat")
			{
				Stadium.get_seat(cust_name, outputfile);
			}


			if (funcs == "cancel_reservation")
			{
				Stadium.cancel_reservation(cust_name, outputfile);
				//Stadium.print();
	
			}

			if (funcs == "reserve_seat_by_row")
			{
				//cout << "reserve by row";
				Stadium.reserve_seat_by_row(cust_name,row_name, outputfile);
				//Stadium.print();
			}
		}



	}
	else
	{
		outputfile << " File can not open" << endl;
	}




	//make a vector of rows to send to block constructor







	return 0;
}