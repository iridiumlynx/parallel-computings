MPICC      := mpicc
MPICCFLAGS := -fopenmp
MPILDFLAGS := -lm
MPITARGETS := ninth
MPIMAINS   := $(addsuffix .o, $(MPITARGETS) )
MPIOBJ     := $(MPIMAINS)
MPIDEPS    :=

CC      := gcc
CCFLAGS := -fopenmp
LDFLAGS := -lm
TARGETS := first second third fourth fifth sixth seventh eighth
MAINS   := $(addsuffix .o, $(TARGETS) )
OBJ     := $(MAINS)
DEPS    :=

.PHONY: all clean

all: $(TARGETS) $(MPITARGETS)

clean:
	rm -f $(TARGETS) $(MPITARGETS) $(OBJ) $(MPIOBJ) $(addsuffix .o, $(TARGETS) ) $(addsuffix .o, $(MPITARGETS) ) a.dat b.dat c.dat matrix.dat matrix_gen

$(OBJ): %.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS)

$(TARGETS): % : $(filter-out $(MAINS), $(OBJ)) %.o
	$(CC) -o $@ $(LIBS) $^ $(CCFLAGS) $(LDFLAGS) 

$(MPIOBJ): %.o : %.c $(MPIDEPS)
	$(MPICC) -c -o $@ $< $(MPICCFLAGS)

$(MPITARGETS): % : $(filter-out $(MPIMAINS), $(MPIOBJ)) %.o
	$(MPICC) -o $@ $(LIBS) $^ $(MPICCFLAGS) $(MPILDFLAGS)

test-prepare:
	echo "3 0 0 21\n1 2 0 19\n1 1 1 15 \nSolution: 7 6 2" > matrix.dat
	$(CC) -o matrix_gen matrix_gen.c $(CCFLAGS) $(LDFLAGS)
