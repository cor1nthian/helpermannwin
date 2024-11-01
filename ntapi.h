#ifndef _NTAPI_H
#define _NTAPI_H

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
#include "config.h"

// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ntqueryinformationfile
enum class FileInfo : unsigned char {
	FileDirectoryInformation = 1,
	FileFullDirectoryInformation,                   // 2
	FileDirectoryBothInformation,                   // 3
	FileBasicInformation,                           // 4
	FileStandardInformation,                        // 5
	FileInternalInformation,                        // 6
	FileEaInformation,                              // 7
	FileAccessInformation,                          // 8
	FileNameInformation,                            // 9
	FileRenameInformation,                          // 10
	FileLinkInformation,                            // 11
	FileNamesInformation,                           // 12
	FileDispositionInformation,                     // 13
	FilePositionInformation,                        // 14
	FileFullEaInformation,                          // 15
	FileModeInformation,                            // 16
	FileAlignmentInformation,                       // 17
	FileAllInformation,                             // 18
	FileAllocationInformation,                      // 19
	FileEndOfFileInformation,                       // 20
	FileAlternateNameInformation,                   // 21
	FileStreamInformation,                          // 22
	FilePipeInformation,                            // 23
	FilePipeLocalInformation,                       // 24
	FilePipeRemoteInformation,                      // 25
	FileMailslotQueryInformation,                   // 26
	FileMailslotSetInformation,                     // 27
	FileCompressionInformation,                     // 28
	FileObjectIdInformation,                        // 29
	FileCompletionInformation,                      // 30
	FileMoveClusterInformation,                     // 31
	FileQuotaInformation,                           // 32
	FileReparsePointInformation,                    // 33
	FileNetworkOpenInformation,                     // 34
	FileAttributeTagInformation,                    // 35
	FileTrackingInformation,                        // 36
	FileIdBothDirectoryInformation,                 // 37
	FileIdFullDirectoryInformation,                 // 38
	FileValidDataLengthInformation,                 // 39
	FileShortNameInformation,                       // 40
	FileIoCompletionNotificationInformation,        // 41
	FileIoStatusBlockRangeInformation,              // 42
	FileIoPriorityHintInformation,                  // 43
	FileSfioReserveInformation,                     // 44
	FileSfioVolumeInformation,                      // 45
	FileHardLinkInformation,                        // 46
	FileProcessIdsUsingFileInformation,             // 47
	FileNormalizedNameInformation,                  // 48
	FileNetworkPhysicalNameInformation,             // 49
	FileIdGlobalTxDirectoryInformation,             // 50
	FileIsRemoteDeviceInformation,                  // 51
	FileUnusedInformation,                          // 52
	FileNumaNodeInformation,                        // 53
	FileStandardLinkInformation,                    // 54
	FileRemoteProtocolInformation,                  // 55
	FileRenameInformationBypassAccessCheck,         // 56
	FileLinkInformationBypassAccessCheck,           // 57
	FileVolumeNameInformation,                      // 58
	FileIdInformation,                              // 59
	FileIdExtdDirectoryInformation,                 // 60
	FileReplaceCompletionInformation,               // 61
	FileHardLinkFullIdInformation,                  // 62
	FileIdExtdBothDirectoryInformation,             // 63
	FileDispositionInformationEx,                   // 64
	FileRenameInformationEx,                        // 65
	FileRenameInformationExBypassAccessCheck,       // 66
	FileDesiredStorageClassInformation,             // 67
	FileStatInformation,                            // 68
	FileMemoryPartitionInformation,                 // 69
	FileStatLxInformation,                          // 70
	FileCaseSensitiveInformation,                   // 71
	FileLinkInformationEx,                          // 72
	FileLinkInformationExBypassAccessCheck,         // 73
	FileStorageReserveIdInformation,                // 74
	FileCaseSensitiveInformationForceAccessCheck,   // 75
	FileKnownFolderInformation,                     // 76
	FileMaximumInformation							// 77
};

