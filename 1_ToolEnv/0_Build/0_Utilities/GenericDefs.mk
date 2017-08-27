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

#macro to create extract only the folders which have c and assembly files in them out of several folder names
#Get_FoldersWithSRCFiles=$(strip $(sort $(dir $(shell find $(1) -name "*.c") $(shell find $(1) -name "*.asm") $(shell find $(1) -name "*.s"))))
Get_FoldersWithSRCFiles=$(or $(shell find $(1) -name "*.c"),$(shell find $(1) -name "*.asm"),$(shell find $(1) -name "*.s"))

#macro makes sed command string to convert the Tasking generated dependency files to GNU format 
TaskingDepConversionString=$(subst a,-e 's/\($(subst .,\.,$(1)).*:\)/$(subst /,\/,$(2))\/\1/g' -e 's/\\/\//g' -e '/\"/d',a)

# Automatically fetch the project name
PROJ_NAME= $(lastword $(subst /, ,$(CURDIR)))

#Source code
SRC_DIR := 0_Src
OUT_DIR := 2_Out/$(TOOL_CHAIN_MAIN)

#the directory name where config info are available #########################
MAK_CONFIG_DIR:=$(PROJ_DIR)/1_ToolEnv/0_Build/1_Config

#the directory name where all make files are generated #########################
MAK_FILE_DIR:=1_ToolEnv/0_Build/9_Make

# Make the dir structure for sub make files ####################################
PROJ_FIRST_SUBMAKE:=$(MAK_FILE_DIR)/$(SRC_DIR).mk

ifeq ($(strip $(DISPLAY_BUILD_CMD)),yes)
EXPOSE=
else
EXPOSE=@
endif

ifeq ($(strip $(ENABLE_MULTI_PROCESS)),yes)
MULTI_PROC=-j4
else
MULTI_PROC=
endif

ifeq ($(strip $(CREATE_LIB_NO_EXE)),yes)
BUILD_TARGET=Archive
else
BUILD_TARGET=ExeTargets
endif

# Defining the binary target names #############################################
ELF_BIN_MAIN := $(OUT_DIR)/$(PROJ_NAME)_$(CORETYP_MAIN).elf
HEX_BIN_MAIN := $(OUT_DIR)/$(PROJ_NAME)_$(CORETYP_MAIN).hex
LIB_OBJ_MAIN := $(OUT_DIR)/Lib$(TOOL_CHAIN_MAIN)_$(PROJ_NAME)_$(CORETYP_MAIN).a
MAP_FILE_MAIN := $(OUT_DIR)/$(PROJ_NAME)_$(CORETYP_MAIN).map
LSL_MAIN_FILE = $(PROJ_DIR)/1_ToolEnv/0_Build/1_Config/Lcf_$(TOOL_CHAIN_MAIN).lsl

#GENERATE_SEC_EXE is set to 1 at the PROJ_FIRST_SUBMAKE if there are some src files are available in sec folders
ifneq ($(GENERATE_SEC_EXE),)
ELF_BIN_SECOND := $(OUT_DIR)/$(PROJ_NAME)_$(CORETYP_SECOND).elf
HEX_BIN_SECOND := $(OUT_DIR)/$(PROJ_NAME)_$(CORETYP_SECOND).hex
LIB_OBJ_SECOND := $(OUT_DIR)/Lib$(TOOL_CHAIN_SECOND)_$(PROJ_NAME)_$(CORETYP_SECOND).a
MAP_FILE_SECOND := $(OUT_DIR)/$(PROJ_NAME)_$(CORETYP_SECOND).map
LSL_SECOND_FILE = $(PROJ_DIR)/1_ToolEnv/0_Build/1_Config/Lcf_$(TOOL_CHAIN_SECOND).lsl
endif

################# DONOT CHANGE THE FOLLOWING IN USUAL CASES ####################
# May be changed only if some BASE_xx_OPTS give error with different toolchain #
# version ######################################################################

ifeq ("$(TOOL_CHAIN_MAIN)","Gnuc")
###########################Start of Gnuc tool chain settings #######################
#Make commands Gnuc
CC:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/tricore-gcc"
AS:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/tricore-gcc"
LD:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/tricore-gcc"
AR:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/tricore-ar"
HX:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/tricore-objcopy"

# C Compiler Options
CC_OPTS= $(CC_OPTIONS_MAIN) $(PROJ_C_INCLUDES) $(USER_C_INCLUDES)

#Assembler options
ASM_OPTS= $(ASM_OPTIONS_MAIN) $(PROJ_C_INCLUDES) -x assembler-with-cpp

#Linker command options
LD_OPTS = $(LD_OPTIONS_MAIN) -Wl,-T $(LSL_MAIN_FILE) -Wl,-Map=$(MAP_FILE_MAIN) -Wl,--extmap=a

##########################End of Gnuc tool chain settings #########################
endif


ifeq ("$(TOOL_CHAIN_MAIN)","Dcc")
###########################Start of Dcc tool chain settings #######################
#Make commands Dcc
CC:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/dcc"
AS:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/das"
LD:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/dld"
AR:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/dar"
DD:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/ddump"

# C Compiler Options
CC_OPTS= $(CC_OPTIONS_MAIN) $(PROJ_C_INCLUDES) $(USER_C_INCLUDES)

