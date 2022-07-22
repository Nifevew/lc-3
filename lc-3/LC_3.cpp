#include "LC_3.h"


void LC_3::run()
{
	registers[static_cast<int>(REGISTERS::COND)] = static_cast<uint16_t>(FLAG::ZRO);

	const uint16_t PC_START = 0x3000;
	registers[static_cast<int>(REGISTERS::PC)] = PC_START;

	uint16_t operation;
	OPERATORS op;

	while (running == 1)
	{
		instruction = readMemory(registers[static_cast<int>(REGISTERS::PC)]++);
		operation = instruction >> 12;

		op = static_cast<OPERATORS>(operation);

		switch (op)
		{
		case OPERATORS::ADD:
		{
			operatorAdd();
			break;
		}
		case OPERATORS::AND:
		{
			operatorAnd();
			break;
		}
		case OPERATORS::NOT:
		{
			operatorNot();
			break;
		}
		case OPERATORS::BR:
		{
			operatorBr();
			break;
		}
		case OPERATORS::JMP:
		{
			operatorJmp();
			break;
		}
		case OPERATORS::JSR:
		{
			operatorJsr();
			break;
		}
		case OPERATORS::LD:
		{
			operatorLd();
			break;
		}
		case OPERATORS::LDI:
		{
			operatorLdi();
			break;
		}
		case OPERATORS::LDR:
		{
			operatorLdr();
			break;
		}
		case OPERATORS::LEA:
		{
			operatorLea();
			break;
		}
		case OPERATORS::ST:
		{
			operatorSt();
			break;
		}
		case OPERATORS::STI:
		{
			operatorSti();
			break;
		}
		case OPERATORS::STR:
		{
			operatorStr();
			break;
		}
		case OPERATORS::TRAP:
		{
			TRAP();
			break;
		}
		case OPERATORS::RES:
		case OPERATORS::RTI:

		default:
		{
			throw std::runtime_error("Unknown command.");
			break;
		}

		}
	}
}


uint16_t LC_3::singExtend(uint16_t x, int bit_count) const 
{
	if (x >> (bit_count - 1) & 1)		//если первый бит 1 => число отрицательное, тогда число расширяется единицами, а не нулями
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


void LC_3::operatorAdd()
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


void LC_3::operatorAnd()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t r_1 = (instruction >> 6) & 0x7;

	uint16_t imm_flag = (instruction >> 5) & 0x1;

	if (imm_flag == 1)
	{
		uint16_t imm5 = singExtend(instruction & 0x1F, 5);
		registers[r_0] = registers[r_1] & imm5;
	}
	else
	{
		uint16_t r_2 = instruction & 0x7;
		registers[r_0] = registers[r_1] & registers[r_2];
	}

	updateFlags(r_0);
}


void LC_3::operatorNot()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t r_1 = (instruction >> 6) & 0x7;

	registers[r_0] = ~registers[r_1];
	updateFlags(r_0);
}


void LC_3::operatorBr()
{
	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);
	uint16_t cond_flag = (instruction >> 9) & 0x7;

	if (cond_flag & registers[static_cast<int>(REGISTERS::COND)])
	{
		registers[static_cast<int>(REGISTERS::PC)] += pc_offset;
	}
}


void LC_3::operatorJmp()
{
	uint16_t r_1 = (instruction >> 6) & 0x7;
	registers[static_cast<int>(REGISTERS::PC)] = registers[r_1];
}


void LC_3::operatorJsr()
{
	uint16_t long_flag = (instruction >> 11) & 0x1;
	registers[static_cast<uint16_t>(REGISTERS::R7)] = registers[static_cast<uint16_t>(REGISTERS::PC)];

	if (long_flag == 1)
	{
		uint16_t long_pc_offset = singExtend(instruction & 0x7FF, 11);
		registers[static_cast<int>(REGISTERS::PC)] += long_pc_offset; // JSR
	}
	else
	{
		uint16_t r_1 = (instruction >> 6) & 0x7;
		registers[static_cast<int>(REGISTERS::PC)] = registers[r_1]; // JSRR
	}
}


void LC_3::operatorLd()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;

	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);
	registers[r_0] = readMemory(registers[static_cast<int>(REGISTERS::PC)] + pc_offset);

	updateFlags(r_0);
}


void LC_3::operatorLdi()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);

	registers[r_0] = readMemory(readMemory(registers[static_cast<int>(REGISTERS::PC)] + pc_offset));

	updateFlags(r_0);
}


void LC_3::operatorLdr()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t r_1 = (instruction >> 6) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x3F, 6);

	registers[r_0] = readMemory(registers[r_1] + pc_offset);

	updateFlags(r_0);
}


void LC_3::operatorLea()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);

	registers[r_0] = registers[static_cast<int>(REGISTERS::PC)] + pc_offset;

	updateFlags(r_0);
}


