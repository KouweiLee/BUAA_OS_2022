/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>
/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
	if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
			ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
			ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
			ehdr->e_ident[EI_MAG3] == ELFMAG3) {
		return 1;
	}

	return 0;
}

/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

/*
   Exercise 1.2. Please complete func "readelf". 
 */
int readelf(u_char *binary, int size)
{
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

	int Nr;

	// check whether `binary` is a ELF file.
	if (size < 4 || !is_elf_format(binary)) {
		printf("not a standard elf format\n");
		return 0;
	}

	/*
	// get section table addr, section header number and section header size.
	ptr_sh_table = (ehdr -> e_shoff) + binary;
	sh_entry_count = ehdr -> e_shnum;
	sh_entry_size = ehdr -> e_shentsize;
	// for each section header, output section number and section addr. 
	// hint: section number starts at 0.
	for(Nr=0;Nr<sh_entry_count;Nr++){
		shdr = (Elf32_Shdr *) (ptr_sh_table + Nr * sh_entry_size);//section table address 
		printf("%d:0x%x\n",Nr,shdr -> sh_addr);
	}*/
	// labexam
	Elf32_Phdr * phdr1 = NULL;
	Elf32_Phdr * phdr2 = NULL;
	u_char *ptr_ph_table = binary + (ehdr->e_phoff);
	Elf32_Half count = ehdr->e_phnum;
	Elf32_Half psize = ehdr->e_phentsize;
	
	for(Nr=0;Nr<count-1;Nr++){
		phdr1 = (Elf32_Phdr *)(ptr_ph_table + Nr*psize);
		for(int i = Nr+1;i<count;i++){
			phdr2 = (Elf32_Phdr *)(ptr_ph_table + i*psize);
		Elf32_Addr l2 = phdr2->p_vaddr;
		Elf32_Addr r1 = phdr1->p_vaddr + phdr1->p_memsz - 1;
		if(r1 > l2){
			printf("Conflict at page va : 0x%x\n", (l2 & 0xfffff000));
			return 0;
		}
		if((r1 & 0xfffff000) == (l2 & 0xfffff000)){
			printf("Overlay at page va : 0x%x\n", (l2 & 0xfffff000));
			return 0;
		}
		}
	}
	for(Nr=0;Nr<count;Nr++){
		phdr1 = (Elf32_Phdr *)(ptr_ph_table + Nr*psize);
		printf("%d:0x%x,0x%x\n",Nr,phdr1->p_filesz, phdr1->p_memsz);
	}
	return 0;
}

