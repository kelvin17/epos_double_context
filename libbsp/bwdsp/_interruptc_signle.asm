/* 
 * 该文件定义的_interruptc_process类似于i386中的_ISR_handler [in irq_asm.S]
 * 如若有疑问，请参考该文件
 */
.text
.ref ___interrupt_table
.ref ___Int_Thread_Dispatch
.ref ___Thread_Dispatch

.global _interruptc_process
_interruptc_process:
  clr CGCR[0] // 关全局中断
  
  	xr1  = 53
	xr0 = v1
	if xr0 == r1 b __itihr0_r_exit 

	xr0 = SWIR_R  // 假设现在只有两种中断
	b _push_exit_r
		
__itihr0_r_exit:		
	xr0 = ITIHR0_R
		
_push_exit_r:
	[u8+0, 0] = xr0 || u8 = u8 + -1   // push return_address by qyq

  [u8+-7, 1] = xyztr1:0 || u8 = u8 + -8

  //开始算pad1的个数
  // push xyztr1:0
  xr0 = v0 || xr1 = v1
  || yr0 = v2 || yr1 = v3
  || zr0 = v4 || zr1 = v5
  || tr0 = v6 || tr1 = v7
  
  [u8+ -7, 1] = xyztr1:0 || u8 = u8 + -8


  //双栈改造开始
  u8 = u8 + -7
  
  [u8 += -8, 1] = xyztr1:0 <bb>  //入栈保存
  || xr0 = v8 || xr1 = v9        //v8-v15通过寄存器中转入栈
  || yr0 = v10 || yr1 = v11
  || zr0 = v12 || zr1 = v13
  || tr0 = v14 || tr1 = v15
  [u8 += -8, 1] = xyztr3:2
  
  [u8 += -8, 1] = xyztr3:2 <bb>
  || xr0 = w0 <bb> || xr1 = w1 <bb>
  || yr0 = w2 <bb> || yr1 = w3 <bb>
  || zr0 = w4 <bb> || zr1 = w5 <bb>
  || tr0 = w6 <bb> || tr1 = w7 <bb>
  [u8 += -8, 1] = xyztr5:4
  
  [u8 += -8, 1] = xyztr5:4 <bb>
  || xr2 = w8 || xr3 = w9
  || yr2 = w10 || yr3 = w11
  || zr2 = w12 || zr3 = w13
  || tr2 = w14 || tr3 = w15
  [u8 += -8, 1] = xyztr7:6
  
  [u8 += -8, 1] = xyztr7:6 <bb>
  || xr2 = u0 <bb> || xr3 = u1 <bb>
  || yr2 = u2 <bb> || yr3 = u3 <bb>
  || zr2 = u4 <bb> || zr3 = u5 <bb>
  || tr2 = u6 <bb> || tr3 = u7 <bb>
  [u8 += -8, 1] = xyztr9:8
  
  [u8 += -8, 1] = xyztr9:8 <bb>
  ||  r4 = llomacc0(u, cut=0)
  ||  r5 = llomacc1(u, cut=0)
  ||  r6 = llomacc2(u, cut=0)
  ||  r7 = llomacc3(u, cut=0)
  [u8 += -8, 1] = xyztr11:10
  
  [u8 += -8, 1] = xyztr11:10 <bb>
  ||  r4 = lhomacc0(u, cut=0) <bb>
  ||  r5 = lhomacc1(u, cut=0) <bb>
  ||  r6 = lhomacc2(u, cut=0) <bb>
  ||  r7 = lhomacc3(u, cut=0) <bb>
  [u8 += -8, 1] = xyztr13:12
  
  [u8 += -8,1] = xyztr13:12 <bb>
  ||  r8 = hlomacc0(u, cut=0)
  ||  r9 = hlomacc1(u, cut=0)
  ||  r10 = hlomacc2(u, cut=0)
  ||  r11 = hlomacc3(u, cut=0)
  [u8 += -8,1] = xyztr15:14
  
  [u8 += -8,1] = xyztr15:14 <bb>
  ||  r8 = hhomacc0(u, cut=0) <bb>
  ||  r9 = hhomacc1(u, cut=0) <bb>
  ||  r10 = hhomacc2(u, cut=0) <bb>
  ||  r11 = hhomacc3(u, cut=0) <bb>
  [u8 += -8,1] = xyztr17:16
  
  [u8 += -8,1] = xyztr17:16 <bb>
  ||  r12 = llomacc4(u, cut=0)
  ||  r13 = llomacc5(u, cut=0)
  ||  r14 = llomacc6(u, cut=0)
  ||  r15 = llomacc7(u, cut=0)
  [u8 += -8,1] = xyztr19:18

  [u8 += -8,1] = xyztr19:18 <bb>
  ||  r12 = lhomacc4(u, cut=0) <bb>
  ||  r13 = lhomacc5(u, cut=0) <bb>
  ||  r14 = lhomacc6(u, cut=0) <bb>
  ||  r15 = lhomacc7(u, cut=0) <bb>
  [u8 += -8,1] = xyztr21:20

  [u8 += -8,1] = xyztr21:20 <bb>
  ||  r16 = hlomacc4(u, cut=0)
  ||  r17 = hlomacc5(u, cut=0)
  ||  r18 = hlomacc6(u, cut=0)
  ||  r19 = hlomacc7(u, cut=0)
  [u8 += -8,1] = xyztr23:22

  [u8 += -8,1] = xyztr23:22 <bb>
  ||  r16 = hhomacc4(u, cut=0) <bb>
  ||  r17 = hhomacc5(u, cut=0) <bb>
  ||  r18 = hhomacc6(u, cut=0) <bb>
  ||  r19 = hhomacc7(u, cut=0) <bb>
  [u8 += -8,1] = xyztr25:24

  [u8 += -8,1] = xyztr25:24 <bb> || r20 = MULCUTR
  [u8 += -8,1] = xyztr27:26
  
  r20 = MULCR <bb>
  r21 = MULFR0  || r21 = MULFR1 <bb>
  || r22 = MULFR2  || r22 = MULFR3 <bb> 
  
  [u8 += -8, 1] = xyztr27:26 <bb>
  || r23 = MULFR4  || r23 = MULFR5 <bb>
  || r24 = MULFR6  || r24 = MULFR7 <bb> 
  || r25 = SPUFR   || r25 = SPUCR <bb>
  [u8 += -8, 1] = xyztr29:28
  
  [u8 += -8,1] = xyztr29:28 <bb> || r26 = SHFCUTR
  r26 = GPLYR32 <bb>
  r27 = SHFCR
  r27 = CPRED <bb>
  [u8 += -8,1] = xyztr31:30

  [u8 += -8, 1] = xyztr31:30 <bb>
  || r28 = SHFFR0  || r28 = SHFFR1 <bb>
  || r29 = SHFFR2  || r29 = SHFFR3 <bb>
  [u8 += -8, 1] = xyztr33:32
  
  [u8 += -8, 1] = xyztr33:32 <bb>
  || r30 = SLTAR0  || r30 = SLTAR1 <bb>
  || r31 = SLTAR2  || r31 = SLTAR3 <bb>
  [u8 += -8, 1] = xyztr35:34
  
  [u8 += -8, 1] = xyztr35:34 <bb>
  || r32 = SCF0 || r32 = SCF1 <bb>
  || r33 = SCF2 || r33 = SCF3 <bb>
  [u8 += -8, 1] = xyztr37:36

  [u8 += -8, 1] = xyztr37:36 <bb>
  || r34 = SHFER0 || r34 = SHFER1 <bb>
  || r35 = SHFER2 || r35 = SHFER3 <bb>
  
  [u8 += -8, 1] = xyztr39:38
  [u8 += -8, 1] = xyztr39:38 <bb>
  [u8 += -8, 1] = xyztr41:40
  [u8 += -8, 1] = xyztr41:40 <bb>
  [u8 += -8, 1] = xyztr43:42
  [u8 += -8, 1] = xyztr43:42 <bb>
  [u8 += -8, 1] = xyztr45:44
  [u8 += -8, 1] = xyztr45:44 <bb>
  [u8 += -8, 1] = xyztr47:46
  [u8 += -8, 1] = xyztr47:46 <bb>
  [u8 += -8, 1] = xyztr49:48
  [u8 += -8, 1] = xyztr49:48 <bb>
  [u8 += -8, 1] = xyztr51:50
  [u8 += -8, 1] = xyztr51:50 <bb>
  [u8 += -8, 1] = xyztr53:52
  [u8 += -8, 1] = xyztr53:52 <bb>
  [u8 += -8, 1] = xyztr55:54
  [u8 += -8, 1] = xyztr55:54 <bb>
  [u8 += -8, 1] = xyztr57:56
  [u8 += -8, 1] = xyztr57:56 <bb>
  [u8 += -8, 1] = xyztr59:58
  [u8 += -8, 1] = xyztr59:58 <bb>
  [u8 += -8, 1] = xyztr61:60
  [u8 += -8, 1] = xyztr61:60 <bb>
  [u8 += -8, 1] = xyztr63:62
  [u8 += -8, 1] = xyztr63:62 <bb>
  
  //Save SACC
  r36r37 = SACC0 || r38r39 = SACC1 || r40r41 = SACC2 || r42r43 = SACC3
  
  //Save ALUDFIRR
  r36 = ALUDFIRRl0 <bb> || r37 = ALUDFIRRl1 <bb> || r38 = ALUDFIRRl2 <bb> || r39 = ALUDFIRRl3 <bb>
  r40 = ALUDFIRRl4 <bb> || r41 = ALUDFIRRl5 <bb> || r42 = ALUDFIRRl6 <bb> || r43 = ALUDFIRRl7 <bb>
  r44 = ALUDFIRRh0 || r45 = ALUDFIRRh1 || r46 = ALUDFIRRh2 || r47 = ALUDFIRRh3
  r48 = ALUDFIRRh4 || r49 = ALUDFIRRh5 || r50 = ALUDFIRRh6 || r51 = ALUDFIRRh7
      
  //Save ALUDFMR
  r44 = ALUDFMRl0 <bb> || r45 = ALUDFMRl1 <bb> || r46 = ALUDFMRl2 <bb> || r47 = ALUDFMRl3 <bb>
  r48 = ALUDFMRl4 <bb> || r49 = ALUDFMRl5 <bb> || r50 = ALUDFMRl6 <bb> || r51 = ALUDFMRl7 <bb>
  r52 = ALUDFMRh0 || r53 = ALUDFMRh1 || r54 = ALUDFMRh2 || r55 = ALUDFMRh3
  r56 = ALUDFMRh4 || r57 = ALUDFMRh5 || r58 = ALUDFMRh6 || r59 = ALUDFMRh7
  
  //Save ALUDFER
  r52 = ALUDFER0 <bb> || r53 = ALUDFER1 <bb> || r54 = ALUDFER2 <bb> || r55 = ALUDFER3 <bb>
  r56 = ALUDFER4 <bb> || r57 = ALUDFER5 <bb> || r58 = ALUDFER6 <bb> || r59 = ALUDFER7 <bb>
  
  //Save ALUCAFR
  r60 = ALUCAFR0 || r61 = ALUCAFR1 || r62 = ALUCAFR2 || r63 = ALUCAFR3
  r60 = ALUCAFR4 <bb> || r61 = ALUCAFR5 <bb> || r62 = ALUCAFR6 <bb> || r63 = ALUCAFR7 <bb>

  [u8 += -8, 1] = xyztr1:0
  [u8 += -8, 1] = xyztr1:0 <bb>
  [u8 += -8, 1] = xyztr3:2
  [u8 += -8, 1] = xyztr3:2 <bb>
  [u8 += -8, 1] = xyztr5:4
  [u8 += -8, 1] = xyztr5:4 <bb>
  [u8 += -8, 1] = xyztr7:6
  [u8 += -8, 1] = xyztr7:6 <bb>
  [u8 += -8, 1] = xyztr9:8
  [u8 += -8, 1] = xyztr9:8 <bb>
  [u8 += -8, 1] = xyztr11:10
  [u8 += -8, 1] = xyztr11:10 <bb>
  [u8 += -8, 1] = xyztr13:12
  [u8 += -8, 1] = xyztr13:12 <bb>
  [u8 += -8, 1] = xyztr15:14
  [u8 += -8, 1] = xyztr15:14 <bb>
  [u8 += -8, 1] = xyztr17:16
  [u8 += -8, 1] = xyztr17:16 <bb>
  [u8 += -8, 1] = xyztr19:18
  [u8 += -8, 1] = xyztr19:18 <bb>
  [u8 += -8, 1] = xyztr21:20
  [u8 += -8, 1] = xyztr21:20 <bb>
  [u8 += -8, 1] = xyztr23:22
  [u8 += -8, 1] = xyztr23:22 <bb>
  [u8 += -8, 1] = xyztr25:24
  [u8 += -8, 1] = xyztr25:24 <bb>
  [u8 += -8, 1] = xyztr27:26
  [u8 += -8, 1] = xyztr27:26 <bb>
  [u8 += -8, 1] = xyztr29:28
  [u8 += -8, 1] = xyztr29:28 <bb>
  [u8 += -8, 1] = xyztr31:30
  [u8 += -8, 1] = xyztr31:30 <bb>
  [u8 += -8, 1] = xyztr33:32
  [u8 += -8, 1] = xyztr33:32 <bb>
  [u8 += -8, 1] = xyztr35:34
  [u8 += -8, 1] = xyztr35:34 <bb>
  [u8 += -8, 1] = xyztr37:36
  [u8 += -8, 1] = xyztr37:36 <bb>
  [u8 += -8, 1] = xyztr39:38
  [u8 += -8, 1] = xyztr39:38 <bb>
  [u8 += -8, 1] = xyztr41:40
  [u8 += -8, 1] = xyztr41:40 <bb>
  [u8 += -8, 1] = xyztr43:42
  [u8 += -8, 1] = xyztr43:42 <bb>
  [u8 += -8, 1] = xyztr45:44
  [u8 += -8, 1] = xyztr45:44 <bb>
  [u8 += -8, 1] = xyztr47:46
  [u8 += -8, 1] = xyztr47:46 <bb>
  [u8 += -8, 1] = xyztr49:48
  [u8 += -8, 1] = xyztr49:48 <bb>
  [u8 += -8, 1] = xyztr51:50
  [u8 += -8, 1] = xyztr51:50 <bb>
  [u8 += -8, 1] = xyztr53:52
  [u8 += -8, 1] = xyztr53:52 <bb>
  [u8 += -8, 1] = xyztr55:54
  [u8 += -8, 1] = xyztr55:54 <bb>
  [u8 += -8, 1] = xyztr57:56
  [u8 += -8, 1] = xyztr57:56 <bb>
  [u8 += -8, 1] = xyztr59:58
  [u8 += -8, 1] = xyztr59:58 <bb>
  [u8 += -8, 1] = xyztr61:60
  [u8 += -8, 1] = xyztr61:60 <bb>
  [u8 += -8, 1] = xyztr63:62
  [u8 += -8, 1] = xyztr63:62 <bb>

   //Save CON
  r0 = CON0 || r1 = CON1 || r2 = CON2 || r3 = CON3 || r4 = CON4 || r5 = CON5 || r6 = CON6 || r7 = CON7
  
  //Save ACF                                                                                          
  r0 = ACF0 <bb> || r1 = ACF1 <bb> || r2 = ACF2 <bb> || r3 = ACF3 <bb> 
  || r4 = ACF4 <bb> || r5 = ACF5 <bb> || r6 = ACF6 <bb> || r7 = ACF7 <bb>

  [u8 += -8, 1] = xyztr1:0
  [u8 += -8, 1] = xyztr1:0 <bb>
  [u8 += -8, 1] = xyztr3:2
  [u8 += -8, 1] = xyztr3:2 <bb>
  [u8 += -8, 1] = xyztr5:4
  [u8 += -8, 1] = xyztr5:4 <bb>
  [u8 += -8, 1] = xyztr7:6
  [u8 += -8, 1] = xyztr7:6 <bb>
  
  //Save ALUFR
  r0 = ALUFR0 || r1 = ALUFR1 || r2 = ALUFR2 || r3 = ALUFR3 || r4 = ALUFR4 || r5 = ALUFR5 || r6 = ALUFR6 || r7 = ALUFR7
	
  //Save ACC
  r0 = ACCl0 <bb> || r1 = ACCl1 <bb> || r2 = ACCl2 <bb> || r3 = ACCl3 <bb>
  || r4 = ACCl4 <bb> || r5 = ACCl5 <bb> || r6 = ACCl6 <bb> || r7 = ACCl7 <bb>

  [u8 += -8, 1] = xyztr1:0
  [u8 += -8, 1] = xyztr1:0 <bb>
  [u8 += -8, 1] = xyztr3:2
  [u8 += -8, 1] = xyztr3:2 <bb>
  [u8 += -8, 1] = xyztr5:4
  [u8 += -8, 1] = xyztr5:4 <bb>
  [u8 += -8, 1] = xyztr7:6
  [u8 += -8, 1] = xyztr7:6 <bb>
  
  //Save ACC
  r0 = ACCm0 || r1 = ACCm1 || r2 = ACCm2 || r3 = ACCm3 || r4 = ACCm4 || r5 = ACCm5 || r6 = ACCm6 || r7 = ACCm7
  r0 = ACCh0 <bb> || r1 = ACCh1 <bb> || r2 = ACCh2 <bb> || r3 = ACCh3 <bb>
  || r4 = ACCh4 <bb> || r5 = ACCh5 <bb> || r6 = ACCh6 <bb> || r7 = ACCh7 <bb>
  
  [u8 += -8, 1] = xyztr1:0
  [u8 += -8, 1] = xyztr1:0 <bb>
  [u8 += -8, 1] = xyztr3:2
  [u8 += -8, 1] = xyztr3:2 <bb>
  [u8 += -8, 1] = xyztr5:4
  [u8 += -8, 1] = xyztr5:4 <bb>
  [u8 += -8, 1] = xyztr7:6
  [u8 += -8, 1] = xyztr7:6 <bb>


  //Save ALUCR,ABFPR,BA,SR, BA,SR,LC,SER,IMACR
  r0 = ALUCR
  r1 = ABFPR

  xr0 = BA <bb> || yr0 = SR <bb> || zr0 = LC0 <bb> || tr0 = LC1 <bb>
  xr1 = LC2 <bb> || yr1 = LC3 <bb> || zr1 = SER <bb> || tr1 = IMACR <bb>
	
  [u8 += -8, 1] = xyztr1:0
  [u8 += -8, 1] = xyztr1:0 <bb>

  //双栈改造结束

  //Save u10~u15
  xr0 = u10 || xr1 = u11 || yr0 = u12 || yr1 = u13 || zr0 = u14 || zr1 = u15
  //Save IMAFR
  tr0 = IMAFR
  //造成了7个空
  [u8 += -8, 1] = xyztr1:0
  u8 = u8 + 7

  tr0 = sr
  [u8 += -1, 0] = tr0 || v0 = ___interrupt_table// || v1 = xr0		// push sr
  xr3=u9
  [u8 +=-1, 0]=xr3  	//push u9

  xr1 = ba
  [u8 += -1,0] = xr1       // push ba

  xr1 = SER
  [u8 +=-1, 0]=xr1	// push ser

  // push all interrupt return address
  xr1  = SWIR_R
  xr2  = JIHR_R
  xr3  = HIHR_R
  xr4  = HINTR_R
  xr5  = ITIHR0_R
  xr6  = ITIHR1_R
  xr7  = TIHR0_R
  xr8  = TIHR1_R
  xr9  = TIHR2_R
  xr10 = TIHR3_R
  xr11 = INTR0_R
  xr12 = INTR1_R
  xr13 = INTR2_R
  xr14 = INTR3_R
  xr15 = INTR4_R
  xr16 = INTR5_R
  xr17 = INTR6_R
  xr18 = INTR7_R
  xr19 = IINT_R
  xr20 = CDMA0_R
  xr21 = CDMA1_R
  xr22 = CDMA2_R
  xr23 = CDMA3_R
  xr24 = CDMA4_R
  xr25 = CDMA5_R
  xr26 = CDMA6_R
  xr27 = CDMA7_R
  xr28 = CDMA8_R
  xr29 = CDMA9_R
  xr30 = CDMA10_R
  xr31 = IODMA0_R
  xr32 = IODMA1_R
  xr33 = IODMA2_R
  xr34 = IODMA3_R
  xr35 = SRIR_R
  xr36 = STIR_R
  xr37 = ITILR0_R
  xr38 = ITILR1_R
  xr39 = TILR0_R
  xr40 = TILR1_R
  xr41 = TILR2_R
  xr42 = TILR3_R
  xr43 = HILR_R
  xr44 = JILR_R
  [u8 + -1, 0] = xr2:1 || u8 = u8 + -2
  [u8 + -1, 0] = xr4:3 || u8 = u8 + -2
  [u8 + -1, 0] = xr6:5 || u8 = u8 + -2
  [u8 + -1, 0] = xr8:7 || u8 = u8 + -2
  [u8 + -1, 0] = xr10:9 || u8 = u8 + -2
  [u8 + -1, 0] = xr12:11 || u8 = u8 + -2
  [u8 + -1, 0] = xr14:13 || u8 = u8 + -2
  [u8 + -1, 0] = xr16:15 || u8 = u8 + -2
  [u8 + -1, 0] = xr18:17 || u8 = u8 + -2
  [u8 + -1, 0] = xr20:19 || u8 = u8 + -2
  [u8 + -1, 0] = xr22:21 || u8 = u8 + -2
  [u8 + -1, 0] = xr24:23 || u8 = u8 + -2
  [u8 + -1, 0] = xr26:25 || u8 = u8 + -2
  [u8 + -1, 0] = xr28:27 || u8 = u8 + -2
  [u8 + -1, 0] = xr30:29 || u8 = u8 + -2
  [u8 + -1, 0] = xr32:31 || u8 = u8 + -2
  [u8 + -1, 0] = xr34:33 || u8 = u8 + -2
  [u8 + -1, 0] = xr36:35 || u8 = u8 + -2
  [u8 + -1, 0] = xr38:37 || u8 = u8 + -2
  [u8 + -1, 0] = xr40:39 || u8 = u8 + -2
  [u8 + -1, 0] = xr42:41 || u8 = u8 + -2
  [u8 + -1, 0] = xr44:43 || u8 = u8 + -2