enum class FolderInfo : unsigned char {
	FileDirectoryInformation = 1,
	FileFullDirectoryInformation,					// 2
	FileDirectoryBothInformation,					// 3
	FileBasicInformation,							// 4
	FileStandardInformation,						// 5
	FileInternalInformation,						// 6
	FileEaInformation,								// 7
	FileAccessInformation,							// 8
	FileNameInformation,							// 9
	FileRenameInformation,							// 10
	FileLinkInformation,							// 11
	FileNamesInformation,							// 12
	FileDispositionInformation,						// 13
	FilePositionInformation,						// 14
	FileFullEaInformation,							// 15
	FileModeInformation,							// 16
	FileAlignmentInformation,						// 17
	FileAllInformation,								// 18
	FileAllocationInformation,						// 19
	FileEndOfFileInformation,						// 20
	FileAlternateNameInformation,					// 21
	FileStreamInformation,							// 22
	FilePipeInformation,							// 23
	FilePipeLocalInformation,						// 24
	FilePipeRemoteInformation,						// 25
	FileMailslotQueryInformation,					// 26
	FileMailslotSetInformation,						// 27
	FileCompressionInformation,						// 28
	FileObjectIdInformation,						// 29
	FileCompletionInformation,						// 30
	FileMoveClusterInformation,						// 31
	FileQuotaInformation,							// 32
	FileReparsePointInformation,					// 33
	FileNetworkOpenInformation,						// 34
	FileAttributeTagInformation,					// 35
	FileTrackingInformation,						// 36
	FileIdBothDirectoryInformation,					// 37
	FileIdFullDirectoryInformation,					// 38
	FileValidDataLengthInformation,					// 39
	FileShortNameInformation,						// 40
	FileMaximumInformation							// 41
};

enum class EvtType : unsigned char {
	NotificationEvent,
	SynchronizationEvent
};

/* typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
#ifdef MIDL_PASS
	[size_is(MaximumLength / 2), length_is((Length) / 2)] USHORT* Buffer;
#else // MIDL_PASS
	_Field_size_bytes_part_opt_(MaximumLength, Length) PWCH   Buffer;
#endif // MIDL_PASS
} UNICODE_STRING;
typedef UNICODE_STRING* PUNICODE_STRING;
typedef const UNICODE_STRING* PCUNICODE_STRING; */

//typedef struct _OBJECT_ATTRIBUTES {
//	ULONG Length;
//	HANDLE RootDirectory;
//	PUNICODE_STRING ObjectName;
//	ULONG Attributes;
//	PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR
//	PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE
//} OBJECT_ATTRIBUTES;
//typedef OBJECT_ATTRIBUTES* POBJECT_ATTRIBUTES;
//typedef const OBJECT_ATTRIBUTES* PCOBJECT_ATTRIBUTES;

