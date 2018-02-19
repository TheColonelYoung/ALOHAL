#!/bin/bash

import subprocess
import os
from pathlib import Path

makefile = open("../Makefile", "r+")
text = makefile.read()
makefile.close()

# Check if Makefile was modified
if text.find("# Modified for C++ by TCY") != -1:
    print("This Makefile was already mofified")
    quit()

# Get rid off the windows end of lines
text.replace(r"\r\n", r"\n")

# Insert mofified tag
print(text.find("# Generic Makefile (based on gcc)"))
position = (text.find("# Generic Makefile (based on gcc)")) + len("# Generic Makefile (based on gcc)") + 1
text = text[:position] + "# Modified for C++ by TCY\n" + text[position:]

# Compile params
text = text.replace("OPT = -Og", "OPT = -Os")

position = (text.find("# ASM sources")) - 1
text = text[:position] + "\nCPP_SOURCES =  \\\n\n" + text[position:]

text = text.replace("$(BINPATH)/", "")
text = text.replace("gcc", "g++")

# ALOHAL DEFINE OF STM FAMILY
position = (text.find("-DSTM32F"))
define = "-DSTM32F" + text[position + len("-DSTM32F")] + " \\\n"
position = (text.find("C_DEFS =  \\")) + len("C_DEFS =  \\") + 1
text = text[:position] + define + text[position:]

# ALOHAL INCLUDE ALOHAL
position = (text.find("C_INCLUDES =  \\")) + len("C_INCLUDES =  \\") + 1
text = text[:position] + "-IALOHAL \\\n" + text[position:]

# Compiling
position = (text.find("CFLAGS = $(MCU)")) + len("CFLAGS = $(MCU)") + 1
text = text[:position] + "-std=c++11 -Wno-write-strings -specs=nano.specs -specs=nosys.specs " + text[position:]

position = (text.find("LDFLAGS = $(MCU)")) + len("LDFLAGS = $(MCU)") + 1
text = text[:position] + "-Wl,--no-wchar-size-warning -specs=nosys.specs " + text[position:]

position = (text.find("# list of ASM program objects")) - 1
text = text[:position] + "\n\nOBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))\nvpath %.cpp $(sort $(dir $(CPP_SOURCES)))\n" + text[position:]

position = (text.find("$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)")) - 1
text = text[:position] + "\n$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)\n\t$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@\n" + text[position:]

# Prog - STM32_Programmer_CLI must be installed
home = str(Path.home())
result = subprocess.run(['find', home, '-name', 'STM32_Programmer_CLI'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
path = result.stdout.decode("utf8")[:-1]

if path:
    position = (text.find("# *** EOF ***")) - 1
    text = text[:position] + "\n# Flash\nprog:\n\t" + path + " -c port=SWD reset=HWrst -w build/$(TARGET).bin 0x08000000 -v -rst\n" + text[position:]
else:
    print("Install STM32 Programmer CLI for Flash firmware")

makefile = open("../Makefile", "w")
makefile.write(text)
makefile.close()
