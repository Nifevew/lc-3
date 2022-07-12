#pragma once

#include <stdint.h>

/// <summary>
/// Класс, реализующий LC-3 виртуальную машину
/// </summary>
class LC_3
{
public:

	void run();

private:

	/// <summary>
	/// Оперативная память 128Кб
	/// </summary>
	uint16_t memory[UINT16_MAX];

	/// <summary>
	/// Доступные регистры
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
	/// Хранение регистров
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
	/// Флаги условий
	/// </summary>
	enum class FLAG
	{
		POS = 1 << 0, // P
		ZRO = 1 << 1, // Z
		NEG = 1 << 2  // N
	};


};



