#brief simple file to compile the Terminal Unite Library
#Author Alexander Poganatz
#Author a_poganatz@outlook.com
#date 2019-08-10

OutputDir = ../output/
HeaderDir = ../output/include
HeaderInclude = -I ../output/include
COMP = $(CXX) -c
#set output file name to target
O = -o $@
LIB = -lncursesw
#Panel files that don't have cpp files but are dependent in many files
PANELS = $(HeaderDir)/Panel.hpp $(HeaderDir)/InputHandler.hpp $(HeaderDir)
#TerminalUnite.lib:
#	ar crf $(OutputDir)
#build console object file

all: tuncurses.o TextBox.o TextBlock.o App.o
	ar rvs $(OutputDir)/libTerminalUnite.a tuncurses.o TextBox.o TextBlock.o App.o

tuncurses.o: ConsoleAPIncurseswImplementation.cpp $(HeaderDir)/ConsoleAPI.hpp
	$(COMP) $< $(O) $(HeaderInclude) $(LIB)

TextBox.o: TextBox.cpp tuncurses.o $(PANELS)
	$(COMP) $< $(O) $(HeaderInclude) $(LIB)
	
TextBlock.o: TextBlock.cpp tuncurses.o $(PANELS)
	$(COMP) $< $(O) $(HeaderInclude) $(LIB)
App.o: App.cpp tuncurses.o $(PANELS)
	$(COMP) $< $(O) $(HeaderInclude) $(LIB)

clean:
	rm App.o
	rm TextBox.o
	rm TextBlock.o
	rm tuncurses.o
