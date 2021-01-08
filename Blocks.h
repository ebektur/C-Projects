#include "Customer.h"

#ifndef hash
#define hash

//blocks should be BST TREE/AVL TREE!
//customer list is hash table



class mySortings //stadium n blocks + one hash table 
{
public:

	explicit mySortings(const string& notFound, int size, vector<Block> copyblock);
	~mySortings();
	mySortings(const mySortings& rhs) : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), array(rhs.array), Customer_Info(rhs.Customer_Info) { }
	void insert(const Block& x);
	inline int leftChild(int i);
	void heapsort(string& row_name);
	void swapentry(Block& a, Block& b);
	void reserve_seat(const string& block_name, const string& row_name, const string& customer_name, int col_num, ofstream& txtOut);
	void reserve_seat_by_row(const string& cust_name, string& row_name, ofstream& txtOut);
	void get_seat(const string& cust_name, ofstream& txtOut);
	void cancel_reservation(string& cust_name, ofstream& txtOut);
	void print(ofstream &txtOut);


	Block & mostvacant() {  //DONT FORGET TO HAVE SIZE+1 BLOCKS.

		return array[array.size()-2];
	}

	void makeEmpty()
	{
		//for (int i = 0; i < array.size(); ++i)
		//{
		//	array[i].~Block();
		//}

		array.clear();
	}



private:
	void addSeat(Block*, const string& row_name, const string& customer_name, int col_num, ofstream& txtOut) const;
	void removeSeat(Block*, const string& row_name, const string& customer_name, int col_num, ofstream& txtOut) const;
	void percDown(vector<Block>& a, int i, int n, string& row_name);
	Block* block_itr;
	vector<Block> array; //array of blocks, will sorted by vacancy in a row
	const string ITEM_NOT_FOUND;
	Block* find(const string& x, Block* t) const;
	int currentSize;
	HashTable* Customer_Info;
	//HashTable customers;
	int seatcount;
	
};

mySortings::mySortings(const string& notFound, int size, vector<Block> copyblock) : ITEM_NOT_FOUND(notFound), currentSize(0), array(size)
{
	//block_itr?
	for (int x = 0; x < size; ++x)
		array[x] = Block();

	for (int y = 1; y < size; y++)
	{
		array[y] = copyblock[y - 1];
		currentSize++;
	}
	
	seatcount = array.size() * array[1].row_size * array[1].column_size;

	Customer_Info = new HashTable(ITEM_NOT_FOUND, seatcount);
	//Customer_Info = &a;

	block_itr = &array[0];
}

void mySortings::insert(const Block& x)
{
	int hole = ++currentSize;
	array[hole] = x;

}


void mySortings::heapsort(string& row_name) //this should sort for vacancy!!
{
	//copy of the vector
	/*vector<SortEntry> heaparray;
	heaparray = array;*/


	// buildHeap
	//for (int i = array.size() / 2; i >= 0; i--)
	//{
	//	percDown(array, i, array.size(), row_name);
	//}

	// sort
	int j = array.size() - 1;

	for (j; j > 0; j--)
	{
		swapentry(array[0], array[j]);  // swap max to the last pos.
		percDown(array, 0, j, row_name); // re-form the heap
	}
}


void mySortings::percDown(vector<Block>& a, int i, int n, string& row_name) //a is our original vector of blocks, sorted by vacancy of the row name
{
	int child;
	int vacancy;
	Block temp = a[i];
	int row_index = a[i].getIndex(row_name, &temp);

	for (vacancy = a[i].getRowVacancy(row_name, &a[i], row_index); leftChild(i) < n; i = child)
	{
		child = leftChild(i);
		if (child != n - 1 && a[child].getRowVacancy(row_name, &a[child], row_index) < a[child + 1].getRowVacancy(row_name, &a[child+1], row_index))
			child++;
		if (a[child].getRowVacancy(row_name, &a[child], row_index) > vacancy)
			a[i] = a[child];
		else
			break;
	}
	a[i] = temp;
}

void mySortings::swapentry(Block& a, Block& b)
{
	Block tmp = a;
	a = b;
	b = tmp;
}


inline int mySortings::leftChild(int i)
{
	return 2 * i + 1;
}
void mySortings::reserve_seat_by_row(const string& cust_name, string& row_name, ofstream& txtOut)
{
	if (Customer_Info->find(cust_name) == ITEM_NOT_FOUND)
	{
		//heapify
		heapsort(row_name); //heapsort by row name
		Block* InsertBlock = &mostvacant();

		if (InsertBlock != NULL)
		{
			//choose the leftmost vacant column
			int row_index = InsertBlock->getIndex(row_name, InsertBlock);
			//int column_index = InsertBlock->getLeftMostColumn(row_index, InsertBlock->seatlist[row_index].size(), InsertBlock);
			int column_index = InsertBlock->getLeftMostColumn(row_index, InsertBlock->column_size, InsertBlock);

			if (row_index && column_index != -1) // if there are seats present
			{
				InsertBlock->seatlist[row_index][column_index].assigned_customer = cust_name;
				InsertBlock->seatlist[row_index][column_index].row_number = row_index;
				InsertBlock->seatlist[row_index][column_index].column_number = column_index;

				//add to the hash table
				Customer_Info->insert(cust_name, InsertBlock->seatlist[row_index][column_index]);

				txtOut << cust_name << " has reserved" << InsertBlock->block_name << " " << row_name << "-" << column_index << " by emptiest block " << endl;

			}
			else
			{
				txtOut << cust_name <<  " could not reserve a seat!" << endl;
			}
		}
	}
	else
	{
		cout << "Customer is already present" << cust_name << "cannot reserve a seat!";
	}
}


