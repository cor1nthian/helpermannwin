#ifndef _SYSHELPER_H
#define _SYSHELPER_H

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
#include <accctrl.h>
#include <shlwapi.h>
#include <Sddl.h>
#include <lm.h>
#include <random>
#include <time.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <thread>
#include "config.h"
#include "strhelper.h"
#include "winerrhelper.h"

// Check memory address access
const unsigned long const dwForbiddenArea = PAGE_GUARD | PAGE_NOACCESS;
const unsigned long const dwReadRights = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
const unsigned long const dwWriteRights = PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

template<unsigned long dwAccessRights>
bool CheckAccess(void* pAddress, size_t nSize) {
	if (!pAddress || !nSize) {
		return false;
	}
	MEMORY_BASIC_INFORMATION sMBI;
	bool bRet = false;
	unsigned long long pCurrentAddress = (unsigned long long)pAddress;
	unsigned long long pEndAdress = pCurrentAddress + (nSize - 1);
	do {
		memset(&sMBI, 0, sizeof(sMBI));
		::VirtualQuery((const void*)pCurrentAddress, &sMBI, sizeof(sMBI));
		bRet = (sMBI.State & MEM_COMMIT)			// memory allocated and
			&& !(sMBI.Protect & dwForbiddenArea)	// access to page allowed and
			&& (sMBI.Protect & dwAccessRights);		// the required rights
		pCurrentAddress = ((unsigned long long)sMBI.BaseAddress + sMBI.RegionSize);
	} while (bRet && pCurrentAddress <= pEndAdress);
	return bRet;
}

#define IsBadWritePtrSz(p, n) (!CheckAccess<dwWriteRights>(p, n))
#define IsBadReadPtrSz(p, n) (!CheckAccess<dwReadRights>(p, n))
#define IsBadStringPtrWSz(p, n) (!CheckAccess<dwReadRights>(p, 2 * n))

typedef bool(__stdcall* LPFN_ISWOW64PROCESS) (::HANDLE procHandle, int &result);

// constexpr to get TZ offset
static constexpr time_t const NULL_TIME = -1;

struct AccountDesc;
struct GroupDesc;
struct WKSid;

enum class SysOpResult : unsigned char {
	Success,
	Fail
};

enum class ResourceType : unsigned char {
	// Accelerator table
	Accelerator = (unsigned char)RT_ACCELERATOR,
	// Animated cursor
	AniCursor = (unsigned char)RT_ANICURSOR,
	// Animated icon
	AniIcon = (unsigned char)RT_ANIICON,
	// Bitmap resource
	Bitmap = (unsigned char)RT_BITMAP,
	// Hardware-dependent cursor resource
	Cursor = (unsigned char)RT_CURSOR,
	// Dialog box
	Dialog = (unsigned char)RT_DIALOG,
	// Allows a resource editing tool to associate a string with an .rc file. Typically, the string is the name of the
	// header file that provides symbolic names. The resource compiler parses the string but otherwise ignores the value.
	// For example,
	// 1 DLGINCLUDE "MyFile.h"
	DialogInculde = (unsigned char)RT_DLGINCLUDE,
	// Font resource
	Font = (unsigned char)RT_FONT,
	// Font directory resource
	FontDirectory = (unsigned char)RT_FONTDIR,
	// Hardware-independent cursor resource
	GroupCursor = (unsigned char)RT_GROUP_CURSOR,
	// Hardware-independent icon resource
	GroupIcon = (unsigned char)RT_GROUP_ICON,
	// HTML resource
	HTML = (unsigned char)RT_HTML,
	// Hardware-dependent icon resource
	Icon = (unsigned char)RT_ICON,
	// Side-by-Side Assembly Manifest
	Manifest = (unsigned char)RT_MANIFEST,
	// Menu resource
	Menu = (unsigned char)RT_MENU,
	// Message-table entry
	MessageTable = (unsigned char)RT_MESSAGETABLE,
	// Plug and Play resource
	PlugNPlay = (unsigned char)RT_PLUGPLAY,
	// Application-defined resource (raw data)
	RCData = (unsigned char)RT_RCDATA,
	// String-table entry
	StringTable = (unsigned char)RT_STRING,
	// Version
	Version = (unsigned char)RT_VERSION,
	// VXD
	VXD = (unsigned char)RT_VXD,
	// Any resource type
	Any = 255
};

