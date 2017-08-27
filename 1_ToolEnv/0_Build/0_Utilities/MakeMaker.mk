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

MAKE_CONFIG_DIR=1_ToolEnv/0_Build/1_Config
include $(MAKE_CONFIG_DIR)/ConfigPrj.mk	#To get all global definitions

#the directory name where required tools to generate make are available
MAK_UTILITY_DIR:=$(PROJ_DIR)/1_ToolEnv/0_Build/0_Utilities

## Part 1: Macro definitions which are used in the make file ###############################

#Macro to create the make file names out of dir structures
#The name created will be concatenation of the first level folder under 0_Src and the Leaf folder
#If the folder structure itself is first level under 0_SRC, the concatenation is not required
Get_MakeFileName=$(if $(word 2,$(subst /, ,$(1))),$(subst  ,,$(firstword $(subst /, ,$(1)))_$(lastword $(subst /, ,$(1)))).mk,$(lastword $(subst /, ,$(1))).mk)

#macro to substitute another make for user exceptions
#replace the paths of automatically specified make files to user make file name and path to config dir
#if no argument given for to, then the whole path for that make is removed
#usage:- $(call SubtstituteExceptionMakes,[from Make filenames],[to Make files],[in make files with proj relative paths])
SubtstituteExceptionMakes=$(filter-out $(foreach exceptions,$(1),%/$(exceptions)),$(3)) \
							$(foreach exceptions,$(filter $(2), $(notdir $(3))),$(MAKE_CONFIG_DIR)/$(exceptions))
							
#Select the core type for the current dir
TEMP_FOLDER_NAMES:=$(subst /, ,$(subst $(PROJ_DIR)/,,$(THIS_DIR_NAME)))
ifneq ($(filter $(CORETYP_SECOND),$(TEMP_FOLDER_NAMES)),)
CORE_TYPE:=SECOND
TEMPLT_MAKE_FILE:= $(subst  ,,$(MAK_UTILITY_DIR)/Maketemplate_Second.txt)
TOOL_CHAIN:=$(TOOL_CHAIN_SECOND)
else 
ifneq ($(filter $(CORETYP_SPECIAL),$(TEMP_FOLDER_NAMES)),)
CORE_TYPE:=SPECIAL
TEMPLT_MAKE_FILE:= $(subst  ,,$(MAK_UTILITY_DIR)/Maketemplate_Special.txt)
TOOL_CHAIN:=$(TOOL_CHAIN_SPECIAL)
else
CORE_TYPE:=MAIN
TEMPLT_MAKE_FILE:= $(subst  ,,$(MAK_UTILITY_DIR)/Maketemplate_Main.txt)
TOOL_CHAIN:=$(TOOL_CHAIN_MAIN)
endif
endif

INC_TOOLCHN_CFGFILE:=include $(subst /,\/,$(subst  ,,$(MAKE_CONFIG_DIR)/CfgCompiler_$(TOOL_CHAIN).mk))
INC_TOOLCHNSEC_CFGFILE:=include $(subst /,\/,$(subst  ,,$(MAKE_CONFIG_DIR)/Cfg$(TOOL_CHAIN_SECOND).mk))

## Part 2: Prepare all the names required to substitute in to the template ##################

# cfile names in the directory for which this make is called.
# These contents will replace the label __cfiles__ in the template make file
L_C_FILES:=$(shell find $(THIS_DIR_NAME) -maxdepth 1 -name "*.c")
L_O_FM_C_FILES:=$(L_C_FILES:%.c=$(OUT_DIR)/%.o)
L_D_FM_C_FILES:=$(L_C_FILES:%.c=$(OUT_DIR)/%.d)
L_C_FILES:=$(subst /,\/,$(subst $(PROJ_DIR)/,,$(L_C_FILES)))          			#the substitution of \/ is required to be used with 'sed' application
L_O_FM_C_FILES:=$(subst /,\/,$(subst $(PROJ_DIR)/,,$(L_O_FM_C_FILES)))
L_D_FM_C_FILES:=$(subst /,\/,$(subst $(PROJ_DIR)/,,$(L_D_FM_C_FILES)))

