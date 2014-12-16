MEMORY
{
	PROG(rx) : ORIGIN = 0x0,      LENGTH = 2048K
    DATA0(rw): ORIGIN = 0x200000, LENGTH = 128K
    DATA1(rw): ORIGIN = 0x400000, LENGTH = 256K
    DATA2(rw): ORIGIN = 0x620000, LENGTH = 128K
	DATA3(rw): ORIGIN = 0xA20000, LENGTH = 128K
    STACK(rw): ORIGIN = 0x600000, LENGTH = 32K
    HEAP(rw): ORIGIN = 0x610000, LENGTH = 32K
    DEBUG_INFO      :       ORIGIN =0x0	, LENGTH =1024K
    DEBUG_ARANGES   :   	ORIGIN =0x0 , LENGTH =256K
    DEBUG_PUBNAMES  :       ORIGIN =0x0 , LENGTH =256K
    DEBUG_LINE      :       ORIGIN =0x0 , LENGTH =256K
    DEBUG_ABBREV    :       ORIGIN =0x0 , LENGTH =256K
}

SECTIONS
{
  .text :
  {
  	*(.text)
  } > PROG
  .data :
  {
	*(.data)
   }  > DATA0
   .bss :
   {
	*(.bss)
   }  > DATA1
  .ccdata0 :
   {
	*(.ccdata0)

   }  > DATA0
   .ccdata1 :
   {
	*(.ccdata1)

   }  > DATA1
   .ccdata2 :
   {
	*(.ccdata2)

   }  > DATA2
    .ccdata3 :
   {
	*(.ccdata3)

   }  > DATA3
  .debug_info :
  {
    *(.debug_info)
  } > DEBUG_INFO
  .debug_aranges : 
  {
    *(.debug_aranges)
  } > DEBUG_ARANGES
  .debug_pubnames :
  {
    *(.debug_pubnames)
  } > DEBUG_PUBNAMES
  .debug_line :
  {
    *(.debug_line)
  } > DEBUG_LINE
  .debug_abbrev :
  {
     *(.debug_line)
  } > DEBUG_ABBREV
 
 }
