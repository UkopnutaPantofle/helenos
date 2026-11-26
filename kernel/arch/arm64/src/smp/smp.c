/*
 * Copyright (c) 2015 Petr Pavlu
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

/** @addtogroup kernel_arm64
 * @{
 */
/** @file
 */

#include <smp/smp.h>
#include <arch/smp/psci.h>
#include <arch/smp/aff.h>
#include <arch/smp/ap.h>
#include <arch/machine_func.h>
#include <cpu.h>
#include <log.h>
#include <errno.h>

#ifdef CONFIG_SMP

/* Initialize SMP. */
void smp_init(void)
{
	/* Set the number of CPU cores. */
	config.cpu_count = machine_get_pe_count();
}

/* Wake application processors up. */
void kmp(void *arg)
{
	affinity_t *affinities;
	affinity_t bsp_aff;
	size_t cpu_count;

	affinities = machine_get_pe_affinities();
	bsp_aff = pe_affinity();
	cpu_count = config.cpu_count;

	assert(affinities != NULL);

	/* Wake the cores up one by one. */
	for (size_t i = 0; i < cpu_count; i++) {
		/* Skip the bootstrap processor. */
		if (affinities[i].val == bsp_aff.val) {
			printf("CPU%" PRIu64 " with affinity %" PRIu8 ".%" PRIu8
			    ".%" PRIu8 ".%" PRIu8 " is up as the BSP.\n", i, bsp_aff.aff3,
			    bsp_aff.aff2, bsp_aff.aff1, bsp_aff.aff0);

			continue;
		}

		psci_return_t ret = psci_cpu_on(affinities[i], KA2PA(ap_boot), 0);

		if (ret == PSCI_SUCCESS) {
			if (semaphore_down_timeout(&ap_completion_semaphore,
			    100000000) != EOK) {
				log(LF_ARCH, LVL_NOTE, "%s: waiting for cpu%" PRIu64
				    " (AffinityValue" " = %" PRIu32 ") timed out", __func__,
				    i, affinities[i].val);
			}

			affinity_t aff = affinities[i];
			printf("CPU%" PRIu64 " with affinity %" PRIu8 ".%" PRIu8
			    ".%" PRIu8 ".%" PRIu8 " is up.\n", i, aff.aff3, aff.aff2,
			    aff.aff1, aff.aff0);
		} else {
			/* Log failed wakeup. */
			log(LF_ARCH, LVL_ERROR, "%s: failed to wake up cpu%" PRIu64
			    " (AffinityValue = %" PRIu32 "): "
			    "psci_cpu_on() returned %" PRIu32, __func__, i,
			    affinities[i].val, ret);
		}
	}
}

#endif /* CONFIG_SMP */

/** @}
 */
