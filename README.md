https://github.com/MFathyMA/RV32IC-Disassembler.git
# RV32IC-Disassembler
CSCE 2303 Computer Organization and Assembly Language Programming - First project

Implementation details:
	main function:
		Accepts arguments through the command line.
		Calls emitError function if the arguments are less than 2.
		opens the file name which is given in the second argument as input file, supporting binary operations, and placing the cursor at the end of the file.
		If the file has been opened
			Gets the size of the file.
			Places the cursor at the beginning of the file.
			Calls emitError function if it failed to read from the file.
			loops
				loads 4 bytes starting from the PC position in instWord variable.
				increments PC variable by 4.
				Breaks the loop if instWord variable got 0 value.
				Calls instDecExec function.
		Otherwise, calls emitError function.
		Calls the system("pause") function to let the program wait until the user presses any key.
		returns 0 as an indication of success.
	emitError function:
		Accepts a string containing the error message.
		prints this error message.
		Calls system("pause").
		calls exit(0) as an indication of success.
	instDecExec function:
		Accepts an unsigned integer variable containing the machine code instruction.
		Declares and initializes opcode, rd, rs1, rs2, funct3, and funct7 variables.
		Declares and initializes the needed immediates; I_imm, S_imm, B_imm, U_imm and J_imm.
		Decrements PC variable by 4 to be used in printing the address of the instruction.
		Prints the address of the instruction and the instruction itself as hexadecimal values.
		Calls getABI function thrice to gets the ABI names for rd, rs1, and rs2 registers.
		Checks for the instruction's type and calls printInst function to print the instruction properly.
	getABI function:
		Accepts an unsigned integer containing the code of the register.
		Checks for the register number and returns the ABI name.
	printInst function (4 parameters):
	Accepts 4 strings; mnemonic, rd's ABI, rs1's ABI, and rs2's ABI.
	Prints the instruction.
	printInst function (6 parameters):
		Accepts 3 strings, an unsigned integer, and 2 booleans.
		Prints the mnemonic.
		Prints rs1 if it is not empty and if second boolean memo, which indicates dealing with memory loading/storing, is false.
		Prints the extension of the hexadecimal numbers and sets the representation into the hexadecimals if the first boolean, which indicates that imm variable contains an address, is true.
		Prints the imm variable, which contains either the immediate or the address for branche and JAL instructions.
		Calls printLabel if the address boolean is true.
		Prints the rs1 between parentheses if memo boolean is true.
		Prints a new line.
	printLabel function:
		Accepts an unsigned integer.
		Creates a static array of unsigned integers.
		Loops over the array
			Assigns the address to this possision and breaks the loop if this possision is 0 value (empty).
			Breaks the loop if the unsigned integer (the address) exists at that posision.
		Prints the label.

Limitations:
	The label is not printed at its instruction's address.

Known issues:
	Sometimes the addresses are strange and sometimes they are inaccurate.

Contributions of each team member:
	Mohammed Fathy Mohammed
		Worked on the whole project parts
			Understaning the given skeleton for the project
			Whole C++ implementation
			Searching and reading about some concepts and functionalities
			Reading, learning, and using GitHub with Visual Studio 2019 for consistent development
