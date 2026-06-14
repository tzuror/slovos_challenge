.PHONY: clean test

agent.out: agent.c crypt.h agent_link.ld
	gcc -s -o '$@'    $^
#-Wall -Wextra -pedantic -std=c99 -fno-pie
run: agent.out
	python3 after_compile.py

clean:
	rm -f agent.out
