CC = gcc
CFLAGS = -g $(shell sdl-config --cflags) -DUSE_SDL=1 -DPLATFORM_UNIX=1 -W -Wall -Wno-unused -DUSE_EXECINFO=1
LDLIBS = $(shell sdl-config --libs) -lSDL -lSDL_mixer -Wl,-E

all: rott

rott: 	\
	cin_actr.o \
	cin_efct.o \
	cin_evnt.o \
	cin_glob.o \
	cin_main.o \
	cin_util.o \
	dosutil.o \
	engine.o \
	fx_man.o \
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
	byteordr.o
	$(CC) $^ $(LDLIBS) -o $@

clean:
	rm -rf *.o

distclean: clean
	rm -rf *~
