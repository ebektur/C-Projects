#pragma once



#ifndef mytemp
#define mytemp
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>


using namespace std;


struct Seats { 
	int column_number;
	string row_number;
	string assigned_customer;
	string block_name;
	//Seats* seat;

	Seats() {
	
	}



	Seats(string rowname, int col_pos) : column_number(col_pos), row_number(rowname)
	{
		assigned_customer = "---";
	}


	friend class Hashtable;
	//customer name hashtable link?

	//WordItem * head;
	//DocumentItem * head;
	// List of DocumentItem’s. In order to keep the documents 
	//you can again use the BST that you are going to implement.
};


class Block
{
public:

	friend class mySortings;

	//for blocks
	string block_name;

	//for seats
	//vector<vector<Seats>> seatlist;
	Seats* column; //should show the first seat in the block
	Seats** seatlist;
	int column_size;
	int row_size;
	vector<string> row_list;

	Block(){}

	Block(const string& theElement, int col_size, vector<string> rows) : //, 
		block_name(theElement), column_size(col_size), row_list(rows)
	{

		seatlist = new Seats * [row_list.size()];
		for (int i = 0; i < row_list.size(); i++)
		{
			seatlist[i] = new Seats[column_size];
		}


		string rowname = std::move(rows.front());
		rows.erase(rows.begin());

		for (int i = 0; i < row_list.size(); i++)
			for (int j = 0; j < column_size; j++)
				seatlist[i][j] = Seats(rowname, j);

		row_size = row_list.size();

		/*for (int x = 0; x < row_list.size(); x++)
		{
			string rowname = rows.front();
			for (int y = 0; y < column_size; y++)
			{
				seatlist[x][y] = Seats(rowname, y);
			} 
		}

		ptr = &seatlist[0][0];*/
	} //, height(h)
	//3lu constructor at page 66
	friend class mySortings;


	int getRowVacancy(const string& x, Block* t, int index)
	{
		int vacancy = 0;
		//int index;
		////auto it = std::find(t->row_list.begin(), t->row_list.end(), x);
		////if (it != t->row_list.end())
		////{
		////	index = it - t->row_list.begin();
		////}

		//for (int j = 0; j < row_list.size(); j++)
		//{
		//	if (t[j].block_name == x)
		//	{
		//		index = 
		//	}
		//}

		for (int i = 0; i < t->column_size; i++)
		{
			if (t->seatlist[index][i].assigned_customer == "---")
				vacancy++;
		}

		return vacancy;
	}

	int getIndex(const string& x, Block* t)
	{

		if (t != nullptr)
		{
			int index = 0;
			for (int k = 0; k < t->row_list.size(); k++)
			{
				if (t->row_list[k] == x)
				{
					index = k;
				}
			}

			//auto it = std::find(t->row_list.begin(), t->row_list.end(), x);
			//if (it != t->row_list.end())
			//{
			//	index = it - t->row_list.begin();
			//}

			return index;
		}

		return -1;

	}

	int getLeftMostColumn(int row_index, int col_size, Block* t) //seats[0][0] acceptable
	{
		int index = 0;
		for (int j = 0; j < col_size; j++)
		{
			if (seatlist[row_index][j].assigned_customer == "---")
			{
				index = j;
				return j;
			}
		}

		return -1;
	}




};


class HashTable //of customer names
{

public:
	explicit HashTable(const string& notFound, int size = 101);

	HashTable(const HashTable& rhs) : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), hasharray(rhs.hasharray), currentSize(rhs.currentSize)
	{

	}

	const string& find(const string& x) const;

	void makeEmpty();

	void insert(const string& x,  Seats& a);

	void deallocate( string& x, string & row, int & col, string& block_name);

	Seats* findSeat(const string& x);


	//void add(const HashedObj & x);

	//void remove( const HashedObj & x );


	//.........

	//void getCustomerInfo(string& row, int& col, string &cust_name)
	//{
	//	row = getSeatofPresentCustomer(cust_name).rowname;
	//	col = getSeatofPresentCustomer(cust_name).column;

	//}


	int nextPrime(int n);

	bool isPrime(int n);

	int hash(const string& key, int tablesize) const;

	enum EntryType { ACTIVE, EMPTY, DELETED };


