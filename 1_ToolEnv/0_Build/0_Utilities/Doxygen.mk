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

include 1_ToolEnv/0_Build/1_Config/CfgDoxygen.mk
include 1_ToolEnv/0_Build/1_Config/ConfigPrj.mk	#To get all global definitions

#macro to prepare the sed string to replace back slash and substitute double front slashs
sed_cmdstring_substslash=$(subst /,\/,$(subst \,/,$(1)))

#Doxygen output dir
DOXYGEN_OUT_DIR=3_Doc/Doxygen/Out
DOXYGEN_LOG_DIR=3_Doc/Doxygen/Log
DOXYGEN_USEROBJ_DIR=3_Doc/Doxygen/_UserFiles
#template file to generate the rules for doxygen tool
DOXYGEN_TEMPLATE_FILE=1_ToolEnv/0_Build/0_Utilities/DoxygenTemplate.txt
#generated file for the doxygen rules
DOXYGEN_RULES_FILE=$(DOXYGEN_LOG_DIR)/DoxygenRules.txt
#Framework documentation files folder
DOXYGEN_FW_FILES_PATH= $(BINUTILS_PATH)/../FrameWorkFiles/HelpGenFiles/Headers
DOXYGEN_FW_FILES_PATH:=$(call sed_cmdstring_substslash, $(DOXYGEN_FW_FILES_PATH))

#list the exclusions
DOXYGEN_EXCLUDE:=$(foreach element,$(DOXYGEN_EXCLUDE_CFG),"$(element)" )
DOXYGEN_EXCLUDE_DIR:= $(foreach element,$(DOXYGEN_EXCLUDE), $(shell find $(PROJ_DIR)/$(SRC_DIR) -iname $(element) -type d))
DOXYGEN_EXCLUDE_DIR:= $(foreach element,$(DOXYGEN_EXCLUDE_DIR), $(element)/%)
DOXYGEN_EXCLUDE_FILE:= $(foreach element,$(DOXYGEN_EXCLUDE), $(shell find $(PROJ_DIR)/$(SRC_DIR) -iname $(element) -type f))

#list all the c files in the project
C_FILES_FOR_DOXYGEN=$(shell find $(PROJ_DIR)/$(SRC_DIR) -name "*.c")
H_FILES_FOR_DOXYGEN=$(shell find $(PROJ_DIR)/$(SRC_DIR) -name "*.h") $(shell find $(DOXYGEN_FW_FILES_PATH) -name "*.h")
#remove the exclusions
C_FILES_FOR_DOXYGEN:= $(filter-out $(DOXYGEN_EXCLUDE_DIR) $(DOXYGEN_EXCLUDE_FILE),$(C_FILES_FOR_DOXYGEN))
H_FILES_FOR_DOXYGEN:= $(filter-out $(DOXYGEN_EXCLUDE_DIR) $(DOXYGEN_EXCLUDE_FILE),$(H_FILES_FOR_DOXYGEN))

#sed command preparation
SRC_FILES_SED_DOXYGEN=$(call sed_cmdstring_substslash, $(C_FILES_FOR_DOXYGEN) $(H_FILES_FOR_DOXYGEN))
SRC_FILES_SED_DOXYGEN:=$(foreach element,$(SRC_FILES_SED_DOXYGEN),$(element))

INC_FILES_SED_DOXYGEN=$(subst -I,,$(PROJ_C_INCLUDES) $(USER_C_INCLUDES))
INC_FILES_SED_DOXYGEN:=$(call sed_cmdstring_substslash, $(INC_FILES_SED_DOXYGEN))
INC_FILES_SED_DOXYGEN:=$(foreach element,$(INC_FILES_SED_DOXYGEN),$(element))

OUT_DIR_SED_DOXYGEN:=$(call sed_cmdstring_substslash,$(PROJ_DIR)/$(DOXYGEN_OUT_DIR))

STRIP_FROM_PATH_SED_DOXYGEN=$(call sed_cmdstring_substslash,$(PROJ_DIR))

HHC_LOC_SED_DOXYGEN=$(call sed_cmdstring_substslash,$(MSHEL_COMPILER))

DOT_PATH_SED_DOXYGEN=$(call sed_cmdstring_substslash,$(DOTTOOL_TOOL_PATH)) 

DOTFILE_DIRS_SED_DOXYGEN=$(call sed_cmdstring_substslash,$(PROJ_DIR)/$(DOXYGEN_OUT_DIR))

IMAGE_PATH_SED_DOXYGEN=$(call sed_cmdstring_substslash,$(PROJ_DIR)/$(DOXYGEN_USEROBJ_DIR) $(BINUTILS_PATH)/../FrameWorkFiles/HelpGenFiles/Images)  

SED_CMD_OPTN_FOR_DOXYGEN:=	-e 's/__doxygen_input_files__/$(SRC_FILES_SED_DOXYGEN)/g'				\
			   				-e 's/__doxygen_include_path__/$(INC_FILES_SED_DOXYGEN)/g'				\
			   				-e 's/__doxygen_output_path__/$(OUT_DIR_SED_DOXYGEN)/g'					\
			   				-e 's/__doxygen_strip_from_path__/$(STRIP_FROM_PATH_SED_DOXYGEN)/g'		\
			   				-e 's/__doxygen_strip_from_inc_path__/$(STRIP_FROM_PATH_SED_DOXYGEN)/g'	\
			   				-e 's/__doxygen_project_name__/$(PROJ_NAME)/g'							\
			   				-e 's/__doxygen_chm_file__/$(PROJ_NAME).chm/g'							\
			   				-e 's/__doxygen_hhc_location__/$(HHC_LOC_SED_DOXYGEN)/g'				\
			   				-e 's/__doxygen_dot_path__/$(DOT_PATH_SED_DOXYGEN)/g'					\
			   				-e 's/__doxygen_dotfile_dirs__/$(DOTFILE_DIRS_SED_DOXYGEN)/g'			\
			   				-e 's/__doxygen_imagepath__/$(IMAGE_PATH_SED_DOXYGEN)/g'

all: $(DOXYGEN_RULES_FILE)

$(DOXYGEN_RULES_FILE): $(C_FILES_FOR_DOXYGEN) $(H_FILES_FOR_DOXYGEN)
	@-rm -rf $(DOXYGEN_OUT_DIR)/*
	@mkdir -p $(DOXYGEN_OUT_DIR) $(DOXYGEN_LOG_DIR)
	@echo Doxygen configuration generated .. 
	@sed $(SED_CMD_OPTN_FOR_DOXYGEN) $(DOXYGEN_TEMPLATE_FILE) >$(DOXYGEN_RULES_FILE)
	@echo ..done
	@echo Doxygen generating documents ..
	$(DOXYGEN) $(DOXYGEN_RULES_FILE) 1>$(DOXYGEN_LOG_DIR)/Doxygen_log.txt 2>$(DOXYGEN_LOG_DIR)/Doxygen_error.txt
	@cp $(DOXYGEN_OUT_DIR)/HTML/$(PROJ_NAME).chm $(DOXYGEN_OUT_DIR) 2>>$(DOXYGEN_LOG_DIR)/Doxygen_error.txt
	@-rm -rf $(DOXYGEN_OUT_DIR)/Html
	@-rm -rf $(DOXYGEN_OUT_DIR)/Rtf 
	@echo ..done
	
clean:
	-rm -rf $(DOXYGEN_OUT_DIR)/*
	-rm -rf $(DOXYGEN_LOG_DIR)/*

