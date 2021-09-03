/*----------------------------------------------------------------------------
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
  Platform specific compilation macros header file.
  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Note:
  
  Alter this file to suit your build environment.
  ----------------------------------------------------------------------------*/

#ifndef COMPILATION_T_H
#define COMPILATION_T_H

/*----------------------------------------------------------------------------
  Platform specific storage class and calling convention
  ----------------------------------------------------------------------------*/

#if defined _WIN32 || defined _WIN64

/* Place STORAGE_CLASS before the type of a public function. */

#define STORAGE_CLASS __declspec(dllexport)

/* Place CALLING_CONVENTION before the name of a public function. */

#define CALLING_CONVENTION __stdcall

#else

/* Place STORAGE_CLASS before the type of a public function. */

#define STORAGE_CLASS

/* Place CALLING_CONVENTION before the name of a public function. */

#define CALLING_CONVENTION

#endif

#endif