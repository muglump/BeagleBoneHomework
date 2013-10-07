TOOLS_CFLAGS	:= -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual \
		   -Wcast-align -Wwrite-strings -Wnested-externs -Winline \
		   -W -Wundef -Wmissing-prototypes
CFLAGS  := -O3 -g
#
# Programs
#
all:	gpioThru gpioToggle SingleToggle DoubleToggle

gpioToggle: gpioToggle.c beaglebone_gpio.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

gpioThru:  gpioThru.c beaglebone_gpio.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
	
SingleToggle: SingleToggle.c beaglebone_gpio.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

DoubleToggle: DoubleToggle.c beaglebone_gpio.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

#
# Objects
#

%.o: %.c
	$(CC) $(CFLAGS) $(TOOLS_CFLAGS) -c $< -o $@

clean:
	rm gpioToggle gpioThru SingleToggle DoubleToggle

print:
	@echo LDFLAGS = $(LDFLAGS)
