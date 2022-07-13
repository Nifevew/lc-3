#include "LC_3.h"


void LC_3::run()
{
	registers[static_cast<int>(REGISTERS::COND)] = static_cast<uint16_t>(FLAG::ZRO);

	enum {PC_START = 0x3000};  // По умолчанию старт системы на 3000 адресе
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


void LC_3::NOT(uint16_t instruction)
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t r_1 = (instruction >> 6) & 0x7;

	registers[r_0] = ~registers[r_1];
	updateFlags(r_0);
}


void LC_3::BR(uint16_t instruction)
{
	uint16_t pc_offset = singExtend(instruction & 0x1FFF, 9);
	uint16_t cond_flag = (instruction >> 9) & 0x7;

	if (cond_flag && registers[static_cast<int>(REGISTERS::COND)])
	{
		registers[static_cast<int>(REGISTERS::PC)] += pc_offset;
	}
}


void LC_3::JMP(uint16_t instruction)
{
	uint16_t r_1 = (instruction >> 6) & 0x7;
	registers[static_cast<int>(REGISTERS::PC)] = registers[r_1];
}


void LC_3::JSR(uint16_t instruction)
{
	uint16_t long_flag = (instruction >> 11) & 0x1;

	if (long_flag == 1)
	{
		uint16_t long_pc_offset = singExtend(instruction & 0x7FF, 11);
		registers[static_cast<int>(REGISTERS::PC)] += long_pc_offset; // JSR
	}
	else
	{
		uint16_t r_1 = (instruction >> 6) & 0x7;
		registers[static_cast<int>(REGISTERS::PC)] = registers[r_1];
	}
}


void LC_3::LD(uint16_t instruction)
{
	uint16_t r_0 = (instruction >> 9) & 0x7;

	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);
	registers[r_0] = mem_read(registers[static_cast<int>(REGISTERS::PC)] + pc_offset);

	updateFlags(r_0);
}


void LC_3::LDR(uint16_t instruction)
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t r_1 = (instruction >> 6) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x3F, 6);

	registers[r_0] = mem_read(registers[static_cast<int>(REGISTERS::PC)] + pc_offset);

	updateFlags(r_0);
}


void LC_3::ST(uint16_t instruction)
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);

	mem_write(registers[static_cast<int>(REGISTERS::PC)] + pc_offset, registers[r_0]);
}


void LC_3::STI(uint16_t instruction)
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);

	mem_write(mem_read(registers[static_cast<int>(REGISTERS::PC)] + pc_offset), registers[r_0]);
}


void LC_3::STR(uint16_t instruction)
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t r_1 = (instruction >> 6) & 0x7;
	uint16_t offset = singExtend(instruction & 0x3F, 6);

	mem_write(registers[r_1] + offset, registers[r_0]);
}


void LC_3::LEA(uint16_t instructon)
{
	uint16_t r_0 = (instructon >> 9) & 0x7;
	uint16_t pc_offset = singExtend(instructon & 0x1FF, 9);

	registers[r_0] = registers[static_cast<int>(REGISTERS::PC)] + pc_offset;

	updateFlags(r_0);
}


void LC_3::TRAP(uint16_t instructon)
{
	registers[static_cast<int>(REGISTERS::R7)] = registers[static_cast<int>(REGISTERS::PC)];

	TRAPS trap = static_cast<TRAPS>(instructon & 0xFF);

	switch (trap)
	{
	case TRAPS::GETC:
	{
		GETC();
		break;
	}
	case TRAPS::OUT:
	{
		OUT();
		break;
	}
	case TRAPS::PUTS:
	{
		PUTS();
		break;
	}
	case TRAPS::IN:
	{
		IN();
		break;
	}
	case TRAPS::PUTSP:
	{
		PUTSP();
		break;
	}
	case TRAPS::HALT:
	{
		HALT();
		break;
	}
	default:
		break;
	}
}


void LC_3::GETC()
{
	registers[static_cast<int>(REGISTERS::R0)] = static_cast<uint16_t>(getchar()); // TODO: заменить getchar с++ реализацией
}


void LC_3::OUT()
{
	putc((char)registers[static_cast<int>(REGISTERS::R0)], stdout);  // std::cout << static_cast<char>(*c);
	fflush(stdout);
}


void LC_3::IN()
{
	printf("Enter a character: ");
	registers[static_cast<int>(REGISTERS::R0)] = static_cast<uint16_t>(getchar()); // TODO: заменить getchar с++ реализацией
}


void LC_3::PUTS()
{
	uint16_t* c = memory + registers[static_cast<int>(REGISTERS::R0)];

	while (*c)
	{
		putc((char)*c, stdout);  // std::cout << static_cast<char>(*c);
		c++;
	}

	fflush(stdout); // можно убрать?
}


void LC_3::PUTSP()
{
	uint16_t* c = memory + registers[static_cast<int>(REGISTERS::R0)];

	while (*c)
	{
		char c_1 = (*c) & 0xFF;
		putc(c_1, stdout); // TODO: std::cout << c_1;
		
		char c_2 = (*c) >> 8;
		if (c_2)
			putc(c_2, stdout); //TODO: std::cout << c_2

		c++;
	}

	fflush(stdout); //TODO: можно убрать?
}


void LC_3::HALT(int& running)
{
	puts("HALT"); //TODO:
	fflush(stdout);

	running = 0;
}


void LC_3::read_image_file(FILE* file)
{
	uint16_t origin;
	fread(&origin, sizeof(origin), 1, file);
	origin = swap16(origin);

	uint16_t max_read = UINT16_MAX - origin;
	uint16_t* p = memory + origin;
	std::size_t read = fread(p, sizeof(uint16_t), max_read, file);

	while (read-- > 0)
	{
		*p = swap16(*p);
		p++;
	}
}


uint16_t swap16(uint16_t x)
{
	return (x << 8) | (x >> 8);
}


int LC_3::read_image(const char* image_path)
{
	FILE* file = fopen(image_path, "rb");
	if (!file)
		return 0;

	read_image_file(file);
	fclose(file);

	return 1;
}
