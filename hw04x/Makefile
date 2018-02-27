
sort-strings: main.c ivec.c
	gcc -o sort-strings main.c ivec.c

clean:
	rm -f *.o sort-strings
	rm -rf cmake-build-debug
	rm -rf .idea

test:
	perl test.t

.PHONY: clean test
