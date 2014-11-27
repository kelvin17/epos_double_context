.global __uint_div_precision

.text
__uint_div_precision:
xr8=0
xr11=0
xr12=0x00000001F
.code_align 4
if xr8==r1 b _uint_div_precision2
xr9=1
xr3=-32
xr5=r1 pos1
xr4=r0 pos1
_uint_div_precision1:
.code_align 4
if xr5>r4 b _uint_div_precision9
xr2=r5-r4
xr6=r0 lshift r2
xr7=r6 pos1
.code_align 4
if xr7==r12 b  _uint_div_precision10
_uint_div_precision11:
.code_align 4
if xr6>=r1 b _uint_div_precision0
_uint_div_precision12:
xr8=r8 lshift r9
xr4=r4-r9
.code_align 4
b _uint_div_precision1

_uint_div_precision10:
.code_align 4
if xr5==r12 b  _uint_div_precision11
xr1=r11-r1
.code_align 4
if xr1>=r6 b _uint_div_precision0
.code_align 4
b _uint_div_precision12

_uint_div_precision0:
xr8=r8 lshift r9
xr8=r8+r9
xr6=r6-r1
xr7=r0 rot r2
xr2=r3-r2
xr7=r7 lshift r2
xr2=r4-r5
xr6=r6 lshift r2
xr0=r6|r7
xr4=r4-r9
.code_align 4
b _uint_div_precision1

_uint_div_precision9:
.code_align 4
ret

_uint_div_precision2:
xr8=0xffffffff
.code_align 4
ret