//.code_align 4
//if xr0 != r1 b _intr_no_swir

  xr1 = _current_stack_base
  w0 = xr1
  xr0 = [w0+0,0]
  [u8 + 0, 0] = xr0
  u8 = u8 + -1

  xr1 = 61
  xr0 = v1
.code_align 4
if xr0 != r1 b _intr_no_swir
  b _intr_swir   // context_switch
  
/**********************end of context save*******************************/
_intr_no_swir:
.ref ___ISR_Nest_level
.ref ___Thread_Dispatch_disable_level
.ref ___ISR_Signals_to_thread_executing
.ref ___Context_Switch_necessary
  v2 = ___ISR_Nest_level
  xr1 = [v2+0,0] || v3 = ___Thread_Dispatch_disable_level
  xr1 += 1 || xr2 =  [v3+0,0]
  [v2+0,0] = xr1 || xr2 += 1					// inc __ISR_Nest_level
  [v3+0,0] = xr2 								// inc __Thread_Dispatch_disable_level

  xr1 = [v0+v1, 0]  //the address of isr
  sr = xr1 || xr1 = u8
  xr1 += 1 || u8 = u8 + -2        // reserve space for parameter of the ISR
  u1 = xr1 			// the second parameter

// the **ISR** may looks like: void _xxx_isr(int vector, context_control *int_reg_frame)
.code_align 4
  call sr             // note: xr0 is the input parameter of ISR
	
