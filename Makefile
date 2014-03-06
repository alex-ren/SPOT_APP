
RM = rm -rf

# ####### ###### # 
#
target=test

src=main.cpp

# ######
#
spot_prefix = $(HOME)/programs/install

SPOT_CPPFLAGS = -I$(spot_prefix)/include/spot \
-I$(spot_prefix)/include

SPOT_LDFLAGS = -L$(spot_prefix)/lib -lspot -lbdd

.PHONY: all

all: $(target)

$(target): main.o mystate.o mykripke_succ_iterator.o mykripke.o
	g++ $^ $(SPOT_LDFLAGS) -o $@ 

main.o: main.cpp
	g++ -g -c $< $(SPOT_CPPFLAGS) -o $@
	
mystate.o: mystate.cpp mystate.hh
	g++ -g -c $< $(SPOT_CPPFLAGS) -o $@

mykripke_succ_iterator.o: mykripke_succ_iterator.cpp mykripke_succ_iterator.hh
	g++ -g -c $< $(SPOT_CPPFLAGS) -o $@

mykripke.o: mykripke.cpp mykripke.hh
	g++ -g -c $< $(SPOT_CPPFLAGS) -o $@

clean:
	$(RM) *.o
	$(RM) $(target)


