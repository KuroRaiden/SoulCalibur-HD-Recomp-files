// SPDX-License-Identifier: BSD-3-Clause
#include <Windows.h>
#include <DbgHelp.h>
#include <cstdio>
namespace {
LONG WINAPI SaveCrash(EXCEPTION_POINTERS* exception) {
  const HANDLE file = CreateFileW(L"logs/startup-crash.dmp", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file != INVALID_HANDLE_VALUE) {
    MINIDUMP_EXCEPTION_INFORMATION info{GetCurrentThreadId(), exception, FALSE};
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, MiniDumpNormal, &info, nullptr, nullptr);
    CloseHandle(file);
  }
  FILE* report = nullptr;
  fopen_s(&report, "logs/startup-crash.txt", "w");
  if (report) {
    const auto& c = *exception->ContextRecord;
    fprintf(report, "code=%08lx rip=%llx access=%llu address=%llx\nrax=%llx rcx=%llx rdx=%llx r8=%llx r9=%llx r10=%llx r11=%llx r12=%llx r13=%llx r14=%llx r15=%llx\n", exception->ExceptionRecord->ExceptionCode, c.Rip, exception->ExceptionRecord->ExceptionInformation[0], exception->ExceptionRecord->ExceptionInformation[1], c.Rax,c.Rcx,c.Rdx,c.R8,c.R9,c.R10,c.R11,c.R12,c.R13,c.R14,c.R15);
    fprintf(report, "image_base=%llx\n", reinterpret_cast<unsigned long long>(GetModuleHandleW(nullptr)));
    CONTEXT walk = c;
    STACKFRAME64 frame{};
    frame.AddrPC = {walk.Rip, AddrModeFlat};
    frame.AddrStack = {walk.Rsp, AddrModeFlat};
    frame.AddrFrame = {walk.Rbp, AddrModeFlat};
    SymInitialize(GetCurrentProcess(), nullptr, TRUE);
    for (int depth = 0; depth < 32; ++depth) {
      fprintf(report, "frame=%llx\n", frame.AddrPC.Offset);
      if (!StackWalk64(IMAGE_FILE_MACHINE_AMD64, GetCurrentProcess(), GetCurrentThread(), &frame, &walk, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr)) break;
    }
    fclose(report);
  }
  return EXCEPTION_EXECUTE_HANDLER;
}
struct InstallCrashReport { InstallCrashReport() { SetUnhandledExceptionFilter(SaveCrash); } } install;
}
