#pragma once
#include <cstdint>
#include <vector>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <d3d11.h>
#include <tchar.h>
#include <windows.h>
#include <dbghelp.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Dbghelp.lib")

#define UNITSIZE 64
//x가 false라면 강제 중단함
#define CHECK(x) { if (!x) __debugbreak(); }
#define STACK(x) { PrintStackTrace(x); }
typedef uint64_t uuid;

typedef enum AssetType
{
	image,
	sound,
	prefab,
	scene,
	script
} assetType;

void PrintStackTrace(unsigned int skipFrames = 1)
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