#include "engine.h"

void PrintStackTrace(unsigned int skipFrames)
{
    HANDLE process = GetCurrentProcess();

    static bool initialized = false;
    if (!initialized)
    {
        SymInitialize(process, nullptr, TRUE);
        initialized = true;
    }

    void* stack[64];
    USHORT frameCount = CaptureStackBackTrace(skipFrames, 64, stack, nullptr);

    SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    if (symbol == nullptr)
        return;

    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = 255;

    DWORD64 displacement = 0;

    for (USHORT i = 0; i < frameCount; i++)
    {
        DWORD64 address = (DWORD64)(stack[i]);

        if (SymFromAddr(process, address, &displacement, symbol))
        {
            std::cout << i << ": " << symbol->Name
                << " [0x" << std::hex << symbol->Address << std::dec << "]\n";
        }
        else
        {
            std::cout << i << ": " << "Unknown"
                << " [0x" << std::hex << address << std::dec << "]\n";
        }
    }

    free(symbol);
}