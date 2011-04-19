AUDIOLIB ?= audiolib.a

AR ?= ar
CC ?= gcc
RANLIB ?= ranlib

CFLAGS += $(shell sdl-config --cflags)
CFLAGS += -g
CFLAGS += -O2
CFLAGS += -Wall

LDLIBS += $(shell sdl-config --libs)
LDLIBS += -lSDL_mixer

OBJS :=
OBJS += fx_man.o
OBJS += dsl.o
OBJS += ll_man.o
OBJS += multivoc.o
OBJS += mv_mix.o
OBJS += mvreverb.o
OBJS += nodpmi.o
OBJS += pitch.o
OBJS += user.o
OBJS += usrhooks.o

$(AUDIOLIB): $(OBJS)
	rm -f $@
	$(AR) rc $@ $^
	$(RANLIB) $@

clean:
	rm -f $(AUDIOLIB) $(OBJS)
