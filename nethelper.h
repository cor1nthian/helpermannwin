#ifndef _NETHELPER_H
#define _NETHELPER_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS

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

#if defined(_WIN32) || defined(_WIN64) 
	#define _WINSOCKAPI_
	#include <winsock2.h>	
	#include <ws2tcpip.h>
	#include <iphlpapi.h>
	#include <Windows.h>
	#include <windns.h>
	#include <Mstcpip.h>
	#include <icmpapi.h>
	#include <stdio.h>
#else
	#include <netdb.h>
	#include <arpa/inet.h>
#endif

#include "config.h"
#include "strhelper.h"
#include "prochelper.h"

#define ICMP_ECHOREPLY		0
#define ICMP_DESTUNREACH	3
#define ICMP_SRCQUENCH		4
#define ICMP_REDIRECT		5
#define ICMP_ECHO			8
#define ICMP_TIMEOUT		11
#define ICMP_PARMERR		12

#define MAXPINGATTEMPTS		1024
#define MAXPINGTIMEOUT		3000
#define MAXTIMEOUTBWPINGS	5000
#define MAXTRACERTTIMEOUT	3000
#define MAXTRACERTHOPS		255
#define MAXTRACERTPINGS		32

struct HostNodeAddr;
struct HostNode;
struct PingResult;
struct TracertResult;
struct ICMPHeader;
struct IPHeader;

#define PINGMULTIPLEEPSA std::map<std::string, std::vector<PingResult>>
#define PINGMULTIPLEEPSW std::map<std::wstring, std::vector<PingResult>>
#define TRACERTMULTIPLEEPS std::vector<TracertResult>
#define TRACERTMULTIPLESPSEPSA std::map<std::string, std::vector<TracertResult>>
#define TRACERTMULTIPLESPSEPSW std::map<std::wstring, std::vector<TracertResult>>

const std::vector<unsigned short int> const gc_CommonPorts = {
	80,     // HTTP
	443,    // HTTPS
	21,     // FTP
	22,     // FTPS / SSH
	53,     // DNS
	110,    // POP3
	995,    // POP3 SSL
	143,    // IMAP
	993,    // IMAP SSL
	25,     // SMTP
	26,     // SMTP
	587,    // SMTP SSL
	3306,   // MySQL
	2082,   // cPanel
	2083,   // cPanel SSL
	2086,   // WHM(Webhost Manager)
	2087,   // WHM(Webhost Manager) SSL
	2095,   // Webmail
	2096,   // Webmail SSL
	2077,   // WebDAV / WebDisk
	2078    // WebDAV / WebDisk SSL
};

const std::map<unsigned short int, std::wstring> const gc_CommonPortsNames = {
	{ 80, L"HTTP" },
	{ 443, L"HTTPS" },
	{ 21, L"FTP" },
	{ 22, L"FTPS / SSH" },
	{ 53, L"DNS" },
	{ 110, L"POP3" },
	{ 995, L"POP3 SSL" },
	{ 143, L"IMAP" },
	{ 993, L"IMAP SSL" },
	{ 25, L"SMTP" },
	{ 26, L"SMTP" },
	{ 587, L"SMTP SSL" },
	{ 3306, L"MySQL" },
	{ 2082, L"cPanel" },
	{ 2083, L"cPanel SSL" },
	{ 2086, L"WHM(Webhost Manager)" },
	{ 2087, L"WHM(Webhost Manager) SSL" },
	{ 2095, L"Webmail" },
	{ 2096, L"Webmail SSL" },
	{ 2077, L"WebDAV / WebDisk" },
	{ 2078, L"WebDAV / WebDisk SSL" }
};

#if defined(_WIN32) || defined(_WIN64)
	extern bool g_WSAStarted;
#endif

enum class NetOpResult : unsigned char {
	Success,
	Fail
};

enum class AddressFamily : unsigned long {
	Unspecified = AF_UNSPEC,
	IPV4 = AF_INET,
	IPV6 = AF_INET6,
	Unix = AF_UNIX,
	ImpLink = AF_IMPLINK,
	PUP = AF_PUP,
	Chaos = AF_CHAOS,
	NS = AF_NS,
	IPX = AF_IPX,
	ISO = AF_ISO,
	OSI = AF_OSI,
	ECMA = AF_ECMA,
	Datakit = AF_DATAKIT,
	CCITT = AF_CCITT,
	SNA = AF_SNA,
	DECnet= AF_DECnet,
	DLI = AF_DLI,
	LAT = AF_LAT,
	Hylink = AF_HYLINK,
	Appletalk = AF_APPLETALK,
	NetBIOS = AF_NETBIOS,
	Voiceview = AF_VOICEVIEW,
	Firefox = AF_FIREFOX,
	Unknown1 = AF_UNKNOWN1,
	Ban = AF_BAN,
	ATM = AF_ATM,
	Cluster = AF_CLUSTER,
	Family12844 = AF_12844,
	IrDA = AF_IRDA,
	NetDES = AF_NETDES
};

