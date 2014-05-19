// Todo.h - Todo class declaration
// An item with text and associated priority, due date, and tags
// Written by Brian Golden

#pragma once
#include <string>
#include <vector>
#include "Tag.h"
#include "Date.h"
using namespace std;

class Todo
{
public:
	// Default constructor
	Todo();

	// Init-constructor
	Todo(const string& text);

	// Init-constructor 2
	Todo(const string& text, char priority, const vector<Tag>& tagList, const Date& dueDate);

	// Checks if todo is valid and sets data members
	bool IsValidAndParse(string& errorMessage);

	// Give the Todo a priority tag (e.g. A, B)
	void AddPriority();

	// Remove the priority tag
	void RemovePriority();

	// Assign a due date
	void AddDueDate();

	// Assign a tag
	void AddTag();

	// Get priority
	char GetPriority() const;

	// Get due date
	Date GetDate() const;

	// Get text of Todo
	string GetText() const;

	// Get the vector of tags
	vector<Tag> GetTags() const;

	// Returns true if Text conatains searchText (case insensitive)
	bool ContainsText(const string& searchText) const;

	// True if a tag is found to contain searchText
	bool TagsContainsText(const string& searchText) const;

	// Used to set color of output in console
	enum Color { DARKBLUE = 1, DARKGREEN, DARKTEAL, DARKRED, DARKPINK, 
		DARKYELLOW, GRAY, DARKGRAY, BLUE, GREEN, TEAL, RED, PINK, YELLOW, WHITE };

	// Sets color of output in console
	void SetColor(Color aColor) const;

private:
	// checks for valid characters
	bool CheckForValidPriority(char aChar) const;

	// Searches for aChar, returns true if found in text
	bool CharacterFound(const string& text, char aChar) const;

	// Move past any leading space characters
	void MovePastWhitespace(const string& text, int& index) const;

	// Remove whitespace from end of text
	void RemoveEndWhitespace(string& text) const;

	// Remove whitespace from beginning of text
	void RemoveStartWhitespace(string& text) const;

	// checks for invalid characters
	bool CheckForInvalidCharacters(const string& text) const;

	// Returns false when tags start or end with 
	// a comma, or if two commas appear in a row
	bool CheckCommas(const string& text) const;

	// returns true if date is correctly formatted
	bool ValidDate(const string& text) const;

	// Data members
	string Text;
	char Priority;
	vector<Tag> MyTags;
	Date DueDate;
};

// Stream output operator
std::ostream& operator << (std::ostream& out, const Todo& aTodo);