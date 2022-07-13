#pragma once

#include <stdint.h>
#include <stdio.h>

#include <iostream>

#include <Windows.h>
#include <conio.h>  // _kbhit


#undef OUT
#undef IN

/// <summary>
/// Класс, реализующий LC-3 виртуальную машину
/// </summary>
class LC_3
{
public:

	void run();

private:

	uint16_t instruction;
	int running;


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


	/// <summary>
	/// Расширение значений числа в дополнительном коде
	/// 
	/// Пример:
	///		из 5-битного числа сделать 16-битное, с учетом знака
	/// 
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
		GETC = 0x20,
		OUT = 0x21,
		PUTS = 0x22,
		IN = 0x23,
		PUTSP = 0x24,
		HALT = 0x25
	};

	void TRAP();

	void Getc();
	void Out();
	void Puts();
	void In();
	void Putsp();
	void Halt();

	void read_image_file(FILE* file);
	uint16_t swap16(uint16_t x);
	int read_image(const char* image_path);

	enum class MR
	{
		KBSR = 0xFE00,
		KBDR = 0xFE02
	};

	void mem_write(uint16_t address, uint16_t value);
	uint16_t mem_read(uint16_t address);

	HANDLE hStdin = INVALID_HANDLE_VALUE;
	uint16_t check_key();
};



