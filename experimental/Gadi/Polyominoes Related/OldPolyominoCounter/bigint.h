#ifndef BIGINT_H
#define BIGINT_H
#include <iostream>
using std::ostream;
using std::istream;

/**
	@author Gadi Aleksandrowicz <gadial@tx.technion.ac.il>
*/

class BigInt
{
public:
	// ctors and dtor
	BigInt();
	BigInt(const BigInt&);
	BigInt(int);
	virtual ~BigInt();
	// operators
	BigInt& operator=(const BigInt&);
	BigInt operator++(int);
	BigInt& operator++();
	BigInt& operator+=(const BigInt&);
	BigInt& operator-=(const BigInt&);

	bool operator>=(const BigInt&) const;
	bool operator>(const BigInt&) const ;
	bool operator<=(const BigInt&) const;
	bool operator<(const BigInt&) const ;
	bool operator==(const BigInt&) const;
	bool operator!=(const BigInt&) const;
	
	//io functions
	ostream& print(ostream&) const;
	istream& read(istream&);
	static const int size=4;
	static const unsigned int limit=1000000000;
	static const unsigned int limitDigits=9;
	
private:
	int number[size];
};

	ostream& operator << (ostream&, const BigInt&);
	istream& operator >> (istream&, BigInt&);
	BigInt operator+(const BigInt&, const BigInt&);
	BigInt operator-(const BigInt&, const BigInt&);
#endif /*BIGINT_H_*/
