#pragma once
#include "../../mem/mem.h"

void	div_0(void);
void	range_exceed(void);
void	invalid_instruction(void);
void	double_fault(void);
void	invalid_tss(void);
void	stack_fault(void);
void	protection_fault(void);
void	page_fault(void);
void	alignement_check(void);
void	control_protection(void);