enum class DNSRecordType : unsigned long {
	ARec = DNS_TYPE_A,
	NSRec = DNS_TYPE_NS,
	MDRec = DNS_TYPE_MD,
	MFRec = DNS_TYPE_MF,
	CNameRec = DNS_TYPE_CNAME,
	SOARec = DNS_TYPE_SOA,
	MBRec = DNS_TYPE_MB,
	MGRec = DNS_TYPE_MG,
	MRRec = DNS_TYPE_MR,
	NullRec = DNS_TYPE_NULL,
	WKSRec = DNS_TYPE_WKS,
	PtrRec = DNS_TYPE_PTR,
	HInfoRec = DNS_TYPE_HINFO,
	MInfoRec = DNS_TYPE_MINFO,
	MXRec = DNS_TYPE_MX,
	TextRec = DNS_TYPE_TEXT,
	RPRec = DNS_TYPE_RP,
	AFSDBRec = DNS_TYPE_AFSDB,
	X25Rec = DNS_TYPE_X25,
	ISDNRec = DNS_TYPE_ISDN,
	RTRec = DNS_TYPE_RT,
	NSAPRec = DNS_TYPE_NSAP,
	NSAPPtrRec = DNS_TYPE_NSAPPTR,
	SigRec = DNS_TYPE_SIG,
	KeyRec = DNS_TYPE_KEY,
	PXRec = DNS_TYPE_PX,
	GPOSRec = DNS_TYPE_GPOS,
	AAAARec = DNS_TYPE_AAAA,
	LocRec = DNS_TYPE_LOC,
	NXTRec = DNS_TYPE_NXT,
	EIDRec = DNS_TYPE_EID,
	NIMLOCRec = DNS_TYPE_NIMLOC,
	SrvRec = DNS_TYPE_SRV,
	ATMARec = DNS_TYPE_ATMA,
	NAPTRRec = DNS_TYPE_NAPTR,
	KXRec = DNS_TYPE_KX,
	CertRec = DNS_TYPE_CERT,
	A6Rec = DNS_TYPE_A6,
	DNameRec = DNS_TYPE_DNAME,
	SinkRec = DNS_TYPE_SINK,
	OPTRec = DNS_TYPE_OPT,
	DSRec = DNS_TYPE_DS,
	RRSIGRec = DNS_TYPE_RRSIG,
	NSecRec = DNS_TYPE_NSEC,
	DNSKetRec = DNS_TYPE_DNSKEY,
	DHCIDRec = DNS_TYPE_DHCID,
	UInfoRec = DNS_TYPE_UINFO,
	UIDRec = DNS_TYPE_UID,
	GIDRec = DNS_TYPE_GID,
	UnspecRec = DNS_TYPE_UNSPEC,
	AddRSRec = DNS_TYPE_ADDRS,
	TKeyRec = DNS_TYPE_TKEY,
	TSigRec = DNS_TYPE_TSIG,
	IXFRRec = DNS_TYPE_IXFR,
	AXFRRec = DNS_TYPE_AXFR,
	MailBRec = DNS_TYPE_MAILB,
	MailARec = DNS_TYPE_MAILA,
	AllRec = DNS_TYPE_ALL,
	AnyRec = DNS_TYPE_ANY,
	WINSRec = DNS_TYPE_WINS,
	WINSRRec = DNS_TYPE_WINSR,
	MBStatRec = DNS_TYPE_NBSTAT
};

enum class DNSClassType : unsigned long {
	InternetClass = DNS_CLASS_INTERNET,
	CSNetClass = DNS_CLASS_CSNET,
	ChaosClass = DNS_CLASS_CHAOS,
	HESIODClass = DNS_CLASS_HESIOD,
	NoneClass = DNS_CLASS_NONE,
	AllClass = DNS_CLASS_ALL,
	AnyClass = DNS_CLASS_ANY
};

