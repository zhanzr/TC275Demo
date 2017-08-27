
-Xgenerate-copytables

LCF_CSA0_SIZE = 8k;
LCF_USTACK0_SIZE = 2k;
LCF_ISTACK0_SIZE = 1k;

LCF_CSA1_SIZE = 8k;
LCF_USTACK1_SIZE = 2k;
LCF_ISTACK1_SIZE = 1k;

LCF_CSA2_SIZE = 8k;
LCF_USTACK2_SIZE = 2k;
LCF_ISTACK2_SIZE = 1k;

LCF_HEAP_SIZE = 2k;

LCF_DSPR2_START = 0x50000000;
LCF_DSPR2_SIZE = 120k;

LCF_DSPR1_START = 0x60000000;
LCF_DSPR1_SIZE = 120k;

LCF_DSPR0_START = 0x70000000;
LCF_DSPR0_SIZE = 112k;

LCF_CSA2_OFFSET	=	(LCF_DSPR2_SIZE - 1k - LCF_CSA2_SIZE);
LCF_ISTACK2_OFFSET =	(LCF_CSA2_OFFSET - 256 - LCF_ISTACK2_SIZE);
LCF_USTACK2_OFFSET =	(LCF_ISTACK2_OFFSET - 256 - LCF_USTACK2_SIZE);

LCF_CSA1_OFFSET	=	(LCF_DSPR1_SIZE - 1k - LCF_CSA1_SIZE);
LCF_ISTACK1_OFFSET =	(LCF_CSA1_OFFSET - 256 - LCF_ISTACK1_SIZE);
LCF_USTACK1_OFFSET =	(LCF_ISTACK1_OFFSET - 256 - LCF_USTACK1_SIZE);

LCF_CSA0_OFFSET	=	(LCF_DSPR0_SIZE - 1k - LCF_CSA0_SIZE);
LCF_ISTACK0_OFFSET =	(LCF_CSA0_OFFSET - 256 - LCF_ISTACK0_SIZE);
LCF_USTACK0_OFFSET =	(LCF_ISTACK0_OFFSET - 256 - LCF_USTACK0_SIZE);

LCF_HEAP0_OFFSET =	(LCF_USTACK0_OFFSET - LCF_HEAP_SIZE);
LCF_HEAP1_OFFSET =	(LCF_USTACK1_OFFSET - LCF_HEAP_SIZE);
LCF_HEAP2_OFFSET =	(LCF_USTACK2_OFFSET - LCF_HEAP_SIZE);

LCF_INTVEC0_START = 0x800F0000;
LCF_TRAPVEC0_START = 0x80000100;
LCF_TRAPVEC1_START = 0x800F6200;
LCF_TRAPVEC2_START = 0x800F6000;

RESET = 0x80000020;

MEMORY
{
	dsram2: org = 0x50000000 + 100, len = 120K - 100 /* Workaround for segment overlap problem*/
  	psram2: org = 0x50100000, len = 24K
  	
  	dsram1: org = 0x60000000 + 100, len = 120K - 100 /* Workaround for segment overlap problem*/
  	psram1: org = 0x60100000, len = 24K
  	
  	dsram0: org = 0x70000000 + 100, len = 112K - 100 /* Workaround for segment overlap problem*/
  	psram0: org = 0x70100000, len = 24K
  	
	psram_local: org = 0xc0000000, len = 24K
	
	pfls0: org = 0x80000000, len = 2M
	pfls0_nc: org = 0xa0000000, len = 2M
	
	pfls1: org = 0x80200000, len = 2M
	pfls1_nc: org = 0xa0200000, len = 2M
	
	dfls0: org = 0xaf000000, len = 384K
	
	lmuram: org = 0x90000000, len = 32K
	lmuram_nc: org = 0xb0000000, len = 32K
	
	edmem: org = 0x9f000000, len = 1M
	edmem_nc: org = 0xbf000000, len = 1M
}

