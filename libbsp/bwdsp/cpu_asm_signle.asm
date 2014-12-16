.text

.ref ___interrupt_table
.ref ___Int_Thread_Dispatch
.ref _current_stack_base

.global __bwdsp_disable_interrupt
__bwdsp_disable_interrupt:
   xr8 = CGCR   
   clr CGCR[0]
.code_align 4
   ret

.global __bwdsp_enable_interrupt
__bwdsp_enable_interrupt:
   //xr0 = gcsr    
   set CGCR[0]
   //[u0+0,0] = xr0
.code_align 4
   ret

/********************************************************/
.global __bwdsp_set_interrupt
.global __bwdsp_clr_interrupt
__bwdsp_set_interrupt:
   set CGCR[0]
   set CGCR[1]
.code_align 4
   ret
   
__bwdsp_clr_interrupt:
   clr CGCR[0]
   clr CGCR[1]
.code_align 4
   ret
	
/********************************************************/
.global __bwdsp_get_gcsr
__bwdsp_get_gcsr:
   xr8 = CGCR
.code_align 4
   ret
	
/*****************************************************/
.global __init_swi
__init_swi:
   rtrap 9  
   //set IMASKRL [6]         // 打开中断
   set IMASKRH [29]
   set CGCR [0]   
   set CGCR [1]   
.code_align 4
   ret
   
/** ust to initialize counter **/
.global __init_counter
__init_counter:
   rtrap 9  
   ITIHR0_E = _ISR_TIMER_H
   //set IMASKRL [6]         // 打开中断
   set IMASKRH [21]
   set CGCR [0]                 
   TLPR = 0xff         //counter 计数周期
   TCR = 0x80000008
.code_align 4
   ret
   
_ISR_TIMER_H:
   nop
.code_align 4
   ret

/** use to timer counter **/
.global __counter_asm
__counter_asm:
   xr0 = TLCNT
   xr1 = THCNT
   [u0+0,0]=xr0
   [u0+1,0]=xr1

.code_align 4
   ret


/*****************************************************/
.global __init_timer0_int
__init_timer0_int:
  xr1 = 32
.code_align 4
  If xr1 > r0 b __init_timer_int_1
  xr5 = IMASKRH
  xr0 -= 32
  xr6 = 1
  xr7 = r6 lshift r0
  xr8 = r5 mset r7
  IMASKRH = xr8
.code_align 4
  b __init_timer_int_0
__init_timer_int_1:
  xr5 = IMASKRL
  xr6 = 1
  xr7 = r6 lshift r0
  xr8 = r5 mset r7
  IMASKRL = xr8
__init_timer_int_0:
  rtrap 9
//   set CGCR [0]
//   set CGCR [1]  //使能中断嵌套
  TLPR = 0x5000 || TCR = 0x80000008
.code_align 4
  ret


/*****************************************************/
.global __clean_timer_int
__clean_timer_int:
  xr1 = 32
.code_align 4
  If xr1 > r0 b __clean_timer_int_1
  xr5 = IMASKRH
  xr0 -= 32
  xr6 = 1
  xr7 = r6 lshift r0
  xr8 = r5 mclr r7
  IMASKRH = xr8
  b __clean_timer_int_0
__clean_timer_int_1:
  xr5 = IMASKRL
  xr6 = 1
  xr7 = r6 lshift r0
  xr8 = r5 mclr r7
  IMASKRL = xr8
__clean_timer_int_0:
.code_align 4
  ret

  

/********************************/
.global ___CPU_Context_switch
___CPU_Context_switch:
   strap 3 
.code_align 4   	
   ret 

/********************************/
.global ___CPU_Context_restore_self
___CPU_Context_restore_self:
   xr8 = u0	//u1=_Thread_Heir->registers(context_control *)
   xr8 += -1    //为了和单纯的中断路径统一
   u8 = xr8     //U8=_Thread_Heir->registers - 1;

