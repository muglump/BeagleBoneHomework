TOOLS_CFLAGS	:= -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual \
		   -Wcast-align -Wwrite-strings -Wnested-externs -Winline \
		   -W -Wundef -Wmissing-prototypes
#
# Programs
#
all:	matrixLEDi2c myi2cget

matrixLEDi2c:  matrixLEDi2c.o i2cbusses.o gpio-utils.o
	$(CC) $(LDFLAGS) -o $@ $^

myi2cget:   myi2cget.o gpio-utils.o
	$(CC) $(LDFLAGS) -o $@ $^





#
# Objects
#

%.o: %.c
	$(CC) $(CFLAGS) $(TOOLS_CFLAGS) -c $< -o $@

clean:
	rm gpio-utils.o matrixLEDi2c.o myi2cget.o matrixLEDi2c myi2cget