struct UnicodeString {
	UnicodeString();
	UnicodeString(const unsigned short length, const unsigned short maxlength);
	UnicodeString(const unsigned short length, const unsigned short maxlength, const wchar_t* buffer);
	UnicodeString(const UnicodeString& other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	UnicodeString(UnicodeString&& other) noexcept;
#endif
	~UnicodeString();
	UnicodeString& operator=(const UnicodeString& other);
	UnicodeString& operator=(UnicodeString&& other) noexcept;
	bool operator==(const UnicodeString& other) const;
	bool operator!=(const UnicodeString& other) const;
	unsigned short	Length;
	unsigned short	MaximumLength;
	wchar_t*		Buffer;
};

struct ANSIString {
	ANSIString();
	ANSIString(const unsigned short length, const unsigned short maxlength);
	ANSIString(const unsigned short length, const unsigned short maxlength, const char* buffer);
	ANSIString(const ANSIString& other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	ANSIString(ANSIString&& other) noexcept;
#endif
	~ANSIString();
	ANSIString& operator=(const ANSIString& other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	ANSIString& operator=(ANSIString&& other) noexcept;
#endif
	bool operator==(const ANSIString& other) const;
	bool operator!=(const ANSIString& other) const;
	unsigned short	Length;
	unsigned short	MaximumLength;
	char*			Buffer;
};

struct ObjectAttributes {
	ObjectAttributes();
	ObjectAttributes(const ObjectAttributes& other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	ObjectAttributes(ObjectAttributes&& other) noexcept;
#endif
	~ObjectAttributes();
	ObjectAttributes& operator=(const ObjectAttributes& other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	ObjectAttributes& operator=(ObjectAttributes&& other) noexcept;
#endif
	bool operator==(const ObjectAttributes& other) const;
	bool operator!=(const ObjectAttributes& other) const;
	unsigned long	uLength;
	::HANDLE		hRootDirectory;
	UnicodeString*	pObjectName;
	unsigned long	uAttributes;
	void*			pSecurityDescriptor;		// Points to type SECURITY_DESCRIPTOR
	void*			pSecurityQualityOfService;	// Points to type SECURITY_QUALITY_OF_SERVICE
};

struct IOStatusBlock {
	IOStatusBlock();
	IOStatusBlock(const NTSTATUS status, const unsigned long long info);
	IOStatusBlock(const IOStatusBlock &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	IOStatusBlock(IOStatusBlock &&other) noexcept;
#endif
	~IOStatusBlock();
	IOStatusBlock& operator=(const IOStatusBlock& other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	IOStatusBlock& operator=(IOStatusBlock &&other);
#endif
	bool operator==(const IOStatusBlock &other) const;
	bool operator!=(const IOStatusBlock &other) const;
	union {
		NTSTATUS		Status;
		void*			Pointer;
	};
	unsigned long long	Information;
};

struct FileDirBothInformation {
	FileDirBothInformation();
	FileDirBothInformation(const FileDirBothInformation& other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	FileDirBothInformation(FileDirBothInformation&& other) noexcept;
#endif
	~FileDirBothInformation();
	FileDirBothInformation& operator=(const FileDirBothInformation& other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	FileDirBothInformation& operator=(FileDirBothInformation&& other) noexcept;
#endif
	bool operator==(const FileDirBothInformation& other) const;
	bool operator!=(const FileDirBothInformation& other) const;
	unsigned long	NextEntryOffset;
	unsigned long	FileIndex;
	::LARGE_INTEGER	CreationTime;
	::LARGE_INTEGER	LastAccessTime;
	::LARGE_INTEGER	LastWriteTime;
	::LARGE_INTEGER	ChangeTime;
	::LARGE_INTEGER	EndOfFile;
	::LARGE_INTEGER	AllocationSize;
	unsigned long	FileAttributes;
	unsigned long	FileNameLength;
	unsigned long	EaSize;
	char			ShortNameLength;
	wchar_t			ShortName[FSH_SHORTNAMELENGTH];
	wchar_t			FileName[1];
};

typedef void(NTAPI* PIO_APC_ROUTINE) (IN void* ApcContext, IN IOStatusBlock* IoStatusBlock, IN unsigned long Reserved);

typedef NTSTATUS(WINAPI* pNtCreateToken)(::HANDLE*, ::ACCESS_MASK, ObjectAttributes*, ::TOKEN_TYPE,
	::LUID*, ::LARGE_INTEGER*, ::TOKEN_USER*, ::TOKEN_GROUPS*, ::TOKEN_PRIVILEGES*,
	::TOKEN_OWNER*, ::TOKEN_PRIMARY_GROUP*, ::TOKEN_DEFAULT_DACL*, ::TOKEN_SOURCE*);
typedef NTSTATUS(WINAPI* pNtQueryInformationToken)(::HANDLE*, TOKEN_INFORMATION_CLASS, void*, unsigned long,
	unsigned long*);
typedef NTSTATUS(WINAPI* pNtSetInformationThread)(::HANDLE*, ::THREAD_INFORMATION_CLASS, void*, unsigned long);
typedef NTSTATUS(WINAPI* pNtDuplicateToken)(::HANDLE, ::ACCESS_MASK, ObjectAttributes*,
	::SECURITY_IMPERSONATION_LEVEL, ::TOKEN_TYPE, ::HANDLE*);
typedef NTSTATUS(WINAPI* pNtAdjustPrivilegesToken)(::HANDLE, int, ::TOKEN_PRIVILEGES*, unsigned long,
	::TOKEN_PRIVILEGES*, unsigned long*);
typedef NTSTATUS(WINAPI* pNtCreateFile)(::HANDLE*, ACCESS_MASK, ObjectAttributes*, IOStatusBlock*,
	::LARGE_INTEGER*, unsigned long, unsigned long, unsigned long, unsigned long, void*, unsigned long);
typedef NTSTATUS(WINAPI* pRtlInitUnicodeString)(UnicodeString*, wchar_t*);
typedef NTSTATUS(WINAPI* pNtCreateEvent)(::HANDLE*, ACCESS_MASK, ObjectAttributes*, EvtType, unsigned char);
typedef NTSTATUS(WINAPI* pNtQueryDirectoryFile)(::HANDLE, ::HANDLE, PIO_APC_ROUTINE, void*, IOStatusBlock*, void*,
	unsigned long, FolderInfo, unsigned char, UnicodeString*, unsigned char);
typedef NTSTATUS(WINAPI* pNtWaitForSingleobject)(::HANDLE, unsigned char, LARGE_INTEGER*);
typedef NTSTATUS(WINAPI* pRtlUnicodeStringToAnsiString)(ANSIString*, UnicodeString*, unsigned char);
typedef NTSTATUS(WINAPI* pNtClearEvent)(::HANDLE);
typedef NTSTATUS(WINAPI* pNtClose)(::HANDLE);

#endif