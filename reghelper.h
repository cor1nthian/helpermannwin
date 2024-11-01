#ifndef _REGHELPER_H
#define _REGHELPER_H

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

#ifdef _WIN64
	#define RIGHTMOD 0
	// #define RIGHTMOD KEY_WOW64_32KEY
#else
	#define RIGHTMOD KEY_WOW64_64KEY
#endif

#define CLOSEKEY_NULLIFY(x) ::RegCloseKey(x); x = 0

#define SOFTWAREVERSION std::pair<std::wstring, std::wstring>

#define REG_MAX_KEY_LENGTH	255
#define REG_MAX_VALUE_NAME	4096
#define REG_READBUFSZ		1024

#define REGHELPERVALBEGIN HKEY keyHandle = { 0 }, rootKey = { 0 }; \
				std::wstring keyPath, valueName = valName; \
				std::vector<std::wstring> strSpl; \
				if (root) { \
					rootKey = *root; \
				} else { \
					rootKey = { 0 }; \
				} \
				prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);

#define REGHELPERKEYBEGIN HKEY keyHandle = { 0 }, rootKey = { 0 }; \
				std::wstring keyPath, valueName = valName; \
				std::vector<std::wstring> strSpl; \
				if (root) { \
					rootKey = *root; \
				} else { \
					rootKey = { 0 }; \
				} \
				prepHKEYKeyPath(rootKey, path, rootKey, keyPath);

#include <Windows.h>
#include <aclapi.h>
#include <algorithm>
#include <iterator>
#include <regex>
#include "config.h"
#include "nethelper.h"
#include "syshelper.h"
#include "strhelper.h"
#include "winerrhelper.h"
#include "prochelper.h"
#include "aclhelper.h"

const void* const gc_wc_emptyVal = (void*)L'Z';
const void* const gc_wc_incorrectVal = (void*)L'Y';

const void* const gc_ul_emptyVal = (void*)0xABCD1234;
const void* const gc_ul_incorrectVal = (void*)0xABCD5678;

const void* const gc_ull_emptyVal = (void*)0xABCDEF12;
const void* const gc_ull_incorrectVal = (void*)0xABCD8765;

const wchar_t* const gc_loadKeyNamePrefix = L"Loaded Hive ";

// maximum menu items added
const unsigned long const gc_maxMenuItems = 512;

enum class RegLoadTarget : unsigned char {
	None,
	LocalMachine,
	Users
};

enum class RegValType : unsigned long {
	None = REG_NONE,
	Str = REG_SZ,
	MultiStr = REG_MULTI_SZ,
	ExpandStr = REG_EXPAND_SZ,
	DWord = REG_DWORD,
	DWordLE = REG_DWORD_LITTLE_ENDIAN,
	DWordBE = REG_DWORD_BIG_ENDIAN,
	QWord = REG_QWORD,
	QWordLE = REG_QWORD_LITTLE_ENDIAN,
	Binary = REG_BINARY,
	ResourceList = REG_RESOURCE_LIST,
	FullResourceDesc = REG_FULL_RESOURCE_DESCRIPTOR,
	ResourceRequirementsList = REG_RESOURCE_REQUIREMENTS_LIST,
	Link = REG_LINK
};

enum class RegOpResult : unsigned char {
	Success,
	Fail
};

