#include "LC_3.h"

void LC_3::run()
{
	registers[static_cast<int>(REGISTERS::COND)] = static_cast<uint16_t>(FLAG::ZRO);

	enum {PC_START = 0x3000};  // ѕо умолчанию старт системсы на 3000 адресе
	registers[static_cast<int>(REGISTERS::PC)] = static_cast<uint16_t>(PC_START);

	uint16_t instruction;
	uint16_t operation;

	int running = 1;
	while (running)
	{
		instruction = mem_read(registers[static_cast<int>(REGISTERS::PC)]++);
		operation = instruction >> 12;


	}
}
