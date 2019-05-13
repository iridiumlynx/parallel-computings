CC      := gcc
CCFLAGS := -fopenmp
LDFLAGS := -lm
TARGETS := first second fourth fifth sixth eighth
MAINS   := $($(TARGETS))
OBJ    := $(MAINS)
DEPS   := 

.PHONY: all clean test-prepare

all: $(TARGETS)

clean:
	rm -f $(TARGETS) $(OBJ) $(addsuffix .o, $(TARGETS) ) a.dat b.dat c.dat

$(OBJ): %.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS)

$(TARGETS): % : $(filter-out $(MAINS), $(OBJ)) %.o
	$(CC) -o $@ $(LIBS) $^ $(CCFLAGS) $(LDFLAGS) 

test-prepare:
	echo "matrix 3x3 1 0 0 0 1 0 0 0 1" > a.dat
	echo "matrix 3x3 1 0 0 0 1 0 0 0 1" > b.dat
