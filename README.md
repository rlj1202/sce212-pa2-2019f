## Project #2: MIPS Emulator

### ***Due on 24:00, November 8 (Friday)***

### Goal

Implement a MIPS emulator that executes MIPS instructions loaded on the memory.


### Problem Specification

- In this PA, we continue on implementing a MIPS emulator. Likewise the previous PAs, the framework gets a line of command, parses it, and then calls relavant procedures to process the request.

-  We define a program as *a list of machine instructions*. It looks like;

	```
	0x8c080000
	0x8c090008  // lw t1, zero + 8
	0xac090020
	0x20080030  // addi t0 zero 0x30
	```

  Each line in a program file starts with a hexadecimal number, which is a MIPS machine code. The machine code might be followed by comments (like the 2nd and 4th instructions above). The emulaor is supposed to load the program into memory and starts executing the instructions until it meets a `halt` instruction. See test case files for examples of programs.

- The machine has 32 registers and their values are in `unsigned int registers[32]`. Their name is specified in `const char *register_names[32]`, and their status can be examined using `show` CLI command. Without specifying the register name, `show` shows all register values as follows;

  ```
  >> show
  [00:zr] 0x00000000    0
  [01:at] 0x00000000    0
  [02:v0] 0x00000000    0
  [03:v1] 0x00000000    0
  [04:a0] 0x00000000    0
  [05:a1] 0x00000000    0
  [06:a2] 0x00000000    0
  [07:a3] 0x00000000    0
  [08:t0] 0x00000000    0
  [09:t1] 0x00000000    0
  [10:t2] 0x00000000    0
  [11:t3] 0x00000000    0
  [12:t4] 0x00000000    0
  [13:t5] 0x00000000    0
  [14:t6] 0x00000000    0
  [15:t7] 0x00000000    0
  [16:s0] 0x00000010    16
  [17:s1] 0x00000001    1
  [18:s2] 0x00000002    2
  [19:s3] 0x00000003    3
  [20:s4] 0xbadacafe    3134900990
  [21:s5] 0x00000005    5
  [22:s6] 0x00000006    6
  [23:s7] 0x00000007    7
  [24:t8] 0x00000000    0
  [25:t9] 0x00000000    0
  [26:k0] 0x00000000    0
  [27:k1] 0x00000000    0
  [28:gp] 0x00000000    0
  [29:sp] 0x00008000    32768
  [30:fp] 0x00000000    0
  [31:ra] 0x00000000    0
  [  pc ] 0x00001000
  ```

  You can specify register name to see the value of the register only. Note that register #0 is named to `zr`.

  ```
  >> show s4
  [20:s4] 0xbadacafe    3134900990
  >> show pc
  [  pc ] 0x00001000
  ```


- The machine also has 1MB memory which is accessible through  `unsigned char memory[]`. Like the registers, the framework provides `dump` CLI command to examine memory contents. It accepts the starting address and length for command parameters. Non-printable characters are printed as '.'.
  ```
  >> dump 0 32
  0x00000000:  00 11 22 33    . . " 3
  0x00000004:  44 55 66 77    D U f w
  0x00000008:  de ad be ef    . . . .
  0x0000000c:  00 00 00 00    . . . .
  0x00000010:  68 65 6c 6c    h e l l
  0x00000014:  6f 20 77 6f    o   w o
  0x00000018:  72 6c 64 21    r l d !
  0x0000001c:  21 00 00 00    ! . . .
  >> dump 0x1000 0x20
  0x00001000:  00 00 00 00
  0x00001004:  00 00 00 00
  0x00001008:  00 00 00 00
  0x0000100c:  00 00 00 00
  0x00001010:  00 00 00 00
  0x00001014:  00 00 00 00
  0x00001018:  00 00 00 00
  0x0000101c:  00 00 00 00
  ```


- The emulator should support following instructions to get full points of this PA. Note that some conditional branch instructions(slt, slti, beq, bne), jump instructions(jr, j, jal), and a special instruction(halt) are added from PA1.

  | Name   | Format |Opcode / opcode + funct |
  | ------ | ------ | ---------------------- |
  | `add`  |   R    | 0 + 0x20               |
  | `addi` |   I    | 0x08                   |
  | `sub`  |   R    | 0 + 0x22               |
  | `and`  |   R    | 0 + 0x24               |
  | `andi` |   I    | 0x0c                   |
  | `or`   |   R    | 0 + 0x25               |
  | `ori`  |   I    | 0x0d                   |
  | `nor`  |   R    | 0 + 0x27               |
  | `sll`  |   R    | 0 + 0x00               |
	| `srl`  |   R    | 0 + 0x02               |
  | `sra`  |   R    | 0 + 0x03               |
  | `lw`   |   I    | 0x23                   |
  | `sw`   |   I    | 0x2b                   |
  | **`slt`**  |   R    | 0 + 0x2a               |
	| **`slti`** |   I    | 0x0a                   |
  | **`beq`**  |   I    | 0x04                   |
  | **`bne`**  |   I    | 0x05                   |
  | **`jr`**   |   R    | 0 + 0x08               |
  | **`j`**    |   J    | 0x02                   |
  | **`jal`**  |   J    | 0x03                   |
  | **`halt`** |   -    | 0x3f                   |

