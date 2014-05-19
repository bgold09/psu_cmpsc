// Complex.h - Complex class declaration
// Written by Brian Golden

#pragma once
#include <iostream>
using namespace std;

// declare user-defined type (UDT) aka Class Complex
template<typename TYPE>
struct Complex
{
	// declare member variables aka data members 
	// aka fields aka properties
	TYPE Re, Im;

	// Default constructor
	Complex()
	{
		Re = Im = 0;
	}

	// Init-constructor
	Complex(TYPE real, TYPE imaginary)
	{
		Re = real;
		Im = imaginary;
	}

	// Copy constructor
	Complex(const Complex& original)
	{
		Re = original.Re;
		Im = original.Im;
	}

	// Member operators
	// binary operator + for adding two complex numbers together
	Complex operator + (const Complex& right) const
	{
		//Complex result(Re + right.Re, Im + right.Im);
		//return result;
		return Complex(Re + right.Re, Im + right.Im);
	}

	// binary operator for adding a real number to a complex number
	Complex operator + (const TYPE& right) const
	{
		return Complex(Re + right, Im);
	}

	// binary operator - for subtracting two complex numbers
	Complex operator - (const Complex& right) const
	{
		return Complex(Re - right.Re, Im - right.Im);
	}

	// binary operator - for subtracting a real number from a complex number
	Complex operator - (const TYPE& right) const
	{
		return Complex(Re - right, Im);
	}

	// binary operator * for multiplying two complex numbers together
	Complex operator * (const Complex& right) const
	{
		return Complex(Re * right.Re, Im * right.Im);
	}

	// binary operator * for multiplying a real number to a complex number
	Complex operator * (const TYPE& right) const
	{
		return Complex(Re * right, Im);
	}

	// binary operator / dividing a complex number by a complex number
	Complex operator / (const Complex& right) const
	{
		return Complex(Re / right.Re, Im / right.Im);
	}
	
	// binary operator / for dividing a complex number by a real number
	Complex operator / (const TYPE& right) const
	{
		return Complex(Re / right, Im);
	}

	// Uniary operator - for negating a complex number
	Complex operator - () const
	{
		return Complex(-Re, -Im);
	}

	// const modifier means the function does NOT modify
	// data members of the class, hence it is SAFE to 
	// call this function on const reference
	TYPE Absolute () const
	{
		return sqrt(Re*Re + Im*Im);
	}

	bool operator == (const Complex& right) const
	{
		return
			(Re == right.Re) && (Im == right.Im);
	}

	bool operator != (const Complex& right) const
	{
		return
			!(*this == right);
	}
	
	bool operator > (const Complex& right) const
	{
		return
			Absolute() > right.Absolute();
	}

	bool operator < (const Complex& right) const
	{
		return
			Absolute() < right.Absolute();
	}

	bool operator >= (const Complex& right) const
	{
		return
			Absolute() >= right.Absolute();
	}

	bool operator <= (const Complex& right) const
	{
		return
			Absolute() <= right.Absolute();
	}

	Complex& operator += (const Complex& right)
	{
		// leverage the existing + operator
		*this = *this + right;
		// ALWAYS return a reference to the current instance of the class
		return *this;
	}

	Complex& operator += (const TYPE& right)
	{
		*this = *this + right;
		return *this;
	}

	Complex& operator -= (const Complex& right)
	{
		*this = *this - right;
		return *this;
	}

	Complex& operator -= (const TYPE& right)
	{
		*this = *this - right;
		return *this;
	}

	Complex& operator *= (const Complex& right)
	{
		*this = *this * right;
		return *this;
	}

	Complex& operator *= (const TYPE& right)
	{
		*this = *this * right;
		return *this;
	}

	Complex& operator /= (const Complex& right)
	{
		*this = *this / right;
		return *this;
	}

	Complex& operator /= (const TYPE& right)
	{
		*this = *this / right;
		return *this;
	}

}; 

template<typename TYPE>
ostream& operator << (ostream& out, const Complex<TYPE>& myComplex)
{
	return 
		out << "Re=" << myComplex.Re << ", Im=" << myComplex.Im;
}