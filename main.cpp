
#include <iostream>
#include <string>
#include <stack>

#define DATA_SIZE 30000


static uint8_t data[DATA_SIZE] = { 0 };
static uint16_t data_pointer = 0;
static std::stack<uint16_t> start_loop_indices;

bool interpert_line(const std::string& line)
{
	if (line == "e") return false;

	for (uint16_t line_index = 0; line_index < line.size(); line_index++)
	{
		char ch = line[line_index];

		switch (ch)
		{
		case '.': // output the char in the current cell
			putchar(data[data_pointer]);
			break;
		case ',': // input a char to the current cell
			data[data_pointer] = getchar();
			break;
		case '>': // move pointer to the right
			data_pointer = (data_pointer + 1) % DATA_SIZE;
			break;

		case '<': // move pointer to the left
			data_pointer = (data_pointer - 1) % DATA_SIZE;
			break;

		case '+': // increment current cell
			data[data_pointer] = (data[data_pointer] + 1) % 255;
			break;

		case '-': // decrement current cell
			data[data_pointer] = (data[data_pointer] - 1) % 255;
			break;

		case '[': // start loop
			start_loop_indices.push(line_index);
			break;

		case ']': // if current cell is 0 exit loop else go to start loop 
			if (!start_loop_indices.size())
			{
				fputs("[error] no start loop indicated", stderr);
				goto exit_loop;
			}
			if (data[data_pointer] == 0)
			{
				start_loop_indices.pop();
			}
			else
			{
				line_index = start_loop_indices.top();
			}
			
			break;
		 //default is comment	
		}
	}

exit_loop: 
	return true;
}

int main(int argc, const char** argv)
{

	if (argc == 1) // no flags, enter interpeter mode
	{
		puts("entered interpeter mode! enter 'e' to exit");
		std::string line;
	
		do {
			std::getline(std::cin, line);
		} while (interpert_line(line));
		return 0;
	}

	if (strcmp(argv[1], "-h") == 0 or strcmp(argv[1], "--help") == 0) //help mode
	{
		puts("flags:\n-f --file <path> for interpeting the content of the file\n-s --string <str> for interepting a string");
		return 0;
	}
	
	if (strcmp(argv[1], "-s") == 0 or strcmp(argv[1], "--string") == 0) // string flag
	{
		if (argc == 2)
		{
			fputs("[error] need to specify a string -s --string <str>\n", stderr);
			return -1;
		}
		interpert_line(argv[2]);
		return 0;
	}

	if (strcmp(argv[1], "-f") == 0 or strcmp(argv[1], "--file") == 0) // file flag
	{
		if (argc == 2)
		{
			fputs("[error] need to specify a filepath -f --file <path>\n", stderr);
			return -1;
		}

		FILE* fp;
		size_t file_size;
		char* buffer;
		fopen_s(&fp, argv[2], "r");

		if (!fp)
		{
			fprintf(stderr, "[error] failed to load from filepath: %s", argv[2]);
			return -1;
		}

		fseek(fp, 0L, SEEK_END);
		file_size = ftell(fp);
		rewind(fp);
		buffer = new char[file_size];
		fread(buffer, sizeof(char), file_size, fp);

		std::string line = buffer;

		for (char& ch : line)
		{
			if (ch == '\n') ch = ' ';
		}

		interpert_line(line);
		delete[] buffer;
		fclose(fp);
		return 0;
	}
}