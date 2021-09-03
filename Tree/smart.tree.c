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
  Smart Tree application programmer's interface (API) implementation file
  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
   Terms:

   - A root node with no children is called a sprout node
   - A root node with one child is called a sapling node
   - A root node with two children is called a trunk node
   - A non-root node with two children is called a fork node
   - A non-root node with one child is called a twig node
   - A non-root node with no children is called a leaf node
  ----------------------------------------------------------------------------*/

#include "compilation.t.h"
#include "types.t.h"
#include "smart.memory.i.h"

/*----------------------------------------------------------------------------
  Private defines, data types and function prototypes
  ----------------------------------------------------------------------------*/

#include "smart.tree.h"

/*----------------------------------------------------------------------------
  Public function prototypes
  ----------------------------------------------------------------------------*/

#include "smart.tree.i.h"

/*----------------------------------------------------------------------------
  Public functions
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeConstructSmartTree
(
    smartTreeHandle * pTree,
    long (* pCompareKeyFunction)(const smartTreeKeyHandle pKey1, const smartTreeKeyHandle pKey2),
	size_t pMemoryMaximum
)
{
    if (NULL == pCompareKeyFunction)
	{
		return(FALSE);
	}
    
	if (!SafeMalloc(pTree, sizeof(smartTree)))
	{
		return(FALSE);
	}

    (* pTree)->compareKeyFunction = pCompareKeyFunction;

	(* pTree)->memoryMaximum = pMemoryMaximum;
	(* pTree)->memoryAllocated = sizeof(smartTree);

    (* pTree)->root = NULL;

	return(TRUE);
}

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeConstructNode
(
    smartTreeHandle pTree,
    smartTreeNodeHandle * pNode,
    smartTreeKeyHandle * pKey,
	size_t pKeySize,
    smartTreeDataHandle * pData,
	size_t pDataSize
)
{
    /*
    ** there is no tree
    */

    if (NULL == pTree)
    {
        return(FALSE);
    }

	if (NULL == pKey)
	{
		return(FALSE);
	}

	if (pTree->memoryMaximum > 0 && pTree->memoryMaximum < pTree->memoryAllocated + sizeof(smartTreeNode) + pKeySize + pDataSize)
	{
		return(FALSE);
	}

	/*
	** allocate memory for the objects
	*/

    if (!SmartMalloc(pNode, sizeof(smartTreeNode), &pTree->memoryAllocated))
	{
		return(FALSE);
	}

    if (!SmartMalloc(pKey, pKeySize, &pTree->memoryAllocated))
	{
		SmartFree(pNode, sizeof(smartTreeNode), &pTree->memoryAllocated);

		return(FALSE);
	}

    if (!SmartMalloc(pData, pDataSize, &pTree->memoryAllocated))
	{
		SmartFree(pKey, pKeySize, &pTree->memoryAllocated);
		SmartFree(pNode, sizeof(smartTreeNode), &pTree->memoryAllocated);

		return(FALSE);
	}

	/*
	** initialize the node
	*/

	(* pNode)->instance = 1;

    (* pNode)->parent = NULL;
    
    (* pNode)->lesser = NULL;
    (* pNode)->lesserNullNodes = 1;
    
    (* pNode)->greater = NULL;
    (* pNode)->greaterNullNodes = 1;

    (* pNode)->key = *pKey;
    (* pNode)->keySize = pKeySize;

    (* pNode)->data = *pData;
    (* pNode)->dataSize = pDataSize;

    return(TRUE);
}

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeInsertNode
(
    smartTreeHandle pTree,
    smartTreeNodeHandle pNode
)
{
    smartTreeNodeHandle lNode;
    smartTreeNodeHandle lChild;

	long lComparisonResult;

	unsigned int lRebalanceThreshold = REBALANCE_THRESHOLD;

    /*
    ** there is no tree
    */

    if (NULL == pTree)
    {
        return(FALSE);
    }

    /*
    ** there is no node
    */

    if (NULL == pNode)
    {
        return(FALSE);
    }

    /*
    ** add a root node when the tree is empty
    */

	lNode = pTree->root;

    if (NULL == lNode)
    {
        pTree->root = pNode;
        return(TRUE);
    }

    /*
    ** traverse the tree to find the insertion point
    */

    for (;;)
    {
		lComparisonResult = pTree->compareKeyFunction((const smartTreeKeyHandle) pNode->key, (const smartTreeKeyHandle) lNode->key);;

		if (0 == lComparisonResult)
		{
			pNode->instance = lNode->instance + 1;

			lComparisonResult = GREATER_THAN;
		}

        if (0 > lComparisonResult)
        {
			/*
			** traverse lesser
			*/

            if (lRebalanceThreshold <= lNode->lesserNullNodes / lNode->greaterNullNodes)
            {
                PivotLesserToGreater(pTree, &lNode);

				lRebalanceThreshold++; /* prevents rebalance hysteresis */

				continue;
			}

            lNode->lesserNullNodes += 1;

            lChild = lNode->lesser;

            if (NULL == lChild)
            {
                pNode->parent = lNode;
                lNode->lesser = pNode;
                return(TRUE);
            }
        }
        else // (0 < lComparisonResult)
        {
			/*
			** traverse greater
			*/

            if (lRebalanceThreshold <= lNode->greaterNullNodes / lNode->lesserNullNodes)
            {
                PivotGreaterToLesser(pTree, &lNode);

				lRebalanceThreshold++; /* prevents rebalance hysteresis */

				continue;
			}

            lNode->greaterNullNodes += 1;

            lChild = lNode->greater;

            if (NULL == lChild)
            {
                pNode->parent = lNode;
                lNode->greater = pNode;
                return(TRUE);
            }
        }

		lNode = lChild;

		lRebalanceThreshold = REBALANCE_THRESHOLD;
    }
}

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetRoot
(
    smartTreeHandle pTree
)
{
    /*
    ** there is no tree
    */

	if (NULL == pTree)
	{
		return(NULL);
	}

	return(pTree->root);
}

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetLesserChildNode
(
    smartTreeNodeHandle pNode
)
{
    /*
    ** there is no node
    */

    if (NULL == pNode)
    {
        return(NULL);
    }

    return(pNode->lesser);
}

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetParentNode
(
    smartTreeNodeHandle pNode
)
{
    /*
    ** there is no node
    */

    if (NULL == pNode)
    {
        return(NULL);
    }

    return(pNode->parent);
}

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetGreaterChildNode
(
    smartTreeNodeHandle pNode
)
{
    /*
    ** there is no node
    */

    if (NULL == pNode)
    {
        return(NULL);
    }

    return(pNode->greater);
}

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetLeastNode
(
    smartTreeHandle pTree
)
{
    smartTreeNodeHandle lNode = NULL;
    smartTreeNodeHandle lChild;

    /*
    ** there is no tree
    */

	if (NULL == pTree)
	{
		return(NULL);
	}

	/*
	** traverse towards the most lesser leaf
	*/

    lChild = pTree->root;

    while (NULL != lChild)
    {
        lNode = lChild;
        lChild = lNode->lesser;
    }

    return(lNode);
}

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetNextLesserNode
(
    smartTreeHandle pTree,
    smartTreeNodeHandle pNode
)
{ 
	smartTreeNodeHandle lChild;

    /*
    ** there is no tree
    */

	if (NULL == pTree)
	{
		return(NULL);
	}

    /*
    ** there is no node
    */

    if (NULL == pNode)
    {
		return(NULL);
	}

	/*
	** traverse to the next lesser node in the tree
	*/

    lChild = pNode->lesser;

    if (NULL != lChild)
    {
        /*
        ** get the greater node of the lesser branch of the node passed
        */

        do
        {
            pNode = lChild;
            lChild = pNode->greater; /* traverse towards a leaf */
        } 
        while (NULL != lChild);
    }
    else
    {
        /*
        ** get the first ancestor of the node passed that references the node's
        ** subtree via a greater branch
        */

        do
        {
            lChild = pNode;
            pNode = lChild->parent; /* traverse towards the root */
        }
        while (NULL != pNode && lChild != pNode->greater);
    }

    return(pNode);
}

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetEqualNode
(
    smartTreeHandle pTree,
    const smartTreeKeyHandle pKey
)
{
    smartTreeNodeHandle lNode;

    long lComparisonResult;

    /*
    ** there is no tree
    */

	if (NULL == pTree)
	{
		return(NULL);
	}

	/*
	** there is no key
	*/

	if (NULL == pKey)
	{
		return(NULL);
	}

	/*
	** traverse towards the node that matches the key object value
	*/

	lNode = pTree->root;

    while (NULL != lNode)
    {
        lComparisonResult = pTree->compareKeyFunction((const smartTreeKeyHandle) pKey, (const smartTreeKeyHandle) lNode->key);

        if (0 > lComparisonResult)
        {
            lNode = lNode->lesser; /* traverse lesser */
        }
        else if (0 < lComparisonResult)
        {
            lNode = lNode->greater; /* traverse greater */
        }
        else /* (0 == lComparisonResult) */
        {
            break; /* found */
        }
    }

    return(lNode);
}

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetNextGreaterNode
(
    smartTreeHandle pTree,
    smartTreeNodeHandle pNode
)
{
	smartTreeNodeHandle lChild;

    /*
    ** there is no tree
    */

	if (NULL == pTree)
	{
		return(NULL);
	}

    /*
    ** there is no node
    */

    if (NULL == pNode)
    {
		return(NULL);
	}

	/*
	** traverse to the next greater node in the tree
	*/

    lChild = pNode->greater;

    if (NULL != lChild)
    {
        /*
        ** get the lesser node of the greater branch of the node passed
        */

        do
        {
            pNode = lChild;
            lChild = pNode->lesser; /* traverse towards a leaf */
        } 
        while (NULL != lChild);
    }
    else
    {
        /*
        ** get the first ancestor of the node passed that references the node's
        ** subtree via a lesser branch
        */

        do
        {
            lChild = pNode;
            pNode = lChild->parent; /* traverse towards the root */
        }
        while (NULL != pNode && lChild != pNode->lesser);
    }

    return(pNode);
}