void LC_3::operatorSt()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);

	writeMemory(registers[static_cast<int>(REGISTERS::PC)] + pc_offset, registers[r_0]);
}


void LC_3::operatorSti()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);

	writeMemory(readMemory(registers[static_cast<int>(REGISTERS::PC)] + pc_offset), registers[r_0]);
}


void LC_3::operatorStr()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t r_1 = (instruction >> 6) & 0x7;
	uint16_t offset = singExtend(instruction & 0x3F, 6);

	writeMemory(registers[r_1] + offset, registers[r_0]);
}


void LC_3::TRAP()
{
	registers[static_cast<int>(REGISTERS::R7)] = registers[static_cast<int>(REGISTERS::PC)];

	TRAPS trap = static_cast<TRAPS>(instruction & 0xFF);

	switch (trap)
	{
	case TRAPS::GETC:
	{
		trapGetc();
		break;
	}
	case TRAPS::OUT:
	{
		trapOut();
		break;
	}
	case TRAPS::PUTS:
	{
		trapPuts();
		break;
	}
	case TRAPS::IN:
	{
		trapIn();
		break;
	}
	case TRAPS::PUTSP:
	{
		trapPutsp();
		break;
	}
	case TRAPS::HALT:
	{
		trapHalt();
		break;
	}
	default:
		break;
	}
}


void LC_3::trapGetc()
{
	registers[static_cast<int>(REGISTERS::R0)] = static_cast<uint16_t>(std::cin.get());
	updateFlags(static_cast<uint16_t>(REGISTERS::R0));
}


void LC_3::trapOut()
{
	std::cout << static_cast<char>(registers[static_cast<int>(REGISTERS::R0)]);
}


void LC_3::trapIn()
{
	std::cout << "Enter a character: ";
	registers[static_cast<int>(REGISTERS::R0)] = static_cast<uint16_t>(std::cin.get());
	updateFlags(static_cast<uint16_t>(REGISTERS::R0));
}


void LC_3::trapPuts()
{
	int i = registers[static_cast<int>(REGISTERS::R0)];

	while (memory[i] != 0x0000)
	{
		std::cout << static_cast<char>(memory[i]);
		i++;
	}
}


void LC_3::trapPutsp()
{
	int i = registers[static_cast<int>(REGISTERS::R0)];

	while (memory[i] != 0x0000)
	{
		char c_1 = memory[i] & 0xFF;
		std::cout << c_1;

		char c_2 = memory[i] >> 8;
		if (c_2)
		{
			std::cout << c_2;
		}

		i++;
	}
}


void LC_3::trapHalt()
{
	std::cout << "HALT";

	running = 0;
}


bool LC_3::loadProgram(std::string program_path)
{
	std::ifstream file{ program_path, std::ios::in | std::ios::binary };
	if (!file.is_open())
		return false;

	uint16_t origin;

	char byte;
	unsigned char ubyte;

	file.read(&byte, 1);
	ubyte = static_cast<unsigned char>(byte);
	origin = (ubyte << 8);

	file.read(&byte, 1);
	ubyte = static_cast<unsigned char>(byte);
	origin = origin | byte;

	int max_read = MAX_MEMORY - origin;
	int i = origin;
	uint16_t tmp = 0;
	std::size_t read = 0;

	while (read < max_read)
	{
		file.read(&byte, 1);
		ubyte = static_cast<unsigned char>(byte);
		tmp = (ubyte << 8);

		file.read(&byte, 1);
		ubyte = static_cast<unsigned char>(byte);
		tmp = tmp | ubyte;

		memory[i] = tmp;
		i++;
		read += 2;
	}

	file.close();

	return true;
}


void LC_3::writeMemory(uint16_t address, uint16_t value)
{
	memory[address] = value;
}


uint16_t LC_3::readMemory(uint16_t address)
{
	if (address == static_cast<uint16_t>(MR::KBSR))
	{
		if (checkKey())
		{
			memory[static_cast<int>(MR::KBSR)] = (1 << 15);
			memory[static_cast<int>(MR::KBDR)] = std::cin.get();
		}
		else
		{
			memory[static_cast<int>(MR::KBSR)] = 0;
		}
	}

	return memory[address];
}


uint16_t LC_3::checkKey()
{
	return WaitForSingleObject(hStdin, 1000) == WAIT_OBJECT_0 && _kbhit();
}


void LC_3::disableInputBuffering()
{
	GetConsoleMode(hStdin, &fdwOldMode);

	fdwMode = fdwOldMode ^ ENABLE_ECHO_INPUT ^ ENABLE_LINE_INPUT; 
	
	SetConsoleMode(hStdin, fdwMode);
	
	FlushConsoleInputBuffer(hStdin); 
}


void LC_3::restoreInputBuffering()
{
	SetConsoleMode(hStdin, fdwOldMode);
}