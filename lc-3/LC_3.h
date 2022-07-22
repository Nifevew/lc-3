#pragma once

#include <stdint.h>
#include <signal.h>

#include <iostream>
#include <fstream>
#include <string>
#include <array>

#include <exception>

#include <Windows.h>
#include <conio.h>  // _kbhit


#undef OUT
#undef IN


/// <summary>
/// �����, ����������� �������� LC-3
/// </summary>
class LC_3
{
public:

	LC_3(std::string path_to_program) 
	{
		if (!loadProgram(path_to_program))
		{
			throw std::runtime_error("Failed to load program: " + path_to_program);
		}

		running = 1;
		instruction = 0;

		fdwMode = NULL;
		fdwOldMode = NULL;

		//memory = {};
		registers = {};

		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		disableInputBuffering();
	}

	/// <summary>
	/// ����� ������� ���������
	/// </summary>
	void run();

	~LC_3()
	{
		restoreInputBuffering();
		std::cout << "destructor" << std::endl;
	}

private:
	/// <summary>
	/// ����, �� *.obj ����� � ���������� ��� LC-3
	/// </summary>
	std::string path_to_program;

	/// <summary>
	/// ������� ����������
	/// </summary>
	uint16_t instruction;

	/// <summary>
	/// ������ ���������
	/// </summary>
	int running;

	/// <summary>
	/// ������������ ������ ������
	/// </summary>
	static const uint16_t MAX_MEMORY = UINT16_MAX;

	/// <summary>
	/// ����������� ������ 128��
	/// </summary>
	std::array<uint16_t, MAX_MEMORY> memory;


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
	/// ������ ��� �������� �������� ���������
	/// </summary>
	std::array<uint16_t, 10> registers;

	/// <summary>
	/// ��������� ������� LC-3
	/// </summary>
	enum class OPERATORS
	{
		BR = 0,		// branch 
		ADD,		// add  
		LD,			// load 
		ST,			// store 
		JSR,		// jump register 
		AND,		// bitwise and 
		LDR,		// load register 
		STR,		// store register 
		RTI,		// unused 
		NOT,		// bitwise not 
		LDI,		// load indirect 
		STI,		// store indirect 
		JMP,		// jump 
		RES,		// reserved (unused) 
		LEA,		// load effective address 
		TRAP		// execute trap 
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
	/// ���������� �������� ������
	/// </summary>
	void updateFlags(uint16_t r);

	void operatorAdd();
	void operatorLdi();
	void operatorAnd();
	void operatorNot();
	void operatorBr();
	void operatorJmp();
	void operatorJsr();
	void operatorLd();
	void operatorLdr();
	void operatorSt();
	void operatorSti();
	void operatorStr();
	void operatorLea();

	/// <summary>
	/// ��������� ����������
	/// </summary>
	enum class TRAPS
	{
		GETC	= 0x20,
		OUT		= 0x21,
		PUTS	= 0x22,
		IN		= 0x23,
		PUTSP	= 0x24,
		HALT	= 0x25
	};

	/// <summary>
	/// ��������� ���������� 
	/// </summary>
	void TRAP();

	void trapGetc();
	void trapOut();
	void trapPuts();
	void trapIn();
	void trapPutsp();
	void trapHalt();

	/// <summary>
	/// �������� ��������� �� *.obj ����� � ���
	/// </summary>
	/// <param name="program_path">: ���� �� ���������</param>
	/// <returns>True, ���� �������; False, ���� ������</returns>
	bool loadProgram(std::string program_path);

	/// <summary>
	/// �������� ��������� ������� ����������
	/// </summary>
	enum class MR
	{
		KBSR = 0xFE00,
		KBDR = 0xFE02
	};

	/// <summary>
	/// ������ �������� � ���
	/// </summary>
	/// <param name="address">: ����� � ���</param>
	/// <param name="value">: ��������</param>
	void writeMemory(uint16_t address, uint16_t value);

	/// <summary>
	/// ������ �� ���
	/// </summary>
	/// <param name="address">: ����� ��� ������</param>
	/// <returns>�������� � ������, 2�����</returns>
	uint16_t readMemory(uint16_t address);

	/// <summary>
	/// �������� �� ������� ������� ����������
	/// </summary>
	/// <returns>������� �������, ���� 0</returns>
	uint16_t checkKey();

	// ���������� ������� Windows

	/// <summary>
	/// ���������� ������������ ������ �������
	/// </summary>
	HANDLE hStdin;

	/// <summary>
	/// ��������� �������
	/// </summary>
	DWORD fdwMode;

	/// <summary>
	/// ������ ��������� �������
	/// </summary>
	DWORD fdwOldMode;

	/// <summary>
	/// ��������� ����������� �������
	/// </summary>
	void disableInputBuffering();

	/// <summary>
	/// ������� ������ ��������� �������
	/// </summary>
	void restoreInputBuffering();
};