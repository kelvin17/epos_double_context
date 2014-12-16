.text
.global _interruptf_process
.global ___interrupt_table
_interruptf_process:

	tr0 = sr 
	tr1 = ser	
	[u8 + -1, 0] = tr1:0 || v0 = ___interrupt_table || v1 = xr0
	u8 = u8 + -2
	xr1 = [v0+v1, 0]
	sr = xr1 || u8 = u8 + -1        // reserve space for parameter of the ISR (added in 20110331)

	call sr             // note: xr0 is the input parameter of ISR
	u8 = u8 + 1
	
	xr1:0 = [u8 + 1, 0] || u8 = u8 + 2
	sr = xr0 || ser = xr1			
||	xr1:0 = [u8 + 1, 0] || u8 = u8 + 2	// pop v0 v1
	v0 = xr0 || v1 = xr1 
|| 	xyztr1:0 = [u8 + 1, 1] || u8 = u8 + 8 	// pop xyztr1:0
	
	//-------------remove these for release version-------------
	rtrap 30

	//----------------------------------------------------------

.code_align 4
	reti
