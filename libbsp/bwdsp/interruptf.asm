.text
.ref _interruptf_process
.ref ___interrupt_table
.ref ___interrupts
.ref _conn_sys_isr
.global __isr_installf

__isr_installf:
  xr1 = 0x3FBFFF00 ||  xr2 = 0xFF00C07C
  xr3 = 1 ||  xr4 = 32
  xr5 = 0 ||  xr6 = 0
  xr7 = 0 ||  u6=xr0	
  xr8 = 0x1FFFF || xr9 = u1
.code_align 4
  If xr9 > r8 (u) b _interruptf_9  // the entrance of ISR can not larger than 0x1FFFF
	
  xr9 = 63
.code_align 4
  If xr0 > r9 (u) b _interruptf_9 // ID of the IRQ can not larger than 63

.code_align 4
  if xr4 > r0  b _interruptf_0    // if ID of the IRQ less than 32, jump to _interruptf_0

  xr0 -= 32
  xr3 = r3 lshift r0
  xr1 = r1 & r3  ||  xr6 = r3

.code_align 4
  if xr1==r5 b _interruptf_9      // compare with the mask, check whether the ID is invalid

//.code_align 4
  sr=_interruptf_1
  call sr

_interruptf_0:
  xr3 = r3 lshift r0
  xr2 = r2 & r3 ||	xr7 = r3
.code_align 4
  if xr2==r5 b _interruptf_9     // compare with the mask, check whether the ID is invalid

_interruptf_1:
  u5 = ___interrupt_table ||	u7 = ___interrupts ||  xr12 = _interruptf_process ||  xr10 = u1
  xr13 = IMASKRh
  xr14 = IMASKRl
  xr11 = [u5+u6, 0]                    // get current value in specific entry of ___interrupt_table, the value will be returned by U4
	
//-----------------------------------  GRui: added 20110407
  xr15 = 1 || xr16 = u1
.code_align 4
  if xr15 != r16 b _interruptf_10      // compare with SIG_IGN, which is 1
	
  xr6 = ! r6 || xr7 = ! r7
  xr13 = r13 & r6 ||  xr14 = r14 & r7  // xr6, xr7 is the bit mask of current IRQ
  IMASKRh = xr13  ||  IMASKRl = xr14   // set specific bit to 1 in IMASKR
	
//.code_align 4
  sr=_interruptf_11
  call sr || u4 = xr11
//-----------------------------------
	
_interruptf_10:
  xr13 = r13 | r6 ||  xr14 = r14 | r7  // xr6, xr7 is the bit mask of current IRQ
  IMASKRh = xr13  ||  IMASKRl = xr14   // set specific bit to 1 in IMASKR
  [u5+u6, 0] = xr10                    // write new value (entry address of new ISR) to specific entry of ___interrupt_table
  [u7+u6, 0] = xr12                    // write the value of _interruptf_process to table ___interrupts. _interruptf_process is defined in _interruptf.asm
  xr0 = u6  
  xr8 = ser	//error! cxl: 2014-7-18 for ECS V1.0.9.5
  [u8+=-1,0] = xr8
  
//.code_align 4
  sr=_conn_sys_isr
  call sr
  
  xr8 = [u8+1,0] || u8 = u8 + 1
  ser = xr8	//error! cxl: 2014-7-18 for ECS V1.0.9.5
  
//.code_align 4
  sr=_interruptf_11
  call sr  || u4 = xr11

_interruptf_9:
  u4 = 0xFFFFFFFF  // GRui modified: 2010-12-28
	
_interruptf_11:
.code_align 4
  ret
