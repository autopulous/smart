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
  Smart Stack internal header file
  ----------------------------------------------------------------------------*/

#ifndef SMART_STACK_H
#define SMART_STACK_H

/*----------------------------------------------------------------------------
  Private defines
  ----------------------------------------------------------------------------*/

/* none */

/*----------------------------------------------------------------------------
  Private data types
  ----------------------------------------------------------------------------*/

typedef void * smartStackDataHandle;

typedef struct smartStackNode {
	struct smartStackNode * below;

	smartStackDataHandle data;
	size_t dataSize;
} smartStackNode;

typedef smartStackNode * smartStackNodeHandle;

typedef struct smartStack { 
	smartStackNodeHandle top;
	smartStackNodeHandle bottom;

	size_t memoryMaximum;
	size_t memoryAllocated;

	unsigned long depth;
} smartStack ;

typedef smartStack * smartStackHandle;

/*----------------------------------------------------------------------------
  Private function prototypes
  ----------------------------------------------------------------------------*/

/* none */

#endif
