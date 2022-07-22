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
/// Класс, реализующий эмулятор LC-3
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
	/// Метод запуска эмулятора
	/// </summary>
	void run();

	~LC_3()
	{
		restoreInputBuffering();
		std::cout << "destructor" << std::endl;
	}

private:
	/// <summary>
	/// Путь, до *.obj файла с программой для LC-3
	/// </summary>
	std::string path_to_program;

	/// <summary>
	/// Текущая инструкция
	/// </summary>
	uint16_t instruction;

	/// <summary>
	/// Статус программы
	/// </summary>
	int running;

	/// <summary>
	/// Максимальный размер памяти
	/// </summary>
	static const uint16_t MAX_MEMORY = UINT16_MAX;

	/// <summary>
	/// Оперативная память 128Кб
	/// </summary>
	std::array<uint16_t, MAX_MEMORY> memory;


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
	/// Массив для хранения значений регистров
	/// </summary>
	std::array<uint16_t, 10> registers;

	/// <summary>
	/// Доступные команды LC-3
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
	/// </summary>
	uint16_t singExtend(uint16_t x, int bit_count) const;

	/// <summary>
	/// Обновление значений флагов
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
	/// Доступные прерывания
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
	/// Обработка прерываний 
	/// </summary>
	void TRAP();

	void trapGetc();
	void trapOut();
	void trapPuts();
	void trapIn();
	void trapPutsp();
	void trapHalt();

	/// <summary>
	/// Загрузка программы из *.obj файла в ОЗУ
	/// </summary>
	/// <param name="program_path">: путь до программы</param>
	/// <returns>True, если успешно; False, если ошибка</returns>
	bool loadProgram(std::string program_path);

	/// <summary>
	/// Регистры обработки нажатий клавиатуры
	/// </summary>
	enum class MR
	{
		KBSR = 0xFE00,
		KBDR = 0xFE02
	};

	/// <summary>
	/// Запись значений в ОЗУ
	/// </summary>
	/// <param name="address">: адрес в ОЗУ</param>
	/// <param name="value">: значение</param>
	void writeMemory(uint16_t address, uint16_t value);

	/// <summary>
	/// Чтение из ОЗУ
	/// </summary>
	/// <param name="address">: адрес для чтений</param>
	/// <returns>Значение в ячейке, 2байта</returns>
	uint16_t readMemory(uint16_t address);

	/// <summary>
	/// Проверка на нажатие клавиши клавиатуры
	/// </summary>
	/// <returns>Нажатая клавиша, либо 0</returns>
	uint16_t checkKey();

	// Управление консоли Windows

	/// <summary>
	/// Дескриптор стандартного вывода консоли
	/// </summary>
	HANDLE hStdin;

	/// <summary>
	/// Состояние консоли
	/// </summary>
	DWORD fdwMode;

	/// <summary>
	/// Старое состояние консоли
	/// </summary>
	DWORD fdwOldMode;

	/// <summary>
	/// Отключить буферизацию консоли
	/// </summary>
	void disableInputBuffering();

	/// <summary>
	/// Вернуть старое состояние консоли
	/// </summary>
	void restoreInputBuffering();
};