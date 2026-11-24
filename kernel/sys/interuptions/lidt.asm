global load_lidt

load_lidt:
	; recuperer l adresse de 4 octet qui est sur la stack
	; vu que passé en argument
	mov eax, [esp + 4]
	lidt [eax]
	ret

