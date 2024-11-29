#ifndef _SERVICE_HELPER_H
#define _SERVICE_HELPER_H

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
// #define NOSERVICE
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
#include <map>
#include <vector>
#include "config.h"
#include "prochelper.h"
#include "strhelper.h"
#include "syshelper.h"
#include "winerrhelper.h"
#include "winverhelper.h"

enum class ScmOpResult : unsigned char {
	OpenManagerFail = 0,
	CloseManagerFail,
	EnumSvcsFail,
	StartSvcFail,
	StartSvcFailAlreadyStarted,
	StopSvcFail,
	StopSvcFailAlreadyStopped,
	InstallFail,
	RemoveFail,
	ChangeServiceFail,
	Success
};

enum class DepCompareType : unsigned char {
	equal = 0,
	notEqual,
	greater,
	less,
	greaterOrEqual,
	lessOrEqual
};

enum class StrCompareType : unsigned char {
	equal = 0,
	equalIgnoreCase,
	partialMatch,
	partialMatchIgnoreCase
};

enum class ServiceState : unsigned long {
	Active = SERVICE_ACTIVE,
	Stopped = SERVICE_INACTIVE,
	Any = SERVICE_STATE_ALL
};

enum class SvcStartUpType : unsigned long {
	AutoStart = SERVICE_AUTO_START,
	BootStart = SERVICE_BOOT_START,
	DemandStart = SERVICE_DEMAND_START,
	Disabled = SERVICE_DISABLED,
	SystemStart = SERVICE_SYSTEM_START
};

enum class SvcType : unsigned long {
	KernelDriver = SERVICE_KERNEL_DRIVER,
	FSDriver = SERVICE_FILE_SYSTEM_DRIVER,
	SvcOwnProc = SERVICE_WIN32_OWN_PROCESS,
	SvcShareProc = SERVICE_WIN32_SHARE_PROCESS,
	Druver = SERVICE_DRIVER | SERVICE_FILE_SYSTEM_DRIVER | SERVICE_KERNEL_DRIVER,
	Service = SERVICE_WIN32
};

enum class ServiceType : unsigned long {
	Driver = SERVICE_DRIVER | SERVICE_FILE_SYSTEM_DRIVER | SERVICE_KERNEL_DRIVER,
	Service = SERVICE_WIN32,
	Any = SERVICE_WIN32 | SERVICE_DRIVER | SERVICE_FILE_SYSTEM_DRIVER | SERVICE_KERNEL_DRIVER
};

enum class SvcErrControl : unsigned char {
	Ignore = SERVICE_ERROR_IGNORE,
	Normal = SERVICE_ERROR_NORMAL,
	Severe = SERVICE_ERROR_SEVERE,
	Critical = SERVICE_ERROR_CRITICAL
};