enum class DNSQueryType : unsigned long {
	QueryOpcode = DNS_OPCODE_QUERY,
	IQueryOpcode = DNS_OPCODE_IQUERY,
	ServerStatusOpCode = DNS_OPCODE_SERVER_STATUS,
	UnknownOpcode = DNS_OPCODE_UNKNOWN,
	NotigyOpcode = DNS_OPCODE_NOTIFY,
	UpdateOpcode = DNS_OPCODE_UPDATE
};

enum class DNSQueryOpts : unsigned long {
	// Standard query. Resets specified flags
	Standard = DNS_QUERY_STANDARD,
	// Returns truncated results. Does not retry under TCP
	QueryAcceptTruncatedResponse = DNS_QUERY_ACCEPT_TRUNCATED_RESPONSE,
	// Uses TCP only for the query
	UseTCPOnly = DNS_QUERY_USE_TCP_ONLY,
	/* Directs the DNS server to perform an iterative query(specifically
		directs the DNS server not to perform recursive resolution to resolve the query). */
	NoRecursion = DNS_QUERY_NO_RECURSION,
	// Bypasses the resolver cache on the lookup
	BypassCache = DNS_QUERY_BYPASS_CACHE,
	/* Directs DNS to perform a query on the local cache only.Windows 2000 Server and
		Windows 2000 Professional: This value is not supported. For similar functionality, use DNS_QUERY_CACHE_ONLY */
	NoWireQuery = DNS_QUERY_NO_WIRE_QUERY,
	/* Directs DNS to ignore the local name.
		Windows 2000 Server and Windows 2000 Professional: This value is not supported */
	NoLocalName = DNS_QUERY_NO_LOCAL_NAME,
	/* Prevents the DNS query from consulting the HOSTS file.
		Windows 2000 Server and Windows 2000 Professional: This value is not supported */
	NoHostFile = DNS_QUERY_NO_HOSTS_FILE,
	/* Prevents the DNS query from using NetBT for resolution.
		Windows 2000 Server and Windows 2000 Professional: This value is not supported */
	NoNetBT = DNS_QUERY_NO_NETBT,
	/* Directs DNS to perform a query using the network only, bypassing local information.
		Windows 2000 Server and Windows 2000 Professional: This value is not supported */
	WireOnly = DNS_QUERY_WIRE_ONLY,
	/* Directs DNS to return the entire DNS response message.
		Windows 2000 Server and Windows 2000 Professional: This value is not supported */
	ReturnMesage = DNS_QUERY_RETURN_MESSAGE,
	/* Prevents the query from using DNS and uses only Local Link Multicast Name Resolution(LLMNR).
		Windows Vista and Windows Server 2008 or later.: This value is supported */
	MulticastOnly = DNS_QUERY_MULTICAST_ONLY,
	// Disable multicast
	NoMulticast = DNS_QUERY_NO_MULTICAST,
	// Prevents the DNS response from attaching suffixes to the submitted name in a name resolution process.
	TreatAsFQDN = DNS_QUERY_TREAT_AS_FQDN,
	/* Windows 7 only: Do not send A type queries if IPv4 addresses are not available on an interface and
		do not send AAAA type queries if IPv6 addresses are not available */
	AddrConfig = DNS_QUERY_ADDRCONFIG,
	/* Windows 7 only: Query both AAAA and A type records and return results for each.Results for A type
		records are mapped into AAAA type. */
	DualAddr = DNS_QUERY_DUAL_ADDR,
	/*  If set, and if the response contains multiple records, records are stored with the TTL corresponding to
		the minimum value TTL from among all records. When this option is set, "Do not change the TTL of
		individual records" in the returned record set is not modified */
	DoNotResetTTLValues = DNS_QUERY_DONT_RESET_TTL_VALUES,
	/* Disables International Domain Name (IDN) encoding support in the DnsQuery, DnsQueryEx, DnsModifyRecordsInSet,
		and DnsReplaceRecordSet APIs. All punycode names are treated as ASCII and will be ASCII encoded on the wire.
		All non-ASCII names are encoded in UTF8 on the wire. Windows 8 or later: This value is supported */
	DisableIDNEncoding = DNS_QUERY_DISABLE_IDN_ENCODING,
	// Append multilabel
	AppendMultilabel = DNS_QUERY_APPEND_MULTILABEL,
	// Reserved
	Reserved = DNS_QUERY_RESERVED
};

