#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

/* Exercise 2.10 */
LEAF(tlb_out)
//1: j 1b
nop
	mfc0	k1,CP0_ENTRYHI
	mtc0	a0,CP0_ENTRYHI// a0 is key, moved to EntryHi
	nop
	tlbp// according to key in EntryHi, set Index to the number of tlb entry
	nop
	nop
	nop
	nop
	mfc0	k0,CP0_INDEX
	bltz	k0,NOFOUND
	nop
	mtc0	zero,CP0_ENTRYHI
	mtc0	zero,CP0_ENTRYLO0
	nop
	tlbwi// set 0 to both this tlb entry's data and key
NOFOUND:

	mtc0	k1,CP0_ENTRYHI// restore the value of EntryHi
	
	j	ra
	nop
END(tlb_out)
