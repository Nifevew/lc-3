#pragma once

#include <stdint.h>
#include <signal.h>

#include <iostream>
#include <fstream>

#include <Windows.h>
#include <conio.h>  // _kbhit


#undef OUT
#undef IN


/// <summary>
/// �����, ����������� LC-3 ����������� ������
/// </summary>
class LC_3
{
public:

	LC_3() 
	{
		int running = 1;
		instruction = 0;
		
		memory = new uint16_t[MAX_MEMORY];
		registers = new uint16_t[static_cast<int>(REGISTERS::COUNT)];

		hStdin = GetStdHandle(STD_INPUT_HANDLE);
	}

	void run(const char* image_path);

	~LC_3()
	{

		delete[] registers;
		delete[] memory;
		

		std::cout << "destructor" << std::endl;
	}

private:
	
	uint16_t instruction;
	int running;
	const uint16_t MAX_MEMORY = (1 << 16);

	/// <summary>
	/// ����������� ������ 128��
	/// </summary>
	//uint16_t memory[UINT16_MAX] = {}; // TODO: ������� � �����������
	uint16_t* memory;

	/// <summary>
	/// ��������� ��������
	/// </summary>
	enum class REGISTERS
	{
		R0 = 0, 
		R1, 
		R2, 
		R3, 
		R4, 
		R5, 
		R6, 
		R7, 
		PC, 
		COND, 
		COUNT
	};

	/// <summary>
	/// �������� ���������
	/// </summary>
	//uint16_t registers[static_cast<int>(REGISTERS::COUNT)];
	uint16_t* registers;

	enum class OPERATORS
	{
		BR = 0, /* branch */
		ADD,    /* add  */
		LD,     /* load */
		ST,     /* store */
		JSR,    /* jump register */
		AND,    /* bitwise and */
		LDR,    /* load register */
		STR,    /* store register */
		RTI,    /* unused */
		NOT,    /* bitwise not */
		LDI,    /* load indirect */
		STI,    /* store indirect */
		JMP,    /* jump */
		RES,    /* reserved (unused) */
		LEA,    /* load effective address */
		TRAP    /* execute trap */
	};

	/// <summary>
	/// ����� �������
	/// </summary>
	enum class FLAG
	{
		POS = 1 << 0, // P
		ZRO = 1 << 1, // Z
		NEG = 1 << 2  // N
	};


	/// <summary>
	/// ���������� �������� ����� � �������������� ����
	/// </summary>
	uint16_t singExtend(uint16_t x, int bit_count) const;

	/// <summary>
	/// 
	/// </summary>
	void updateFlags(uint16_t r);


	void Add();
	void Ldi();
	void And();
	void Not();
	void Br();
	void Jmp();
	void Jsr();
	void Ld();
	void Ldr();
	void St();
	void Sti();
	void Str();
	void Lea();

	enum class TRAPS
	{
		GETC	= 0x20,
		OUT		= 0x21,
		PUTS	= 0x22,
		IN		= 0x23,
		PUTSP	= 0x24,
		HALT	= 0x25
	};

	void TRAP();

	void Getc();
	void Out();
	void Puts();
	void In();
	void Putsp();
	void Halt();

	void read_image_file(std::ifstream& file);
	int read_image(const char* image_path);

	enum class MR
	{
		KBSR = 0xFE00,
		KBDR = 0xFE02
	};

	void writeMemory(uint16_t address, uint16_t value);
	uint16_t readMemory(uint16_t address);

	HANDLE hStdin;
	uint16_t check_key();

	// windows console methods
	DWORD fdwMode;
	DWORD fdwOldMode;

	void disable_input_buffering();
	void restore_input_buffering();

	static void handle_interrupt(int signal);

};