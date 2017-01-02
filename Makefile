


# here are the directories of the projects
# LIBS_ROOT=E:
LIBS_ROOT=/Users/markoates/Repos
ALLEGRO_DIR=$(LIBS_ROOT)/allegro5/build
ALLEGRO_FLARE_DIR=$(LIBS_ROOT)/allegro_flare


# these are the names of the libs you are linking
ALLEGRO_FLARE_LIB=allegro_flare-0.8.8wip
ALLEGRO_LIBS=-lallegro_color -lallegro_font -lallegro_ttf -lallegro_dialog -lallegro_audio -lallegro_acodec -lallegro_primitives -lallegro_image -lallegro_main -lallegro
OPENGL_LIB=-framework OpenGL
# OPENGL_LIB=-lopengl32
# OPENGL_LIB=-lGL


# this is the EXE extension that is used (if any)
EXE_EXTENSION=
# EXE_EXTENSION=.exe




.PHONY: all clean



ALL_PROGRAM_BASENAMES=$(filter-out vanilla_allegro, $(basename $(notdir $(wildcard programs/*.cpp))))
ALL_PROGRAM_EXES=$(addprefix bin/, $(addsuffix $(EXE_EXTENSION), $(ALL_PROGRAM_BASENAMES)))


all: $(ALL_PROGRAM_EXES) bin/vanilla_allegro$(EXE_EXTENSION)



bin/%$(EXE_EXTENSION): programs/%.cpp
	g++ -std=gnu++11 $< -o $@ -l$(ALLEGRO_FLARE_LIB) $(ALLEGRO_LIBS) -L$(ALLEGRO_FLARE_DIR)/lib -L$(ALLEGRO_DIR)/lib $(OPENGL_LIB) -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include



bin/vanilla_allegro$(EXE_EXTENSION): programs/vanilla_allegro.cpp
	g++ programs/vanilla_allegro.cpp -o bin/vanilla_allegro $(ALLEGRO_LIBS) -L$(ALLEGRO_DIR)/lib -I$(ALLEGRO_DIR)/include



clean:
	-rm $(addsuffix .o, $(addprefix obj/,$(ALL_PROGRAM_BASENAMES)))
	-rm $(addsuffix $(EXE_EXTENSION), $(addprefix bin/,$(ALL_PROGRAM_BASENAMES)))
	-rm bin/vanilla_allegro



