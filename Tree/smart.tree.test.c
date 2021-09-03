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
  Smart Tree test program implementation file
  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Standard libraries
  ----------------------------------------------------------------------------*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*----------------------------------------------------------------------------
  Public data types
  ----------------------------------------------------------------------------*/

#include "compilation.t.h"
#include "types.t.h"
#include "math.t.h"

#include "smart.tree.t.h"

/*----------------------------------------------------------------------------
  Public functions
  ----------------------------------------------------------------------------*/

#include "smart.memory.i.h"

#include "smart.tree.i.h"

/*----------------------------------------------------------------------------
  Private defines, data types and function prototypes
  ----------------------------------------------------------------------------*/

#include "smart.tree.test.h"

/*----------------------------------------------------------------------------
  <Eeek> Globals </Eeek>
  ----------------------------------------------------------------------------*/

smartTreeHandle gTree;

smartTreeNodeHandle gNodes[TEST_NODES];

unsigned long gNodeCount;

/*----------------------------------------------------------------------------
  Main
  ----------------------------------------------------------------------------*/

void main
(
    void
)
{
    int lOption;

    smartTreeNodeHandle lNode = NULL;

	srand(TEST_SEED);

	ConstructNodes();

    do
    {
		printf("Option: ");

		do
		{
			lOption = toupper(fgetc(stdin));
		}
		while (!isprint(lOption)); /* eat carriage returns (etc) */

        switch ((char) lOption)
        {
            case '?':
            {
				DisplayOptions();
                break;
            }

            case 'Q':
            {
				DestructNodes();
				break;
            }

            case 'R':
            {
				DestructNodes();
				ConstructNodes();
				break;
            }

            case 'C':
            {
				CreateNode(stdout, &lNode);
				break;
            }
        
            case 'F':
            {
				ChooseNode(stdout, &lNode);
                break;
            }

            case 'L':
            {
                OutputNextLesserNode(stdout, &lNode);
                break;
            }

            case 'G':
            {
                OutputNextGreaterNode(stdout, &lNode);
                break;
            }

			case 'U':
			{
				ModifyNode(lNode);
				OutputNode(stdout, lNode);
				break;
			}

			case 'X':
            {
				RemoveNode(stdout, &lNode);
				break;
            }

            case 'P':
            {
				IteratedRandomSelfPerformanceTest();
                break;
            }

            case 'T':
            {
				IteratedRandomSelfTest(NULL);
                break;
            }

			case 'D':
			{
				OutputTreeDiagram(stdout);
				break;
			}

			case 'S':
            {
                OutputTreeStructure(stdout);
                break;
            }

			case 'A':
            {
                OutputTreeDataAscending(stdout);
                break;
            }

			case 'Z':
            {
                OutputTreeDataDescending(stdout);
                break;
            }

			case 'I':
            {
                OutputTreeInformation(stdout);
                break;
            }

            default:
            {
                printf("Valid options are C,F,L,G,U,X,R,P,T,D,S,A,Z,I,Q,?\n");
                break;
            }
        }
    }
	while ('Q' != lOption);
}

void DisplayOptions
(
    void
)
{
	printf("\n"
		   "Options:\n"
		   "(C) Create a node (sets current node)\n\n"
		   "(F) Find a node (sets current node)\n"
		   "(L) Navigate to the next lesser node (sets current node)\n"
		   "(G) Navigate to the next greater node (sets current node)\n\n"
		   "(U) Update current node\n"
		   "(X) Delete current node\n\n"
		   "(R) Remove all nodes\n\n"
		   "(P) Iterated performance speed test\n"
		   "(T) Iterated thorough test\n\n"
		   "(D) Display tree disgramatically\n"
		   "(S) Display tree structurally\n"
		   "(A) Display nodes ordered A-Z\n"
		   "(Z) Display nodes ordered Z-A\n"
		   "(I) Display tree information\n\n"
		   "(Q) Quit\n"
		   "(?) Display this option list\n"
		   "\n");
}

