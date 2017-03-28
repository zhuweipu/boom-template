GCC=riscv64-unknown-elf-gcc
CFLAGS=-mcmodel=medany -std=gnu99 -O2 -fno-common -fno-builtin-printf
LDFLAGS=-static -nostdlib -nostartfiles -lgcc
OBJDUMP=riscv64-unknown-elf-objdump
PYTHON=python

PROGRAMS=partition concat ir cond-update append split boolgen filter \
	sort colreduce aggregate logalu column-select
EXECUTABLES=$(addsuffix .riscv,$(PROGRAMS))
DUMPS=$(addsuffix .dump,$(PROGRAMS))

default: $(EXECUTABLES)

dumps: $(DUMPS)

%.o: %.S
	$(GCC) $(CFLAGS) -D__ASSEMBLY__=1 -c $< -o $@

%.o: %.c adamacc.h
	$(GCC) $(CFLAGS) -c $< -o $@

ir.o: ir.c ir-dataset.h adamacc.h
	$(GCC) $(CFLAGS) -c $< -o $@

ir-dataset.h: ir-datagen.py
	$(PYTHON) $< > $@

sort.o: sort.c sort-dataset.h adamacc.h
	$(GCC) $(CFLAGS) -c $< -o $@

sort-dataset.h: sort-datagen.py
	$(PYTHON) $< > $@

colreduce.o: colreduce.c colreduce-dataset.h adamacc.h
	$(GCC) $(CFLAGS) -c $< -o $@

colreduce-dataset.h: colreduce-datagen.py
	$(PYTHON) $< > $@

%.riscv: %.o crt.o syscalls.o link.ld
	$(GCC) -T link.ld $(LDFLAGS) $< crt.o syscalls.o -o $@

%.dump: %.riscv
	$(OBJDUMP) -D $< > $@

clean:
	rm -f *.riscv *.o *.dump
