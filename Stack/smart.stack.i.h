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
  Smart Stack application programmer's interface (API) header file
  ----------------------------------------------------------------------------*/

#ifndef SMART_STACK_I_H
#define SMART_STACK_I_H

/*----------------------------------------------------------------------------
  SmartStackConstructSmartStack()
  ----------------------------------------------------------------------------
  Construct an empty stack.
  ----------------------------------------------------------------------------
  Parameters:

  pStack         - (I/O) Pointer to recieve the stack handle
  pMemoryMaximum - (I)   The maximum number of bytes used by the stack
  ----------------------------------------------------------------------------
  Return Values:

  True  - Stack was succesfully constructed

  False - Stack was not successfully constructed due to:

          1. The SafeMalloc() failed
  ----------------------------------------------------------------------------
  Notes:

  The maximum memory paramter may be set to zero to allow the stack to grow to
  the operating system controlled process memory limit.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackConstructSmartStack
(
    smartStackHandle * pStack,
	size_t pMemoryMaximum
);

/*----------------------------------------------------------------------------
  SmartStackConstructNode()
  ----------------------------------------------------------------------------
  Construct a stack node.
  ----------------------------------------------------------------------------
  Parameters:

  pStack    - (I)   Stack handle
  pNode     - (I/O) Pointer to recieve the node handle
  pData     - (I/O) Pointer to receive the data object handle
  pDataSize - (I)   The number of bytes used by the data object
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully constructed

  False - Node was not successfully constructed due to:

          1. The stack handle was invalid
		  2. The pointer to the data object handle was invalid
		  3. SmartMalloc() failed
		  4. Would make the stack exceed its maximum number of bytes
  ----------------------------------------------------------------------------
  Notes:

  The memory allocated for a node, its key object and its data object are
  managed by the stack associated to the node. Pushing and popping of nodes
  from the associated stack does nothing to the memory allocation. As a result
  nodes may be reused (without reconstruction) after being popped.

  Destructing a stack will destruct all associated nodes and data objects.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackConstructNode
(
    smartStackHandle pStack,
    smartStackNodeHandle * pNode,
    smartStackDataHandle * pData,
	size_t pDataSize
);

/*----------------------------------------------------------------------------
  SmartStackPushNode()
  ----------------------------------------------------------------------------
  Place a node on the stack.
  ----------------------------------------------------------------------------
  Parameters:

  pStack - (I) Stack handle
  pNode  - (I) Node handle
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully pushed

  False - Node was not successfully pushed due to:

          1. The pStack handle was NULL
          2. The pNode handle was NULL
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackPushNode
(
    smartStackHandle pStack,
    smartStackNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartStackPopNode()
  ----------------------------------------------------------------------------
  Pop a node from the stack.
  ----------------------------------------------------------------------------
  Parameters:

  pStack - (I) Handle of the stack in which the node to pop exists
  ----------------------------------------------------------------------------
  Return Values:

  smartStackNodeHandle - Node was successfully popped from the stack

  NULL - Node was not successfully popped from the stack due to:

          1. The pStack handle was NULL
		  2. The stack is empty
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartStackNodeHandle CALLING_CONVENTION SmartStackPopNode
(
    smartStackHandle pStack
);

/*----------------------------------------------------------------------------
  SmartStackGetNextNode()
  ----------------------------------------------------------------------------
  Get the node below the node passed from the stack.
  ----------------------------------------------------------------------------
  Parameters:

  pStack - (I) Handle of the stack in which the node to get exists
  pNode  - (I) Node handle
  ----------------------------------------------------------------------------
  Return Values:

  smartStackNodeHandle - Node was successfully retrieved from the stack

  NULL - Node was not successfully retrieved from the stack due to:

          1. The pStack handle was NULL
		  2. The stack is empty
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartStackNodeHandle CALLING_CONVENTION SmartStackGetNextNode
(
    smartStackHandle pStack,
    smartStackNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartStackGetTop()
  ----------------------------------------------------------------------------
  Get the node at the top of the stack.
  ----------------------------------------------------------------------------
  Parameters:

  pStack - (I) Handle of the stack in which the node to get exists
  ----------------------------------------------------------------------------
  Return Values:

  smartStackNodeHandle - Node was successfully retrieved from the stack

  NULL - Node was not successfully retrieved from the stack due to:

          1. The pStack handle was NULL
		  2. The stack is empty
  ----------------------------------------------------------------------------*/

#define SmartStackGetTopNode(pStack) SmartStackGetNextNode(pStack, NULL)