# assembly file names in the directory for which this make is called.
# These contents will replace the label __asmfiles__ in the template make file
L_ASM_FILES:=$(shell find $(THIS_DIR_NAME) -maxdepth 1 -name "*.asm")
L_S_FILES:=$(shell find $(THIS_DIR_NAME) -maxdepth 1 -name "*.s")
L_O_FM_S_FILES:=$(L_ASM_FILES:%.asm=$(OUT_DIR)/%.o) $(strip $(L_S_FILES:%.s=$(OUT_DIR)/%.o))
L_D_FM_S_FILES:=$(L_ASM_FILES:%.asm=$(OUT_DIR)/%.d) $(strip $(L_S_FILES:%.s=$(OUT_DIR)/%.d))
L_ASM_FILES:=$(subst /,\/,$(subst $(PROJ_DIR)/,,$(L_ASM_FILES) $(L_S_FILES)))	#the substitution of \/ is required to be used with 'sed' application
L_O_FM_S_FILES:=$(subst /,\/,$(subst $(PROJ_DIR)/,,$(L_O_FM_S_FILES)))			#the substitution of \/ is required to be used with 'sed' application
L_D_FM_S_FILES:=$(subst /,\/,$(subst $(PROJ_DIR)/,,$(L_D_FM_S_FILES)))			#the substitution of \/ is required to be used with 'sed' application

# Extract the folder names and the target for next level
# The folder names will be used to include in the generated make file and in the next recursion
# the same make files will be generated
FOLDRS_NEXT_LVL:=$(sort $(shell find $(THIS_DIR_NAME) -maxdepth 1 -mindepth 1 -type d))

# Check if there are some .c files with in and levels below THIS_DIR_NAME
FOLDRS_NEXT_LVL:=$(foreach folder, $(FOLDRS_NEXT_LVL), \
					$(if $(call Get_FoldersWithSRCFiles,$(folder)),$(folder)))	#select only those folders which have c files in their folder structure
FOLDRS_NEXT_LVL:= $(foreach folder, $(FOLDRS_NEXT_LVL), \
					$(subst $(strip $(PROJ_DIR)/$(SRC_DIR)/),,$(folder)))		#Make this as absolute dir for subsequent recursive make calls

#Create the make file names to be substituted at __include_make_files__ in to template make file
#The name created will be concatenation of the first level folder under 0_Src and the Leaf folder
TEMP_MAKE_TARGTS:=$(foreach folder, $(FOLDRS_NEXT_LVL),$(folder)/$(call Get_MakeFileName,$(folder)))
#Get the targets for next sub-make directories The include is generated only if next level folders are present
TEMP_MAKE_TARGTS:=$(TEMP_MAKE_TARGTS:%.mk=$(MAK_FILE_DIR)/%.mk)
TEMP_MAKE_TARGTS:=$(call SubtstituteExceptionMakes,$(DIFFERED_FROM_AUTOMAKES),$(DIFFERED_FROM_AUTOMAKES),$(TEMP_MAKE_TARGTS)) #insert differed makes if any
#add the include string only if there is something to include as next submakes

#NAMES_OF_SUBMKS will be actually substituted to the __include_make_files__
ifneq ($(strip $(TEMP_MAKE_TARGTS)),)
NAMES_OF_SUBMKS:=include $(subst /,\/,$(TEMP_MAKE_TARGTS))
else
NAMES_OF_SUBMKS:=
endif

# CURNT_FLDR_R_NAME contains the proj relative path of this folder
# This content will be substituted at the __sub_directory__ in template make file
CURNT_FLDR_R_NAME:= $(subst $(PROJ_DIR)/,,$(THIS_DIR_NAME))
# substitue the escape char for / to use CURNT_FLDR_R_NAME inside sed command
CURNT_FLDR_R_NAME:= $(subst /,\/,$(CURNT_FLDR_R_NAME))

#find if there are some files in secondary core type and if present set global option in 0_src make to compile for these
TEMP_ELF_SECOND_NEEDED:=$(findstring $(CORETYP_SECOND),$(subst $(PROJ_DIR)/$(SRC_DIR),,$(call Get_FoldersWithSRCFiles,$(PROJ_DIR)/$(SRC_DIR))))