STORAGE_CLASS smartTreeNodeHandle CALLING_CONVENTION SmartTreeGetGreatestNode
(
    smartTreeHandle pTree
)
{
    smartTreeNodeHandle lNode = NULL;
    smartTreeNodeHandle lChild;

    /*
    ** there is no tree
    */

	if (NULL == pTree)
	{
		return(NULL);
	}

	/*
	** traverse towards the most greater leaf
	*/

    lChild = pTree->root;

    while (NULL != lChild)
    {
        lNode = lChild;
        lChild = lNode->greater;
    }

    return(lNode);
}

STORAGE_CLASS smartTreeKeyHandle CALLING_CONVENTION SmartTreeGetNodeKey
(
    smartTreeNodeHandle pNode
)
{ 
    /*
    ** there is no node
    */

    if (NULL == pNode)
    {
        return(NULL);
    }

    return(pNode->key);
}

STORAGE_CLASS smartTreeDataHandle CALLING_CONVENTION SmartTreeGetNodeData
(
    smartTreeNodeHandle pNode
)
{ 
    /*
    ** there is no node
    */

    if (NULL == pNode)
    {
        return(NULL);
    }

    return(pNode->data);
}

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeDeleteNode
(
    smartTreeHandle pTree, 
    smartTreeNodeHandle pNode
)
{
	smartTreeNodeHandle lRoot;
    smartTreeNodeHandle lNode;
    smartTreeNodeHandle lChild;

	long lComparisonResult;

	Bool lNodeToDeleteFound = FALSE;

    /*
    ** there is no tree
    */

    if (NULL == pTree)
    {
        return(FALSE);
    }

    /*
    ** the tree is empty
    */

	lRoot = pTree->root;

    if (NULL == lRoot)
    {
        return(FALSE);
    }

    /*
    ** there is no node to delete
    */

    if (NULL == pNode)
    {
        return(FALSE);
    }

    /*
    ** traverse the tree to find a suitable substitute leaf or twig node 
	** to be promoted into the position of node being deleted and rebalance
	** the tree along the way
    */
	
	lNode = lRoot;

    for (;;)
    {
		/*
		** decide which branch to follow
		*/

		lComparisonResult = NodeCompare(pTree, pNode, lNode);

		if (0 == lComparisonResult)
		{
			lNodeToDeleteFound = TRUE;

			if (NULL != lNode->lesser)
			{
				lComparisonResult = LESS_THAN;
			}
			else
			{
				lComparisonResult = GREATER_THAN;
			}
		}

		/*
		** follow branch
		*/

        if (0 > lComparisonResult)
        {
			/*
			** traverse lesser
			*/

            lChild = lNode->lesser;

            if (NULL == lChild)
            {
				break;
            }

			if (REBALANCE_THRESHOLD <= lNode->greaterNullNodes / (lNode->lesserNullNodes - 1))
			{
				PivotGreaterToLesser(pTree, &lNode);
	            lChild = lNode->lesser;
			}

			lNode->lesserNullNodes -= 1;
        }
        else // (0 < lComparisonResult)
        {
			/*
			** traverse greater
			*/

            lChild = lNode->greater;

            if (NULL == lChild)
            {
                break;
            }

			if (REBALANCE_THRESHOLD <= lNode->lesserNullNodes / (lNode->greaterNullNodes - 1))
			{
				PivotLesserToGreater(pTree, &lNode);
	            lChild = lNode->greater;
			}

			lNode->greaterNullNodes -= 1;
        }

        lNode = lChild;
    }

	/*
	** node to delete was not encountered in the tree during traversal
	*/

	if (!lNodeToDeleteFound)
	{
		return(FALSE); /* branch weights are messed up now! */
	}

	if (pNode == lNode)
	{
		/*
		** the node to be deleted is a leaf node therefore there's no
		** need for promotion, just disconnect it from its parent
		*/

		if (lRoot == lNode)
		{
			pTree->root = NULL;
		}
		else if (pNode->parent->lesser == lNode)
		{
			pNode->parent->lesser = NULL;
		}
		else // (pNode->parent->greater == lNode)
		{
			pNode->parent->greater = NULL;
		}
	}
	else // (pNode != lNode)
	{
		/*
		** promote a leaf or twig node
		*/

		/*
		** when a lesser or greater child branch is attached to the node to promote
		** move the child branch up to take the place of the to be promoted node
		*/

		if (NULL != lNode->lesser)
		{
			if (lNode->parent->lesser == lNode)
			{
				lNode->parent->lesser = lNode->lesser;
			}
			else
			{
				lNode->parent->greater = lNode->lesser;
			}

			lNode->lesser->parent = lNode->parent;
		}
		else if (NULL != lNode->greater)
		{
			if (lNode->parent->lesser == lNode)
			{
				lNode->parent->lesser = lNode->greater;
			}
			else
			{
				lNode->parent->greater = lNode->greater;
			}

			lNode->greater->parent = lNode->parent;
		}
		else
		{
			/*
			** disconnect the node being promoted from the old parent
			*/

			if (lNode == lNode->parent->lesser)
			{
				lNode->parent->lesser = NULL;
			}
			else // (lNode == lNode->parent->greater)
			{
				lNode->parent->greater = NULL;
			}
		}

		/*
		** make the found node assume the position of the node about to be deleted within the tree
		*/

		/*
		** connect the new parent to the node being promoted
		*/

		if (NULL == pNode->parent)
		{
			pTree->root = lNode;
		}		
		else if (pNode == pNode->parent->lesser)
		{
			pNode->parent->lesser = lNode;
		}
		else // (pNode == pNode->parent->greater)
		{
			pNode->parent->greater = lNode;
		}

		/*
		** connect the node being promoted to the new parent
		*/

		lNode->parent = pNode->parent;

		/*
		** connect lesser child to the node being promoted
		*/

		if (lNode == pNode->lesser)
		{
			lNode->lesser = NULL;
		}
		else
		{
			lNode->lesser = pNode->lesser;

			if (NULL != lNode->lesser)
			{
				lNode->lesser->parent = lNode;
			}
		}

		lNode->lesserNullNodes = pNode->lesserNullNodes;

		/*
		** connect the greater child to the node being promoted
		*/

		if (lNode == pNode->greater)
		{
			lNode->greater = NULL;
		}
		else
		{
			lNode->greater = pNode->greater;
			
			if (NULL != lNode->greater)
			{
				lNode->greater->parent = lNode;
			}
		}

		lNode->greaterNullNodes = pNode->greaterNullNodes;
	}

    return(TRUE);
}
  
STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeDestructNode
(
    smartTreeHandle pTree,
    smartTreeNodeHandle * pNode
)
{
	if (!SmartFree(&(* pNode)->data, (* pNode)->dataSize, &pTree->memoryAllocated))
	{
		return(FALSE);
	}

	if (!SmartFree(&(* pNode)->key, (* pNode)->keySize, &pTree->memoryAllocated))
	{
		return(FALSE);
	}

	if (!SmartFree(pNode, sizeof(smartTreeNode), &pTree->memoryAllocated))
	{
		return(FALSE);
	}

	return(TRUE);
}

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeDestructSmartTree
(
    smartTreeHandle * pTree
)
{
    /*
    ** there is no tree
    */

    if (NULL == pTree)
    {
        return(FALSE);
    }

    if (!SubtreePrune(*pTree, &(* pTree)->root))
	{
		return(FALSE);
	}

    if (!SafeFree(pTree))
	{
		return(FALSE);
	}

	return(TRUE);
}

STORAGE_CLASS long CALLING_CONVENTION SmartTreeGetNodeCount
(
	smartTreeNodeHandle pNode
)
{  
    if (NULL == pNode)
	{
		return(0);
	}  

	return(1 + SmartTreeGetNodeCount(pNode->lesser) + SmartTreeGetNodeCount(pNode->greater));
}  

