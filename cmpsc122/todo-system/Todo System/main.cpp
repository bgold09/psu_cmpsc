// main.cpp - main program entry point
// Written by Brian Golden

#include <iostream>
#include <fstream>
#include <string>
#include "TodoList.h"
#include "Todo.h"
#include "Date.h"
using namespace std;

#define NUMBER_OF_INVALID_CHARACTERS	 8
#define NUMBER_OF_VALID_PRIORITIES		 26
#define NOT_FOUND						-1
#define INVALID_CHARACTERS				"{}()[]/,"
#define VALID_PRIORITIES				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define HELP							"help"
#define ADD								"add"
#define DONE							"done"
#define MOVE							"move"
#define LIST							"list"
#define LIST_D							"list-d"
#define LIST_P							"list-p"
#define PRI								"pri"
#define DEPRI							"depri"
#define DATE							"date"
#define TAG								"tag"
#define SEARCH							"search"
#define SEARCH_T						"search-t"

// Display all commands and brief description of their functions
void help ()
{
	cout << "add:       add a new item to your list" << endl;
	cout << "done:      mark an item as completed" << endl;
	cout << "move:      move an item to a new position in the list" << endl;
	cout << "list:      display all items in your list" << endl;
	cout << "list-d:    display items in order of due date (earliest first)" << endl;
	cout << "list-p:    display items in order of priority" << endl;
	cout << "pri:       add a prioritization to an item (e.g. A, B, C, etc.)" << endl;
	cout << "depri:     unprioritize an item" << endl;
	cout << "date:      add a due date to an item" << endl;
	cout << "tag:       add a tag to an item (e.g. work, school, vacation)" << endl;
	cout << "search:    display items containing the given query" << endl;
	cout << "search-t:  display items with given specific tag" << endl << endl;
}

// returns true if the line is empty (i.e. all spaces, a blank
// line in todo.txt)
bool isEmpty(string text)
{
	bool nonSpaceCharacterFound = false;
	for (int i = 0; i < int(text.size()); i++)
		if (text[i] != ' ')
			nonSpaceCharacterFound = true;

	return !nonSpaceCharacterFound;
}

