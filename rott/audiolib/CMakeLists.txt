
project(audiolib)

add_library(audiolib STATIC
        dsl.c
        fx_man.c
        ll_man.c
        multivoc.c
        mv_mix.c
        mvreverb.c
        nodpmi.c
        pitch.c
        user.c
        usrhooks.c
        )
