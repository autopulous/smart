/*----------------------------------------------------------------------------
  Smart Tree test application

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
  Smart Tree test program header file
  ----------------------------------------------------------------------------*/

#ifndef SMART_TREE_TEST_H
#define SMART_TREE_TEST_H

#define TEST_NODES 100000

#define DATA_ELEMENT_SIZE 64

#ifdef UNPREDICTABLE_RANDOMNESS
#define TEST_SEED ((unsigned int)time(NULL))
#else
#define TEST_SEED 1
#endif

/*----------------------------------------------------------------------------
  Private function prototypes
  ----------------------------------------------------------------------------*/

void DisplayOptions
(
    void
);

void DestructNodes
(
    void
);

void ConstructNodes
(
	void
);

void CreateNode
(
	FILE * pFile,
	smartTreeNodeHandle * pNode
);

void ChooseNode
(
	FILE * pFile,
	smartTreeNodeHandle * pNode
);

void InsertNode
(
	smartTreeNodeHandle pNode
);

void SelectNode
(
	long * pKeyPointer,
	smartTreeNodeHandle * pNode
);

void SelectNextLesserNode
(
	smartTreeNodeHandle * pNode
);

void SelectNextGreaterNode
(
	smartTreeNodeHandle * pNode
);

void FetchNode
(
	smartTreeNodeHandle pNode,
	long ** pKeyPointer,
	char ** pDataPointer
);

void ModifyNode
(
	smartTreeNodeHandle pNode
);

void RemoveNode
(
	FILE *pFile,
	smartTreeNodeHandle * pNode
);

void OutputNode
(
	FILE * pFile,
	smartTreeNodeHandle pNode
);

void OutputNextLesserNode
(
	FILE * pFile,
	smartTreeNodeHandle * pNode
);

void OutputNextGreaterNode
(
	FILE * pFile,
	smartTreeNodeHandle * pNode
);

void OutputNodeContent
(
	FILE * pFile,
	long * pKeyPointer,
	char * pDataPointer
);

/*!
 * Print a tree diagram
 *
 * \param pFile [I] - the output stream
 *
 * \return nothing
 */

void OutputTreeDiagram
(
    FILE * pFile
);

/*!
 * Print a level within a tree diagram
 *
 * This function recursively calls itself (left to right / lesser to greater) to 
 * find all of the nodes on a particular level of the tree to print. For each
 * level within the tree this function must restart from the tree's root to find
 * the nodes at the given level.
 *
 * \param pFile [I] - the output stream
 * \param pNode [I] - the node to potentially print or traverse to its children
 * \param pTreeDepth [I] - the number of levels within the tree being printed
 * \param pPrintLevel [I] - the level being printed within the tree 
 * \param pTraversalLevel [I] - the level being considered for printing within the tree 
 *
 * \return nothing
 */

void OutputTreeDiagramLevel
(
    FILE * pFile,
    smartTreeNodeHandle pNode,
	unsigned long pTreeDepth,
	unsigned long pPrintLevel,
	unsigned long pTraversalLevel
);

/*
 Print the tree structure
 
 * \param pFile [I]
 * \param pTree [I]
 * \param pNodes [I]
 * \return nothing
 */

void OutputTreeStructure
(
    FILE * pFile
);

void OutputTreeDataAscending
(
    FILE * pFile
);

void OutputTreeDataDescending
(
    FILE * pFile
);

void OutputTreeInformation
(
    FILE * pFile
);

void IteratedRandomSelfTest
(
	char * pOutputFile
);

void IteratedRandomSelfPerformanceTest
(
	void
);

long _compare
(
    const smartTreeKeyHandle pKey1,
    const smartTreeKeyHandle pKey2
);

#endif
