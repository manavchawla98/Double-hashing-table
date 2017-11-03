#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H


/****************************************
 * UW User ID:  m6chawla
 * Submitted for ECE 250
 * Semester of Submission:  (Fall) 2017
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 ****************************************/

#include "Exception.h"
#include "ece250.h"

enum state { EMPTY, OCCUPIED, DELETED };

template<typename T>
class DoubleHashTable {
	private:
		int count; 			
		int power;			
		int array_size;
		T *array;
		state *array_state;

		int h1( T const & ) const; // first hash function
		int h2( T const & ) const; // second hash function

	public:
		DoubleHashTable( int = 5 );
		~DoubleHashTable();
		int size() const;				// returns the number of entries in the hash table
		int capacity() const;			// returns the capacity of the hash table	
		bool empty() const;				// returns true if the table is empty, false if not
		bool member( T const & ) const;	// returns true/false depending on whether parameter is an entry in the table or not
		T bin( int ) const;				// returns the entry in table[n]

		void print() const;				// prints the table

		void insert( T const & );		// insert passed parameter into table, throw overflow exception if table is already full
		bool remove( T const & );		// remove passed parameter from table and return true. Returns false if parameter is not an entry in the table
		void clear();					// clears the entire table
};

template<typename T >
DoubleHashTable<T >::DoubleHashTable( int m ):					// constructor definition
count( 0 ), power( m ),
array_size( 1 << power ),
array( new T [array_size] ),
array_state( new state[array_size] ) {

	for ( int i = 0; i < array_size; ++i ) {
		array_state[i] = EMPTY;
	}
}

template<typename T >
DoubleHashTable<T >::~DoubleHashTable() {						// destructor definition

	delete [] array;											
	delete [] array_state;
}

template<typename T >
int DoubleHashTable<T >::size() const {
	return count;
}

template<typename T >
int DoubleHashTable<T >::capacity() const {
	return array_size;
}


template<typename T >
bool DoubleHashTable<T >::empty() const {

	if(count==0)
		return true;
	else 
		return false;

}

template<typename T >
int DoubleHashTable<T >::h1( T const &obj ) const {

	int i=static_cast<int>(obj);	// typecasting; % works only with int and as we are using a template class, the passed parameter can be of any data type
	int probe;
	probe=i%array_size;

	if(probe<0)										// if negative, add M to it
		probe+=array_size;

	return probe;
}

template<typename T >
int DoubleHashTable<T >::h2( T const &obj ) const {

	int i=static_cast<int>(obj);		// same reason as in h1()

	int offset;
	offset= (i/array_size)%array_size;

	if(offset<0)										// if negative, add M to it
		offset+=array_size;
	if(offset%2==0)										// if even, add 1 to make odd
		offset++;

	return offset;
}

template<typename T >
bool DoubleHashTable<T >::member( T const &obj ) const {	

	if(count==0)
		return false;

	int index=h1(obj);
	int offset=h2(obj);
	int counter=0;

	
	if(array[index]==obj){
		return true;
	}

	while(array[index]!=obj){

		counter++;
		index=(index+offset)%array_size;
		if(array[index]==obj)
			return true;

		if(counter==count)				// will return false if the obj is not a member of the table and break the while loop
			return false;
	}



}

template<typename T >
T DoubleHashTable<T >::bin( int n ) const {                    
	return array[n];
}

template<typename T >
void DoubleHashTable<T >::insert( T const &obj ) {

	if(count==array_size){			// throw overflow exception if table is already full
		throw overflow();
	}
	else {
		count++;
		int probe, offset;
		probe=h1(obj);

		offset=h2(obj);
		while(array_state[probe]==OCCUPIED)
			probe=(probe+offset)%array_size;

		array_state[probe]=OCCUPIED;		// update bin status 
		array[probe]=obj;
		

		}
	
	return;
}

template<typename T >
bool DoubleHashTable<T >::remove( T const &obj ) {

	
	int index=h1(obj);
	int offset=h2(obj);

	if(count==0)
		return false;
	else if(member(obj)){
		while(array[index]!=obj){
			index=(index+offset)%array_size;
		}
		count--;
		array_state[index]=DELETED;		// update bin status
		array[index]=NULL;
		return true;

	}
	else 
		return false;
	

}

template<typename T>
void DoubleHashTable<T >::clear() {

	 for(int i=0; i<array_size;i++){
	 	array_state[i]=EMPTY;				// update bin status
	 }
	 count=0;
	 	
	 return; 
}

template<typename T >
void DoubleHashTable<T >::print() const {

	for(int i=0; i<array_size; i++){
		if(array_state[i]==OCCUPIED)									
			std::cout << array[i] << " ";
	}
	return;
}

#endif