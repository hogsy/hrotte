CC = gcc
CFLAGS = -g $(shell sdl-config --cflags)
LDLIBS = $(shell sdl-config --libs) -lSDL_mixer

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
	rt_sqrt.o \
	rt_stat.o \
	rt_state.o \
	rt_str.o \
	rt_ted.o \
	rt_util.o \
	rt_view.o \
	rt_vid.o \
	scriplib.o \
	w_wad.o \
	watcom.o \
	z_zone.o
	$(CC) $^ $(LDLIBS) -o $@

# rt_in.c, rt_swift.c, rt_spbal.c, rt_error.c, modexlib.c, sbconfig.c
# rt_dr_a, rt_fc_a, rt_vh_a, rt_sc_a, f_scale
# rt_err.obj - some dos text screen for error messages

clean:
	rm *.o

distclean: clean
	rm *~
