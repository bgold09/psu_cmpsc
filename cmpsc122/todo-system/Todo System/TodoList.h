// TodoList.h - TodoList class declaration;
// Container for Todos
// Written by Brian Golden

#pragma once
#include <string>
#include <vector>
#include "Todo.h"

class TodoList
{
public:
	// Default constructor, does nothing
	TodoList () {}

	// Adds a todo to the list
	void Push(Todo* aTodo);

	// Add a new Todo to the list, prompts for each part
	void Add();

	// Remove the given Todo at pos
	void Remove(int index);

	// Move aTodo to location index
	void Move(int todoToMove, int newIndex);
	
	// Print items with earliest date first
	void PrintByDate() const;

	// Print items with highest priority first
	void PrintByPriority() const;
	
	// Search for items that contain text
	// and display them to the user
	void Search(const string& text) const;

	// Search for items that have tags that 
	// contain text and display them to the user
	void SearchByTag(const string& searchText) const;

	// Returns a vector of Todos
	vector<Todo*> GetTodoList () const;

	// Returns length of encapsulated vector
	int GetSize() const;

	// Index operator
	Todo& operator [] (int index) const;

	// Returns true if there are no todos in the list
	bool Empty() const;

private:
	// checks for invalid characters
	bool CheckForInvalidCharacters(const string& text) const;

	// Searches for aChar, returns true if found in text
	bool CharacterFound(const string& text, char aChar) const;

	// Remove whitespace from end of text
	void RemoveEndWhitespace(string& text) const;

	// Remove whitespace from beginning of text
	void RemoveStartWhitespace(string& text) const;

	// returns true if date is correctly formatted
	bool ValidDate(const string& text) const;

	// Data member
	vector<Todo*> MyTodos;
};

// Stream output operator
std::ostream& operator << (std::ostream& out, const TodoList& aTodoList);