/**********************************************************************
 * Copyright (c) 2019
 *  Sang-Hoon Kim <sanghoonkim@ajou.ac.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include <arpa/inet.h>

/* To avoid security error on Visual Studio */
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING FROM THIS LINE ******       */
#define MAX_NR_TOKENS	32	/* Maximum length of tokens in a command */
#define MAX_TOKEN_LEN	64	/* Maximum length of single token */
#define MAX_COMMAND	256 /* Maximum length of command string */

#define INITIAL_PC	0x1000
#define INITIAL_SP	0x8000

typedef unsigned char bool;
#define true	1
#define false	0

static unsigned char memory[1 << 20] = {	/* 1MB memory at 0x0000 0000 -- 0x0100 0000 */
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
	0xde, 0xad, 0xbe, 0xef,
};

static unsigned int registers[32] = {
	0,
};

static unsigned int __pc = INITIAL_PC;
/*          ****** DO NOT MODIFY ANYTHING UP TO THIS LINE ******      */
/*====================================================================*/


static int process_instruction(unsigned int instr)
{
	printf("%08x\n", instr);
	return 0;
}

static int load_program(char * const filename, unsigned int starting_addr)
{
	/**
	 * HINT:
	 * file = fopen("file");
	 * while (fgets(buffer, file)) {
	 *	do load ...
	 * }
	 * fclose(file);
	 *
	 * Note that x86 is little-endian whereas MIP is big-endian
	 */
	return 0;
}

static int run_program(unsigned int starting_addr)
{
	return 0;
}


/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING FROM THIS LINE ******       */
const char *register_names[] = {
	"zr", "at", "v0", "v1", "a0", "a1", "a2", "a3",
	"t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
	"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
	"t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"
};
static void __show_registers(char * const register_name)
{
	int from = 0, to = 0;
	bool include_pc = false;

	if (strncmp(register_name, "all", strlen("all")) == 0) {
		from = 0;
		to = 32;
		include_pc = true;
	} else if (strncmp(register_name, "pc", strlen("pc")) == 0) {
		include_pc = true;
	} else {
		for (int i = 0; i < sizeof(register_names) / sizeof(*register_names); i++) {
			if (strncmp(register_name, register_names[i], strlen(register_names[i])) == 0) {
				from = i;
				to = i + 1;
			}
		}
	}

	for (int i = from; i < to; i++) {
		printf("[%02d:%2s] %10u  0x%08x\n", i, register_names[i], registers[i], registers[i]);
	}
	if (include_pc) {
		printf("[  pc ] %10u  0x%08x\n", __pc, __pc);
	}
}

static void __dump_memory(unsigned int addr, size_t length)
{
    for (size_t i = 0; i < length; i += 4) {
        printf("%08lx:  %02x %02x %02x %02x    %c %c %c %c\n", addr + i,
                memory[addr + i],     memory[addr + i + 1],
                memory[addr + i + 2], memory[addr + i + 3],
                memory[addr + i],     memory[addr + i + 1],
                memory[addr + i + 2], memory[addr + i + 3]);
    }
}

static bool __is_interpret = false;

static void __process_command(int argc, char *argv[])
{
	/* Interpret mode running line-by-line */
	if (__is_interpret) {
		if (strncmp(argv[0], "end", strlen("end")) == 0) {
			__is_interpret = false;
		} else {
			process_instruction(strtoimax(argv[0], NULL, 0));
		}
		return;
	} else if (strncmp(argv[0], "int", strlen("int")) == 0) {
		__is_interpret = true;
		return;
	}

	if (strncmp(argv[0], "load", strlen("load")) == 0) {
		if (argc == 2) {
			load_program(argv[1], INITIAL_PC);
		} else if (argc == 3) {
			load_program(argv[1], strtoimax(argv[2], NULL, 0));
		} else {
			printf("Usage: load [file] { [address] }\n");
		}
	} else if (strncmp(argv[0], "run", strlen("run")) == 0) {
		if (argc == 1) {
			run_program(INITIAL_PC);
		} else if (argc == 2) {
			run_program(strtoimax(argv[1], NULL, 0));
		} else {
			printf("Usage: run { [address] }\n");
		}
	} if (strncmp(argv[0], "show", strlen("show")) == 0) {
		if (argc == 1) {
			__show_registers("all");
		} else if (argc == 2) {
			__show_registers(argv[1]);
		} else {
			printf("Usage: show { [register name] }\n");
		}
	} else if (strncmp(argv[0], "dump", strlen("dump")) == 0) {
		if (argc == 3) {
			__dump_memory(strtoimax(argv[1], NULL, 0), strtoimax(argv[2], NULL, 0));
		} else {
			printf("Usage: dump [start address] [length]\n");
		}
	}
}

static int __parse_command(char *command, int *nr_tokens, char *tokens[])
{
	char *curr = command;
	int token_started = false;
	*nr_tokens = 0;

	while (*curr != '\0') {  
		if (isspace(*curr)) {  
			*curr = '\0';
			token_started = false;
		} else {
			if (!token_started) {
				tokens[*nr_tokens] = curr;
				*nr_tokens += 1;
				token_started = true;
			}
		}
		curr++;
	}

	/* Exclude comments from tokens */
	for (int i = 0; i < *nr_tokens; i++) {
		if (strncmp(tokens[i], "//", strlen("//")) == 0) {
			*nr_tokens = i;
			tokens[i] = NULL;
		}
	}

	return 0;
}

/***********************************************************************
 * The main function of this program.
 */
int main(int argc, char * const argv[])
{
	char command[MAX_COMMAND] = { '\0' };
	FILE *input = stdin;

	if (argc > 1) {
		input = fopen(argv[1], "r");
		if (!input) {
			fprintf(stderr, "No input file %s\n", argv[0]);
			return EXIT_FAILURE;
		}
	}

	if (input == stdin) {
		printf("*********************************************************\n");
		printf("*          >> SCE212 MIPS translator  v0.01 <<          *\n");
		printf("*                                                       *\n");
		printf("*                                       .---.           *\n");
		printf("*                           .--------.  |___|           *\n");
		printf("*                           |.------.|  |=. |           *\n");
		printf("*                           || >>_  ||  |-- |           *\n");
		printf("*                           |'------'|  |   |           *\n");
		printf("*                           ')______('~~|___|           *\n");
		printf("*                                                       *\n");
		printf("*                                   Fall 2019           *\n");
		printf("*********************************************************\n\n");
		printf(">> ");
	}

	while (fgets(command, sizeof(command), input)) {
		char *tokens[MAX_NR_TOKENS] = { NULL };
		int nr_tokens = 0;
		unsigned int machine_code;

		for (size_t i = 0; i < strlen(command); i++) {
			command[i] = tolower(command[i]);
		}

		if (__parse_command(command, &nr_tokens, tokens) < 0)
			continue;

		__process_command(nr_tokens, tokens);

		if (input == stdin && !__is_interpret) printf(">> ");
	}

	if (input != stdin) fclose(input);

	return EXIT_SUCCESS;
}
