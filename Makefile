


# here are the directories of the projects 
ALLEGRO_FLARE_DIR=E:/allegro_flare
ALLEGRO_DIR=E:/allegro-5.1.8-mingw-4.7.0
FGUI_DIR=E:/flare_gui

# these are the names of the libs you are linking
ALLEGRO_MONOLITH_LIB=allegro-5.1.8-monolith-md
FGUI_LIB=flare_gui-0.6.5-mingw-4.8.1
ALLEGRO_FLARE_LIB=allegro_flare-0.8.5-mingw-4.8.1


.PHONY: all clean


all: bin/flare_main.exe bin/flare_gui_main.exe


bin/flare_main.exe: flare_main.o
	g++ flare_main.o -o bin/flare_main -l$(ALLEGRO_FLARE_LIB) -l$(ALLEGRO_MONOLITH_LIB) -L$(ALLEGRO_FLARE_DIR)/lib -L$(ALLEGRO_DIR)/lib

bin/flare_gui_main.exe: flare_gui_main.o
	g++ flare_gui_main.o -o bin/flare_gui_main -l$(FGUI_LIB) -l$(ALLEGRO_FLARE_LIB) -l$(ALLEGRO_MONOLITH_LIB) -L$(ALLEGRO_FLARE_DIR)/lib -L$(FGUI_DIR)/lib -L$(ALLEGRO_DIR)/lib


flare_gui_main.o: flare_gui_main.cpp
	g++ -c -std=gnu++11 $(basename $@).cpp -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include -I$(FGUI_DIR)/include

flare_main.o: flare_main.cpp
	g++ -c -std=gnu++11 $(basename $@).cpp -I$(ALLEGRO_FLARE_DIR)/include -I$(ALLEGRO_DIR)/include -I$(FGUI_DIR)/include


clean:
	del *.o
	del .\bin\*.exe