STORAGE_CLASS long CALLING_CONVENTION SmartTreeGetMaxDepth
(
	smartTreeNodeHandle pNode
)
{  
    long lLesserDepth;
	long lGreaterDepth;

    if (NULL == pNode)
	{
		return 0;
	}  

	lLesserDepth = SmartTreeGetMaxDepth(pNode->lesser);
	lGreaterDepth = SmartTreeGetMaxDepth(pNode->greater);

	return((lLesserDepth > lGreaterDepth) ? (1 + lLesserDepth) : (1 + lGreaterDepth));
}

STORAGE_CLASS size_t CALLING_CONVENTION SmartTreeGetMemoryAllocated
(
	smartTreeHandle pTree
)
{
    /*
    ** there is no tree
    */

	if (NULL == pTree)
	{
		return(0);
	}

	return(pTree->memoryAllocated);
}

STORAGE_CLASS Bool CALLING_CONVENTION SmartTreeIsValid
(
    smartTreeHandle pTree
)
{
    /*
    ** there is no tree
    */

	if (NULL == pTree)
	{
		return(TRUE);
	}

	/*
	** there are no nodes in the tree
	*/

	if (NULL == pTree->root)
	{
		return(TRUE);
	}

	/*
	** validate the root subtree
	*/

	if (TRUE == SubtreeIsValid(pTree, pTree->root))
	{
		return(TRUE);
	}

	/*
	** the tree is invalid
	*/

	return(FALSE);
}

