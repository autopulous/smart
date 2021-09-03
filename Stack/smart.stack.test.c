/*----------------------------------------------------------------------------
  Smart Stack test application
 
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
  Smart Stack test program implementation file
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

#include "smart.stack.t.h"

/*----------------------------------------------------------------------------
  Public functions
  ----------------------------------------------------------------------------*/

#include "smart.stack.i.h"

/*----------------------------------------------------------------------------
  Private defines, data types and function prototypes
  ----------------------------------------------------------------------------*/

#include "smart.stack.test.h"

/*----------------------------------------------------------------------------
  <Eeek> Globals </Eeek>
  ----------------------------------------------------------------------------*/

smartStackHandle gStack;

smartStackNodeHandle gNodes[TEST_NODES];

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

	smartStackNodeHandle lNode = NULL;

	ConstructNodes(stdout);

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
				DestructNodes(stdout);
				break;
            }

            case 'X':
            {
				DestructNodes(stdout);
				ConstructNodes(stdout);
				lNode  = NULL;
				break;
            }

            case '+':
            {
				AddNode(stdout);
				PeekTopNode(stdout, &lNode);
				break;
            }
        
            case '-':
            {
				SubtractNode(stdout);
				PeekTopNode(stdout, &lNode);
                break;
            }

            case '=':
            {
				PeekTopNode(stdout, &lNode);
                break;
            }

            case '>':
            {
				PeekNextNode(stdout, &lNode);
                break;
            }

            case '1':
            {
				IteratedTest1();
				lNode  = NULL;
                break;
            }

			case '2':
            {
				IteratedTest2();
				lNode  = NULL;
                break;
            }

            case '3':
            {
				IteratedTest3();
				lNode  = NULL;
                break;
            }

			case '9':
			{
				OutputStackArray(stdout);
				break;
			}

			case '0':
			{
				OutputStackContent(stdout);
				lNode  = NULL;
				break;
			}

			case 'I':
            {
				OutputStackInformation(stdout);
                break;
            }

            default:
            {
                printf("Valid options are +,-,=,>,!,|,*,X,1,2,3,I,9,0,Q,?\n");
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
		   "(+) Push a node to the stack\n"
		   "(-) Pop the top stack node (removes node)\n"
		   "(=) Get the top stack node (preserves node, sets current node)\n"
		   "(>) Get the node beneath the current node (preserves node, sets current node)\n\n"
		   "(!) Split the stack at the current node\n"
		   "(|) Split the stack at its middle node\n"
		   "(*) Splice 2 stacks\n\n"
		   "(X) Remove all nodes\n\n"
		   "(1) Iterated artificial performance test\n"
		   "(2) Iterated realistic performance speed test\n"
		   "(3) Iterated thorough test\n\n"
		   "(I) Display stack information\n"
		   "(9) Display application stack array\n"
		   "(0) Display stack contents (pop all)\n\n"
		   "(Q) Quit\n"
		   "(?) Display this option list\n"
		   "\n");
}

void ConstructNodes
(
    FILE * pFile
)
{
    SmartStackConstructSmartStack(&gStack, (size_t) 0);

	memset(&gNodes, 0, sizeof(gNodes));

    gNodeCount = 0;

	ValidateStack(pFile);
}

void ConstructNode
(
	smartStackNodeHandle * pNode
)
{
	if (SmartStackConstructNode(gStack, pNode, NULL, (size_t) DATA_ELEMENT_SIZE))
	{
		gNodeCount++;
	}
}

void AddNode
(
    FILE * pFile
)
{
    smartStackNodeHandle lNode = gNodes[gNodeCount];

	ConstructNode(&lNode);

	gNodes[gNodeCount - 1] = lNode;

	PushNode(lNode);

	OutputNodeContent(pFile, lNode);
	fprintf(pFile, "\n");

	ValidateStack(stdout);
}

