TOOLS_CFLAGS	:= -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual \
		   -Wcast-align -Wwrite-strings -Wnested-externs -Winline \
		   -W -Wundef -Wmissing-prototypes
#
# Programs
#
all:	 Sketch Sketch2 LEDSwitch gpioThru

Sketch: Sketch.o gpio-utils.o
	$(CC) $(LDFLAGS) -o $@ $^

Sketch2: Sketch2.o gpio-utils.o
	$(CC) $(LDFLAGS) -o $@ $^

LEDSwitch: LEDSwitch.o gpio-utils.o
	$(CC) $(LDFLAGS) -o $@ $^

gpioThru: gpioThru.o gpio-utils.o
	$(CC) $(LDFLAGS) -o $@ $^



#
# Objects
#

%.o: %.c
	$(CC) $(CFLAGS) $(TOOLS_CFLAGS) -c $< -o $@

clean:
	rm Sketch.o  Sketch2.o LEDSwitch.o gpio-utils.o gpioThru 
