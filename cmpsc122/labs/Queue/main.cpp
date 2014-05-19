// main.cpp - main program entry point
// Written by Brian Golden

#include <queue>
#include <string>
#include <iostream>
using namespace std;

struct OperationPriority
{
	OperationPriority()
	{
	}

	// Init-constructor
	OperationPriority(char operation, int priority)
	{
		Operation = operation;
		Priority = priority;
	}

	char Operation;   
	int Priority;
};

// Stream output operator
ostream& operator << (std::ostream& out, const OperationPriority& anOperationPriority)
{
	return out << '(' << anOperationPriority.Operation << ',' << anOperationPriority.Priority << ')';
}

void main()
{
	queue<OperationPriority> myQueue;
	int nestingLevel = 0;
	char aChar;

	string expression;
	cout << "Enter a mathematical expression: ";
	getline(cin, expression);

	for (int i = 0; i < expression.length(); i++)
	{
		aChar = expression[i];
		// to satisfy compiler
		OperationPriority anOperationPriority;

		switch (aChar)
		{
			case '(': 
				nestingLevel++;
				break;
			case ')': 
				nestingLevel--;
				break;

			case '*': 
			case '/': 
				// Add it to queue including the priority value
				anOperationPriority.Operation = aChar;
				anOperationPriority.Priority = nestingLevel*2 + 2;
				myQueue.push(anOperationPriority);
				break;

			case '+':
			case '-': 
				// Add it to queue including the priority value
				anOperationPriority.Operation = aChar;
				anOperationPriority.Priority = nestingLevel*2 + 1;
				myQueue.push(anOperationPriority);
				break;
		}
	}

	while ( myQueue.size() != 0 )
	{
		OperationPriority frontOpPri = myQueue.front();
		cout << frontOpPri << " ";
		myQueue.pop();
	}
	cout << '\n';


	system("pause");
}

