# warn if format string is a literal
warnings := -Wall -Wextra -Wunused -Wformat=2 -Winit-self -Wmissing-include-dirs -Wstrict-overflow=4 -Wfloat-equal -Wreorder -Wsign-promo -Wwrite-strings -Wconversion -Wlogical-op -Wundef -Wunsafe-loop-optimizations -Wtrigraphs -Wunused-parameter -Wunknown-pragmas -Wcast-align -Wswitch-default -Wswitch-enum -Wmissing-noreturn -Wmissing-format-attribute -Wpacked -Wredundant-decls -Wunreachable-code -Winline -Winvalid-pch -Wmissing-declarations -Wdisabled-optimization -Wstack-protector -Woverloaded-virtual -Wsign-promo -Woverloaded-virtual -Wold-style-cast -Wstrict-null-sentinel -Wunused-macros -Wno-pmf-conversions
# -Weffc++
#-Waggregate-return : aggrevating is more the term.
osdevops :=-nostdinc++	-nostdinc -nostartfiles	-fno-exceptions	-fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -fno-rtti

experimentalops := -fextended-identifiers

assembly_output := #-fverbose-asm -save-temps #-masm=intel

dangerous_o_flags := -fmerge-all-constants -funsafe-loop-optimizations -fsched-spec-load-dangerous -fipa-struct-reorg -fipa-matrix-reorg -ffast-math -funsafe-math-optimizations -ffinite-math-only -frename-registers -fomit-frame-pointer # -flto # -fwhole-program #-flto

# -flto == has not been enabled in this configuration (looks cool, I want)

# -floop-interchange -fgraphite-identity -floop-parallelize-all -floop-block === This optimization applies to all the languages supported by GCC and is not limited to Fortran. To use this code transformation, GCC has to be configured with --with-ppl and --with-cloog to enable the Graphite loop transformation infrastructure.
gcc46 := -ftree-bit-ccp

more := -fforward-propagate -fregmove -fno-peephole2  -finline-small-functions


fthings := -Ofast -fstrict-aliasing -fno-rtti -fweb  -fmodulo-sched -fmodulo-sched-allow-regmoves -fgcse-sm -fgcse-las  -fgcse-after-reload -fsched-pressure -fsched2-use-superblocks -fschedule-insns2  -fipa-pta -ftree-loop-linear  -ftree-loop-distribution -ftree-loop-im -ftree-loop-ivcanon -fivopts -fvect-cost-model -ftracer -fvariable-expansion-in-unroller -freorder-blocks-and-partition -falign-jumps -fbranch-target-load-optimize2 ${dangerous_o_flags} ${gcc46} ${more} ${assembly_output} # -fkeep-inline-functions


#-fwhole-program == problems
# ???? -ftree-loop-if-convert
# Compiling C++ no matter what the file extensions are when invoking gcc.
cxx_selection := -std=gnu++0x -x c++

includes := -I./src

ignore_define := -U i386


#-ftrapv can't use yet because some internal funcctions are not defined.
CODEGEN := -freg-struct-return -fno-common -frecord-gcc-switches

args := ${warnings} ${fthings} ${osdevops} ${experimentalops} \
	 -Wwrite-strings ${includes} ${cxx_selection} -m32 ${ignore_define} \
	 ${CODEGEN} -pipe

# -fforward-propagate -fregmove -fno-peephole2  -finline-small-functions

CXX ?= g++
CXX_CHECK_SYNTAX := /usr/x86_64-pc-linux-gnu/i686-pc-linux-gnu/gcc-bin/4.5.0/i686-pc-linux-gnu-gcc

GRUB_STAGE2 := stage2_eltorito


# CXXOPS
# LDFLAGS
# non source files.
AUXFILES := Makefile

PROJDIRS := src
SRCFILES := $(shell find "src" -name "*.cpp")
ASMFILES := $(shell find "src" -name "*.asm")
HDRFILES := $(shell find "src" -name "*.h")

