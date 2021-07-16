#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include "stdlib.h"

using namespace std;

unsigned int pc = 0x0;
char memory[8*1024];	// only 8KB of memory located at address 0

void emitError(string s)
{
	cout << s << endl;
	system("pause");
	exit(0);
}

void instDecExec(unsigned int instWord)
{
	unsigned int
		opcode = instWord & 0x0000007F,
		rd = (instWord >> 7) & 0x0000001F,
		funct3 = (instWord >> 12) & 0x00000007,
		rs1 = (instWord >> 15) & 0x0000001F,
		rs2 = (instWord >> 20) & 0x0000001F,
		funct7 = (instWord >> 25) & 0x0000007F;

	// â€” inst[31] â€” inst[30:25] inst[24:21] inst[20]
	unsigned int 
		I_imm = ((instWord >> 20) & 0x7FF) | (((instWord >> 31) ? 0xFFFFF800 : 0x0)),
		S_imm,
		B_imm,
		U_imm,
		J_imm;

	unsigned int address;
	unsigned int instPC = pc - 4;

	cout << "0x" << hex << setfill('0') << setw(8) << instPC << "\t0x" << setw(8) << instWord << '\t' << dec;

	if(opcode == 0x33)	// R Instructions
		switch (funct3)
		{
		case 0:
			if(funct7 == 32) cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			else cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			break;
		default: cout << "\tUnkown R Instruction \n";
		}
	else if (opcode == 0x13)	// I instructions
		switch(funct3)
		{
		case 0:	cout << "\tADDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			break;
		default: cout << "\tUnkown I Instruction \n";
		}
	else
		cout << "\tUnkown Instruction \n";
}

int main(int argc, char* argv[])
{
	unsigned int instWord = 0;
	ifstream inFile;
	ofstream outFile;

	if (argc < 2) emitError("use: rvcdiss <machine_code_file_name>\n");

	inFile.open(argv[1], ios::in | ios::binary | ios::ate);

	if (inFile.is_open())
	{
		int fsize = inFile.tellg();
		inFile.seekg(0, inFile.beg);

		if (!inFile.read((char*)memory, fsize)) emitError("Cannot read from input file\n");

		while (true)
		{
			instWord = (unsigned char)memory[pc] |
				(((unsigned char)memory[pc + 1]) << 8) |
				(((unsigned char)memory[pc + 2]) << 16) |
				(((unsigned char)memory[pc + 3]) << 24);

			pc += 4;

			// remove the following line once you have a complete simulator
			if (pc == 40) break;	// stop when PC reached address 32
			instDecExec(instWord);
		}
	}
	else
		emitError("Cannot access input file\n");
	system("pause");
	return 0;
}