#create a string for substitution in template file
SED_CMD_OPTN:= $(if $(strip $(L_C_FILES)),,-e '/__This_is_line_for_c_files__/d')\
			   $(if $(strip $(L_ASM_FILES)),,-e '/__This_is_line_for_asm_files__/d')\
			   $(if $(TEMP_ELF_SECOND_NEEDED),,-e '/__This_is_for_SecTyp_only__/d')\
			   -e 's/__cfiles__/$(L_C_FILES)/g'					\
			   -e 's/__o_fm_c_files__/$(L_O_FM_C_FILES)/g'	\
			   -e 's/__dep_fm_c_files__/$(L_D_FM_C_FILES)/g'	\
			   -e 's/__asmfiles__/$(L_ASM_FILES)/g'				\
			   -e 's/__o_fm_s_files__/$(L_O_FM_S_FILES)/g' \
			   -e 's/__dep_fm_s_files__/$(L_D_FM_S_FILES)/g'	\
			   -e 's/__core_type__/$(CORE_TYPE)/g' 				\
			   -e 's/__sub_directory__/$(CURNT_FLDR_R_NAME)/g'	\
			   -e 's/__include_make_files__/$(NAMES_OF_SUBMKS)/g' \
			   -e 's/__include_toolchain_cfg__/$(INC_TOOLCHN_CFGFILE)/g'\
			   -e 's/__This_is_line_for_c_files__//g' 			\
			   -e 's/__This_is_line_for_asm_files__//g'			\
			   -e 's/__This_is_line_for_0_Src_Only__//g'		\
			   -e 's/__This_is_for_SecTyp_only__//g'

# SED_CMD_OPTN_FOR_TOOLCHAIN contains the sed command options to select the command lines for particulat tool chain
SED_CMD_OPTN_FOR_TOOLCHAIN:= -e '/__Tasking_cmd__/d' -e '/__Gnuc_cmd__/d' -e '/__Dcc_cmd__/d' -e '/__ToolChain_SecCore_cmd__/d'
REMOVING_SED_SEQ= $(subst  ,,__$(TOOL_CHAIN)_cmd__)
 
SED_CMD_OPTN_FOR_TOOLCHAIN:= $(subst $(REMOVING_SED_SEQ),__something_else__,$(SED_CMD_OPTN_FOR_TOOLCHAIN)) \
			   					-e 's/__toolchain__/$(TOOL_CHAIN)/g'	\
			   					-e 's/__Tasking_cmd__//g'				\
			   					-e 's/__Gnuc_cmd__//g'					\
			   					-e 's/__Dcc_cmd__//g'					\
			   					-e 's/__ToolChain_SecCore_cmd__//g'

# Part 3: Generating the make files ###########################################################

#If user defined makes are available no need of generating thes files now. But subfolders will still be generated if they are not excluded
NEXT_MAKE_TARGTS:=$(call SubtstituteExceptionMakes,$(DIFFERED_FROM_AUTOMAKES),,$(TEMP_MAKE_TARGTS)) #remove all the differed makes in target


#Get the make file name which will be create now this will generate the make file with the macro if current target is not for $(SRC_DIR)
ifeq ($(lastword $(subst /, ,$(THIS_DIR_NAME))),$(SRC_DIR))
CURNT_MAKE_TARGET:= $(lastword $(subst /, ,$(THIS_DIR_NAME))).mk

# the following make statement calls the shell command to get all *.h files in
# current directory then gets only the driectory name out of it
SED_C_INCLUDES :=$(sort $(shell find $(PROJ_DIR)/$(SRC_DIR) -maxdepth 2 -mindepth 1 -type d))
SED_C_INCLUDES :=$(subst / , ,$(sort $(dir $(shell find $(PROJ_DIR)/$(SRC_DIR) -name "*.h")) $(SED_C_INCLUDES)) )
SED_C_INCLUDES :=$(subst $(PROJ_DIR),.,$(SED_C_INCLUDES))
SED_C_INCLUDES :=$(subst /,\/,$(foreach Includes,$(SED_C_INCLUDES),-I$(Includes))) #the substitution of \/ is required to be used with 'sed' application
SED_ASM_INCLUDES :=$(subst / , ,$(sort $(dir $(shell find $(PROJ_DIR)/$(SRC_DIR) -name "*.inc"))) )
SED_ASM_INCLUDES :=$(subst /,\/,$(foreach Includes,$(SED_ASM_INCLUDES),-I$(Includes))) #the substitution of \/ is required to be used with 'sed' application

# the following make statement calls the shell command to get all *.a/lib files in
# current directory then gets only the driectory name out of it

