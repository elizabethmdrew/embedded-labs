/****************************************************************************
 * This code is copyright (c) 2017 by the University of Waterloo and may    *
 * not be redistributed without explicit written permission.                *
 ****************************************************************************/
 
 /*
 * Names:
 *   1.  uWaterloo User ID:  stskaric@uwaterloo.ca
 *   2.  uWaterloo User ID:  emdrew@uwaterloo.ca
 */

/*****************************************************************
*
*                   Exercise 4.1 - C Formatting and Style
*
*****************************************************************
* Instructions: Record 5 changes you made to the code to improve
*               the formatting and style here. Include the 
*               function in which the change was made and a brief
*               justification for the change. You may make changes 
*               to main.c or bst.c.
***************************************************************** n
*  1. Indentation
*  2. Spacing in brackets (function parameters)
*  3. Comments spacing, location and capitalization
*  4. if/else statement style (no brackets for in line) 
*  5. Spaces between functions
****************************************************************/

/*****************************************************************
*
*                   Exercise 6.1 - Logic Bugs
*
*****************************************************************
* Instructions: Record 4 logic bugs you found here. Include the 
*               function where the bug was found and a brief
*               description of how you used the tools to find and
*               fix it. They can be in main.c or bst.c.
*****************************************************************
*  1. Replaced single & with double && - (Previously bitwise operator, did not affect output)
*  2. While loop in case 3 delete logic was incorrect - accessing wrong node (left instead of right for max node)
*  3. Children placement logic was incorrect in case 2 delete - (trying to place child node from deleted curr node incorrectly)
*  4. Size increment instead of decrement when deleting - (tree size is incorrect)
****************************************************************/

#include <stdio.h>
#include "bst.h"

//Helper functions
void bsn_init(bsn_t *node, S32 val) 
{
	node->val = val;
	node->left = NULL;
	node->right = NULL;
}

//Initialize the binary search tree so that it is empty. Run time: &Theta(1)
void bst_init(bst_t *tree) 
{
	tree->root = NULL;
	tree->size = 0;
}

//Remove all nodes from this binary search tree. Run time: T(n) with T(n) memory. 
//Note: Remember that you do not have to keep a search-tree structure while you are 
//destroying the tree. If you maintain the search-tree structure, the run time will be O(n2). 
//This run time is not required for 2014.
void bst_destroy(bst_t *tree) {}

//Return the number of nodes in the binary search tree. Run time: &Theta(1)
size_t bst_size(bst_t *tree) 
{
	size_t treeSize = 0;
	treeSize = tree->size;
	return treeSize;
}

//Insert the integer n into the binary search tree and return false if n is 
//already in the tree (do not add a duplicate into the tree) and true otherwise. 
//Ideally, such a function should not be implemented using recursion. 
//See the marking scheme at the end. Run time: O(h)
bool bst_insert(bst_t *tree, S32 val) 
{	
	bsn_t *p_currNode = tree->root;
	bsn_t *p_prevNode = p_currNode;
	bsn_t *p_newNode = (bsn_t *)malloc(sizeof(bsn_t));
	
	bsn_init(p_newNode, val);
	
	//Special case: inserting at the root node
	if(p_currNode == NULL) 
	{
		tree->root = p_newNode;
		tree->size = tree->size + 1;	
		return __TRUE;
	}

	//Anything but root
	while (p_currNode != NULL) 
	{
		if (p_currNode->val == val) 
		{ 
			//The value is already in the tree, don't insert again
			free(p_newNode);
			return __FALSE;
		}
		else if (val < p_currNode->val) 
		{
			//Move left in tree
			p_prevNode = p_currNode;
			p_currNode = p_currNode->left;
		}
		else 
		{ 
		  //Move right in tree
			p_prevNode = p_currNode;
			p_currNode = p_currNode->right;
		}
	}
	
	//p_prevNode now points to the last node before the leaf where we are inserting
	//insert as left child
	if(p_prevNode->val > val) 
		p_prevNode->left = p_newNode;
	//Insert as right child
	else 
		p_prevNode->right = p_newNode;
	
	tree->size = tree->size + 1;
	
	return __TRUE;
}

//Returns the smallest integer in the binary search tree. Return INT_MAX if the tree is empty. Run time: O(h)
S32 bst_min(bst_t *tree) 
{
	bsn_t *p_currNode = tree->root;
	
	if(tree->size == 0) 
		return INT_MAX;
	else 
	{ 
		//Smallest int will be the left-most bottom node
		while(p_currNode->left != NULL) 
			p_currNode = p_currNode->left;
	}
	
	return p_currNode->val;
}