/**********************return frome ISR*****************************/
  u8 = u8 + 2 	// skip the space reserved for parameter of the ISR
  v2 = ___ISR_Nest_level
  xr1 = [v2+0,0] || v3 = ___Thread_Dispatch_disable_level
  xr1 += -1 || xr2 =  [v3+0,0]
  [v2+0,0] = xr1 || xr2 += -1					// decl __ISR_Nest_level
  [v3+0,0] = xr2 								// decl __Thread_Dispatch_disable_level

//--_ISR_Nest_level != 0时，继续中断	
  xr0 = 0
.code_align 4       // if interrupts are nested
  if xr1 != r0 b  _exitme          //by zwj,use _exitme ,not use _restore ,easy to understand its role

//--_Thread_Dispatch_disable_level != 0   继续中断
.code_align 4  	
  if xr2 != r0 b _exitme

  xr0 = 0
  v1 = ___Context_Switch_necessary
  xr1 = [v1+0,0]

.code_align 4		/* Is task switch necessary? */	
  if xr1 == r0 b _exitme               /* No, then call the schedule */

_schedule:
  xr0 = u8
  xr0 += 1
  u0 = xr0

  u8 = u8 + -1
  sr= ___Int_Thread_Dispatch
.code_align 4
  call sr
  xr0 = u4
  u1 = xr0

  u8 = u8 + 1

  b ___CPU_Context_restore  // interrupt except swir(context_switch)

