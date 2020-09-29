CC = g++

OPT = -O3
DEFINES = -Dprivate=public $(HAL_DEFS)

HAL_INCLUDES =  -I../Inc -I../Drivers \
-I../Drivers/STM32L4xx_HAL_Driver/Inc \
-I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy \
-I../Drivers/CMSIS/Device/ST/STM32L4xx/Include \
-I../Drivers/CMSIS/Include

HAL_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32L432xx

INCLUDES = -I . $(HAL_INCLUDES)
FLAGS = -std=c++17 -Wall -Wextra -pedantic $(DEFINES) $(INCLUDES)

TARGET = alohal_test
SOURCEDIR = .
BUILDDIR = build

SOURCES = $(wildcard $(SOURCEDIR)/t_*.cpp)
SOURCES += $(wildcard $(SOURCEDIR)/**/t_*.cpp)
OBJECTS = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

.PHONY: all binaries clean depend run
# .PRECIOUS: $(BUILD_DIR)/ $(BUILD_DIR)%/

# multi core compilation
all:
	@ make -j --no-print-directory binaries

binaries: $(TARGET)
	@ ./$(TARGET)

# Basic build folder
$(BUILDDIR)/:
	@ mkdir -p $@

# Folders inside build folder
$(BUILDDIR)/%/:
	@ mkdir -p $@

# Linking of whole target
$(TARGET): $(OBJECTS)
	@ echo LDX $<
	@ $(CC) $(FLAGS) $(OPT) $^ -o $@

# Object files compilations
.SECONDEXPANSION:
$(OBJECTS): $(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp | $$(@D)/ $(BUILDDIR)/
	@ echo CXX $<
	@ $(CC) $(FLAGS) $(OPT) -c $< -o $@

depend: .depend

# Create dependency
.depend: $(SOURCES)
	rm -rf $(BUILDDIR)/.depend
	$(CXX) $(FLAGS) -MM $^ -MF $(BUILDDIR)/.depend
	@ cat $(BUILDDIR)/.depend

clean:
	rm -rf $(BUILDDIR)
	rm -f $(TARGET)

lines:
	@ echo -n Lines:
	@ wc -l **/*.cpp **/*.hpp | tail -n 1

-include $(BUILDDIR)/.depend
