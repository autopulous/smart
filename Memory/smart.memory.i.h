/*----------------------------------------------------------------------------
  Smart Memory
 
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
  Smart Memory application programmer's interface (API) header file
  ----------------------------------------------------------------------------*/

#ifndef SMART_MEMORY_I_H
#define SMART_MEMORY_I_H

/*----------------------------------------------------------------------------
  SafeMalloc()
  ----------------------------------------------------------------------------
  Allocates a block of memory.
  ----------------------------------------------------------------------------
  Parameters:
  
  pBuffer     - (I/O) The address of a memory pointer to hold the result of
                      the Malloc()
  pSize       - (I)   The number of bytes to allocate.
  ----------------------------------------------------------------------------
  Return Values:

  True  - Memory was succesfully allocated

  False - Memory was not successfully allocated due to one of the following:

          1. The buffer pointer pointer was NULL.
          2. The buffer pointer pointed to be the buffer pointer pointer was
             not initialized to NULL.
          3. Zero or fewer bytes were requested to be allocated.
          4. Alloc() failed.
  ----------------------------------------------------------------------------
  Notes:

  This function requires the contents of the pBuffer pointer to be initialized
  to NULL prior to calling this function because this function assumes that 
  a pointer with value represents an unfreed memory block and therefore
  overwritting this pointer with a new value would orphan the previously
  allocated memory block.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SafeMalloc
(
    void ** pBuffer,
    size_t  pSize
);

/*----------------------------------------------------------------------------
  SafeCalloc()
  ----------------------------------------------------------------------------
  Allocates and initializes a block of memory to zeros.
  ----------------------------------------------------------------------------
  Parameters:
  
  pBuffer     - (I/O) The address of a memory pointer to hold the result of
                      the Calloc()
  pSize       - (I)   The number of bytes to allocate.
  ----------------------------------------------------------------------------
  Return Values:

  True  - Memory was succesfully allocated

  False - Memory was not successfully allocated due to one of the following:

          1. The buffer pointer pointer was NULL.
          2. The buffer pointer pointed to be the buffer pointer pointer was
             not initialized to NULL.
          3. Zero or fewer bytes were requested to be allocated.
          4. Alloc() failed.
  ----------------------------------------------------------------------------
  Notes:

  This function requires the contents of the pBuffer pointer to be initialized
  to NULL prior to calling this function because this function assumes that 
  a pointer with value represents an unfreed memory block and therefore
  overwritting this pointer with a new value would orphan the previously
  allocated memory block.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SafeCalloc
(
    void ** pBuffer,
    size_t  pSize
);

/*----------------------------------------------------------------------------
  SafeRealloc()
  ----------------------------------------------------------------------------
  Reallocates a block of memory leaving the current contents unchanged.
  ----------------------------------------------------------------------------
  Parameters:
  
  pBuffer     - (I/O) The address of a memory pointer to hold the result of
                      the Realloc()
  pSize       - (I)   The number of bytes to allocate.
  ----------------------------------------------------------------------------
  Return Values:

  True  - Memory was succesfully allocated

  False - Memory was not successfully allocated due to one of the following:

          1. The buffer pointer pointer was NULL.
          2. Zero or fewer bytes were requested to be allocated.
          3. Realloc() failed.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SafeRealloc
(
    void ** pBuffer,
    size_t  pSize
);

/*----------------------------------------------------------------------------
  SafeFree()
  ----------------------------------------------------------------------------
  Deallocates a block of memory, setting the memory pointer to NULL.
  ----------------------------------------------------------------------------
  Parameters:
  
  pBuffer     - (I/O) The address of a memory pointer to the memory block
                      being deallocated by Free().
  ----------------------------------------------------------------------------
  Return Values:

  True  - Memory was succesfully deallocated

  False - Memory was not successfully deallocated due to one of the following:

          1. The buffer pointer pointer was NULL.
          2. The buffer pointer pointed to be the buffer pointer pointer was
             not initialized to NULL.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SafeFree
(
    void ** pBuffer
);

/*----------------------------------------------------------------------------
  SmartMalloc()
  ----------------------------------------------------------------------------
  Allocates a block of memory and then adds the size value to a memory
  management variable. This function is designed for use with a cache or other
  data structure where the total number of bytes in use needs to be managed.
  ----------------------------------------------------------------------------
  Parameters:
  
  pBuffer     - (I/O) The address of a memory pointer to hold the result of
                      the SafeMalloc()
  pSize       - (I)   The number of bytes to allocate.
  pMemoryUsed - (I/O) A pointer to a memory management variable.
  ----------------------------------------------------------------------------
  Return Values:

  True  - Memory was succesfully allocated

  False - Memory was not successfully allocated due to one of the following:

          1. The memory management variable pointer was NULL.
          2. Zero or fewer bytes were requested to be allocated.
          3. SafeAlloc() failed.
  ----------------------------------------------------------------------------
  Notes:

  Because this function uses SafeMalloc() it requires the contents of the
  pBuffer pointer to be initialized to NULL prior to calling this function
  because the SafeAlloc function assumes that a pointer with value
  represents an unfreed memory block and therefore overwritting this pointer
  with a new value would orphan the previously allocated memory block.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartMalloc
(
    void **  pBuffer,
    size_t   pSize,
    size_t * pMemoryUsed
);

/*----------------------------------------------------------------------------
  SmartCalloc()
  ----------------------------------------------------------------------------
  Allocates and initializes a block of memory to zeros and then adds the size
  value to a memory management variable. This function is designed for use
  with a cache or other data structure where the total number of bytes in use
  needs to be managed.
  ----------------------------------------------------------------------------
  Parameters:
  
  pBuffer     - (I/O) The address of a memory pointer to hold the result of
                      the SafeCalloc()
  pSize       - (I)   The number of bytes to allocate.
  pMemoryUsed - (I/O) A pointer to a memory management variable.
  ----------------------------------------------------------------------------
  Return Values:

  True  - Memory was succesfully allocated

  False - Memory was not successfully allocated due to one of the following:

          1. The memory management variable pointer was NULL.
          2. Zero or fewer bytes were requested to be allocated.
          3. SafeAlloc() failed.
  ----------------------------------------------------------------------------
  Notes:

  Because this function uses SafeCalloc() it requires the contents of the
  pBuffer pointer to be initialized to NULL prior to calling this function
  because the SafeAlloc function assumes that a pointer with value
  represents an unfreed memory block and therefore overwritting this pointer
  with a new value would orphan the previously allocated memory block.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartCalloc
(
    void **  pBuffer,
    size_t   pSize,
    size_t * pMemoryUsed
);

/*----------------------------------------------------------------------------
  SmartFree()
  ----------------------------------------------------------------------------
  Deallocates a block of memory, setting the memory pointer to NULL and then
  subtracts the passed size value from the memory management variable. This
  function is designed for use with a cache or other data structure where the
  total number of bytes in use needs to be managed.
  ----------------------------------------------------------------------------
  Parameters:
  
  pBuffer     - (I/O) The address of a memory pointer to the memory block
                      being deallocated by SafeFree().
  pSize       - (I)   The number of bytes being deallocate.
  pMemoryUsed - (I/O) A pointer to a memory management variable.
  ----------------------------------------------------------------------------
  Return Values:

  True  - Memory was succesfully deallocated

  False - Memory was not successfully deallocated due to one of the following:

          1. The memory management variable pointer was NULL.
          2. Zero or fewer bytes were requested to be deallocated.
          3. SafeFree() failed.
  ----------------------------------------------------------------------------*/

STORAGE_CLASS Bool CALLING_CONVENTION SmartFree
(
    void **  pBuffer,
    size_t   pSize,
    size_t * pMemoryUsed
);

#endif
