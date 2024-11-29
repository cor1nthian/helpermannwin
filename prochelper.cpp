// #include <iostream>
#include "prochelper.h"

ProcResource::ProcResource() {
	pid = 0;
	exitCode = 0;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
}

ProcResource::ProcResource(const unsigned long PID, unsigned long ExitCode, STARTUPINFO SI,
	PROCESS_INFORMATION PI) {
	pid = PID;
	exitCode = ExitCode;
	memcpy(&si, &SI, sizeof(STARTUPINFO));
	memcpy(&pi, &PI, sizeof(PROCESS_INFORMATION));
}

ProcResource::ProcResource(const ProcResource &other) {
	if (this != &other) {
		pid = other.pid;
		exitCode = other.exitCode;
		memcpy(&si, &other.si, sizeof(STARTUPINFO));
		memcpy(&pi, &other.pi, sizeof(PROCESS_INFORMATION));
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
ProcResource::ProcResource(ProcResource &&other) noexcept {
	if (this != &other) {
		pid = std::exchange(other.pid, 0);
		exitCode = std::exchange(other.exitCode, 0);
		// memcpy(&si, &other.si, sizeof(STARTUPINFO));
		si = std::move(other.si);
		memset(&other.si, 0, sizeof(STARTUPINFO));
		// memcpy(&pi, &other.pi, sizeof(PROCESS_INFORMATION));
		pi = std::move(other.pi);
		memset(&other.pi, 0, sizeof(PROCESS_INFORMATION));
	}
}
#endif

ProcResource::~ProcResource() {}

ProcResource& ProcResource::operator=(const ProcResource& other) {
	if (this != &other) {
		pid = other.pid;
		exitCode = other.exitCode;
		memcpy(&si, &other.si, sizeof(STARTUPINFO));
		memcpy(&pi, &other.pi, sizeof(PROCESS_INFORMATION));
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
ProcResource& ProcResource::operator=(ProcResource &&other) noexcept {
	if (this != &other) {
		pid = std::exchange(other.pid, 0);
		exitCode = std::exchange(other.exitCode, 0);
		// memcpy(&si, &other.si, sizeof(STARTUPINFO));
		si = std::move(other.si);
		memset(&other.si, 0, sizeof(STARTUPINFO));
		// memcpy(&pi, &other.pi, sizeof(PROCESS_INFORMATION));
		pi = std::move(other.pi);
		memset(&other.pi, 0, sizeof(PROCESS_INFORMATION));
	}
	return *this;
}
#endif

bool ProcResource::operator==(const ProcResource& other) const {
	if (this != &other) {
		return(pid == other.pid && exitCode == other.exitCode &&
			!memcmp(&si, &other.si, sizeof(STARTUPINFO)) &&
			!memcmp(&pi, &other.pi, sizeof(PROCESS_INFORMATION)));
	} else {
		return true;
	}
}

bool ProcResource::operator!=(const ProcResource& other) const {
	if (this != &other) {
		return(pid != other.pid || exitCode != other.exitCode ||
			memcmp(&si, &other.si, sizeof(STARTUPINFO)) ||
			memcmp(&pi, &other.pi, sizeof(PROCESS_INFORMATION)));
	} else {
		return false;
	}
}

ProcDesc::ProcDesc() {
	size = 0;
	usage = 0;
	pid = 0;
	threadnum = 0;
	parentPid = 0;
	flags = 0;
	moduleId = 0;
	defHeapId = 0;
	threadPriority = 0;
}

ProcDesc::ProcDesc(const unsigned long sz,
	const unsigned long use,
	const unsigned long procid,
	const unsigned long threadn,
	const unsigned long parentpid,
	const unsigned long flgs,
	const unsigned long moduleid,
	const unsigned long long defheapId,
	const long threadpriority,
	const std::wstring pathexe) {
	size = sz;
	usage = use;
	pid = procid;
	threadnum = threadn;
	parentPid = parentpid;
	flags = flgs;
	moduleId = moduleid;
	defHeapId = defheapId;
	threadPriority = threadpriority;
	exepath = pathexe;
}

ProcDesc::ProcDesc(const ProcDesc &other) {
	if (this != &other) {
		size = other.size;
		usage = other.usage;
		pid = other.pid;
		threadnum = other.threadnum;
		parentPid = other.parentPid;
		flags = other.flags;
		moduleId = other.moduleId;
		defHeapId = other.defHeapId;
		threadPriority = other.threadPriority;
		exepath = other.exepath;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
ProcDesc::ProcDesc(ProcDesc &&other) noexcept {
	if (this != &other) {
		size = std::exchange(other.size, 0);
		usage = std::exchange(other.usage, 0);
		pid = std::exchange(other.pid, 0);
		threadnum = std::exchange(other.threadnum, 0);
		parentPid = std::exchange(other.parentPid, 0);
		flags = std::exchange(other.flags, 0);
		moduleId = std::exchange(other.moduleId, 0);
		defHeapId = std::exchange(other.defHeapId, 0);
		threadPriority = std::exchange(other.threadPriority, 0);
		exepath = std::move(other.exepath);
	}
}
#endif

ProcDesc::~ProcDesc() {}

ProcDesc& ProcDesc::operator=(const ProcDesc &other) {
	if (this != &other) {
		size = other.size;
		usage = other.usage;
		pid = other.pid;
		threadnum = other.threadnum;
		parentPid = other.parentPid;
		flags = other.flags;
		moduleId = other.moduleId;
		defHeapId = other.defHeapId;
		threadPriority = other.threadPriority;
		exepath = other.exepath;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
ProcDesc& ProcDesc::operator=(ProcDesc &&other) noexcept {
	if (this != &other) {
		size = std::exchange(other.size, 0);
		usage = std::exchange(other.usage, 0);
		pid = std::exchange(other.pid, 0);
		threadnum = std::exchange(other.threadnum, 0);
		parentPid = std::exchange(other.parentPid, 0);
		flags = std::exchange(other.flags, 0);
		moduleId = std::exchange(other.moduleId, 0);
		defHeapId = std::exchange(other.defHeapId, 0);
		threadPriority = std::exchange(other.threadPriority, 0);
		exepath = std::move(other.exepath);
	}
	return *this;
}
#endif

bool ProcDesc::operator==(const ProcDesc &other) const {
	if (this != &other) {
		return (size == other.size &&
			usage == other.usage &&
			pid == other.pid &&
			threadnum == other.threadnum &&
			parentPid == other.parentPid &&
			flags == other.flags &&
			moduleId == other.moduleId &&
			defHeapId == other.defHeapId &&
			threadPriority == other.threadPriority &&
			lower_copy(exepath) == lower_copy(other.exepath));
	} else {
		return true;
	}
}

bool ProcDesc::operator!=(const ProcDesc &other) const {
	if (this != &other) {
		return (size != other.size ||
			usage != other.usage ||
			pid != other.pid ||
			threadnum != other.threadnum ||
			parentPid != other.parentPid ||
			flags != other.flags ||
			moduleId != other.moduleId ||
			defHeapId != other.defHeapId ||
			threadPriority != other.threadPriority ||
			lower_copy(exepath) != lower_copy(other.exepath));
	} else {
		return false;
	}
}

ProcessHandler::ProcessHandler() {}

ProcessHandler::~ProcessHandler() {}

ProcResource ProcessHandler::StartProc(const std::wstring exepath, const std::wstring args,
	const ProcAwait awaitTime, const unsigned long procAwaitTime, const bool freeRes) {
	std::wstring cmdline = args.length() ? startsWith(args, L" ") ? args : L" " + args : L"";
	ProcResource ret;
	::STARTUPINFO si = { 0 };
	::PROCESS_INFORMATION pi = { 0 };
	memset(&si, 0, sizeof(::STARTUPINFO));
	memset(&pi, 0, sizeof(::PROCESS_INFORMATION));
	if (::CreateProcess((wchar_t*)exepath.c_str(),	// Exe path
		(wchar_t*)cmdline.c_str(),					// Command line (prior space is mandatory)
		0,											// Process handle not inheritable
		0,											// Thread handle not inheritable
		true,										// Set handle inheritance to TRUE
		CREATE_NEW_PROCESS_GROUP,					// Creates new process group
		0,											// Use parent's environment block
		0,											// Use parent's starting directory 
		&si,										// Pointer to STARTUPINFO structure
		&pi)										// Pointer to PROCESS_INFORMATION structure
		) {
		ret.pid = pi.dwProcessId;
		ret.si = si;
		ret.pi = pi;
		if (ProcAwait::Infinite == awaitTime) {
			::WaitForSingleObject(pi.hProcess, INFINITE);
			::GetExitCodeProcess(pi.hProcess, &ret.exitCode);
			if (freeRes) {
				FreeProcResources(ret);
			}
		} else if (ProcAwait::ExactTime == awaitTime) {
			::WaitForSingleObject(pi.hProcess, procAwaitTime);
			::GetExitCodeProcess(pi.hProcess, &ret.exitCode);
			if (freeRes) {
				FreeProcResources(ret);
			}
		} else if (ProcAwait::NoWait == awaitTime) {
			::GetExitCodeProcess(pi.hProcess, &ret.exitCode);
			if (freeRes) {
				FreeProcResources(ret, false);
			}
		}
	}
	return ret;
}

bool ProcessHandler::FreeProcResources(const ProcResource &procRes, bool stopProc) {
	bool funcres = ::CloseHandle(procRes.pi.hProcess) && ::CloseHandle(procRes.pi.hThread);
	if (stopProc) {
		return StopProc(procRes.pid);
	}
	return funcres;
}

bool ProcessHandler::StopProc(const unsigned long pid, const std::vector<ProcDesc> *proclist) {
	std::vector<ProcDesc> procs;
	if (proclist) {
		procs = *proclist;
	} else {
		if (ProcOpResult::Fail == ListProcesses(procs)) {
			return false;
		}
	}
	for (size_t i = 0; i < procs.size(); ++i) {
		if (procs[i].pid == pid) {
			::HANDLE hProc = ::OpenProcess(PROCESS_TERMINATE, false, pid);
			if (INVALID_HANDLE_VALUE != hProc) {
				if (!::TerminateProcess(hProc, 1)) {
					::CloseHandle(hProc);
					return false;
				}
				return ::CloseHandle(hProc);
			} else {
				return false;
			}
		}
	}
	return true;
}

bool ProcessHandler::StopProc(const std::wstring exepath,
	const std::vector<ProcDesc> *proclist) {
	bool ret = false;
	std::vector<ProcDesc> procs;
	if (proclist) {
		procs = *proclist;
	} else {
		if (ProcOpResult::Fail == ListProcesses(procs)) {
			return false;
		}
	}
	for (size_t i = 0; i < procs.size(); ++i) {
		if (lower_copy(procs[i].exepath) == lower_copy(exepath)) {
			::HANDLE hProc = ::OpenProcess(PROCESS_TERMINATE, false, procs[i].pid);
			if (INVALID_HANDLE_VALUE != hProc) {
				if (!::TerminateProcess(hProc, 1)) {
					::CloseHandle(hProc);
					return false;
				}
				return ::CloseHandle(hProc);
			} else {
				return false;
			}
		}
	}
	return true;
}

bool ProcessHandler::EnableDebugPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_DEBUG_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableBackupPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_BACKUP_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableRestorePrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_RESTORE_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableLockMemoryPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_LOCK_MEMORY_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableIncreaseQuotaPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_INCREASE_QUOTA_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableUnsolicitedInputPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_UNSOLICITED_INPUT_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableMachineAccountPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_MACHINE_ACCOUNT_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableTcbPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_TCB_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableSecurityPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_SECURITY_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableTakeOwnershipPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_TAKE_OWNERSHIP_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableLoadDriverPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_LOAD_DRIVER_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableSystemProfilePrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_SYSTEM_PROFILE_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableSystemTimePrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_SYSTEMTIME_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableProfSingleProcessPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_PROF_SINGLE_PROCESS_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableIncBasePriorutyPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_INC_BASE_PRIORITY_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableCreatePageFilePrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_CREATE_PAGEFILE_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableCreatePermanentPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_CREATE_PERMANENT_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableShutdownPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_SHUTDOWN_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableAuditPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_AUDIT_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableSystemEnvironmentPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_SYSTEM_ENVIRONMENT_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableChangeNotifyPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_CHANGE_NOTIFY_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableRemoteShutdownPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_REMOTE_SHUTDOWN_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableUndockPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_UNDOCK_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableSyncAgentPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_SYNC_AGENT_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableDelegationPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_ENABLE_DELEGATION_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableManageVolumePrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_MANAGE_VOLUME_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableImpersonatePrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_IMPERSONATE_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableCreateGlobalPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_CREATE_GLOBAL_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableTrustedCredManAccessPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_TRUSTED_CREDMAN_ACCESS_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableRelabelPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_RELABEL_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableIncWorkingSetPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_INC_WORKING_SET_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableTimezonePrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_TIME_ZONE_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableCreateSymbolicLinkPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_CREATE_SYMBOLIC_LINK_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableDelegateSessionUserImpersonatePrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableCreateTokenPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_CREATE_TOKEN_NAME, true, desiredProcRights);
}

bool ProcessHandler::EnableAssignPrimaryTokenPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_ASSIGNPRIMARYTOKEN_NAME, true, desiredProcRights);
}

bool ProcessHandler::DisableAssignPrimaryTokenPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_ASSIGNPRIMARYTOKEN_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableCreateTokenPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_CREATE_TOKEN_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableDelegateSessionUserImpersonatePrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableCreateSymbolicLinkPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_CREATE_SYMBOLIC_LINK_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableTimezonePrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_TIME_ZONE_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableIncWorkingSetPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_INC_WORKING_SET_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableRelabelPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_RELABEL_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableTrustedCredManAccessPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_TRUSTED_CREDMAN_ACCESS_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableCreateGlobalPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_CREATE_GLOBAL_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableImpersonatePrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_IMPERSONATE_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableManageVolumePrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_MANAGE_VOLUME_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableDelegationPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_ENABLE_DELEGATION_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableSyncAgentPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_SYNC_AGENT_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableUndockPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_UNDOCK_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableRemoteShutdownPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_REMOTE_SHUTDOWN_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableChangeNotifyPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_CHANGE_NOTIFY_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableSystemEnvironmentPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_SYSTEM_ENVIRONMENT_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableAuditPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_AUDIT_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableShutdownPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_SHUTDOWN_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableCreatePermanentPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_CREATE_PERMANENT_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableCreatePageFilePrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_CREATE_PAGEFILE_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableIncBasePriorutyPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_INC_BASE_PRIORITY_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableProfSingleProcessPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_PROF_SINGLE_PROCESS_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableSystemTimePrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_SYSTEMTIME_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableSystemProfilePrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_SYSTEM_PROFILE_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableLoadDriverPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_LOAD_DRIVER_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableTakeOwnershipPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_TAKE_OWNERSHIP_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableSecurityPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_SECURITY_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableTcbPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_TCB_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableMachineAccountPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_MACHINE_ACCOUNT_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableUnsolicitedInputPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_UNSOLICITED_INPUT_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableBackupPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_BACKUP_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableRestorePrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_RESTORE_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableLockMemoryPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_LOCK_MEMORY_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableIncreaseQuotaPrivilege(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_INCREASE_QUOTA_NAME, false, desiredProcRights);
}

bool ProcessHandler::DisableDebugPrivilege(const unsigned long pid, const unsigned long desiredProcRights) const {
	return privilegeController(pid, SE_DEBUG_NAME, false, desiredProcRights);
}

bool ProcessHandler::IsProcRunning(const std::wstring exepath, const std::vector<ProcDesc> *proclist) {
	std::vector<ProcDesc> vec;
	if (proclist) {
		vec = *proclist;
	} else {
		if (ProcOpResult::Fail == ListProcesses(vec)) {
			return false;
		}
	}
	for (size_t i = 0; i < vec.size(); ++i) {
		if (lower_copy(vec[i].exepath) == lower_copy(exepath)) {
			return true;
		}
	}
	return false;
}

std::wstring ProcessHandler::GetProcessExePathByPid(const unsigned long pid, const unsigned long desiredProcRights) {
	std::wstring ret;
	::HANDLE hProc = ::OpenProcess(desiredProcRights, true, pid);
	if (INVALID_HANDLE_VALUE != hProc) {
		NEW_ARR_NULLIFY(buf, wchar_t, 2048);
		if (buf) {
			if (::GetProcessImageFileName(hProc, buf, 2048)) {
				if(startsWith(lower_copy(buf), L"\\device\\harddiskvolume")) {
					FSHandler fsh;
					ret = fsh.ReplaceDrivePathWithPartLetter(buf);
				} else {
					ret = buf;
				}
			}
			SAFE_ARR_DELETE(buf);
		}
		::CloseHandle(hProc);
	}
	return ret;
}

ProcDesc ProcessHandler::GetProcessByPid(const unsigned long pid, std::vector<ProcDesc> *procList) {
	std::vector<ProcDesc> procs;
	ProcDesc ret;
	if (procList) {
		procs = *procList;
	} else {
		if (ProcOpResult::Fail == ListProcesses(procs)) {
			return ret;
		}
	}
	for (size_t i = 0; i < procs.size(); ++i) {
		if (procs[i].pid == pid) {
			return procs[i];
		}
	}
	return ret;
}

std::vector<ProcDesc> ProcessHandler::GetProcessByExePath(const std::wstring exePath, std::vector<ProcDesc> *procList) {
	std::vector<ProcDesc> procs;
	std::vector<ProcDesc> ret;
	if (procList) {
		procs = *procList;
	} else {
		if (ProcOpResult::Fail == ListProcesses(procs)) {
			return ret;
		}
	}
	for (size_t i = 0; i < procs.size(); ++i) {
		if (lower_copy(procs[i].exepath) == lower_copy(exePath)) {
			ret.push_back(procs[i]);
		}
	}
	return ret;
}

unsigned long ProcessHandler::GetCurrentProcPid() const {
	return ::GetCurrentProcessId();
}

std::vector<std::wstring> ProcessHandler::GetProcPrivileges(const unsigned long pid,
	const unsigned long desiredProcRights) const {
	std::vector<std::wstring> ret;
	::HANDLE hProc = ::OpenProcess(desiredProcRights, true, pid);
	if (hProc && INVALID_HANDLE_VALUE != hProc) {
		::HANDLE hToken = 0;
		if (::OpenProcessToken(hProc, TOKEN_QUERY, &hToken)) {
			if (hToken && INVALID_HANDLE_VALUE != hToken) {
				unsigned long bufSize = 0, retval = NO_ERROR;
				if (!::GetTokenInformation(hToken, TokenPrivileges, 0, 0, &bufSize)) {
					retval = ::GetLastError();
					if (ERROR_INSUFFICIENT_BUFFER == retval) {
						NEW_ARR_NULLIFY(buf, unsigned char, bufSize * sizeof(unsigned char));
						if (buf) {
							if (::GetTokenInformation(hToken, TokenPrivileges, (void*)buf, bufSize, &bufSize)) {
								TOKEN_PRIVILEGES* priv = (TOKEN_PRIVILEGES*)buf;
								LUID luid = { 0 };
								wchar_t pbuf[64] = { 0 };
								for (size_t i = 0; i < priv->PrivilegeCount; ++i) {
									bufSize = 64;
									if (priv->Privileges[i].Attributes & SE_PRIVILEGE_ENABLED) {
										luid = priv->Privileges[i].Luid;
										if (::LookupPrivilegeName(0, &luid, pbuf, &bufSize)) {
											ret.push_back(pbuf);
										} else {
											SAFE_ARR_DELETE(buf);
											::CloseHandle(hToken);
											::CloseHandle(hProc);
											return ret;
										}
									}
									memset(pbuf, 0, 64 * sizeof(wchar_t));
								}
								SAFE_ARR_DELETE(buf);
								::CloseHandle(hToken);
								::CloseHandle(hProc);
								return ret;
							} else {
								::CloseHandle(hToken);
								::CloseHandle(hProc);
								return ret;
							}
						} else {
							::CloseHandle(hToken);
							::CloseHandle(hProc);
							return ret;
						}
					} else {
						::CloseHandle(hToken);
						::CloseHandle(hProc);
						return ret;
					}
				}
			} else {
				return ret;
			}
		} else {
			return ret;
		}
	}
	return ret;
}

ProcOpResult ProcessHandler::RunCommandPiped(const std::wstring command, std::wstring &output,
	std::wstring &erroutput, unsigned char restartNum, unsigned long sleepTime, const ProcAwait awaitTime,
	const unsigned long procAwaitTime) {
	::HANDLE hChildStd_OUT_Rd = 0;
	::HANDLE hChildStd_OUT_Wr = 0;
	::HANDLE hChildStd_ERR_Rd = 0;
	::HANDLE hChildStd_ERR_Wr = 0;
	::SECURITY_ATTRIBUTES sa;
	memset(&sa, 0, sizeof(::SECURITY_ATTRIBUTES));
	// Set the bInheritHandle flag so pipe handles are inherited.
	sa.nLength = sizeof(::SECURITY_ATTRIBUTES);
	sa.bInheritHandle = true;
	sa.lpSecurityDescriptor = 0;
	// Create a pipe for the child process's STDERR.
	if (!::CreatePipe(&hChildStd_ERR_Rd, &hChildStd_ERR_Wr, &sa, 0)) {
		return ProcOpResult::Fail;
	}
	// Ensure the read handle to the pipe for STDERR is not inherited.
	if (!::SetHandleInformation(hChildStd_ERR_Rd, HANDLE_FLAG_INHERIT, 0)) {
		return ProcOpResult::Fail;
	}
	// Create a pipe for the child process's STDOUT.
	if (!::CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &sa, 0)) {
		return ProcOpResult::Fail;
	}
	// Ensure the read handle to the pipe for STDOUT is not inherited
	if (!::SetHandleInformation(hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
		return ProcOpResult::Fail;
	}
	::PROCESS_INFORMATION piProcInfo = { 0 };
	::STARTUPINFO siStartInfo = { 0 };
	bool bSuccess = false;
	// Set up members of the PROCESS_INFORMATION structure.
	memset(&piProcInfo, 0, sizeof(::PROCESS_INFORMATION));
	// Set up members of the STARTUPINFO structure.
	// This structure specifies the STDERR and STDOUT handles for redirection.
	memset(&siStartInfo, 0, sizeof(::STARTUPINFO));
	siStartInfo.cb = sizeof(::STARTUPINFO);
	siStartInfo.hStdError = hChildStd_ERR_Wr;
	siStartInfo.hStdOutput = hChildStd_OUT_Wr;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
	// Create the child process.
	bSuccess = ::CreateProcess(
		0,							// program path
		(wchar_t*)command.c_str(),	// command line
		0,							// process security attributes
		0,							// primary thread security attributes
		true,						// handles are inherited
		CREATE_NO_WINDOW,			// creation flags (this is what hides the window)
		0,							// use parent's environment
		0,							// use parent's current directory
		&siStartInfo,				// STARTUPINFO pointer
		&piProcInfo					// receives PROCESS_INFORMATION
	);
	::CloseHandle(hChildStd_ERR_Wr);
	::CloseHandle(hChildStd_OUT_Wr);
	if (!bSuccess) {
		::CloseHandle(hChildStd_ERR_Rd);
		::CloseHandle(hChildStd_OUT_Rd);
		return ProcOpResult::Fail;
	}
	// read output
	unsigned long dwRead = 0;
	NEW_ARR_NULLIFY(cBuf, char, PH_OUTCMDBUFSIZE);
	if (!cBuf) {
		return ProcOpResult::Fail;
	}
	bool bSuccess2 = false;
	// read stdout
	for (;;) {
		memset(cBuf, 0, PH_OUTCMDBUFSIZE * sizeof(char));
		bSuccess2 = ::ReadFile(hChildStd_OUT_Rd, cBuf, PH_OUTCMDBUFSIZE * sizeof(char), &dwRead, 0);
		if (!bSuccess2 || !dwRead) {
			break;
		}
		output = output + str2wstr(cBuf);
	}
	dwRead = 0;
	// read stderr
	for (;;) {
		memset(cBuf, 0, PH_OUTCMDBUFSIZE * sizeof(char));
		bSuccess2 = ::ReadFile(hChildStd_ERR_Rd, cBuf, PH_OUTCMDBUFSIZE * sizeof(char), &dwRead, 0);
		if (!bSuccess2 || !dwRead) {
			break;
		}
		erroutput = erroutput + str2wstr(cBuf);
	}
	ProcResource ret;
	ret.pid = piProcInfo.dwProcessId;
	ret.si = siStartInfo;
	ret.pi = piProcInfo;
	if (ProcAwait::Infinite == awaitTime) {
		::WaitForSingleObject(piProcInfo.hProcess, INFINITE);
		::GetExitCodeProcess(piProcInfo.hProcess, &ret.exitCode);
		if (!FreeProcResources(ret)) {
			SAFE_ARR_DELETE(cBuf);
			::CloseHandle(hChildStd_ERR_Rd);
			::CloseHandle(hChildStd_OUT_Rd);
			return ProcOpResult::Fail;
		}
	} else if (ProcAwait::ExactTime == awaitTime) {
		::WaitForSingleObject(piProcInfo.hProcess, procAwaitTime);
		::GetExitCodeProcess(piProcInfo.hProcess, &ret.exitCode);
		if (!FreeProcResources(ret)) {
			SAFE_ARR_DELETE(cBuf);
			::CloseHandle(hChildStd_ERR_Rd);
			::CloseHandle(hChildStd_OUT_Rd);
			return ProcOpResult::Fail;
		}
	} else if (ProcAwait::NoWait == awaitTime) {
		::GetExitCodeProcess(piProcInfo.hProcess, &ret.exitCode);
		if (!FreeProcResources(ret)) {
			SAFE_ARR_DELETE(cBuf);
			::CloseHandle(hChildStd_ERR_Rd);
			::CloseHandle(hChildStd_OUT_Rd);
			return ProcOpResult::Fail;
		}
	}
	SAFE_ARR_DELETE(cBuf);
	::CloseHandle(hChildStd_ERR_Rd);
	::CloseHandle(hChildStd_OUT_Rd);
	if (!output.length()) {
		if (restartNum) {
			for (size_t i = 0; i < restartNum; ++i) {
				RunCommandPiped(command, output, erroutput, 0, 0, awaitTime, procAwaitTime);
				// std::cout << "RESTARTED!" << std::endl;
				if (output.length()) {
					break;
				}
				if (sleepTime) {
					Sleep(sleepTime);
				}
			}
		}
	}
	if (output.length()) {
		return ProcOpResult::Success;
	} else {
		return ProcOpResult::Fail;
	}
}

ProcOpResult ProcessHandler::GetProcUserSID(const unsigned long pid, ::PSID &sid,
	const unsigned long desiredProcRights) const {
	::HANDLE hProc = ::OpenProcess(desiredProcRights, true, pid);
	if (hProc && INVALID_HANDLE_VALUE != hProc) {
		::HANDLE hToken = 0;
		if (::OpenProcessToken(hProc, TOKEN_QUERY, &hToken)) {
			if (INVALID_HANDLE_VALUE != hToken) {
				unsigned long bufSize = 0, retval = NO_ERROR;
				if (!::GetTokenInformation(hToken, TokenUser, 0, 0, &bufSize)) {
					retval = ::GetLastError();
					if (ERROR_INSUFFICIENT_BUFFER == retval) {
						::TOKEN_USER* buf = (::TOKEN_USER*)malloc(bufSize);
						if (buf) {
							if (::GetTokenInformation(hToken, ::TOKEN_INFORMATION_CLASS::TokenUser,
								(void*)buf, bufSize, &bufSize)) {
								sid = (SID*)::LocalAlloc(LPTR, sizeof(SID));
								if (!sid) {
									return ProcOpResult::Fail;
								}
								memcpy(sid, buf->User.Sid, sizeof(SID));
								// sid = buf->User.Sid;
								SAFE_FREE(buf);
								::CloseHandle(hToken);
								::CloseHandle(hProc);
								return ProcOpResult::Success;
							}
							SAFE_FREE(buf);
							::CloseHandle(hToken);
							::CloseHandle(hProc);
							return ProcOpResult::Fail;
						} else {
							::CloseHandle(hToken);
							::CloseHandle(hProc);
							return ProcOpResult::Fail;
						}
					} else {
						::CloseHandle(hToken);
						::CloseHandle(hProc);
						return ProcOpResult::Fail;
					}
				} else {
					::CloseHandle(hToken);
					::CloseHandle(hProc);
					return ProcOpResult::Fail;
				}
			} else {
				::CloseHandle(hProc);
				return ProcOpResult::Fail;
			}
		} else {
			::CloseHandle(hProc);
			return ProcOpResult::Fail;
		}
	}
	return ProcOpResult::Fail;
}

ProcOpResult ProcessHandler::ListProcesses(std::vector<ProcDesc> &procList) {
	std::vector<ProcDesc> ret;
	ProcDesc elem;
	::HANDLE hProcSnap = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe32;
	hProcSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE != hProcSnap) {
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hProcSnap, &pe32)) {
			do {
				elem.pid = pe32.th32ProcessID;
				std::wstring tstr = GetProcessExePathByPid(elem.pid);
				if (tstr.length()) {
					elem.exepath = tstr;
				} else {
					elem.exepath = pe32.szExeFile;
				}
				elem.flags = pe32.dwFlags;
				elem.parentPid = pe32.th32ParentProcessID;
				elem.size = pe32.dwSize;
				elem.threadnum = pe32.cntThreads;
				elem.threadPriority = pe32.pcPriClassBase;
				elem.usage = pe32.cntUsage;
				elem.moduleId = pe32.th32ModuleID;
				elem.defHeapId = pe32.th32DefaultHeapID;
				procList.push_back(elem);
			} while (Process32Next(hProcSnap, &pe32));
			::CloseHandle(hProcSnap);
		} else {
			::CloseHandle(hProcSnap);
		}
	}
	if (procList.size()) {
		return ProcOpResult::Success;
	} else {
		return ProcOpResult::Fail;
	}
}

