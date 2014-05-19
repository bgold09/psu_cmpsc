// TodoList.cpp - TodoList class implementation
// Container for Todos
// Written by Brian Golden

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TodoList.h"
#include "Tag.h"
#include "Date.h"

#define NUMBER_OF_INVALID_CHARACTERS							8
#define NOT_FOUND											   -1
#define VALID_DATE_LENGTH										5
#define	VALID_DATE_MONTH_LENGTH									2
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
#define	NINE													9
#define TEN														10
#define INVALID_CHARACTERS										"{}()[]/,"
#define VALID_PRIORITIES										"ABCDEFGHIJKLMNOPQRSTUVWXYZ"

// Adds a todo to the list
void TodoList::Push(Todo* aTodo)
{
	MyTodos.push_back(aTodo);
}

// Add a new Todo to the list
void TodoList::Add ()
{
	string todoText, tags, dateText;
	char priority;
	vector<Tag> tagList;
	Date dueDate;

	string validPriorities = VALID_PRIORITIES;

	cout << "Enter a task: ";
	getline(cin, todoText);

	// validate text
	while ( !CheckForInvalidCharacters(todoText) || todoText == "" )
	{
		cout << "Invalid text of the todo. Enter a new one: ";
		getline(cin, todoText);
	}

	cout << "Give this task a priority [type 0 if no priority]: ";
	cin >> priority;
	if ( priority != '0' )
	{
		priority = toupper(priority);

		while ( !CharacterFound(validPriorities, priority) || priority == '0' )
		{
			cout << "Invalid priority. Enter another [or 0 for no priority]: ";
			cin >> priority;
			if (priority != '0')
				priority = toupper(priority);
		}
	}
	cin.ignore();
	if (priority == '0')
		priority = NULL;

	cout << "Add tag(s) [comma-seperated, 0 if no tags]: ";
	getline(cin, tags);
	if (tags != "0")
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
			tagList.push_back(aTag);

			// Look for next comma after current nextCommaIndex
			previousCommaIndex = nextCommaIndex;
		}
	}

	cout << "Assign a due date (dd/mm format) [0 if no due date]: ";
	getline(cin, dateText);
	if (dateText != "0")
	{
		RemoveStartWhitespace(dateText);
		RemoveEndWhitespace(dateText);

		// validate due date
		while ( !ValidDate(dateText) || dateText == "0" )
		{
			cout << "Invalid due date. Enter another [0 if no due date]: ";
			getline(cin, dateText);
			RemoveStartWhitespace(dateText);
			RemoveEndWhitespace(dateText);
		}

		if (dateText != "0")
			dueDate.Set(dateText);
	}

	MyTodos.push_back(new Todo(todoText, priority, tagList, dueDate));
	cout << "Todo added"  << endl << endl;
}

// Remove the given Todo at pos
void TodoList::Remove (int index)
{
	// open file in append mode
	ofstream archive("done.txt", ios::app);

	// get todo at pos and remove it
	Todo* aTodo = MyTodos[index];
	MyTodos.erase(MyTodos.begin() + index);

	// add its text to done.txt
	archive << "xx " << *aTodo << endl;
	cout << "Todo " << index+1 << " marked as done." << endl;
}

// Move aTodo to location index
void TodoList::Move (int todoToMove, int newIndex)
{
	Todo* temp = MyTodos[todoToMove];
	vector<Todo*>::iterator deletePos = MyTodos.begin() + todoToMove;
	MyTodos.erase(deletePos);
	vector<Todo*>::iterator insertPos = MyTodos.begin() + newIndex;
	MyTodos.insert(insertPos, temp);
}

// Print items with earliest date first
void TodoList::PrintByDate() const
{
	vector<Todo*> todos = MyTodos;
	vector<Todo*> temp;
	
	for (vector<Todo*>::iterator pos = todos.begin(); pos != todos.end(); pos++)
	{
		Todo* aTodo = *pos;
		// Add any todo that has a due date
		if ( !aTodo->GetDate().IsEmpty() )
			temp.push_back(aTodo);
	}

	// sort Todos, highest priority first (A, B, ..., Y, Z)
	for ( int i = 0; i < int(temp.size()-1); i++ )
		for ( int j = i + 1; j < int(temp.size()); j++ )
			if ( temp[j]->GetDate() < temp[i]->GetDate() )
				swap(temp[j], temp[i]);

	// Add items with no due date to end
	for (vector<Todo*>::iterator pos = todos.begin(); pos != todos.end(); pos++)
	{
		Todo* aTodo = *pos;
		if ( aTodo->GetDate().IsEmpty() )
			temp.push_back(aTodo);
	}

	// Print sorted todos
	for (int i = 0; i < int(temp.size()); i++)
		cout << *temp[i] << endl;
	cout << endl;
}

// Print items with highest priority first
void TodoList::PrintByPriority() const
{
	vector<Todo*> todos = MyTodos;
	vector<Todo*> temp;
	// Add any todo that has a priority
	for (vector<Todo*>::iterator pos = todos.begin(); pos != todos.end(); pos++)
	{
		Todo* aTodo = *pos;
		if (aTodo->GetPriority() != NULL)
			temp.push_back(aTodo);
	}

	// sort Todos, highest priority first (A, B, ..., Y, Z)
	for ( int i = 0; i < int(temp.size()-1); i++ )
		for ( int j = i + 1; j < int(temp.size()); j++ )
			if ( temp[j]->GetPriority() < temp[i]->GetPriority() )
				swap(temp[j], temp[i]);

	// Add items with no priority to end
	for (vector<Todo*>::iterator pos = todos.begin(); pos != todos.end(); pos++)
	{
		Todo* aTodo = *pos;
		if (aTodo->GetPriority() == NULL)
			temp.push_back(aTodo);
	}
	
	// Print sorted todos
	for (int i = 0; i < int(temp.size()); i++)
		cout << *temp[i] << endl;
	cout << endl;
}