enum class DNSRecordFlags : unsigned long {
	QuestionFlag = DNSREC_QUESTION,
	AnswerFlag = DNSREC_ANSWER,
	AuthorutyFlag = DNSREC_AUTHORITY,
	AdditionalFlag = DNSREC_ADDITIONAL,
	// Flag refers to an RR's section within an update DNS message per RFC 2136
	ZoneFlag = DNSREC_ZONE,
	// Flag refers to an RR's section within an update DNS message per RFC 2136
	PrereqFlag = DNSREC_PREREQ,
	// Flag refers to an RR's section within an update DNS message per RFC 2136
	UpdateFkag = DNSREC_UPDATE,
	// Mutually exclusive with NoExistFlag
	DeleteFlag = DNSREC_DELETE,
	// Mutually exclusive with DeleteFlag
	NoExistFlag = DNSREC_NOEXIST
};

enum class DNSUpdateOpts : unsigned long {
	// Uses the default behavior, which is specified in the registry, for secure dynamic DNS updates.
	SecurityUseDefault = DNS_UPDATE_SECURITY_USE_DEFAULT,
	// Does not attempt secure dynamic updates
	SecurityOff = DNS_UPDATE_SECURITY_OFF,
	// Does not attempt secure dynamic updates
	UpdateSecurityOff = DNS_UPDATE_SECURITY_OFF,
	// Attempts non-secure dynamic update; if refused, attempts secure dynamic update.
	UpdateSecurityOn = DNS_UPDATE_SECURITY_ON,
	// Attempts secure dynamic updates only
	UpdateSecurityOnly = DNS_UPDATE_SECURITY_ONLY,
	// Caches the security context for use in future transactions
	UpdateCacheSecurityContext = DNS_UPDATE_CACHE_SECURITY_CONTEXT,
	// Uses credentials of the local computer account
	TestUseLocalSysAccount = DNS_UPDATE_TEST_USE_LOCAL_SYS_ACCT,
	// Does not use cached security context
	ForceSecurityNegotiation = DNS_UPDATE_FORCE_SECURITY_NEGO,
	// Sends DNS updates to all multi-master DNS servers
	TryAllMasterServers = DNS_UPDATE_TRY_ALL_MASTER_SERVERS,
	/* Do not update adapters where dynamic DNS updates are disabled.
		Windows 2000 Server with SP2 or later.: This value is supported. */
	SkipNUpdateAdapters =  DNS_UPDATE_SKIP_NO_UPDATE_ADAPTERS,
	/* Register CNAME records on a remote server in addition to the local DNS server.
		Windows 2000 Server with SP2 or later.: This value is supported */
	RemoteServer = DNS_UPDATE_REMOTE_SERVER,
	// Reserved
	Reserved = DNS_UPDATE_RESERVED
};

enum class DNSResponseCode : unsigned long {
	// No error
	NoError = DNS_RCODE_NOERROR,
	// Format error
	FormatError = DNS_RCODE_FORMERR,
	// Server failure
	ServerFail = DNS_RCODE_SERVFAIL,
	// Name error
	NXDomain = DNS_RCODE_NXDOMAIN,
	// Not implemented
	NotImplmented = DNS_RCODE_NOTIMPL,
	// Connection refused
	Refused = DNS_RCODE_REFUSED,
	// Domain name should not exist
	YXDomain = DNS_RCODE_YXDOMAIN,
	// Resource Record (RR) set should not exist
	YXRRSet = DNS_RCODE_YXRRSET,
	// RR set does not exist
	NXRRSet = DNS_RCODE_NXRRSET,
	// Not authoritative for zone
	NotAuthorative = DNS_RCODE_NOTAUTH,
	// Name not in zone
	NotZone = DNS_RCODE_NOTZONE,
	// Bad Extension Mechanism for DNS (EDNS) version
	BadVersion = DNS_RCODE_BADVERS,
	// Bad signature
	BadSignature = DNS_RCODE_BADSIG,
	// Bad key
	BadKey = DNS_RCODE_BADKEY,
	// Bad timestamp
	BadTimestamp = DNS_RCODE_BADTIME
};

const std::map<DNSResponseCode, std::wstring> const gc_DnsResultTest = {
	{ DNSResponseCode::NoError, L"No error" },
	{ DNSResponseCode::FormatError, L"Format error" },
	{ DNSResponseCode::ServerFail, L"Server failure" },
	{ DNSResponseCode::NXDomain, L"Name error" },
	{ DNSResponseCode::NotImplmented, L"Not implemented" },
	{ DNSResponseCode::Refused, L"Connection refused" },
	{ DNSResponseCode::YXDomain, L"Domain name should not exist" },
	{ DNSResponseCode::YXRRSet, L"Resource Record (RR) set should not exist" },
	{ DNSResponseCode::NXRRSet, L"Resource Record set does not exist" },
	{ DNSResponseCode::NotAuthorative, L"Not authoritative for zone" },
	{ DNSResponseCode::NotZone, L"Name not in zone" },
	{ DNSResponseCode::BadSignature, L"Bad signature" },
	{ DNSResponseCode::BadKey, L"Bad key" },
	{ DNSResponseCode::BadTimestamp, L"Bad timestamp" },
};

