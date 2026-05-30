.PHONY: clean test

agent.out: agent.c crypt.h agent_link.ld
	gcc -s -o '$@'    $^
#-Wall -Wextra -pedantic -std=c99 -fno-pie
clean:
	rm -f agent.out
