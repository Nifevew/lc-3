#include "LC_3.h"


void LC_3::run(const char* image_path)
{
	if (!read_image(image_path))
	{
		printf("failed to load image: %s\n", image_path);
		exit(1);
	}

	//signal(SIGINT, handle_interrupt);
	disable_input_buffering();

	registers[static_cast<int>(REGISTERS::COND)] = static_cast<uint16_t>(FLAG::ZRO);

	enum {PC_START = 0x3000};  // По умолчанию старт системы на 3000 адресе
	registers[static_cast<int>(REGISTERS::PC)] = static_cast<uint16_t>(PC_START);

	//uint16_t instruction;
	uint16_t operation;

	OPERATORS op;

	//int running = 1;
	while (running)
	{
		instruction = mem_read(registers[static_cast<int>(REGISTERS::PC)]++);
		operation = instruction >> 12;

		op = static_cast<OPERATORS>(operation);

		switch (op)
		{
		case OPERATORS::ADD:
		{
			Add();
			break;
		}
		case OPERATORS::AND:
		{
			And();
			break;
		}
		case OPERATORS::NOT:
		{
			Not();
			break;
		}
		case OPERATORS::BR:
		{
			Br();
			break;
		}
		case OPERATORS::JMP:
		{
			Jmp();
			break;
		}
		case OPERATORS::JSR:
		{
			Jsr();
			break;
		}
		case OPERATORS::LD:
		{
			Ld();
			break;
		}
		case OPERATORS::LDI:
		{
			Ldi();
			break;
		}
		case OPERATORS::LDR:
		{
			Ldr();
			break;
		}
		case OPERATORS::LEA:
		{
			Lea();
			break;
		}
		case OPERATORS::ST:
		{
			St();
			break;
		}
		case OPERATORS::STI:
		{
			Sti();
			break;
		}
		case OPERATORS::STR:
		{
			Str();
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
			abort();
			break;
		}

		}
	}

	restore_input_buffering();
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


void LC_3::Add()
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


void LC_3::And()
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


void LC_3::Not()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t r_1 = (instruction >> 6) & 0x7;

	registers[r_0] = ~registers[r_1];
	updateFlags(r_0);
}


void LC_3::Br()
{
	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);
	uint16_t cond_flag = (instruction >> 9) & 0x7;

	if (cond_flag & registers[static_cast<int>(REGISTERS::COND)])
	{
		registers[static_cast<int>(REGISTERS::PC)] += pc_offset;
	}
}


void LC_3::Jmp()
{
	uint16_t r_1 = (instruction >> 6) & 0x7;
	registers[static_cast<int>(REGISTERS::PC)] = registers[r_1];
}


void LC_3::Jsr()
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


void LC_3::Ld()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;

	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);
	registers[r_0] = mem_read(registers[static_cast<int>(REGISTERS::PC)] + pc_offset);

	updateFlags(r_0);
}


void LC_3::Ldi()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);

	registers[r_0] = mem_read(mem_read(registers[static_cast<int>(REGISTERS::PC)] + pc_offset));

	updateFlags(r_0);
}


void LC_3::Ldr()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t r_1 = (instruction >> 6) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x3F, 6);

	registers[r_0] = mem_read(registers[r_1] + pc_offset);

	updateFlags(r_0);
}


void LC_3::Lea()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);

	registers[r_0] = registers[static_cast<int>(REGISTERS::PC)] + pc_offset;

	updateFlags(r_0);
}


void LC_3::St()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);

	mem_write(registers[static_cast<int>(REGISTERS::PC)] + pc_offset, registers[r_0]);
}


void LC_3::Sti()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t pc_offset = singExtend(instruction & 0x1FF, 9);

	mem_write(mem_read(registers[static_cast<int>(REGISTERS::PC)] + pc_offset), registers[r_0]);
}


void LC_3::Str()
{
	uint16_t r_0 = (instruction >> 9) & 0x7;
	uint16_t r_1 = (instruction >> 6) & 0x7;
	uint16_t offset = singExtend(instruction & 0x3F, 6);

	mem_write(registers[r_1] + offset, registers[r_0]);
}