void DestructNodes
(
    void
)
{
    SmartTreeDestructSmartTree(&gTree);

	SmartTreeIsValid(gTree);
}

void ConstructNodes
(
	void
)
{
    SmartTreeConstructSmartTree(&gTree, _compare, (size_t) 0);

	SmartTreeIsValid(gTree);

    memset(gNodes, 0, sizeof(gNodes));

    gNodeCount = 0;
}

void CreateNode
(
	FILE * pFile,
	smartTreeNodeHandle * pNode
)
{
	static unsigned long lCreationOrdinal = 1;

	smartTreeNodeHandle lNode = NULL;

    long * lKeyPointer = NULL;
    char * lDataPointer = NULL;

	SmartTreeConstructNode(gTree, &lNode, &lKeyPointer, sizeof(long), &lDataPointer, (size_t) DATA_ELEMENT_SIZE);

	printf("\n");
	printf("Enter integer key for new node: ");
	scanf("%ld", lKeyPointer);
	printf("\n");

	sprintf(lDataPointer, "Creation Ordinal:%08ld", lCreationOrdinal++);

	InsertNode(lNode);

	SelectNode(lKeyPointer, &lNode);

	FetchNode(lNode, &lKeyPointer, &lDataPointer);

	OutputNodeContent(pFile, lKeyPointer, lDataPointer);

	*pNode = lNode;
}

void ChooseNode
(
	FILE * pFile,
	smartTreeNodeHandle * pNode
)
{
	/* because the key isn't being attached to the tree we may allocate it from the stack */

	long lKey;
	long * lKeyPointer = &lKey;

	printf("\n");
	printf("Enter key of node to fetch: ");
	scanf("%ld", lKeyPointer);
	printf("\n");

	SelectNode(lKeyPointer, pNode);

	OutputNode(pFile, *pNode);
}

void InsertNode
(
	smartTreeNodeHandle pNode
)
{
	SmartTreeInsertNode(gTree, pNode);

	gNodes[gNodeCount++] = pNode;

	SmartTreeIsValid(gTree);
}

void SelectNode
(
	long * pKeyPointer,
	smartTreeNodeHandle * pNode
)
{
    *pNode = SmartTreeGetEqualNode(gTree, pKeyPointer);
	
	SmartTreeIsValid(gTree);
}

void SelectNextLesserNode
(
	smartTreeNodeHandle * pNode
)
{
	smartTreeNodeHandle lNode = SmartTreeGetNextLesserNode(gTree, *pNode);

	if (NULL == lNode)
	{
		printf("No lesser node\n");
	}
	else
	{
		*pNode = lNode;
	}

	SmartTreeIsValid(gTree);
}

void SelectNextGreaterNode
(
	smartTreeNodeHandle * pNode
)
{
	smartTreeNodeHandle lNode = SmartTreeGetNextGreaterNode(gTree, *pNode);

	if (NULL == lNode)
	{
		printf("No greater node\n");
	}
	else
	{
		*pNode = lNode;
	}

	SmartTreeIsValid(gTree);
}

void FetchNode
(
	smartTreeNodeHandle pNode,
	long ** pKeyPointer,
	char ** pDataPointer
)
{
	if (NULL == pNode)
	{
		*pKeyPointer = NULL;
		*pDataPointer = NULL;
	}
	else
	{
		*pKeyPointer = (long *) SmartTreeGetNodeKey(pNode);
		*pDataPointer = (char *) SmartTreeGetNodeData(pNode);
	}

	SmartTreeIsValid(gTree);
}

void ModifyNode
(
	smartTreeNodeHandle pNode
)
{
	long * lKeyPointer;
	char * lDataPointer;

	time_t lTime;

	if (NULL == pNode)
	{
		printf("Use find node to establish the node to modify\n\n");
	}
	else
	{
		FetchNode(pNode, &lKeyPointer, &lDataPointer);

		time(&lTime);

		strftime(lDataPointer, (size_t) DATA_ELEMENT_SIZE, "Modified: %Y-%m=%d %a %H:%M:%S", localtime(&lTime));
	}
}

