# File: Makefile
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 26 Aug 2020
# Rev.: 27 Aug 2020
#
# Makefile for the serial boot loader running on the TI Tiva TM4C1294 Connected
# LaunchPad Evaluation Kit.
#



# ********** Check on which OS we are compiling. **********
OS       = $(shell uname -s)



# ********** Program parameters. **********
PROJECT       = boot_serial
SOURCE_FILES  = bl_user.c                   \
                bl_user_io.c                \
                bl_userhooks.c              \
                hw/gpio/gpio.c              \
                hw/gpio/gpio_led.c          \
                hw/uart/uart.c              \

HEADER_FILES  = bl_config.h                 \
                bl_user.h                   \
                bl_user_io.h                \
                bl_userhooks.h              \
                hw/gpio/gpio.h              \
                hw/gpio/gpio_led.h          \
                hw/uart/uart.h              \

EXTRA_SOURCES = bl_autobaud.c               \
                bl_can.c                    \
                bl_check.c                  \
                bl_crc32.c                  \
                bl_decrypt.c                \
                bl_emac.c                   \
                bl_flash.c                  \
                bl_i2c.c                    \
                bl_main.c                   \
                bl_packet.c                 \
                bl_ssi.c                    \
                bl_startup_${COMPILER}.c    \
                bl_uart.c                   \
                bl_usb.c                    \
                bl_usbfuncs.c               \
                uartstdio.c                 \
                ustdlib.c                   \



