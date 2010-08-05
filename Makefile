# warn if format string is a literal
warnings := -Wall -Wextra -Weffc++ -Wunused -Wformat=2 -Winit-self -Wmissing-include-dirs -Wstrict-overflow=4 -Wfloat-equal -Wreorder -Weffc++ -Wsign-promo -Wwrite-strings -Wconversion -Wlogical-op -Wundef -Wunsafe-loop-optimizations -Wtrigraphs -Wunused-parameter -Wunknown-pragmas -Wcast-align -Wswitch-default -Wswitch-enum -Wmissing-noreturn -Wmissing-format-attribute -Wpacked -Wredundant-decls -Wunreachable-code -Winline -Winvalid-pch -Wmissing-declarations -Wdisabled-optimization -Wstack-protector -Woverloaded-virtual -Wsign-promo -Woverloaded-virtual -Wold-style-cast -Wstrict-null-sentinel -Wunused-macros

#-Waggregate-return : aggrevating is more the term.
osdevops :=-nostdinc++	-nostdinc -nostartfiles	-fno-exceptions	-fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -fno-rtti

experimentalops := -fextended-identifiers

assembly_output :=-masm=intel -save-temps

fthings := -fstrict-aliasing -fno-rtti

# Compiling C++ no matter what the file extensions are when invoking gcc.
cxx_selection := -std=gnu++0x -x c++

includes := -I./src

args := ${warnings} ${fthings} ${osdevops} ${experimentalops} \
	 -Wwrite-strings ${includes} ${cxx_selection}

gcc := /usr/x86_64-pc-linux-gnu/i686-pc-linux-gnu/gcc-bin/4.5.0/i686-pc-linux-gnu-gcc
# non source files.
AUXFILES := Makefile

PROJDIRS := src
SRCFILES := $(shell find  -name "*.cpp")
HDRFILES := $(shell find -name "*.h")

OBJFILES := $(patsubst %.cpp,%.o,$(SRCFILES))
DEPFILES := $(patsubst %.cpp,%.d,$(SRCFILES))

# declare that these rules don't exist elsewhere.
.PHONY: all clean dist test testdrivers todolist

all: tacospp.bin

tacospp.bin: $(OBJFILES)
	@nasm -f elf -o loader.o loader.s
	@i686-pc-linux-gnu-ld -T linker.ld -o tacospp.bin loader.o ${OBJFILES}
	@echo "Done! Linked the following into tacospp.bin:" ${OBJFILES}
	@echo "Size (in bytes):" $(shell du --bytes tacospp.bin)

%.o: %.cpp Makefile
	@$(gcc) $(args) -save-temps -O0 -MMD -MP -MT "$*.d $*.o"  -c $< -o $@
	@echo "Compiled" $(shell du --bytes $(patsubst %.cpp,%.o,$<))

floppy:
	dd if=/dev/zero of=pad bs=1 count=750
	cat /boot/grub/stage1 /boot/grub/stage2 pad tacospp.bin > floppy.img

check-syntax:
	${gcc} ${args}  -o /dev/null -c ${CHK_SOURCES}

clean:
	-@$(RM) $(wildcard $(OBJFILES) $(DEPFILES) $(REGFILES) \
		tacospp.bin tacospp.iso)

grub:
	@mkdir -p isofiles/boot/grub
	@cp /boot/grub/stage2_eltorito  isofiles/boot/grub/
	@cp tacospp.bin isofiles/boot

	@genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4                 -boot-info-table --input-charset utf-8 -o tacospp.iso isofiles

qemu: grub
	qemu -cdrom tacospp.iso

doxygen: all
	doxygen .doxygenrc