void RemoveNode
(
	FILE *pFile,
	smartTreeNodeHandle * pNode
)
{
	long * lKeyPointer;
	char * lDataPointer;

    unsigned long lNodeIndex;

	if (NULL == pNode || NULL == *pNode)
	{
		printf("Use find node to establish the node to delete\n\n");
	}
	else
	{
		/*
		** find the node to delete in the node array
		*/

		for (lNodeIndex = 0; *pNode != gNodes[lNodeIndex]; lNodeIndex++);

		/*
		** print the node content
		*/

		FetchNode(*pNode, &lKeyPointer, &lDataPointer);

		OutputNodeContent(pFile, lKeyPointer, lDataPointer);

		/*
		** delete the node
		*/

		SmartTreeDeleteNode(gTree, *pNode);
		SmartTreeDestructNode(gTree, pNode);

		/*
		** squish the deleted node from the node array
		*/

		for (--gNodeCount; lNodeIndex < gNodeCount; lNodeIndex++)
		{
			gNodes[lNodeIndex] = gNodes[lNodeIndex + 1];
		}

		gNodes[lNodeIndex] = NULL;
	}

	SmartTreeIsValid(gTree);
}

void OutputNode
(
	FILE * pFile,
	smartTreeNodeHandle pNode
)
{
	long * lKeyPointer;
	char * lDataPointer;

	FetchNode(pNode, &lKeyPointer, &lDataPointer);

	OutputNodeContent(pFile, lKeyPointer, lDataPointer);
}

void OutputNextLesserNode
(
	FILE * pFile,
	smartTreeNodeHandle * pNode
)
{
	long * lKeyPointer = NULL;
	char * lDataPointer = NULL;

	if (NULL != *pNode)
	{
		SelectNextLesserNode(pNode);

		FetchNode(*pNode, &lKeyPointer, &lDataPointer);
	}

	OutputNodeContent(pFile, lKeyPointer, lDataPointer);
}

void OutputNextGreaterNode
(
	FILE * pFile,
	smartTreeNodeHandle * pNode
)
{
	long * lKeyPointer = NULL;
	char * lDataPointer = NULL;

	if (NULL != *pNode)
	{
		SelectNextGreaterNode(pNode);

		FetchNode(*pNode, &lKeyPointer, &lDataPointer);
	}

	OutputNodeContent(pFile, lKeyPointer, lDataPointer);
}

void OutputNodeContent
(
	FILE * pFile,
	long * pKeyPointer,
	char * pDataPointer
)
{
	if (NULL != pKeyPointer)
	{
		fprintf(pFile, "Key (% 06ld), Data \"%s\"\n\n", *pKeyPointer, pDataPointer);
	}
	else
	{
		fprintf(pFile, "Node not found\n\n");
	}
}

void OutputTreeDiagram
(
    FILE * pFile
)
{  
	smartTreeNodeHandle lRoot;

	unsigned long lTreeDepth;
	unsigned long lPrintLevel;

	lRoot = SmartTreeGetRoot(gTree);

	lTreeDepth = SmartTreeGetMaxDepth(lRoot);

	/*
	** print the tree a level at a time
	*/

	for (lPrintLevel = 1; lTreeDepth >= lPrintLevel; lPrintLevel++)
	{
		/*
		** center the the nodes on the current level
		*/

		if (lTreeDepth > lPrintLevel)
		{
			fprintf(pFile, "%*c", (TwoRaisedToThePowerOf(lTreeDepth - lPrintLevel) - 1) * 5, ' ');
		}

		/*
		** print the nodes from the current level
		*/

		OutputTreeDiagramLevel(pFile, lRoot, lTreeDepth, lPrintLevel, 1);

        fprintf(pFile, "\n");
	}

    fprintf(pFile, "\n");
}