/*----------------------------------------------------------------------------
  Private functions
  ----------------------------------------------------------------------------*/

static long NodeCompare
(
    smartTreeHandle pTree,
    smartTreeNodeHandle pNode1,
    smartTreeNodeHandle pNode2
)
{
	long lComparisonResult;

	lComparisonResult = pTree->compareKeyFunction((const smartTreeKeyHandle) pNode1->key, (const smartTreeKeyHandle) pNode2->key);

	if (0 == lComparisonResult)
	{
		lComparisonResult = pNode1->instance - pNode2->instance;
	}

	return(lComparisonResult);
}

static void PivotLesserToGreater
(
    smartTreeHandle pTree,
    smartTreeNodeHandle * pRoot
)
{
    smartTreeNodeHandle lOldRoot = *pRoot;
    smartTreeNodeHandle lNewRoot = lOldRoot->lesser;

    lNewRoot->parent = lOldRoot->parent;

    if (pTree->root == lOldRoot)
    {
        pTree->root = lNewRoot;
    }
    else
    {
        if (lOldRoot->parent->lesser == lOldRoot)
        {
            lOldRoot->parent->lesser = lNewRoot;
        }
        else
        {
            lOldRoot->parent->greater = lNewRoot;
        }
    }

    lOldRoot->parent = lNewRoot;

    lOldRoot->lesser = lNewRoot->greater;

    if (NULL != lNewRoot->greater)
    {
        lNewRoot->greater->parent = lOldRoot;
    }

    lNewRoot->greater = lOldRoot;

	/*
    ** update child node counts for the new and old parent working from lower to higher in the subtree
    */

    if (NULL == lOldRoot->lesser)
	{
	    lOldRoot->lesserNullNodes = 1;
	}
	else
    {
		lOldRoot->lesserNullNodes = lOldRoot->lesser->lesserNullNodes + lOldRoot->lesser->greaterNullNodes;
    }

	lNewRoot->greaterNullNodes = lOldRoot->lesserNullNodes + lOldRoot->greaterNullNodes;

	/*
    ** set the new parent in the calling function's variable so that in process tree traversals can continue
    */

    (* pRoot) = lNewRoot;
}