void mySortings::reserve_seat(const string& block_name, const string& row_name, const string& customer_name, int col_num, ofstream& txtOut) //reserve_seat
{
	
	if (Customer_Info->find(customer_name) == ITEM_NOT_FOUND)
	{
		addSeat(find(block_name, block_itr), row_name, customer_name, col_num, txtOut);
	}

	else
	{
		txtOut <<  customer_name << "could not reserve a seat!";
	}
}

void mySortings::removeSeat(Block* t, const string& row_name, const string& customer_name, int col_num, ofstream& txtOut) const
{

	//CHECK IF THE SEAT IS ALREADY OCCUPIED!!!
	if (t != NULL)
	{

			t->seatlist[t->getIndex(row_name, t)][col_num].assigned_customer = customer_name; //desired row and col

			//cout << "removed" << endl;

			//add customer info to hash table as well

			//Customer_Info->insert(customer_name, t->seatlist[t->getIndex(row_name, t)][col_num]);
			//customers.insert(customer_name, t->seatlist[t->getIndex(row_name, t)][col_num]);
	}
}



void mySortings::addSeat(Block* t, const string& row_name, const string& customer_name, int col_num, ofstream& txtOut) const
{

	//CHECK IF THE SEAT IS ALREADY OCCUPIED!!!
	if (t != NULL)
	{
		if (t->seatlist[t->getIndex(row_name, t)][col_num].assigned_customer == "---")
		{
			t->seatlist[t->getIndex(row_name, t)][col_num].assigned_customer = customer_name; //desired row and col
			t->seatlist[t->getIndex(row_name, t)][col_num].column_number = col_num;
			t->seatlist[t->getIndex(row_name, t)][col_num].row_number = row_name;
			t->seatlist[t->getIndex(row_name, t)][col_num].block_name = t->block_name;
			//cout << "added" << endl;

			//add customer info to hash table as well

			Customer_Info->insert(customer_name, t->seatlist[t->getIndex(row_name, t)][col_num]);
			//customers.insert(customer_name, t->seatlist[t->getIndex(row_name, t)][col_num]);

			txtOut << customer_name << " has reserved " << t->block_name << " " << row_name << "-" << col_num << endl;
		}

		else
		{
			txtOut << customer_name <<  " could not reserve a seat!" << endl;
		}
	}
}

void mySortings::get_seat(const string & cust_name, ofstream& txtOut)
{
	//Seats temp;
		string name = Customer_Info->find(cust_name);

		if (name != ITEM_NOT_FOUND)
		{
			string block_name = Customer_Info->findSeat(cust_name)->block_name;
			string row_name = Customer_Info->findSeat(cust_name)->row_number;
			int column_number = Customer_Info->findSeat(cust_name)->column_number;
			txtOut << name << " reserved " << block_name <<" " << row_name << "-" << column_number << endl;
		}

		else
		{
			txtOut << "There is no reservation made for " << cust_name << endl;
		}
}

void mySortings::cancel_reservation(string& cust_name, ofstream& txtOut)
{
	string name = Customer_Info->find(cust_name);
	string row_name;
	int col_name;
	string block_name;

	if (name != ITEM_NOT_FOUND)
	{
		Customer_Info->deallocate(cust_name, row_name, col_name, block_name);
		//remove customer from the seatlist


		removeSeat(find(block_name, block_itr), row_name, "---", col_name,txtOut);
		txtOut << "Cancelled the reservation of " << cust_name << endl;

	}

	else
	{
		txtOut << "Could not cancel the reservation for "<< cust_name <<"; no reservation found!" << endl;
	}

}

void mySortings::print(ofstream& txtOut)
{
	for (int j = 1; j < array.size(); j++) //for every block
	{
		txtOut << array[j].block_name << endl;
		txtOut << "~~~~~~~" << endl;

		for (int i = 0; i < array[j].row_size; i++) //for each row
		{
			txtOut << array[j].row_list[i];
			for (int k = 0; k < array[j].column_size; k++) //through column
			{
				txtOut << array[j].seatlist[i][k].assigned_customer.substr(0,3) << "  ";
			}

			txtOut << endl;
		}

		txtOut << "=======" << endl;

	}
	
}

Block* mySortings::find(const string& x, Block* t) const //x = row name,  find the specific block!
{
	//look through the emptiest index
	if (t == NULL)
	{
		return NULL;
	}

	else
	{
		for (int i = 0; i < array.size(); i++)
		{
			if (array[i].block_name == x)
			{
				return &t[i];
			}
		}
	}
}



mySortings::~mySortings()
{
	makeEmpty();
}

#endif