void OutputTreeDiagramLevel
(
    FILE * pFile,
    smartTreeNodeHandle pNode,
	unsigned long pTreeDepth,
	unsigned long pPrintLevel,
	unsigned long pTraversalLevel
)
{
	if (pPrintLevel > pTraversalLevel)
	{
		/*
		** not at the level currently being printed:
		**
		** traverse to the lesser and greater children at the next lower level
		*/

	    smartTreeNodeHandle lNode = NULL;

		pTraversalLevel++;

		if (NULL != pNode)
		{
			lNode = SmartTreeGetLesserChildNode(pNode);
		}

		OutputTreeDiagramLevel(pFile, lNode, pTreeDepth, pPrintLevel, pTraversalLevel);

		if (NULL != pNode)
		{
			lNode = SmartTreeGetGreaterChildNode(pNode);
		}

		OutputTreeDiagramLevel(pFile, lNode, pTreeDepth, pPrintLevel, pTraversalLevel);
	}
	else
	{
		/*
		** at the level currently being printed:
		**
		** print a blank for an empty node or the data from a populated node
		*/

		if (NULL == pNode)
		{
	        fprintf(pFile, "%*c", ((TwoRaisedToThePowerOf(pTreeDepth - (pPrintLevel - 1)) - 1) * 5) + 5, ' ');
		}
		else
		{
	        fprintf(pFile, "% 05ld%*c", *((long *) SmartTreeGetNodeKey(pNode)), ((TwoRaisedToThePowerOf(pTreeDepth - (pPrintLevel - 1)) - 1) * 5), ' ');
		}
	}
}

void OutputTreeStructure
(
    FILE * pFile
)
{
    long * lKeyPointer;
    char * lDataPointer;

    long lIndex;

    smartTreeNodeHandle lNode;

    if (NULL != gNodes[0])
    {
        fprintf(pFile, "Index Parent   Current  Lesser   Greater\n"
                       "----- -------- -------- -------- --------\n");

        for (lIndex = 0; NULL != gNodes[lIndex] && lIndex < TEST_NODES; lIndex++)
        {
            fprintf(pFile, "% 4ld: ", lIndex + 1);

            lNode = SmartTreeGetParentNode(gNodes[lIndex]);

            if (NULL == lNode)
            {
                fprintf(pFile, "( root ) ");
            }
            else
            {
                lKeyPointer = (long *) SmartTreeGetNodeKey(lNode);

                fprintf(pFile, "(% 06ld) ", *lKeyPointer);
            }

            lKeyPointer = (long *) SmartTreeGetNodeKey(gNodes[lIndex]);
            lDataPointer = (char *) SmartTreeGetNodeData(gNodes[lIndex]);

            fprintf(pFile, "(% 06ld) ", *lKeyPointer);

            lNode = SmartTreeGetLesserChildNode(gNodes[lIndex]);

            if (NULL == lNode)
            {
                fprintf(pFile, "(      ) ", lIndex);
            }
            else
            {
                lKeyPointer = (long *) SmartTreeGetNodeKey(lNode);

                fprintf(pFile, "(% 06ld) ", *lKeyPointer);
            }

            lNode = SmartTreeGetGreaterChildNode(gNodes[lIndex]);

            if (NULL == lNode)
            {
                fprintf(pFile, "(      )\n", lIndex);
            }
            else
            {
                lKeyPointer = (long *) SmartTreeGetNodeKey(lNode);

                fprintf(pFile, "(% 06ld)\n", *lKeyPointer);
            }
        }
    }

	fprintf(pFile, "\n");
}

