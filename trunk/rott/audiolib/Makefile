AUDIOLIB ?= audiolib.a

CFLAGS += $(shell sdl-config --cflags)

LDLIBS += $(shell sdl-config --libs)
LDLIBS += -lSDL_mixer

AR ?= ar
RANLIB ?= ranlib

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

clean distclean:
	rm -f $(AUDIOLIB) $(OBJS)