enum class DNSProtocol : unsigned long {
	Unspecified = DNS_PROTOCOL_UNSPECIFIED,
	UDP = DNS_PROTOCOL_UDP,
	TCP = DNS_PROTOCOL_TCP,
	DOH = DNS_PROTOCOL_DOH,
	NoWire = DNS_PROTOCOL_NO_WIRE
};

enum class SocketType : unsigned long {
	Stream = SOCK_STREAM,
	Datagram = SOCK_DGRAM,
	Raw = SOCK_RAW,
	RDM = SOCK_RDM,
	SeqPacket = SOCK_SEQPACKET
};

enum class AddressType : unsigned char {
	IPv4,
	IPv6,
	NetBios
};

enum class NWProtocol : unsigned long {
	HopOpts = IPPROTO::IPPROTO_HOPOPTS,
	ICMP = IPPROTO::IPPROTO_ICMP,
	IGMP = IPPROTO::IPPROTO_IGMP,
	GGP = IPPROTO::IPPROTO_GGP,
	TCP = IPPROTO::IPPROTO_TCP,
	UDP = IPPROTO::IPPROTO_UDP,
	PUP = IPPROTO::IPPROTO_PUP,
	IDP = IPPROTO::IPPROTO_IDP,
	RDP = IPPROTO::IPPROTO_RDP,
	ESP = IPPROTO::IPPROTO_ESP,
	AH = IPPROTO::IPPROTO_AH,
	ND = IPPROTO::IPPROTO_ND,
	ICLFXBM = IPPROTO::IPPROTO_ICLFXBM,
	PIM = IPPROTO::IPPROTO_PIM,
	PGM = IPPROTO::IPPROTO_PGM,
	L2TP = IPPROTO::IPPROTO_L2TP,
	SCTP = IPPROTO::IPPROTO_SCTP,
	IPv6None = IPPROTO::IPPROTO_NONE,
	IPv6DSTOpts = IPPROTO::IPPROTO_DSTOPTS,
	IPv6Routing = IPPROTO::IPPROTO_ROUTING,
	IPv6Fragment = IPPROTO::IPPROTO_FRAGMENT,
	ICMPv6 = IPPROTO::IPPROTO_ICMPV6,
	IPV4 = IPPROTO::IPPROTO_IPV4,
	IPV6 = IPPROTO::IPPROTO_IPV6
};