OBJFILES := $(patsubst %.cpp,%.o,$(SRCFILES))
AOBJFILES := $(patsubst %.asm,%.o,$(ASMFILES))
DEPFILES := $(patsubst %.cpp,%.d,$(SRCFILES))

BUILD_TYPE := debug

all: nop.bin

nop.bin: $(OBJFILES) ${AOBJFILES}
	@nasm -f elf -o loader.o loader.s
	@${CXX} ${LDFLAGS} -Wl,-melf_i386 -nostdlib -T linker.ld -o nop.bin loader.o ${OBJFILES} ${AOBJFILES}
#	@echo "Done! Linked the following into nop.bin:" ${OBJFILES}

%.o: %.cpp Makefile
	@$(CXX) $(args) -m32 -MMD -MP -MT "$*.d $*.o"  -c $< -o $@
# For looking at assembly with objdump if wanted.
# -Wa,-adhln -g
#	@echo "Compiled" $<

%.o: %.asm Makefile
	@nasm -f elf -o $@ $<

floppy:
	dd if=/dev/zero of=pad bs=1 count=750
	cat /boot/grub/stage1 /boot/grub/stage2 pad nop.bin > floppy.img

check-syntax:
	${CXX_CHECK_SYNTAX} ${args}  -o /dev/null -c ${CHK_SOURCES}

clean:
	@$(RM) $(wildcard $(OBJFILES) $(DEPFILES) $(REGFILES) \
		nop.bin nop.iso)
	@find -name '*_flymake*' -delete

grub:
	@mkdir -p isofiles/boot/grub
	@cp ${GRUB_STAGE2} isofiles/boot/grub/
	@cp nop.bin isofiles/boot

	@genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4                 -boot-info-table --input-charset utf-8 -o nop.iso isofiles

qemu: grub
	@qemu-system-i386 -cpu core2duo -m 32M -usb -serial 'file:nop.out' -name 'nop' -monitor stdio -daemonize -no-reboot -no-shutdown -smp 16 -cdrom nop.iso

qemu-no-ui: grub
	@qemu-system-i386 -cpu core2duo -m 32M -usb -serial 'file:nop.out' -name 'nop' -monitor null -daemonize -no-reboot -nographic -cdrom nop.iso 
	@sleep 6
	@pkill -f qemu-system-i386
	@echo "============     NOP OUT    ============="
	@cat nop.out
	@echo "\n==============---------------==================="
#-pidfile 'nop-qemu.pid'
#-mem-path '/tmp/nop-ram'
#-no-shutdown
#-monitor stdio

doxygen: all
	doxygen .doxygenrc

t:
	@echo ${CXX}

bochs: grub
	bochs -rc .bochsdbg -q

# Generate a BROWSE file for use with M-x ebrowse in emacs.
ebrowse:
	ebrowse **/*.h **/*.cpp

etags:
	/usr/bin/ctags -e --c++-kinds  --language-force=c++ -R --exclude='*.o' --exclude='*~' --exclude='*.asm' src/

ctags:
	ctags --declarations --defines --language=c++ --globals --members --typedefs-and-c++ **/*.h **/*.cpp

asm:
	@${CXX} -S -O2 -fverbose-asm -g  ${args} ${asm_file}.cpp -o ${asm_file}.s
	as --32 -alhnd ${asm_file}.s

stats:
	@echo "commits" $(shell git log --oneline | wc -l) "additions" $(shell git log -p | grep -v '+++' | grep -c '^+') "removals" $(shell git log -p | grep -v -e '---' | grep -c '^-')

complog:
	@echo `date --rfc-3339=seconds -u | sed 's/\+.*/::size=/'` `size nop.bin | tail -n1 |  sed 's/nop.bin//'` `sha1sum nop.bin | sed 's/ .*//g'` >> complog.log
	@tail -n7 complog.log
