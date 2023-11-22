sim: dram.c shell.c sim.c
	gcc -g -O2 $^ -o $@

test: shell.c sim.c
	gcc -g -O2 -DTEST_MODE $^ -o $@

.PHONY: clean
clean:
	rm -rf *.o *~ sim
