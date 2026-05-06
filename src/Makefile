
CXX=/usr/local/Cellar/gcc/13.2.0/bin/g++-13
INCLUDE=/usr/local/Cellar/gcc/13.2.0/include/c++/13
FLAGS = -std=c++11 -march=x86-64

LIBOBJS = nexrad_l2_header.o nexrad_l2_message.o nexrad_l2_message5.o nexrad_l2_message31.o \
		  nexrad_l2_message31_momrad.o bin_utils.o nexrad_l2_chunk.o
LIBNAME = libnexrad.a

%.o: %.cxx %.hxx
	$(CXX) $(FLAGS) -c $< -o $@ -I$(INCLUDE)

all: $(LIBNAME)

clean:
	rm $(LIBOBJS) $(LIBNAME)

$(LIBNAME): $(LIBOBJS)
	ar rcs $(LIBNAME) $(LIBOBJS)

nexrad_l2_chunk.o: nexrad_l2_message.o
nexrad_l2_message.o: nexrad_l2_header.o
nexrad_l2_header.o: bin_utils.o
nexrad_l2_message5.o: nexrad_l2_message.o bin_utils.o
nexrad_l2_message31.o: nexrad_l2_message.o nexrad_l2_message31_momrad.o bin_utils.o
nexrad_l2_message31_momrad.o: bin_utils.o