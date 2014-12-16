.global ___interrupt_table
.macro _comm_isr  int_id
    //-------------remove these for release version-------------
	rtrap 6
	rtrap 29
	nop
	nop
	[u8+-7, 1] = xyztr1:0 || u8 = u8 + -10 || xr0 = v0 || xr1 = v1		// push xyztr1:0
	[u8+1, 0] = xr1:0 || v0 = ___interrupts || v1 = \int_id || xr0 = ba 	// push v1 v0
	[u8+0, 0] = xr0 || u8 = u8 + -1 || yr0 = [v0+v1, 0] 			// push ba
	xr0 = \int_id || ba = yr0 	
	//clr PMASKH[21]
	.code_align 4									
	b ba       // note: xr0 is the interrupt vector
	// We will not return here at all!

.endm

.text
.ref ___interrupts
.global _conn_sys_isr

_SWIR_E:

    .callm _comm_isr 61

_JIHR_E:

    .callm _comm_isr 59

_HIHR_E:

    .callm _comm_isr 57

_HINTR_E:

    .callm _comm_isr 54

_ITIHR0_E:

    .callm _comm_isr 53

_ITIHR1_E:

    .callm _comm_isr 52

_TIHR0_E:

    .callm _comm_isr 51

_TIHR1_E:

    .callm _comm_isr 50

_TIHR2_E:

    .callm _comm_isr 49

_TIHR3_E:

    .callm _comm_isr 48

_INTR0_E:

    .callm _comm_isr 47

_INTR1_E:

    .callm _comm_isr 46

_INTR2_E:

    .callm _comm_isr 45

_INTR3_E:

    .callm _comm_isr 44

_INTR4_E:

    .callm _comm_isr 43

_INTR5_E:

    .callm _comm_isr 42

_INTR6_E:

    .callm _comm_isr 41

_INTR7_E:

    .callm _comm_isr 40

_IINT_E:
	.callm _comm_isr 38

_CDMA0_E:
	.callm _comm_isr 35

_CDMA1_E:
	.callm _comm_isr 34

_CDMA2_E:
	.callm _comm_isr 33

_CDMA3_E:
	.callm _comm_isr 32

_CDMA4_E:
	.callm _comm_isr 31

_CDMA5_E:
	.callm _comm_isr 30

_CDMA6_E:
	.callm _comm_isr 29

_CDMA7_E:
	.callm _comm_isr 28

_CDMA8_E:
	.callm _comm_isr 27

_CDMA9_E:
	.callm _comm_isr 26

_CDMA10_E:
	.callm _comm_isr 25

_IODMA0_E:
	.callm _comm_isr 24

_IODMA1_E:
	.callm _comm_isr 23

_IODMA2_E:
	.callm _comm_isr 22

_IODMA3_E:
	.callm _comm_isr 21

_SRIR_E:

    .callm _comm_isr 17

_STIR_E:

    .callm _comm_isr 16

_ITILR0_E:

	.callm _comm_isr 13

_ITILR1_E:

	.callm _comm_isr 12

_TILR0_E:

    .callm _comm_isr 11

_TILR1_E:

    .callm _comm_isr 10

_TILR2_E:

    .callm _comm_isr 9

_TILR3_E:

    .callm _comm_isr 8

_HILR_E:
	.callm _comm_isr 5

_JILR_E:
	.callm _comm_isr 3				

//---------------------------------------------
//  input:
//         xr0: ID of specific INT
//
_conn_sys_isr:

  xr1 = ba
  [u8+0, 0] = xr1 || u8 = u8 + -1	//±£´æ ba

  u0 = _sys_isr_jmp_tbl || u1 = xr0
  xr1 = [u0 + u1, 0]
  ba = xr1 || xr1 = _active_isr_end
  b ba

_active_isr_none:
  ba = xr1
  b ba
_active_isr_swir:
  ba = xr1
  SWIR_E = _SWIR_E
  b ba
_active_isr_jihr:
	ba = xr1
	JIHR_E = _JIHR_E
	b ba
_active_isr_hihr:
	ba = xr1
	HIHR_E = _HIHR_E
	b ba
_active_isr_hintr:
	ba = xr1
	HINTR_E = _HINTR_E
	b ba
_active_isr_itihr0:
	ba = xr1
	ITIHR0_E = _ITIHR0_E
	b ba
_active_isr_itihr1:
	ba = xr1
	ITIHR1_E = _ITIHR1_E
	b ba
_active_isr_tihr0:
	ba = xr1
	TIHR0_E = _TIHR0_E
	b ba
_active_isr_tihr1:
	ba = xr1
	TIHR1_E = _TIHR1_E
	b ba
_active_isr_tihr2:
	ba = xr1
	TIHR2_E = _TIHR2_E
	b ba
_active_isr_tihr3:
	ba = xr1
	TIHR3_E = _TIHR3_E
	b ba
_active_isr_intr0:
	ba = xr1
	INTR0_E = _INTR0_E
	b ba
_active_isr_intr1:
	ba = xr1
	INTR1_E = _INTR1_E
	b ba
_active_isr_intr2:
	ba = xr1
	INTR2_E = _INTR2_E
	b ba
_active_isr_intr3:
	ba = xr1
	INTR3_E = _INTR3_E
	b ba
_active_isr_intr4:
	ba = xr1
	INTR4_E = _INTR4_E
	b ba
_active_isr_intr5:
	ba = xr1
	INTR5_E = _INTR5_E
	b ba
_active_isr_intr6:
	ba = xr1
	INTR6_E = _INTR6_E
	b ba