- `halt` instruction is not an actual MIPS instruction but a pseudo instruction for testing purpose. It's machine code is 0xffffffff. The machine is supposed to stop executing the program once it see this instruction.

- You are asked to implement three functions; `process_instruction()`, `load_program()`,  and `run_program()`. See the comments in the code for the expected tasks for each function.

- `process_instruction()` is called when the user inputs a hexadecimal number on the CLI prompt. The function is supposed to execute the MIPS instruction. For example, when the user inputs 0x014b4820, the emulator should add t2 and t3 and stores the result into t1, load the data from memory s1+16 into register t2 for 0x8e2a0010, etc.

  ```
  >> show
  [08:t0] 0x00000003    3
  ...
  [17:s1] 0x00000001    1
  [18:s2] 0x00000002    2
  ...
  >> 0x02324020		// add t0 t1 t2
  >> show t0
  [08:t0] 0x00000003    3
  ```

- `load_program()` is to load a program into the memory. The program should be loaded from `INITIAL_PC`, and ends with the `halt` instruction. Assume a file "program" contains following instruction;

  ```
  0x02324020  >> add t0 s1 s2
  0x8e090000  >> lw t1 s0 0
  0x8e0a0004  >> lw t2 s0 4
  0xae0a0008  >> sw t2 s0 8
  ```

  Then the memory should looks like;

  ```
  >> dump 0x1000 32
  0x00001000:  00 00 00 00
  0x00001004:  00 00 00 00
  0x00001008:  00 00 00 00
  0x0000100c:  00 00 00 00
  0x00001010:  00 00 00 00
  0x00001014:  00 00 00 00
  0x00001018:  00 00 00 00
  0x0000101c:  00 00 00 00
  >> load program
  >> dump 0x1000 32
  0x00001000:  02 32 40 20    . 2 @
  0x00001004:  8e 09 00 00    . . . .
  0x00001008:  8e 0a 00 04    . . . .
  0x0000100c:  ae 0a 00 08    . . . .
  0x00001010:  ff ff ff ff    . . . .
  0x00001014:  00 00 00 00    . . . .
  0x00001018:  00 00 00 00    . . . .
  0x0000101c:  00 00 00 00    . . . .
  ```

  Note that the halt instruction is inserted at  0x00001010 (0xffffffff) after the program.

- `run_program()` is to execute the loaded program. Start executing the program from `INITIAL_PC` until it meets the halt instruction. You can actually use `process_instruction()` in this function.



### Comments

- Use the program for PA1 to generate the machine codes for the testing input. Further, you can paste your `translate()` function from PA1 into pa2.c and compile with `-DINPUT_ASSEMBLY` to make the program inputs assembly instead of machine code!
- Use `strtoimax/strtol` to implement `load_program()`.
- Be careful to handle endianness. Sometimes, values seem to be flipped because MIPS is big endian whereas x86 is little endian. If you `printf()` an integer value, it will be interpreted as little endian!!!
- You can make a pull request to correct mistakes / bugfixes. This will be accounted for your bonus points.
- From this project, some testcases will show only the decision (i.e., pass or fail) not the details. However, the their results will be used for grading.
- Do not modify codes in restriction zones. However, you can do whatever you want outside the zones.


### Submission / Grading

- Use [PAsubmit](https://sslab.ajou.ac.kr/pasubmit) for submission
	- 420 pts + 10 pts
	- Some testcases might wil be hidden

- Source: pa2.c (400 pts)
	- Basic execution: 200 pts (10 pts x 20 instructions)
	- Load program: 40 pts
	- Run program: 160 pts
    - Basic: 60 pts
    - With jumps and loops: 100 pts

- Document: one PDF document (20 pts) including;
	- How you process the MIPS instructions
	- How you load the program into the memory
	- How you run the program
	- Lesson learned (if you have any)
	- No more than three pages

- Git repository (HTTP URL) at http://git.ajou.ac.kr (10 pts). Use deploy token and deploy password.

- WILL NOT ANSWER THE QUESTIONS ABOUT THOSE ALREADY SPECIFIED ON THE HANDOUT.
