


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



bin/flare_main.exe: flare_main.o
	g++ flare_main.o -o bin/flare_main -l$(ALLEGRO_FLARE_LIB) -l$(ALLEGRO_MONOLITH_LIB) -L$(ALLEGRO_FLARE_DIR)/lib -L$(ALLEGRO_DIR)/lib

bin/flare_gui_main.exe: flare_gui_main.o
	g++ flare_gui_main.o -o bin/flare_gui_main -l$(FGUI_LIB) -l$(ALLEGRO_FLARE_LIB) -l$(ALLEGRO_MONOLITH_LIB) -L$(ALLEGRO_FLARE_DIR)/lib -L$(FGUI_DIR)/lib -L$(ALLEGRO_DIR)/lib

bin/flare_3d_main.exe: flare_3d_main.o
	g++ flare_3d_main.o -o bin/flare_3d_main -lopengl32 -l$(FGUI_LIB) -l$(ALLEGRO_FLARE_LIB) -l$(ALLEGRO_MONOLITH_LIB) -L$(ALLEGRO_FLARE_DIR)/lib -L$(FGUI_DIR)/lib -L$(ALLEGRO_DIR)/lib

bin/vanilla_allegro.exe: vanilla_allegro.o
	g++ vanilla_allegro.o -o bin/vanilla_allegro -l$(ALLEGRO_MONOLITH_LIB) -L$(ALLEGRO_DIR)/lib



flare_main.o: flare_main.cpp
	g++ -c -std=gnu++11 flare_main.cpp -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include

flare_gui_main.o: flare_gui_main.cpp
	g++ -c -std=gnu++11 flare_gui_main.cpp -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include -I$(FGUI_DIR)/include

flare_3d_main.o: flare_3d_main.cpp
	g++ -c -std=gnu++11 flare_3d_main.cpp -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include -I$(FGUI_DIR)/include

vanilla_allegro.o: vanilla_allegro.cpp
	g++ -c vanilla_allegro.cpp -I$(ALLEGRO_DIR)/include



clean:
	rm *.o
	rm ./bin/*.exe