_exitme:
   //load current_stack_base of _Thread_Heir
  u8 = u8 + 1   
  xr0 = [u8+0,0]  
  xr1 = _current_stack_base
  v0 = xr1
  [v0+0,0] = xr0

  // pop all interrupt return address
  xr2:1 = [u8 + 1, 0] || u8 = u8 + 2
  xr4:3 = [u8 + 1, 0] || u8 = u8 + 2
  xr6:5 = [u8 + 1, 0] || u8 = u8 + 2
  xr8:7 = [u8 + 1, 0] || u8 = u8 + 2
  xr10:9 = [u8 + 1, 0] || u8 = u8 + 2
  xr12:11= [u8 + 1, 0] || u8 = u8 + 2
  xr14:13= [u8 + 1, 0] || u8 = u8 + 2
  xr16:15= [u8 + 1, 0] || u8 = u8 + 2
  xr18:17= [u8 + 1, 0] || u8 = u8 + 2
  xr20:19= [u8 + 1, 0] || u8 = u8 + 2
  xr22:21= [u8 + 1, 0] || u8 = u8 + 2
  xr24:23= [u8 + 1, 0] || u8 = u8 + 2
  xr26:25= [u8 + 1, 0] || u8 = u8 + 2
  xr28:27= [u8 + 1, 0] || u8 = u8 + 2
  xr30:29= [u8 + 1, 0] || u8 = u8 + 2
  xr32:31= [u8 + 1, 0] || u8 = u8 + 2
  xr34:33= [u8 + 1, 0] || u8 = u8 + 2
  xr36:35= [u8 + 1, 0] || u8 = u8 + 2
  xr38:37= [u8 + 1, 0] || u8 = u8 + 2
  xr40:39= [u8 + 1, 0] || u8 = u8 + 2
  xr42:41= [u8 + 1, 0] || u8 = u8 + 2
  xr44:43= [u8 + 1, 0] || u8 = u8 + 2
  SWIR_R = xr44
  JIHR_R = xr43
  HIHR_R = xr42
  HINTR_R = xr41
  ITIHR0_R = xr40
  ITIHR1_R = xr39
  TIHR0_R = xr38
  TIHR1_R = xr37
  TIHR2_R = xr36
  TIHR3_R = xr35
  INTR0_R = xr34
  INTR1_R = xr33
  INTR2_R = xr32
  INTR3_R = xr31
  INTR4_R = xr30
  INTR5_R = xr29
  INTR6_R = xr28
  INTR7_R = xr27
  IINT_R  = xr26
  CDMA0_R = xr25
  CDMA1_R = xr24
  CDMA2_R = xr23
  CDMA3_R = xr22
  CDMA4_R = xr21
  CDMA5_R = xr20
  CDMA6_R = xr19
  CDMA7_R = xr18
  CDMA8_R = xr17
  CDMA9_R = xr16
  CDMA10_R = xr15
  IODMA0_R = xr14
  IODMA1_R = xr13
  IODMA2_R = xr12
  IODMA3_R = xr11
  SRIR_R = xr10
  STIR_R = xr9
  ITILR0_R = xr8
  ITILR1_R = xr7
  TILR0_R = xr6
  TILR1_R = xr5
  TILR2_R = xr4
  TILR3_R = xr3
  HILR_R = xr2
  JILR_R = xr1
		
  xr1 = [u8 + 1,0] || u8 = u8 + 1  // pop SER
  SER = xr1

  xr1 = [u8+1, 0] || u8 = u8+1  //pop ba
  ba = xr1

  xr1:0 = [u8 + 1, 0] || u8 = u8 + 2     	//pop  u9
  sr = xr1 || u9 = xr0

  u8 = u8+1 //让u8指向栈顶，而非下一位
  //恢复u10-u15
  xyztr1:0 = [u8 += 8, 1]
  u10 = xr0 || u11 = xr1 || u12 = yr0 || u13 = yr1 || u14 = zr0 || u15 = zr1

  //恢复IMAFR
  IMAFR = tr0
  //双栈改造开始

  //恢复ALUCR,ABFPR,BA,SR, BA,SR,IMACR,SER,LC

  xyztr1:0 = [u8 += 8, 1] <bb>
  xyztr1:0 = [u8 += 8, 1]

  xyztALUCR = r0
  xyztABFPR = r1
  IMACR = tr1 <bb> ||  LC3 = yr1 <bb> || LC2 = xr1 <bb>
  LC1 = tr0 <bb> || LC0 = zr0 <bb> || SR = yr0 <bb> || BA = xr0 <bb>

  //恢复ACC ALUFR
  xyztr7:6 = [u8 += 8, 1] <bb>
  xyztr7:6 = [u8 += 8, 1]
  xyztr5:4 = [u8 += 8, 1] <bb>
  xyztr5:4 = [u8 += 8, 1]
  xyztr3:2 = [u8 += 8, 1] <bb>
  xyztr3:2 = [u8 += 8, 1]
  xyztr1:0 = [u8 += 8, 1] <bb>
  xyztr1:0 = [u8 += 8, 1]

  xyztACCm6 = r6 || xyztACCm7 = r7 || xyztACCm5 = r5 || xyztACCm4 = r4
  xyztACCm3 = r3 || xyztACCm2 = r2 || xyztACCm1 = r1 || xyztACCm0 = r0
  xyztACCh6 = r6 <bb> || xyztACCh7 = r7 <bb> || xyztACCh5 = r5 <bb> || xyztACCh4 = r4 <bb>
  xyztACCh3 = r3 <bb> || xyztACCh2 = r2 <bb> || xyztACCh1 = r1 <bb> || xyztACCh0 = r0 <bb>

  xyztr7:6 = [u8 += 8, 1] <bb>
  xyztr7:6 = [u8 += 8, 1]
  xyztr5:4 = [u8 += 8, 1] <bb>
  xyztr5:4 = [u8 += 8, 1]
  xyztr3:2 = [u8 += 8, 1] <bb>
  xyztr3:2 = [u8 += 8, 1]
  xyztr1:0 = [u8 += 8, 1] <bb>
  xyztr1:0 = [u8 += 8, 1]

  xyztACCl6 = r6 <bb> || xyztACCl7 = r7 <bb> || xyztACCl5 = r5 <bb> || xyztACCl4 = r4 <bb>
  xyztACCl3 = r3 <bb> || xyztACCl2 = r2 <bb> || xyztACCl1 = r1 <bb> || xyztACCl0 = r0 <bb>

  xyztALUFR7 = r7 || xyztALUFR6 = r6 || xyztALUFR5 = r5 || xyztALUFR4 = r4
  xyztALUFR3 = r3 || xyztALUFR2 = r2 || xyztALUFR1 = r1 || xyztALUFR0 = r0

  //恢复 ACF CON
  xyztr7:6 = [u8 += 8, 1] <bb>
  xyztr7:6 = [u8 += 8, 1]
  xyztr5:4 = [u8 += 8, 1] <bb>
  xyztr5:4 = [u8 += 8, 1]
  xyztr3:2 = [u8 += 8, 1] <bb>
  xyztr3:2 = [u8 += 8, 1]
  xyztr1:0 = [u8 += 8, 1] <bb>
  xyztr1:0 = [u8 += 8, 1]

  xyztACF7 = r7 <bb> || xyztACF6 = r6 <bb> || xyztACF5 = r5 <bb> || xyztACF4 = r4 <bb>
  xyztACF3 = r3 <bb> || xyztACF2 = r2 <bb> || xyztACF1 = r1 <bb> || xyztACF0 = r0 <bb>

  xyztCON7 = r7 || xyztCON6 = r6 || xyztCON5 = r5 || xyztCON4 = r4
  xyztCON3 = r3 || xyztCON2 = r2 || xyztCON1 = r1 || xyztCON0 = r0

  xyztr63:62 = [u8 += 8, 1] <bb>
  xyztr63:62 = [u8 += 8, 1]
  xyztr61:60 = [u8 += 8, 1] <bb>
  xyztr61:60 = [u8 += 8, 1]
  xyztr59:58 = [u8 += 8, 1] <bb>
  xyztr59:58 = [u8 += 8, 1]
  xyztr57:56 = [u8 += 8, 1] <bb>
  xyztr57:56 = [u8 += 8, 1]
  xyztr55:54 = [u8 += 8, 1] <bb>
  xyztr55:54 = [u8 += 8, 1]
  xyztr53:52 = [u8 += 8, 1] <bb>
  xyztr53:52 = [u8 += 8, 1]
  xyztr51:50 = [u8 += 8, 1] <bb>
  xyztr51:50 = [u8 += 8, 1]
  xyztr49:48 = [u8 += 8, 1] <bb>
  xyztr49:48 = [u8 += 8, 1]
  xyztr47:46 = [u8 += 8, 1] <bb>
  xyztr47:46 = [u8 += 8, 1]
  xyztr45:44 = [u8 += 8, 1] <bb>
  xyztr45:44 = [u8 += 8, 1]
  xyztr43:42 = [u8 += 8, 1] <bb>
  xyztr43:42 = [u8 += 8, 1]
  xyztr41:40 = [u8 += 8, 1] <bb>
  xyztr41:40 = [u8 += 8, 1]
  xyztr39:38 = [u8 += 8, 1] <bb>
  xyztr39:38 = [u8 += 8, 1]
  xyztr37:36 = [u8 += 8, 1] <bb>
  xyztr37:36 = [u8 += 8, 1]
  xyztr35:34 = [u8 += 8, 1] <bb>
  xyztr35:34 = [u8 += 8, 1]
  xyztr33:32 = [u8 += 8, 1] <bb>
  xyztr33:32 = [u8 += 8, 1]
  xyztr31:30 = [u8 += 8, 1] <bb>
  xyztr31:30 = [u8 += 8, 1]
  xyztr29:28 = [u8 += 8, 1] <bb>
  xyztr29:28 = [u8 += 8, 1]
  xyztr27:26 = [u8 += 8, 1] <bb>
  xyztr27:26 = [u8 += 8, 1]
  xyztr25:24 = [u8 += 8, 1] <bb>
  xyztr25:24 = [u8 += 8, 1]
  xyztr23:22 = [u8 += 8, 1] <bb>
  xyztr23:22 = [u8 += 8, 1]
  xyztr21:20 = [u8 += 8, 1] <bb>
  xyztr21:20 = [u8 += 8, 1]
  xyztr19:18 = [u8 += 8, 1] <bb>
  xyztr19:18 = [u8 += 8, 1]
  xyztr17:16 = [u8 += 8, 1] <bb>
  xyztr17:16 = [u8 += 8, 1]
  xyztr15:14 = [u8 += 8, 1] <bb>
  xyztr15:14 = [u8 += 8, 1]
  xyztr13:12 = [u8 += 8, 1] <bb>
  xyztr13:12 = [u8 += 8, 1]
  xyztr11:10 = [u8 += 8, 1] <bb>
  xyztr11:10 = [u8 += 8, 1]
  xyztr9:8 = [u8 += 8, 1] <bb>
  xyztr9:8 = [u8 += 8, 1]
  xyztr7:6 = [u8 += 8, 1] <bb>
  xyztr7:6 = [u8 += 8, 1]
  xyztr5:4 = [u8 += 8, 1] <bb>
  xyztr5:4 = [u8 += 8, 1]
  xyztr3:2 = [u8 += 8, 1] <bb>
  xyztr3:2 = [u8 += 8, 1]
  xyztr1:0 = [u8 += 8, 1] <bb>
  xyztr1:0 = [u8 += 8, 1]

  //ALUCAFR
  xyztALUCAFR0 = r60 || xyztALUCAFR1 = r61 || xyztALUCAFR2 = r62 || xyztALUCAFR3 = r63
  xyztALUCAFR4 = r60 <bb> || xyztALUCAFR5 = r61 <bb> || xyztALUCAFR6 = r62 <bb> || xyztALUCAFR7 = r63 <bb>

  //ALUDFER,ALUDFMR,ALUDFIRR
  xyztALUDFER0 = r52 <bb> || xyztALUDFER1 = r53 <bb> || xyztALUDFER2 = r54 <bb> || xyztALUDFER3 = r55 <bb>
  xyztALUDFER4 = r56 <bb> || xyztALUDFER5 = r57 <bb> || xyztALUDFER6 = r58 <bb> || xyztALUDFER7 = r59 <bb>
  xyztALUDFMRl0 = r44 <bb> || xyztALUDFMRl1 = r45 <bb> || xyztALUDFMRl2 = r46 <bb> || xyztALUDFMRl3 = r47 <bb>
  xyztALUDFMRl4 = r48 <bb> || xyztALUDFMRl5 = r49 <bb> || xyztALUDFMRl6 = r50 <bb> || xyztALUDFMRl7 = r51 <bb>
  xyztALUDFMRh0 = r52 || xyztALUDFMRh1 = r53 || xyztALUDFMRh2 = r54 || xyztALUDFMRh3 = r55
  xyztALUDFMRh4 = r56 || xyztALUDFMRh5 = r57 || xyztALUDFMRh6 = r58 || xyztALUDFMRh7 = r59
  xyztALUDFIRRl0 = r36 <bb> ||  xyztALUDFIRRl1 = r37 <bb> || xyztALUDFIRRl2 = r38 <bb> || xyztALUDFIRRl3 = r39 <bb>
  xyztALUDFIRRl4 = r40 <bb> ||  xyztALUDFIRRl5 = r41 <bb> || xyztALUDFIRRl6 = r42 <bb> || xyztALUDFIRRl7 = r43 <bb>
  xyztALUDFIRRh0 = r44 ||  xyztALUDFIRRh1 = r45 || xyztALUDFIRRh2 = r46 || xyztALUDFIRRh3 = r47
  xyztALUDFIRRh4 = r48 ||  xyztALUDFIRRh5 = r49 || xyztALUDFIRRh6 = r50 || xyztALUDFIRRh7 = r51

  //SACC
  xyztSACC0 = r36:r37 || xyztSACC1 = r38:r39 || xyztSACC2 = r40:r41 || xyztSACC3 = r42:r43
  xyztSHFER0 = r34 || xyztSHFER1 = r34 <bb> || xyztSHFER2 = r35 || xyztSHFER3 = r35 <bb>
  xyztSCF0 = r32 || xyztSCF1 = r32 <bb> || xyztSCF2 = r33 || xyztSCF3 = r33 <bb>
  xyztSLTAR0 = r30 || xyztSLTAR1 = r30 <bb> || xyztSLTAR2 = r31 || xyztSLTAR3 = r31 <bb>
  xyztSHFFR0 = r28 || xyztSHFFR1 = r28 <bb>
  xyztSHFFR2 = r29 || xyztSHFFR0 = r29 <bb>
  xyztSHFCR = r27
  xyztCPRED = r27 <bb>
  xyztSHFCUTR = r26
  xyztGPLYR32 = r26 <bb>

  xyztSPUFR = r25 || xyztSPUCR = r25 <bb>

  xyztMULFR0 = r21 || xyztMULFR1 = r21 <bb>
  xyztMULFR2 = r22 || xyztMULFR3 = r22 <bb>
  xyztMULFR4 = r23 || xyztMULFR5 = r23 <bb>
  xyztMULFR6 = r24 || xyztMULFR7 = r24 <bb>
  xyztMULCUTR = r20
  xyztMULCR = r20 <bb>
  xyztMACC0 = r4(0,p) || xyztMACC1 = r5(0,p) || xyztMACC2 = r6(0,p) || xyztMACC3 = r7(0,p)   //MACC[31:0]
  xyztMACC4 = r12(0,p) || xyztMACC5 = r13(0,p) || xyztMACC6 = r14(0,p) || xyztMACC7 = r15(0,p)
  xyztMACC0 = r4(32,p) <bb> || xyztMACC1 = r5(32,p) <bb> || xyztMACC2 = r6(32,p) <bb> || xyztMACC3 = r7(32,p) <bb> //MACC[63:32]
  xyztMACC4 = r12(32,p) <bb> || xyztMACC5 = r13(32,p) <bb> || xyztMACC6 = r14(32,p) <bb> || xyztMACC7 = r15(32,p) <bb>
  xyztHOMACC0 = r8(0,p) || xyztHOMACC1 = r9(0,p) || xyztHOMACC2 = r10(0,p) || xyztHOMACC3 = r11(0,p)  //MACC[95:64]
  xyztHOMACC4 = r16(0,p) || xyztHOMACC5 = r17(0,p) || xyztHOMACC6 = r18(0,p) || xyztHOMACC7 = r19(0,p)
  xyztHOMACC0 = r8(32,p) <bb> || xyztHOMACC1 = r9(32,p) <bb> || xyztHOMACC2 = r10(32,p) <bb> || xyztHOMACC3 = r11(32,p) <bb> //MACC[127:96]
  xyztHOMACC4 = r16(32,p) <bb> || xyztHOMACC5 = r17(32,p) <bb> || xyztHOMACC6 = r18(32,p) <bb> || xyztHOMACC7 = r19(32,p) <bb>

  u0 = xr2 <bb> || u1 = xr3 <bb> || u2 = yr2 <bb> || u3 = yr3 <bb> || u4 = zr2 <bb> || u5 = zr3 <bb> || u6 = tr2 <bb> || u7 = tr3 <bb>
  w0 = xr0 <bb> || w1 = xr1 <bb> || w2 = yr0 <bb> || w3 = yr1 <bb> || w4 = zr0 <bb> || w5 = zr1 <bb> || w6 = tr0 <bb> || w7 = tr1 <bb>
  w8 = xr2 || w9 = xr3 || w10 = yr2 || w11 = yr3 || w12 = zr2 || w13 = zr3 || w14 = tr2 || w15 = tr3
  v8 = xr0 || v9 = xr1 || v10 = yr0 || v11 = yr1 || v12 = zr0 || v13 = zr1 || v14 = tr0 || v15 = tr1

  xyztr63:62 = [u8 += 8, 1] <bb>
  xyztr63:62 = [u8 += 8, 1]
  xyztr61:60 = [u8 += 8, 1] <bb>
  xyztr61:60 = [u8 += 8, 1]
  xyztr59:58 = [u8 += 8, 1] <bb>
  xyztr59:58 = [u8 += 8, 1]
  xyztr57:56 = [u8 += 8, 1] <bb>
  xyztr57:56 = [u8 += 8, 1]
  xyztr55:54 = [u8 += 8, 1] <bb>
  xyztr55:54 = [u8 += 8, 1]
  xyztr53:52 = [u8 += 8, 1] <bb>
  xyztr53:52 = [u8 += 8, 1]
  xyztr51:50 = [u8 += 8, 1] <bb>
  xyztr51:50 = [u8 += 8, 1]
  xyztr49:48 = [u8 += 8, 1] <bb>
  xyztr49:48 = [u8 += 8, 1]
  xyztr47:46 = [u8 += 8, 1] <bb>
  xyztr47:46 = [u8 += 8, 1]
  xyztr45:44 = [u8 += 8, 1] <bb>
  xyztr45:44 = [u8 += 8, 1]
  xyztr43:42 = [u8 += 8, 1] <bb>
  xyztr43:42 = [u8 += 8, 1]
  xyztr41:40 = [u8 += 8, 1] <bb>
  xyztr41:40 = [u8 += 8, 1]
  xyztr39:38 = [u8 += 8, 1] <bb>
  xyztr39:38 = [u8 += 8, 1]
  xyztr37:36 = [u8 += 8, 1] <bb>
  xyztr37:36 = [u8 += 8, 1]
  xyztr35:34 = [u8 += 8, 1] <bb>
  xyztr35:34 = [u8 += 8, 1]
  xyztr33:32 = [u8 += 8, 1] <bb>
  xyztr33:32 = [u8 += 8, 1]
  xyztr31:30 = [u8 += 8, 1] <bb>
  xyztr31:30 = [u8 += 8, 1]
  xyztr29:28 = [u8 += 8, 1] <bb>
  xyztr29:28 = [u8 += 8, 1]
  xyztr27:26 = [u8 += 8, 1] <bb>
  xyztr27:26 = [u8 += 8, 1]
  xyztr25:24 = [u8 += 8, 1] <bb>
  xyztr25:24 = [u8 += 8, 1]
  xyztr23:22 = [u8 += 8, 1] <bb>
  xyztr23:22 = [u8 += 8, 1]
  xyztr21:20 = [u8 += 8, 1] <bb>
  xyztr21:20 = [u8 += 8, 1]
  xyztr19:18 = [u8 += 8, 1] <bb>
  xyztr19:18 = [u8 += 8, 1]
  xyztr17:16 = [u8 += 8, 1] <bb>
  xyztr17:16 = [u8 += 8, 1]
  xyztr15:14 = [u8 += 8, 1] <bb>
  xyztr15:14 = [u8 += 8, 1]
  xyztr13:12 = [u8 += 8, 1] <bb>
  xyztr13:12 = [u8 += 8, 1]
  xyztr11:10 = [u8 += 8, 1] <bb>
  xyztr11:10 = [u8 += 8, 1]
  xyztr9:8 = [u8 += 8, 1] <bb>
  xyztr9:8 = [u8 += 8, 1]
  xyztr7:6 = [u8 += 8, 1] <bb>
  xyztr7:6 = [u8 += 8, 1]
  xyztr5:4 = [u8 += 8, 1] <bb>
  xyztr5:4 = [u8 += 8, 1]
  xyztr3:2 = [u8 += 8, 1] <bb>
  xyztr3:2 = [u8 += 8, 1]
  xyztr1:0 = [u8 += 8, 1] <bb>
  xyztr1:0 = [u8 += 8, 1]

  v0 = xr0 || v1 = xr1 || v2 = yr0 || v3 = yr1 || v4 = zr0 || v5 = zr1 || v6 = tr0 || v7 = tr1

  xyztr1:0 = [u8 += 7,1]

  u8 = u8 + 0xC //跳过中断入口保存的那些值+return address。

.code_align 4
   ret      //通过ser返回

/****OSIntCtxSw****/
__OSIntCtxSw:
.code_align 4
   ret

.end