//Returns the largest integer in the binary search tree. Return INT_MIN if the tree is empty. Run time: O(h)
S32 bst_max(bst_t *tree) 
{
	bsn_t *p_currNode = tree->root;
	
	if(tree->size == 0)
	{
		return INT_MIN;
	}
	else
	{ 
		//Largest int will be the right-most bottom node
		while(p_currNode->right != NULL) 
			p_currNode = p_currNode->right;
	}
	
	return p_currNode->val;
}

//Perform the actual deletion of the node
//There are three possible cases:
//1. It is a leaf node
//2. It has only one child
//3. It has two children
void bst_delete (bsn_t *p_currNode, bsn_t *p_parentNode)
{
	bsn_t *p_swapNode = NULL;
	bsn_t *p_swapParent = NULL;
	
	//Case 1: leaf node, just delete
	if((p_currNode->right == NULL) && (p_currNode->left == NULL)) 
	{
		//Update the pointer on the parent node to NULL
		if(p_parentNode->left == p_currNode) 
			p_parentNode->left = NULL;
		else 
			p_parentNode->right = NULL;
	}
	
	//Case 3: two children, pick one to replace with
	//balancing was not a parameter, so we will always take max of left branch to replace it with
	else if ((p_currNode->right != NULL) && (p_currNode->left != NULL)) 
	{
		p_swapNode = p_currNode->left;
		p_swapParent = p_currNode;
		
		//Traverse until we find the max of the subtree
		while(p_swapNode->right != NULL) 
		{
			p_swapParent = p_swapNode;
			p_swapNode = p_swapNode->right; 
		}
		
		//We now have the max of the subtree. By definition, it can only have one left or no children.
		//if it is a leaf, we can simply copy the value into the current node and delete the leaf instead
		p_currNode->val = p_swapNode->val;
		
		if(p_swapParent->left == p_swapNode) 
			p_swapParent->left = p_swapNode->left; 
		else p_swapParent->right = p_swapNode->left;
			p_currNode = p_swapNode;
	}
	//Case 2: one child, replace with child, delete child
	else
	{
		//Child is to the left
		if(p_currNode->left != NULL) 
		{
			// need to know whether we are to the left or right of the parent
			if(p_parentNode->left == p_currNode)
				p_parentNode->left = p_currNode->left;
			
			else
				p_parentNode->right = p_currNode->left;
		}
		else //Child is to the right
		{
			//Need to know whether we are to the left or right of the parent
			if(p_parentNode->left == p_currNode)
				p_parentNode->left = p_currNode->right; 
			else
				p_parentNode->right = p_currNode->right; 
		}
	}
	p_currNode->left = NULL;
	p_currNode->right = NULL;
	p_currNode->val = 0;
	free(p_currNode);
}

//If the object is in the binary search tree, remove it and return true; otherwise, return false and do nothing. Run time: O(h)
bool bst_erase(bst_t *tree, S32 val) 
{
	bsn_t *p_currNode = tree->root;
	bsn_t *p_parentNode = p_currNode;
	
	while (p_currNode != NULL)
	{
		if (p_currNode->val == val)
		{
			//We are removing the root with one child left
			if(p_parentNode == p_currNode) 
			{
				if((p_currNode->left != NULL) && (p_currNode->right != NULL))  
					//Can swap in from left as usual
					bst_delete ( p_currNode, p_parentNode);
				else
				{
					if ((p_currNode->left == NULL) && (p_currNode->right == NULL)) 
						//Last element in tree
						tree->root = NULL;
					else if (p_currNode->left != NULL) 
						tree->root = p_currNode->left;
					else 
						tree->root = p_currNode->right;
					free(p_currNode);
				}
			}
			else 
				bst_delete (p_currNode, p_parentNode);
			//Decrement size of tree
			tree->size -= 1; 
	
			return __TRUE;
		}
		else if (val < p_currNode->val) 
		{ 
			//Move left in tree
			p_parentNode = p_currNode;
			p_currNode = p_currNode->left;
		}
		else 
		{ 
			//Move right in tree
			p_parentNode = p_currNode;
			p_currNode = p_currNode->right;
		}
	}
	
	return __FALSE;
}
