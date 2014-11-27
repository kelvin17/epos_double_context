.global __int_div_precision

.text
__int_div_precision:
xr8=0
xr9=1
xr3=-32
xr13=0
xr11=0
xr12=0x00000001F
.code_align 4
if xr0>=r8 b _int_div_precision5
xr0=r8-r0
xr13=r13 ^ r9
_int_div_precision5:
.code_align 4
if xr1>=r8 b _int_div_precision6
xr1=r8-r1
xr13=r13 ^ r9
_int_div_precision6:
.code_align 4
if xr8==r1 b _int_div_precision2
xr5=r1 pos1
xr4=r0 pos1
_int_div_precision1:
.code_align 4
if xr5>r4 b _int_div_precision9
xr2=r5-r4
xr6=r0 lshift r2
xr7=r6 pos1
.code_align 4
if xr7==r12 b  _int_div_precision10
_int_div_precision11:
.code_align 4
if xr6>=r1 b _int_div_precision0
_int_div_precision12:
xr8=r8 lshift r9
xr4=r4-r9
.code_align 4
b _int_div_precision1

_int_div_precision10:
.code_align 4
if xr5==r12 b  _int_div_precision11
xr1=r11-r1
.code_align 4
if xr1>=r6 b _int_div_precision0
.code_align 4
b _int_div_precision12

_int_div_precision0:
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
b _int_div_precision1

_int_div_precision9:
.code_align 4
if xr9 > r13 b _int_div_precision3
xr9=0
xr8=r9-r8
_int_div_precision3:
.code_align 4
ret

_int_div_precision2:
xr8=0x80000000
.code_align 4
ret