void OutputTreeDataAscending
(
    FILE * pFile
)
{
    long * lLastKeyPointer = NULL;
    long * lKeyPointer;
    char * lDataPointer;

    long lIndex = 0;

    smartTreeNodeHandle lNode = SmartTreeGetLeastNode(gTree);

    while (NULL != lNode)
    {
        lKeyPointer = (long *) SmartTreeGetNodeKey(lNode);
        lDataPointer = (char *) SmartTreeGetNodeData(lNode);

        fprintf(pFile, "% 4ld: key (% 05ld), data = \"%s\"", ++lIndex, *lKeyPointer, lDataPointer);

		if (NULL != lLastKeyPointer && 0 < _compare(lLastKeyPointer, lKeyPointer))
        {
            fprintf(pFile, " <error> - out of sequence");
        }

        fprintf(pFile, "\n");

        lLastKeyPointer = lKeyPointer;

        lNode = SmartTreeGetNextGreaterNode(gTree, lNode);
    }

	fprintf(pFile, "\n");
}

void OutputTreeDataDescending
(
    FILE * pFile
)
{
    long * lLastKeyPointer = NULL;
    long * lKeyPointer;
    char * lDataPointer;

    long lIndex = 0;

    smartTreeNodeHandle lNode = SmartTreeGetGreatestNode(gTree);

    while (NULL != lNode)
    {
        lKeyPointer = (long *) SmartTreeGetNodeKey(lNode);
        lDataPointer = (char *) SmartTreeGetNodeData(lNode);

        fprintf(pFile, "% 4ld: key (% 05ld), data = \"%s\"", ++lIndex, *lKeyPointer, lDataPointer);

		if (NULL != lLastKeyPointer && 0 > _compare(lLastKeyPointer, lKeyPointer))
        {
            fprintf(pFile, " <error> - out of sequence");
        }

        fprintf(pFile, "\n");

        lLastKeyPointer = lKeyPointer;

        lNode = SmartTreeGetNextLesserNode(gTree, lNode);
    }

	fprintf(pFile, "\n");
}

void OutputTreeInformation
(
    FILE * pFile
)
{
	smartTreeNodeHandle lNode = SmartTreeGetRoot(gTree);

	fprintf(pFile, "\n", SmartTreeGetNodeCount(lNode));
	fprintf(pFile, "Node Count = %ld\n", SmartTreeGetNodeCount(lNode));
	fprintf(pFile, "Tree Depth = %ld\n", SmartTreeGetMaxDepth(lNode));
	fprintf(pFile, "\n", SmartTreeGetNodeCount(lNode));
	fprintf(pFile, "Memory Allocated = %ld\n\n", SmartTreeGetMemoryAllocated(gTree));
}