enum class SysArch : unsigned char {
	X32,
	X64
};

enum class SidType : unsigned char {
	User = SID_NAME_USE::SidTypeUser,
	Group = SID_NAME_USE::SidTypeGroup,
	Domain = SID_NAME_USE::SidTypeDomain,
	Alias = SID_NAME_USE::SidTypeAlias,
	WellKnownGroup = SID_NAME_USE::SidTypeWellKnownGroup,
	DeletedAcc = SID_NAME_USE::SidTypeDeletedAccount,
	Invalid = SID_NAME_USE::SidTypeInvalid,
	Unknown = SID_NAME_USE::SidTypeUnknown,
	Computer = SID_NAME_USE::SidTypeComputer,
	Label = SID_NAME_USE::SidTypeLabel,
	LogonSeesion = SID_NAME_USE::SidTypeLogonSession,
	SidTypeUnknown = 255
};

bool IsBadReadPtr(void* p);
bool IsBadWritePtr(void* p);
long long rnd(const long long from, const long long to);
bool IsSIDWellKnown(const std::wstring strsid);
bool IsSIDWellKnown(const ::PSID sid);
std::vector<WKSid> GetWellKnownStrSIDs(::PSID domainSID = 0);

// const std::vector<WKSid> const gc_WellKnownStrSIDs = GetWellKnownStrSIDs();

