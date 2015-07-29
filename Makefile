


# here are the directories of the projects 
ALLEGRO_DIR=E:/allegro-5.1.11-mingw-edgar
ALLEGRO_FLARE_DIR=E:/allegro_flare
FGUI_DIR=E:/flare_gui

# these are the names of the libs you are linking
ALLEGRO_MONOLITH_LIB=allegro_monolith-debug.dll
FGUI_LIB=flare_gui-0.6.6-mingw-4.8.1
ALLEGRO_FLARE_LIB=allegro_flare-0.8.6-mingw-4.8.1



.PHONY: all clean



all: bin/flare_main.exe bin/flare_gui_main.exe bin/flare_3d_main.exe bin/vanilla_allegro.exe



bin/flare_main.exe: obj/flare_main.o
	g++ obj/flare_main.o -o bin/flare_main -l$(ALLEGRO_FLARE_LIB) -l$(ALLEGRO_MONOLITH_LIB) -L$(ALLEGRO_FLARE_DIR)/lib -L$(ALLEGRO_DIR)/lib

bin/flare_gui_main.exe: obj/flare_gui_main.o
	g++ obj/flare_gui_main.o -o bin/flare_gui_main -l$(FGUI_LIB) -l$(ALLEGRO_FLARE_LIB) -l$(ALLEGRO_MONOLITH_LIB) -L$(ALLEGRO_FLARE_DIR)/lib -L$(FGUI_DIR)/lib -L$(ALLEGRO_DIR)/lib

bin/flare_3d_main.exe: obj/flare_3d_main.o
	g++ obj/flare_3d_main.o -o bin/flare_3d_main -lopengl32 -l$(FGUI_LIB) -l$(ALLEGRO_FLARE_LIB) -l$(ALLEGRO_MONOLITH_LIB) -L$(ALLEGRO_FLARE_DIR)/lib -L$(FGUI_DIR)/lib -L$(ALLEGRO_DIR)/lib

bin/vanilla_allegro.exe: obj/vanilla_allegro.o
	g++ obj/vanilla_allegro.o -o bin/vanilla_allegro -l$(ALLEGRO_MONOLITH_LIB) -L$(ALLEGRO_DIR)/lib



obj/flare_main.o: flare_main.cpp
	g++ -o $@ -c -std=gnu++11 $< -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include

obj/flare_gui_main.o: flare_gui_main.cpp
	g++ -o $@ -c -std=gnu++11 $< -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include -I$(FGUI_DIR)/include

obj/flare_3d_main.o: flare_3d_main.cpp
	g++ -o $@ -c -std=gnu++11 $< -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include -I$(FGUI_DIR)/include

obj/vanilla_allegro.o: vanilla_allegro.cpp
	g++ -o $@ -c $< -I$(ALLEGRO_DIR)/include



clean:
	rm *.o
	rm ./bin/*.exe



