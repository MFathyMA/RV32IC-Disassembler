#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include "stdlib.h"

using namespace std;

unsigned int pc = 0x0;
char memory[8*1024];	// only 8KB of memory located at address 0
void printInst(string inst, string rd, string rs1, string rs2)
{
	cout << inst << ' ' << rd << ", " << rs1 << ", " << rs2 << '\n';
}

void printInst(string inst, string rd, string rs1, unsigned int imm)
{
	cout << rd << ", " << rs1 << ", " << hex << "0x" << (int)imm << '\n' << dec;
}

string getABI(unsigned int r)
{
	if (!r) return "zero";
	if (r == 1) return "ra";
	if (r == 2) return "sp";
	if (r == 3) return "gp";
	if (r == 4) return "tp";
	if (r >= 5 && r <= 7) return "t" + to_string(r - 5);
	if (r == 8 || r == 9) return "s" + to_string(r - 8);
	if (r >= 10 && r <= 17) return "a" + to_string(r - 10);
	if (r >= 18 && r <= 27) return "s" + to_string(r - 16);
	if (r >= 28 && r <= 31) return "s" + to_string(r - 25);
	return "x" + to_string(r);
}

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
		S_imm = ((instWord >> 7) & 0x1F) | ((instWord >> 20) & 0x7E0) | ((instWord >> 31) ? 0xFFFFF800 : 0x0),
		B_imm = ((instWord >> 7) & 0x1E) | ((instWord >> 20) & 0x7E0) | ((instWord << 4) & 0x800) | ((instWord >> 31) ? 0xFFFFF000 : 0x0),
		U_imm = (instWord & 0xFFFFF000),
		J_imm = ((instWord >> 20) & 0x7FE) | ((instWord >> 9) & 0x800) | (instWord & 0xFF000) | ((instWord >> 31) ? 0xFFF00000 : 0x0);

	unsigned int address;
	unsigned int instPC = pc - 4;

	cout << "0x" << hex << setfill('0') << setw(8) << instPC << "\t0x" << setw(8) << instWord << '\t' << dec;

	string rdABI = getABI(rd);
	string rs1ABI = getABI(rs1);
	string rs2ABI = getABI(rs2);

	if (opcode == 0x33)	// R Instructions
		switch (funct3)
		{
		case 0:
			if (!funct7) printInst("ADD", rdABI, rs1ABI, rs2ABI);
			else printInst("SUB", rdABI, rs1ABI, rs2ABI);
			break;
		case 4: printInst("XOR", rdABI, rs1ABI, rs2ABI);
			break;
		case 6: printInst("OR", rdABI, rs1ABI, rs2ABI);
			break;
		case 7: printInst("AND", rdABI, rs1ABI, rs2ABI);
			break;
		case 1: printInst("SLL", rdABI, rs1ABI, rs2ABI);
			break;
		case 5:
			if (!funct7) printInst("SRL", rdABI, rs1ABI, rs2ABI);
			else printInst("SRA", rdABI, rs1ABI, rs2ABI);
			break;
		case 2: printInst("SLT", rdABI, rs1ABI, rs2ABI);
			break;
		case 3: printInst("SLTU", rdABI, rs1ABI, rs2ABI);
			break;
		default: cout << "\tUnknown R Instruction!\n";
		}
	else if (opcode == 0x13)	// I instructions
		switch (funct3)
		{
		case 0: printInst("ADDI", rdABI, rs1ABI, I_imm);
			break;
		case 4: printInst("XORI", rdABI, rs1ABI, I_imm);
			break;
		case 6: printInst("ORI", rdABI, rs1ABI, I_imm);
			break;
		case 7: printInst("ANDI", rdABI, rs1ABI, I_imm);
			break;
		case 1: printInst("SLLI", rdABI, rs1ABI, I_imm);
			break;
		case 5:
			if (!funct7) printInst("SRLI", rdABI, rs1ABI, rs2);
			else printInst("SRAI", rdABI, rs1ABI, rs2);
			break;
		case 2: printInst("SLTI", rdABI, rs1ABI, I_imm);
			break;
		case 3: printInst("SLTIU", rdABI, rs1ABI, I_imm);
			break;
		default: cout << "\tUnknown I Instruction!\n";
		}
	else
		cout << "\tUnknown Instruction \n";
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
