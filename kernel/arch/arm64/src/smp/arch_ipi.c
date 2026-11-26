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
 * @brief IPI related functions.
 */

#include <arch/smp/arch_ipi.h>
#include <arch/interrupt.h>
#include <arch/machine_func.h>
#include <ddi/irq.h>

#ifdef CONFIG_SMP

static irq_t tlb_shootdown_irq;

/** A TLB shootdown IPI handler. */
static void tlb_shootdown_recv(irq_t *irq)
{
	irq_spinlock_unlock(&irq->lock, false);

	/* Call genarch handler. */
	tlb_shootdown_ipi_recv();

	irq_spinlock_lock(&irq->lock, false);
}

/** Claim an IPI interrupt. */
static irq_ownership_t ipi_claim(irq_t *irq)
{
	(void) irq;
	return IRQ_ACCEPT;
}

/** Initialize IPIs. */
void ipi_init(void)
{
	/* Register the interrupt. */
	irq_initialize(&tlb_shootdown_irq);
	tlb_shootdown_irq.preack = true; /* Send EOI as soon as possible. */
	tlb_shootdown_irq.inr = VECTOR_TLB_SHOOTDOWN_IPI;
	tlb_shootdown_irq.claim = ipi_claim;
	tlb_shootdown_irq.handler = tlb_shootdown_recv;
	irq_register(&tlb_shootdown_irq);
}

/** Initialize IPI for a processing element. */
void ipi_pe_init(void)
{
	/** Enable IPI IRQ. */
	machine_pe_enable_ipi_irq(VECTOR_TLB_SHOOTDOWN_IPI);
}

/* Send TLB shootdown to all other cores. */
void ipi_send_tlb_shootdown_all(void)
{
	/**
	 * TLB shootdown IPIs are EOIed before the handler reads the messages, so
	 * if there is already a pending shootdown, the target will read the
	 * message anyway. Therefore, there is no need to wait for the previous
	 * shootdown to complete before sending another one.
	 */

	machine_send_ipi_all(VECTOR_TLB_SHOOTDOWN_IPI);
}

#endif /* CONFIG_SMP */

/** @}
 */