struct SvcRunState {
	SvcRunState();
	SvcRunState(const SvcRunState &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	SvcRunState(SvcRunState &&other) noexcept;
#endif
	~SvcRunState();
	SvcRunState& operator=(const SvcRunState &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	SvcRunState& operator=(SvcRunState &&other) noexcept;
#endif
	bool operator==(const SvcRunState &other) const;
	bool operator!=(const SvcRunState &other) const;
	bool svcRunning;
	bool svcProcRunning;
};

struct SvcTriggerData {
	SvcTriggerData();
	SvcTriggerData(const unsigned short triggerdatasize,
		const unsigned long triggerdatatype,
		std::vector<unsigned char> triggerdata);
	SvcTriggerData(const SvcTriggerData &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	SvcTriggerData(SvcTriggerData &&other) noexcept;
#endif
	~SvcTriggerData();
	SvcTriggerData& operator=(const SvcTriggerData &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	SvcTriggerData& operator=(SvcTriggerData &&other) noexcept;
#endif
	bool operator ==(const SvcTriggerData& other) const;
	bool operator !=(const SvcTriggerData& other) const;
	unsigned short triggerDataSize;
	unsigned long triggerDataType;
	std::vector<unsigned char> triggerData;
};

struct SvcTrigger {
	SvcTrigger();
	SvcTrigger(const unsigned long triggertype, const unsigned long triggeraction,
		const std::wstring triggerguid, std::vector <SvcTriggerData> trigdata);
	SvcTrigger(const SvcTrigger &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	SvcTrigger(SvcTrigger &&other) noexcept;
#endif
	~SvcTrigger();
	SvcTrigger& operator=(const SvcTrigger &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	SvcTrigger& operator=(SvcTrigger &&other) noexcept;
#endif
	bool operator==(const SvcTrigger &other) const;
	bool operator!=(const SvcTrigger &other) const;
	unsigned long triggerType;
	unsigned long triggerAction;
	std::wstring triggerGuid;
	std::vector<SvcTriggerData> triggerData;
};

struct SvcRecord {
	SvcRecord();
	SvcRecord(const std::wstring svcname,
		const std::wstring svcdisplayname,
		const std::wstring svcdesc,
		const std::wstring binarypath,
		const std::wstring startsfrom,
		const std::wstring loadordergroup,
		const std::wstring failresetmsg,
		const std::wstring failcommand,
		const std::wstring reqprivileges,
		const unsigned long servicetype,
		const unsigned long starttype,
		const unsigned long errorcontrol,
		const unsigned long tagid,
		const unsigned long failresetperiod,
		const unsigned long failactions,
		const unsigned long failactiondelay,
		const unsigned long preshutdowntimeout,
		const unsigned long svcsidtype,
		const unsigned long launchprotected,
		const unsigned short preferrednode,
		const bool preferrednodedeleteflag,
		const bool failureactionsonnoncrashfailures,
		const bool delayedautostart);
	SvcRecord(const std::wstring svcname,
		const std::wstring svcdisplayname,
		const std::wstring svcdesc,
		const std::wstring binarypath,
		const std::wstring startsfrom,
		const std::wstring loadordergroup,
		const std::wstring failresetmsg,
		const std::wstring failcommand,
		const std::wstring reqprivileges,
		const unsigned long servicetype,
		const unsigned long starttype,
		const unsigned long errorcontrol,
		const unsigned long tagid,
		const unsigned long failresetperiod,
		const unsigned long failactions,
		const unsigned long failactiondelay,
		const unsigned long preshutdowntimeout,
		const unsigned long svcsidtype,
		const unsigned long launchprotected,
		const unsigned short preferrednode,
		const bool preferrednodedeleteflag,
		const bool delayedautostart,
		const bool failureactionsonnoncrashfailures,
		const std::vector<std::wstring> deps,
		const std::vector<SvcTrigger> trgs,
		const SC_ACTION_TYPE failactiontype,
		const SERVICE_STATUS_PROCESS svcprocdetial);
	SvcRecord(const SvcRecord &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	SvcRecord(SvcRecord &&other) noexcept;
#endif
	~SvcRecord();
	SvcRecord& operator=(const SvcRecord &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	SvcRecord& operator=(SvcRecord &&other) noexcept;
#endif
	bool operator==(const SvcRecord &other) const;
	bool operator!=(const SvcRecord &other) const;
	std::vector<std::wstring> dependencies;
	std::vector<SvcTrigger> triggers;
	std::wstring svcName;
	std::wstring svcDisplayName;
	std::wstring svcDesc;
	std::wstring binaryPath;
	std::wstring startsFrom;
	std::wstring loadOrderGroup;
	std::wstring failResetMsg;
	std::wstring failCommand;
	std::wstring privileges;
	unsigned long serviceType;
	unsigned long startType;
	unsigned long errorControl;
	unsigned long tagId;
	unsigned long failResetPeriod;
	unsigned long failActions;
	unsigned long failActionDelay;
	unsigned long preshutdownTimeout;
	unsigned long svcSidType;
	unsigned long launchProtected;
	unsigned short preferredNode;;
	bool preferredNodeDeleteFlag;
	bool delayedAutoStart;
	bool failureActionsOnNonCrashFailures;
	SC_ACTION_TYPE failActionType;
	SERVICE_STATUS_PROCESS svcProcDetail;
};

class SvcHandler {
	public:
		SvcHandler();
		SvcHandler(const ::SC_HANDLE schandle, const unsigned long scmrights, const bool scmopen);
		SvcHandler(const SvcHandler &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		SvcHandler(SvcHandler &&other) noexcept;
#endif
		~SvcHandler();
		SvcHandler& operator=(const SvcHandler &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		SvcHandler& operator=(SvcHandler &&other) noexcept;
#endif
		bool operator==(const SvcHandler &other) const;
		bool operator!=(const SvcHandler &other) const;
		ScmOpResult SCM_OpenManager(unsigned long desiredRghts = 0);
		ScmOpResult SCM_CloseManager();
		ScmOpResult InstallSvc(const std::wstring svcName, const std::wstring displayName,
			const std::wstring binPath, const unsigned long desiredRights = 0, const unsigned long tagId = 0,
			const SvcType svcType = SvcType::SvcOwnProc,
			const SvcStartUpType startupType = SvcStartUpType::AutoStart,
			const SvcErrControl errControlType = SvcErrControl::Ignore,
			std::wstring loadOrderGroup = L"", std::wstring startupAccLogin = L"",
			std::wstring startupAccPwd = L"", const std::vector<std::wstring> *depenendencies = 0);
		ScmOpResult RemoveSvc(const std::wstring svcName,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const unsigned long desiredRights = 0, const std::vector<SvcRecord> *svcRecords = 0);
		ScmOpResult StartSvc(const std::wstring displayName, const unsigned long desiredRights = 0,
			const std::vector<std::wstring>* svcArgs = 0, const std::vector<SvcRecord> *svcRecords = 0);
		ScmOpResult StopSvc(const std::wstring displayName, const bool stopDependentSvcs = false,
			const bool stopDrivers = false, const unsigned long desiredRights = 0,
			const unsigned long timeout = 30000, const std::vector<SvcRecord> *svcRecords = 0);
		bool IsSvcDriver(const SvcRecord &svcrec);
		bool IsSvcDriver(const std::wstring serviceName, const unsigned long desiredRights = 0,
			const std::vector<SvcRecord> *svcRecords = 0);
		ScmOpResult EnumSvcsStatus(std::vector<SvcRecord> &svcrecs,
			const unsigned long desiredRights = 0, ServiceType svcType = ServiceType::Any,
			ServiceState = ServiceState::Any);
		ScmOpResult SetSvcStartupType(const std::wstring svcName,
			SvcStartUpType startupType = SvcStartUpType::DemandStart,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const unsigned long desiredRights = 0, const std::vector<SvcRecord> *svcRecords = 0);
		ScmOpResult SetSvcStartName(const std::wstring svcName, const std::wstring accLogin,
			const std::wstring accPwd, const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const unsigned long desiredRights = 0, const std::vector<SvcRecord> *svcRecords = 0);
		ScmOpResult SetSvcDisplayName(const std::wstring svcName, const std::wstring newDisplayName,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const unsigned long desiredRights = 0, const std::vector<SvcRecord> *svcRecords = 0);
		ScmOpResult SetSvcDependencies(const std::wstring svcName, const std::vector<std::wstring> dependencies,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const unsigned long desiredRights = 0, const std::vector<SvcRecord> *svcRecords = 0);
		ScmOpResult SetSvcDelayedAutostart(const std::wstring svcName, const bool enableDelayedAutostart,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const unsigned long desiredRights = 0, const std::vector<SvcRecord> *svcRecords = 0);
		ScmOpResult SetSvcDescription(const std::wstring svcName, const std::wstring svcDescription,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const unsigned long desiredRights = 0, const std::vector<SvcRecord> *svcRecords = 0);
		ScmOpResult SetSvcType(const std::wstring svcName, const SvcType svcType = SvcType::SvcOwnProc,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const unsigned long desiredRights = 0, const std::vector<SvcRecord> *svcRecords = 0);
		ScmOpResult SetSvcBinary(const std::wstring svcName, const std::wstring svcBinPath,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const unsigned long desiredRights = 0, const std::vector<SvcRecord> *svcRecords = 0);
		SvcRunState IsServiceRunning(const std::wstring svcDisplayName,
			const unsigned long desiredRights = 0,
			const std::vector<SvcRecord> *svcRecords = 0);
		std::vector<SvcRecord> GetSvcsWithNTriggers(size_t trigCount,
			const DepCompareType depCompareType = DepCompareType::equal,
			const bool uniqueResults = true, const unsigned long desiredRights = 0,
			const std::vector<SvcRecord> *svcRecords = 0);
		std::vector<SvcRecord> GetSvcsWithNDependencies(size_t depCount,
			const DepCompareType depCompareType = DepCompareType::equal,
			const bool uniqueResults = true, const unsigned long desiredRights = 0,
			const std::vector<SvcRecord> *svcRecords = 0);
		std::vector<SvcRecord> GetSvcByDisplayName(const std::wstring displayName,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			bool uniqueResults = true, const unsigned long desiredRights = 0,
			const std::vector<SvcRecord> *svcRecords = 0);
		std::vector<SvcRecord> GetSvcBySvcName(const std::wstring svcName,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const bool uniqueResults = true, const unsigned long desiredRights = 0,
			const std::vector<SvcRecord> *svcRecords = 0);
		std::vector<std::wstring> GetSvcExecutableByDisplayName(const std::wstring displayName,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const bool uniqueResults = true, const bool removeParameters = true,
			const unsigned long desiredRights = 0, const std::vector<SvcRecord> *svcRecords = 0);
		std::vector<std::wstring> GetSvcExecutableBySvcName(const std::wstring svcName,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const bool uniqueResults = true, const bool removeParameters = true,
			const unsigned long desiredRights = 0, const std::vector<SvcRecord> *svcRecords = 0);
		std::vector<std::wstring> GetDisplayNameBySvcName(const std::wstring svcName,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const bool uniqueResults = true, const unsigned long desiredRights = 0,
			const std::vector<SvcRecord> *svcRecords = 0);
		std::vector<std::wstring> GetSvcNameByDisplayName(const std::wstring displayName,
			const StrCompareType compareType = StrCompareType::equalIgnoreCase,
			const bool uniqueResults = true, const unsigned long desiredRights = 0,
			const std::vector<SvcRecord> *svcRecords = 0);
	protected:

	private:
		ScmOpResult QuerySvcDesc(::SC_HANDLE& svcHandle, SvcRecord& rec);
		ScmOpResult QuerySvcDelayedAutostart(::SC_HANDLE& svcHandle, SvcRecord& rec);
		ScmOpResult QuerySvcFailureActions(::SC_HANDLE& svcHandle, SvcRecord& rec);
		ScmOpResult QuerySvcFailureActionsFlag(::SC_HANDLE& svcHandle, SvcRecord& rec);
		ScmOpResult QuerySvcPreferredNode(::SC_HANDLE& svcHandle, SvcRecord& rec);
		ScmOpResult QuerySvcPreshutdownInfo(::SC_HANDLE& svcHandle,  SvcRecord& rec);
		ScmOpResult QuerySvcPrivileges(::SC_HANDLE& svcHandle,  SvcRecord& rec);
		ScmOpResult QuerySvcSIDInfo(::SC_HANDLE& svcHandle,  SvcRecord& rec);
		ScmOpResult QuerySvcTriggerInfo(::SC_HANDLE& svcHandle,  SvcRecord& rec);
		ScmOpResult QuerySvcLaunchProtected(::SC_HANDLE& svcHandle,  SvcRecord& rec);
		ScmOpResult StopDependentSvc(const SvcRecord& svcRec, const bool stopDrivers = false,
			const unsigned long desiredRights = 0, const unsigned long timeout = 30000,
			const std::vector<SvcRecord> *svcRecords = 0);
		inline ScmOpResult ObtainSCMHanele(const unsigned long desiredRights = 0);
		bool m_scmOpen;
		unsigned long m_scmRights;
		::SC_HANDLE m_scmHandle;
};

#endif // _SERVICE_HELPER_H