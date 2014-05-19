// main.cpp - Email checker main program entry point
// Written by Brian Golden
// Final Version 02 February 2011

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

#define MAX_SIZE							260
#define NOT_FOUND							-1
#define NO_ERROR							-1
#define NUMBER_OF_VALID_CHARACTERS			83

// encoded error codes
#define GREATER_THAN_MAX_LENGTH				0
#define AT_ISSUE							1
#define GREATER_THAN_LOCALPART_MAX_LENGTH	2
#define	INVALID_CHARACTERS					3
#define	LOCALPART_DASH_RULE					4
#define LOCALPART_DOT_RULE					5
#define GREATER_THAN_DOMAIN_MAX_LENGTH		6
#define	DOMAIN_DASH_RULE					7
#define DOMAIN_DOT_RULE						8
#define INVALID_TLD							9
#define UNREGISTERED_TLD					10

// decoded error codes
#define DECODED_ERROR_CODES \
{"email length > 254 characters", "@ issue", "localPart length > 64 characters",  \
 "one or more characters was invalid", "localPart violates Dash Rule", \
 "localPart violates Dot Rule", "domain > 64 characters",			   \
 "domain violates Dash Rule", "domain violates Dot Rule",			   \
 "TLD invalid", "unregistered TLD"}

// list of valid characters
#define VALID_CHARACTERS \
{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&*+-/=?^_`{}~.'"}

// Returns length of text (by searching for position of null terminator)
int GetLength(char* text) 
{
	int pos = 0;
	for ( ; text[pos] != '\0'; pos++);
	return pos;
}

// Searches text for aChar and returns its index
// returns -1 (NOT_FOUND) when aChar is not found in text
// Find(email + atIndex, '.')
int Find(char* text, char aChar)
{
	int textLength = GetLength(text);
	int pos = 0;
	for ( ; pos < textLength && *(text + pos) != aChar; pos++);
	if ( *(text + pos) == aChar)
		return pos;
	else 
		return NOT_FOUND;
}

// Returns false if emailPart contains characters not found in validCharacters
bool CheckForValidCharacters(char* emailPart, char* validCharacters, int emailPartLength)
{	
	int validCharacterIndex = 0;
	// check each character against the list of valid ones
	for (int emailPos = 0; emailPos < emailPartLength; emailPos++)
	{
		char currentCharacter = *(emailPart + emailPos);
		validCharacterIndex = Find(validCharacters, currentCharacter);
		if (validCharacterIndex == NOT_FOUND)
			return false;
	}
	return true;
}

// Returns false when emailPart violates the Dot Rule 
// or Dash Rule, whichever is specizfied by aChar
bool CheckRule(char* emailPart, char aChar, int emailPartLength)
{
	// returns false if first character is aChar
	if ( *emailPart == aChar)
		return false;

	// returns false if last character is aChar
	if ( *(emailPart + emailPartLength - 1) == aChar)
		return false;

	// returns false if aChar is immediately followed by another instance of aChar
	for (int i = 0; i < emailPartLength; i++)
		if ( *(emailPart + i) == aChar)
			if ( *(emailPart + i + 1) == aChar)
				return false;

	return true;
}

// Checks the top-level domain against the list 
// of registered ones provided by the ICANN list
bool CheckForValidTLD (char* topLevelDomain)
{
	// - open ICANN list
	ifstream tld_list ("tld_list.txt");

	// - while we are not at end of tld_list file
	while ( !tld_list.eof() )
	{
		char valid_TLD[MAX_SIZE];

		// - Read TLD from tld_list file
		tld_list.getline(valid_TLD, MAX_SIZE);

		// - Chech valid_TLD against topLevelDomain
		// - get length of topLevelDomain
		int topLevelDomainLength = GetLength(topLevelDomain);
		int valid_TLD_Length = GetLength(valid_TLD);

		if (valid_TLD_Length != topLevelDomainLength)
			continue;

		// if a character in topLevelDomain is not the same
		// as the corresponding one in valid_TLD, then
		// topLevelDomain is not a valid TLD
		bool validChar = true;
		int i = 0;
		for ( ; i < topLevelDomainLength && validChar == true; i++) {
			char aChar = topLevelDomain[i];
			aChar = toupper(aChar);
			if ( aChar != valid_TLD[i] )
				validChar = false;
		}

		// check to see why porgram exited loop:
		// If the last char in both the source and 
		// registered TLD are the same, then they are
		// the same. If not, then they are not the same
		// and the source TLD is invalid
		if ( topLevelDomain[i] == valid_TLD[i] )
		{
			tld_list.close();
			return true;
		}
		else 
			continue;
	}
	return false;
}

void Copy(char* source, char* destination, int startPosition, int endPosition)
{
	// copy chars from source to destination
	int i = 0;
	for ( ; i < endPosition; i++)
		destination[i] = source[startPosition + i];

	// copy null terminator
	destination[i] = '\0';
}

// Returns true when email address is valid
bool IsValid(char* email, int& errorCode)
{
	// 1. Check email size, if larger than 254 return false
	int emailLength = GetLength(email); 
	if (emailLength > 254)
	{
		errorCode = GREATER_THAN_MAX_LENGTH;
		return false;
	}

	// 2. Find '@' and place its position in atIndex
	//	  - if '@' is not or in invalid position found return false
	int atIndex = Find(email, '@');
	if (atIndex == NOT_FOUND || atIndex == 0 || atIndex == emailLength-1)
	{
		errorCode = AT_ISSUE;
		return false;
	}
	// 3. localPart = substring of email starting from position 0 
	//				  until position atIndex
	char localPart[MAX_SIZE];
	Copy (email, localPart, 0, atIndex);
	
	// 4. domain = substring from position atIndex+1
	//			   until end of email string
	char domain[MAX_SIZE];
	Copy (email, domain, atIndex+1, emailLength-1);

	// SUBTASK -- Make sure LocalPart is valid
	// - make sure localPart length is under 65
	int localPartLength = GetLength(localPart);
	if (localPartLength > 64)
	{
		errorCode = GREATER_THAN_LOCALPART_MAX_LENGTH;
		return false;
	}

	// - make sure localPart has no invalid characters
	char validCharacters[NUMBER_OF_VALID_CHARACTERS] = VALID_CHARACTERS;
	if ( !CheckForValidCharacters(localPart, validCharacters, localPartLength) )
	{
		errorCode = INVALID_CHARACTERS;
		return false;
	}

	// - check dash rule (cannot begin or end with dash, two in a row)
	if ( !CheckRule(localPart, '-', localPartLength) )
	{
		errorCode = LOCALPART_DASH_RULE;
		return false;
	}

	// - check dot rule (cannot begin or end with dot, two in a row)
	if ( !CheckRule(localPart, '.', localPartLength) )
	{
		errorCode = LOCALPART_DOT_RULE;
		return false;
	}

	// SUBTASK -- Make sure domain is valid
	// - make sure domain length is under 254
	int domainLength = GetLength(domain);
	if (domainLength > 253)
	{
		errorCode = GREATER_THAN_DOMAIN_MAX_LENGTH;
		return false;
	}

	// - make sure domain has no invalid characters
	if ( !CheckForValidCharacters(domain, validCharacters, domainLength) )
	{
		errorCode = INVALID_CHARACTERS;
		return false;
	}

	// - check dash rule
	if ( !CheckRule(domain, '-', domainLength) )
	{
		errorCode = DOMAIN_DASH_RULE;
		return false;
	}

	// - check dot rule
	if ( !CheckRule(domain, '.', domainLength) )
	{
		errorCode = DOMAIN_DOT_RULE;
		return false;
	}

	// SUBTASK -- Make sure Top-Level Domain (TLD) is valid
	// - find last '.' in domain
	int lastDotIndex = NOT_FOUND;
	for (int i = 0 ; i < domainLength; i++)
		if (domain[i] == '.')
			lastDotIndex = i;
	if (lastDotIndex == NOT_FOUND)
	{
		errorCode = INVALID_TLD;
		return false;
	}

	if ( domain[lastDotIndex - 1] == '-' )
	{
		errorCode = INVALID_TLD;
		return false;
	}

	// - topLevelDomain = from last '.' to end of domain
	char topLevelDomain[MAX_SIZE];
	Copy (domain, topLevelDomain, lastDotIndex+1, domainLength-1);

	// - match TLD against ICANN list 
	if ( !CheckForValidTLD (topLevelDomain) ) 
	{
		errorCode = UNREGISTERED_TLD;
		return false;
	}

	return true;
}

void main () 
{
	// 1) Open input file, open output file
	ifstream input ("Email.txt");
	ofstream output("Result.txt");

	// 2) While we are not at end of input file
	while ( !input.eof() )
	{
		char email[MAX_SIZE];
		int errorCode = NO_ERROR;
		string decodedErrorCodes[] = DECODED_ERROR_CODES;

		// 3) Read email from input file
		if ( !input.getline(email, MAX_SIZE) )
		{
		   // clear error flag from stream
		   input.clear();

		   // ignore the rest of the line
		   input.ignore(std::numeric_limits<streamsize>::max(),'\n');
		}

		// 4) Validate email
		if ( IsValid(email, errorCode) )
			// 5) Write result to output
			output << "1" <<  "										" << email << endl;			
		else
		{
			// print the corresponding error code along with the 0
			string decodedError = decodedErrorCodes[errorCode];
			output << "0	" << left << setw(35) << decodedError << "	" << email << endl;
		}
	}

	system("pause");
}