/*----------------------------------------------------------------------------
  SmartStackGetNodeData()
  ----------------------------------------------------------------------------
  Get the data object attached to the node.
  ----------------------------------------------------------------------------
  Parameters:

  pNode - (I) Handle of the node to get its data object
  ----------------------------------------------------------------------------
  Return Values:

  smartStackDataHandle - The data object attached to the node

  NULL - No node handle was provided or the data handle was NULL
  ----------------------------------------------------------------------------
  Notes:

  It is permissible to change the data value after the node is inserted into
  the stack.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS smartStackDataHandle CALLING_CONVENTION SmartStackGetNodeData
(
    smartStackNodeHandle pNode
);

/*----------------------------------------------------------------------------
  SmartStackSpilt()
  ----------------------------------------------------------------------------
  Append a stack to another stack.
  ----------------------------------------------------------------------------
  Parameters:

  pTopStack    - (I) Handle of the stack to be bifricated
  pNode        - (I) Node within pTopStack at which to split pTopStack
  pBottomStack - (O) Handle of the stack created from the bottom of pTopStack
  ----------------------------------------------------------------------------
  Return Values:

  True  - Stack was succesfully bifricated

  False - Stack was not successfully bifricated due to:

          1. The pTopStack handle was NULL
          2. The pBottomStack handle was NULL
  ----------------------------------------------------------------------------
  Notes:

  The passed node becomes the top of the bottom stack.

  When pNode is NULL the top stack will be spilt at the middle node.

  Splitting an empty stack has the effect of creating an empty bottom stack.

  The created bottom stack has the same the maximum number of bytes as the
  top stack from which it was created.
  ----------------------------------------------------------------------------*/
	
STORAGE_CLASS Bool CALLING_CONVENTION SmartStackSplit
(
    smartStackHandle pTopStack,
    smartStackNodeHandle pNode,
    smartStackHandle * pBottomStack
);

/*----------------------------------------------------------------------------
  SmartStackSplice()
  ----------------------------------------------------------------------------
  Append a stack to another stack.
  ----------------------------------------------------------------------------
  Parameters:

  pTopStack    - (I)   Handle of the stack to be extended
  pBottomStack - (I/O) Handle of the stack to be appended and destructed
  ----------------------------------------------------------------------------
  Return Values:

  True  - Stack was succesfully appended

  False - Stack was not successfully appended due to:

          1. The pTopStack handle was NULL
          2. The pBottomStack handle was NULL
		  3. Would make the top stack exceed its maximum number of bytes
  ----------------------------------------------------------------------------*/
	
STORAGE_CLASS Bool CALLING_CONVENTION SmartStackSplice
(
    smartStackHandle pTopStack,
    smartStackHandle * pBottomStack
);

/*----------------------------------------------------------------------------
  SmartStackDestructNode()
  ----------------------------------------------------------------------------
  Destruct a node.
  ----------------------------------------------------------------------------
  Parameters:

  pStack - (I)   Handle of the stack to which the node is associated
  pNode  - (I/O) Handle of the node remove
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully destructed

  False - Node was not successfully destructed due to:

          1. The pStack handle was NULL
          2. The pNode handle pointer was NULL
          3. The node data destruction failed
          4. The node destruction failed
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackDestructNode
(
    smartStackHandle pStack,
    smartStackNodeHandle * pNode
);

/*----------------------------------------------------------------------------
  SmartStackDestructSmartStack()
  ----------------------------------------------------------------------------
  Destruct a stack by removing all nodes.
  ----------------------------------------------------------------------------
  Parameters:

  pStack - (I/O) Pointer to the stack handle
  ----------------------------------------------------------------------------
  Return Values:

  True  - Stack was succesfully destructed

  False - Stack was not successfully destructed due to:

          1. The pStack handle pointer was NULL
		  2. The destruction of a node data object failed
          3. The SafeFree() of a node failed
		  4. The SafeFree() of the stack failed
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackDestructSmartStack
(
    smartStackHandle * pStack
);

/*----------------------------------------------------------------------------
  SmartStackGetDepth()
  ----------------------------------------------------------------------------
  Determine the nodes in the stack.
  ----------------------------------------------------------------------------
  Parameters:

  pStack - (I) Handle of the stack
  ----------------------------------------------------------------------------
  Return Values:

  long - The number of nodes within the stack
  ----------------------------------------------------------------------------*/

STORAGE_CLASS unsigned long CALLING_CONVENTION SmartStackGetDepth
(
    smartStackHandle pStack
);

/*----------------------------------------------------------------------------
  SmartStackGetMemoryAllocated()
  ----------------------------------------------------------------------------
  Determine the number bytes allocated by the stack.
  ----------------------------------------------------------------------------
  Parameters:

  pStack - (I) Stack handle
  ----------------------------------------------------------------------------
  Return Values:

  size_t - The number of bytes allocated to the stack
  ----------------------------------------------------------------------------*/

STORAGE_CLASS size_t CALLING_CONVENTION SmartStackGetMemoryAllocated
(
    smartStackHandle pStack
);

/*----------------------------------------------------------------------------
  SmartStackIsValid()
  ----------------------------------------------------------------------------
  Test every node within a stack to assure that the structure is correct.
  ----------------------------------------------------------------------------
  Parameters:

  pStack - (I) The stack handle
  ----------------------------------------------------------------------------
  Return Values:

  True  - Stack passed the validity check

  False - Stack did not pass the validity check due to:

          1. The depth of the stack was found to be incorrect
          2. The bottom of the stack was incorrect
  ----------------------------------------------------------------------------
  Note:

  This function is primarily used for debugging of the smartStack module. A
  validity check failure suggests a bug in the smartStack implementation.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartStackIsValid
(
    smartStackHandle pStack
);

#endif
