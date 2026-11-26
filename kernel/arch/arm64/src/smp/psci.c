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
 * @brief PSCI related functions.
 */

#include <arch/smp/psci.h>

#ifdef CONFIG_SMP

#define PSCI_TARGET_CPU_AFF012_MASK (UINT64_C(0xFFFFFF))
#define PSCI_TARGET_CPU_AFF3_SHIFT  (32)

extern uint64_t psci_cpu_on_asm(uint64_t aff, uint64_t entry_point,
    uint64_t context_id);

/**
 * Send the PSCI CPU_ON command to the secure firmware to wake up a core.
 *
 * @param aff Affinity of the target core.
 * @param entry_point Entry point address for the target core.
 * @param context_id Context ID for the target core that will end up in the x0
 * register of the core.
 * @return PSCI return code.
 */
psci_return_t psci_cpu_on(affinity_t aff, uintptr_t entry_point,
    uint64_t context_id)
{
	uint64_t target_cpu;
	uint64_t entry_point_address;
	uint64_t result;

	target_cpu = ((uint64_t)aff.val) & PSCI_TARGET_CPU_AFF012_MASK;
	target_cpu |= (((uint64_t)aff.aff3) << PSCI_TARGET_CPU_AFF3_SHIFT);
	entry_point_address = (uint64_t)entry_point;

	result = psci_cpu_on_asm(target_cpu, entry_point_address, context_id);
	return (psci_return_t)result;
}

#endif

/** @}
 */
