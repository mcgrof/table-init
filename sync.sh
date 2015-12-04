#!/bin/bash

cp ~/devel/table-init/include/linux/tables.h include/linux/tables.h
cp ~/devel/table-init/arch/x86/include/asm/x86_init_fn.h arch/x86/include/asm/x86_init_fn.h
cp ~/devel/table-init/arch/x86/kernel/init.c  arch/x86/kernel/init.c

git add \
	include/linux/tables.h \
	arch/x86/include/asm/x86_init_fn.h \
	arch/x86/kernel/init.c