private:

	struct Customer

	{
		string name; //name of the customer
		//seat info
		string rowname;
		int column;
		string block_name;
		//customer booking availability
		EntryType info;

		Seats ptr; //doc
		Seats* ptr2;

		Customer() {}

		Customer(const string& e, const Seats& o,
			EntryType i = EMPTY)
			: name(e), ptr(o), info(i) { }


	};

	vector<Customer> hasharray;

	void getSeatofPresentCustomer(string& x, string & row, int & col, string & block)
	{
		int currentPos = findPos(x);

		if (isActive(currentPos))
		{
			x = hasharray[currentPos].name;
			row = hasharray[currentPos].rowname;
			col = hasharray[currentPos].column;
			block = hasharray[currentPos].block_name;

		}
	}
	
	int currentSize;
	const string ITEM_NOT_FOUND;
	//Block* BlockInfo; kullanilmamis?

	bool isActive(int CurrentPos) const;
	int findPos(const string& x) const;
	void rehash();


};


int HashTable::nextPrime(int n)
{
	if (n % 2 == 0)
		n++;

	for (; !isPrime(n); n += 2)
		;

	return n;
}

bool HashTable::isPrime(int n)
{
	if (n == 2 || n == 3)
		return true;

	if (n == 1 || n % 2 == 0)
		return false;

	for (int i = 3; i * i <= n; i += 2)
		if (n % i == 0)
			return false;

	return true;
}


HashTable::HashTable(const string& notFound, int size) : ITEM_NOT_FOUND(notFound), hasharray(nextPrime(size))

{
	makeEmpty();
}

void HashTable::makeEmpty()
{
	for (int j = 0; j < hasharray.size(); j++)
	{
		hasharray[j].info = EMPTY;
	}
}

int HashTable::findPos(const string& x) const

{

	int collisionNum = 0;

	int currentPos = hash(x, hasharray.size());
	
	while (hasharray[currentPos].info != EMPTY &&

		hasharray[currentPos].name != x)

	{

		currentPos += 2 * ++collisionNum - 1; // add the difference

		if (currentPos >= hasharray.size())
		{// perform the mod

			currentPos = currentPos - hasharray.size(); // if necessary
		}

	}

	return currentPos;

}

bool HashTable::isActive(int currentPos) const

{

	return hasharray[currentPos].info == ACTIVE;

}

const string& HashTable::find(const string& x) const
{
	int currentPos = findPos(x);

	if (isActive(currentPos))
	{
		return hasharray[currentPos].name;
	}

	return ITEM_NOT_FOUND;

}

Seats* HashTable::findSeat(const string& x) 
{
	int currentPos = findPos(x);

	if (isActive(currentPos))
	{
		return hasharray[currentPos].ptr2;
	}

	return nullptr;

}



void HashTable::insert(const string& x, Seats& a)

{
	// Insert x as active
	bool isFound = false;

	int currentPos = findPos(x);

	if (isActive(currentPos))
	{
		isFound = true;
		return;
	}

	if (isFound == false)
	{
		hasharray[currentPos] = Customer(x, a, ACTIVE);
		hasharray[currentPos].ptr2 = &a;

	}


	// enlarge the hash table if necessary

	if (++currentSize >= hasharray.size() / 2)
	{
		cout << "rehashing...";
		rehash();
	}

}

void HashTable::deallocate( string& x, string & row, int & col, string &block_name)
{
	int currentPos = findPos(x);
	Seats newseat;

	bool isFound = false;


	if (!isActive(currentPos))
	{
		return;
	}

	else
	{
		col = hasharray[currentPos].ptr2->column_number;
		row = hasharray[currentPos].ptr2->row_number; //abuksubuk bir sayi geliyor
		block_name = hasharray[currentPos].ptr2->block_name;


		hasharray[currentPos] = Customer("", newseat, DELETED);
	}
}

void HashTable::rehash()

{

	vector<HashTable::Customer> oldArray;
	oldArray = hasharray;

	// Create new double-sized, empty table

	hasharray.resize(nextPrime(2 * oldArray.size()));

	cout << "previous table size:" << oldArray.size() << " new table size: " << hasharray.size() << ", current unique word count " << currentSize << "," << endl;
	cout << "current load factor: " << float(currentSize) / hasharray.size() << endl;


	for (int j = 0; j < hasharray.size(); j++)

		hasharray[j].info = EMPTY;

	// Copy table over

	currentSize = 0;

	for (int i = 0; i < oldArray.size(); i++)

		if (oldArray[i].info == ACTIVE)

			insert(oldArray[i].name, *oldArray[i].ptr2);
}

int HashTable::hash(const string& key, int tablesize) const //key here should be upcoming word
{
	int sum = 0;
	for (int i = 0; i < key.length(); i++)
	{
		sum = sum + key[i];
		return (sum % tablesize);
	}

}

#endif