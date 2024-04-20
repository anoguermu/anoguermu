OPCIONS = -D_JUDGE_ -D_GLIBCXX_DEBUG -O2 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11
#
G++ = g++  #esto solo es necesario en la FIB, ya que su g++ es muy antiguo
#
program.exe: program.o Procesador.o Cluster.o Proces.o Prioritat.o Area_Processos.o
	$(G++) -o program.exe program.o Procesador.o Cluster.o Proces.o Prioritat.o Area_Processos.o
#
program.o: program.cc  procesador.hh Cluster.hh Proces.hh Prioritat.hh Area_Processos.hh
	$(G++) -c program.cc $(OPCIONS) 
#
Procesador.o: Procesador.cc Proces.hh
	$(G++) -c Procesador.cc $(OPCIONS) 
	
#
Proces.o: Proces.cc
	$(G++) -c Proces.cc $(OPCIONS)
	
#
Cluster.o: Cluster.cc procesador.hh
	$(G++) -c Cluster.cc $(OPCIONS)
#
Prioritat.o: Prioritat.cc Proces.hh Cluster.hh
	$(G++) -c Prioritat.cc $(OPCIONS)
#
Area_Processos.o: Area_Processos.cc Prioritat.hh Cluster.hh
	$(G++) -c Area_Processos.cc $(OPCIONS)


#
clean:
	rm -f *.o
	rm -f *.exe
	
#
practica.tar: program.exe html.zip;
	tar cvf practica.tar program.cc Procesador.cc Area_Processos.cc Cluster.cc Prioritat.cc Proces.cc  procesador.hh Cluster.hh Proces.hh Prioritat.hh Area_Processos.hh BinTree.hh Makefile html.zip jp_abel.perello.zip
