################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
aic3204_loop_linein.obj: ../aic3204_loop_linein.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5502 -g --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/emulation/boards/ezdsp5502_v1/tests/aic3204/../../C55xxCSL/include" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/emulation/boards/ezdsp5502_v1/tests/aic3204/../../include" --diag_warning=225 --ptrdiff_size=32 --memory_model=large --preproc_with_compile --preproc_dependency="aic3204_loop_linein.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

aic3204_test.obj: ../aic3204_test.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5502 -g --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/emulation/boards/ezdsp5502_v1/tests/aic3204/../../C55xxCSL/include" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/emulation/boards/ezdsp5502_v1/tests/aic3204/../../include" --diag_warning=225 --ptrdiff_size=32 --memory_model=large --preproc_with_compile --preproc_dependency="aic3204_test.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

aic3204_tone_headphone.obj: ../aic3204_tone_headphone.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5502 -g --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/emulation/boards/ezdsp5502_v1/tests/aic3204/../../C55xxCSL/include" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/emulation/boards/ezdsp5502_v1/tests/aic3204/../../include" --diag_warning=225 --ptrdiff_size=32 --memory_model=large --preproc_with_compile --preproc_dependency="aic3204_tone_headphone.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

lnkx.out: ../lnkx.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: Linker'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5502 -g --diag_warning=225 --ptrdiff_size=32 --memory_model=large -z -m"aic3204.map" --warn_sections -i"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/lib" -i"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" -i"C:/Program Files (x86)/Texas Instruments/ccsv4/emulation/boards/ezdsp5502_v1/tests/aic3204/../../lib" -i"C:/Program Files (x86)/Texas Instruments/ccsv4/emulation/boards/ezdsp5502_v1/tests/aic3204/../../C55xxCSL/lib" --reread_libs --rom_model -o "$@" "$<" "../lnkx.cmd"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5502 -g --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/emulation/boards/ezdsp5502_v1/tests/aic3204/../../C55xxCSL/include" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/emulation/boards/ezdsp5502_v1/tests/aic3204/../../include" --diag_warning=225 --ptrdiff_size=32 --memory_model=large --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