SECTIONS
{	
	/*This section is always required as Boot mode header 0 address absolutely restricted at address 0x80000000*/
	GROUP BIND(0x80000000) : 
	{
		.bmhd_0 (CONST) : 
		{
			BootModeHeader0 = .;
			KEEP(*(.bmhd_0))
		}
	} > pfls0
	
	/*This section is always required as Boot mode header 1 address absolutely restricted at address 0x80020000*/
	GROUP BIND(0x80020000) : 
	{
		.bmhd_1 (CONST) : 
		{
			BootModeHeader1 = .;
			KEEP(*(.bmhd_1))
		}
	} > pfls0
	
	/*This section is always required as user start address absolutely restricted at address 0x80000020*/
	GROUP BIND(0x80000020) : 
	{
		.startup (TEXT) : 
		{
			BootModeIndex = .;
			. = ALIGN(8);
			KEEP(*(.start))
			. = ALIGN(8);
		}
	} > pfls0
	
	/*This section contains the data indirection pointers to interface external devices*/
	GROUP BIND(0x80000040) : 
	{
		.interface_const (CONST) :
		{
			__IF_CONST = .;
			KEEP (*(.interface_const))
			. = ALIGN(8);
		}
	} > pfls0

	GROUP BIND(LCF_TRAPVEC0_START) : 
	{
		.traptab_tc0 (TEXT) :
		{
			__TRAPTAB_CPU0 = .;
			KEEP (*(.traptab_cpu0))
		}
	} > pfls0
	
	GROUP :
	{
		.zrodata (CONST) :
		{
			*(.zrodata)	
		}
	
		.srodata (CONST) :
		{
			*(.srodata)
	    	*(.ldata)
	    	*(.lbss)	/*Workaround to getrid of linker warning for external const definitions*/
		}
		_LITERAL_DATA_ = SIZEOF(.srodata) ? ADDR(.srodata) + 32k : (ADDR(.srodata) & 0xF0000000) + 32k ;
		__A1_MEM = _LITERAL_DATA_;
		 	
	   	.rodata (CONST) :
		{
			*(.rodata)
	  	}
	  	
	  	.copytable (CONST) : 
	  	{
	  		__DATA_ROM = .;
	  	}
	  	
	  	.text (TEXT) :
		{
			*(.text)
			*(.frame_info)
			*(.init)
			*(.fini)
			. = ALIGN(8);
		}
		
		.ctors (CONST) ALIGN(4) : 
		{
			ctordtor.o(.ctors)
			*(.ctors) 
		}
		.dtors (CONST) ALIGN(4) :
		{ 
			ctordtor.o(.dtors)
			*(.dtors)
		}
	} > pfls0
	
	GROUP BIND(LCF_TRAPVEC2_START) : 
	{	
		.traptab_tc2 (TEXT) :
		{
			__TRAPTAB_CPU2 = .;
			KEEP (*(.traptab_cpu2))
		}
	} > pfls0
		
	GROUP BIND(LCF_TRAPVEC1_START) : 
	{
		.traptab_tc1 (TEXT) :
		{
			__TRAPTAB_CPU1 = .;
			KEEP (*(.traptab_cpu1))
		}
	
	} > pfls0
	
	/*Near data sections*/
	GROUP : 
	{
		.zdata_tc2 (DATA) LOAD(> pfls0) COPYTABLE :
		{
			*(.zdata_cpu2)
	    }
	    
	    .zbss_tc2 (BSS) :
		{
			*(.zbss_cpu2)
		}
	} > dsram2
	
	GROUP : 
	{
		.zdata_tc1 (DATA) LOAD(> pfls0) COPYTABLE :
		{
			*(.zdata_cpu1)
	    }
	    
	    .zbss_tc1 (BSS) :
		{
			*(.zbss_cpu1)
		}
	} > dsram1
	
	GROUP : 
	{
		.zdata_tc0 (DATA) LOAD(> pfls0) COPYTABLE :
		{  
			*(.zdata_cpu0)
	    }
	    
	    .zbss_tc0 (BSS) :
		{
			*(.zbss_cpu0)
		}
	} > dsram0
	
	/*RAM sections without cpu sufix will be here*/
	
	GROUP : 
	{	
		.zdata (DATA) LOAD(> pfls0) COPYTABLE :
		{
			*(.zdata) 	
		}
		
		.sdata (DATA) LOAD(> pfls0) COPYTABLE :
		{
			*(.sdata)
		}
		
		.sbss (BSS) :
		{
			*(.sbss)
		}
		
		_SMALL_DATA_ = SIZEOF(.sdata) ? ADDR(.sdata) + 32k : (ADDR(.sdata) & 0xF0000000) + 32k ;
		__A0_MEM = _SMALL_DATA_;
				
		.data LOAD(> pfls0) COPYTABLE :
		{
			*(.data)	
		}
		
		.bss (BSS) :
		{
			*(.bss) 	
		}
		
		.heap  :
		{
    		. = ALIGN(8);
    		__HEAP = .;
    		__HEAP_START = .;
    		__HEAP_END = . + LCF_HEAP_SIZE;
		}
/*Un comment one of the below statements to enable CpuX DMI RAM to hold global variables*/	
/*	} > dsram0 */
	} > dsram1 
/*	} > dsram2 */
		
	GROUP :	
	{
		.data_tc2 (DATA) LOAD(> pfls0) COPYTABLE :
		{
			*(.data_cpu2)
		}
		
		.bss_tc2 (BSS) :
		{
			*(.bss_cpu2)
		}
	} > dsram2
	
	GROUP :
	{	
		.data_tc1 (DATA) LOAD(> pfls0) COPYTABLE :
		{
			*(.data_cpu1)
		}
		
		.bss_tc1 (BSS) :
		{
			*(.bss_cpu1)
		}
	} > dsram1
	
	GROUP : 
	{	
		.data_tc0 (DATA) LOAD(> pfls0) COPYTABLE :
		{
			*(.data_cpu0)
		}
		
		.bss_tc0 (BSS) :
		{
			*(.bss_cpu0)
		}
	} > dsram0
	
	/* CPU2 Stack and csa reservation*/
	GROUP BIND(LCF_DSPR2_START + LCF_USTACK2_OFFSET) : 
	{
		.ustack_tc2 (BSS) :
		{
			__USTACK2_END = .;
			. = . + LCF_USTACK2_SIZE;
			__USTACK2 = .;
		}
	} > dsram2
		
	GROUP BIND(LCF_DSPR2_START + LCF_ISTACK2_OFFSET) : 
	{
		.istack_tc2 (BSS) :
		{
			__ISTACK2_END = .;
			. = . + LCF_ISTACK2_SIZE;
			__ISTACK2 = .;
		}
	} > dsram2	
	
	GROUP BIND(LCF_DSPR2_START + LCF_CSA2_OFFSET) : 
	{
		.csa_tc2 (BSS) :
		{
			__CSA2 = .;
			. = . + LCF_CSA2_SIZE;
			__CSA2_END = .;
		}
	} > dsram2
	
	/*CPU1 Stack and csa reservation*/		
		
	GROUP BIND(LCF_DSPR1_START + LCF_USTACK1_OFFSET) : 
	{
		.ustack_tc1 (BSS) :
		{
			__USTACK1_END = .;
			. = . + LCF_USTACK1_SIZE;
			__USTACK1 = .;
		}
	} > dsram1
		
	GROUP BIND(LCF_DSPR1_START + LCF_ISTACK1_OFFSET) : 
	{	
		.istack_tc1 (BSS) :
		{
			__ISTACK1_END = .;
			. = . + LCF_ISTACK1_SIZE;
			__ISTACK1 = .;
		}
	} > dsram1	
	
	GROUP BIND(LCF_DSPR1_START + LCF_CSA1_OFFSET) : 
	{	
		.csa_tc1 (BSS) :
		{
			__CSA1 = .;
			. = . + LCF_CSA1_SIZE;
			__CSA1_END = .;
		}
	} > dsram1
	
	/*CPU0 Stack and csa reservation*/

	GROUP BIND(LCF_DSPR0_START + LCF_USTACK0_OFFSET) : 
	{	
		.ustack_tc0 (BSS) :
		{
			__USTACK0_END = .;
			. = . + LCF_USTACK0_SIZE;
			__USTACK0 = .;
		}
	} > dsram0
		
	GROUP BIND(LCF_DSPR0_START + LCF_ISTACK0_OFFSET) : 
	{	
		.istack_tc0 (BSS) :
		{
			__ISTACK0_END = .;
			. = . + LCF_ISTACK0_SIZE;
			__ISTACK0 = .;
		}
	} > dsram0
		
	GROUP BIND(LCF_DSPR0_START + LCF_CSA0_OFFSET) : 
	{	
		.csa_tc0 (BSS) :
		{
			__CSA0 = .;
			. = . + LCF_CSA0_SIZE;
			__CSA0_END = .;
		}
	} > dsram0
	
	GROUP : 
	{			
	   	.rodata_a8 :
		{
			*(.rodata_a8)
		}
	} > pfls0
	
	__A8_MEM = SIZEOF(.rodata_a8) ? ADDR(.rodata_a8) + 32k : (ADDR(.rodata_a8) & 0xF0000000) + 32k ;
	
	GROUP : 
	{			
		.lmudata (DATA) LOAD(> pfls0) COPYTABLE :
		{
			*(.sdata_a9)
		}
		
		.lmubss (BSS) :
		{
			*(.sbss_a9)
		}
		__A9_MEM = SIZEOF(.lmudata) ? ADDR(.lmudata) + 32k : (ADDR(.lmudata) & 0xF0000000) + 32k ;
	} > lmuram
					
	GROUP BIND(LCF_INTVEC0_START + 0x0)   : { .inttab_tc0_000 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_0)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x20)   : { .inttab_tc0_001 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_1)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x40)   : { .inttab_tc0_002 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_2)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x60)   : { .inttab_tc0_003 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_3)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x80)   : { .inttab_tc0_004 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_4)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xA0)   : { .inttab_tc0_005 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_5)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xC0)   : { .inttab_tc0_006 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_6)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xE0)   : { .inttab_tc0_007 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_7)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x100)   : { .inttab_tc0_008 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_8)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x120)   : { .inttab_tc0_009 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_9)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x140)   : { .inttab_tc0_00A (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_10)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x160)   : { .inttab_tc0_00B (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_11)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x180)   : { .inttab_tc0_00C (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_12)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1A0)   : { .inttab_tc0_00D (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_13)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1C0)   : { .inttab_tc0_00E (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_14)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1E0)   : { .inttab_tc0_00F (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_15)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x200)   : { .inttab_tc0_010 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_16)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x220)   : { .inttab_tc0_011 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_17)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x240)   : { .inttab_tc0_012 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_18)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x260)   : { .inttab_tc0_013 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_19)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x280)   : { .inttab_tc0_014 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_20)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x2A0)   : { .inttab_tc0_015 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_21)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x2C0)   : { .inttab_tc0_016 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_22)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x2E0)   : { .inttab_tc0_017 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_23)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x300)   : { .inttab_tc0_018 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_24)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x320)   : { .inttab_tc0_019 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_25)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x340)   : { .inttab_tc0_01A (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_26)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x360)   : { .inttab_tc0_01B (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_27)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x380)   : { .inttab_tc0_01C (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_28)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x3A0)   : { .inttab_tc0_01D (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_29)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x3C0)   : { .inttab_tc0_01E (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_30)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x3E0)   : { .inttab_tc0_01F (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_31)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x400)   : { .inttab_tc0_020 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_32)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x420)   : { .inttab_tc0_021 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_33)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x440)   : { .inttab_tc0_022 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_34)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x460)   : { .inttab_tc0_023 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_35)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x480)   : { .inttab_tc0_024 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_36)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x4A0)   : { .inttab_tc0_025 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_37)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x4C0)   : { .inttab_tc0_026 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_38)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x4E0)   : { .inttab_tc0_027 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_39)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x500)   : { .inttab_tc0_028 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_40)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x520)   : { .inttab_tc0_029 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_41)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x540)   : { .inttab_tc0_02A (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_42)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x560)   : { .inttab_tc0_02B (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_43)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x580)   : { .inttab_tc0_02C (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_44)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x5A0)   : { .inttab_tc0_02D (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_45)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x5C0)   : { .inttab_tc0_02E (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_46)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x5E0)   : { .inttab_tc0_02F (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_47)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x600)   : { .inttab_tc0_030 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_48)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x620)   : { .inttab_tc0_031 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_49)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x640)   : { .inttab_tc0_032 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_50)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x660)   : { .inttab_tc0_033 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_51)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x680)   : { .inttab_tc0_034 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_52)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x6A0)   : { .inttab_tc0_035 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_53)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x6C0)   : { .inttab_tc0_036 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_54)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x6E0)   : { .inttab_tc0_037 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_55)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x700)   : { .inttab_tc0_038 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_56)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x720)   : { .inttab_tc0_039 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_57)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x740)   : { .inttab_tc0_03A (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_58)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x760)   : { .inttab_tc0_03B (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_59)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x780)   : { .inttab_tc0_03C (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_60)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x7A0)   : { .inttab_tc0_03D (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_61)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x7C0)   : { .inttab_tc0_03E (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_62)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x7E0)   : { .inttab_tc0_03F (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_63)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x800)   : { .inttab_tc0_040 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_64)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x820)   : { .inttab_tc0_041 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_65)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x840)   : { .inttab_tc0_042 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_66)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x860)   : { .inttab_tc0_043 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_67)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x880)   : { .inttab_tc0_044 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_68)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x8A0)   : { .inttab_tc0_045 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_69)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x8C0)   : { .inttab_tc0_046 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_70)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x8E0)   : { .inttab_tc0_047 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_71)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x900)   : { .inttab_tc0_048 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_72)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x920)   : { .inttab_tc0_049 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_73)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x940)   : { .inttab_tc0_04A (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_74)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x960)   : { .inttab_tc0_04B (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_75)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x980)   : { .inttab_tc0_04C (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_76)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x9A0)   : { .inttab_tc0_04D (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_77)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x9C0)   : { .inttab_tc0_04E (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_78)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x9E0)   : { .inttab_tc0_04F (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_79)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xA00)   : { .inttab_tc0_050 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_80)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xA20)   : { .inttab_tc0_051 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_81)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xA40)   : { .inttab_tc0_052 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_82)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xA60)   : { .inttab_tc0_053 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_83)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xA80)   : { .inttab_tc0_054 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_84)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xAA0)   : { .inttab_tc0_055 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_85)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xAC0)   : { .inttab_tc0_056 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_86)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xAE0)   : { .inttab_tc0_057 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_87)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xB00)   : { .inttab_tc0_058 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_88)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xB20)   : { .inttab_tc0_059 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_89)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xB40)   : { .inttab_tc0_05A (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_90)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xB60)   : { .inttab_tc0_05B (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_91)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xB80)   : { .inttab_tc0_05C (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_92)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xBA0)   : { .inttab_tc0_05D (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_93)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xBC0)   : { .inttab_tc0_05E (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_94)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xBE0)   : { .inttab_tc0_05F (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_95)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xC00)   : { .inttab_tc0_060 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_96)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xC20)   : { .inttab_tc0_061 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_97)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xC40)   : { .inttab_tc0_062 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_98)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xC60)   : { .inttab_tc0_063 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_99)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xC80)   : { .inttab_tc0_064 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_100)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xCA0)   : { .inttab_tc0_065 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_101)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xCC0)   : { .inttab_tc0_066 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_102)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xCE0)   : { .inttab_tc0_067 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_103)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xD00)   : { .inttab_tc0_068 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_104)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xD20)   : { .inttab_tc0_069 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_105)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xD40)   : { .inttab_tc0_06A (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_106)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xD60)   : { .inttab_tc0_06B (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_107)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xD80)   : { .inttab_tc0_06C (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_108)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xDA0)   : { .inttab_tc0_06D (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_109)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xDC0)   : { .inttab_tc0_06E (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_110)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xDE0)   : { .inttab_tc0_06F (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_111)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xE00)   : { .inttab_tc0_070 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_112)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xE20)   : { .inttab_tc0_071 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_113)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xE40)   : { .inttab_tc0_072 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_114)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xE60)   : { .inttab_tc0_073 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_115)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xE80)   : { .inttab_tc0_074 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_116)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xEA0)   : { .inttab_tc0_075 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_117)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xEC0)   : { .inttab_tc0_076 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_118)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xEE0)   : { .inttab_tc0_077 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_119)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xF00)   : { .inttab_tc0_078 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_120)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xF20)   : { .inttab_tc0_079 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_121)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xF40)   : { .inttab_tc0_07A (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_122)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xF60)   : { .inttab_tc0_07B (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_123)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xF80)   : { .inttab_tc0_07C (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_124)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xFA0)   : { .inttab_tc0_07D (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_125)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xFC0)   : { .inttab_tc0_07E (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_126)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0xFE0)   : { .inttab_tc0_07F (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_127)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1000)   : { .inttab_tc0_080 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_128)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1020)   : { .inttab_tc0_081 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_129)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1040)   : { .inttab_tc0_082 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_130)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1060)   : { .inttab_tc0_083 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_131)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1080)   : { .inttab_tc0_084 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_132)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x10A0)   : { .inttab_tc0_085 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_133)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x10C0)   : { .inttab_tc0_086 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_134)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x10E0)   : { .inttab_tc0_087 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_135)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1100)   : { .inttab_tc0_088 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_136)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1120)   : { .inttab_tc0_089 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_137)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1140)   : { .inttab_tc0_08A (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_138)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1160)   : { .inttab_tc0_08B (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_139)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1180)   : { .inttab_tc0_08C (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_140)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x11A0)   : { .inttab_tc0_08D (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_141)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x11C0)   : { .inttab_tc0_08E (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_142)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x11E0)   : { .inttab_tc0_08F (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_143)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1200)   : { .inttab_tc0_090 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_144)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1220)   : { .inttab_tc0_091 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_145)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1240)   : { .inttab_tc0_092 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_146)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1260)   : { .inttab_tc0_093 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_147)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1280)   : { .inttab_tc0_094 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_148)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x12A0)   : { .inttab_tc0_095 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_149)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x12C0)   : { .inttab_tc0_096 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_150)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x12E0)   : { .inttab_tc0_097 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_151)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1300)   : { .inttab_tc0_098 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_152)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1320)   : { .inttab_tc0_099 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_153)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1340)   : { .inttab_tc0_09A (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_154)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1360)   : { .inttab_tc0_09B (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_155)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1380)   : { .inttab_tc0_09C (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_156)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x13A0)   : { .inttab_tc0_09D (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_157)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x13C0)   : { .inttab_tc0_09E (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_158)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x13E0)   : { .inttab_tc0_09F (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_159)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1400)   : { .inttab_tc0_0A0 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_160)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1420)   : { .inttab_tc0_0A1 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_161)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1440)   : { .inttab_tc0_0A2 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_162)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1460)   : { .inttab_tc0_0A3 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_163)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1480)   : { .inttab_tc0_0A4 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_164)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x14A0)   : { .inttab_tc0_0A5 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_165)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x14C0)   : { .inttab_tc0_0A6 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_166)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x14E0)   : { .inttab_tc0_0A7 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_167)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1500)   : { .inttab_tc0_0A8 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_168)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1520)   : { .inttab_tc0_0A9 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_169)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1540)   : { .inttab_tc0_0AA (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_170)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1560)   : { .inttab_tc0_0AB (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_171)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1580)   : { .inttab_tc0_0AC (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_172)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x15A0)   : { .inttab_tc0_0AD (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_173)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x15C0)   : { .inttab_tc0_0AE (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_174)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x15E0)   : { .inttab_tc0_0AF (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_175)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1600)   : { .inttab_tc0_0B0 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_176)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1620)   : { .inttab_tc0_0B1 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_177)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1640)   : { .inttab_tc0_0B2 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_178)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1660)   : { .inttab_tc0_0B3 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_179)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1680)   : { .inttab_tc0_0B4 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_180)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x16A0)   : { .inttab_tc0_0B5 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_181)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x16C0)   : { .inttab_tc0_0B6 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_182)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x16E0)   : { .inttab_tc0_0B7 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_183)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1700)   : { .inttab_tc0_0B8 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_184)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1720)   : { .inttab_tc0_0B9 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_185)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1740)   : { .inttab_tc0_0BA (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_186)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1760)   : { .inttab_tc0_0BB (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_187)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1780)   : { .inttab_tc0_0BC (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_188)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x17A0)   : { .inttab_tc0_0BD (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_189)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x17C0)   : { .inttab_tc0_0BE (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_190)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x17E0)   : { .inttab_tc0_0BF (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_191)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1800)   : { .inttab_tc0_0C0 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_192)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1820)   : { .inttab_tc0_0C1 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_193)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1840)   : { .inttab_tc0_0C2 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_194)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1860)   : { .inttab_tc0_0C3 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_195)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1880)   : { .inttab_tc0_0C4 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_196)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x18A0)   : { .inttab_tc0_0C5 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_197)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x18C0)   : { .inttab_tc0_0C6 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_198)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x18E0)   : { .inttab_tc0_0C7 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_199)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1900)   : { .inttab_tc0_0C8 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_200)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1920)   : { .inttab_tc0_0C9 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_201)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1940)   : { .inttab_tc0_0CA (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_202)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1960)   : { .inttab_tc0_0CB (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_203)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1980)   : { .inttab_tc0_0CC (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_204)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x19A0)   : { .inttab_tc0_0CD (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_205)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x19C0)   : { .inttab_tc0_0CE (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_206)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x19E0)   : { .inttab_tc0_0CF (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_207)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1A00)   : { .inttab_tc0_0D0 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_208)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1A20)   : { .inttab_tc0_0D1 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_209)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1A40)   : { .inttab_tc0_0D2 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_210)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1A60)   : { .inttab_tc0_0D3 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_211)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1A80)   : { .inttab_tc0_0D4 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_212)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1AA0)   : { .inttab_tc0_0D5 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_213)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1AC0)   : { .inttab_tc0_0D6 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_214)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1AE0)   : { .inttab_tc0_0D7 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_215)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1B00)   : { .inttab_tc0_0D8 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_216)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1B20)   : { .inttab_tc0_0D9 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_217)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1B40)   : { .inttab_tc0_0DA (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_218)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1B60)   : { .inttab_tc0_0DB (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_219)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1B80)   : { .inttab_tc0_0DC (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_220)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1BA0)   : { .inttab_tc0_0DD (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_221)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1BC0)   : { .inttab_tc0_0DE (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_222)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1BE0)   : { .inttab_tc0_0DF (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_223)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1C00)   : { .inttab_tc0_0E0 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_224)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1C20)   : { .inttab_tc0_0E1 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_225)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1C40)   : { .inttab_tc0_0E2 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_226)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1C60)   : { .inttab_tc0_0E3 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_227)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1C80)   : { .inttab_tc0_0E4 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_228)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1CA0)   : { .inttab_tc0_0E5 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_229)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1CC0)   : { .inttab_tc0_0E6 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_230)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1CE0)   : { .inttab_tc0_0E7 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_231)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1D00)   : { .inttab_tc0_0E8 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_232)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1D20)   : { .inttab_tc0_0E9 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_233)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1D40)   : { .inttab_tc0_0EA (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_234)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1D60)   : { .inttab_tc0_0EB (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_235)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1D80)   : { .inttab_tc0_0EC (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_236)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1DA0)   : { .inttab_tc0_0ED (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_237)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1DC0)   : { .inttab_tc0_0EE (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_238)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1DE0)   : { .inttab_tc0_0EF (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_239)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1E00)   : { .inttab_tc0_0F0 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_240)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1E20)   : { .inttab_tc0_0F1 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_241)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1E40)   : { .inttab_tc0_0F2 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_242)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1E60)   : { .inttab_tc0_0F3 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_243)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1E80)   : { .inttab_tc0_0F4 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_244)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1EA0)   : { .inttab_tc0_0F5 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_245)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1EC0)   : { .inttab_tc0_0F6 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_246)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1EE0)   : { .inttab_tc0_0F7 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_247)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1F00)   : { .inttab_tc0_0F8 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_248)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1F20)   : { .inttab_tc0_0F9 (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_249)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1F40)   : { .inttab_tc0_0FA (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_250)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1F60)   : { .inttab_tc0_0FB (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_251)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1F80)   : { .inttab_tc0_0FC (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_252)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1FA0)   : { .inttab_tc0_0FD (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_253)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1FC0)   : { .inttab_tc0_0FE (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_254)) }}> pfls0
	GROUP BIND(LCF_INTVEC0_START + 0x1FE0)   : { .inttab_tc0_0FF (TEXT) ALIGN(8) : { KEEP (*(.intvec_tc0_255)) }}> pfls0		

	__INTTAB_CPU0 = LCF_INTVEC0_START;
	__INTTAB_CPU1 = LCF_INTVEC0_START;
	__INTTAB_CPU2 = LCF_INTVEC0_START;
	__SP_END = __USTACK0_END;
}
	
