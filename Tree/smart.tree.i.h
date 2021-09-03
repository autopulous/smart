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
  Smart Tree application programmer's interface (API) header file
  ----------------------------------------------------------------------------*/

#ifndef SMART_TREE_I_H
#define SMART_TREE_I_H

/*----------------------------------------------------------------------------
  SmartTreeConstructSmartTree()
  ----------------------------------------------------------------------------
  Construct an empty tree.
  ----------------------------------------------------------------------------
  Parameters:

  pTree                 - (I/O) Pointer to recieve the tree handle
  pCompareKeyFunction   - (I)   Pointer to the node key comparison function
  pMemoryMaximum        - (I)   The maximum number of bytes used by the tree
  ----------------------------------------------------------------------------
  Return Values:

  True  - Tree was succesfully constructed

  False - Tree was not successfully constructed due to:

          1. The ComparisonKeyFunction was NULL
          2. The SafeMalloc() failed
  ----------------------------------------------------------------------------
  Notes:

  This function requires the contents of the pTree handle to be initialized
  to NULL prior to calling this function because this function assumes that 
  a pointer with value represents an unfreed memory block and therefore
  overwritting this pointer with a new value would orphan the previously
  allocated memory block.

  The key comparison function is required. This function must adhere to the
  behavior of the standard C function strcmp(), mathematically returning
  (pKey1 - pKey2) where:

                  <  0   means pKey1 is less than pKey2
                  == 0   means pKey1 is equal to pKey2
                  >  0   means pKey1 is greater than pKey2

  The maximum memory paramter may be set to zero to allow the tree to grow to
  the operating system controlled process memory limit.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeConstructSmartTree
(
    smartTreeHandle * pTree,
    long (* pCompareKeyFunction)(const smartTreeKeyHandle pKey1, const smartTreeKeyHandle pKey2),
	size_t pMemoryMaximum
);

/*----------------------------------------------------------------------------
  SmartTreeConstructNode()
  ----------------------------------------------------------------------------
  Construct a tree node.
  ----------------------------------------------------------------------------
  Parameters:

  pTree     - (I)   Tree handle
  pNode     - (I/O) Pointer to recieve the node handle
  pKey      - (I/O) Pointer to receive the key object handle
  pKeySize  - (I)   The number of bytes used by the key object
  pData     - (I/O) Pointer to receive the data object handle
  pDataSize - (I)   The number of bytes used by the data object
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully constructed

  False - Node was not successfully constructed due to:

          1. The tree handle was invalid
		  2. The pointer to the key object handle was invalid
		  3. The pointer to the data object handle was invalid
		  4. SmartMalloc() failed
		  5. Would make the tree exceed its maximum number of bytes
  ----------------------------------------------------------------------------
  Notes:

  The memory allocated for a node, its key object and its data object are
  managed by the tree associated to the node. Insertion and deletion of nodes
  from the associated tree do nothing to the memory allocation. As a result
  nodes may be reused (without reconstruction) after deletion from the tree.

  Destructing a tree will destruct all associated nodes, key objects and data
  objects.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeConstructNode
(
    smartTreeHandle pTree,
    smartTreeNodeHandle * pNode,
    smartTreeKeyHandle * pKey,
	size_t pKeySize,
    smartTreeDataHandle * pData,
	size_t pDataSize
);

/*----------------------------------------------------------------------------
  SmartTreeInsertNode()
  ----------------------------------------------------------------------------
  Place a node into a tree.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) Tree handle
  pNode - (I) Node handle
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully inserted

  False - Node was not successfully inserted due to:

          1. The pTree handle was NULL.
          2. The pNode handle was NULL.
  ----------------------------------------------------------------------------
  Operational Note:
  
  Self balancing is an intrinsic (single pass) effect of both the insert and
  delete operations with node rebalancing occuring according to the computed
  relative weight of the lesser and greater branches of each node considered
  during traversal of the tree for insertion or deletion. A maximum 3 to 1
  ratio of "lesser to greater" or "greater to lesser" branch weights is
  permitted before rebalancing.

  Usage Note:

  The SmartTree supports insertion of duplicate key values. Differentiation of
  duplicates within the tree is achieved by an instance counter that acts as a
  version number for duplicate keys. Calling applications that wish to handle
  duplicate key insertion as an error, a data overwrite or as ingorable are
  responsible for searching for duplicates prior to attempting an insert.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeInsertNode
(
    smartTreeHandle pTree,
    smartTreeNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartTreeGetRoot()
  ----------------------------------------------------------------------------
  Get the root node of the tree.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) Tree handle
  ----------------------------------------------------------------------------
  Return Values:

  NULL - There is no tree or there is no root node in the tree

  smartTreeNodeHandle - The root node of the tree
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetRoot
(
    smartTreeHandle pTree
);

/*----------------------------------------------------------------------------
  SmartTreeGetLesserChildNode()
  ----------------------------------------------------------------------------
  Get the lesser child node of the passed node.
  ----------------------------------------------------------------------------
  Parameters:

  pNode - (I) Handle of the node to get its lesser node
  ----------------------------------------------------------------------------
  Return Values:

  NULL - The passed node was NULL or it is a leaf node in the tree

  smartTreeNodeHandle - The lesser child node of the node passed
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetLesserChildNode
(
    smartTreeNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartTreeGetParentNode()
  ----------------------------------------------------------------------------
  Get the physical parent node of the passed node.
  ----------------------------------------------------------------------------
  Parameters:

  pNode - (I) Handle of the node to get its parent node
  ----------------------------------------------------------------------------
  Return Values:

  NULL - The passed node was NULL or it is the root node of the tree

  smartTreeNodeHandle - The parent node of the node passed
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetParentNode
(
    smartTreeNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartTreeGetGreaterChildNode()
  ----------------------------------------------------------------------------
  Get the greater child node of the passed node.
  ----------------------------------------------------------------------------
  Parameters:

  pNode - (I) Handle of the node to get its greater node
  ----------------------------------------------------------------------------
  Return Values:

  NULL - The passed node was NULL or it is a leaf node in the tree

  smartTreeNodeHandle - The greater child node of the node passed
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetGreaterChildNode
(
    smartTreeNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartTreeGetLeastNode()
  ----------------------------------------------------------------------------
  Get the node with the lowest key value from the tree.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) Tree handle
  ----------------------------------------------------------------------------
  Return Values:

  NULL - There is no tree or there are no nodes are in the tree

  smartTreeNodeHandle - The lowest key valued node in the tree
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetLeastNode
(
    smartTreeHandle pTree
);

/*----------------------------------------------------------------------------
  SmartTreeGetNextLesserNode()
  ----------------------------------------------------------------------------
  Get the node with the next lower key value in the tree.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) Tree handle
  pNode - (I) Handle of the node to get the next lesser node in the tree
  ----------------------------------------------------------------------------
  Return Values:

  NULL - There is no tree or there is no lower node in the tree

  smartTreeNodeHandle - The next lower key valued node in the tree
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetNextLesserNode
(
    smartTreeHandle pTree,
    smartTreeNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartTreeGetEqualNode()
  ----------------------------------------------------------------------------
  Get a node that matches the provided key object from the tree.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) Tree handle
  pKey -  (I) The key object to use to find a node in the tree
  ----------------------------------------------------------------------------
  Return Values:

  NULL - No node was found that matched the value of the key object

  smartTreeNodeHandle - A node from the tree that matched the value of the
                       provided key object
  ----------------------------------------------------------------------------
  Note:

  For nodes with duplicated key values the first encountered instance of the
  node will be returned from the tree. Due to tree rebalancing the particular
  instance of a duplicate node returned is unpredictable.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetEqualNode
(
    smartTreeHandle pTree,
    const smartTreeKeyHandle pKey
);

/*----------------------------------------------------------------------------
  SmartTreeGetNextGreaterNode()
  ----------------------------------------------------------------------------
  Get the node with the next higher key value in the tree.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) Tree handle
  pNode - (I) Handle of the node to get the next greater node in the tree
  ----------------------------------------------------------------------------
  Return Values:

  NULL - There is no tree or there is no higher node in the tree

  smartTreeNodeHandle - The next higher key valued node in the tree
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetNextGreaterNode
(
    smartTreeHandle pTree,
    smartTreeNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartTreeGetGreatestNode()
  ----------------------------------------------------------------------------
  Get the node with the lowest key value from the tree.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) Tree handle
  ----------------------------------------------------------------------------
  Return Values:

  NULL - There is no tree or there are no nodes are in the tree

  smartTreeNodeHandle - The highest key valued node in the tree
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetGreatestNode
(
    smartTreeHandle pTree
);

/*----------------------------------------------------------------------------
  SmartTreeGetNodeKey()
  ----------------------------------------------------------------------------
  Get the key object attached to the node.
  ----------------------------------------------------------------------------
  Parameters:

  pNode - (I) Handle of the node to get its key object
  ----------------------------------------------------------------------------
  Return Values:

  NULL - No node handle was provided or the key handle was NULL

  smartTreeKeyHandle - The key object attached to the node
  ----------------------------------------------------------------------------
  Notes:

  It is possible to change the key value after the node is inserted into the
  tree. Under most circumstances this is an extraordinarily bad idea because
  there is a high likelihood of destroying a tree greater/lesser relationship.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartTreeKeyHandle CALLING_CONVENTION SmartTreeGetNodeKey
(
    smartTreeNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartTreeGetNodeData()
  ----------------------------------------------------------------------------
  Get the data object attached to the node.
  ----------------------------------------------------------------------------
  Parameters:

  pNode - (I) Handle of the node to get its data object
  ----------------------------------------------------------------------------
  Return Values:

  NULL - No node handle was provided or the data handle was NULL

  smartTreeDataHandle - The data object attached to the node
  ----------------------------------------------------------------------------
  Notes:

  It is permissible to change the data value after the node is inserted into
  the tree.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartTreeDataHandle CALLING_CONVENTION SmartTreeGetNodeData
(
    smartTreeNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartTreeDeleteNode()
  ----------------------------------------------------------------------------
  Remove a node from the tree.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) Handle of the tree in which the node to delete exists
  pNode - (I) Handle of the node remove
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully deleted

  False - Node was not successfully deleted due to:

          1. The pTree handle was NULL
		  2. The tree is empty
          3. The pNode handle pointer was NULL
		  4. The node to delete was not found in the tree
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeDeleteNode
(
    smartTreeHandle pTree,
    smartTreeNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartTreeDestructNode()
  ----------------------------------------------------------------------------
  Destruct a node.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) Handle of the tree to which the node is associated
  pNode - (I) Handle of the node remove
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully destructed

  False - Node was not successfully destructed due to:

          1. The pTree handle was NULL
          2. The pNode handle pointer was NULL
          3. The node data destruction failed
          4. The node key destruction failed
          5. The node destruction failed
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeDestructNode
(
    smartTreeHandle pTree,
    smartTreeNodeHandle * pNode
);

/*----------------------------------------------------------------------------
  SmartTreeDestructSmartTree()
  ----------------------------------------------------------------------------
  Destruct a tree by removing all nodes.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I/O) Pointer to the tree handle
  ----------------------------------------------------------------------------
  Return Values:

  True  - Tree was succesfully destructed

  False - Tree was not successfully destructed due to:

          1. The pTree handle pointer was NULL
		  2. The destruction of a node key object failed
		  3. The destruction of a node data object failed
          4. The SafeFree() of a node failed
		  5. The SafeFree() of the tree failed
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeDestructSmartTree
(
    smartTreeHandle * pTree
);

/*----------------------------------------------------------------------------
  SmartTreeGetNodeCount()
  ----------------------------------------------------------------------------
  Determine the number of NULL nodes within a subtree.
  ----------------------------------------------------------------------------
  Parameters:

  pNode - (I) Pointer to a subtree root node
  ----------------------------------------------------------------------------
  Return Values:

  long - The number of NULL nodes within the subtree
  ----------------------------------------------------------------------------
  Note:

  Every leaf node has a NULL lesser and NULL greater child nodes. The count
  returned by this function is the total of all leaf nodes within the subtree.

  The number of NULL child nodes is always 1 more than the number of nodes in
  the subtree.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS long CALLING_CONVENTION SmartTreeGetNodeCount
(
	smartTreeNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartTreeGetMaxDepth()
  ----------------------------------------------------------------------------
  Determine the number of levels in the subtree.
  ----------------------------------------------------------------------------
  Parameters:

  pNode - (I) Pointer to a subtree root node
  ----------------------------------------------------------------------------
  Return Values:

  long - The number of levels within the subtree
  ----------------------------------------------------------------------------
  Note:

  This function determines the branch length of the longest route from the
  subtree root node to a leaf node. Each node within the longest route from 
  the subtree root to a leaf node is considered a level within the subtree.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS long CALLING_CONVENTION SmartTreeGetMaxDepth
(
	smartTreeNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartTreeGetmemoryAllocated()
  ----------------------------------------------------------------------------
  Determine the number bytes allocated by the tree.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) Tree handle
  ----------------------------------------------------------------------------
  Return Values:

  size_t - The number of bytes allocated to the tree
  ----------------------------------------------------------------------------*/

STORAGE_CLASS size_t CALLING_CONVENTION SmartTreeGetMemoryAllocated
(
	smartTreeHandle pTree
);

/*----------------------------------------------------------------------------
  SmartTreeIsValid()
  ----------------------------------------------------------------------------
  Test every node within a tree to assure that the structure is correct.
  ----------------------------------------------------------------------------
  Parameters:

  pTree - (I) The tree handle
  ----------------------------------------------------------------------------
  Return Values:

  True  - Tree passed the validity check

  False - Tree did not pass the validity check due to:

          1. A node had a greater value located on a lesser branch
          2. A node had a lesser value located on a greater branch
		  3. A node had branch weight that was incorrect
  ----------------------------------------------------------------------------
  Note:

  This function is primarily used for debugging of the smartTree module. A
  validity check failure suggests a bug in the smartTree implementation.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeIsValid
(
    smartTreeHandle pTree
);

#endif
