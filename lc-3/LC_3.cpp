#include "LC_3.h"


void LC_3::run()
{
	registers[static_cast<int>(REGISTERS::COND)] = static_cast<uint16_t>(FLAG::ZRO);

	enum {PC_START = 0x3000};  // По умолчанию старт системсы на 3000 адресе
	registers[static_cast<int>(REGISTERS::PC)] = static_cast<uint16_t>(PC_START);

	uint16_t instruction;
	uint16_t operation;

	OPERATORS op;

	int running = 1;
	while (running)
	{
		instruction = mem_read(registers[static_cast<int>(REGISTERS::PC)]++);
		operation = instruction >> 12;

		op = static_cast<OPERATORS>(operation);

		switch (op)
		{
		case OPERATORS::ADD:
		{
			ADD(instruction);
			break;
		}
		case OPERATORS::AND:
		{
			AND(instruction);
			break;
		}
		case OPERATORS::NOT:
		{
			NOT(instruction);
			break;
		}
		case OPERATORS::BR:
		{
			BR(instruction);
			break;
		}
		case OPERATORS::JMP:
		{
			JMP(instruction);
			break;
		}
		case OPERATORS::JSR:
		{
			JSR(instruction);
			break;
		}
		case OPERATORS::LD:
		{
			LD(instruction);
			break;
		}
		case OPERATORS::LDI:
		{
			LDI(instruction);
			break;
		}
		case OPERATORS::LDR:
		{
			LDR(instruction);
			break;
		}
		case OPERATORS::LEA:
		{
			LEA(instruction);
			break;
		}
		case OPERATORS::ST:
		{
			ST(instruction);
			break;
		}
		case OPERATORS::STI:
		{
			STI(instruction);
			break;
		}
		case OPERATORS::STR:
		{
			STR(instruction);
			break;
		}
		case OPERATORS::TRAP:
		{
			TRAP(instruction);
			break;
		}
		case OPERATORS::RES:
		case OPERATORS::RTI:

		default:
		{
			
			break;
		}

		}
	}
}


// TODO: возможно стоит сделать изменение значения через ссылку
uint16_t LC_3::singExtend(uint16_t x, int bit_count) const 
{
	if (x >> (bit_count - 1) & 1)		//если первый бит 1 => число отрицательное, тогда число раширяется еденицами, а не нулями
	{
		x |= (0xFFFF << bit_count);
	}

	return x;
}


void LC_3::updateFlags(uint16_t r)
{
	if (registers[r] == 0)
	{
		registers[static_cast<int>(REGISTERS::COND)] = static_cast<uint16_t>(FLAG::ZRO);
	}
	else if (registers[r] >> 15)
	{
		registers[static_cast<int>(REGISTERS::COND)] = static_cast<uint16_t>(FLAG::NEG);
	}
	else
	{
		registers[static_cast<int>(REGISTERS::COND)] = static_cast<uint16_t>(FLAG::POS);
	}
}


void LC_3::ADD(uint16_t instruction)
{
	uint16_t r_0 = (instruction >> 9) & 0x7;						// адрес регистра результата
	uint16_t r_1 = (instruction >> 6) & 0x7;						// адрес регистра первого слагаемого

	uint16_t imm_flag = (instruction >> 5) & 0x1;

	if (imm_flag == 1)
	{
		uint16_t imm5 = singExtend(instruction & 0x1F, 5); 
		registers[r_0] = registers[r_1] + imm5;
	}
	else
	{
		uint16_t r_2 = instruction & 0x7;							// адрес регистра второго слагаемого
		registers[r_0] = registers[r_1] + registers[r_2];
	}

	updateFlags(r_0);
}


void LC_3::LDI(uint16_t instruction)
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x1FFF, 9);

	registers[r_0] = mem_read(registers[static_cast<int>(REGISTERS::PC)] + pc_offset);

	updateFlags(r_0);
}


void LC_3::AND(uint16_t instruction)
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t r_1 = (instruction >> 6) & 0x7;

	uint16_t imm_flag = (instruction >> 5) & 0x1;

	if (imm_flag == 1)
	{
		uint16_t imm5 = singExtend(instruction & 0x1F, 5); // TODO: what is 0x1F ???
	}
	else
	{
		uint16_t r_2 = instruction & 0x7;
		registers[r_0] = registers[r_1] & registers[r_2];
	}

	updateFlags(r_0);
}


void LC_3::BR(uint16_t instruction)
{

}
