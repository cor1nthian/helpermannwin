#ifndef _ACL_HELPER_H
#define _ACL_HELPER_H

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
#include "strhelper.h"
#include "syshelper.h"

#define PERMISSION std::pair<std::pair<std::wstring, std::wstring>, AceType>
#define PERMISSIONFULL std::pair<std::pair<std::wstring, std::wstring>, std::pair<AceType, unsigned long>>

enum class AceType : unsigned char {
	// same as ACCESS_MIN_MS_ACE_TYPE
	AccessAllowed = ACCESS_ALLOWED_ACE_TYPE,
	AccessDenied = ACCESS_DENIED_ACE_TYPE,
	SystemAudit = SYSTEM_AUDIT_ACE_TYPE,
	// same as ACCESS_MAX_MS_V2_ACE_TYPE
	SystemAlarm = SYSTEM_ALARM_ACE_TYPE
};

enum class SecInfo : unsigned long {
	/* The resource properties of the object being referenced. The resource properties are
	stored in SYSTEM_RESOURCE_ATTRIBUTE_ACE types in the SACL of the security descriptor.
	Windows Server 2008 R2, Windows 7, Windows Server 2008, Windows Vista,
	Windows Server 2003 and Windows XP: This bit flag is not available. */
	AttributeSecInfo = ATTRIBUTE_SECURITY_INFORMATION,
	/* All parts of the security descriptor. This is useful for backup and restore
	software that needs to preserve the entire security descriptor.
	Windows Server 2008 R2, Windows 7, Windows Server 2008, Windows Vista,
	Windows Server 2003 and Windows XP: This bit flag is not available. */
	BackupSecInfo = BACKUP_SECURITY_INFORMATION,
	// The DACL of the object is being referenced.
	DACLSecInfo = DACL_SECURITY_INFORMATION,
	// The primary group identifier of the object is being referenced.
	GroupSecInfo = GROUP_SECURITY_INFORMATION,
	/* The mandatory integrity label is being referenced. The mandatory integrity label is an
	ACE in the SACL of the object. Windows Server 2003 and Windows XP: This bit flag is not available. */
	LabelSecInfo = LABEL_SECURITY_INFORMATION,
	// The owner identifier of the object is being referenced.
	OwnerSecInfo = OWNER_SECURITY_INFORMATION,
	// The DACL cannot inherit access control entries(ACEs).
	ProtectedDACLSecInfo = PROTECTED_DACL_SECURITY_INFORMATION,
	// The SACL cannot inherit ACEs.
	ProtectedSACLSecInfo = PROTECTED_SACL_SECURITY_INFORMATION,
	// The SACL of the object is being referenced.
	SACLSecInfo = SACL_SECURITY_INFORMATION,
	/* The Central Access Policy (CAP) identifier applicable on the object that is being
	referenced. Each CAP identifier is stored in a SYSTEM_SCOPED_POLICY_ID_ACE type in the SACL of the SD.
	Windows Server 2008 R2, Windows 7, Windows Server 2008, Windows Vista,
	Windows Server 2003 and Windows XP: This bit flag is not available. */
	ScopeSecInfo = SCOPE_SECURITY_INFORMATION,
	// The DACL inherits ACEs from the parent object.
	UnprotectedDACLSecInfo = UNPROTECTED_DACL_SECURITY_INFORMATION,
	// The SACL inherits ACEs from the parent object.
	UnprotectedSACLSecInfo = UNPROTECTED_SACL_SECURITY_INFORMATION
};

enum class ACLOpResult : unsigned char {
	Success,
	Fail
};