TEMP_PROJ_LIBS := $(subst $(PROJ_DIR)/,,$(shell find $(PROJ_DIR)/$(SRC_DIR) -name "*.a"))
TEMP_PROJ_LIBS += $(subst $(PROJ_DIR)/,,$(shell find $(PROJ_DIR)/$(SRC_DIR) -name "*.lib"))

# PROJ_OBJS contains all project defined specific obj files and the libraries
TEMP_PROJ_OBJS := $(subst $(PROJ_DIR)/,,$(shell find $(PROJ_DIR)/$(SRC_DIR) -name "*.o"))
TEMP_PROJ_OBJS += $(TEMP_PROJ_LIBS)

#Find if there is some lib/obj file present with second core type then exclude that for PROJ_OBJS_MAIN
SED_OBJS_MAIN:=$(subst /,\/,$(foreach obj,$(TEMP_PROJ_OBJS),$(if $(findstring $(CORETYP_SECOND),$(obj)),,$(obj)))) #the substitution of \/ is required for'sed'
#Find if there is some lib/obj file present with second core type then include that for PROJ_OBJS_SECOND
SED_OBJS_SECOND:=$(subst /,\/,$(foreach obj,$(TEMP_PROJ_OBJS),$(if $(findstring $(CORETYP_SECOND),$(obj)),$(obj),))) #the substitution of \/ is required for'sed'

REMOVING_SED_SEQ:=-e '/$(subst  ,,__$(TOOL_CHAIN_SECOND)_cmd__)/d'
SED_CMD_OPTN_FOR_TOOLCHAIN:=$(subst $(REMOVING_SED_SEQ),,$(SED_CMD_OPTN_FOR_TOOLCHAIN)) \
							-e 's/__include_toolchainsec_cfg__/$(INC_TOOLCHNSEC_CFGFILE)/g'\
							-e 's/__proj_objs_main__/$(SED_OBJS_MAIN)/g'\
							-e 's/__proj_objs_second__/$(SED_OBJS_SECOND)/g'\
							-e 's/__proj_c_includes__/$(SED_C_INCLUDES)/g'\
							-e 's/__proj_asm_includes__/$(SED_ASM_INCLUDES)/g'
else 
CURNT_MAKE_TARGET:= $(call Get_MakeFileName,$(subst $(strip $(PROJ_DIR)/$(SRC_DIR)/),,$(THIS_DIR_NAME)))
SED_CMD_OPTN:= -e '/__This_is_line_for_0_Src_Only__/d' $(SED_CMD_OPTN)
endif

# Get the relative path name of current folder.
# LOC_DIR_PATH has the path name of the folder for which this make is called
# The path name till ..0_SRC/ is removed. This will not generate an extra 0_SRC folder in make folders
LOC_DIR_PATH:= $(subst $(PROJ_DIR)/$(SRC_DIR),,$(subst $(PROJ_DIR)/$(SRC_DIR)/,,$(THIS_DIR_NAME)))

# add the remaining part of absolute path for the CURNT_MAKE_TARGET
CURNT_MAKE_TARGET:=$(MAK_FILE_DIR)/$(if $(strip $(LOC_DIR_PATH)),$(LOC_DIR_PATH)/$(CURNT_MAKE_TARGET),$(CURNT_MAKE_TARGET))
CURNT_MAKE_TARGET:=$(subst  ,,$(CURNT_MAKE_TARGET))


#all: $(TEMPLT_MAKE_FILE)
#	@echo Generating the make file for: $(CURNT_MAKE_TARGET) $(NEXT_MAKE_TARGTS)

$(CURNT_MAKE_TARGET): $(TEMPLT_MAKE_FILE) $(NEXT_MAKE_TARGTS)
	@echo Generating the make file for: $(@F)
	@mkdir -p $(@D)										#create the directory for the submakes
	@sed  $(SED_CMD_OPTN) $(SED_CMD_OPTN_FOR_TOOLCHAIN) $< >$@
	@echo

$(NEXT_MAKE_TARGTS): $(TEMPLT_MAKE_FILE)
	@$(MAKE) $(MULTI_PROC) -f $(MAK_UTILITY_DIR)/MakeMaker.mk THIS_DIR_NAME=$(subst $(MAK_FILE_DIR),$(PROJ_DIR)/0_Src,$(@D))
