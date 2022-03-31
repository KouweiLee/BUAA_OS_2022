#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

LEAF(_my_getchar)

loop_begin:
	lui s0, 0xb000
	lb s1, 0x00(s0)
	beq s1,zero,loop_begin
	nop

sb s1, 0x00(s0)
move v0, s1
jr ra
nop
END(_my_getchar)

LEAF(_my_putchar)
	lui s0, 0xb000
	sb a0, 0x00(s0)
	jr ra
	nop
END(_my_putchar)

LEAF(_my_exit)
lui s0, 0xb000
lb s1, 0x10(s0)
	jr ra
	nop
END(_my_exit)
