// Unpacker for LDPINCH Malware
// Example SHA256: cc65200e7c748e095f65a8d22ecf8618257cc1b2163e1f9df407a0a47ae17b79


#include "stdafx.h"

typedef uint8_t byte;

void unpack(uint32_t start, uint32_t end, byte *buffer) 
{
	for(auto i = start; i <= end; i++)
	{
		buffer[i] ^= 0x89;
	}

	buffer[start] ^= 0x9f;
	byte key = 0x54;
	for(auto i = start + 1; i <= end; i++)
	{
		buffer[i] ^= key;
		key += 0x12;
		key ^= 0x68;
		key -= 0x4;
	}
	
}

byte* readFile(char *path, long *read_bytes)
{
	FILE *file;
	fopen_s(&file, path, "rb");
	fseek(file, 0, SEEK_END);
	*read_bytes = ftell(file);
	fseek(file, 0, SEEK_SET);

	const auto buffer = static_cast<byte*>(malloc(*read_bytes));
	fread(buffer, *read_bytes, 1, file);
	fclose(file);


	return buffer;
}

void writeFile(byte* buffer, const long buf_size, char *path)
{
	FILE *file;
	fopen_s(&file, path, "wb");
	fwrite(buffer, buf_size, 1, file);
	fclose(file);
}

int main(const int argc, char **argv)
{
    if(argc != 3)
    {
	    printf("LDPINCH Unpacker\nUsage: %s [input] [output]", argv[0]);
		return 0;
    }

	long read_bytes = 0;
	const auto buffer = readFile(argv[1], &read_bytes);
	
	// Mapped = 0x10001080 and 0x10002373. Base address is 0x10000000 in Cutter.
	// Physical address of Code section is 0x00000400, virtual address is 0x00001000.
	// First 0x80 bytes are skipped in the decryption.
	unpack(0x480, 0x1773, buffer); 

	writeFile(buffer, read_bytes, argv[2]);

	free(buffer);
}



