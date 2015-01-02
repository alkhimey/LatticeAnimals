#include <string>
#include <sstream>
#include "bigint.h"

using std::string;
using std::istringstream;
using std::ostringstream;
using std::cout;
using std::endl;

BigInt::BigInt(){
	for (int i=0; i<size; i++)
		number[i]=0;
}

BigInt::BigInt(const BigInt& source){
	for (int i=0; i<size; i++)
		number[i]=source.number[i];
}

BigInt::BigInt(int source){
	for (int i=0; i<size; i++)
		number[i]=0;
	int i=0;
	while (source!=0){
		number[i]=source%limit;
		source/=limit;
		i++;
	}
}

BigInt::~BigInt(){}

BigInt& BigInt::operator=(const BigInt& rhs){
	for (int i=0; i<size; i++)
		number[i]=rhs.number[i];
		return *this;
}

BigInt BigInt::operator++(int){
	BigInt temp=*this;
	++(*this);
	return temp;
}
BigInt& BigInt::operator++(){
	for (int i=0; i<size; i++){
		number[i]++;
		if (number[i]!=0) return *this;
	}
	return *this;
}

BigInt& BigInt::operator+=(const BigInt& rhs){
	unsigned int carry=0;
	for (int i=0; i<size; i++){
		number[i]+=(rhs.number[i]+carry);
		if (number[i]>=limit){
			carry=1;
			number[i]%=limit;
		}
		else
			carry=0;
	}
	return *this;
}

BigInt& BigInt::operator-=(const BigInt& rhs){
	if (*this<rhs){
		*this=rhs-*this;
		for (int i=0; i<size; i++)
			number[i]*=-1;
		return *this;
	}
	unsigned int carry=0;
	for (int i=0; i<size; i++){
		number[i]-=(rhs.number[i]+carry);
		if (number[i]<0){
			carry=1;
			number[i]+=limit;
		}
		else
			carry=0;
	}
	return *this;
}

ostream& BigInt::print(ostream& o) const{
	for (int i=size-1; i>0; i--)
		if (number[i]!=0) {
			for (int j=i; j>=0; j--){
				o.fill('0');
				if (j!=i)
					o.width(limitDigits);
				o<<number[j];
			}
			return o;
		}
		o << number[0];
		return o;
}
bool BigInt::operator==(const BigInt& other) const{
	for (int i=0; i<size; i++)
		if (number[i]!=other.number[i])
			return false;
	return true;
}
ostream& operator << (ostream& o, const BigInt& source){
	return (source.print(o));
}

BigInt operator+(const BigInt& lhs, const BigInt& rhs){
	BigInt temp=lhs;
	temp+=rhs;
	return temp;
}

BigInt operator-(const BigInt& lhs, const BigInt& rhs){
	BigInt temp=lhs;
	temp-=rhs;
	return temp;
}

bool BigInt::operator>=(const BigInt& other) const{
	for (int i=size-1; i>=0; i--){
		if (number[i]>other.number[i])
			return true;
		if (number[i]<other.number[i])
			return false;
	}
	return true;
}


bool BigInt::operator>(const BigInt& rhs) const{
	for (int i=size-1; i>=0; i--){
		if (number[i]>rhs.number[i])
			return true;
		if (number[i]<rhs.number[i])
			return false;
	}
	return false;
}

bool BigInt::operator<=(const BigInt& rhs) const{
	for (int i=size-1; i>=0; i--){
		if (number[i]<rhs.number[i])
			return true;
		if (number[i]>rhs.number[i])
			return false;
	}
	return true;
}
bool BigInt::operator<(const BigInt& rhs) const{
	for (int i=size-1; i>=0; i--){
		if (number[i]<rhs.number[i])
			return true;
		if (number[i]>rhs.number[i])
			return false;
	}
	return false;
}
bool BigInt::operator!=(const BigInt& rhs) const{
	for (int i=0; i<size; i++)
		if (number[i]!=rhs.number[i])
			return true;
	return false;
}

istream& BigInt::read(istream& o){
	string numberString;
	o>>numberString;
	
	ostringstream tempStringStream;
	tempStringStream.fill('0');
	tempStringStream.width(limitDigits*size);
	tempStringStream<<numberString;
	numberString=tempStringStream.str();
	for (int i=0; i<size; i++){
		istringstream temp(numberString.substr(i*limitDigits,limitDigits));
		temp>>number[size-1-i];
	}
	return o;
}

istream& operator >> (istream& o, BigInt& num){
	return (num.read(o));
}
