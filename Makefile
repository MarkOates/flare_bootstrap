


# here are the directories of the projects 
# LIBS_ROOT=E:
LIBS_ROOT=/Users/markoates/Repos
ALLEGRO_DIR=$(LIBS_ROOT)/allegro5/build
ALLEGRO_FLARE_DIR=$(LIBS_ROOT)/allegro_flare

# these are the names of the libs you are linking
ALLEGRO_FLARE_LIB=allegro_flare-0.8.8wip
ALLEGRO_LIBS=-lallegro_color -lallegro_font -lallegro_ttf -lallegro_dialog -lallegro_audio -lallegro_acodec -lallegro_primitives -lallegro_image -lallegro_main -lallegro

EXE_EXTENSION=
# EXE_EXTENSION=.exe

OPENGL_LIB=-framework OpenGL
# OPENGL_LIB=-lopengl32




.PHONY: all clean



all: bin/flare_main$(EXE_EXTENSION) bin/flare_gui_main$(EXE_EXTENSION) bin/flare_3d_main$(EXE_EXTENSION) bin/flare_3d_face$(EXE_EXTENSION) bin/flare_3d_world$(EXE_EXTENSION) bin/flare_3d_shadows$(EXE_EXTENSION) bin/simple_3d$(EXE_EXTENSION) bin/vanilla_allegro$(EXE_EXTENSION)



bin/flare_main$(EXE_EXTENSION): obj/flare_main.o
	g++ obj/flare_main.o -o bin/flare_main -l$(ALLEGRO_FLARE_LIB) $(ALLEGRO_LIBS) -L$(ALLEGRO_FLARE_DIR)/lib -L$(ALLEGRO_DIR)/lib

bin/flare_gui_main$(EXE_EXTENSION): obj/flare_gui_main.o
	g++ obj/flare_gui_main.o -o bin/flare_gui_main -l$(ALLEGRO_FLARE_LIB) $(ALLEGRO_LIBS) -L$(ALLEGRO_FLARE_DIR)/lib -L$(ALLEGRO_DIR)/lib

bin/flare_3d_main$(EXE_EXTENSION): obj/flare_3d_main.o
	g++ obj/flare_3d_main.o -o bin/flare_3d_main -l$(ALLEGRO_FLARE_LIB) $(ALLEGRO_LIBS) -L$(ALLEGRO_FLARE_DIR)/lib -L$(ALLEGRO_DIR)/lib $(OPENGL_LIB)

bin/flare_3d_face$(EXE_EXTENSION): obj/flare_3d_face.o
	g++ obj/flare_3d_face.o -o bin/flare_3d_face -l$(ALLEGRO_FLARE_LIB) $(ALLEGRO_LIBS) -L$(ALLEGRO_FLARE_DIR)/lib -L$(ALLEGRO_DIR)/lib $(OPENGL_LIB)

bin/flare_3d_world$(EXE_EXTENSION): obj/flare_3d_world.o
	g++ obj/flare_3d_world.o -o bin/flare_3d_world -l$(ALLEGRO_FLARE_LIB) $(ALLEGRO_LIBS) -L$(ALLEGRO_FLARE_DIR)/lib -L$(ALLEGRO_DIR)/lib $(OPENGL_LIB)

bin/flare_3d_shadows$(EXE_EXTENSION): obj/flare_3d_shadows.o
	g++ obj/flare_3d_shadows.o -o bin/flare_3d_shadows -l$(ALLEGRO_FLARE_LIB) $(ALLEGRO_LIBS) -L$(ALLEGRO_FLARE_DIR)/lib -L$(ALLEGRO_DIR)/lib $(OPENGL_LIB)

bin/simple_3d$(EXE_EXTENSION): obj/simple_3d.o
	g++ obj/simple_3d.o -o bin/simple_3d -l$(ALLEGRO_FLARE_LIB) $(ALLEGRO_LIBS) -L$(ALLEGRO_FLARE_DIR)/lib -L$(ALLEGRO_DIR)/lib $(OPENGL_LIB)

bin/vanilla_allegro$(EXE_EXTENSION): obj/vanilla_allegro.o
	g++ obj/vanilla_allegro.o -o bin/vanilla_allegro $(ALLEGRO_LIBS) -L$(ALLEGRO_DIR)/lib



obj/flare_main.o: flare_main.cpp
	g++ -o $@ -c -std=gnu++11 $< -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include

obj/flare_gui_main.o: flare_gui_main.cpp
	g++ -o $@ -c -std=gnu++11 $< -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include

obj/flare_3d_main.o: flare_3d_main.cpp
	g++ -o $@ -c -std=gnu++11 $< -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include

obj/flare_3d_face.o: flare_3d_face.cpp
	g++ -o $@ -c -std=gnu++11 $< -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include

obj/flare_3d_world.o: flare_3d_world.cpp
	g++ -o $@ -c -std=gnu++11 $< -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include

obj/flare_3d_shadows.o: flare_3d_shadows.cpp
	g++ -o $@ -c -std=gnu++11 $< -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include

obj/simple_3d.o: simple_3d.cpp
	g++ -o $@ -c -std=gnu++11 $< -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include

obj/vanilla_allegro.o: vanilla_allegro.cpp
	g++ -o $@ -c $< -I$(ALLEGRO_DIR)/include



clean:
	rm *.o
	rm ./bin/*.exe