void SubtractNode
(
    FILE * pFile
)
{
    smartStackNodeHandle lNode;

	PopNode(&lNode);

	OutputNodeContent(pFile, lNode);
	fprintf(pFile, "\n");

	DestructNode(&lNode);

	gNodes[gNodeCount] = lNode;

	ValidateStack(stdout);
}

void PeekTopNode
(
    FILE * pFile,
	smartStackNodeHandle * pNode
)
{
	GetTopNode(pNode);

	OutputNodeContent(pFile, *pNode);
	fprintf(pFile, "\n");
}

void PeekNextNode
(
    FILE * pFile,
	smartStackNodeHandle * pNode
)
{
	GetNextNode(pNode);

	OutputNodeContent(pFile, *pNode);
	fprintf(pFile, "\n");
}

void PushNode
(
	smartStackNodeHandle pNode
)
{
	static unsigned long lCreationOrdinal = 1;

    char * lDataPointer = SmartStackGetNodeData(pNode);

	sprintf(lDataPointer, "Creation Ordinal:%08ld", lCreationOrdinal++);

	SmartStackPushNode(gStack, pNode);
}

void PopNode
(
	smartStackNodeHandle * pNode
)
{
	*pNode = SmartStackPopNode(gStack);
}

void GetTopNode
(
	smartStackNodeHandle * pNode
)
{
	*pNode = SmartStackGetTopNode(gStack);
}

void GetNextNode
(
	smartStackNodeHandle * pNode
)
{
	*pNode = SmartStackGetNextNode(gStack, *pNode);
}

void DestructNode
(
	smartStackNodeHandle * pNode
)
{
	if (SmartStackDestructNode(gStack, pNode))
	{
		gNodeCount--;
	}
}

void DestructNodes
(
    FILE * pFile
)
{
    SmartStackDestructSmartStack(&gStack);

	gNodeCount = 0;

	ValidateStack(pFile);
}

void OutputStackInformation
(
    FILE * pFile
)
{
	fprintf(pFile, "Stack Depth = %ld\n", SmartStackGetDepth(gStack));
	fprintf(pFile, "Memory Allocated = %ld\n\n", SmartStackGetMemoryAllocated(gStack));
}

void OutputStackArray
(
    FILE * pFile
)
{
	unsigned long lNodeIndex;

	fprintf(pFile, "\n");

	for (lNodeIndex = 0; lNodeIndex < gNodeCount; lNodeIndex++)
	{
		fprintf(pFile, "%06d ", lNodeIndex + 1);

		OutputNodeContent(pFile, gNodes[lNodeIndex]);
	}

	fprintf(pFile, "\n");
}

void OutputStackContent
(
    FILE * pFile
)
{
	smartStackNodeHandle lNode;

	unsigned long lNodeIndex;

	fprintf(pFile, "\n");

	for (lNodeIndex = 1; ; lNodeIndex++)
	{
		PopNode(&lNode);

		if (NULL == lNode)
		{
			fprintf(pFile, "\n");
			return;
		}

		fprintf(pFile, "%06d ", lNodeIndex);

		OutputNodeContent(pFile, lNode);
		
		DestructNode(&lNode);

		gNodes[gNodeCount] = lNode;
	}
}

void OutputNodeContent
(
	FILE * pFile,
	smartStackNodeHandle pNode
)
{
    char * lDataPointer = SmartStackGetNodeData(pNode);

	fprintf(pFile, "Data \"%s\"\n", lDataPointer);
}

