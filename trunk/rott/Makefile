#-----------------------------------------------------------------------------#
# ROTT makefile.
#-----------------------------------------------------------------------------#

CC = gcc

CFLAGS += -g -W -Wall -Wno-unused
LDFLAGS += -Wl,-E

SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs) -lSDL_mixer

ALL_CFLAGS = $(SDL_CFLAGS) -DUSE_SDL=1 -DUSE_EXECINFO=1 -DPLATFORM_UNIX=1 $(CFLAGS) $(EXTRACFLAGS)
LDLIBS = $(LDFLAGS) $(SDL_LDFLAGS) $(EXTRALDFLAGS)

%.o: %.c
	$(CC) $(ALL_CFLAGS) -c $< -o $@

all: rott

audiolib/audiolib.a:
	$(MAKE) -C audiolib CC="$(CC)" CFLAGS="$(ALL_CFLAGS)" LDLIBS="$(LDLIBS)"

OBJS = \
	cin_actr.o \
	cin_efct.o \
	cin_evnt.o \
	cin_glob.o \
	cin_main.o \
	cin_util.o \
	dosutil.o \
	engine.o \
	isr.o \
	modexlib.o \
	rt_actor.o \
	rt_battl.o \
	rt_build.o \
	rt_cfg.o \
	rt_crc.o \
	rt_com.o \
	rt_debug.o \
	rt_dmand.o \
	rt_door.o \
	rt_draw.o \
	rt_floor.o \
	rt_game.o \
	rt_in.o \
	rt_main.o \
	rt_map.o \
	rt_menu.o \
	rt_msg.o \
	rt_net.o \
	rt_playr.o \
	rt_rand.o \
	rt_scale.o \
	rt_sound.o \
	rt_spbal.o \
	rt_sqrt.o \
	rt_stat.o \
	rt_state.o \
	rt_str.o \
	rt_swift.o \
	rt_ted.o \
	rt_util.o \
	rt_view.o \
	rt_vid.o \
	rt_err.o \
	scriplib.o \
	w_wad.o \
	watcom.o \
	z_zone.o \
	byteordr.o \
	dukemusc.o \
	winrott.o

$(OBJS): develop.h
rott: $(OBJS) audiolib/audiolib.a
	$(CC) $^ $(LDLIBS) -o $@

clean:
	$(MAKE) -C audiolib clean
	rm -f rott rott.exe *.o

distclean: clean
	$(MAKE) -C audiolib distclean
	rm -f *~
