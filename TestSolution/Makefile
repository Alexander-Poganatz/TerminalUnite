#brief simple file to compile the Terminal Unite Library
#Author Alexander Poganatz
#Author a_poganatz@outlook.com
#date 2019-08-10

OutputDir = ../output/
HeaderDir = ../output/include
HeaderInclude = -I $(HeaderDir)
COMP = $(CXX)
TERMUNITE = $(OutputDir)libTerminalUnite.a

all: $(TERMUNITE) main.o
	$(COMP) -o a.out main.o $(TERMUNITE) -lncursesw $(HeaderInclude)

$(TERMUNITE):
	touch ../TerminalUnite/App.cpp
	$(MAKE) -B ../TerminalUnite/Makefile
	
main.o:
	$(COMP) -o $@ -c main.cpp -L$(OutputDir) -lTerminalUnite -lncursesw $(HeaderInclude)
	
clean:
	rm $(TERMUNITE)
	rm main.o
	rm a.out