static void PivotGreaterToLesser
(
    smartTreeHandle pTree,
    smartTreeNodeHandle * pRoot
)
{
    smartTreeNodeHandle lOldRoot = (* pRoot);
    smartTreeNodeHandle lNewRoot = lOldRoot->greater;

    lNewRoot->parent = lOldRoot->parent;

    if (pTree->root == lOldRoot)
    {
        pTree->root = lNewRoot;
    }
    else
    {
        if (lOldRoot->parent->lesser == lOldRoot)
        {
            lOldRoot->parent->lesser = lNewRoot;
        }
        else
        {
            lOldRoot->parent->greater = lNewRoot;
        }
    }

    lOldRoot->parent = lNewRoot;

    lOldRoot->greater = lNewRoot->lesser;

    if (NULL != lNewRoot->lesser)
    {
        lNewRoot->lesser->parent = lOldRoot;
    }

    lNewRoot->lesser = lOldRoot;

	/*
    ** update child node counts for the new and old parent working from lower to higher in the subtree
    */

    if (NULL == lOldRoot->greater)
	{
	    lOldRoot->greaterNullNodes = 1;
	}
	else
    {
		lOldRoot->greaterNullNodes = lOldRoot->greater->lesserNullNodes + lOldRoot->greater->greaterNullNodes;
    }

	lNewRoot->lesserNullNodes = lOldRoot->lesserNullNodes + lOldRoot->greaterNullNodes;

	/*
    ** set the new parent in the calling function's variable so that in process tree traversals can continue
    */

    (* pRoot) = lNewRoot;
}

static Bool SubtreePrune
(
    smartTreeHandle pTree,
    smartTreeNodeHandle * pRoot
)
{
    if (NULL != pRoot && NULL != *pRoot)
    {
        if (NULL != (* pRoot)->lesser)
        {
            if (!SubtreePrune(pTree, &(* pRoot)->lesser))
			{
				return(FALSE);
			}
        }

        if (NULL != (* pRoot)->greater)
        {
            if (!SubtreePrune(pTree, &(* pRoot)->greater))
			{
				return(FALSE);
			}
        }

		if (!SmartTreeDestructNode(pTree, pRoot))
		{
			return(FALSE);
		}
    }

	return(TRUE);
}

static Bool SubtreeIsValid
(
    smartTreeHandle pTree,
    smartTreeNodeHandle pRoot
)
{
	if (NULL != pRoot->lesser)
	{
		/*
		** validate that the node values are correct
		*/

		if (0 > NodeCompare(pTree, pRoot, pRoot->lesser))
		{
			return(FALSE); // set breakpoint here for debugging
		}

		/*
		** validate that the node weights are correct
		*/

		if (NULL != pRoot->lesser && pRoot->lesserNullNodes != pRoot->lesser->lesserNullNodes + pRoot->lesser->greaterNullNodes)
		{
			return(FALSE); // set breakpoint here for debugging
		}

		/*
		** validate the lesser subtree
		*/

		if (FALSE == SubtreeIsValid(pTree, pRoot->lesser))
		{
			return(FALSE);
		}
	}
	
	if (NULL != pRoot->greater)
	{
		/*
		** validate that the node values are correct
		*/

		if (0 < NodeCompare(pTree, pRoot, pRoot->greater))
		{
			return(FALSE); // set breakpoint here for debugging
		}

		/*
		** validate that the node weights are correct
		*/

		if (pRoot->greaterNullNodes != pRoot->greater->lesserNullNodes + pRoot->greater->greaterNullNodes)
		{
			return(FALSE); // set breakpoint here for debugging
		}

		/*
		** validate the greater subtree
		*/

		if (FALSE == SubtreeIsValid(pTree, pRoot->greater))
		{
			return(FALSE);
		}
	}

	return(TRUE);
}
