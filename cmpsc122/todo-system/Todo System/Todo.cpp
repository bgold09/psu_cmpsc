// Todo.cpp - Todo class implementation
// An item with text and associated priority, due date, and tags
// Written by Brian Golden

#include <string>
#include <vector>
#include <iostream>
#include <windows.h>
#include "Todo.h"
#include "Date.h"
HANDLE hCon;

#define NUMBER_OF_VALID_PRIORITIES								26
#define NOT_FOUND											   -1
#define NUMBER_OF_INVALID_CHARACTERS							8
#define JANUARY													1
#define FEBRUARY												2
#define MARCH													3
#define APRIL													4
#define MAY														5
#define JUNE													6
#define JULY													7
#define AUGUST													8
#define SEPTEMBER												9
#define OCTOBER													10
#define NOVEMBER												11
#define DECEMBER												12
#define DAYS_IN_APRIL_JUNE_SEPTEMBER_NOVEMBER					30
#define DAYS_IN_JANUARY_MARCH_MAY_JULY_AUGUST_OCTOBER_DECEMBER	31
#define DAYS_IN_FEBRUARY										29
#define INVALID_CHARACTERS										"{}()[]/,"
#define VALID_PRIORITIES										"ABCDEFGHIJKLMNOPQRSTUVWXYZ"

// Default constructor
Todo::Todo ()
{
	Priority = NULL;
}

// Init-constructor
Todo::Todo(const string& text)
{
	Text = text;
	Priority = NULL;
}

// Init-constructor 2
Todo::Todo(const string& text, char priority, const vector<Tag>& tagList, const Date& dueDate)
{
	Text = text;
	Priority = priority;
	MyTags = tagList;
	DueDate = dueDate;
}

