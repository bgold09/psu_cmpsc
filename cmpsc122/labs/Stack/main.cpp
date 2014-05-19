// main.cpp - main program entry point
// Written by Brian Golden

#include <stack>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

void main ()
{
	try
	{
		stack<char> myStack;
		string expression;
		cout << "Enter a mathematical expression: ";
		getline(cin, expression);

		char aChar;
		stringstream error;
		int i = 0;

		// Run through the expression while not at the end of it 
		// and while there are still matching parenthesis/brace
		for ( ; i < expression.length(); i++)
		{
			// Current character
			aChar = expression[i];

			// Check if there is a closed brace and if the stack is empty, which
			// means that there is no matching open one
			if ( (aChar == ')' || aChar == ']' || aChar == '}') && myStack.empty() )
			{
				error << "Expression is unbalanced because '" << aChar << "' at index " << i << " does not have a matching open character.";
				throw error.str();
			}

			switch(aChar)
			{
				// Add any open parenthesis/brace to top of stack
			case '(':
			case '[':
			case '{':
				myStack.push(aChar);
				break;

			// Check top of stack to see if any closed parenthesis/brace
			// has a matching open one
			case ')':
				// If there is a matching open parenthesis, just take it off
				// the top of the stack and move on
				if (myStack.top() == '(')
					myStack.pop();
				// If there is not a matching open parenthesis, return an error and 
				// exit the loop
				else 
				{
					error << "Closed parenthesis at index " << i << " does not have a matching open parenthesis";
					throw error.str();
				}
				break;

			case ']':
				if (myStack.top() == '[')
					myStack.pop();
				else 
				{
					error << "Closed brace at index " << i << " does not have a matching open brace";
					throw error.str();
				}
				break;

			case '}':
				if (myStack.top() == '{')
					myStack.pop();
				else 
				{
					error << "Closed curly brace at index " << i << " does not have a matching open curly brace";
					throw error.str();
				}
				break;

			default:
				break;
			}
		}

		// Check if stack is still storing an open parenthesis/brace; if it is that 
		// means that it does not have a matching closed one
		if ( !myStack.empty() )
		{
			error << "Expression is unbalanced because '" << aChar << "' at index " << i-1 << " does not have a matching closed character.";
			throw error.str();
		}

		cout << "Expression is balanced." << endl;
	}

	catch (string error)
	{
		cout << error << endl;
	}

	system("pause");

}