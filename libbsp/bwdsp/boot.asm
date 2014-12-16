
.text
.ref ___finit
.ref ___minit
.ref __main
.ref __exit
//------------------------------------------------
// 饱和控制位清零，即不做饱和处理
clr xalucr[1] || clr yalucr[1] || clr zalucr[1] || clr talucr[1]
clr xmulcr[1] || clr ymulcr[1] || clr zmulcr[1] || clr tmulcr[1]
clr xshfcr[1] || clr yshfcr[1] || clr zshfcr[1] || clr tshfcr[1]
//-------------------GRui added-------------------
/*
// 中断向量初始化
SWIR = _empty_isr    || HINTR = _empty_isr   || TIHR0 = _empty_isr   || TIHR1 = _empty_isr
TIHR2 = _empty_isr   || TIHR3 = _empty_isr   || TIHR4 = _empty_isr   || INTR0 = _empty_isr
INTR1 = _empty_isr   || INTR2 = _empty_isr   || INTR3 = _empty_isr   || DMAIR0 = _empty_isr
DMAIR1 = _empty_isr  || DMAIR2 = _empty_isr  || DMAIR3 = _empty_isr  || DMAIR4 = _empty_isr
DMAIR5 = _empty_isr  || DMAIR6 = _empty_isr  || DMAIR7 = _empty_isr  || DMAIR8 = _empty_isr
DMAIR9 = _empty_isr  || DMAIR10 = _empty_isr || DMAIR11 = _empty_isr || DMAIR12 = _empty_isr
DMAIR13 = _empty_isr || DMAIR14 = _empty_isr || DMAIR15 = _empty_isr || DMAIR16 = _empty_isr
DMAIR17 = _empty_isr || SRIR = _empty_isr    || STIR = _empty_isr    || TILR0 = _empty_isr
TILR1 = _empty_isr   || TILR2 = _empty_isr   || TILR3 = _empty_isr   || TILR4 = _empty_isr
*/
//------------------------------------------------

// 栈帧指针初始化
u8 = 0x63fffe || u9 = 0x63fffe
// 堆初始化
// u0 = 0x610000 || xr1 = 0x10000
//.code_align 4
//call ___minit

.code_align 4
call ___finit

//wannghao
	xr63=u9
||	u9=u8+0
||	xr19=r0
	u7=u1+0
	[u8+0,0] = xr63
||	u8=u8+-18

	u7=_ARGC
||	u0=_ARGVS
||	u6=u9+0
	xr10=[u7+0,0]
||	xr18=u0
||	u5=u6+-8
	u6=u0+16
	[u5+0,0] = xr18
||	u5=u9+0
||	xr10=r10
	xr17=u6
	u7=u5+-8
||	xr0=r10
	u5=u0+32
	[u7+1,0] = xr17
||	u7=u9+0
	xr16=u5
	u6=u7+-8
	u7=u0+48
	[u6+2,0] = xr16
||	u6=u9+0
	xr15=u7
	u5=u6+-8
	u6=u0+64
	[u5+3,0] = xr15
||	u5=u9+0
	xr14=u6
	u7=u5+-8
	u5=u0+80
	[u7+4,0] = xr14
||	u7=u9+0
	xr13=u5
	u6=u7+-8
	u7=u0+96
	[u6+5,0] = xr13
||	u6=u9+0
	xr12=u7
||	u7=u0+112
	u5=u6+-8
	xr11=u7
	[u5+6,0] = xr12
||	u5=u9+0
	u6=u5+-8
	u5=u9+0
	[u6+7,0] = xr11
	u5=u5+-8
	u1=u5+0
	xr1=u1

//set CGCR[0] // 使能全局中断
//set CGCR[1] // 使能中断嵌套
//set CGCR[13] // 使能BPB（分支预测表）修改
clr CGCR[0] // 使能全局中断
clr CGCR[1] // 使能中断嵌套
set CGCR[13] // 使能BPB（分支预测表）修改

.code_align 4
call __main



__exit_loop:
rtrap 3 // 通知bwsim结束运行
.code_align 4
B  __exit_loop


//------------------------------------------------

.data
.global _current_stack_base
_current_stack_base: .word 0x63fffe
.global _context_B_base
_context_B_base: .word 0xA3fffe
.global _ARGC
_ARGC: .bes 4
.global _ARGVS
_ARGVS: .space 512
.end