_active_isr_intr7:
	ba = xr1
	INTR7_E = _INTR7_E
	b ba
_active_isr_iint:
	ba = xr1
	IINT_E = _IINT_E
	b ba
_active_isr_cdma0:
	ba = xr1
	CDMA0_E = _CDMA0_E
	b ba
_active_isr_cdma1:
	ba = xr1
	CDMA1_E = _CDMA1_E
	b ba
_active_isr_cdma2:
	ba = xr1
	CDMA2_E = _CDMA2_E
	b ba
_active_isr_cdma3:
	ba = xr1
	CDMA3_E = _CDMA3_E
	b ba
_active_isr_cdma4:
	ba = xr1
	CDMA4_E = _CDMA4_E
	b ba
_active_isr_cdma5:
	ba = xr1
	CDMA5_E = _CDMA5_E
	b ba
_active_isr_cdma6:
	ba = xr1
	CDMA6_E = _CDMA6_E
	b ba
_active_isr_cdma7:
	ba = xr1
	CDMA7_E = _CDMA7_E
	b ba
_active_isr_cdma8:
	ba = xr1
	CDMA8_E = _CDMA8_E
	b ba
_active_isr_cdma9:
	ba = xr1
	CDMA9_E = _CDMA9_E
	b ba
_active_isr_cdma10:
	ba = xr1
	CDMA10_E = _CDMA10_E
	b ba
_active_isr_iodma0:
	ba = xr1
	IODMA0_E = _IODMA0_E
	b ba
_active_isr_iodma1:
	ba = xr1
	IODMA1_E = _IODMA1_E
	b ba
_active_isr_iodma2:
	ba = xr1
	IODMA2_E = _IODMA2_E
	b ba
_active_isr_iodma3:
	ba = xr1
	IODMA3_E = _IODMA3_E
	b ba
_active_isr_srir:
	ba = xr1
	SRIR_E = _SRIR_E
	b ba
_active_isr_stir:
	ba = xr1
	STIR_E = _STIR_E
	b ba
_active_isr_itilr0:
	ba = xr1
	ITILR0_E = _ITILR0_E
	b ba
_active_isr_itilr1:
	ba = xr1
	ITILR1_E = _ITILR1_E
	b ba
_active_isr_tilr0:
	ba = xr1
	TILR0_E = _TILR0_E
	b ba
_active_isr_tilr1:
	ba = xr1
	TILR1_E = _TILR1_E
	b ba
_active_isr_tilr2:
	ba = xr1
	TILR2_E = _TILR2_E
	b ba
_active_isr_tilr3:
	ba = xr1
	TILR3_E = _TILR3_E
	b ba
_active_isr_hilr:
	ba = xr1
	HILR_E = _HILR_E
	b ba
_active_isr_jilr:
	ba = xr1
	JILR_E = _JILR_E
	b ba

_active_isr_end:
  xr1 = [u8+1, 0] || u8 = u8 + 1
  ba = xr1

  .code_align 4
  ret

.data
_sys_isr_jmp_tbl:
  .word _active_isr_none      // 0
  .word _active_isr_none      // 1
  .word _active_isr_none      // 2
  .word _active_isr_jilr      // 3
  .word _active_isr_none      // 4
  .word _active_isr_hilr //5
  .word _active_isr_none  //6
  .word _active_isr_none  //7
  .word _active_isr_tilr3 //8
  .word _active_isr_tilr2 //9
  .word _active_isr_tilr1 //10
  .word _active_isr_tilr0 //11
  .word _active_isr_itilr1 //12
  .word _active_isr_itilr0 //13
  .word _active_isr_none  //14
  .word _active_isr_none  //15
  .word _active_isr_stir //16
  .word _active_isr_srir //17
  .word _active_isr_none  //18
  .word _active_isr_none  //19
  .word _active_isr_none  //20
  .word _active_isr_iodma3 //21
  .word _active_isr_iodma2 //22
  .word _active_isr_iodma1 //23
  .word _active_isr_iodma0 //24
  .word _active_isr_cdma10 //25
  .word _active_isr_cdma9  //26
  .word _active_isr_cdma8  //27
  .word _active_isr_cdma7  //28
  .word _active_isr_cdma6  //29
  .word _active_isr_cdma5  //30
  .word _active_isr_cdma4  //31
  .word _active_isr_cdma3  //32
  .word _active_isr_cdma2  //33
  .word _active_isr_cdma1  //34
  .word _active_isr_cdma0  //35
  .word _active_isr_none  //36
  .word _active_isr_none  //37
  .word _active_isr_iint   //38
  .word _active_isr_none  //39
  .word _active_isr_intr7  //40
  .word _active_isr_intr6  //41
  .word _active_isr_intr5  //42
  .word _active_isr_intr4  //43
  .word _active_isr_intr3  //44
  .word _active_isr_intr2  //45
  .word _active_isr_intr1  //46
  .word _active_isr_intr0  //47
  .word _active_isr_tihr3  //48
  .word _active_isr_tihr2  //49
  .word _active_isr_tihr1  //50
  .word _active_isr_tihr0  //51
  .word _active_isr_itihr1  //52
  .word _active_isr_itihr0  //53
  .word _active_isr_hintr   //54
  .word _active_isr_none  //55
  .word _active_isr_none  //56
  .word _active_isr_hihr  //57
  .word _active_isr_none  //58
  .word _active_isr_jihr  //59
  .word _active_isr_hintr     // 60
  .word _active_isr_swir      // 61
  .word _active_isr_none      // 62
  .word _active_isr_none      // 63