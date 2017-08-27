###############################################################################
#                                                                             #
#        Copyright © 2011 Infineon Technologies AG. All rights reserved.      #
#                                                                             #
#                                                                             #
#                              IMPORTANT NOTICE                               #
#                                                                             #
#                                                                             #
# Infineon Technologies AG (Infineon) is supplying this file for use          #
# exclusively with Infineon’s microcontroller products. This file can be      #
# freely distributed within development tools that are supporting such        #
# microcontroller products.                                                   #
#                                                                             #
# THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED #
# OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF          #
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.#
# INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,#
# OR CONSEQUENTIAL DAMAGES, FOR	ANY REASON WHATSOEVER.                        #
#                                                                             #
###############################################################################
# Subdir make file for 0_Src
# This is for core type MAIN and Tasking compiler !!!!
###############################################################################

include 1_ToolEnv/0_Build/1_Config/CfgCompiler_Tasking.mk								
include 1_ToolEnv/0_Build/1_Config/ConfigPrj.mk			
														
														
PROJ_C_INCLUDES:= -I./0_Src/0_AppSw -I./0_Src/0_AppSw/Config -I./0_Src/0_AppSw/Config/Common -I./0_Src/0_AppSw/Tricore -I./0_Src/1_SrvSw -I./0_Src/1_SrvSw/Tricore -I./0_Src/1_SrvSw/Tricore/Compilers -I./0_Src/1_SrvSw/_Utilities -I./0_Src/1_SrvSw/_Utilities -I./0_Src/4_McHal -I./0_Src/4_McHal/Tricore -I./0_Src/4_McHal/Tricore/Cpu/CStart -I./0_Src/4_McHal/Tricore/Cpu/Irq -I./0_Src/4_McHal/Tricore/Cpu/Std -I./0_Src/4_McHal/Tricore/Cpu/Trap -I./0_Src/4_McHal/Tricore/Dma/Dma -I./0_Src/4_McHal/Tricore/Dma/Std -I./0_Src/4_McHal/Tricore/Port/Std -I./0_Src/4_McHal/Tricore/Scu/Std -I./0_Src/4_McHal/Tricore/Src/Std -I./0_Src/4_McHal/Tricore/Stm/Std -I./0_Src/4_McHal/Tricore/_Impl -I./0_Src/4_McHal/Tricore/_Reg 					
PROJ_ASM_INCLUDES:=  				

include 1_ToolEnv/0_Build/9_Make/0_AppSw/0_AppSw.mk 1_ToolEnv/0_Build/9_Make/1_SrvSw/1_SrvSw.mk 1_ToolEnv/0_Build/9_Make/4_McHal/4_McHal.mk  

# current directory relative to project
L_DIR:=0_Src								
																									
PROJ_OBJS_MAIN:=  																	
																									
.PHONY: All_Targets																					
																									
ExeTargets:$(ELF_BIN_MAIN) $(HEX_BIN_MAIN) $(ELF_BIN_SECOND) $(HEX_BIN_SECOND)						
	@echo '...Done'																					
																									
$(ELF_BIN_MAIN) $(HEX_BIN_MAIN):$(OBJ_FILES_SPECIAL) $(OBJ_FILES_MAIN) $(PROJ_OBJS_MAIN) $(LSL_MAIN_FILE) 
	@echo 'Building target: $@'																		
	$(LD) $(OBJ_FILES_MAIN) $(OBJ_FILES_SPECIAL) $(PROJ_OBJS_MAIN) $(LD_OPTS) -L$(LIB_DIR)	
	@echo ' '																																													
																									
Archive: $(LIB_OBJ_MAIN) $(LIB_OBJ_SECOND)															
																									
$(LIB_OBJ_MAIN): $(OBJ_FILES_SPECIAL) $(OBJ_FILES_MAIN) $(PROJ_OBJS_MAIN)				
	@echo 'Building library: $@'														
	$(AR) -rc $@ $(OBJ_FILES_SPECIAL) $(OBJ_FILES_MAIN) $(PROJ_OBJS_MAIN)				


-include $(DEP_FILES_MAIN)																			
-include $(DEP_FILES_SPECIAL)																		

