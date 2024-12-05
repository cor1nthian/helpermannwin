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

#pragma warning(disable : 4996)

#include <iostream>
#include <conio.h>
#include <locale>
#include <clocale>
#include <Windows.h>
#include "reghelper.h"
#include "strhelper.h"
#include "svchelper.h"
#include "winverhelper.h"
#include "prochelper.h"
#include "fshelper.h"
#include "syshelper.h"
#include "aclhelper.h"
#include "nethelper.h"
#include "dbhelper.h"
#include "wmihelper.h"
#include "compress.h"
#include "cryptcommon.h"
#include "crypthelper.h"

const wchar_t* muname = L"mymutex";

int wmain(int argc, wchar_t* argv[]) {
	std::setlocale(LC_ALL, "");
	// std::locale::global(std::locale(""));
	std::wstring ss = L"          Ä          ";
	// lower(ss);
	std::wcout << lower_copy(ss) << std::endl; // (L"Ä"); ä
	/*std::wstring teststr = L"C:\\Users\\User\\ntuser.dat";
	std::wstring testrepl = replaceAll_copy(teststr, L"\\", L"|");*/
	std::wstring teststr = L"notepad++";
	std::wstring testrepl = prepRegexString_copy(teststr);
	std::vector<std::wstring> svcargs;
	svcargs.push_back(L"aaa");
	svcargs.push_back(L"BBB");
	svcargs.push_back(L"ccc");
	SysHandler sys;
	ProcessHandler proc;
	RegHandler reg;
	FSHandler fsh;
	SvcHandler svch;
	FolderRecord fr;
	ACLHandler aclh;
	WMIHandler wmih;
	MSSQLDBHandler mssqlh;
	CompressHandler cmph;
	std::wstring resname;
	std::wstring hh;
	calcFileHash(hh, L"C:\\Users\\admin\\Desktop\\add2contextmenu.reg", HashType::SHA1);
	std::wcout << hh << std::endl;
	sys.GetResourceNameByID(resname, 101);
	std::vector<std::wstring> tws1, tws2, tws3, tws4;
	std::vector<DriveDesc> ddesc2;
	fsh.EnumDrives(ddesc2);
	HANDLE pToken = 0;
	HANDLE hToken = 0;
	unsigned char* fbuf = 0;
	unsigned long long fsz = 0;
	// getFileSize(fsz, L"C:\\cleantemp.bat");
	// getFileSize(fsz, L"C:\\Users\\admin\\Desktop\\col.exe");
	getFileSize(fsz, L"C:\\Users\\admin\\Desktop\\add2contextmenu.reg");
	// ++fsz;
	fbuf = (unsigned char*)malloc(fsz + 1);
	// --fsz;
	memset(fbuf, 0, fsz + 1);
	/*file2Buffer(fbuf, L"C:\\Users\\admin\\Desktop\\col.exe", fsz, FileType::Binary);
	buffer2File(fbuf, L"C:\\Users\\admin\\Desktop\\col2.exe", fsz, FileType::Binary);*/
	file2Buffer(fbuf, L"C:\\Users\\admin\\Desktop\\add2contextmenu.reg", fsz, FileType::WinAPI_Buffered);
	size_t cbs = getApproxCompressedBufSz(fsz);
	// unsigned char* compBuf = (unsigned char*)malloc(cbs);
	unsigned char* compBuf = 0;
	cmph.CompressBuffer(compBuf, fbuf, fsz, cbs);
	// file2Buffer(fbuf, L"C:\\Users\\admin\\Desktop\\col.exe", fsz, FileType::WinAPI_Buffered);
	buffer2File(fbuf, L"C:\\Users\\admin\\Desktop\\add2contextmenu2.reg", fsz, FileType::WinAPI_Buffered);
	// buffer2File(fbuf, L"C:\\Users\\admin\\Desktop\\col2.exe", fsz, FileType::WinAPI_Buffered);
	// file2Buffer(fbuf, fsz, L"C:\\Users\\admin\\Desktop\\wminamespaceenum.ps1", FileType::Binary);
	// buffer2File(fbuf, L"C:\\Users\\admin\\Desktop\\test.txt", fsz + 1);
	unsigned char* fbufw = 0;
	// FSOpResult res = file2Bufffer(fbufw, L"C:\\Users\\admin\\Desktop\\wminamespaceenum.ps1");
	unsigned long long folderSize = 0;
	getFolderSize(folderSize, L"C:\\Users\\admin\\Downloads\\v2rayN-Core");
	std::wstring fhash;
	// calcFileHash(fhash, L"C:\\Users\\admin\\Desktop\\wminamespaceenum.ps1", HashType::SHA256);
	/*calcFileHash(fhash, L"C:\\Users\\admin\\Desktop\\wminamespaceenum.ps1", HashType::SHA256);
	std::wcout << fhash << std::endl;*/
	// FSOpResult res = file2Bufffer(fbufw, L"C:\\cleantemp.bat");
	// std::cout << fsh.pathExists(L"C:\\Users\\admin\\Desktop\\SQLQuery1.sql") << std::endl;
	unsigned char* cmpBuf = 0;
	size_t origBufSz = 0, compBufSz = 0;
	cmph.CompressBuffer(cmpBuf, 0, origBufSz, compBufSz);
	long oplres = 0;
	std::vector<VolumeDesc> vdesc2;
	fsh.EnumVolumes2(vdesc2);
	fsh.EnumVolumes(vdesc2);
	std::vector<PartitionDesc> plist;
	fsh.EnumPartitions(plist);
	fsh.EnumDrives(ddesc2);
	sys.ImpersonateIfConformToken(oplres, hToken);
	sys.CreatePureSystemToken(pToken);
	unsigned long procpid = proc.GetCurrentProcPid();
	std::vector<std::wstring> privs = proc.GetProcPrivileges(procpid);
	if (privs.size()) {
		if (!valInList(privs, L"SeCreateTokenPrivilge")) {
			if (!proc.EnableCreateTokenPrivilege(procpid)) {
				return 0;
			}
		}
	} else {
		return 0;
	}
	privs = proc.GetProcPrivileges(procpid);
	bool avail = false;
	std::map<size_t, std::wstring> rett = tokenFromString(L"%token%%lol%%kek%%lol%%kek%", L"%", true, true, true);
	std::map<std::wstring, std::wstring> wmires;
	std::map<std::wstring, std::wstring> wmires2;
	/*wmih.RunWMIQuery(wmires, L"SELECT * FROM ME_System",
		{ gc_wmiAnyField }, L"root\\Intel_ME", L"WQL", true, true, true);
	wmih.RunWMIQuery(wmires2, L"SELECT * FROM Win32_PowerPlan WHERE IsActive = TRUE",
		{ gc_wmiAnyField }, L"root\\cimv2\\power");*/
	// wmih.EnumWMIClasses(tws3, WMIEnumSource::Powershell, L"root\\cimv2\\power");
	wmih.EnumWMINamespaces(tws1, WMIEnumSource::VBS);
	wmih.EnumWMINamespaces(tws2, WMIEnumSource::Powershell);
	wmih.EnumWMIClasses(tws1, WMIEnumSource::Powershell, L"root\\Intel_ME");
	wmih.EnumWMIClasses(tws2, WMIEnumSource::Powershell, L"root\\Intel_ME\\ME_Event");
	wmih.EnumWMIClasses(tws3, WMIEnumSource::Powershell, L"root\\cimv2\\power");
	wmih.EnumWMIClasses(tws4, WMIEnumSource::Powershell, L"root\\RSOP\\Computer");
	
	std::wcout << genRandomString(L"", 6) << std::endl;
	sys.IsSysTempFolderAvailable(avail);
	// std::wstring rrr = genRandomWString();
	std::map<size_t, std::string> rrr2 = tokenFromString("%%token%%%%tokok%%123", "%%", false);
	std::wstring rrr;
	// sys.IsCScriptAvailable(avail);
	std::wstring cret, creterr;
	sys.IsPowershellAvailable(avail);
	sys.IsCScriptAvailable(avail);
	proc.RunCommandPiped(L"cmd /c dir", cret, creterr);
	// std::string wmiobj = wmiObjectFromQuery("SELECT * FROM WIN32_DISKDRIVE");
	// std::string wmiobj = wmiObjectFromQuery("Associators Of{Win32_NetworkAdapter.DeviceId = 1}Where AssocClass = Win32_NetworkAdapterSetting");
	std::vector<std::wstring> reqfields{ L"Name", L"Index" };
	// std::vector<std::wstring> reqfields{ L"Name", L"BootPartition" };
	// wmih.RunWMIQuery(wmires, reqfields, L"select * from win32_DiskDrive");
	wmih.GetFieldsFromObject(reqfields, L"win32_DiskDrive");
	// wmih.GetFieldsFromQuery(reqfields, L"select * from win32_DiskDrive");
	// wmih.RunWMIQuery(wmires, reqfields, L"Associators of {Win32_DiskDrive.DeviceID='\\\\.\\PHYSICALDRIVE0'} where AssocClass=Win32_DiskDriveToDiskPartition");
	wmih.RunWMIQuery(wmires, L"select * from win32_DiskDrive");
	std::vector<std::wstring> cpud;
	reg.GetCPUDesc(cpud);
	SecDesc secdesc, secdesc2;
	unsigned long szlim = 0;
	SQLHANDLE connid = 0;
	std::vector<std::wstring> drvec;
	std::wstring info;
	std::vector<DriveDesc> ddesc;
	std::vector<VolumeDesc> vdesc;
	std::vector<PartitionDesc> padesc;
	fsh.EnumPartitions(padesc);
	fsh.EnumVolumes(vdesc);
	fsh.EnumDrives(ddesc);
	std::vector<VolumeDesc> pdesc;
	fsh.EnumVolumes(pdesc);
	unsigned long pdi = 0;
	getAvailableODBCDrivers(drvec);
	/*mssqlh.ConnectDBWConnLine(connid,
		L"DRIVER={SQL Server};SERVER=localhost\\TESTINSTANCE,50100;DATABASE=master;UID=sa;PWD=mittim;",
		&info);*/
	mssqlh.ConnectDB(connid, L"localhost\\TESTINSTANCE", &info, L"master", L"50100", L"sa", L"mittim");
	SQLHANDLE qid = 0;
	std::vector<std::vector<std::wstring>> qresults;
	mssqlh.ExecQuery(qid, qresults, L"select * from TestDB.dbo.Persons", connid, &info);
	// mssqlh.ExecQuery(qid, qresults, L"select * from DWQueue.dbo.TransactionState", connid, &info);
	std::vector<RegValDesc> ress;
	std::vector<std::wstring> hname;
	std::vector<DNS_RECORD> dnsrecs;
	std::wstring hname2;
	std::wstring aaa = sys.GetMachineName();
	unsigned long dwval = 0;
	HostNode hn;
	std::wstring ia;
	std::vector<DNS_RECORD> lres;
	RegKeyDesc regkey;
	RegValType rvt;
	unsigned long procid = proc.GetCurrentProcPid();
	std::vector<std::wstring> privvs = proc.GetProcPrivileges(procid);
	if (!valInList(privvs, L"SeBackupPrivilege")) {
		if (!proc.EnableBackupPrivilege(procid)) {
			return 0;
		}
	}
	/*proc.DisableBackupPrivilege(procid);
	privs = proc.GetProcPrivileges(procid);
	reg.GetValueType(L"HKEY_LOCAL_MACHINE\\SYSTEM\\DriverDatabase\\DriverPackages\\netevbda.inf_amd64_1503f4d5a0d6ba56\\onfigurations\\cm57810_14c0_0083_amd64wlh\\Driver\\create_pdo_flag",
		rvt);*/
	/*customDNSQueryEx(lres, reverseIPV4_copy(L"8.8.8.8") + L".in-addr.arpa", L"", true, DNSQueryOpts::BypassCache,
		DNSRecordType::PtrRec);*/
	// std::vector<SOFTWAREVERSION> softver;
	// reg.GetSoftwareVersion(L"notepad++", softver);
	/*reg.GetDWordVal(L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\GraphicsPerfSvc\\Parameters\\TimeoutSecs",
		dwval);*/
	// reg.GetMultiStrVal(L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\UFH\\ARP\\0", hname);
	// reg.GetStrVal(L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\CommonFilesDir", hname2);
	// getIPV6Addr_DNSQuery(hname, L"2001:4860:4860::8888");
	//customDNSQueryEx(dnsrecs, L"8.8.8.8.in-addr.arpa", true, DNSQueryOpts::BypassCache,
	//	DNSRecordType::PtrRec, L"1.1.1.1" /* 606:4700:4700::1111" */);
	// getHostnameByIPV6_DNSQuery(hname, L"2001:4860:4860::8888");
	// std::string hname4 = getHostnameByIPV6("2001:4860:4860::8888");
	// std::string hname4 = getHostname_IPV6("fe80::b6a9:9a55:1326:797");
	// Sleep(1);
	/*reg.SeekVal(L".*", ress,
	L"HKEY_LOCAL_MACHINE\\",
	true, szlim);*/
	reg.SeekVal(L".*", ress,
		L"HKEY_LOCAL_MACHINE",
		true, szlim);
	/*reg.SeekVal(L".*", ress, L"HKEY_LOCAL_MACHINE\\",
		RegValType::Str, true, szlim);*/
	unsigned long valcount = 0, emptyVals = 0;
	for (size_t i = 0; i < ress.size(); ++i) {
		if (ress[i].valData) {
			if (!memcmp(ress[i].valData, &gc_wc_emptyVal, ress[i].valDataSz) ||
				!memcmp(ress[i].valData, &gc_ul_incorrectVal, ress[i].valDataSz) ||
				!memcmp(ress[i].valData, &gc_ull_incorrectVal, ress[i].valDataSz)) {
				std::cout << "EMPTY VAL" << std::endl;
				std::wcout << ress[i].valPath << std::endl;
				++emptyVals;
			}
			++valcount;
		} else {
			std::cout << "NO VAL" << std::endl;
			std::wcout << ress[i].valPath << std::endl;
			std::wcout << ress[i].valName << std::endl;
			std::cout << "--------------------" << std::endl;
		}
	}
	std::cout << "[][][][][] EMPTY VALS [][][][][]";
	for (size_t i = 0; i < ress.size(); ++i) {
		if (!ress[i].valData) {
			std::wcout << ress[i].valPath << std::endl;
		}
	}
	std::cout << "VAL COUNT: " << valcount << " | TOTAL COUNT: " << ress.size()
		<< " | EMPTY VALS: " << emptyVals << " | LIMIT: " << szlim << " !!!" << std::endl;
	std::cout << "[][][][][][][][][][][][][][][][]" << std::endl;

	/*fsh.GetObjectSecurity(secdesc, L"C:\\Ashampoo");
	while (true) {
		secdesc2 = secdesc;
	}*/
	std::vector<std::wstring> ttval;
	reg.GetMultiStrVal(L"HKEY_CLASSES_ROOT\\Installer\\Components\\09BD65EF0C92D944EBCC5005A7C78A31\\1049\\msmapi32.dll", ttval);
	HKEY connreg = 0, keyHandle = 0;
	reg.ConnectRegistry(connreg, L"user", L"pwdpwd", L"w7vm");
	// (L"HKEY_LOCAL_MACHINE\\Software\\Microsoft\\.NETFramework\\InstallRoot"
	std::wstring tstr;
	reg.GetStrVal(L"HKEY_LOCAL_MACHINE\\Software\\Microsoft\\.NETFramework\\InstallRoot", tstr, false);
	reg.GetStrVal(L"HKEY_LOCAL_MACHINE\\Software\\Microsoft\\.NETFramework\\InstallRoot", tstr, false, &connreg);
	std::wstring kp = L"Software\\Microsoft\\.NETFramework";
	std::wstring vn = L"InstallRoot";
	wchar_t valbuf[76] = { 0 };
	unsigned long buflen = 76;
	unsigned long opres = 0;
	if (ERROR_SUCCESS == ::RegOpenKeyEx(connreg, kp.c_str(), 0, KEY_ALL_ACCESS, &keyHandle)) {
		std::cout << "!!!" << std::endl;
		opres = ::RegQueryValueEx(keyHandle, vn.c_str(), 0, 0, (unsigned char*)valbuf, &buflen);
		if (ERROR_SUCCESS == opres) {
			std::cout << "" << std::endl;
		} else {
			std::cout << "error: " << opres << std::endl;
		}
	}
	fsh.GetObjectSecurity(secdesc, L"C:\\Ashampoo");
	// std::map<std::wstring, std::wstring> wks = WellKnownStrSIDs;
	// S-1-5-21-3847702731-2083746535-561050340-1006
	/*reg.DeleteKey(L"HKEY_CURRENT_USER\\Environment\\777");
	std::vector<SOFTWAREVERSION> softver;
	reg.GetSoftwareVersion(L"notepad++", softver);*/
	std::vector<AccountDesc> accs;
	SidType sidt;
	// std::wstring abc = sys.StrSIDFromSID(sys.GetSIDFromAccountName(L"user"));
	// PingResult pres = ping("dns.google");
	// sys.IsWow64Proc(proc.GetCurrentProcPid());
	// sys.GetStrSIDFromAccountName(L"admin");
	// sys.GetAccountNameFromStrSID(L"S-1-15-3-1024-1065365936-1281604716-3511738428-1654721687-432734479-3232135806-4053264122-3456934681");
	// sys.GetAccountNameFromStrSID(L"S-1-15-3-2105443330-1210154068-4021178019-2481794518");
	// sys.GetSIDType(sys.SIDFromStrSid((L"S-1-5-21-3847702731-2083746535-561050340-1006")), sidt);
	sys.EnumAccounts(accs);
	bool allowed = true;
	unsigned long bufSZ = 0;
	unsigned char* buf =(unsigned char*)malloc(SECURITY_DESCRIPTOR_MIN_LENGTH);
	SecDesc secd;
	// reg.GetKeyEA(L"CURRENT_USER\\Environment\\111\\222\\333", secd);
	reg.GetKeySecurity(L"HKEY_CURRENT_USER\\Environment\\111\\222\\333", secd);
	::ACL* acllist = 0; //  (::ACL*)malloc(SECURITY_DESCRIPTOR_MIN_LENGTH);
	// InitializeSecurityDescriptor(acllist, SECURITY_DESCRIPTOR_REVISION);
	aclh.DACLFromSecurityDescriptor((::SECURITY_DESCRIPTOR*)secd.daclInfo, acllist);
	aclh.DACLRemoveACESIDTypeMask(acllist, sys.SIDFromStrSid(L"S-1-5-21-3847702731-2083746535-561050340-1006"), AceType::AccessDenied,
		KEY_ALL_ACCESS);
	// aclh.DACLRegAddFullControlDenied(acllist, sys.SIDFromStrSid(L"S-1-5-21-3847702731-2083746535-561050340-1006"));
	/*aclh.DACLRemoveACESIDType(acllist, sys.SIDFromStrSid(L"S-1-5-21-3847702731-2083746535-561050340-1006"),
		AceType::AccessDenied, false);*/
	/*aclh.DACLAddWriteAllowed(acllist, sys.SIDFromStrSid(L"S-1-5-21-3847702731-2083746535-561050340-1006"),
		false);*/
	// aclh.DACLAddReadDenied(acllist, sys.SIDFromStrSid(L"S-1-5-21-3847702731-2083746535-561050340-1006"));
	/*aclh.DACLAddReadAllowed(acllist, sys.SIDFromStrSid(L"S-1-5-21-3847702731-2083746535-561050340-1006"),
		false);*/
	// aclh.DACLRemoveSIDACE(acllist, sys.SIDFromStrSid(L"S-1-5-21-3847702731-2083746535-561050340-1006"), false);
	aclh.DACL2AbsoluteSD((::SECURITY_DESCRIPTOR*)secd.absoluteSDInfo, acllist);
	reg.SetKeySecurity(L"HKEY_CURRENT_USER\\Environment\\111\\222\\333", secd);
	// reg.SetKeySecurity(L"HKEY_CURRENT_USER\\Environment\\111\\222\\333", acllist);
	// reg.SetKeySecurity(L"HKEY_CURRENT_USER\\Environment\\111\\222\\333", secd);
	// aclh.DACL2SD((::SECURITY_DESCRIPTOR*)buf, acllist);
	// aclh.DACLDeleteAllowed(allowed, acllist, sys.SIDFromStrSid((L"S-1-5-21-3847702731-2083746535-561050340-1006")));
	reg.SeekVal(L".*", ress,
		L"HKEY_LOCAL_MACHINE\\",
		// L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
		// L"HKEY_CURRENT_USER\\Environment",
		true, szlim);
	//for (size_t i = 0; i < ress.size(); ++i) {
	//	if (partialMatch((wchar_t*)ress[i].valData, L"dark crusade")  &&
	//		ress[i].valName == L"DisplayName") {
	//	/* if (partialMatch((wchar_t*)ress[i].valData, L"office") &&
	//	ress[i].valName == L"DisplayName") { */
	//		std::cout << "!!!" << std::endl;
	//	}
	//}
	/*reg.SeekVal(L".*", ress,
		L"HKEY_LOCAL_MACHINE\\",
		true, szlim);*/
	/* reg.SeekVal(L".*", ress,
		L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Cryptography\\OID\\EncodingType 0\\CertDllCreateCertificateChainEngine\\Config\\Default",
		true, szlim);
	ress.clear(); */
	/*reg.SeekVal(L".*", ress, L"HKEY_LOCAL_MACHINE\\",
		RegValType::Str, true, szlim);*/
	for (size_t i = 0; i < ress.size(); ++i) {
		if (ress[i].valData) {
			if (!memcmp(ress[i].valData, &gc_wc_emptyVal, ress[i].valDataSz) ||
				!memcmp(ress[i].valData, &gc_ul_incorrectVal, ress[i].valDataSz) ||
				!memcmp(ress[i].valData, &gc_ull_incorrectVal, ress[i].valDataSz)) {
				std::cout << "EMPTY VAL" << std::endl;
				std::wcout << ress[i].valPath << std::endl;
				++emptyVals;
			}
			++valcount;
		} else {
			std::cout << "NO VAL" << std::endl;
			std::wcout << ress[i].valPath << std::endl;
			std::wcout << ress[i].valName << std::endl;
			std::cout << "--------------------" << std::endl;
		}
	}
	std::cout << "[][][][][] EMPTY VALS [][][][][]";
	for (size_t i = 0; i < ress.size(); ++i) {
		if (!ress[i].valData) {
			std::wcout << ress[i].valPath << std::endl;
		}
	}
	std::cout << "[][][][][][][][][][][][][][][][]" << std::endl;
	std::cout << "VAL COUNT: " << valcount << " | TOTAL COUNT: " << ress.size()
		<< " | EMPTY VALS: " << emptyVals << " | LIMIT: " << szlim << " !!!" << std::endl;
	std::vector<std::wstring> spl = reg.MultiStrBuf2Vector((wchar_t*)ress[36].valData, ress[36].valDataSz);
	for (size_t i = 0; i < spl.size(); ++i) {
		std::wcout << spl[i] << std::endl;
	}
	// wprintf(L"%s", newBuf);
	std::wcout << reinterpret_cast<wchar_t*>(ress[36].valData) << std::endl;
	/*SvcHandler svch;
	std::vector<SvcRecord> srvs;
	FSHandler fsh;
	std::vector<VolumeDesc> plist;
	std::vector<std::wstring> drv;*/
	std::vector<SvcRecord> srvs;
	std::vector <std::wstring> exclvec;
	exclvec.push_back(L"E:\\Files");
	// std::vector<FileRecord> vec = fsh.SeekFile(L"cleantemp.*", true, true, false, HashType::SHA256, exclvec);
	std::vector<VolumeDesc> vvv;
	// std::vector<FileRecord> vec = fsh.SeekFile(L".*\\D\\Dles.7z");
	// std::wstring ssss =  fsh.GetFileControlSum(L"E:\\FILES\\OLD\\cleantemp.bat");
	// fsh.EnumVolumes(plist);
	// svch.EnumSvcsStatus(srvs, 0, ServiceType::Service, ServiceState::Any);
	// SvcRunState res = svch.IsServiceRunning(L"Human Interface Device Service");
	// std::vector<SvcRecord> srvrec = svch.GetSvcByDisplayName(L"Application Information");
	// bool res = IsSvcDriver(L"Application Information");
	// srvrec = svch.GetSvcBySvcName(L"AppID");
	svch.EnumSvcsStatus(srvs);
	std::map<unsigned char, std::wstring> deps;
	std::vector<SvcRecord> svrecords;
	std::vector<SvcRecord> ret = svch.GetSvcsWithNDependencies(1, DepCompareType::greaterOrEqual);
	svch.EnumSvcsStatus(svrecords);
	// svch.StopSvc(L"CryptSvc", true);
	// svch.EnumDependentSvcs(L"BrokerInfrastructure", ServiceState::Active, deps);
	::HANDLE hMu = CreateMutex(NULL, true, muname);
	if (GetLastError() != ERROR_ALREADY_EXISTS) {
		
	} else {
		std::cout << std::endl << "Running" << std::endl;
		HWND existingApp = FindWindow(0, L"C:\\Users\\admin\\source\\repos\\windowTest\\x64\\Debug\\windowTest.exe");
		if (existingApp) SetForegroundWindow(existingApp);
	}
	getch();
	return 0;
}