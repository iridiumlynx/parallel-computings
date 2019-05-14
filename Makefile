CC      := gcc
CCFLAGS := -fopenmp
LDFLAGS := -lm
TARGETS := first second third fourth fifth sixth seventh eighth
MAINS   := $($(TARGETS))
OBJ    := $(MAINS)
DEPS   := 

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS) $(OBJ) $(addsuffix .o, $(TARGETS) ) a.dat b.dat c.dat matrix.dat matrix_gen

$(OBJ): %.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS)

$(TARGETS): % : $(filter-out $(MAINS), $(OBJ)) %.o
	$(CC) -o $@ $(LIBS) $^ $(CCFLAGS) $(LDFLAGS) 

test-prepare:
	echo "3 0 0 21\n1 2 0 19\n1 1 1 15 \nSolution: 7 6 2" > matrix.dat
	$(CC) -o matrix_gen matrix_gen.c $(CCFLAGS) $(LDFLAGS)