// Checks if text is valid and sets each part
// of the todo; Returns true if it was a valid Todo
bool Todo::IsValidAndParse(string& errorMessage)
{
	string		text = Text;
	int			indexOfText = 0;
	Tag			aTag;
	vector<Tag> listOfTags;

	// Move past any leading space characters
	RemoveStartWhitespace(text);
	RemoveEndWhitespace(text);

	int	todoLength = text.length();	// For performance
	
	// May not start with following chars, invalid format
	if (text[0] == '{' || text[0] == '}' || text[0] == ')' || text[0] == ']')
	{
		errorMessage = "Invalid format.";
		return false;
	}

	// 1. Check for priority tag
	if ( text[indexOfText] == '(' )
	{
		int closedParenthesisIndex = NOT_FOUND;
		for (int i = 0; i < int(text.size()); i++)
			if ( text[i] == ')' )
				closedParenthesisIndex = i;
		
		// Invalid if no matching parenthesis or if closed is last char;
		// todo cannot contain only a priority
		if (closedParenthesisIndex == NOT_FOUND || closedParenthesisIndex == text.size()-1)
		{
			errorMessage = "Invalid format";
			return false;
		}

		char aPriority = toupper(text[indexOfText+1]);
		if ( CheckForValidPriority(aPriority) )
		{
			Priority = aPriority;
			indexOfText += 3;
		}
		else 
		{
			errorMessage =  "Priority tag '";
			errorMessage += text[indexOfText+1];
			errorMessage += "' is an invalid character. ";
			return false;
		}
	}

	// 2. Get due date
	int i = indexOfText;
	int openBracketIndex = NOT_FOUND;
	for ( ; i < todoLength && openBracketIndex == NOT_FOUND; i++)
		if (text[i] == '[')
			openBracketIndex = i;

	if (openBracketIndex != NOT_FOUND)
	{
		int closedBracketIndex = NOT_FOUND;
		for ( ; i < todoLength && closedBracketIndex == NOT_FOUND; i++)
			if (text[i] == ']')
				closedBracketIndex = i;

		if ( closedBracketIndex == NOT_FOUND || closedBracketIndex == text.size()-1 )
		{
			errorMessage = "Invalid date format, no closed bracket found. ";
			return false;
		}

		string dateText = text.substr(openBracketIndex+1, closedBracketIndex-openBracketIndex-1);
		if ( !ValidDate(dateText) )
		{
			errorMessage = "Invalid date format. ";
			return false;
		}

		DueDate.Set(dateText);
		indexOfText = closedBracketIndex+1;
	}

	MovePastWhitespace(text, indexOfText);

	// 3. Get Text of the todo
	i = indexOfText;
	int openBraceIndex = NOT_FOUND;
	for ( ; i < todoLength && openBraceIndex == NOT_FOUND; i++)
		if (text[i] == '{')
			openBraceIndex = i;

	// If no open brace found, set Text as everything remaining in the input text
	if (openBraceIndex == NOT_FOUND)
	{
		Text = text.substr(indexOfText);
		RemoveEndWhitespace(Text);
		// check for invalid chars
		if (!CheckForInvalidCharacters(Text))
		{
			errorMessage = "Invalid characters found.";
			return false;
		}
		return true;
	}
	// Otherwise, '{' should mark the beginning of tag list, so set
	// Text as everything up to that index
	else
	{
		Text = text.substr(indexOfText, i-indexOfText-1);
		RemoveEndWhitespace(Text);
		indexOfText = i;
		if (!CheckForInvalidCharacters(Text))
		{
			errorMessage = "Invalid characters found in tags.";
			return false;
		}
	}

	// 4. Get tags
	int closedBraceIndex;
	openBraceIndex = indexOfText-1;
	closedBraceIndex = NOT_FOUND;

	// - Find closed brace; marks end of tag list
	for (i = indexOfText+1; i < todoLength; i++)
		if ( text[i] == '}' )
			closedBraceIndex = i;

	// - todo is invalid if there is nothing between the braces
	// or if no closed brace is found
	if ( closedBraceIndex == NOT_FOUND || openBraceIndex == closedBraceIndex-1 )
	{
		errorMessage = "Invalid tag format. ";
		return false;
	}

	// invalid if nonspace chars are found after the closed brace;
	// nothing should come after the tags, if any
	for (int i = closedBraceIndex+1; i < todoLength; i++)
		if (text[i] != ' ')
		{
			errorMessage = "No characters may follow the closed brace. ";
			return false;
		}
		// - tags is substring from open brace to closed brace; includes all tags
		string tags = text.substr(openBraceIndex+1, closedBraceIndex-openBraceIndex-1);

		// Check to see if only spaces exist between braces
		int lastNonSpaceCharacter = NOT_FOUND;
		for (int i = 0; i < int(tags.length()); i++)
			if (tags[i] != ' ')
				lastNonSpaceCharacter = i;

		if (lastNonSpaceCharacter == NOT_FOUND || !CheckCommas(tags))
		{
			errorMessage = "Invalid tag format. ";
			return false;
		}
		// - Look for last comma
		int lastCommaIndex = NOT_FOUND;
		for (int pos = 0; pos < int(tags.length()); pos++)
			if ( tags[pos] == ',' )
				lastCommaIndex = pos;

		// - If no comma found, there is only one tag
		if ( lastCommaIndex == NOT_FOUND )
		{
			aTag = tags.substr(0);
			listOfTags.push_back(aTag);
			MyTags = listOfTags;
		}
		else
		{
			int previousCommaIndex = NOT_FOUND;
			int nextCommaIndex = NOT_FOUND;
			for (int pos = 0; pos < int(tags.size()) || nextCommaIndex == NOT_FOUND; pos += previousCommaIndex)
			{
				Tag aTag;
				string temp;
				nextCommaIndex = NOT_FOUND;
				// Find index of the next ','			
				if (previousCommaIndex == NOT_FOUND)
				{
					for (int i = pos; i < int(tags.size()); i++)
						if (tags[i] == ',')
							nextCommaIndex = i;
				}
				else
					for (int i = previousCommaIndex+1; i < int(tags.size()); i++)
						if (tags[i] == ',')
							nextCommaIndex = i;

				// If no previous comma was found yet, substring = characters from beginning of 
				// tags until next comma
				if ( previousCommaIndex == NOT_FOUND && nextCommaIndex != NOT_FOUND )
					temp = tags.substr(0, nextCommaIndex);
				// else substring = characters from previous until next comma
				else if ( previousCommaIndex != NOT_FOUND && nextCommaIndex != NOT_FOUND )
					temp = tags.substr(previousCommaIndex + 1, nextCommaIndex - previousCommaIndex - 1);
				// else substring = characters from previous comma until end of tags
				else
				{
					temp = tags.substr(previousCommaIndex+1);
					nextCommaIndex = tags.size();				// to exit the loop
				}

				// Remove any spaces from beginning and end of tag text
				RemoveEndWhitespace(temp);
				RemoveStartWhitespace(temp);

				// Insert aTag into vector listOfTags
				aTag = temp;
				listOfTags.push_back(aTag);

				// Look for next comma after current nextCommaIndex
				previousCommaIndex = nextCommaIndex;
			}
			MyTags = listOfTags;
		}
		return true;
}

