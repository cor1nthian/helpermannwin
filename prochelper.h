#ifndef _PROCESS_HELPER_H
#define _PROCESS_HELPER_H

/*       In loving memory of @pixo $oft       */
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOKERNEL
#define NOMEMMGR
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NODRAWTEXT
#define NOWINSTYLES
#define NOSHOWWINDOW
#define NOWINMESSAGES
#define NOWH

#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <map>
#include <vector>
#include "config.h"
#include "fshelper.h"
#include "strhelper.h"
#include "syshelper.h"

enum class ProcOpResult : unsigned char {
	Success = 0,
	Fail
};

enum class ProcAwait : unsigned char {
	Infinite = 0,
	ExactTime,
	NoWait
};

struct ProcResource {
	ProcResource();
	ProcResource(const unsigned long PID, unsigned long ExitCode, STARTUPINFO SI,
		PROCESS_INFORMATION PI);
	ProcResource(const ProcResource &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	ProcResource(ProcResource &&other) noexcept;
#endif
	~ProcResource();
	ProcResource& operator=(const ProcResource &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	ProcResource& operator=(ProcResource &&other) noexcept;
#endif
	bool operator==(const ProcResource &other) const;
	bool operator!=(const ProcResource &other) const;
	unsigned long pid;
	unsigned long exitCode;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
};

struct ProcDesc {
	ProcDesc();
	ProcDesc(const unsigned long sz,
		const unsigned long use,
		const unsigned long procid,
		const unsigned long threadn,
		const unsigned long parentpid,
		const unsigned long flgs,
		const unsigned long moduleid,
		const unsigned long long defHeapId,
		const long threadpriority,
		const std::wstring pathexe);
	ProcDesc(const ProcDesc &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	ProcDesc(ProcDesc &&other) noexcept;
#endif
	~ProcDesc();
	ProcDesc& operator=(const ProcDesc &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	ProcDesc& operator=(ProcDesc &&other) noexcept;
#endif
	bool operator==(const ProcDesc &other) const;
	bool operator!=(const ProcDesc &other) const;
	unsigned long size;
	unsigned long usage;
	unsigned long pid;
	unsigned long threadnum;
	unsigned long parentPid;
	unsigned long flags;
	unsigned long moduleId;
	unsigned long long defHeapId;
	long threadPriority;
	std::wstring exepath;
};

class ProcessHandler {
	public:
		ProcessHandler();
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		ProcessHandler(const ProcessHandler &other) = delete;
#else
		ProcessHandler(const ProcessHandler &other) {}
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		ProcessHandler(ProcessHandler &&other) noexcept = delete;
#endif
		~ProcessHandler();
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		ProcessHandler& operator=(const ProcessHandler &other) = delete;
#else
		ProcessHandler& operator=(const ProcessHandler &other) {}
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		ProcessHandler& operator=(ProcessHandler &other) noexcept = delete;
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		bool operator==(const ProcessHandler &other) const = delete;
#else
		bool operator==(const ProcessHandler &other) {}
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		bool operator!=(const ProcessHandler &other) const = delete;
#else
		bool operator!=(const ProcessHandler &other) {}
#endif
		ProcResource StartProc(const std::wstring exepath, const std::wstring args = L"",
			const ProcAwait awaitTime = ProcAwait::Infinite, const unsigned long procAwaitTime = 0,
			const bool freeRes = true);
		bool FreeProcResources(const ProcResource &procRes, bool stopProc = true);
		bool StopProc(const unsigned long pid, const std::vector<ProcDesc> *proclist = 0);
		bool StopProc(const std::wstring exepath, const std::vector<ProcDesc> *proclist = 0);
		bool EnableDebugPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableBackupPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableRestorePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableLockMemoryPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableIncreaseQuotaPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableUnsolicitedInputPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableMachineAccountPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableTcbPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableSecurityPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableTakeOwnershipPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableLoadDriverPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableSystemProfilePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableSystemTimePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableProfSingleProcessPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableIncBasePriorutyPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableCreatePageFilePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableCreatePermanentPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableShutdownPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableAuditPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableSystemEnvironmentPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableChangeNotifyPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableRemoteShutdownPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableUndockPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableSyncAgentPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableDelegationPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableManageVolumePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableImpersonatePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableCreateGlobalPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableTrustedCredManAccessPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableRelabelPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableIncWorkingSetPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableTimezonePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableCreateSymbolicLinkPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableDelegateSessionUserImpersonatePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableCreateTokenPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool EnableAssignPrimaryTokenPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableAssignPrimaryTokenPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableCreateTokenPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableDelegateSessionUserImpersonatePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableCreateSymbolicLinkPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableTimezonePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableIncWorkingSetPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableRelabelPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableTrustedCredManAccessPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableCreateGlobalPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableImpersonatePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableManageVolumePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableDelegationPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableSyncAgentPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableUndockPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableRemoteShutdownPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableChangeNotifyPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableSystemEnvironmentPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableAuditPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableShutdownPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableCreatePermanentPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableCreatePageFilePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableIncBasePriorutyPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableProfSingleProcessPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableSystemTimePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableSystemProfilePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableLoadDriverPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableTakeOwnershipPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableSecurityPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableTcbPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableMachineAccountPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableUnsolicitedInputPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableBackupPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableRestorePrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableLockMemoryPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableIncreaseQuotaPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool DisableDebugPrivilege(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		bool IsProcRunning(const std::wstring exepath, const std::vector<ProcDesc> *proclist = 0);
		std::wstring GetProcessExePathByPid(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS);
		ProcDesc GetProcessByPid(const unsigned long pid, std::vector<ProcDesc> *procList = 0);
		std::vector<ProcDesc> GetProcessByExePath(const std::wstring exePath,
			std::vector<ProcDesc> *procList = 0);
		unsigned long GetCurrentProcPid() const;
		std::vector<std::wstring> GetProcPrivileges(const unsigned long pid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		ProcOpResult RunCommandPiped(const std::wstring command, std::wstring &output, std::wstring &erroutput,
			unsigned char restartNum = 10, unsigned long sleepTime = 1000,
			const ProcAwait awaitTime = ProcAwait::Infinite, const unsigned long procAwaitTime = 60000);
		ProcOpResult GetProcUserSID(const unsigned long pid, ::PSID &sid,
			const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		ProcOpResult ListProcesses(std::vector<ProcDesc> &procList);
		ProcOpResult GetWindowPID(unsigned long &pid, const std::wstring className,
			const std::wstring windowName) const;
	protected:

	private:
		/*       FUNCTIONS       */
		bool privilegeController(const unsigned long pid, const std::wstring privName,
			const bool enable = true, const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
};

#endif // _PROCESS_HELPER_H