void IteratedRandomSelfTest
(
	char * pOutputFile
)
{
	FILE * lFile;

	unsigned long lIterations;
	unsigned long lIteration;

    unsigned long lIndex;
    unsigned long lNodeIndex;

	long * lKeyPointer;
    char * lDataPointer;

	printf("\n");
	printf("Iterations: ");
	scanf("%ld", &lIterations);

	for (lIteration = 1; ; lIteration++)
	{
		gNodeCount = 0;
		memset(&gNodes, 0, sizeof(gNodes));

		SmartTreeDestructSmartTree(&gTree);
		SmartTreeConstructSmartTree(&gTree, _compare, (size_t) 0);

		if (lIteration > lIterations)
		{
			break;
		}

		if (NULL != pOutputFile)
		{
			FILE * lFile = fopen(pOutputFile, "w");

			fprintf(lFile, "Iteration: %ld\n\n", lIteration);

			fclose(lFile);
		}

		printf("Iteration : %ld ", lIteration);

		for (lNodeIndex = 0; lNodeIndex < TEST_NODES; lNodeIndex++)
		{
			lKeyPointer = NULL;
			lDataPointer = NULL;

			SmartTreeConstructNode(gTree, &gNodes[lNodeIndex], &lKeyPointer, sizeof(long), &lDataPointer, (size_t) DATA_ELEMENT_SIZE);

			*lKeyPointer = (rand() + rand() - RAND_MAX) % 99999;
			sprintf(lDataPointer, "Entry #%06d", lNodeIndex + 1);

			SmartTreeInsertNode(gTree, gNodes[lNodeIndex]);

			SmartTreeIsValid(gTree);

			if (NULL != pOutputFile)
			{
				lFile = fopen(pOutputFile, "a");

				fprintf(lFile, "-------------------------------------Insert---------------------------------------\n");
				fprintf(lFile, "%s: (%ld)\n\n", lDataPointer, *lKeyPointer);

				fclose(lFile);

				lFile = fopen("smartTree.txt", "a");

				OutputTreeStructure(lFile);

				fprintf(lFile, "\n");

				OutputTreeDataAscending(lFile);

				fclose(lFile);
			}

			if (0 == lNodeIndex % (TEST_NODES / 100))
			{
				switch ((lNodeIndex / (TEST_NODES / 100)) % 4)
				{
				case 0:
					printf("-\b");
					break;
				case 1:
					printf("\\\b");
					break;
				case 2:
					printf("|\b");
					break;
				case 3:
					printf("/\b");
					break;
				}
			}
		}

		printf(">");

		if (NULL != pOutputFile)
		{
			lFile = fopen(pOutputFile, "a");

			fprintf(lFile, "\n");

			OutputTreeDataDescending(lFile);

			fclose(lFile);
		}

		for (gNodeCount = lNodeIndex; gNodeCount > 0;)
		{
			lIndex = rand() % gNodeCount;

			lKeyPointer = SmartTreeGetNodeKey(gNodes[lIndex]);
			lDataPointer = SmartTreeGetNodeData(gNodes[lIndex]);

			if (NULL != pOutputFile)
			{
				lFile = fopen(pOutputFile, "a");

				fprintf(lFile, "--------------------------------------Delete--------------------------------------\n");
				fprintf(lFile, "%s: (%ld)\n\n", lDataPointer, *lKeyPointer);

				fclose(lFile);
			}

			SmartTreeDeleteNode(gTree, gNodes[lIndex]);
			SmartTreeDestructNode(gTree, &gNodes[lIndex]);

			gNodeCount--;

			SmartTreeIsValid(gTree);

			/*
			** squish the deleted node from the node array
			*/
	        
			for (; lIndex < gNodeCount; lIndex++)
			{
				gNodes[lIndex] = gNodes[lIndex + 1];
			}

			if (NULL != pOutputFile)
			{
				lFile = fopen(pOutputFile, "a");

				OutputTreeStructure(lFile);

				fprintf(lFile, "\n");

				OutputTreeDataAscending(lFile);

				fclose(lFile);
			}

			if (0 == gNodeCount % (TEST_NODES / 100))
			{
				switch ((gNodeCount / (TEST_NODES / 100)) % 4)
				{
				case 0:
					printf("-\b");
					break;
				case 1:
					printf("/\b");
					break;
				case 2:
					printf("|\b");
					break;
				case 3:
					printf("\\\b");
					break;
				}
			}
		}

		printf("<");

		if (NULL != pOutputFile)
		{
			lFile = fopen(pOutputFile, "a");

			fprintf(lFile, "\n");

			OutputTreeDataDescending(lFile);

			fclose(lFile);
		}

		printf("\r");
	}

	SmartTreeIsValid(gTree);

	printf("\n\n");
}

