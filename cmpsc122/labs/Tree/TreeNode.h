// TreeNode.h - TreeNode struct implementation
// Written by Brian Golden

template<typename T> struct TreeNode
{
   TreeNode(const T& value, TreeNode<T>* left = NULL, TreeNode<T>* right = NULL)
   {
      Value = value;
      Left = left;
      Right = right;
   }

   T Value;
   TreeNode<T>* Left;
   TreeNode<T>* Right;

   bool IsLeaf() const
   {
      return Left == NULL && Right == NULL;
   }

   void Traverse(TreeNode<T>* root)
   {
	   if ( root->IsLeaf() )
		   return;
	   else
	   {
		   Traverse(root->Right);
		   Traverse(root->Left);
	   }
   }

   // Evaluates the tree formed by the mathematical expression
   float ValueOf(TreeNode<char>* root)
   {
	   if ( root->IsLeaf() )
		   // convert character stored in value to its integer equivalent
		   return root->Value - '0';
	   else 
	   {
		   switch (root->Value)
		   {	
		   case ('+'):
			   return ValueOf(root->Left) + ValueOf(root->Right);
		   case ('-'):
			   return ValueOf(root->Left) - ValueOf(root->Right);
		   case ('*'):
			   return ValueOf(root->Left) * ValueOf(root->Right);
		   case ('/'):
			   return ValueOf(root->Left) / ValueOf(root->Right);
		   default:
				string error = "TreeNode::ValueOf(): invalid operand";
				throw error;
		   }
	   }
   }


};