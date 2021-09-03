/*----------------------------------------------------------------------------
  Smart Stack
 
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
  Smart Stack application programmer's interface (API) implementation file
  ----------------------------------------------------------------------------*/

#include "compilation.t.h"
#include "types.t.h"
#include "smart.memory.i.h"

/*----------------------------------------------------------------------------
  Private defines, data types and function prototypes
  ----------------------------------------------------------------------------*/

#include "smart.stack.h"

/*----------------------------------------------------------------------------
  Public function prototypes
  ----------------------------------------------------------------------------*/

#include "smart.stack.i.h"

/*----------------------------------------------------------------------------
  Public functions
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackConstructSmartStack
(
    smartStackHandle * pStack,
	size_t pMemoryMaximum
)
{
    /*
    ** there is no stack handle
    */

    if (NULL == pStack)
    {
        return(FALSE);
    }

	/*
	** construct the stack
	*/

    if (!SafeMalloc(pStack, sizeof(smartStack)))
	{
		return(FALSE);
	}

	/*
	** initialize the stack control structure
	*/

    (* pStack)->top = NULL;
    (* pStack)->bottom = NULL;

	(* pStack)->memoryMaximum = pMemoryMaximum;
	(* pStack)->memoryAllocated = sizeof(smartStack);

    (* pStack)->depth = 0;

	return(TRUE);
}

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackConstructNode
(
    smartStackHandle pStack,
    smartStackNodeHandle * pNode,
    smartStackDataHandle * pData,
	size_t pDataSize
)
{
	void * lData = NULL;

    /*
    ** there is no stack
    */

    if (NULL == pStack)
    {
        return(FALSE);
    }

    /*
    ** there is no node handle
    */

    if (NULL == pNode)
    {
        return(FALSE);
    }

	if (pStack->memoryMaximum > 0 && pStack->memoryMaximum < pStack->memoryAllocated + sizeof(smartStackNode) + pDataSize)
	{
		return(FALSE);
	}

	/*
	** allocate memory for the node
	*/

    if (!SmartMalloc(pNode, sizeof(smartStackNode), &pStack->memoryAllocated))
	{
		return(FALSE);
	}

	/*
	** allocate memory for the data object
	*/

	if (!SmartMalloc(&lData, pDataSize, &pStack->memoryAllocated))
	{
		SmartFree(pNode, sizeof(smartStackNode), &pStack->memoryAllocated);

		return(FALSE);
	}

	if (NULL != pData)
    {
		*pData = (smartStackHandle) lData; 
    }

	/*
	** initialize the node
	*/

    (* pNode)->below = NULL;

    (* pNode)->data = lData;
    (* pNode)->dataSize = pDataSize;

    return(TRUE);
}

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackPushNode
(
    smartStackHandle pStack,
    smartStackNodeHandle pNode
)
{
    /*
    ** there is no stack
    */

    if (NULL == pStack)
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

    pNode->below = pStack->top;
    pStack->top = pNode;

    if (NULL == pStack->bottom)
    {
        pStack->bottom = pNode;
    }

    pStack->depth++;

	return(TRUE);
}

STORAGE_CLASS smartStackNodeHandle CALLING_CONVENTION SmartStackPopNode
(
    smartStackHandle pStack
)
{
    smartStackNodeHandle lNode;
    smartStackNodeHandle lBelow;

    /*
    ** there is no stack
    */

    if (NULL == pStack)
    {
        return(NULL);
    }

	/*
	** the stack is empty
	*/

    if (NULL == pStack->top)
    {
        return(NULL);
    }

	/*
	** retrieve the top node
	*/

	lNode = pStack->top;
	lBelow = lNode->below;

	/*
	** update stack
	*/

	pStack->top = lBelow;

    if (NULL == lBelow)
    {
        pStack->bottom = NULL;
    }

    pStack->depth--;

	/*
	** return the node handle
	*/

    return(lNode);
}

STORAGE_CLASS smartStackNodeHandle CALLING_CONVENTION SmartStackGetNextNode
(
    smartStackHandle pStack,
    smartStackNodeHandle pNode
)
{
    /*
    ** there is no stack
    */

	if (NULL == pStack)
	{
		return(NULL);
	}

    /*
    ** get the top node
    */

    if (NULL == pNode)
    {
        return(pStack->top);
    }

	/*
	** get the next node
	*/

	return(pNode->below);
}

