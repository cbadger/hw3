filename=hw3
inputFile=input1.txt
EXE=$(filename).o
SRC=$(filename).c
p=8

$(EXE): $(SRC)
	mpicc -o $@ $<

run: $(EXE)
	mpirun -np $(p) $(EXE) < $(inputFile)

clean:
	rm -fv $(EXE) *~