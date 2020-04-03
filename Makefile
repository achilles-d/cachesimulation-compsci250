all: cachesim virt2phys

virt2phys: virt2phys.c
	gcc -g -o $@ $<

cachesim: cachesim.c
	gcc -g -o $@ $<

clean:
	rm -f cachesim virt2phys
