#brief simple file to compile the Terminal Unite Library
#Author Alexander Poganatz
#Author a_poganatz@outlook.com
#date 2019-08-10

OutputDir = ../output/
HeaderDir = ../output/include
HeaderInclude = -I $(HeaderDir)
COMP = $(CXX)
TERMUNITE = $(OutputDir)libTerminalUnite.a

all: $(TERMUNITE) main.cpp
	$(COMP) -o a.out main.cpp -L$(OutputDir) -lTerminalUnite -lncursesw $(HeaderInclude)

$(TERMUNITE):
	touch ../TerminalUnite/app.cpp
	$(MAKE) -B ../TerminalUnite/Makefile
	
clean:
	rm main.o
	rm a.out