STORAGE_CLASS smartStackDataHandle CALLING_CONVENTION SmartStackGetNodeData
(
    smartStackNodeHandle pNode
)
{ 
    /*
    ** there is no node
    */

    if (NULL == pNode)
    {
        return(NULL);
    }

	/*
	** return the data handle
	*/

    return(pNode->data);
}

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackSplit
(
    smartStackHandle pTopStack,
    smartStackNodeHandle pNode,
    smartStackHandle * pBottomStack
)
{
    smartStackNodeHandle lNode;
    smartStackNodeHandle lTopStackBottomNode;

	size_t lTopAllocated = 0;

	unsigned long lTopDepth;
	unsigned long lNodeIndex;

    /*
    ** there is no top stack
    */

	if (NULL == pTopStack)
	{
		return(FALSE);
	}

    /*
    ** there is no bottom stack handle
    */

	if (NULL == pBottomStack)
	{
		return(FALSE);
	}

	/*
	** traverse to the split point
	*/

	lNode = pTopStack->top;

	if (NULL != pNode)
	{
		for (lNodeIndex = 0; pNode != lNode; lNodeIndex++)
		{
			lTopAllocated += lNode->dataSize;

			lTopStackBottomNode = lNode;

			lNode = lNode->below;

			if (NULL == lNode)
			{
				return(FALSE); /* split node not found */
			}
		}

		lTopDepth = lNodeIndex;
	}
	else
	{
		lTopDepth = pTopStack->depth / 2;

		for (lNodeIndex = 0; lTopDepth > lNodeIndex; lNodeIndex++)
		{
			lTopAllocated += lNode->dataSize;

			lTopStackBottomNode = lNode;

			lNode = lNode->below;
		}
	}

	SmartStackConstructSmartStack(pBottomStack, pTopStack->memoryMaximum);

	(* pBottomStack)->top = lNode;
	(* pBottomStack)->bottom = pTopStack->bottom;
	(* pBottomStack)->depth = pTopStack->depth - lTopDepth;
	(* pBottomStack)->memoryAllocated = pTopStack->memoryAllocated - lTopAllocated;

	if (pTopStack->top = pNode)
	{
		pTopStack->top = NULL;
		pTopStack->bottom = NULL;
	}
	else
	{
		pTopStack->bottom = lTopStackBottomNode;
	}

	pTopStack->depth = lTopDepth;
	pTopStack->memoryAllocated = lTopAllocated;

	return(TRUE);
}

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackSplice
(
    smartStackHandle pTopStack,
    smartStackHandle * pBottomStack
)
{
    /*
    ** there is no top stack
    */

	if (NULL == pTopStack)
	{
		return(FALSE);
	}

    /*
    ** there is no bottom stack
    */

	if (NULL == pBottomStack || NULL == *pBottomStack)
	{
		return(FALSE);
	}

	/*
	** the top stack cannot grow to accomodate the addition of the bottom stack
	*/

	if (pTopStack->memoryMaximum < pTopStack->memoryAllocated + (* pBottomStack)->memoryAllocated - sizeof(smartStack))
	{
		return(FALSE);
	}

	/*
	** attach the bottom stack to the top stack
	*/

    pTopStack->bottom->below = (* pBottomStack)->top;

	(* pBottomStack)->top = NULL;

    pTopStack->memoryAllocated += (* pBottomStack)->memoryAllocated - sizeof(smartStack);

	(* pBottomStack)->memoryAllocated = 0;

    pTopStack->depth += (* pBottomStack)->depth;

	(* pBottomStack)->depth = 0;

	/*
	** destruct the bottom stack's control structure
	*/

	SmartStackDestructSmartStack(pBottomStack);

	return(TRUE);
}

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackDestructNode
(
    smartStackHandle pStack,
    smartStackNodeHandle * pNode
)
{
    /*
    ** there is no stack
    */

    if (NULL == pStack)
    {
        return(FALSE);
    }

    /*
    ** there is no node
    */

    if (NULL == pNode || NULL == *pNode)
    {
        return(FALSE);
    }

	/*
	** destruct the data and node
	*/

	if (!SmartFree(&(* pNode)->data, (* pNode)->dataSize, &pStack->memoryAllocated))
	{
		return(FALSE);
	}

	if (!SmartFree(pNode, sizeof(smartStackNode), &pStack->memoryAllocated))
	{
		return(FALSE);
	}

	return(TRUE);
}

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackDestructSmartStack
(
    smartStackHandle * pStack
)
{
    smartStackNodeHandle lNode;
    smartStackNodeHandle lBelowNode;

    /*
    ** there is no stack
    */

    if (NULL == pStack || NULL == *pStack)
	{
		return(TRUE);
	}

	/*
	** destruct all of the nodes in the stack
	*/

	for (lNode = (* pStack)->top; NULL != lNode; lNode = lBelowNode)
    {
        lBelowNode = lNode->below;

		if (!SmartStackDestructNode(*pStack, &lNode))
		{
			return(FALSE);
		}
    }

	/*
	** destruct the stack control structure
	*/

    SafeFree(pStack);

	return(TRUE);
}

STORAGE_CLASS unsigned long CALLING_CONVENTION SmartStackGetDepth
(
    smartStackHandle pStack
)
{
    /*
    ** there is no stack
    */

    if (NULL == pStack)
    {
        return(0);
    }

    return(pStack->depth);
}

STORAGE_CLASS size_t CALLING_CONVENTION SmartStackGetMemoryAllocated
(
    smartStackHandle pStack
)
{
    /*
    ** there is no stack
    */

    if (NULL == pStack)
    {
        return(0);
    }

    return(pStack->memoryAllocated);
}

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackIsValid
(
    smartStackHandle pStack
)
{
	unsigned long lDepth;

    smartStackNodeHandle lNode;

    /*
    ** there is no stack
    */

	if (NULL == pStack)
	{
		return(TRUE);
	}

	lNode = pStack->top;

	/*
	** there are no nodes in the stack
	*/

	if (NULL == lNode)
	{
		return(TRUE);
	}

	/*
	** verify the stack depth
	*/

	for (lDepth = 1; NULL != lNode->below; lNode = lNode->below)
	{
		lDepth++;
	}

	if (pStack->depth != lDepth)
	{
		return(FALSE);
	}

	/*
	** verify the bottom node
	*/

	if (pStack->bottom != lNode)
	{
		return(FALSE);
	}

	/*
	** the stack is valid
	*/

	return(TRUE);
}

/*----------------------------------------------------------------------------
  Private functions
  ----------------------------------------------------------------------------*/
