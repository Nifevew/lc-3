#pragma once

#include <stdint.h>

/// <summary>
/// �����, ����������� LC-3 ����������� ������
/// </summary>
class LC_3
{
public:

	void run();

private:

	/// <summary>
	/// ����������� ������ 128��
	/// </summary>
	uint16_t memory[UINT16_MAX];

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
	uint16_t registers[static_cast<int>(REGISTERS::COUNT)];

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
	/// 
	/// ������:
	///		�� 5-������� ����� ������� 16-������, � ������ �����
	/// 
	/// </summary>
	uint16_t singExtend(uint16_t x, int bit_count) const;

	/// <summary>
	/// 
	/// </summary>
	void updateFlags(uint16_t r);


	void ADD(uint16_t instruction);
};