#Assembler options
ASM_OPTS= $(ASM_OPTIONS_MAIN)

#Linker command options
LD_OPTS = $(LD_OPTIONS_MAIN) -m6 $(LSL_MAIN_FILE) -@O=$(MAP_FILE_MAIN)
##########################End of Dcc tool chain settings #########################
endif


ifeq ("$(TOOL_CHAIN_MAIN)","Tasking")
###########################Start of Tasking tool chain settings #######################
#Make commands Tasking
CC:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/ctc"
AS:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/astc"
LD:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/ltc"
AR:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/artc"
HX:=$(EXPOSE)"$(TOOLCHAIN_DIR_MAIN)/bin/tricore-objcopy"

# C Compiler Options
CC_OPTS = $(PROJ_C_INCLUDES) $(USER_C_INCLUDES) $(CC_OPTIONS_MAIN)

#Assembler options
ASM_OPTS:= $(ASM_OPTIONS_MAIN) $(PROJ_ASM_INCLUDES) $(USER_ASM_INCLUDES)

#Linker command options
LD_OPTS := $(LIBS) --output=$(ELF_BIN_MAIN):elf --output=$(HEX_BIN_MAIN):IHEX \
-I$(PROJ_DIR)/1_ToolEnv/0_Build/1_Config --lsl-file="1_ToolEnv/0_Build/1_Config/Lcf_Tasking.lsl" \
$(LD_OPTIONS_MAIN)

##########################End of Tasking tool chain settings ###########################
endif

#Settings for secondary core
#For the test purpose the "Gnu tool chain" name is used with dummy files NOT ACTUAL
# Name of the TOOL_CHAIN_SECOND is always fixed to ToolChain_SecCore
TOOL_CHAIN_SECOND:=ToolChain_SecCore

###########################Start of ToolChainSecCore settings #######################
CC_S=$(EXPOSE)"$(TOOLCHAIN_DIR_SECOND)/bin/tricore-gcc.exe"
AS_S=$(EXPOSE)"$(TOOLCHAIN_DIR_SECOND)/bin/tricore-as.exe"
LD_S=$(EXPOSE)"$(TOOLCHAIN_DIR_SECOND)/bin/tricore-ld.exe"
AR_S=$(EXPOSE)"$(TOOLCHAIN_DIR_SECOND)/bin/tricore-ar.exe"
HX_S=$(EXPOSE)"$(TOOLCHAIN_DIR_SECOND)/bin/tricore-objcopy.exe"

# Libraries directories
LIB_DIR_S= -L$(TOOLCHAIN_DIR_SECOND)/bin/../lib/gcc/tricore/3.4.5/tc16 \
-L$(TOOLCHAIN_DIR_SECOND)/bin/../tricore/lib/tc16

LIBS_S=

# C Compiler Options
CC_OPTS_S= $(CC_OPTIONS_SECOND) $(PROJ_C_INCLUDES) $(USER_C_INCLUDES)

#Assembler options
ASM_OPTS_S= $(ASM_OPTIONS_SECOND)

#Linker command options
LD_OPTS_S = $(LD_OPTIONS_SECOND) -T $(LSL_SECOND_FILE) -Map $(MAP_FILE_SECOND)
##########################End of ToolChainSecCore settings #########################

#Make commands Mcs assembler
GTMASM=$(EXPOSE)"1_ToolEnv/0_Build/0_Utilities/caspr-mcs.exe"


#OBJ_FILES present in the application ###
OBJ_FILES_MAIN:=
OBJ_FILES_SECOND:=
OBJ_FILES_SPECIAL:=

#Dependency files a present in the application ###
DEP_FILES_MAIN:=
DEP_FILES_SECOND:=
DEP_FILES_SPECIAL:=

# Indent, coding style format tool configuration#####################################
INDENT_TOOL_EXE=$(if $(strip $(INDENT_TOOL_PATH)),"$(INDENT_TOOL_PATH)/Indent.exe",Indent.exe)
INDENT:=$(EXPOSE)$(INDENT_TOOL_EXE)

# Doxygen Documentation format tool configuration###################################
DOXYGEN:=$(EXPOSE)"$(DOXYGEN_TOOL_PATH)/doxygen.exe"
MSHEL_COMPILER:="$(MSHELP_COMPILER_PATH)/hhc.exe"

# Make generation dependency check ###############
# If there is change in the 0_Src folder, macro will return "rmall"
# or if there is change in files at config dir, macro will return "rmall"
# else it returns nothing
#usage <var>=$(call GetVarTargetValBasedOnChngsInDir,<src folder>,<config folder>)
GetVarTargetValBasedOnChngsInDir=$(if $(shell find $(MAK_FILE_DIR) -type f -name "$(notdir $(PROJ_FIRST_SUBMAKE)")),\
$(if $(shell find $(1) -type d -newer $(PROJ_FIRST_SUBMAKE)),rmall,\
$(if $(shell find $(2) -newer $(PROJ_FIRST_SUBMAKE) -type f -name "ConfigPrj.mk"),rmall,\
$(if $(shell find $(2) -newer $(PROJ_FIRST_SUBMAKE) -type f -name "CfgCompiler_$(TOOL_CHAIN_MAIN).mk"),rmall))))