// Give the Todo a character priority tag (e.g. A, B)
void Todo::AddPriority ()
{
	char priority;
	cout << "Enter priority tag (A-Z are valid): ";
	cin >> priority;
	priority = toupper(priority);

	// Valid if a letter a-z
	while ( !CheckForValidPriority(priority) )
	{
		cout << "Invalid priority. Enter another: ";
		cin >> priority;
		priority = toupper(priority);
	}

	Priority = priority;
}

// Remove the priority tag
void Todo::RemovePriority ()
{
	Priority = NULL;
}

// Assign a due date
void Todo::AddDueDate ()
{
	string dateText;
	cout << "Enter a due date [mm/dd format]: ";
	getline(cin, dateText);

	while ( !ValidDate(dateText) )
	{
		cout << "Invlaid date format, enter another: ";
		getline(cin, dateText);
	}
	DueDate.Set(dateText);
}

// Assign one or more tags
void Todo::AddTag ()
{
	Tag aTag;
	string tagText;
	// Add aTag to the vector of tags [strings]
	cout << "Enter tag: " << ends;

	getline(cin, tagText);
	aTag = tagText;
	// Valid if it doesn't contain any invalid chars (#defined)
	while ( !aTag.IsValid() )
	{
		cout << "Invalid tag format, enter another: ";
		getline(cin, tagText);
		aTag = tagText;
	}

	// Add new tag to MyTags
	MyTags.push_back(aTag);
	cout << endl;
}

// Return priority tag
char Todo::GetPriority () const
{
	return Priority;
}

//Return due date of the todo
Date Todo::GetDate() const
{
	return DueDate;
}

// Get text of Todo
string Todo::GetText() const
{
	return Text;
}

// Return the vector of tags
vector<Tag> Todo::GetTags() const
{
	return MyTags;
}

// Returns true if Text conatains searchText (case insensitive)
bool Todo::ContainsText(const string& searchText) const
{
	int searchTextSize = searchText.size();
	string searchFor = searchText;

	// make searchText all lowercase
	for (int c = 0; c < int(searchText.size()); c++)
		searchFor[c] = tolower(searchText[c]);

	// Search as long as substring of searchTextSize chars 
	// can be made without going out of bounds
	for (int i = 0; i < int(Text.size()-searchTextSize+1); i++)
	{
		// make a temp string as long as searchText
		string temp = Text.substr(i, searchTextSize);
		// make temp all lowercase
		for (int c = 0; c < int(temp.size()); c++)
			temp[c] = tolower(temp[c]);

		// True if searchText and substring text of the todo are the same
		if (temp == searchFor)
			return true;
	}
	return false;
}

