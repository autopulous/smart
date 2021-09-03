/*----------------------------------------------------------------------------
  Smart Tree
 
  Copyright 2010 John L. Hart IV. All rights reserved.
 
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
 
  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
 
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
 
  THIS SOFTWARE IS PROVIDED BY John L. Hart IV ``AS IS'' AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
  NO EVENT SHALL John L. Hart IV OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.
 
  The views and conclusions contained in the software and documentation are
  those of the authors and should not be interpreted as representing official
  policies, either expressed or implied, of John L Hart IV.
  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Smart Tree internal header file
  ----------------------------------------------------------------------------*/

#ifndef SMART_TREE_H
#define SMART_TREE_H

/*----------------------------------------------------------------------------
  Private defines
  ----------------------------------------------------------------------------*/

#define REBALANCE_THRESHOLD 3

#define LESS_THAN -1
#define GREATER_THAN 1

/*----------------------------------------------------------------------------
  Private data types
  ----------------------------------------------------------------------------*/

typedef void * smartTreeKeyHandle;
typedef void * smartTreeDataHandle;

typedef struct smartTreeNode {
	struct smartTreeNode * parent;
  
	struct smartTreeNode * lesser;
	unsigned long lesserNullNodes;

	struct smartTreeNode * greater;
	unsigned long greaterNullNodes;

	smartTreeKeyHandle key;
	size_t keySize;

	smartTreeDataHandle data;
	size_t dataSize;

	unsigned long instance;
} smartTreeNode;

typedef smartTreeNode * smartTreeNodeHandle;

typedef struct smartTree {
	smartTreeNodeHandle root;

	long (* compareKeyFunction)(const smartTreeKeyHandle pKey1, const smartTreeKeyHandle pKey2);
	
	size_t memoryMaximum;
	size_t memoryAllocated;
} smartTree;

typedef smartTree * smartTreeHandle;

/*----------------------------------------------------------------------------
  Private function prototypes
  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  NodeCompare()
  ----------------------------------------------------------------------------
  Compare the key values of a pair of nodes
  ----------------------------------------------------------------------------
  Parameters:

  pTree  - (I) The tree handle
  pNode1 - (I) The first node to compare
  pNode2 - (I) The second node to compare
  ----------------------------------------------------------------------------
  Return Values:

  < 0   means pNode1->key is less than pNode2->key
  > 0   means pNode1->key is greater than pNode2->key
  
  ----------------------------------------------------------------------------
  Notes:

  This function uses the key comparison function established by the calling
  system when the tree that the node resides in was constructed. By definition
  this function must adhere to the behavior of the standard C function
  strcmp(), mathematically returning (pNode1->key - pNode2->key).

  In the case where node keys are duplicated within the tree (e.g. logically 
  pNode1->key == pNode2->key) the node instance values are compared to
  distinguish the relative value of the nodes.
  ----------------------------------------------------------------------------*/

static long NodeCompare
(
    smartTreeHandle pTree,
    smartTreeNodeHandle pNode1,
    smartTreeNodeHandle pNode2
);

/*----------------------------------------------------------------------------
  PivotLesserToGreater()
  ----------------------------------------------------------------------------
  Pivot a subtree moving nodes from the lesser side of the subtree to the
  greater side of the subtree
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) The tree handle
  pRoot - (I) The root of the subtree to rebalance
  ----------------------------------------------------------------------------
  Return Values:

  None
  ----------------------------------------------------------------------------
  Notes:

  1. Make the lesser child of the old root the new root
  2. Make the old root the greater child of the new root
  3. Make the greater child of the new root the lesser child of the old root

                 Old                           New

                  Y                             W.
                 / \                          /   \
                W   Z     -- Becomes ->     U       Y.
               / \                         / \     / \     
              U   X                       T   V   X.  Z
             / \               
            T   V               Altered nodes denoted with a period

  The old root's (Y) lesser and new root's (W) greater children were altered
  as a result weights on these branches must be recalculated
  ----------------------------------------------------------------------------*/

static void PivotLesserToGreater
(
    smartTreeHandle pTree, 
    smartTreeNodeHandle * pRoot
);

/*----------------------------------------------------------------------------
  PivotGreaterToLesser()
  ----------------------------------------------------------------------------
  Pivot a subtree moving nodes from the greater side of the subtree to the
  lesser side of the subtree
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) The tree handle
  pRoot - (I) The root of the subtree to rebalance
  ----------------------------------------------------------------------------
  Return Values:

  None
  ----------------------------------------------------------------------------
  Notes:

  1. Make the greater child of the old root the new root
  2. Make the old root the lesser child of the new root
  3. Make the lesser child of the new root the greater child of the old root

                 Old                           New

                  B                             D.
                 / \                          /   \
                A   D     -- Becomes ->     B.      F
                   / \                     / \     / \     
                  C   F                   A   C.  E   G
                     / \               
                    E   G       Altered nodes denoted with a period

  The old root's (B) greater and new root's (D) lesser children were altered
  as a result weights on these branches must be recalculated
  ----------------------------------------------------------------------------*/

static void PivotGreaterToLesser
(
    smartTreeHandle pTree, 
    smartTreeNodeHandle * pRoot
);

/*----------------------------------------------------------------------------
  SubtreePrune()
  ----------------------------------------------------------------------------
  Delete all nodes from a subtree 
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) The tree handle
  pRoot - (I) The root of the subtree to prune
  ----------------------------------------------------------------------------
  Return Values:

  True  - Subtree was succesfully pruned

  False - Subtree was not successfully pruned due to one of the following:

          1. A node destruction failed
          2. A SafeFree() failed
  ----------------------------------------------------------------------------*/

static Bool SubtreePrune
(
    smartTreeHandle pTree,
    smartTreeNodeHandle * pRoot
);

/*----------------------------------------------------------------------------
  SubtreeIsValid()
  ----------------------------------------------------------------------------
  Test every node within a subtree to assure that the structure is correct.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) The tree handle
  pRoot - (I) The root of the subtree to check
  ----------------------------------------------------------------------------
  Return Values:

  True  - Subtree passed the validity check

  False - Subtree did not pass the validity check due to one of the following:

          1. A node had a greater value that is located on a lesser branch
          2. A node had a lesser value that is located on a greater branch
		  3. A node had branch weight that was incorrect
  ----------------------------------------------------------------------------
  Note:

  This function is primarily used for debugging of the smartTree module. A
  validity check failure suggests a bug in the smartTree implementation.
  ----------------------------------------------------------------------------*/

static Bool SubtreeIsValid
(
    smartTreeHandle pTree,
    smartTreeNodeHandle pRoot
);

#endif
