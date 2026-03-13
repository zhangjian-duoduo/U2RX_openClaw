/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include "mmu.h"

void mmu_setttbase(register rt_uint32_t i)
{
    register rt_uint32_t value;

    /* Invalidates all TLBs.Domain access is selected as
     * client by configuring domain access register,
     * in that case access controlled by permission value
     * set by page table entry
     */
    value = 0;
    asm volatile ("mcr p15, 0, %0, c8, c7, 0"::"r"(value));

    value = 0x55555555;
    asm volatile ("mcr p15, 0, %0, c3, c0, 0"::"r"(value));
    asm volatile ("mcr p15, 0, %0, c2, c0, 0"::"r"(i));
}

void mmu_set_domain(register rt_uint32_t i)
{
    asm volatile ("mcr p15,0, %0, c3, c0,  0": :"r" (i));
}

void mmu_enable(void)
{
    register rt_uint32_t i;

    /* read control register */
    asm volatile ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));

    i |= 0x1;
    /* Enables the extended page tables to be configured for
       the hardware page translation mechanism, Subpage AP bits disabled */
    i |= (1 << 23); /* support for ARMv6 MMU features */
    i |= (1 << 13); /* High exception vectors selected, address range = 0xFFFF0000-0xFFFF001C */

    /* write back to control register */
    asm volatile ("mcr p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_disable(void)
{
    register rt_uint32_t i;

    /* read control register */
    asm volatile ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));

    i &= ~0x1;

    /* write back to control register */
    asm volatile ("mcr p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_enable_icache(void)
{
    register rt_uint32_t i;

    /* read control register */
    asm volatile ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));

    i |= (1 << 12);

    /* write back to control register */
    asm volatile ("mcr p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_enable_branchpredictor(void)
{
    register rt_uint32_t i;

    /* read control register */
    asm ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));

    i |= (1 << 11);

    /* write back to control register */
    asm ("mcr p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_enable_dcache(void)
{
    register rt_uint32_t i;

    /* read control register */
    asm volatile ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));

    i |= (1 << 2);

    /* write back to control register */
    asm volatile ("mcr p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_disable_icache(void)
{
    register rt_uint32_t i;

    /* read control register */
    asm volatile ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));

    i &= ~(1 << 12);

    /* write back to control register */
    asm volatile ("mcr p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_disable_dcache(void)
{
    register rt_uint32_t i;

    /* read control register */
    asm volatile ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));

    i &= ~(1 << 2);

    /* write back to control register */
    asm volatile ("mcr p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_enable_alignfault(void)
{
    register rt_uint32_t i;

    /* read control register */
    asm volatile ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));

    i |= (1 << 1);

    /* write back to control register */
    asm volatile ("mcr p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_disable_alignfault(void)
{
    register rt_uint32_t i;

    /* read control register */
    asm volatile ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));

    i &= ~(1 << 1);

    /* write back to control register */
    asm volatile ("mcr p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_clean_invalidated_cache_index(int index)
{
    asm volatile ("mcr p15, 0, %0, c7, c14, 2": :"r" (index));
}

void mmu_clean_invalidated_dcache(rt_uint32_t buffer, rt_uint32_t size)
{
    unsigned int ptr;

    ptr = buffer & ~(CACHE_LINE_SIZE - 1);

    while (ptr < buffer + size)
    {
        asm volatile ("mcr p15, 0, %0, c7, c14, 1": :"r" (ptr));
        ptr += CACHE_LINE_SIZE;
    }
}


void mmu_clean_dcache(rt_uint32_t buffer, rt_uint32_t size)
{
    unsigned int ptr;

    ptr = buffer & ~(CACHE_LINE_SIZE - 1);

    while (ptr < buffer + size)
    {
        asm volatile ("mcr p15, 0, %0, c7, c10, 1": :"r" (ptr));
        ptr += CACHE_LINE_SIZE;
    }
}

void mmu_clean_dcache_all(void)
{
	unsigned int v = 0;
    asm volatile ("mcr p15, 0, %0, c7, c10, 0": :"r" (v));

}


void mmu_invalidate_dcache(rt_uint32_t buffer, rt_uint32_t size)
{
    unsigned int ptr;

    ptr = buffer & ~(CACHE_LINE_SIZE - 1);

    while (ptr < buffer + size)
    {
        asm volatile ("mcr p15, 0, %0, c7, c6, 1": :"r" (ptr));
        ptr += CACHE_LINE_SIZE;
    }
}

void mmu_invalidate_tlb(void)
{
    asm volatile ("mcr p15, 0, %0, c8, c7, 0": :"r" (0));
}

void mmu_invalidate_icache(void)
{
    asm volatile ("mcr p15, 0, %0, c7, c5, 0": :"r" (0));
}

void mmu_invalidate_dcache_all(void)
{
    asm volatile ("mcr p15, 0, %0, c7, c6, 0": :"r" (0));
}

/* level1 page table */
static volatile unsigned int _pgd_table[4*1024] ALIGN(16*1024);
/*
 * level2 page table
 * RT_MMU_PTE_SIZE must be 1024*n
 */
#define RT_MMU_PTE_SIZE        4096
static volatile unsigned int _pte_table[RT_MMU_PTE_SIZE] ALIGN(1*1024);

void mmu_create_pgd(struct mem_desc *mdesc)
{
    volatile rt_uint32_t *pTT;
    volatile int i, nSec;

    if (mdesc->vaddr_start & (0x100000 - 1))
    {
        while (1)
            ; /*uart not available now, so cann't print...*/
    }

    pTT = (rt_uint32_t *)_pgd_table + (mdesc->vaddr_start >> 20);
    nSec = (mdesc->vaddr_end >> 20) - (mdesc->vaddr_start >> 20);
    for (i = 0; i <= nSec; i++)
    {
        *pTT = mdesc->sect_attr | (((mdesc->paddr_start >> 20) + i) << 20);
        pTT++;
    }
}

int addr_is_cached(void *addr)
{
    if ((_pgd_table[(rt_uint32_t)addr >> 20] & 0xfffff) == SECT_RWX_CB)
    {
        return 1;
    }

    return 0;
}

int addr_all_cached(void *addr1, void *addr2)
{
    if (((_pgd_table[(rt_uint32_t)addr1 >> 20] & 0xfffff) == SECT_RWX_CB) &&
        ((_pgd_table[(rt_uint32_t)addr2 >> 20] & 0xfffff) == SECT_RWX_CB))
    {
        return 1;
    }

    return 0;
}

void mmu_create_pte(struct mem_desc *mdesc)
{
    volatile rt_uint32_t *pTT;
    volatile rt_uint32_t *p_pteentry;
    int i;
    rt_uint32_t vaddr;
    rt_uint32_t total_page = 0;
    rt_uint32_t pte_offset = 0;
    rt_uint32_t sect_attr = 0;

    total_page = (mdesc->vaddr_end >> 12) - (mdesc->vaddr_start >> 12) + 1;
    pte_offset = mdesc->sect_attr & 0xfffffc00;
    sect_attr = mdesc->sect_attr & 0x3ff;
    vaddr = mdesc->vaddr_start;

    for (i = 0; i < total_page; i++)
    {
        pTT = (rt_uint32_t *)_pgd_table + (vaddr >> 20);
        if (*pTT == 0) /* Level 1 page table item not used, now update pgd item */
        {
            *pTT = pte_offset | sect_attr;
            p_pteentry = (rt_uint32_t *)pte_offset +
                ((vaddr & 0x000ff000) >> 12);
            pte_offset += 1024;
        }
        else /* using old Level 1 page table item */
        {
            p_pteentry = (rt_uint32_t *)(*pTT & 0xfffffc00) +
                ((vaddr & 0x000ff000) >> 12);
        }


        *p_pteentry = mdesc->page_attr | (((mdesc->paddr_start >> 12) + i) << 12);
        vaddr += 0x1000;
    }
}

static void build_pte_mem_desc(struct mem_desc *mdesc, rt_uint32_t size)
{
    rt_uint32_t pte_offset = 0;
    rt_uint32_t nsec = 0;
    /* set page table */
    for (; size > 0; size--)
    {
        if (mdesc->mapped_mode == PAGE_MAPPED)
        {
            nsec = (RT_ALIGN(mdesc->vaddr_end, 0x100000) - RT_ALIGN_DOWN(mdesc->vaddr_start, 0x100000)) >> 20;
            mdesc->sect_attr |= (((rt_uint32_t)_pte_table)& 0xfffffc00) + pte_offset;
            pte_offset += nsec << 10;
        }
        if (pte_offset >= RT_MMU_PTE_SIZE)
        {
            rt_kprintf("PTE table size too little\n");
            RT_ASSERT(0);
        }

        mdesc++;
    }
}

void rt_hw_init_mmu_table(struct mem_desc *mdesc, unsigned int size)
{
    rt_memset((void *)_pgd_table, 0, 16*1024);
    rt_memset((void *)_pte_table, 0, RT_MMU_PTE_SIZE);
    build_pte_mem_desc(mdesc, size);
    for (; size > 0; size--)
    {
        if (mdesc->mapped_mode == SECT_MAPPED)
        {
            mmu_create_pgd(mdesc);
        }
        else
        {
            mmu_create_pte(mdesc);
        }
        mdesc++;
    }
}

/* void rt_hw_mmu_init(struct mem_desc *mdesc, unsigned int size)   */
void rt_hw_mmu_init(void)
{
    /* disable I/D cache */
    mmu_disable_dcache();
    mmu_disable_icache();
    mmu_disable();
    mmu_invalidate_tlb();

    /* set MMU table address */
    mmu_setttbase((rt_uint32_t)_pgd_table);

    /* enables MMU */
    mmu_enable();

    /* enable Instruction Cache */
    mmu_enable_icache();

    /* enable Data Cache */
    mmu_enable_dcache();

    mmu_enable_branchpredictor();

    mmu_invalidate_icache();
    mmu_invalidate_dcache_all();
}
void rt_hw_mmu_resume(void)
{



    mmu_setttbase((unsigned int)_pgd_table);

    mmu_enable();

    /* enable Instruction Cache */
    mmu_enable_icache();

    /* enable Data Cache */
    mmu_enable_dcache();

    mmu_enable_branchpredictor();

	mmu_disable_alignfault();

}