struct WKSid {
	WKSid();
	WKSid(std::wstring strsid, std::wstring sidname, std::wstring sidnameorig);
	WKSid(const WKSid &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	WKSid(WKSid &&other) noexcept;
#endif
	~WKSid();
	WKSid& operator=(const WKSid &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	WKSid& operator=(WKSid &&other) noexcept;
#endif
	bool operator==(const WKSid &other) const;
	bool operator!=(const WKSid &other) const;
	std::wstring StrSID;
	std::wstring SIDName;
	std::wstring SIDNameOrig;
};

// User account description struct
struct AccountDesc {
	AccountDesc();
	AccountDesc(const AccountDesc &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	AccountDesc(AccountDesc &&other) noexcept;
#endif
	~AccountDesc();
	AccountDesc& operator=(const AccountDesc &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	AccountDesc& operator=(AccountDesc &&other) noexcept;
#endif
	bool operator==(const AccountDesc &other) const;
	bool operator!=(const AccountDesc &other) const;
	bool ncNormalAcc;
	bool ncTempDupAcc;
	bool ncWorkstationTrustAcc;
	bool ncServerTrustAcc;
	bool ncInterdomainTrustAcc;
	bool accountLocked;
	bool accountDisabled;
	bool accountNotDelegated;
	bool accountDESKeysOnly;
	bool accountNoKerberosPreauth;
	bool accountTrustedToAuthForDelegation;
	bool accountTrustedForDelegation;
	bool authPrintOp;
	bool authCommOp;
	bool authSrvOp;
	bool authAccOp;
	bool logonScriptExecuted;
	bool passwordNotNeeded;
	bool passwordCantChange;
	bool passwordNoExpire;
	bool passwordReverseEncryption;
	bool passwordSmartCardNeeded;
	bool passwordExpired;
	bool passwordEncrypted;
	unsigned long passwordAge;
	unsigned long passwordBadCount;
	unsigned long lastLogOn;
	unsigned long lastLogOff;
	unsigned long privileges;
	unsigned long expireDate;
	unsigned long maxStorage;
	unsigned long logonCount;
	unsigned long countryCode;
	unsigned long codePage;
	unsigned long primaryGroupId;
	std::wstring sid;
	std::wstring comment;
	std::wstring accountName;
	std::wstring accountFullname;
	std::wstring logonScriptPath;
	std::wstring homeDirPath;
	std::wstring homeDirDrive;
	std::wstring profileDirPath;
	std::wstring params;
	std::wstring allowedWorkstations;
	std::wstring logonServer;
	std::wstring passwordSetStr;
	std::wstring lastLogOnStr;
	std::wstring lastLogOffStr;
	std::wstring privilegesStr;
	std::wstring expireDateStr;
	std::wstring maxStorageStr;
	std::vector<GroupDesc> groups;
};

// Account group definiton struct
struct GroupDesc {
	GroupDesc();
	GroupDesc(const bool isADGroyp, const std::wstring groupName, const std::wstring groupStrSid,
		const std::wstring comment, const std::vector<AccountDesc> accounts);
	GroupDesc(const GroupDesc &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	GroupDesc(GroupDesc &&other) noexcept;
#endif
	~GroupDesc();
	GroupDesc& operator=(const GroupDesc &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	GroupDesc& operator=(GroupDesc &&other) noexcept;
#endif
	bool operator==(const GroupDesc &other) const;
	bool operator!=(const GroupDesc &other) const;
	bool IsADGroup;
	std::wstring GroupName;
	std::wstring GroupStrSid;
	std::wstring Comment;
	std::vector<AccountDesc> Accounts;
};

class SysHandler {
	public:
		SysHandler();
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		SysHandler(const SysHandler &other) = delete;
#else
		SysHandler(const SysHandler &other) {}
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		SysHandler(SysHandler &&other) noexcept = delete;
#endif
		~SysHandler();
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		SysHandler& operator=(const SysHandler &other) = delete;
#else
		SysHandler& operator=(const SysHandler &other) {}
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		SysHandler& operator=(SysHandler &&other) = delete;
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		bool operator==(const SysHandler &other) const = delete;
#else
		bool operator==(const SysHandler &other) {}
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		bool operator!=(const SysHandler &other) const = delete;
#else
		bool operator!=(const SysHandler &other) {}
#endif
		/* Gets machine name
			Param:
			None
			Returns machine name */
		std::wstring GetMachineName() const;
		/* Gets machine arch - x32 or x64
			Param:
			None
			Returns system architecture (SysArch enum value) */
		SysArch GetMachineArch() const;
		SysOpResult GetResourceIDByName(unsigned long &resourceID, const std::wstring resourceName,
			const ::HMODULE hModule = 0, const ResourceType resourceType = ResourceType::Any) const;
		SysOpResult GetResourceNameByID(std::wstring &resourceName, const unsigned long resourceID) const;
		/* Extracts resource from resource section
			Param:
			[in] path to extract resource to
			[in] resource id in resource file
			Returns true on success, false on failure */
		bool ExtractResource(const std::wstring extractPath, const unsigned long resId) const;
		/* Checks if current x86 process runs under Wow64 environment /
			Checks if current provess is x86 under x64 system
			Param:
			None
			Returns true if current x86 process runs under Wow64 environment /
			Returns true if current provess is x86 under x64 system, false otherwise */
		bool IsWow64Proc() const;
		/* Checks if a given x86 process runs under Wow64 environment /
			Checks if a given provess is x86 runming under x64 system
			Param:
			[in] [mandatory] process id
			[in] [optional] [default - PROCESS_ALL_ACCESS] roghts to open requested process
			Returns true if a given x86 process runs under Wow64 environment /
			Returns true if a given x86 process runs under x64 system, false otherwise */
		bool IsWow64Proc(const unsigned long pid, const unsigned long desiredProcRights = PROCESS_ALL_ACCESS) const;
		/* Converts string SID to SID type
			Param:
			[in] [mandatory] account string SID
			Returns ::PSID related with source string SID
			Returned ::PSID must be freed with LocalFree */
		::PSID SIDFromStrSid(const std::wstring sidstr) const;
		/* Converts ::PSID to string SID
			Param:
			[in] account ::PSID
			Returns string SID */
		std::wstring StrSIDFromSID(const ::PSID sid) const;
		/* Gets account name based on SID
			Param:
			[in] target ::PSID
			[in] [default - "."] target machine name. Leave it unchanged or empty to get local machine accounts
			Returns account name */
		std::wstring GetAccountNameFromSID(const ::PSID sid, const std::wstring machineName = L".") const;
		/* Gets account name based on str SID
			Param:
			[in] target str SID
			[in] [default - "."] target machine name. Leave it unchanged or empty to get local machine accounts
			Returns account name */
		std::wstring GetAccountNameFromStrSID(const std::wstring strSid,
			const std::wstring machineName = L".") const;
		/* Gets string SID based on account name
			Param:
			[in] account name
			[in] [default - "."]	target machine name. Leave it unchanged or empty to get local machine accounts
			[in] [default - false]	set to true if specified account is domain acc
			Returns result code of the operation */
		std::wstring GetStrSIDFromAccountName(const std::wstring accName,
			const std::wstring machineName = L".", const bool isDomainAcc = false) const;
		/* Gets ::PSID from a given account name
			Param:
			[in] [mandatory] account name / login
			[in] [optional] [default - "."]		target machine name. Leave it unchanged or empty to get local machine accounts
			[in] [optional] [default - false]	set to true if requested account belongs to domain
			Returns account ::PSID
			Returned ::PSID must be freed with LocalFree */
		::PSID GetSIDFromAccountName(const std::wstring accName,
			const std::wstring machineName = L".", const bool isDomainAcc = false) const;
		/* Converts current system time to Unix time
			Param:
			None
			Returns Unix timestamp (number of seconds since 01.01.1970) with current time */
		unsigned long long CurrentTime2Unixtime() const;
		/* Converts Unix timestamp to human - readable format
			Param:
			[in] Unix timestamp
			Returns datetime string based on a timestamp */
		std::wstring UnixTime2StrTime(const time_t &timestamp) const;
		/* Gets TZ offset even without date and time API calls
			Param:
			[in]	[default - null time/use local time] timepoint to get offset from
			Returns TZ offset at Unix time format (number of seconds since 01.01.1970) */
		long GetTZOffset(time_t when = NULL_TIME) const;
		/* Gets number of CPU cores
			Param:
			None
			Returns number of CPU cores */
		unsigned char GetCPUCoreNum() const;
		/* Gets max number of threads available without oversubscription (cpucorenum - 1)
			Param:
			None
			Returns max number of threads available without oversubscription (cpucorenum - 1) */
		unsigned char GetThreadNum() const;
		SysOpResult IsAccountMemberOfGroup(const ::PSID groupSID, const ::PSID testSID, bool &isMember,
			const std::wstring machineName = L".") const;
		SysOpResult IsAccountMemberOfGroup(const std::wstring groupName, const std::wstring testAccName,
			bool &isMember, const std::wstring machineName = L".") const;
		SysOpResult GetSIDType(const ::PSID sid, SidType &sidType, const std::wstring machineName = L".", 
			const std::wstring domainName = L".") const;
		SysOpResult GetPhysRAM(unsigned long long &ramAmount) const;
		/* Gets RAM info
			Param:
			[out]	free RAM
			[out]	total RAM
			Returns result code of the operation (enum value) */
		SysOpResult GetRam(unsigned long long &freeRam, unsigned long long &totalRam, unsigned char &precentInUse) const;
		/* Gets paging file info
			Param:
			[out]	free paging file
			[out]	total paging file
			Returns result code of the operation (enum value) */
		SysOpResult GetPageFile(unsigned long long &freePageFile, unsigned long long &totalPageFile) const;
		/* Gets virtual memory info
			Param:
			[out]	free virtual memory
			[out]	total virtual memory
			Returns result code of the operation (enum value) */
		SysOpResult GetVirtualMem(unsigned long long &freeVirtMem, unsigned long long &totalVirtMem) const;
		SysOpResult LocalGroupListFromUsername(std::vector<GroupDesc> &outGroupList, const std::wstring userName,
			const std::wstring machineName = L".") const;
		SysOpResult LocalGroupListFromStrSID(std::vector<GroupDesc> &outGroupList, const std::wstring strSID,
			const std::wstring machineName = L".") const;
		SysOpResult UserLogon(::HANDLE &token, std::wstring userName, std::wstring password,
			std::wstring domain = L".") const;
		SysOpResult ImpersonateUser(::HANDLE &token) const;
		SysOpResult GetSysTempFolderPath(std::wstring &tempFldPath, const HKEY *root = 0) const;
		SysOpResult RunVBSScript(std::wstring &output, std::wstring &erroutput, const std::wstring scriptPath,
			const std::wstring scriptArgs) const;
		SysOpResult RunPSScript(std::wstring& output, std::wstring &erroutput, const std::wstring scriptPath,
			const std::wstring scriptArgs) const;
		SysOpResult ExpandToken(std::wstring &outStr, const std::wstring token) const;
		SysOpResult IsSysTempFolderAvailable(bool &available, const HKEY *root = 0) const;
		SysOpResult IsPowershellAvailable(bool &available) const;
		SysOpResult IsCScriptAvailable(bool &available) const;
		SysOpResult GetLocalSystemSID(::PSID &localSysSID) const;
		SysOpResult GetAdministratorsSID(::PSID &adminSID) const;
		SysOpResult GetEveryoneSID(::PSID& everyoneSID) const;
		SysOpResult CreateTokenStatistics(::PTOKEN_STATISTICS& tokenStats, unsigned long* lpGroupCount,
			unsigned long* lpPrivilegeCount, ::SECURITY_IMPERSONATION_LEVEL* lpImpersonationLevel,
			::LUID* lpTokenId, ::LUID* lpAuthenticationId, ::LARGE_INTEGER* lpExpirationTime,
			::TOKEN_TYPE* lpTokenType, unsigned long* lpDynamicCharged, unsigned long* lpDynamicAvailable,
			::LUID* lpModifiedId) const;
		SysOpResult CreateTokenGroups(::PTOKEN_GROUPS &tokenGroups, ::PSID_AND_ATTRIBUTES lpPSIDGroupsAttr) const;
		SysOpResult CreateTokenPriv(::PTOKEN_PRIVILEGES &tokenPriv, unsigned long& dwPrivGranted, LPCTSTR* lpszPriv,
			const bool enableAll) const;
		SysOpResult GetAuthedUsersSID(::PSID &authedUsersSID) const;
		SysOpResult CreatePureSystemToken(::HANDLE &hToken) const;
		SysOpResult CreatePureUserToken(::HANDLE &hToken) const;
		SysOpResult ImpersonateIfConformToken(long &operatonStatus, ::HANDLE &hToken);
		/* Enumerates local groups
			Param:
			[out] [mandatory] group desctiption vector to receive data
			[in] [optional] [default - "."]	target machine name. Leave it unchanged or empty to get local accounts
			[in] [optional] [default - true] add a short list of group memners
			[in] [optional] [default - 0] pointer to account list to use. Leaving value as 0 causes account enumeration (EnumAccounts call)
			Returns result code of the operation (enum value) */
		SysOpResult EnumLocalGroups(std::vector<GroupDesc> &groupList,
			const std::wstring machineName = L".", const bool enumAccs = true,
			const std::vector<AccountDesc> *accList = 0) const;
		/* Enumerates local accounts
			Param:
			[out] [mandatory] account desctiption vector to receive data
			[in] [optional] [default - "."]	target machine name. Leave it unchanged or empty to get local accounts
			[in] [optional] [default - true] add a short list of groups to which account belongs
			Returns result code of the operation (enum value) */
		SysOpResult EnumAccounts(std::vector<AccountDesc> &accountList,
			const std::wstring machineName = L".", const bool enumGroups = true) const;
	protected:
	private:
};

#endif // _SYSHELPER_H