// Look for searchText in the tag list
bool Todo::TagsContainsText(const string& searchText) const
{
	int searchTextSize = searchText.size();
	string searchFor = searchText;

	// make searchText all lowercase
	for (int c = 0; c < int(searchText.size()); c++)
		searchFor[c] = tolower(searchText[c]);

	for (int i = 0; i < int(MyTags.size()); i++)
	{
		if ( MyTags[i].ContainsText(searchText) )
			return true;
	}
	return false;
}

// True if aChar is letter a-z
bool Todo::CheckForValidPriority(char aChar) const
{
	string validPriorities = VALID_PRIORITIES;

	// check the character against the list of valid ones
	for (int i = 0; i < NUMBER_OF_VALID_PRIORITIES; i++)
	{
		char currentCharacter = validPriorities[i];
		// True if aChar matches a char in VALID_PRIORITIES
		if ( currentCharacter == aChar )
			return true;
	}
	return false;
}

// Searches for aChar, returns true if found in text
bool Todo::CharacterFound(const string& text, char aChar) const
{
	int pos = 0;
	for ( ; text[pos] != '\0' && text[pos] != aChar; pos++);
	if ( text[pos] == aChar)
		return true;
	else 
		return false;
}

// Move past any space characters
void Todo::MovePastWhitespace(const string& text, int& index) const
{
	for ( ; text[index] == ' '; index++);
}

// Remove whitespace from end of text (space characters)
void Todo::RemoveEndWhitespace(string& text) const
{
	string temp = text;
	int lastNonSpaceCharacter;
	for (int i = 0; i < int(text.length()); i++)
		if (text[i] != ' ')
			lastNonSpaceCharacter = i;
	text = temp.substr(0, lastNonSpaceCharacter+1);
}

// Remove whitespace from beginning of text
void Todo::RemoveStartWhitespace(string& text) const
{
	string temp = text;
	int firstNonSpaceCharacter = NOT_FOUND;
	for (int i = 0; i < int(text.length()) && firstNonSpaceCharacter == NOT_FOUND; i++)
		if (text[i] != ' ')
			firstNonSpaceCharacter = i;
	text = temp.substr(firstNonSpaceCharacter);
}

// checks for invalid characters
bool Todo::CheckForInvalidCharacters(const string& text) const
{
	string invalidCharacters = INVALID_CHARACTERS;

	for (int i = 0; i < NUMBER_OF_INVALID_CHARACTERS; i++)
	{
		char invalidChar = invalidCharacters[i];
		// look for invalidChar within text
		if ( CharacterFound(text, invalidChar) )
			return false;
	}

	return true;
}

// Returns false when tags start or end with 
// a comma, or if two commas appear in a row
bool Todo::CheckCommas(const string& tags) const
{
	int length = tags.length();
	// returns false if first character is comma
	if ( tags[0] == ',')
		return false;

	// returns false if last character is comma
	if ( tags[length - 1] == ',')
		return false;

	// returns false if comma is immediately followed by another instance of aChar
	for (int i = 0; i < ( length-1 ); i++)
		if ( tags[i] == ',')
			if ( tags[i + 1] == ',')
				return false;

	return true;
}

