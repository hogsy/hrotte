CC=gcc
CFLAGS=-g

all: rott

rott: 	engine.o \
	rt_actor.o \
	rt_build.o \
	rt_debug.o \
	rt_door.o \
	rt_draw.o \
	rt_floor.o \
	rt_game.o \
	rt_main.o \
	rt_map.o \
	rt_menu.o \
	rt_playr.o \
	rt_rand.o \
	rt_scale.o \
	rt_sound.o \
	rt_stat.o \
	rt_state.o \
	rt_str.o \
	rt_ted.o \
	rt_vid.o \
	scriplib.o \
	w_wad.o \
	z_zone.o

# rt_util.c, rt_in.c, rt_swift.c
# rt_dr_a, rt_fc_a, rt_vh_a, rt_sc_a, f_scale

clean:
	rm *.o

distclean: clean
	rm *~