void IteratedTest1
(
	void
)
{
	unsigned long lIterations;
	unsigned long lIteration;

    unsigned long lNodeIndex;

	char * lDataPointer;

	time_t lStartTime;
	double lConstructSeconds = 1, lPushSeconds = 1, lPopSeconds = 1, lDestructSeconds = 1, lTotalSeconds = 1;

	SmartStackDestructSmartStack(&gStack);
	SmartStackConstructSmartStack(&gStack, (size_t) 0);

	gNodeCount = 0;
	memset(&gNodes, 0, sizeof(gNodes));

	lStartTime = time(NULL);

	for (lNodeIndex = 0; lNodeIndex < TEST_NODES; lNodeIndex++)
	{
		lDataPointer = NULL;

		SmartStackConstructNode(gStack, &gNodes[lNodeIndex], &lDataPointer, (size_t) DATA_ELEMENT_SIZE);

		sprintf(lDataPointer, "Entry #%06d", lNodeIndex + 1);

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

	lConstructSeconds += difftime(time(NULL), lStartTime);

	printf("\n");
	printf("Iterations: ");
	scanf("%ld", &lIterations);

	for (lIteration = 1; ; lIteration++)
	{
		if (lIteration > lIterations)
		{
			break;
		}

		printf("Iteration : %ld ", lIteration);

		lStartTime = time(NULL);

		for (lNodeIndex = 0; lNodeIndex < TEST_NODES; lNodeIndex++)
		{
			SmartStackPushNode(gStack, gNodes[lNodeIndex]);

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

		lPushSeconds += difftime(time(NULL), lStartTime);

		printf(">");

		lStartTime = time(NULL);

		for (lNodeIndex = TEST_NODES - 1 ;; lNodeIndex--)
		{
			gNodes[lNodeIndex] = SmartStackPopNode(gStack);

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

			if (0 == lNodeIndex)
			{
				break;
			}
		}

		lPopSeconds += difftime(time(NULL), lStartTime);

		printf("<");

		printf("\r");
	}

	printf("\n");

	lStartTime = time(NULL);

	for (lNodeIndex = 0; lNodeIndex < TEST_NODES; lNodeIndex++)
	{
		SmartStackDestructNode(gStack, &gNodes[lNodeIndex]);

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

	lDestructSeconds += difftime(time(NULL), lStartTime);

	lTotalSeconds = lConstructSeconds + lPushSeconds + lPopSeconds + lDestructSeconds;

	printf("\n\n");

	printf("Construct Timer: %07.3f%% %06.0f secs %9.0f nodes/sec\n", 100.0 * lConstructSeconds/lTotalSeconds, lConstructSeconds, ((double) (TEST_NODES))/lConstructSeconds);
	printf("Push Timer:      %07.3f%% %06.0f secs %9.0f nodes/sec\n", 100.0 * lPushSeconds/lTotalSeconds, lPushSeconds, ((double) (lIterations*TEST_NODES))/lPushSeconds);
	printf("Pop Timer:       %07.3f%% %06.0f secs %9.0f nodes/sec\n", 100.0 * lPopSeconds/lTotalSeconds, lPopSeconds, ((double) (lIterations*TEST_NODES))/lPopSeconds);
	printf("Destruct Timer:  %07.3f%% %06.0f secs %9.0f nodes/sec\n", 100.0 * lDestructSeconds/lTotalSeconds, lDestructSeconds, ((double) (TEST_NODES))/lDestructSeconds);
	printf("                 -----------------------------------------------\n");
	printf("          Total: %7.3f%% %06.0f secs\n\n", 100.0, lTotalSeconds);
}

void IteratedTest2
(
	void
)
{
	unsigned long lIterations;
	unsigned long lIteration;

    unsigned long lNodeIndex;

	char * lDataPointer;

	time_t lStartTime;
	double lPushSeconds = 1, lPopSeconds = 1, lTotalSeconds = 1;

	printf("\n");
	printf("Iterations: ");
	scanf("%ld", &lIterations);

	for (lIteration = 1; ; lIteration++)
	{
		gNodeCount = 0;
		memset(&gNodes, 0, sizeof(gNodes));

		SmartStackDestructSmartStack(&gStack);
		SmartStackConstructSmartStack(&gStack, (size_t) 0);

		if (lIteration > lIterations)
		{
			break;
		}

		printf("Iteration : %ld ", lIteration);

		lStartTime = time(NULL);

		for (lNodeIndex = 0; lNodeIndex < TEST_NODES; lNodeIndex++)
		{
			lDataPointer = NULL;

			SmartStackConstructNode(gStack, &gNodes[lNodeIndex], &lDataPointer, (size_t) DATA_ELEMENT_SIZE);

			sprintf(lDataPointer, "Entry #%06d", lNodeIndex + 1);

			SmartStackPushNode(gStack, gNodes[lNodeIndex]);

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

		lPushSeconds += difftime(time(NULL), lStartTime);

		printf(">");

		lStartTime = time(NULL);

		for (lNodeIndex = TEST_NODES - 1 ;; lNodeIndex--)
		{
			gNodes[lNodeIndex] = SmartStackPopNode(gStack);
			SmartStackDestructNode(gStack, &gNodes[lNodeIndex]);

			gNodeCount--;

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

			if (0 == lNodeIndex)
			{
				break;
			}
		}

		lPopSeconds += difftime(time(NULL), lStartTime);

		printf("<");

		printf("\r");
	}

	lTotalSeconds = lPushSeconds + lPopSeconds;

	printf("\n\n");

	printf("Construct/Push Timer: %07.3f%% %06.0f secs %9.0f nodes/sec\n", 100.0 * lPushSeconds/lTotalSeconds, lPushSeconds, ((double) (lIterations*TEST_NODES))/lPushSeconds);
	printf("Destruct/Pop Timer:   %07.3f%% %06.0f secs %9.0f nodes/sec\n", 100.0 * lPopSeconds/lTotalSeconds, lPopSeconds, ((double) (lIterations*TEST_NODES))/lPopSeconds);
	printf("                      -----------------------------------------------\n");
	printf("               Total: %7.3f%% %06.0f secs\n\n", 100.0, lTotalSeconds);
}

void IteratedTest3
(
	void
)
{
	unsigned long lIterations;
	unsigned long lIteration;

    unsigned long lNodeIndex;

	char * lDataPointer;

	smartStackNodeHandle lNode;

	printf("\n");
	printf("Iterations: ");
	scanf("%ld", &lIterations);

	for (lIteration = 1; ; lIteration++)
	{
		gNodeCount = 0;
		memset(&gNodes, 0, sizeof(gNodes));

		SmartStackDestructSmartStack(&gStack);
		SmartStackConstructSmartStack(&gStack, (size_t) 0);

		ValidateStack(stdout);

		if (lIteration > lIterations)
		{
			break;
		}

		printf("Iteration : %ld ", lIteration);

		for (lNodeIndex = 0; lNodeIndex < TEST_NODES; lNodeIndex++)
		{
			lDataPointer = NULL;

			SmartStackConstructNode(gStack, &gNodes[lNodeIndex], &lDataPointer, (size_t) DATA_ELEMENT_SIZE);

			sprintf(lDataPointer, "Entry #%06d", lNodeIndex + 1);

			SmartStackPushNode(gStack, gNodes[lNodeIndex]);

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

		ValidateStack(stdout);

		printf(">");

		for (lNodeIndex = TEST_NODES - 1 ;; lNodeIndex--)
		{
			lNode = SmartStackPopNode(gStack);

			if (gNodes[lNodeIndex] != lNode)
			{
				fprintf(stdout, "Node pop disagreement: expected %p returned %p\n", gNodes[lNodeIndex], lNode);
			}

			SmartStackDestructNode(gStack, &gNodes[lNodeIndex]);

			gNodeCount--;

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

			if (0 == lNodeIndex)
			{
				break;
			}
		}

		ValidateStack(stdout);

		printf("<");

		printf("\r");
	}

	printf("\n");
}

void ValidateStack
(
	FILE * pFile
)
{
	unsigned long lNodeCount;

	if (!SmartStackIsValid(gStack))
	{
		fprintf(pFile, "Stack self validation failed.\n\n");
		return;
	}
	
	lNodeCount = SmartStackGetDepth(gStack);

	if (gNodeCount != lNodeCount)
	{
		fprintf(pFile, "Stack depth disagreement: Application = %d, API = %d\n\n", gNodeCount, lNodeCount);
	}
}
