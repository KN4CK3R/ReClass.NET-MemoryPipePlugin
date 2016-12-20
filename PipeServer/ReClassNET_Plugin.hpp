#pragma once

#include <type_traits>
#include <cstdint>

// Types

using RC_Pointer = void*;
using RC_Size = size_t;
using RC_UnicodeChar = wchar_t;

// Constants

const int PATH_MAXIMUM_LENGTH = 260;

// Enumerations

enum class ProcessAccess
{
	Read,
	Write,
	Full
};

enum class SectionProtection
{
	NoAccess = 0,

	Read = 1,
	Write = 2,
	Execute = 4,

	Guard = 8
};

inline SectionProtection operator|(SectionProtection lhs, SectionProtection rhs)
{
	using T = std::underlying_type_t<SectionProtection>;

	return static_cast<SectionProtection>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline SectionProtection& operator|=(SectionProtection& lhs, SectionProtection rhs)
{
	using T = std::underlying_type_t<SectionProtection>;

	lhs = static_cast<SectionProtection>(static_cast<T>(lhs) | static_cast<T>(rhs));

	return lhs;
}

enum class SectionType
{
	Unknown,

	Private,
	Mapped,
	Image
};

enum class SectionCategory
{
	Unknown,
	CODE,
	DATA,
	HEAP
};

enum class ControlRemoteProcessAction
{
	Suspend,
	Resume,
	Terminate
};

enum class DebugContinueStatus
{
	Handled,
	NotHandled
};

enum class HardwareBreakpointRegister
{
	InvalidRegister,

	Dr0,
	Dr1,
	Dr2,
	Dr3
};

enum class HardwareBreakpointTrigger
{
	Execute,
	Access,
	Write,
};

enum class HardwareBreakpointSize
{
	Size1 = 1,
	Size2 = 2,
	Size4 = 4,
	Size8 = 8
};

enum class DebugEventType
{
	CreateProcess,
	ExitProcess,
	CreateThread,
	ExitThread,
	LoadDll,
	UnloadDll,
	Exception
};

// Structures

#pragma pack(push, 1)

struct EnumerateProcessData
{
	RC_Size Id;
	RC_UnicodeChar ModulePath[PATH_MAXIMUM_LENGTH];
};

struct InstructionData
{
	int Length;
	uint8_t Data[15];
	RC_UnicodeChar Instruction[64];
};

struct EnumerateRemoteSectionData
{
	RC_Pointer BaseAddress;
	RC_Size Size;
	SectionType Type;
	SectionCategory Category;
	SectionProtection Protection;
	RC_UnicodeChar Name[16];
	RC_UnicodeChar ModulePath[PATH_MAXIMUM_LENGTH];
};

struct EnumerateRemoteModuleData
{
	RC_Pointer BaseAddress;
	RC_Size Size;
	RC_UnicodeChar Path[PATH_MAXIMUM_LENGTH];
};

struct CreateProcessDebugInfo
{
	RC_Pointer FileHandle;
	RC_Pointer ProcessHandle;
};

struct ExitProcessDebugInfo
{
	RC_Size ExitCode;
};

struct CreateThreadDebugInfo
{
	RC_Pointer ThreadHandle;
};

struct ExitThreadDebugInfo
{
	RC_Size ExitCode;
};

struct LoadDllDebugInfo
{
	RC_Pointer FileHandle;
	RC_Pointer BaseOfDll;
};

struct UnloadDllDebugInfo
{
	RC_Pointer BaseOfDll;
};

struct ExceptionDebugInfo
{
	RC_Size ExceptionCode;
	RC_Size ExceptionFlags;
	RC_Pointer ExceptionAddress;

	HardwareBreakpointRegister CausedBy;

	bool IsFirstChance;

	struct RegisterInfo
	{
#ifdef _WIN64
		RC_Pointer Rax;
		RC_Pointer Rbx;
		RC_Pointer Rcx;
		RC_Pointer Rdx;
		RC_Pointer Rdi;
		RC_Pointer Rsi;
		RC_Pointer Rsp;
		RC_Pointer Rbp;
		RC_Pointer Rip;

		RC_Pointer R8;
		RC_Pointer R9;
		RC_Pointer R10;
		RC_Pointer R11;
		RC_Pointer R12;
		RC_Pointer R13;
		RC_Pointer R14;
		RC_Pointer R15;
#else
		RC_Pointer Eax;
		RC_Pointer Ebx;
		RC_Pointer Ecx;
		RC_Pointer Edx;
		RC_Pointer Edi;
		RC_Pointer Esi;
		RC_Pointer Esp;
		RC_Pointer Ebp;
		RC_Pointer Eip;
#endif
	};
	RegisterInfo Registers;
};

struct DebugEvent
{
	DebugContinueStatus ContinueStatus;

	RC_Pointer ProcessId;
	RC_Pointer ThreadId;

	DebugEventType Type;

	union
	{
		CreateProcessDebugInfo CreateProcessInfo;
		ExitProcessDebugInfo ExitProcessInfo;
		CreateThreadDebugInfo CreateThreadInfo;
		ExitThreadDebugInfo ExitThreadInfo;
		LoadDllDebugInfo LoadDllInfo;
		UnloadDllDebugInfo UnloadDllInfo;
		ExceptionDebugInfo ExceptionInfo;
	};
};

#pragma pack(pop)

// Callbacks

typedef void(__stdcall *EnumerateProcessCallback)(EnumerateProcessData* data);

typedef void(__stdcall EnumerateRemoteSectionsCallback)(EnumerateRemoteSectionData* data);
typedef void(__stdcall EnumerateRemoteModulesCallback)(EnumerateRemoteModuleData* data);