void LC_3::TRAP()
{
	registers[static_cast<int>(REGISTERS::R7)] = registers[static_cast<int>(REGISTERS::PC)];

	TRAPS trap = static_cast<TRAPS>(instruction & 0xFF);

	switch (trap)
	{
	case TRAPS::GETC:
	{
		Getc();
		break;
	}
	case TRAPS::OUT:
	{
		Out();
		break;
	}
	case TRAPS::PUTS:
	{
		Puts();
		break;
	}
	case TRAPS::IN:
	{
		In();
		break;
	}
	case TRAPS::PUTSP:
	{
		Putsp();
		break;
	}
	case TRAPS::HALT:
	{
		Halt();
		break;
	}
	default:
		break;
	}
}


void LC_3::Getc()
{
	registers[static_cast<int>(REGISTERS::R0)] = (uint16_t)getchar();   //static_cast<uint16_t>(getchar()); // TODO: заменить getchar с++ реализацией
	updateFlags(static_cast<uint16_t>(REGISTERS::R0));
}


void LC_3::Out()
{
	std::cout << static_cast<char>(registers[static_cast<int>(REGISTERS::R0)]);

	//putc((char)registers[static_cast<int>(REGISTERS::R0)], stdout);  // std::cout << static_cast<char>(*c);
	//fflush(stdout);
}


void LC_3::In()
{
	printf("Enter a character: ");
	registers[static_cast<int>(REGISTERS::R0)] = static_cast<uint16_t>(getchar()); // TODO: заменить getchar с++ реализацией
	updateFlags(static_cast<uint16_t>(REGISTERS::R0));
}


void LC_3::Puts()
{
	uint16_t* c = memory + registers[static_cast<int>(REGISTERS::R0)];

	while (*c)
	{
		putc((char)*c, stdout);  // std::cout << static_cast<char>(*c);
		c++;
	}

	fflush(stdout); // можно убрать?
}


void LC_3::Putsp()
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


void LC_3::Halt()
{
	puts("HALT"); //TODO:
	fflush(stdout);

	running = 0;
}


void LC_3::read_image_file(FILE* file)
{
	//uint16_t origin;
	//char byte[2];
	//fin.read(byte, 2);

	//origin = static_cast<uint16_t>(byte[2]);
	//origin = swap16(origin);

	//uint16_t max_read = UINT16_MAX - origin;
	//uint16_t* p = memory + origin;

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


uint16_t LC_3::swap16(uint16_t x)
{
	return (x << 8) | (x >> 8);
}


int LC_3::read_image(const char* image_path)
{
	//std::ifstream fin{ image_path, std::ios::in | std::ios::binary };

	//if (!fin.is_open())
	//	return 0;

	//read_image_file(fin);
	//fin.close();

	//return 1;

	FILE* file = fopen(image_path, "rb");
	if (!file)
		return 0;

	read_image_file(file);
	fclose(file);

	return 1;
}


void LC_3::mem_write(uint16_t address, uint16_t value)
{
	memory[address] = value;
}

uint16_t LC_3::mem_read(uint16_t address)
{
	if (address == static_cast<uint16_t>(MR::KBSR))
	{
		if (check_key())
		{
			memory[static_cast<int>(MR::KBSR)] = (1 << 15);
			memory[static_cast<int>(MR::KBDR)] = getchar();
		}
		else
		{
			memory[static_cast<int>(MR::KBSR)] = 0;
		}
	}

	return memory[address];
}


uint16_t LC_3::check_key()
{
	return WaitForSingleObject(hStdin, 1000) == WAIT_OBJECT_0 && _kbhit();
}




void LC_3::disable_input_buffering()
{
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hStdin, &fdwOldMode); /* save old mode */
	fdwMode = fdwOldMode
		^ ENABLE_ECHO_INPUT  /* no input echo */
		^ ENABLE_LINE_INPUT; /* return when one or
								more characters are available */
	SetConsoleMode(hStdin, fdwMode); /* set new mode */
	FlushConsoleInputBuffer(hStdin); /* clear buffer */
}

void LC_3::restore_input_buffering()
{
	SetConsoleMode(hStdin, fdwOldMode);
}

void LC_3::handle_interrupt(int signal)
{
	restore_input_buffering();
	printf("\n");
	exit(-2);
}