_intr_swir:
   xr0=u8	//u0=&_Thread_Executing->registers
   xr0 += 1 
   [u0+0,0]=xr0

.global ___CPU_Context_restore
___CPU_Context_restore:
   xr8 = u1	//u1=_Thread_Heir->registers(context_control *)
   xr8 += -1
   u8 = xr8     //U8=_Thread_Heir->registers - 1;

_exitme:
  //load current_stack_base of _Thread_Heir
  xr0 = [u8+1,0]
  xr1 = _current_stack_base
  v0 = xr1
  [v0+0,0] = xr0

  u8 = u8 + 1  //加上 _current_stack_base 那一位?

//_exitme:
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
  
  //restore -- correspond to start_interrupt.asm:_comm_isr
 /**不论从哪个中断恢复，都可以得到正确的返回地址**/ 
  xr0 = [u8+1,0] || u8 = u8 + 1
  SWIR_R = xr0
  JIHR_R = xr0
  HIHR_R = xr0
  HINTR_R = xr0
  ITIHR0_R = xr0
  ITIHR1_R = xr0
  TIHR0_R = xr0
  TIHR1_R = xr0
  TIHR2_R = xr0
  TIHR3_R = xr0
  INTR0_R = xr0
  INTR1_R = xr0
  INTR2_R = xr0
  INTR3_R = xr0
  INTR4_R = xr0
  INTR5_R = xr0
  INTR6_R = xr0
  INTR7_R = xr0
  IINT_R  = xr0
  CDMA0_R = xr0
  CDMA1_R = xr0
  CDMA2_R = xr0
  CDMA3_R = xr0
  CDMA4_R = xr0
  CDMA5_R = xr0
  CDMA6_R = xr0
  CDMA7_R = xr0
  CDMA8_R = xr0
  CDMA9_R = xr0
  CDMA10_R = xr0
  IODMA0_R = xr0
  IODMA1_R = xr0
  IODMA2_R = xr0
  IODMA3_R = xr0
  SRIR_R = xr0
  STIR_R = xr0
  ITILR0_R = xr0
  ITILR1_R = xr0
  TILR0_R = xr0
  TILR1_R = xr0
  TILR2_R = xr0
  TILR3_R = xr0
  HILR_R = xr0
  JILR_R = xr0
 /************************************/ 
  xr0 = [u8+1,0] || u8 = u8 + 1
  ba = xr0
  xr1:0 = [u8+1,1] || u8 = u8 + 2
  //v0/v1 的恢复
  v0 = xr0
  v1 = xr1
  xyztr1:0 = [u8+1,1] || u8 = u8 + 8

//guohl///直接返回就好了
  set CGCR[0] // 开全局中断
  reti
////////

//-------------remove these for release version-------------
   rtrap 30

  set CGCR[0] // 开全局中断
  yr0 = PMASKRl
  yr1 = PMASKRh
  yr0 = r0 + r1
  yr1 = 0
  if yr0 != r1 b _context_restore_from_intr
  
.code_align 4
  yr0 = ITIHR0_R
  SER = yr0
  ret

//----------------------------------------------------------
.code_align 4
_context_restore_from_intr:
   reti

_simple_exit:
   xr1=0
   sr=_exitme
.code_align 4
   call sr
.code_align 4
   ret
