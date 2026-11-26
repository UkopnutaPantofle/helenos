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

/** @addtogroup kernel_arm64_smp
 * @{
 */
/** @file
 * @brief PSCI related declarations.
 */

#ifndef KERN_arm64_PSCI_H_
#define KERN_arm64_PSCI_H_

#ifdef CONFIG_SMP
#ifndef __ASSEMBLER__

#include <typedefs.h>
#include <arch/smp/aff.h>

#define PSCI_CPU_ON_FUNC_ID 0xC4000003U

typedef enum {
	PSCI_SUCCESS = 0,
	PSCI_NOT_SUPPORTED = -1,
	PSCI_INVALID_PARAMETERS = -2,
	PSCI_DENIED = -3,
	PSCI_ALREADY_ON = -4,
	PSCI_ON_PENDING = -5,
	PSCI_INTERNAL_FAILURE = -6,
	PSCI_NOT_PRESENT = -7,
	PSCI_DISABLED = -8,
	PSCI_INVALID_ADDRESS = -9,
} psci_return_t;

extern psci_return_t psci_cpu_on(affinity_t aff, uintptr_t entry_point,
    uint64_t context_id);

#else /* __ASSEMBLER__ */

#define PSCI_CPU_ON_FUNC_ID 0xC4000003

#endif /* __ASSEMBLER__ */
#endif /* CONFIG_SMP */
#endif /* KERN_arm64_PSCI_H_ */

/** @}
 */
