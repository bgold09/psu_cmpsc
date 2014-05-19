// TldPart.h - email local part class declaration
// Written by Brian Golden

#pragma once

#include "SubdomainPart.h"
#include "String.h"
#include "Vector.h"

// TldPart is subclass of SubdomainPart
class TldPart: public SubdomainPart
{
public:
	// Default constuctor, does nothing
	TldPart() {}

	// Init-constructor
	TldPart(const String& address, Vector<String> validTlds);

	// Performs TLD validation
	virtual bool IsValid();

private:
	// Address derived from SubdomainPart
	Vector<String> ValidTlds;
};