// Search for items that contain searchText and display 
// them to the user (case insensitive)
void TodoList::Search (const string& searchText) const
{
	vector<Todo*> tempList;

	// Find all todos with Text that contains text
	for (int i = 0; i < int(MyTodos.size()); i++)
	{
		Todo aTodo = *MyTodos[i];
		if (aTodo.ContainsText(searchText))
			tempList.push_back(MyTodos[i]);
	}

	if ( !tempList.empty() )
	{
		for (int i = 0; i < int(tempList.size()); i++)
			cout << *tempList[i] << endl;
		cout << endl;
	}
	else 
		cout << "No todos containing '" << searchText << "' were found." << endl << endl;
}

// Search for items that have tags that 
// contain text and display them to the user
void TodoList::SearchByTag(const string& searchText) const
{
	vector<Todo*> tempList;

	// Find all todos with tags that contains searchText
	for (int i = 0; i < int(MyTodos.size()); i++)
	{
		Todo aTodo = *MyTodos[i];
		if ( aTodo.TagsContainsText(searchText) )
			tempList.push_back(MyTodos[i]);
	}

	if ( !tempList.empty() )
	{
		for (int i = 0; i < int(tempList.size()); i++)
			cout << *tempList[i] << endl;
		cout << endl;
	}
	else 
		cout << "No todos containing '" << searchText << "' were found." << endl << endl;
}

// Returns the vector of Todos
vector<Todo*> TodoList::GetTodoList () const
{
	return MyTodos;
}

// Returns length of encapsulated vector
int TodoList::GetSize () const
{
	return int(MyTodos.size());
}

// Index operator
Todo& TodoList::operator [] (int index) const
{
	int size = GetSize();
	if ( (index < 0) || (index >= size) )
	{
		stringstream error;
		error << "Vector::operator[] Index " << index << " out of bounds (0.." << (size-1) << ")";
		throw error.str();
	}
	vector<Todo*> aTodoList = this->MyTodos;

	return *aTodoList[index];
}

// Returns true if there are no todos in the list
bool TodoList::Empty () const
{
	return MyTodos.empty();
}

// checks if text contains only valid characters
bool TodoList::CheckForInvalidCharacters(const string& text) const
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

// Searches for aChar, returns true if found in text
bool TodoList::CharacterFound(const string& text, char aChar) const
{
	for (int i = 0; i < int(text.size()); i++)
		if ( text[i] == aChar)
			return true;

	return false;
}

// Remove whitespace from end of text
void TodoList::RemoveEndWhitespace(string& text) const
{
	string temp = text;
	int lastNonSpaceCharacter;
	for (int i = 0; i < int(text.length()); i++)
		if (text[i] != ' ')
			lastNonSpaceCharacter = i;
	text = temp.substr(0, lastNonSpaceCharacter+1);
}

// Remove whitespace from beginning of text
void TodoList::RemoveStartWhitespace(string& text) const
{
	string temp = text;
	int firstNonSpaceCharacter = NOT_FOUND;
	for (int i = 0; i < int(text.length()) && firstNonSpaceCharacter == NOT_FOUND; i++)
		if (text[i] != ' ')
			firstNonSpaceCharacter = i;
	text = temp.substr(firstNonSpaceCharacter);
}

// returns true if date is correctly formatted
bool TodoList::ValidDate(const string& text) const
{
	int day = 0;
	int month = 0;
	string dayText, monthText;

	// format should be dd/mm, so no greater than 5 characters; 
	// may be less though (e.g. 3/12, 2/7, etc.)
	if (text.size() > VALID_DATE_LENGTH)
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
		if (text[i] > '9' || text[i] < '0')
			if (text[i] != '/')
				return false;

	// day is from beginning to slash; month from slash to end
	monthText = text.substr(0, slashIndex);
	dayText = text.substr(slashIndex+1);

	// month or day may not be, for example "102"
	if (dayText.size() > VALID_DATE_MONTH_LENGTH || monthText.size() > VALID_DATE_MONTH_LENGTH)
		return false;

	if (monthText.size() == 1)
		// subtract '0' to get int value
		month = monthText[0] - '0';
	else
	{
		if (monthText[0] == '0')
			month = monthText[1] - '0';
		else
			month = TEN + (monthText[1] - '0');
	}

	if (dayText.size() == 1)
		day = dayText[0] - '0';
	else
	{
		if (dayText[0] == '0')
			day = dayText[1] - '0';
		else
			day = TEN*(dayText[0] - '0') + (dayText[1] - '0');		
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

// Stream output operator
std::ostream& operator << (std::ostream& out, const TodoList& aTodoList)
{
	for (int i = 0; i < aTodoList.GetSize(); i++)
	{
		// Print any todos with number less than 10 (index 9)
		// as "01", "02" etc.
		if (i < NINE)
			out << '0' << i+1 << " " << aTodoList[i] << endl;
		else 
			out << i+1 << " " << aTodoList[i] << endl;
	}
	return out;
}