void IteratedRandomSelfPerformanceTest
(
	void
)
{
	unsigned long lIterations;
	unsigned long lIteration;

    unsigned long lNodeIndex;

	long lKey;
	long * lKeyPointer;
    char * lDataPointer;

	time_t lStartTime;
	double lInsertSeconds = 0, lSelectSeconds = 0, lDeleteSeconds = 0, lTotalSeconds = 0;

	printf("\n");
	printf("Iterations: ");
	scanf("%ld", &lIterations);

	for (lIteration = 1; ; lIteration++)
	{
		gNodeCount = 0;
		memset(&gNodes, 0, sizeof(gNodes));

		SmartTreeDestructSmartTree(&gTree);
		SmartTreeConstructSmartTree(&gTree, _compare, (size_t) 0);

		if (lIteration > lIterations)
		{
			break;
		}

		printf("Iteration : %ld ", lIteration);

		lStartTime = time(NULL);

		for (lNodeIndex = 0; lNodeIndex < TEST_NODES; lNodeIndex++)
		{
			lKeyPointer = NULL;
			lDataPointer = NULL;

			SmartTreeConstructNode(gTree, &gNodes[lNodeIndex], &lKeyPointer, sizeof(long), &lDataPointer, (size_t) DATA_ELEMENT_SIZE);

			*lKeyPointer = (rand() + rand() - RAND_MAX) % 99999;
			sprintf(lDataPointer, "Entry #%06d", lNodeIndex + 1);

			SmartTreeInsertNode(gTree, gNodes[lNodeIndex]);

			gNodeCount++;

			if (0 == lNodeIndex % (TEST_NODES / 10))
			{
				switch ((lNodeIndex / (TEST_NODES / 10)) % 4)
				{
				case 0:
					printf("-\b");
					break;
				case 1:
					printf("\\\b");
					break;
				case 2:
					printf("|\b");
					break;
				case 3:
					printf("/\b");
					break;
				}
			}
		}

		lInsertSeconds += difftime(time(NULL), lStartTime);

		printf(">");

		lStartTime = time(NULL);

		for (lNodeIndex = 0; lNodeIndex < TEST_NODES * 10; lNodeIndex++)
		{
			lKey = (rand() + rand() - RAND_MAX) % 99999;

			SmartTreeGetEqualNode(gTree, &lKey);

			if (0 == lNodeIndex % (TEST_NODES / 100))
			{
				switch ((lNodeIndex / (TEST_NODES / 100)) % 4)
				{
				case 0:
					printf(".  \b\b\b");
					break;
				case 1:
					printf(" . \b\b\b");
					break;
				case 2:
					printf("  .\b\b\b");
					break;
				case 3:
					printf(" . \b\b\b");
					break;
				}
			}
		}

		lSelectSeconds += difftime(time(NULL), lStartTime);

		printf(":");

		lStartTime = time(NULL);

		for (lNodeIndex = 0; lNodeIndex < TEST_NODES; lNodeIndex++)
		{
			SmartTreeDeleteNode(gTree, gNodes[lNodeIndex]);
			SmartTreeDestructNode(gTree, &gNodes[lNodeIndex]);

			gNodeCount--;

			if (0 == lNodeIndex % (TEST_NODES / 10))
			{
				switch ((lNodeIndex / (TEST_NODES / 10)) % 4)
				{
				case 0:
					printf("-\b");
					break;
				case 1:
					printf("/\b");
					break;
				case 2:
					printf("|\b");
					break;
				case 3:
					printf("\\\b");
					break;
				}
			}
		}

		lDeleteSeconds += difftime(time(NULL), lStartTime);

		printf("<");

		printf("\r");
	}

	lTotalSeconds = lInsertSeconds + lSelectSeconds + lDeleteSeconds;

	printf("\n\n");

	printf("Insert Timer: %07.3f%% %06.0f secs %9.0f nodes/sec\n", 100.0 * lInsertSeconds/lTotalSeconds, lInsertSeconds, ((double) (lIterations*TEST_NODES))/lInsertSeconds);
	printf("Select Timer: %07.3f%% %06.0f secs %9.0f nodes/sec\n", 100.0 * lSelectSeconds/lTotalSeconds, lSelectSeconds, ((double) (lIterations*TEST_NODES*10))/lSelectSeconds);
	printf("Delete Timer: %07.3f%% %06.0f secs %9.0f nodes/sec\n", 100.0 * lDeleteSeconds/lTotalSeconds, lDeleteSeconds, ((double) (lIterations*TEST_NODES))/lDeleteSeconds);
	printf("       -----------------------------------------------\n");
	printf("       Total: %7.3f%% %06.0f secs\n\n", 100.0, lTotalSeconds);
}

long _compare
(
    const smartTreeKeyHandle pKey1,
    const smartTreeKeyHandle pKey2
)
{
  return(* (long *) pKey1 - * (long *) pKey2);
}
