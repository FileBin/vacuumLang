# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build

# Utility rule file for acc_gen.

# Include the progress variables for this target.
include include/llvm/Frontend/OpenACC/CMakeFiles/acc_gen.dir/progress.make

include/llvm/Frontend/OpenACC/CMakeFiles/acc_gen: include/llvm/Frontend/OpenACC/ACC.h.inc
include/llvm/Frontend/OpenACC/CMakeFiles/acc_gen: include/llvm/Frontend/OpenACC/ACC.inc


include/llvm/Frontend/OpenACC/ACC.h.inc: bin/llvm-tblgen
include/llvm/Frontend/OpenACC/ACC.h.inc: bin/llvm-tblgen
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenACC/ACC.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/CodeGen/SDNodeProperties.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/CodeGen/ValueTypes.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/Directive/DirectiveBase.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenACC/ACC.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenMP/OMP.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/Attributes.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/AttributesAMDGPU.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/Intrinsics.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsAArch64.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsAMDGPU.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsARM.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsBPF.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsHexagon.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsHexagonDep.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsMips.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsNVVM.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsPowerPC.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsRISCV.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsSystemZ.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsVE.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsVEVL.gen.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsWebAssembly.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsX86.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsXCore.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Option/OptParser.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/TableGen/Automaton.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/TableGen/SearchableTable.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/GenericOpcodes.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/GlobalISel/Combine.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/GlobalISel/RegisterBank.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/GlobalISel/SelectionDAGCompat.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/GlobalISel/Target.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/Target.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/TargetCallingConv.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/TargetInstrPredicate.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/TargetItinerary.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/TargetPfmCounters.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/TargetSchedule.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/TargetSelectionDAG.td
include/llvm/Frontend/OpenACC/ACC.h.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenACC/ACC.td
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building ACC.h.inc..."
	cd /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build/include/llvm/Frontend/OpenACC && ../../../../bin/llvm-tblgen --gen-directive-decl -I /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenACC -I/mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build/include -I/mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenACC/ACC.td --write-if-changed -o /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build/include/llvm/Frontend/OpenACC/ACC.h.inc

include/llvm/Frontend/OpenACC/ACC.inc: bin/llvm-tblgen
include/llvm/Frontend/OpenACC/ACC.inc: bin/llvm-tblgen
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenACC/ACC.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/CodeGen/SDNodeProperties.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/CodeGen/ValueTypes.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/Directive/DirectiveBase.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenACC/ACC.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenMP/OMP.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/Attributes.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/AttributesAMDGPU.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/Intrinsics.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsAArch64.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsAMDGPU.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsARM.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsBPF.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsHexagon.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsHexagonDep.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsMips.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsNVVM.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsPowerPC.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsRISCV.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsSystemZ.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsVE.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsVEVL.gen.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsWebAssembly.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsX86.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/IR/IntrinsicsXCore.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Option/OptParser.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/TableGen/Automaton.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/TableGen/SearchableTable.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/GenericOpcodes.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/GlobalISel/Combine.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/GlobalISel/RegisterBank.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/GlobalISel/SelectionDAGCompat.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/GlobalISel/Target.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/Target.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/TargetCallingConv.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/TargetInstrPredicate.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/TargetItinerary.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/TargetPfmCounters.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/TargetSchedule.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Target/TargetSelectionDAG.td
include/llvm/Frontend/OpenACC/ACC.inc: /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenACC/ACC.td
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building ACC.inc..."
	cd /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build/include/llvm/Frontend/OpenACC && ../../../../bin/llvm-tblgen --gen-directive-impl -I /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenACC -I/mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build/include -I/mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenACC/ACC.td --write-if-changed -o /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build/include/llvm/Frontend/OpenACC/ACC.inc

acc_gen: include/llvm/Frontend/OpenACC/CMakeFiles/acc_gen
acc_gen: include/llvm/Frontend/OpenACC/ACC.h.inc
acc_gen: include/llvm/Frontend/OpenACC/ACC.inc
acc_gen: include/llvm/Frontend/OpenACC/CMakeFiles/acc_gen.dir/build.make

.PHONY : acc_gen

# Rule to build all files generated by this target.
include/llvm/Frontend/OpenACC/CMakeFiles/acc_gen.dir/build: acc_gen

.PHONY : include/llvm/Frontend/OpenACC/CMakeFiles/acc_gen.dir/build

include/llvm/Frontend/OpenACC/CMakeFiles/acc_gen.dir/clean:
	cd /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build/include/llvm/Frontend/OpenACC && $(CMAKE_COMMAND) -P CMakeFiles/acc_gen.dir/cmake_clean.cmake
.PHONY : include/llvm/Frontend/OpenACC/CMakeFiles/acc_gen.dir/clean

include/llvm/Frontend/OpenACC/CMakeFiles/acc_gen.dir/depend:
	cd /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/llvm/include/llvm/Frontend/OpenACC /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build/include/llvm/Frontend/OpenACC /mnt/c/Users/FileBinsLapTop/Documents/Projects/VacuumLang/llvm-project/build/include/llvm/Frontend/OpenACC/CMakeFiles/acc_gen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : include/llvm/Frontend/OpenACC/CMakeFiles/acc_gen.dir/depend

