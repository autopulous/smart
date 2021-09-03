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
  Smart Stack test program header file
  ----------------------------------------------------------------------------*/

#ifndef SMART_STACK_TEST_H
#define SMART_STACK_TEST_H

#define TEST_NODES 100000

#define DATA_ELEMENT_SIZE 64

/*----------------------------------------------------------------------------
  Private function prototypes
  ----------------------------------------------------------------------------*/

void DisplayOptions
(
    void
);

void ConstructNodes
(
    FILE * pFile
);

void ConstructNode
(
	smartStackNodeHandle * pNode
);

void DestructNode
(
	smartStackNodeHandle * pNode
);

void AddNode
(
    FILE * pFile
);

void SubtractNode
(
    FILE * pFile
);

void PeekTopNode
(
    FILE * pFile,
	smartStackNodeHandle * pNode
);

void PeekNextNode
(
    FILE * pFile,
	smartStackNodeHandle * pNode
);

void PushNode
(
	smartStackNodeHandle pNode
);

void PopNode
(
	smartStackNodeHandle * pNode
);

void GetTopNode
(
	smartStackNodeHandle * pNode
);

void GetNextNode
(
	smartStackNodeHandle * pNode
);

void DestructNodes
(
    FILE * pFile
);

void OutputStackInformation
(
    FILE * pFile
);

void OutputStackArray
(
    FILE * pFile
);

void OutputStackContent
(
    FILE * pFile
);

void OutputNodeContent
(
	FILE * pFile,
	smartStackNodeHandle pNode
);

void IteratedTest1
(
	void
);

void IteratedTest2
(
	void
);

void IteratedTest3
(
	void
);

void ValidateStack
(
	FILE * pFile
);

#endif
