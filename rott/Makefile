CC=gcc
CFLAGS=-g

all: rott

rott: 	engine.o \
	rt_actor.o \
	rt_door.o \
	rt_draw.o \
	rt_floor.o \
	rt_main.o \
	rt_menu.o \
	rt_playr.o \
	rt_rand.o \
	rt_scale.o \
	rt_stat.o \
	rt_state.o \
	rt_str.o \
	rt_ted.o \
	rt_vid.o \
	w_wad.o \
	z_zone.o

# rt_util.c
# rt_dr_a, rt_fc_a, rt_vh_a

clean:
	rm *.o

distclean: clean
	rm *~
