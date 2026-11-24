#include "list_interupt.h"

void	div_0(void)
{
	KERNEL_PANIC(" DIVISION PAR 0 EN RING 0");
}

void	range_exceed(void)
{
	KERNEL_PANIC(" BOUND RANGE EXCEED");
}

void	invalid_instruction(void)
{
	KERNEL_PANIC(" INSTRUCTION INVALIDE");
}

void	double_fault(void)
{
	KERNEL_PANIC(" DOUBLE FAULT");
}

void	invalid_tss(void)
{
	KERNEL_PANIC(" TSS INVALIDE");
}

void	stack_fault(void)
{
	KERNEL_PANIC(" STACK SEGMENT FAULT");
}

void	protection_fault(void)
{
	KERNEL_PANIC(" GENERAL PROTECTION FAULT");
}

void	page_fault(void)
{
	KERNEL_PANIC(" PAGE FAULT");
}

void	alignement_check(void)
{
	KERNEL_PANIC(" ALIGNEMENT CHECK");
}

void	control_protection(void)
{
	KERNEL_PANIC(" CONTROL PROTECTION EXCEPTION");
}