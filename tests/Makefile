GCC=riscv64-unknown-elf-gcc
CFLAGS=-mcmodel=medany -std=gnu99 -O2 -fno-common -fno-builtin-printf
LDFLAGS=-static -nostdlib -nostartfiles -lgcc
OBJDUMP=riscv64-unknown-elf-objdump
PYTHON=python

PROGRAMS=pwm matrix simple scatter_gather partition stitch ir
EXECUTABLES=$(addsuffix .riscv,$(PROGRAMS))
DUMPS=$(addsuffix .dump,$(PROGRAMS))

default: $(EXECUTABLES)

dumps: $(DUMPS)

%.o: %.S
	$(GCC) $(CFLAGS) -D__ASSEMBLY__=1 -c $< -o $@

%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

ir.o: ir.c ir-dataset.h
	$(GCC) $(CFLAGS) -c $< -o $@

ir-dataset.h: ir-datagen.py
	$(PYTHON) $< > $@

%.riscv: %.o crt.o syscalls.o link.ld
	$(GCC) -T link.ld $(LDFLAGS) $< crt.o syscalls.o -o $@

%.dump: %.riscv
	$(OBJDUMP) -D $< > $@

clean:
	rm -f *.riscv *.o *.dump