// returns true if date is correctly formatted
bool Todo::ValidDate(const string& text) const
{
	int day = 0;
	int month = 0;
	string dayText, monthText;

	// format should be dd/mm, so no greater than 5 characters; 
	// may be less though (e.g. 3/12, 2/7, etc.)
	if (text.size() > 5)
		return false;

	// Look for '/' character
	int slashIndex = NOT_FOUND;
	bool slashFound = false;
	for (int i = 0; i < int(text.size()); i++)
	{
		if ( text[i] == '/' && !slashFound )
			slashIndex = i;
		// return false if a slash has already been found
		else if ( text[i] == '/' && slashFound )
			return false;
	}

	// Invalid if '/' was not found, or if first or last character
	if ( slashIndex == NOT_FOUND || slashIndex == 0 || slashIndex == text.size()-1 )
		return false;

	// check that there are only numbers and '/' char
	for (int i = 0 ; i < int(text.size()); i++)
	{
		char currentChar = text[i];
		if ((text[i] > '9' || text[i] < '0') && text[i] != '/')
			return false;
	}

	// day is from beginning to slash; month from slash to end
	monthText = text.substr(0, slashIndex);
	dayText = text.substr(slashIndex+1);

	if (monthText.size() == 1)
		month = monthText[0] - '0';
	else
	{
		if (monthText[0] == '0')
			month = monthText[1] - '0';
		else if (monthText[0] == '1')
			// subtract '0' to get int value
			month = 10 + (monthText[1] - '0');
		else 
			return false;
	}

	if (dayText.size() == 1)
		day = dayText[0] - '0';
	else
	{
		if (dayText[0] == '0')
			day = dayText[1] - '0';
		else if (dayText[0] == '1' || dayText[0] == '2' || dayText[0] == '3')
			// subtract '0' to get int value
			day = 10*(dayText[0] - '0') + (dayText[1] - '0');		
		else 
			return false;
	}

	if ( (day <= 0 || day > DAYS_IN_JANUARY_MARCH_MAY_JULY_AUGUST_OCTOBER_DECEMBER) || (month < JANUARY || month > DECEMBER) )
		return false;

	// 31 days in the following months
	if ( (month == JANUARY || month == MARCH || month == MAY || month == JULY || month == AUGUST || 
		month == OCTOBER || month == DECEMBER) && (day > DAYS_IN_JANUARY_MARCH_MAY_JULY_AUGUST_OCTOBER_DECEMBER) )
		return false;

	// 30 days in the following months
	if ( (month == APRIL || month == JUNE || month == SEPTEMBER || month == NOVEMBER) && 
		(day > DAYS_IN_APRIL_JUNE_SEPTEMBER_NOVEMBER) )
		return false;

	// 29 possible days in the February (assumes that user knows 
	// whether or not it is a leap year)
	if ( month == FEBRUARY && day > DAYS_IN_FEBRUARY )
		return false;

	return true;
}

// Sets color of output in console
void Todo::SetColor(Color aColor) const
{
	if (hCon == NULL)
		hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon, aColor);
}

// Stream output operator; prints in the following format:
// "(Priority tag) [Duedate Text] {Tag_1 / Tag_2 / Tag_3}
// ex: "(A) [4/17] write essay #3 {history, computer}
std::ostream& operator << (std::ostream& out, const Todo& aTodo)
{
	char			myPriority = aTodo.GetPriority();
	Date			myDueDate = aTodo.GetDate();
	string			todoText = aTodo.GetText();
	vector<Tag>		tagList = aTodo.GetTags();
	
	// If the todo has a priority print it
	if (myPriority != NULL)
	{
		// If priority is A, B, C, or D print colors for added emphasis
		switch (myPriority)
		{
		case 'A':
			aTodo.SetColor(Todo::YELLOW);
			out << "(" << myPriority << ") ";
			break;
		case 'B':
			aTodo.SetColor(Todo::GREEN);
			out << "(" << myPriority << ") ";
			break;
		case 'C':
			aTodo.SetColor(Todo::RED);
			out << "(" << myPriority << ") ";
			break;
		case 'D':
			aTodo.SetColor(Todo::TEAL);
			out << "(" << myPriority << ") ";
			break;
		}
	}

	// If the todo has a due date print it
	if ( !myDueDate.IsEmpty() )
		out << myDueDate << " ";

	out << todoText << " ";

	// Print tags by traversing vector of tags, as long as there are tags available to print
	if ( !tagList.empty() )
	{
		int pos = 0;
		out << "{";
		for (; pos < int(tagList.size()-1); pos++ )
			out << tagList[pos] << ", ";
		out << tagList[pos] << "}";
	}
	aTodo.SetColor(Todo::GRAY);			// reset output color
	return out;
}