struct DNSQueryContext {
	DNSQueryContext();
	DNSQueryContext(const unsigned long refCount, const wchar_t* queryName, const unsigned long queryNameLen,
		const unsigned short int queryType, const unsigned long queryOptions, const DNS_QUERY_RESULT queryRes,
		const DNS_QUERY_CANCEL queryCancel, const ::HANDLE queryCompletedEvent);
	DNSQueryContext(const DNSQueryContext &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	DNSQueryContext(DNSQueryContext &&other) noexcept;
#endif
	~DNSQueryContext();
	DNSQueryContext& operator=(const DNSQueryContext &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	DNSQueryContext& operator=(DNSQueryContext &&other) noexcept;
#endif
	bool operator==(const DNSQueryContext &other) const;
	bool operator!=(const DNSQueryContext &other) const;
	unsigned long RefCount;
	wchar_t QueryName[DNS_MAX_NAME_BUFFER_LENGTH + 16];
	unsigned short QueryType;
	unsigned long QueryOptions;
	DNS_QUERY_RESULT QueryResults;
	DNS_QUERY_CANCEL QueryCancelContext;
	::HANDLE QueryCompletedEvent;
};

struct HostNodeAddr {
	HostNodeAddr();
	HostNodeAddr(const SocketType sockType, const NWProtocol protocol, const AddressType addrType,
		const std::wstring address);
	HostNodeAddr(const HostNodeAddr &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	HostNodeAddr(HostNodeAddr &&other) noexcept;
#endif
	~HostNodeAddr();
	HostNodeAddr& operator=(const HostNodeAddr &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	HostNodeAddr& operator=(HostNodeAddr &&other) noexcept;
#endif
	bool operator==(const HostNodeAddr &other) const;
	bool operator!=(const HostNodeAddr &other) const;
	SocketType SockType;
	NWProtocol Protocol;
	AddressType AddrType;
	std::wstring Address;
};

struct HostNode {
	HostNode();
	HostNode(const std::vector<HostNodeAddr> address);
	HostNode(const HostNode &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	HostNode(HostNode&& other) noexcept;
#endif
	~HostNode();
	HostNode& operator=(const HostNode &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	HostNode& operator=(HostNode &&other) noexcept;
#endif
	bool operator==(const HostNode &other) const;
	bool operator!=(const HostNode &other) const;
	std::vector<HostNodeAddr> Address;
};

struct PingResult {
	PingResult();
	PingResult(const bool result, const unsigned short int rtt, const unsigned short int ttl);
	PingResult(const PingResult &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	PingResult(PingResult &&other) noexcept;
#endif
	~PingResult();
	PingResult& operator=(const PingResult &other);
	PingResult& operator=(PingResult &&other) noexcept;
	bool operator==(const PingResult &other) const;
	bool operator!=(const PingResult &other) const;
	bool Result;
	unsigned short int TTL;
	unsigned short int RoundTripTime;
};

struct TracertResult {
	TracertResult();
	TracertResult(const std::wstring address, const std::wstring addressv4,
		const std::wstring addressv6, const unsigned short int ttl,
		const unsigned short int rtt, const std::vector<PingResult> pings);
	TracertResult(const TracertResult &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	TracertResult(TracertResult &&other) noexcept;
#endif
	TracertResult& operator=(const TracertResult &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	TracertResult& operator=(TracertResult &&other) noexcept;
#endif
	bool operator==(const TracertResult &other) const;
	bool operator!=(const TracertResult &other) const;
	~TracertResult();
	unsigned short int TTL;
	unsigned short int RoundTripTime;
	std::wstring Address;
	std::wstring AddressIPV4;
	std::wstring AddressIPV6;
	std::vector<PingResult> Pings;
};

struct ICMPHeader {
	ICMPHeader();
	ICMPHeader(const unsigned char Type, const unsigned char Code, const unsigned short int Checksum,
		const unsigned long ID, const unsigned short int SeqNum, const unsigned long Timestamp);
	ICMPHeader(const ICMPHeader &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	ICMPHeader(ICMPHeader &&other) noexcept;
#endif
	~ICMPHeader();
	ICMPHeader& operator=(const ICMPHeader &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	ICMPHeader& operator=(ICMPHeader &&other) noexcept;
#endif
	bool operator==(const ICMPHeader &other) const;
	bool operator!=(const ICMPHeader &other) const;
	unsigned char type;				// ICMP message type
	unsigned char code;				// Sub code
	unsigned short checksum;		// Checksum
	unsigned short seqnum;			// Sequence number
	unsigned long id;				// Unique id
	// This is not the std header, but we reserve space for time
	unsigned long timestamp;
};

struct IPHeader {
	IPHeader();
	IPHeader(const unsigned int headerLen, const unsigned int ver, const unsigned char tos, const unsigned short int packlen,
		const unsigned short int id, const unsigned short int packetflags, const unsigned char timetolive,
		const unsigned char proto, const unsigned short int chksum, const unsigned int srcIP,
		const unsigned int destinationIP);
	IPHeader(const IPHeader &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	IPHeader(IPHeader &&other) noexcept;
#endif
	~IPHeader();
	IPHeader& operator=(const IPHeader &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	IPHeader& operator=(IPHeader &&other) noexcept;
#endif
	bool operator==(const IPHeader &other) const;
	bool operator!=(const IPHeader &other) const;
	unsigned int   headerlen;		// Length of the header
	unsigned int   version;			// Version of IP
	unsigned char  typeofoservice;	// Type of service
	unsigned short packetlen;		// Total length of the packet
	unsigned short identifier;		// Unique identifier
	unsigned short flags;			// Flags
	unsigned char  ttl;				// Time to live
	unsigned char  protocol;		// Protocol (TCP, UDP etc)
	unsigned short checksum;		// IP checksum
	unsigned int   sourceIP;		// Source IP
	unsigned int   destIP;			// Destination IP
};

NetOpResult ping(std::vector<PingResult> &results, const std::string address, const unsigned short numAttempts = 4,
	const unsigned short timeout = 1000, const unsigned short timeoutBetweenPings = 1000);
NetOpResult ping(std::vector<PingResult> &results, const std::wstring address, const unsigned short numAttempts = 4,
	const unsigned short timeout = 1000, const unsigned short timeoutBetweenPings = 1000);
NetOpResult ping_MultipleEndPoints(PINGMULTIPLEEPSA &results, const std::string address,
	const std::string portOrSvcName = "80", const unsigned short numAttempts = 4,
	const unsigned short timeout = 1000, const unsigned short timeoutBetweenPings = 1000);
NetOpResult ping_MultipleEndPoints(PINGMULTIPLEEPSW &results, const std::wstring address,
	const std::wstring portOrSvcName = L"80", const unsigned short numAttempts = 4,
	const unsigned short timeout = 1000, const unsigned short timeoutBetweenPings = 1000);
NetOpResult traceroute(std::vector<TracertResult> &results, const std::string address,  const unsigned char maxHops = 30,
	const bool doPings = true, const unsigned short int tracertTimeout = 1000, const unsigned short int pingAttempts = 4,
	const unsigned short int pingTimeout = 1000, const unsigned short int timeoutBetweenPings = 1000);
NetOpResult traceroute(std::vector<TracertResult> &results, const std::wstring address, const unsigned char maxHops = 30,
	const bool doPings = true, const unsigned short int tracertTimeout = 1000, const unsigned short int pingAttempts = 4,
	const unsigned short int pingTimeout = 1000, const unsigned short int timeoutBetweenPings = 1000);
NetOpResult traceroute_MultipleEndPoints(TRACERTMULTIPLEEPS &results, const std::string address,
	const std::string portOrSvcName = "80", const unsigned char maxHops = 30,
	const bool doPings = true, const unsigned short int tracertTimeout = 1000,
	const unsigned short int pingAttempts = 4, const unsigned short int pingTimeout = 1000,
	const unsigned short int timeoutBetweenPings = 1000);
NetOpResult traceroute_MultipleEndPoints(TRACERTMULTIPLEEPS &results, const std::wstring address,
	const std::wstring portOrSvcName = L"80", const unsigned char maxHops = 30,
	const bool doPings = true, const unsigned short int tracertTimeout = 1000,
	const unsigned short int pingAttempts = 4, const unsigned short int pingTimeout = 1000,
	const unsigned short int timeoutBetweenPings = 1000);
NetOpResult traceroute_MultipleStartPointsMultipleEndPoints(TRACERTMULTIPLESPSEPSA &results,
	const std::string address, const std::string portOrSvcName = "80", const unsigned char maxHops = 30,
	const bool doPings = true, const bool strictEndPointMatch = false, const unsigned short int tracertTimeout = 1000,
	const unsigned short int pingAttempts = 4, const unsigned short int pingTimeout = 1000,
	const unsigned short int timeoutBetweenPings = 1000);
NetOpResult traceroute_MultipleStartPointsMultipleEndPoints(TRACERTMULTIPLESPSEPSW &results,
	const std::wstring address, const std::wstring portOrSvcName = L"80", const unsigned char maxHops = 30,
	const bool doPings = true, const bool strictEndPointMatch = false,  const unsigned short int tracertTimeout = 1000,
	const unsigned short int pingAttempts = 4, const unsigned short int pingTimeout = 1000,
	const unsigned short int timeoutBetweenPings = 1000);
NetOpResult traceroute_RawSocket(std::vector<TracertResult> &results, const std::string address,
	const unsigned char maxHops = 30);
NetOpResult traceroute_RawSocket(std::vector<TracertResult> &results, const std::wstring address,
	const unsigned char maxHops = 30);
NetOpResult lookupIPAddresses(HostNode &node, const std::string dnsName,
	const std::string portOrSvcName = "80", const AddressFamily addressFmaily = AddressFamily::IPV4,
	const SocketType socketType = SocketType::Stream, const NWProtocol protocol = NWProtocol::TCP);
NetOpResult lookupIPAddresses(HostNode &node, const std::wstring dnsName,
	const std::wstring portOrSvcName = L"80", const AddressFamily addressFmaily = AddressFamily::IPV4, 
	const SocketType socketType = SocketType::Stream, const NWProtocol protocol = NWProtocol::TCP);
NetOpResult getHostnameByIPV4_DNSQuery(std::string &hostName, const std::string ipAddr,
	const std::string dnsAddr = "", const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getHostnameByIPV4_DNSQuery(std::wstring &hostName, const std::wstring ipAddr,
	const std::wstring dnsAddr = L"", const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getHostnameByIPV6_DNSQuery(std::string &hostName, const std::string ipAddr,
	const std::string dnsAddr = "", const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getHostnameByIPV6_DNSQuery(std::wstring &hostName, const std::wstring ipAddr,
	const std::wstring dnsAddr = L"", const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getIPV4AddrByHostname_DNSQuery(std::string &ipAddr, const std::string hostName,
	const std::string dnsAddr = "", const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getIPV4AddrByHostname_DNSQuery(std::wstring &ipAddr, const std::wstring hostName,
	const std::wstring dnsAddr = L"", const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getIPV6AddrByHostname_DNSQuery(std::string &ipAddr, const std::string hostName,
	const std::string dnsAddr = "", const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getIPV6AddrByHostname_DNSQuery(std::wstring &ipAddr, const std::wstring hostName,
	const std::wstring dnsAddr = L"", const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult customDNSQuery(PDNS_RECORD &queryResults, const std::string objectName,
	const std::wstring dnsAddr = L"", const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache,
	const DNSRecordType dnsRecordType = DNSRecordType::CNameRec);
NetOpResult customDNSQuery(PDNS_RECORD &queryResults, const std::wstring objectName,
	const std::wstring dnsAddr = L"", const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache,
	const DNSRecordType dnsRecordType = DNSRecordType::CNameRec);
NetOpResult getIPV4AddrByHostname_DNSQueryEx(std::vector<std::wstring> &ipAddrs, const std::string hostName,
	const std::string dnsAddr = "", const bool async = true,
	const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getIPV4AddrByHostname_DNSQueryEx(std::vector<std::wstring> &ipAddrs, const std::wstring hostName,
	const std::wstring dnsAddr = L"", const bool async = true,
	const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getIPV6AddrByHostname_DNSQueryEx(std::vector<std::wstring> &ipAddrs, const std::string hostName,
	const std::string dnsAddr = "", const bool async = true,
	const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getIPV6AddrByHostname_DNSQueryEx(std::vector<std::wstring> &ipAddrs, const std::wstring hostName,
	const std::wstring dnsAddr = L"", const bool async = true,
	const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getHostnameByIPV4_DNSQueryEx(std::vector<std::wstring> &hostName, const std::string ipV4Addr,
	const std::string dnsIPV4Addr = "", const bool async = true,
	const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getHostnameByIPV4_DNSQueryEx(std::vector<std::wstring> &hostName, const std::wstring ipV4Addr,
	const std::wstring dnsIPV4Addr = L"", const bool async = true,
	const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getHostnameByIPV6_DNSQueryEx(std::vector<std::wstring> &results, const std::string objectName,
	const std::string dnsIPV4Addr = "", const bool async = true,
	const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult getHostnameByIPV6_DNSQueryEx(std::vector<std::wstring> &results, const std::wstring objectName,
	const std::wstring dnsIPV4Addr = L"", const bool async = true,
	const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache);
NetOpResult customDNSQueryEx(std::vector<DNS_RECORD> &queryResults, const std::string objectName,
	const std::string dnsIPV4Addr = "", const bool async = true,
	const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache,
	const DNSRecordType dnsRecordType = DNSRecordType::CNameRec);
NetOpResult customDNSQueryEx(std::vector<DNS_RECORD> &queryResults, const std::wstring objectName,
	const std::wstring dnsIPV4Addr = L"", const bool async = true,
	const DNSQueryOpts queryOptions = DNSQueryOpts::BypassCache,
	const DNSRecordType dnsRecordType = DNSRecordType::CNameRec);
std::wstring getDNSOpTextResult(const DNSResponseCode resultCode);
std::string lookupIPV4Address(const std::string dnsName);
std::wstring lookupIPV4Address(const std::wstring dnsName);
std::string getHostnameByIPV4(const std::string ip, unsigned short int port = 80);
std::wstring getHostnameByIPV4(const std::wstring ip, unsigned short int port = 80);
std::string getHostnameByIPV6(const std::string ip, unsigned short int port = 80);
std::wstring getHostnameByIPV6(const std::wstring ip, unsigned short int port = 80);
void __stdcall DNSQueryExQueryCompleteCallback(void* context, DNS_QUERY_RESULT* queryResult);
unsigned short ICMPHeaderChecksum(unsigned short* buffer, int size);
int decodeResponse(char* buf, int bytes, SOCKADDR_IN* from, int ttl);

#endif // _NETHELPER_H