# ********** Additional settings. **********
BACKUP_DIR         = backup
BACKUP_FILES_SRC   = $(SOURCE_FILES) $(HEADER_FILES) Makefile
RM_FILES_CLEAN     = core *.o $(COMPILER)/*.axf $(COMPILER)/*.bin $(COMPILER)/*.d $(COMPILER)/*.o
RM_FILES_REALCLEAN = $(RM_FILES_CLEAN) $(COMPILER) *.bak *~ \
                     $(addsuffix ~, $(SOURCE_FILES)) \
                     $(addsuffix ~, $(HEADER_FILES))



# ********** TivaWare configuration. **********
# Defines the part type that this project uses.
PART=TM4C1294NCPDT

# The base directory for TivaWare.
TIVAWARE=../TivaWare/SW-TM4C-2.2.0.295

# Include the common make definitions.
include $(TIVAWARE)/makedefs

# Where to find source files that do not live in this directory.
#VPATH  = $(TIVAWARE)/utils
VPATH  = ..
VPATH += $(TIVAWARE)/boot_loader

# Where to find header files that do not live in the source directory.
IPATH  = $(TIVAWARE)
IPATH += .
IPATH += ..
IPATH += ../hw
IPATH += ../hw/gpio
IPATH += ../hw/uart

# Rules for building the project.
SCATTERgcc_$(PROJECT)=$(TIVAWARE)/boot_loader/bl_link.ld
ENTRY_$(PROJECT)=ResetISR
CFLAGSgcc=-DTARGET_IS_TM4C129_RA2



# ********** Compiler configuration. **********
CPP      = $(CC) -E
CFLAGS   += -O2 -Wall
CXXFLAGS += -O2 -Wall
LDFLAGS  +=
INCLUDES += -I.
LDLIBS   += -L.



# ********** Auxiliary programs, **********
BZIP2           = bzip2
CD              = cd
CP              = cp -a
CVS             = cvs
DATE            = date
DATE_BACKUP     = $(DATE) +"%Y-%m-%d_%H-%M-%S"
ECHO            = echo
ECHO_ERR        = $(ECHO) "**ERROR:"
EDIT			= gvim
EXIT            = exit
EXPORT          = export
FALSE           = false
GIT             = git
GREP            = grep
GZIP            = gzip
LM4FLASH        = lm4flash
LN              = ln -s
MAKE            = make
MKDIR           = mkdir
MSGVIEW         = msgview
MV              = mv
SLEEP           = sleep
SH              = sh -c
RM              = rm
TAIL            = tail -n 5
TAR             = tar
TCL             = tclsh
TEE             = tee
TOUCH           = touch
WISH            = wish



# ********** Generate object files variable. **********
OBJS := $(SOURCE_FILES:.c=.o)
OBJS += $(EXTRA_SOURCES:.c=.o)
OBJS := $(addprefix $(COMPILER)/, $(OBJS))



# ********** Generate libraries variable. **********
LIBS  = $(TIVAWARE)/driverlib/$(COMPILER)/libdriver.a



# ********** Rules. **********
.PHONY: all exec edit flash install clean real_clean mrproper mk_backup mk_backup_src $(COMPILER)

all: $(COMPILER)
all: $(COMPILER) $(COMPILER)/$(PROJECT).axf

$(COMPILER):
	@$(MKDIR) -p $(COMPILER)
	@$(MKDIR) -p $(addprefix $(COMPILER)/, $(subst ../,,$(filter-out $(TIVAWARE), $(IPATH))))

edit: $(SOURCE_FILES) $(HEADER_FILES)
	@$(EDIT) $(SOURCE_FILES) $(HEADER_FILES)

exec: install

flash: install

install: all $(COMPILER) $(COMPILER)/$(PROJECT).axf $(COMPILER)/$(PROJECT).bin
# Program the boot loader at the beginning of the flash to address 0x0.
#	@$(LM4FLASH) -E -v -S 0x0 $(COMPILER)/$(PROJECT).bin
	@$(LM4FLASH) -E -S 0x0 $(COMPILER)/$(PROJECT).bin

$(COMPILER)/$(PROJECT).axf: $(OBJS) $(LIBS)

$(OBJS): $(SOURCE_FILES) $(HEADER_FILES)

$(LIBS): $(TIVAWARE)
	@$(CD) $(TIVAWARE) && $(MAKE)

debug: clean
debug: CFLAGS+=-g -D DEBUG
debug: $(COMPILER)
debug: $(COMPILER)/$(PROJECT).axf
debug: flash
debug:
	debug/debug_nemiver.sh $(PROJECT)



# ********** Check if all necessary files and dirctories are there. **********
$(SOURCE_FILES) $(HEADER_FILES):
	@$(ECHO_ERR) "Some source files are missing!"
	@$(ECHO) "Check:"
	@$(SH) 'for source_file in $(SOURCE_FILES) $(HEADER_FILES); do \
		if [ ! -e $$source_file ]; then \
			$(ECHO) $$source_file; \
		fi; \
	done'
	@$(FALSE)

$(BACKUP_DIR):
	@$(ECHO_ERR) "Backup directory is missing!"
	@$(ECHO) "Check:"
	@$(ECHO) "$(BACKUP_DIR)"



# ********** Create backup of current state. **********
mk_backup: mk_backup_src

mk_backup_src: $(BACKUP_DIR) $(SOURCE_FILES) $(HEADER_FILES)
	@$(SH) ' \
	backup_file=$(PROJECT)_src_`$(DATE_BACKUP)`.tgz; \
	$(EXPORT) backup_file; \
	$(TAR) cfz "$(BACKUP_DIR)/$$backup_file" $(BACKUP_FILES_SRC); \
	TAR_RETURN=$$?; \
	if [ ! $$TAR_RETURN = 0 ]; then \
		$(ECHO_ERR) "Error occured backing up files."; \
	fi; \
	if [ -f $(BACKUP_DIR)/$$backup_file ]; then \
		$(ECHO) "Created source file(s) backup \"$(BACKUP_DIR)/$$backup_file\"."; \
	else \
		$(ECHO_ERR) "Cannot create \"$(BACKUP_DIR)/$$backup_file\"."; \
	fi'



# ********** Tidy up. **********
clean:
	@$(SH) 'RM_FILES="$(RM_FILES_CLEAN)"; \
		$(EXPORT) RM_FILES; \
		$(ECHO) "Removing files: \"$$RM_FILES\""; \
		$(RM) -r $$RM_FILES 2> /dev/null; \
		$(ECHO) -n'

real_clean:
	@$(SH) 'RM_FILES="$(RM_FILES_REALCLEAN)"; \
		$(EXPORT) RM_FILES; \
		$(ECHO) "Removing files: \"$$RM_FILES\""; \
		$(RM) -rf $$RM_FILES 2> /dev/null; \
		$(ECHO) -n'

mrproper: real_clean

