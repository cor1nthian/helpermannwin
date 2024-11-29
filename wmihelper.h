#ifndef _WMIHELPER_H
#define _WMIHELPER_H

#define _WIN32_DCOM
#include <comdef.h>
#include <oledb.h>
#include <Wbemidl.h>
#include "config.h"
#include "syshelper.h"
#include "strhelper.h"

const std::wstring const gc_wmiEmptyval		= L"(null)";
const std::wstring const gc_wmiArray		= L"(array)";
const std::wstring const gc_wmiAnyField		= L"*";
const std::wstring const gc_wmiJoinSymbol	= L" ]|[ ";

const std::wstring const gc_wmiCScriptNamespaceQuery = L"WScript.Echo \"QueryResults:\"\n\
strComputer = \".\"\n\
Call EnumNameSpaces(\"?replace?\")\n\
Sub EnumNameSpaces(strNameSpace)\n\
WScript.Echo strNameSpace\n\
Set objWMIService=GetObject(\"winmgmts:{impersonationLevel=impersonate}\\\\\"&strComputer&\"\\\"&strNameSpace)\n\
Set colNameSpaces=objWMIService.InstancesOf(\"__NAMESPACE\")\n\
For Each objNameSpace In colNameSpaces\n\
Call EnumNameSpaces(strNameSpace&\"\\\"&objNameSpace.Name)\n\
Next\n\
End Sub";
const std::wstring const gc_wmiCScriptClassesQuery = L"WSCript.Echo \"QueryResults:\"\n\
Set objWMIService=GetObject(\"winmgmts:{impersonationLevel=impersonate}?replace?\")\n\
Set colClasses=objWMIService.SubClassesOf\n\
For Each objClass In colClasses\n\
WScript.Echo objClass.Path_.Class\n\
Next";

const std::wstring const gc_wmiPSNamespaceQuery = L"function EnumNamespace{Param([Parameter(Position=0,Mandatory=$true)]\
[System.String]$namespaceName)[System.String[]]$namespaces=Get-CimInstance -Class __Namespace -Namespace $namespaceName|\
ForEach-Object Name;foreach($nspc in $namespaces){$nspc = $namespaceName +'\\' + $nspc;EnumNamespace $nspc;\
Write-Output $nspc;}}Write-Output QueryResults:;Write-Output ?replace?;EnumNamespace ?replace?";
const std::wstring const gc_wmiPSClassesQuery = L"Write-Output QueryResults:;\
Get-CimClass -Namespace ?replace?|ForEach-Object CimClassName;";

enum class WMIOpResult : unsigned char {
	Success,
	Fail
};

enum class SortOrder : unsigned char {
	Ascending,
	Descending
};

enum class WMIEnumSource : unsigned char {
	VBS,
	Powershell,
	Native
};

class WMIQueryAsyncSink;
class WMIHandler;

