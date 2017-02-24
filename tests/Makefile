GCC=riscv64-unknown-elf-gcc
CFLAGS=-mcmodel=medany -std=gnu99 -O2 -fno-common -fno-builtin-printf
LDFLAGS=-static -nostdlib -nostartfiles -lgcc
OBJDUMP=riscv64-unknown-elf-objdump

PROGRAMS=pwm matrix simple scatter_gather partition
EXECUTABLES=$(addsuffix .riscv,$(PROGRAMS))
DUMPS=$(addsuffix .dump,$(PROGRAMS))

default: $(EXECUTABLES)

dumps: $(DUMPS)

%.o: %.S
	$(GCC) $(CFLAGS) -D__ASSEMBLY__=1 -c $< -o $@

%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

%.riscv: %.o crt.o syscalls.o
	$(GCC) -T link.ld $(LDFLAGS) $^ -o $@

%.dump: %.riscv
	$(OBJDUMP) -D $< > $@

clean:
	rm -f *.riscv *.o *.dump