struct SecDesc {
	SecDesc();
	SecDesc(const SecDesc &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	SecDesc(SecDesc &&other) noexcept;
#endif
	~SecDesc();
	SecDesc& operator=(const SecDesc &other);
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
	SecDesc& operator=(SecDesc &&other) noexcept;
#endif
	bool operator==(const SecDesc &other) const;
	bool operator!=(const SecDesc &other) const;
	unsigned long daclInfoSz;
	unsigned long daclAbsInfoSz;
	unsigned long saclInfoSz;
	unsigned long saclAbsInfoSz;
	unsigned long ownerInfoSz;
	unsigned long primaryGroupInfoSz;
	unsigned long absoluteSDInfoSz;
	unsigned long selfRelativeSDInfoSz;
	void* daclInfo;
	void* daclAbsInfo;
	void* saclInfo;
	void* saclAbsInfo;
	void* absoluteSDInfo;
	void* selfRelativeSDInfo;
	std::wstring ownerInfo;
	std::wstring primaryGroupInfo;
};

class ACLHandler {
	public:
		ACLHandler();
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		ACLHandler(const ACLHandler &other) = delete;
#else
		ACLHandler(const ACLHandler &other) {}
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		ACLHandler(ACLHandler &&other) noexcept = delete;
#endif
		~ACLHandler();
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		ACLHandler& operator=(const ACLHandler &other) = delete;
#else
		ACLHandler& operator=(const ACLHandler &other) {}
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		ACLHandler& operator=(ACLHandler &&other) noexcept = delete;
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		bool operator==(const ACLHandler &other) const = delete;
#else
		bool operator==(const ACLHandler &other) {}
#endif
#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
		bool operator!=(const ACLHandler &other) const = delete;
#else
		bool operator!=(const ACLHandler &other) {}
#endif
		ACLOpResult SecurityDescriptor2StringSecurityDescriptor(unsigned char* &secDesc,
			std::wstring &textSecDesc, SecInfo secInfo = SecInfo::DACLSecInfo) const;
		ACLOpResult StringSecurityDescriptor2SecurityDescriptor(const std::wstring textSecDesc,
			unsigned char* &secDesc, unsigned long &secDeszSz) const;
		ACLOpResult DACLReadAllowed(bool &allowed, const ::ACL* testACL, const ::PSID sid) const;
		ACLOpResult DACLWriteAllowed(bool &allowed, const ::ACL* testACL, const ::PSID sid) const;
		ACLOpResult DACLExecuteAllowed(bool &allowed, const ::ACL* testACL, const ::PSID sid) const;
		ACLOpResult DACLDeleteAllowed(bool &allowed, const ::ACL* testACL, const ::PSID sid) const;
		ACLOpResult DACLFullControlAllowed(bool &allowed, const ::ACL* testACL, const ::PSID sid) const;
		ACLOpResult DACLAddCustomAllowedACE(::ACL* &dacl, const ::PSID sid, const unsigned long aclMask,
			const unsigned char aceFlags, const bool removeExistingBan = true) const;
		ACLOpResult DACLAddCustomDeniedACE(::ACL* &dacl, const ::PSID sid, unsigned long aclMask,
			const unsigned char aceFlags) const;
		ACLOpResult DACLAddCustomACE(::ACL* &dacl, const ::PSID sid, const AceType aceTyoe, unsigned long aclMask,
			const unsigned char aceFlags, const bool removeExistingBan = true) const;
		ACLOpResult DACLAddFullControlAllowed(::ACL* &dacl, const ::PSID sid, const bool removeExistingBan = true) const;
		ACLOpResult DACLAddDeleteAllowed(::ACL* &dacl, const ::PSID sid, const bool removeExistingBan = true) const;
		ACLOpResult DACLAddExecuteAllowed(::ACL*& dacl, const ::PSID sid, const bool removeExistingBan = true) const;
		ACLOpResult DACLAddWriteAllowed(::ACL* &dacl, const ::PSID sid, const bool removeExistingBan = true) const;
		ACLOpResult DACLAddReadAllowed(::ACL* &dacl, const ::PSID sid, const bool removeExistingBan = true) const;
		ACLOpResult DACLAddReadDenied(::ACL* &dacl, const ::PSID sid) const;
		ACLOpResult DACLAddWriteDenied(::ACL* &dacl, const ::PSID sid) const;
		ACLOpResult DACLAddExecuteDenied(::ACL* &dacl, const ::PSID sid) const;
		ACLOpResult DACLAddDeleteDenied(::ACL* &dacl, const ::PSID sid) const;
		ACLOpResult DACLAddFullControlDenied(::ACL* &dacl, const ::PSID sid) const;
		ACLOpResult DACLRegAddFullControlAllowed(::ACL* &dacl, const ::PSID sid,
			const bool removeExistingBan = true) const;
		ACLOpResult DACLRegAddQueryValueAllowed(::ACL* &dacl, const ::PSID sid,
			const bool removeExistingBan = true) const;
		ACLOpResult DACLRegAddSetValueAllowed(::ACL* &dacl, const ::PSID sid,
			const bool removeExistingBan = true) const;
		ACLOpResult DACLRegAddCreateSubkeyAllowed(::ACL* &dacl, const ::PSID sid,
			const bool removeExistingBan = true) const;
		ACLOpResult DACLRegAddEnumSubkeyAllowed(::ACL* &dacl, const ::PSID sid,
			const bool removeExistingBan = true) const;
		ACLOpResult DACLRegAddNotifyAllowed(::ACL* &dacl, const ::PSID sid,
			const bool removeExistingBan = true) const;
		ACLOpResult DACLRegAddCreateLinkAllowed(::ACL* &dacl, const ::PSID sid,
			const bool removeExistingBan = true) const;
		ACLOpResult DACLRegAddCreateLinkDenied(::ACL* &dacl, const ::PSID sid) const;
		ACLOpResult DACLRegAddNotifyDenied(::ACL* &dacl, const ::PSID sid) const;
		ACLOpResult DACLRegAddEnumSubkeyDenied(::ACL* &dacl, const ::PSID sid) const;
		ACLOpResult DACLRegAddCreateSubkeyDenied(::ACL* &dacl, const ::PSID sid) const;
		ACLOpResult DACLRegAddSetValueDenied(::ACL* &dacl, const ::PSID sid) const;
		ACLOpResult DACLRegAddQueryValueDenied(::ACL* &dacl, const ::PSID sid) const;
		ACLOpResult DACLRegAddFullControlDenied(::ACL* &dacl, const ::PSID sid) const;
		ACLOpResult DACLRemoveACESIDTypeMaskFlags(::ACL* &dacl, const ::PSID sid, const AceType aceType,
			const unsigned long aclMask, const unsigned char aclFlags, const bool includeGroups = true) const;
		ACLOpResult DACLRemoveACESIDTypeMask(::ACL* &dacl, const ::PSID sid, const AceType aceType,
			const unsigned long aclMask, const bool includeGroups = true) const;
		ACLOpResult DACLRemoveACESIDType(::ACL* &dacl, const ::PSID sid, const AceType aceType,
			const bool includeGroups = true) const;
		ACLOpResult DACLRemoveACESID(::ACL* &dacl, const ::PSID sid, const bool includeGroups = true) const;
		ACLOpResult DACLGetSIDsByAceType(::ACL* dacl, AceType reqAceType, std::vector<PERMISSION> &permissions,
			const std::wstring machineName = L".") const;
		ACLOpResult DACLGetPermissionBySID(::ACL* dacl, ::PSID sid, std::vector<PERMISSION> &permissions,
			const bool includeGroups = true, const std::wstring machineName = L".") const;
		ACLOpResult DACLGetPermissionMaskBySID(::ACL* dacl, ::PSID sid, std::vector<PERMISSIONFULL> &permissions,
			const bool includeGroups = true, const std::wstring machineName = L".") const;
		ACLOpResult DACLFromSecurityDescriptor(::SECURITY_DESCRIPTOR* secDesc, ::ACL* &dacl) const;
		ACLOpResult SACLFromSecurityDescriptor(::SECURITY_DESCRIPTOR* secDesc, ::ACL* &sacl) const;
		ACLOpResult OwnerSIDFromSecurityDescriptor(::SECURITY_DESCRIPTOR* secDesc, ::PSID &sid) const;
		ACLOpResult PrimaryGroupSIDFromSecurityDescriptor(::SECURITY_DESCRIPTOR* secDesc, ::PSID &sid) const;
		ACLOpResult CreateAbsoluteSecDesc(SecDesc &secDesc) const;
		ACLOpResult DACL2AbsoluteSD(::SECURITY_DESCRIPTOR* secDesc, ::ACL* dacl) const;
		ACLOpResult SACL2AbsoluteSD(::SECURITY_DESCRIPTOR* secDesc, ::ACL* sacl) const;
	protected:
		
	private:
		ACLOpResult BuildACE(::ACE_HEADER* &ace, const ::PSID sid, const AceType aceType,
			const ACCESS_MASK accessMask, const unsigned char aceFlags) const;
		ACLOpResult DACLDenyPermissionSetter(::ACL* &dacl, const ::PSID sid, const unsigned long aclMask,
			const unsigned char aclFlags = CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE) const;
		ACLOpResult DACLAllowPermissionSetter(::ACL* &dacl, const ::PSID sid, const bool removeExistingBan,
			const unsigned long aclMask, const unsigned char aclFlags = CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE) const;
		ACLOpResult DACLPermissionGetter(bool &allowed, const ::ACL* testACL, const ::PSID sid, const unsigned long mask,
			const bool checkGroups = true) const;
};

#endif // _ACL_HELPER_H