// https://learn.microsoft.com/en-us/windows/win32/wmisdk/wmi-c---application-examples
// https://learn.microsoft.com/en-us/windows/win32/wmisdk/iwbemobjectsink
class WMIQueryAsyncSink : public ::IWbemObjectSink {
	public:
		WMIQueryAsyncSink();
		WMIQueryAsyncSink(const WMIQueryAsyncSink &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		WMIQueryAsyncSink(WMIQueryAsyncSink &&other) noexcept;
#endif
		WMIQueryAsyncSink& operator=(const WMIQueryAsyncSink &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		WMIQueryAsyncSink& operator=(WMIQueryAsyncSink &&other) noexcept;
#endif
		bool operator==(const WMIQueryAsyncSink &other) const;
		bool operator!=(const WMIQueryAsyncSink &other) const;
		~WMIQueryAsyncSink();
		// https://learn.microsoft.com/en-us/windows/win32/wmisdk/iwbemobjectsink
		virtual unsigned long STDMETHODCALLTYPE AddRef();
		virtual unsigned long STDMETHODCALLTYPE Release();
		virtual ::HRESULT STDMETHODCALLTYPE QueryInterface(const GUID &riid, void** ppv);
		virtual ::HRESULT STDMETHODCALLTYPE Indicate(long lObjectCount,
			::IWbemClassObject __RPC_FAR* __RPC_FAR* apObjArray);
		virtual ::HRESULT STDMETHODCALLTYPE SetStatus(long lFlags, ::HRESULT hResult, ::BSTR strParam,
			::IWbemClassObject __RPC_FAR* pObjParam);
		bool IsDone();
		WMIOpResult SetHandler(WMIHandler *wmiHandler);
	protected:
	private:
		int m_bDone;
		long m_lRef;
		WMIHandler* m_WMIHandler;
		::CRITICAL_SECTION m_threadLock;
};

class WMIHandler {
	public:
		WMIHandler();
		WMIHandler(const WMIHandler &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		WMIHandler(WMIHandler &&other) noexcept;
#endif
		WMIHandler& operator=(const WMIHandler &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		WMIHandler& operator=(WMIHandler &&other) noexcept;
#endif
		bool operator==(const WMIHandler &other) const;
		bool operator!=(const WMIHandler &other) const;
		~WMIHandler();
		friend class WMIQueryAsyncSink;
		WMIOpResult InitWMI(const std::wstring namespacePath = L"ROOT\\CIMV2");
		WMIOpResult RunWMIQuery(std::map<std::wstring, std::wstring> &results,
			const std::wstring query, const std::vector<std::wstring> requiredFields = { gc_wmiAnyField },
			const std::wstring namespacePath = L"ROOT\\CIMV2", const std::wstring queryLanguage = L"WQL",
			const bool clearResults = true, const bool initWMI = true, const bool shutdownWMI = false);
		WMIOpResult RunWMIQueryAsync(std::map<std::wstring, std::wstring> &results,
			const std::wstring query, const std::vector<std::wstring> requiredFields = { gc_wmiAnyField },
			const std::wstring namespacePath = L"ROOT\\CIMV2", const std::wstring queryLanguage = L"WQL",
			const bool clearResults = true, const bool initWMI = true, const bool shutdownWMI = false,
			const unsigned long awaitTimeout = 500);
		WMIOpResult GetFieldsFromQuery(std::vector<std::wstring> &fields, const std::wstring query,
			const std::wstring namespacePath = L"ROOT\\CIMV2", const std::wstring queryLanguage = L"WQL",
			const bool clearFields = true, const bool initWMI = true, const bool shutdownWMI = false);
		WMIOpResult GetFieldsFromObject(std::vector<std::wstring> &fields, const std::wstring objectName,
			const std::wstring namespacePath = L"ROOT\\CIMV2", const bool clearFields = true,
			const bool initWMI = true, const bool shutdownWMI = false);
		WMIOpResult EnumWMINamespaces(std::vector<std::wstring> &namespaces,
			const WMIEnumSource enumSource = WMIEnumSource::VBS, const std::wstring namespaceName = L"ROOT",
			const bool clearList = true, const bool sortList = true, const SortOrder sortOrder = SortOrder::Ascending,
			const HKEY *root = 0, const std::wstring namespacePath = L"ROOT", const bool initWMI = true,
			const bool shutdownWMI = false);
		WMIOpResult EnumWMIClasses(std::vector<std::wstring> &classes,
			const WMIEnumSource enumSource = WMIEnumSource::VBS, const std::wstring namespaceName = L"ROOT\\CIMV2",
			const bool clearList = true, const bool sortList = true, const SortOrder sortOrder = SortOrder::Ascending,
			const HKEY *root = 0, const bool initWMI = true, const bool shutdownWMI = false);
		WMIOpResult IsWMIAvailable(bool &available);
		WMIOpResult ShutdownWMI();
	protected:
		std::vector<std::wstring> m_asyncRequiredFields;
		std::map<std::wstring, std::wstring> *m_asyncResults;
	private:
		/*       FUNCTIONS       */
		WMIOpResult processQueryFields(std::map<std::wstring, std::wstring> &results,
			const std::vector<std::wstring> requiredFields, ::IEnumWbemClassObject* enumerator) const;
		WMIOpResult processQueryFieldsAsync(std::map<std::wstring, std::wstring>& results,
			const std::vector<std::wstring> requiredFields) const;
		/*       VARIABLES       */
		bool m_WMIInitialized;
		::IWbemLocator *m_pLoc;
		::IWbemServices *m_pSvc;
};

#endif // _WMIHELPER_H