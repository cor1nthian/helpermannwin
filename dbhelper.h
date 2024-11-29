#ifndef _DBHELPER_H
#define _DBHELPER_H

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
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <sal.h>
#include "config.h"
#include "strhelper.h"

enum class MSSQLOpResult : unsigned char {
	Success,
	Fail
};

enum class MSSQLDriverType : unsigned char {
	SQLServer,
	Custom
};

enum class MSSQLDriverPreference : unsigned char  {
	SQLServer,
	Any
};

enum class MSSQLConnTrust : unsigned char {
	Undefined,
	Trusted,
	NotTrusted
};

enum class PGSQLOpResult : unsigned char {
	Success,
	Fail
};

const std::map<MSSQLDriverType, std::wstring> const gc_SQLDriverType = {
	{ MSSQLDriverType::SQLServer, L"SQL Server" }
};

MSSQLOpResult getAvailableODBCDrivers(std::vector<std::wstring> &drivers, const bool includeAttrs = false);

struct MSSQLBinding {
	MSSQLBinding();
	MSSQLBinding(const MSSQLBinding &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	MSSQLBinding(MSSQLBinding &&other) noexcept;
#endif
	~MSSQLBinding();
	MSSQLBinding& operator=(const MSSQLBinding &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	MSSQLBinding& operator=(MSSQLBinding &&other) noexcept;
#endif
	bool operator==(const MSSQLBinding &other) const;
	bool operator!=(const MSSQLBinding &other) const;
	short			cDisplaySize;	/* size to display  */
	wchar_t*		wszBuffer;		/* display buffer   */
#ifdef _WIN64
	long long		indPtr;			/* size or null     */
#else
	long			indPtr;			/* size or null     */
#endif
	int				fChar;			/* character col?   */
	MSSQLBinding	*sNext;			/* linked list      */
};

struct MSSQLOutBuf {
	MSSQLOutBuf();
	MSSQLOutBuf(const size_t outbufsz);
	MSSQLOutBuf(const wchar_t* outbuf, const size_t outbufsz);
	MSSQLOutBuf(const MSSQLOutBuf &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	MSSQLOutBuf(MSSQLOutBuf &&other) noexcept;
#endif
	~MSSQLOutBuf();
	MSSQLOutBuf& operator=(const MSSQLOutBuf &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	MSSQLOutBuf& operator=(MSSQLOutBuf &&other) noexcept;
#endif
	bool operator==(const MSSQLOutBuf &other) const;
	bool operator!=(const MSSQLOutBuf &other) const;
	wchar_t* OutBuf;
	size_t OutBufSz;
};

struct MSSQLQuery {
	MSSQLQuery();
	MSSQLQuery(const SQLHANDLE dbid, const SQLHSTMT queryid, const std::wstring querystr);
	MSSQLQuery(const MSSQLQuery &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	MSSQLQuery(MSSQLQuery &&other) noexcept;
#endif
	~MSSQLQuery();
	MSSQLQuery& operator=(const MSSQLQuery &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	MSSQLQuery& operator=(MSSQLQuery &&other) noexcept;
#endif
	bool operator==(const MSSQLQuery &other) const;
	bool operator!=(const MSSQLQuery &other) const;
	SQLHANDLE DBID;
	SQLHSTMT QueryID;
	std::wstring QueryStr;
};

class MSSQLDBHandler {
	public:
		MSSQLDBHandler();
		MSSQLDBHandler(const SQLHANDLE henv, const std::map<SQLHANDLE, std::wstring> connectedDBs,
			const std::map<SQLHANDLE, MSSQLOutBuf> outBuffers, std::vector<MSSQLQuery> runningQueries);
		MSSQLDBHandler(const MSSQLDBHandler &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		MSSQLDBHandler(MSSQLDBHandler &&other) noexcept;
#endif
		~MSSQLDBHandler();
		MSSQLDBHandler& operator=(const MSSQLDBHandler &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		MSSQLDBHandler& operator=(MSSQLDBHandler &&other) noexcept;
#endif
		bool operator==(const MSSQLDBHandler &other) const;
		bool operator!=(const MSSQLDBHandler &other) const;
		MSSQLOpResult ConnectDB(SQLHANDLE &connID, const std::wstring serverAddr, std::wstring *infoBuf = 0,
			const std::wstring dbName = L"", const std::wstring port = L"50100", const std::wstring login = L"",
			const std::wstring pwd = L"", const MSSQLConnTrust trustRel = MSSQLConnTrust::Undefined,
			const MSSQLDriverType driverType = MSSQLDriverType::SQLServer,
			const MSSQLDriverPreference driverPref = MSSQLDriverPreference::SQLServer,
			const std::wstring defaultDriver = L"", /* L"SQL Server Native Client RDA 11.0", */
			const unsigned long connOutBufLen = MSSQLMAXOUTBUF);
		MSSQLOpResult ConnectDBWConnLine(SQLHANDLE &connID, const std::wstring connLine, std::wstring *infoBuf = 0);
		MSSQLOpResult DisconnectDB(const SQLHANDLE connDBID = 0, const std::wstring serverAddr = L"",
			std::wstring *infoBuf = 0);
		MSSQLOpResult ExecQuery(SQLHANDLE &queryID, std::vector<std::vector<std::wstring>> &results,
			const std::wstring querystr, const SQLHANDLE dbHadle = 0,
			std::wstring *infoBuf = 0);
		MSSQLOpResult CancelQuery(const SQLHSTMT queryID = 0, const std::wstring query = L"",
			std::wstring *infoBuf = 0);
	protected:

	private:
		/*       FUNCTIONS       */
		MSSQLOpResult EvalSQLResult(const short resultCode, const SQLHANDLE objectID,
			const unsigned char queryType, std::wstring *infoBuf, const unsigned short infoBufLen) const;
		MSSQLOpResult QueryComplete(std::vector<std::vector<std::wstring>> &results, SQLHSTMT queryHandle = 0,
			const std::wstring queryStr = L"", std::wstring *infoBuf = 0);
		MSSQLOpResult QueryCancelled(std::wstring *infoBuf = 0);
		MSSQLOpResult SQLAllocateBindings(const SQLHSTMT queryHandle, const short colNum, MSSQLBinding **binding,
			short *display, std::wstring *infoBuf = 0) const;
		MSSQLOpResult SQLGetTitles(std::vector<std::vector<std::wstring>> &results, const SQLHSTMT queryHandle,
			unsigned long displaySize, MSSQLBinding* binding, std::wstring* infoBuf = 0);
		MSSQLOpResult SQLInfoDetails(const SQLHANDLE handle, const short recordType, const short code,
			wchar_t* &infoBuf, const size_t infoBufSz) const;
		/*       VARIABLES       */
		SQLHANDLE m_hEnv;
		std::map<SQLHANDLE, std::wstring> m_ConnectedDBs;
		std::map<SQLHANDLE, MSSQLOutBuf> m_OutBuffers;
		std::vector<MSSQLQuery> m_RunningQueries;
};

class PGSQLDBHandler {
	public:
		PGSQLDBHandler();
		PGSQLDBHandler(const std::map<unsigned long, std::wstring> connectedDBS,
			const std::map<unsigned long, std::wstring> runningQueries);
		PGSQLDBHandler(const PGSQLDBHandler &other);
		PGSQLDBHandler(PGSQLDBHandler &&other) noexcept;
		~PGSQLDBHandler();
		PGSQLDBHandler& operator=(const PGSQLDBHandler &other);
		PGSQLDBHandler& operator=(PGSQLDBHandler && other) noexcept;
		bool operator==(const PGSQLDBHandler &other) const;
		bool operator!=(const PGSQLDBHandler &other) const;
		PGSQLOpResult ConnectDB(const std::wstring serverAddr, const std::wstring login, const std::wstring pwd);
		PGSQLOpResult DisconnectDB(const std::wstring serverAddr);
		PGSQLOpResult ExecQuery(unsigned long &queyID, const std::wstring query);
		PGSQLOpResult CancelQuery(const unsigned long queyID);
	protected:

	private:
		/*       FUNCTIONS       */
		PGSQLOpResult QueryComplete();
		PGSQLOpResult QueryCancelled();
		/*       VARIABLES       */
		std::map<unsigned long, std::wstring> m_ConnectedDBs;
		std::map<unsigned long, std::wstring> m_RunningQueries;
};

#endif // _DBHELPER_H