struct RegValDesc {
	RegValDesc();
	RegValDesc(const RegValDesc &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	RegValDesc(RegValDesc &&other) noexcept;
#endif
	~RegValDesc();
	void FreeData();
	RegValDesc& operator=(const RegValDesc &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	RegValDesc& operator=(RegValDesc &&other) noexcept;
#endif
	bool operator==(const RegValDesc &other) const;
	bool operator!=(const RegValDesc &other) const;
	RegValType valType;
	void* valData;
	unsigned long valDataSz;
	std::wstring valPath;
	std::wstring valName;
	std::wstring valDataHex;
};

struct RegKeyDesc {
	RegKeyDesc();
	RegKeyDesc(const RegKeyDesc &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	RegKeyDesc(RegKeyDesc &&other) noexcept;
#endif
	~RegKeyDesc();
	void FreeValues();
	RegKeyDesc& operator=(const RegKeyDesc &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	RegKeyDesc& operator=(RegKeyDesc &&other) noexcept;
#endif
	bool operator==(const RegKeyDesc &other) const;
	bool operator!=(const RegKeyDesc &other) const;
	std::wstring keyPath;
	std::wstring keyName;
	std::vector<RegKeyDesc> keys;
	std::vector<RegValDesc> values;
};

class RegHandler {
	public:
		RegHandler();
		RegHandler(const RegHandler &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		RegHandler(RegHandler &&other) noexcept;
#endif
		~RegHandler();
		RegHandler& operator=(const RegHandler &other);
		RegHandler& operator=(RegHandler &&other) noexcept;
		bool operator==(const RegHandler &other) const;
		bool operator!=(const RegHandler &other) const;
		RegOpResult GetStrVal(const std::wstring valName, std::wstring &val, const bool cleanupString = true,
			const HKEY *root = 0) const;
		RegOpResult GetDWordVal(const std::wstring valName, unsigned long &val, const HKEY *root = 0) const;
		RegOpResult GetQWordVal(const std::wstring valName, unsigned long long &val, const HKEY *root = 0) const;
		RegOpResult GetBinaryVal(const std::wstring valName, unsigned char* &buf, unsigned long &bufSz,
			const HKEY *root = 0) const;
		RegOpResult GetMultiStrVal(const std::wstring valName, wchar_t* &buf, unsigned long &bufSz,
			const HKEY *root = 0) const;
		RegOpResult GetMultiStrVal(const std::wstring valName, std::vector<std::wstring> &val, const bool split = true,
			const wchar_t* splitter = L"\n", const HKEY *root = 0) const;
		RegOpResult GetResourceList(const std::wstring valName, unsigned long long &val, const HKEY *root = 0) const;
		RegOpResult GetVal(const std::wstring valName, unsigned char* &buf, unsigned long &bufSz, RegValType &valType,
			const HKEY *root = 0) const;
		RegOpResult GetValueType(const std::wstring valName, RegValType &valtype, const HKEY *root = 0) const;
		RegOpResult GetCPUDesc(std::vector<std::wstring> &cpuDesc, const HKEY *root = 0) const;
		RegOpResult CreateStrVal(const std::wstring valName, const std::wstring& val, const HKEY *root = 0) const;
		RegOpResult CreateDWordVal(const std::wstring valName, const unsigned long& val, const HKEY *root = 0) const;
		RegOpResult CreateQWordVal(const std::wstring valName, const unsigned long long &val,
			const HKEY *root = 0) const;
		RegOpResult CreateBinaryVal(const std::wstring valName, unsigned char* &buf,
			unsigned long bufSz, const HKEY *root = 0) const;
		RegOpResult CreateMultiStrVal(const std::wstring valName, const std::vector<std::wstring> &val,
			const HKEY *root = 0) const;
		RegOpResult AddItem2ExplorerContextMenu(const std::wstring itemName,
			const std::wstring objectPath, const bool add2DirMenu = true, const bool add2DriveMenu = true,
			const HKEY *root = 0) const;
		RegOpResult RemoveItemFromExplorerContextMenu(const std::wstring itemName, const bool removeFromDirMenu = true,
			const bool removeFromDriveMenu = true, const HKEY *root = 0) const;
		RegOpResult AddItem2DesktopContextMenu(const std::wstring itemName, const std::wstring objectPath,
			const std::wstring subMenuName = L"My Menu", const std::wstring subCommandMenuName = L"MyMenu",
			const std::wstring subCommandMenuNameAdd = L".mycmd", const std::wstring menuIconPath = L"shell32.dll,43",
			const HKEY *root = 0) const;
		RegOpResult RemoveItemFromDesktopContextMenu(const std::wstring itemName,
			const std::wstring subMenuName = L"My Menu", const std::wstring subCommandMenuName = L"MyMenu",
			const std::wstring subCommandMenuNameAdd = L".mycmd", const HKEY *root = 0) const;
		RegOpResult GetSoftwareVersion(const std::wstring softwareName, std::vector<SOFTWAREVERSION> &versions,
			const bool uniqueValues = true, const bool anySoftware = true, const HKEY *root = 0) const;
		RegOpResult GetSysTempPath(std::wstring &path, const HKEY *root = 0);
		RegOpResult DeleteVal(const std::wstring valName, const HKEY *root = 0) const;
		RegOpResult CopyVal(const std::wstring source, const std::wstring destKeyName,
			const std::wstring destValName, const HKEY *root = 0) const;
		RegOpResult MoveVal(const std::wstring source, const std::wstring destKeyName,
			const std::wstring destValName, const HKEY *root = 0) const;
		RegOpResult SeekKey(const std::wstring keyName, std::vector<RegKeyDesc> &results,
			const std::wstring startPath = L"", const bool acquireVals = false,
			const unsigned long limitResults = 700, const bool cleanupStrings = true, const HKEY *root = 0) const;
		RegOpResult EnumKey(std::wstring keyName, RegKeyDesc &keyDesc, const bool acquireVals = false,
			const unsigned long limitResults = 700, const bool cleanupStrings = true, const HKEY* root = 0) const;
		RegOpResult SeekVal2(const std::wstring valName, std::vector<RegValDesc> &results,
			const std::wstring startPath = L"", const bool acquireVals = false, const unsigned long limitResults = 700,
			const bool cleanupStrings = true, const HKEY *root = 0) const;
		RegOpResult SeekVal(const std::wstring valName, std::vector<RegValDesc> &results,
			const std::wstring startPath = L"", const bool acquireVals = false, const unsigned long limitResults = 700,
			const bool cleanupStrings = true, const HKEY *root = 0) const;
		RegOpResult SeekVal(const std::wstring valName, std::vector<RegValDesc> &results,
			const std::wstring startPath = L"", const RegValType valType = RegValType::Str,
			const bool acquireVals = false, const unsigned long limitResults = 700, const bool cleanupStrings = true,
			const HKEY *root = 0) const;
		RegOpResult AcquireValues(RegKeyDesc &keyDesc, const unsigned long limitVals = 700,
			const bool cleanupStrings = true, const HKEY *root = 0) const;
		RegOpResult AcquireValues(std::vector<RegKeyDesc> &keyList, const unsigned long limitVals = 700,
			const bool cleanupStrings = true, const HKEY *root = 0) const;
		RegOpResult AcquireValues(std::vector<RegValDesc> &valList, const unsigned long limitVals = 700,
			const bool cleanupStrings = true, const HKEY *root = 0) const;
		RegOpResult CreateValues(const RegKeyDesc &keyDesc, const HKEY *root = 0) const;
		RegOpResult CreateValues(const std::vector<RegKeyDesc> &keyList, const HKEY *root = 0) const;
		RegOpResult CreateValues(const std::vector<RegValDesc> &valList, const HKEY *root = 0) const;
		RegOpResult DeleteValues(const RegKeyDesc &keyDesc, const HKEY *root = 0) const;
		RegOpResult DeleteValues(const std::vector<RegKeyDesc> &keyList, const HKEY *root = 0) const;
		RegOpResult DeleteValues(const std::vector<RegValDesc> &valList, const HKEY *root = 0) const;
		RegOpResult FreeValues(const RegKeyDesc &keyDesc, const HKEY *root = 0) const;
		RegOpResult FreeValues(const std::vector<RegKeyDesc> &keyList, const HKEY *root = 0) const;
		RegOpResult FreeValues(const std::vector<RegValDesc> &valList, const HKEY *root = 0) const;
		RegOpResult GetKeySecurity(const std::wstring keyName, SecDesc &secDesc, const HKEY *root = 0) const;
		RegOpResult SetKeySecurity(const std::wstring keyName, SecDesc &secDesc, const HKEY *root = 0) const;
		RegOpResult CreateKey(const std::wstring keyName, const bool createMissingKeys = true,
			const HKEY *root = 0) const;
		RegOpResult DeleteKey(const std::wstring keyName, const bool deleteSubKeys = true, const HKEY *root = 0) const;
		RegOpResult CopyKey(const std::wstring source, const std::wstring destination, const bool copyVals = true,
			const HKEY *root = 0) const;
		RegOpResult MoveKey(const std::wstring source, const std::wstring destination, const bool moveValues = true,
			const HKEY *root = 0) const;
		RegOpResult EnumUserProfileHives(std::vector<std::wstring> &hiveList, const bool mountHives = true);
		RegOpResult MountHive_LoadKey(const std::wstring hivePath,
			const RegLoadTarget loadTarget = RegLoadTarget::Users);
		RegOpResult UnmountHive_UnloadKey(const std::wstring unloadKeyName,
			const RegLoadTarget unloadTarget = RegLoadTarget::Users);
		RegOpResult ConnectRegistry(HKEY &connectedNode, const std::wstring userName,
			const std::wstring password, const std::wstring remoteComputerName,
			const bool checkPing = false, const unsigned char pingAttempts = 1);
		RegOpResult DisconnectRegistry(HKEY connectedReg);
		unsigned long long ReadResourceList(const unsigned char* &resListBuf, const size_t bufSz) const;
		std::vector<std::wstring> MultiStrBuf2Vector(const wchar_t* multiStrBuf,
			const size_t bufSz) const;
	protected:
		
	private:
		/*       FUNCTIONS       */
		RegOpResult SeekKeyRec(const std::wstring keyName, std::vector<RegKeyDesc> &results,
			const std::wstring startPath = L"",	const HKEY *root = 0) const;
		RegOpResult SeekValRec(const std::wstring valName,
			std::vector<RegValDesc> &results, const std::wstring startPath = L"",
			const HKEY *root = 0) const;
		RegOpResult SeekValRecType(const std::wstring valName,
			std::vector<RegValDesc> &results, const RegValType valType = RegValType::Str,
			const std::wstring startPath = L"", const HKEY *root = 0) const;
		inline std::wstring hkey2wstr(const HKEY key) const;
		RegOpResult prepHKEYKeyPath(const HKEY &kayHandleSet,
			const std::wstring& keyPathSet, HKEY &kayHandle, std::wstring &keyPath) const;
		RegOpResult prepHKEYKeyPathValueName(HKEY &kayHandle, const HKEY &kayHandleSet,
			const std::wstring &valPath, std::wstring &keyPath,
			std::wstring &valName) const;
		RegOpResult assignHKEYKeyPath(const std::wstring keyPath, const HKEY &hkeySet,
			HKEY &tgtHKEY, std::wstring &tgtKeyPath) const;
		std::wstring rebuildSearchKeyPath(const HKEY &root, const std::wstring &origPath,
			const std::wstring keyName) const;
		unsigned long getRightMod() const;
		inline std::wstring pickLoadKey(const std::wstring hivePath) const;
		inline HKEY pickLoadTarget(const RegLoadTarget loadTarget = RegLoadTarget::None) const;
		HKEY pickUnloadKey(const std::wstring loadTarget) const;
		RegLoadTarget pickUnloadKey(const HKEY tgtHKEY) const;
		std::wstring pickMenuModNum(const std::wstring subMenuKey, const HKEY *root = 0) const;
		/*       VARIABLES       */
		size_t m_mountedHiveCount;
		std::map<std::wstring, HKEY> m_mountedHives;
		std::map<std::wstring, HKEY> m_connectedRegs;
};

#endif // _REGHELPER_H