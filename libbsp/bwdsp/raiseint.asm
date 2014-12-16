
.text
.global __raiseint

__raiseint:

	xr1 = 0x3FBFFF00 ||	xr2 = 0xFF00C07C
	xr3 = 1 ||	xr4 = 32
	xr5 = 0 || xr6 = 61
	xr8 = 0
	
  xr9 = 63
.code_align 4
	If xr0 > r9 (u) b _raiseint_error // ID of the IRQ can not larger than 63

.code_align 4
	if xr4 > r0  b _raiseint_low32    // if ID of the IRQ less than 32, jump to _raiseint_low32
	
	xr10 = r0
	xr10 -= 32
	xr3 = r3 lshift r10
	xr1 = r1 & r3
.code_align 4
	if xr1 == r5 b _raiseint_error     // compare with the mask, check whether the ID is valid
	
.code_align 4
	if xr0 != r6 b _raiseint_1         // Is software excp or not?
	strap 1
	//.code_align 4
	sr =_raiseint_exit
	call sr 
	
_raiseint_1:
	isrh = xr3
//.code_align 4
	sr = _raiseint_exit
	call sr

_raiseint_low32:
	xr3 = r3 lshift r0
	xr2 = r2 & r3
.code_align 4
	if xr2==r5 b _raiseint_error      // compare with the mask, check whether the ID is valid
	isrl = xr3
//.code_align 4
	sr = _raiseint_exit
	call sr
	
_raiseint_error:
	xr8 = 0xFFFFFFFF
	
_raiseint_exit:
.code_align 4
	ret
	
	
	