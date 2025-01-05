/*
* experiment and porting tencent/libco to windows using wepoll.
* modifications by bbqz007.

* Copyright (C) 2025, bbqz007, github.com/bbqz007. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/

#define ZPort
.globl _zport_coctx_swap
#ifndef ZPort
#else
.def	_zport_coctx_swap;	.scl	2;	.type 32;	.endef
#endif // ZPort
_zport_coctx_swap:
// ?? __fastcall (ecx, edx)
#if defined(__i386__)
    movl %ecx, %eax
   movl %esp, %ecx
    movl %esp,  28(%eax)
    movl %ebp, 24(%eax)
    movl %esi, 20(%eax)
    movl %edi, 16(%eax)
    leal 16(%eax), %esp
   pushl %fs:0x8
   pushl %fs:0x4
    pushl %ebx      // 4(%eax)
    pushl %fs:0x0   // (%eax)
   movl %ecx, %esp

  movl %edx, %eax
    leal (%eax), %esp
    popl %fs:0x0
    popl %ebx
    popl %fs:0x4
    popl %fs:0x8
    movl 16(%eax), %edi
    movl 20(%eax), %esi
    movl 24(%eax), %ebp
    movl 28(%eax), %esp

	ret

#elif defined(__x86_64__)
	// not implemnt
	ret
#endif
