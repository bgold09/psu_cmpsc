// TreeNode.h - TreeNode struct implementation
// Written by Brian Golden
using namespace std;

template<typename T> struct TreeNode
{
	TreeNode(const T& data, TreeNode<T>* left = NULL, TreeNode<T>* right = NULL)
	{
		Data = data;
		Left = left;
		Right = right;
	}

	T Data;
	TreeNode<T>* Left;
	TreeNode<T>* Right;

	bool IsLeaf() const
	{
		return Left == NULL && Right == NULL;
	}

	// Checks to see if the node has only one child
	bool hasOneChild() const
	{
		if ( (Left == NULL && Right != NULL) || (Left != NULL && Right == NULL) )
			return true;
		return false;
	}

	template<typename T>void Insert(TreeNode<T>*& root, const T& insertData)
	{
		if (root == NULL)
			root = new TreeNode<T> (insertData);
		else
		{
			if (root->Data == insertData)
				return;
			else if (root->Data > insertData)
				Insert(root->Left, insertData);
			else
				Insert(root->Right, insertData);
		}
	}

	template<typename T> void Print(TreeNode<T>* root)
	{
		if ( root == NULL )
			return;
		else
		{
			Print(root->Left);
			cout << root->Data << " ";
			Print(root->Right);
		}
	}

	bool operator < (const &TreeNode<T>* compareTo)
	{
		return (*this->Data < compareTo->Data);
	}

	bool operator > (const &TreeNode<T>* compareTo)
	{
		return (*this->Data > compareTo->Data);
	}

	bool operator == (const &TreeNode<T>* compareTo)
	{
		return (*this->Data == compareTo->Data);
	}

	bool operator != (const &TreeNode<T>* compareTo)
	{
		return (*this->Data != compareTo->Data);
	}
};