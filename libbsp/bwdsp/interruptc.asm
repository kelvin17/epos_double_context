.text
.ref _interruptc_process
.ref ___interrupt_table
.ref ___interrupts
.ref _conn_sys_isr
.global __isr_installc
//����C���Ե��û��Ĺ���
// input:
// xr0 interrupt vector number
// u1 	isr address(<0x1ffff)				00001000000000000000000000000000
 						//xr2 = 11111111000000001100000001111100  (��32λ)  
__isr_installc:					//xr1 = 00111111101111111111111100000000  (��32λ)   Ϊ0��ʾ�����ж�
   xr1 = 0x3FBFFF00 ||	xr2 = 0xFF00C07C	// bit[n]=0 means reserved interrupt number
   xr3 = 1 ||	xr4 = 32
   xr5 = 0 ||	xr6 = 0
   xr7 = 0 ||	u6=xr0
   xr8 = 0x1FFFFF || xr9 = u1//modified by zwj
   xr10 = SER
   [u8 +=-1, 0]=xr10 
   // xr9 as well as u1 has the addr of isr to be install 
   // u6 as well as xr0 has the int_no to be install

//�жϳ�����ڵ�ַ���ܳ��� 0x1FFFFF, �����ֻ����ô��
.code_align 4
   If xr9 > r8 (u) b _interruptc_9  // the entrance of ISR can not larger than 0x1FFFFF

//�жϺŲ��ܴ��� 63 ���ܹ�ֻ�� 64 ���ж�
   xr9 = 63
.code_align 4
   If xr0 > r9 (u) b _interruptc_9  // ID of the IRQ can not larger than 63


//�жϺ�С�� 32 ��ִ�� _interruptc_0
.code_align 4
   If xr4 > r0  b _interruptc_0    // if ID of the IRQ less than 32, jump to _interruptc_0 (lower 32)


//�����жϺ� > 32 ���жϺŽ����ж��Ƿ�Ϸ�
   xr0 -= 32
   xr3 = r3 lshift r0
   xr1 = r1 & r3 ||	xr6 = r3
   
.code_align 4
   if xr1==r5 b _interruptc_9      // compare with the mask, check whether the ID is invalid
   
//�Ϸ��жϺţ����� _interruptc_1
//.code_align 4
   sr = _interruptc_1
   call sr
// xr1 has the bit to be set, as well as xr6&xr3 if the ID is valid


//�����жϺ� < 32 ���жϺŽ����ж��Ƿ�Ϸ�
_interruptc_0:			// lower 32 interrupts
   xr3 = r3 lshift r0
   xr2 = r2 & r3 ||	xr7 = r3
   
.code_align 4
   if xr2==r5 b _interruptc_9     // compare with the mask, check whether the ID is invalid
// xr2 has the bit to be set, as well as xr7&xr3 if the ID is valid


//___interrupt_table ������Ǹ��ж� C �������ڵ�ַ
//___interrupts      ������Ǹ��жϻ�����ڵ�ַ

//�Ϸ����жϺ�	
_interruptc_1:		
   u5 = ___interrupt_table ||	u7 = ___interrupts ||  xr12 = _interruptc_process ||  xr10 = u1
   xr13 = IMASKRh	
   xr14 = IMASKRl
   xr11 = [u5+u6, 0]                    // get current value in specific entry of ___interrupt_table, the value will be returned by U4	
   //-----------------------------------  GRui: added 20110407
   xr15 = 1 || xr16 = u1

// u1 != 1, �� _interruptc_10 ��
.code_align 4
   if xr15 != r16 b _interruptc_10      // compare with SIG_IGN, which is 1
   //�ڶ���������SIT_IGNʱ������ָ�����ж��ź�
   xr6 = ! r6 || xr7 = ! r7
   xr13 = r13 & r6 ||	xr14 = r14 & r7  // xr6, xr7 is the bit mask of current IRQ
   IMASKRh = xr13  ||	IMASKRl = xr14   // set specific bit to 1 in IMASKR
   
//.code_align 4
   sr = _interruptc_11
   call sr  || u4 = xr11
//-----------------------------------


//---------------------�ص�������----------------------	
_interruptc_10:
   xr13 = r13 | r6 ||	xr14 = r14 | r7  // xr6, xr7 is the bit mask of current IRQ
   IMASKRh = xr13  ||	IMASKRl = xr14   // set specific bit to 1 in IMASKR	
   [u5+u6, 0] = xr10                    // write new value (entry address of new ISR) to specific entry of ___interrupt_table
   [u7+u6, 0] = xr12                    // write the value of _interruptc_process to table ___interrupts. _interruptc_process is defined in _interruptc.asm


//���� conn_sys_isr ����������Ϊ�жϺţ������� xr0 ��
   xr0 = u6
   /* xr8 = ra  note by zwj
   [u8+=-1,0] = xr8 */
.code_align 4
   sr= _conn_sys_isr
   call sr

   /* note by zwj
   xr8 = [u8+1,0] || u8 = u8+1
   ser = xr8 
   */
//.code_align 4
   sr=_interruptc_11
   call sr  || u4 = xr11 		// ����ԭ����isr��ַ

_interruptc_9:
   u4 = 0xFFFFFFFF  // GRui modified: 2010-12-28
					// return -1
	
_interruptc_11:
   xr17 = [u8+1,0] || u8 = u8+1
   ser = xr17 //error! cxl: 2014-07-18 for ECS V1.0.9.5  // add by zwj,,�����ѹջ��ջ��û��������,��������ע�͵���
.code_align 4
   ret
