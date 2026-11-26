/*
 * Copyright (c) 2016 Petr Pavlu
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

/** @addtogroup kernel_arm64
 * @{
 */
/** @file
 * @brief Declarations of machine specific functions.
 *
 * These functions enable to differentiate more kinds of ARM platforms.
 */

#ifndef KERN_arm64_MACHINE_FUNC_H_
#define KERN_arm64_MACHINE_FUNC_H_

#include <arch/istate.h>
#include <typedefs.h>
#ifdef CONFIG_SMP
#include <arch/smp/aff.h>

struct arm_smp_machine_ops;
#endif /* CONFIG_SMP */

struct arm_machine_ops {
	void (*machine_init)(void);
	void (*machine_irq_exception)(unsigned int, istate_t *);
	void (*machine_output_init)(void);
	void (*machine_input_init)(void);
	inr_t (*machine_enable_vtimer_irq)(void);
	size_t (*machine_get_irq_count)(void);
	const char *(*machine_get_platform_name)(void);
	void (*machine_early_uart_output)(char32_t);
#ifdef CONFIG_SMP
	struct arm_smp_machine_ops *smp_ops;
#endif /* CONFIG_SMP */
};

#ifdef CONFIG_SMP
struct arm_smp_machine_ops {
	affinity_t *(*machine_get_pe_affinities)(void);
	unsigned int (*machine_get_pe_count)(void);
	void (*machine_ap_init)(void);
	inr_t (*machine_ap_enable_vtimer_irq)(void);
	void (*machine_pe_enable_ipi_irq)(int);
	void (*machine_send_ipi_all)(int);
};
#endif /* CONFIG_SMP */

extern void machine_ops_init(void);
extern void machine_init(void);
void machine_irq_exception(unsigned int, istate_t *);
extern void machine_output_init(void);
extern void machine_input_init(void);
extern inr_t machine_enable_vtimer_irq(void);
extern size_t machine_get_irq_count(void);
extern const char *machine_get_platform_name(void);
extern void machine_early_uart_output(char32_t);

#ifdef CONFIG_SMP
extern affinity_t *machine_get_pe_affinities(void);
extern unsigned int machine_get_pe_count(void);
extern void machine_ap_init(void);
extern inr_t machine_ap_enable_vtimer_irq(void);
extern void machine_pe_enable_ipi_irq(int);
extern void machine_send_ipi_all(int);
#endif /* CONFIG_SMP */

#endif

/** @}
 */
