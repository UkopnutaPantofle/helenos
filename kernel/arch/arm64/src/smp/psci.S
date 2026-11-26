/*
 * Copyright (c) 2026 Jakub Kodym
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <arch/smp/psci.h>
#include <abi/asmtool.h>

#ifdef CONFIG_SMP

.text

/*
 * PSCI CPU ON function.
 *
 * x0: affinity
 * x1: entry point
 * x2: context ID
 */
FUNCTION_BEGIN(psci_cpu_on_asm)

/* The context ID. */
mov x3, x2
/* The AP PE entry point. */
mov x2, x1
/* The selected PE's affinity. */
mov x1, x0
/* Function ID for CPU_ON. */
ldr x0, =PSCI_CPU_ON_FUNC_ID

dsb ish
isb

/* Secure Monitor Call */
/* The instruction guarantees preservation of all registers. */
smc 0

ret
FUNCTION_END(psci_cpu_on_asm)

#endif