void main ()
{
	try 
	{
		int			currentTodo = 0;
		string		inputCommand, command;
		TodoList	myTodoList;
		Tag			aTag;
		vector<Tag> listOfTags;

		// Open input file
		ifstream input("todo.txt");

		// While not at end of file
		while ( !input.eof() )
		{
			string errorMessage;

			// Get todo items from file
			string inputText;
			getline(input, inputText);

			// if there is a blank line, continue
			if ( isEmpty(inputText) )
				continue;

			Todo* aTodo = new Todo(inputText);

			// IsValid function parses imputText and places each part in 
			// its respective data member (Priority, Date, Text, Tags); if it has
			// an invalid format it will not add it to myTodoList and returns a message
			if ( !aTodo->IsValidAndParse(errorMessage) )
				cout << "Todo item #" << currentTodo+1 << " is invalid, not added to the list: " << errorMessage << endl;
			else
				myTodoList.Push(aTodo);

			currentTodo++;
		}

		cout << "Welcome to your todo list. Start using the system, or type 'help' for a list of available commands." << endl;
		cout << "Type 'exit' at any time to close the interface." << endl << endl;

		do 
		{
			cout << "action: ";
			getline(cin, inputCommand);
			string command = inputCommand;		
			// make command all lowercase to match case of if statements
			for (int i = 0; i < int(inputCommand.size()); i++)
				command[i] = tolower(command[i]);

			if (command == HELP)
				help();
			else if (command == ADD)
				myTodoList.Add();
			else if (command == DONE)
			{
				if (myTodoList.Empty())
					cout << "There are no todos in your list." << endl;

				else
				{
					int todoNumber;
					cout << "Mark which todo as completed? ";
					cin >> todoNumber;

					while ( todoNumber < 1 || todoNumber > myTodoList.GetSize() )
					{
						cout << "That number todo does not exist. Enter another: ";
						cin >> todoNumber;
					}
					myTodoList.Remove(todoNumber-1);
				}
			}
			else if (command == MOVE)
			{
				if (myTodoList.Empty())
					cout << "There are no todos in your list." << endl;

				else
				{
					int todoToMove, newLocation;
					cout << "Move which todo? ";
					cin >> todoToMove;

					while ( todoToMove < 1 || todoToMove > myTodoList.GetSize() )
					{
						cout << "That number todo does not exist. Enter another: ";
						cin >> todoToMove;
					}

					cout << "Move to what number in the list? ";
					cin >> newLocation;
					while ( newLocation < 1 || newLocation > myTodoList.GetSize() )
					{
						cout << "That number todo does not exist. Enter another: ";
						cin >> todoToMove;
					}

					myTodoList.Move(todoToMove-1, newLocation-1);
				}

			}


			else if (command == LIST)
			{
				if (myTodoList.Empty())
					cout << "There are no todos in your list." << endl;
				else
					cout << myTodoList << endl;
			}
			else if (command == LIST_D)
			{
				if (myTodoList.Empty())
					cout << "There are no todos in your list." << endl;
				else
					myTodoList.PrintByDate();
			}
			else if (command == LIST_P)
			{
				if (myTodoList.Empty())
					cout << "There are no todos in your list." << endl;
				else
					myTodoList.PrintByPriority();
			}
			else if (command == PRI)
			{
				if (myTodoList.Empty())
					cout << "There are no todos in your list." << endl;

				else
				{
					int todoNumber;
					cout << "Add a priority to which todo? ";
					cin >> todoNumber;
					while ( todoNumber < 1 || todoNumber > myTodoList.GetSize() )
					{
						cout << "That number todo does not exist. Enter another: ";
						cin >> todoNumber;
					}

					myTodoList[todoNumber-1].AddPriority();
				}
			}
			else if (command == DEPRI)
			{
				if (myTodoList.Empty())
					cout << "There are no todos in your list." << endl;
				else
				{
					int todoNumber;
					cout << "Remove priority from which todo? ";
					cin >> todoNumber;
					while ( todoNumber < 1 || todoNumber > myTodoList.GetSize() )
					{
						cout << "That number todo does not exist. Enter another: ";
						cin >> todoNumber;
					}
					cin.ignore();
					myTodoList[todoNumber-1].RemovePriority();
					cout << endl;
				}
			}

			else if (command == DATE)
			{
				if (myTodoList.Empty())
					cout << "There are no todos in your list." << endl;
				else
				{
					int todoNumber;
					cout << "Add due date to which todo? ";
					cin >> todoNumber;
					while ( todoNumber < 1 || todoNumber > myTodoList.GetSize() )
					{
						cout << "That number todo does not exist. Enter another: ";
						cin >> todoNumber;
					}
					cin.ignore();
					myTodoList[todoNumber-1].AddDueDate();
				}
			}
			else if (command == TAG)
			{
				if (myTodoList.Empty())
					cout << "There are no todos in your list." << endl;
				int todoNumber;
				cout << "Add tag to which todo? ";
				cin >> todoNumber;
				while ( todoNumber < 1 || todoNumber > myTodoList.GetSize() )
				{
					cout << "That number todo does not exist. Enter another: ";
					cin >> todoNumber;
				}
				cin.ignore();

				myTodoList[todoNumber-1].AddTag();
			}
			else if (command == SEARCH)
			{
				string searchText;
				cout << "Search for: ";
				getline(cin, searchText);
				myTodoList.Search(searchText);
			}
			else if (command == SEARCH_T)
			{
				string searchText;
				cout << "Search for: ";
				getline(cin, searchText);
				myTodoList.SearchByTag(searchText);
			}
			else if (command == "exit")
				break;
			else
				cout << "'" << command << "' is not a valid command." << endl << endl;
		}
		while (command != "exit");

		// open file
		ofstream output("todo.txt");

		// User is finished, overwrite todo.txt with the todos that remain
		if ( !myTodoList.Empty() )
			for (int i = 0; i < int(myTodoList.GetSize()); i++)
				output << myTodoList[i] << endl;
	}

	catch (string error)
	{
		cout << error << endl;
	}

	system("pause");
}