bool ProcessHandler::privilegeController(const unsigned long pid, const std::wstring privName, const bool enable,
	const unsigned long desiredProcRights) const {
	::HANDLE hProc = ::OpenProcess(desiredProcRights, true, pid);
	if (hProc && INVALID_HANDLE_VALUE != hProc) {
		::HANDLE hToken = 0;
		::LUID luid = { 0 };
		::TOKEN_PRIVILEGES tkp = { 0 };
		if (::OpenProcessToken(hProc, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
			if (INVALID_HANDLE_VALUE != hToken) {
				::LookupPrivilegeValue(0, privName.c_str(), &luid);
				tkp.PrivilegeCount = 1;
				tkp.Privileges[0].Luid = luid;
				if (enable) {
					tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
				} else {
					tkp.Privileges[0].Attributes = SE_PRIVILEGE_REMOVED;
				}
				if (::AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), 0, 0)) {
					::CloseHandle(hToken);
					::CloseHandle(hProc);
					return true;
				} else {
					::CloseHandle(hToken);
					::CloseHandle(hProc);
					return false;
				}
			}
			::CloseHandle(hProc);
			return false;
		} else {
			::CloseHandle(hProc);
			return false;
		}
	}
	return false;
}

ProcOpResult ProcessHandler::GetWindowPID(unsigned long &pid, const std::wstring className,
	const std::wstring windowName) const {
	wchar_t* wcClass = 0;
	wchar_t* wcWindow = 0;
	if (className.length()) {
		wcClass = const_cast<wchar_t*>(className.c_str());
	}
	if (windowName.length()) {
		wcWindow = const_cast<wchar_t*>(windowName.c_str());
	}
	::HWND hWnd = ::FindWindow(wcClass, wcWindow);
	if (!hWnd) {
		return ProcOpResult::Fail;
	}
	unsigned long pID = 0;
	if (!::GetWindowThreadProcessId(hWnd, &pID)) {
		return ProcOpResult::Fail;
	}
	pid = pID;
	return ProcOpResult::Success;
}