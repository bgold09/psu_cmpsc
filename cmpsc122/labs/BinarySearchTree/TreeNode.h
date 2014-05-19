// TreeNode.h - TreeNode struct implementation
// Written by Brian Golden

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

};