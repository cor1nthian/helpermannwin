//#if defined(_WIN64)
//	#define _AMD64_
//#else
//	#define _X86_
//#endif

#include <iostream>
#include "syshelper.h"
#include "reghelper.h"
#include "prochelper.h"
#include "fshelper.h"
#include "ntapi.h"
// #include <ntdef.h>

bool IsBadReadPtr(void* p) {
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if (::VirtualQuery(p, &mbi, sizeof(mbi))) {
		unsigned long mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
		bool b = !(mbi.Protect & mask);
		// check the page is not a guard page
		if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) {
			b = true;
		}
		return b;
	}
	return true;
}

bool IsBadWritePtr(void* p) {
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if (::VirtualQuery(p, &mbi, sizeof(mbi))) {
		unsigned long mask = (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
		bool b = !(mbi.Protect & mask);
		// check the page is not a guard page
		if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) {
			b = true;
		}
		return b;
	}
	return true;
}

long long rnd(const long long from, const long long to) {
	std::mt19937 rng;
	rng.seed(std::random_device()());
	//rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<std::mt19937::result_type> dist(from, to);
	return dist(rng);
	// srand(time(0));
	// return rand() % (to - from + 1) + from;
}

bool IsSIDWellKnown(const std::wstring strsid) {
	std::vector<WKSid> wkSIDS = GetWellKnownStrSIDs();
	for (size_t i = 0; i < wkSIDS.size(); ++i) {
		if (wkSIDS[i].StrSID == strsid) {
			return true;
		}
	}
	return false;
}

bool IsSIDWellKnown(const ::PSID sid) {
	SysHandler sys;
	std::wstring strSID = sys.StrSIDFromSID(sid);
	if (strSID.length()) {
		std::vector<WKSid> wkSIDS = GetWellKnownStrSIDs();
		for (size_t i = 0; i < wkSIDS.size(); ++i) {
			if (wkSIDS[i].StrSID == strSID) {
				return true;
			}
		}
		return false;
	}
	return false;
}

std::vector<WKSid> GetWellKnownStrSIDs(::PSID domainSID) {
	std::vector<WKSid> ret;
	unsigned char wksids[] = {
		WELL_KNOWN_SID_TYPE::WinNullSid,
		WELL_KNOWN_SID_TYPE::WinWorldSid,
		WELL_KNOWN_SID_TYPE::WinLocalSid,
		WELL_KNOWN_SID_TYPE::WinCreatorOwnerSid,
		WELL_KNOWN_SID_TYPE::WinCreatorGroupSid,
		WELL_KNOWN_SID_TYPE::WinCreatorOwnerServerSid,
		WELL_KNOWN_SID_TYPE::WinCreatorGroupServerSid,
		WELL_KNOWN_SID_TYPE::WinNtAuthoritySid,
		WELL_KNOWN_SID_TYPE::WinDialupSid,
		WELL_KNOWN_SID_TYPE::WinNetworkSid,
		WELL_KNOWN_SID_TYPE::WinBatchSid,
		WELL_KNOWN_SID_TYPE::WinInteractiveSid,
		WELL_KNOWN_SID_TYPE::WinServiceSid,
		WELL_KNOWN_SID_TYPE::WinAnonymousSid,
		WELL_KNOWN_SID_TYPE::WinProxySid,
		WELL_KNOWN_SID_TYPE::WinEnterpriseControllersSid,
		WELL_KNOWN_SID_TYPE::WinSelfSid,
		WELL_KNOWN_SID_TYPE::WinAuthenticatedUserSid,
		WELL_KNOWN_SID_TYPE::WinRestrictedCodeSid,
		WELL_KNOWN_SID_TYPE::WinTerminalServerSid,
		WELL_KNOWN_SID_TYPE::WinRemoteLogonIdSid,
		WELL_KNOWN_SID_TYPE::WinLogonIdsSid,
		WELL_KNOWN_SID_TYPE::WinLocalSystemSid,
		WELL_KNOWN_SID_TYPE::WinLocalServiceSid,
		WELL_KNOWN_SID_TYPE::WinNetworkServiceSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinDomainSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinAdministratorsSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinUsersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinGuestsSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinPowerUsersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinAccountOperatorsSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinSystemOperatorsSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinPrintOperatorsSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinBackupOperatorsSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinReplicatorSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinPreWindows2000CompatibleAccessSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinRemoteDesktopUsersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinNetworkConfigurationOperatorsSid,
		WELL_KNOWN_SID_TYPE::WinAccountAdministratorSid,
		WELL_KNOWN_SID_TYPE::WinAccountGuestSid,
		WELL_KNOWN_SID_TYPE::WinAccountKrbtgtSid,
		WELL_KNOWN_SID_TYPE::WinAccountDomainAdminsSid,
		WELL_KNOWN_SID_TYPE::WinAccountDomainUsersSid,
		WELL_KNOWN_SID_TYPE::WinAccountDomainGuestsSid,
		WELL_KNOWN_SID_TYPE::WinAccountComputersSid,
		WELL_KNOWN_SID_TYPE::WinAccountControllersSid,
		WELL_KNOWN_SID_TYPE::WinAccountCertAdminsSid,
		WELL_KNOWN_SID_TYPE::WinAccountSchemaAdminsSid,
		WELL_KNOWN_SID_TYPE::WinAccountEnterpriseAdminsSid,
		WELL_KNOWN_SID_TYPE::WinAccountPolicyAdminsSid,
		WELL_KNOWN_SID_TYPE::WinAccountRasAndIasServersSid,
		WELL_KNOWN_SID_TYPE::WinNTLMAuthenticationSid,
		WELL_KNOWN_SID_TYPE::WinDigestAuthenticationSid,
		WELL_KNOWN_SID_TYPE::WinSChannelAuthenticationSid,
		WELL_KNOWN_SID_TYPE::WinThisOrganizationSid,
		WELL_KNOWN_SID_TYPE::WinOtherOrganizationSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinIncomingForestTrustBuildersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinPerfMonitoringUsersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinPerfLoggingUsersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinAuthorizationAccessSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinTerminalServerLicenseServersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinDCOMUsersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinIUsersSid,
		WELL_KNOWN_SID_TYPE::WinIUserSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinCryptoOperatorsSid,
		WELL_KNOWN_SID_TYPE::WinUntrustedLabelSid,
		WELL_KNOWN_SID_TYPE::WinLowLabelSid,
		WELL_KNOWN_SID_TYPE::WinMediumLabelSid,
		WELL_KNOWN_SID_TYPE::WinHighLabelSid,
		WELL_KNOWN_SID_TYPE::WinSystemLabelSid,
		WELL_KNOWN_SID_TYPE::WinWriteRestrictedCodeSid,
		WELL_KNOWN_SID_TYPE::WinCreatorOwnerRightsSid,
		WELL_KNOWN_SID_TYPE::WinCacheablePrincipalsGroupSid,
		WELL_KNOWN_SID_TYPE::WinNonCacheablePrincipalsGroupSid,
		WELL_KNOWN_SID_TYPE::WinEnterpriseReadonlyControllersSid,
		WELL_KNOWN_SID_TYPE::WinAccountReadonlyControllersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinEventLogReadersGroup,
		WELL_KNOWN_SID_TYPE::WinNewEnterpriseReadonlyControllersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinCertSvcDComAccessGroup,
		WELL_KNOWN_SID_TYPE::WinMediumPlusLabelSid,
		WELL_KNOWN_SID_TYPE::WinLocalLogonSid,
		WELL_KNOWN_SID_TYPE::WinConsoleLogonSid,
		WELL_KNOWN_SID_TYPE::WinThisOrganizationCertificateSid,
		WELL_KNOWN_SID_TYPE::WinApplicationPackageAuthoritySid,
		WELL_KNOWN_SID_TYPE::WinBuiltinAnyPackageSid,
		WELL_KNOWN_SID_TYPE::WinCapabilityInternetClientSid,
		WELL_KNOWN_SID_TYPE::WinCapabilityInternetClientServerSid,
		WELL_KNOWN_SID_TYPE::WinCapabilityPrivateNetworkClientServerSid,
		WELL_KNOWN_SID_TYPE::WinCapabilityPicturesLibrarySid,
		WELL_KNOWN_SID_TYPE::WinCapabilityVideosLibrarySid,
		WELL_KNOWN_SID_TYPE::WinCapabilityMusicLibrarySid,
		WELL_KNOWN_SID_TYPE::WinCapabilityDocumentsLibrarySid,
		WELL_KNOWN_SID_TYPE::WinCapabilitySharedUserCertificatesSid,
		WELL_KNOWN_SID_TYPE::WinCapabilityEnterpriseAuthenticationSid,
		WELL_KNOWN_SID_TYPE::WinCapabilityRemovableStorageSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinRDSRemoteAccessServersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinRDSEndpointServersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinRDSManagementServersSid,
		WELL_KNOWN_SID_TYPE::WinUserModeDriversSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinHyperVAdminsSid,
		WELL_KNOWN_SID_TYPE::WinAccountCloneableControllersSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinAccessControlAssistanceOperatorsSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinRemoteManagementUsersSid,
		WELL_KNOWN_SID_TYPE::WinAuthenticationAuthorityAssertedSid,
		WELL_KNOWN_SID_TYPE::WinAuthenticationServiceAssertedSid,
		WELL_KNOWN_SID_TYPE::WinLocalAccountSid,
		WELL_KNOWN_SID_TYPE::WinLocalAccountAndAdministratorSid,
		WELL_KNOWN_SID_TYPE::WinAccountProtectedUsersSid,
		WELL_KNOWN_SID_TYPE::WinCapabilityAppointmentsSid,
		WELL_KNOWN_SID_TYPE::WinCapabilityContactsSid,
		WELL_KNOWN_SID_TYPE::WinAccountDefaultSystemManagedSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinDefaultSystemManagedGroupSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinStorageReplicaAdminsSid,
		WELL_KNOWN_SID_TYPE::WinAccountKeyAdminsSid,
		WELL_KNOWN_SID_TYPE::WinAccountEnterpriseKeyAdminsSid,
		WELL_KNOWN_SID_TYPE::WinAuthenticationKeyTrustSid,
		WELL_KNOWN_SID_TYPE::WinAuthenticationKeyPropertyMFASid,
		WELL_KNOWN_SID_TYPE::WinAuthenticationKeyPropertyAttestationSid,
		WELL_KNOWN_SID_TYPE::WinAuthenticationFreshKeyAuthSid,
		WELL_KNOWN_SID_TYPE::WinBuiltinDeviceOwnersSid
	};
	const wchar_t* wksidnames[] = {
		L"WinNullSid",
		L"WinWorldSid",
		L"WinLocalSid",
		L"WinCreatorOwnerSid",
		L"WinCreatorGroupSid",
		L"WinCreatorOwnerServerSid",
		L"WinCreatorGroupServerSid",
		L"WinNtAuthoritySid",
		L"WinDialupSid",
		L"WinNetworkSid",
		L"WinBatchSid",
		L"WinInteractiveSid",
		L"WinServiceSid",
		L"WinAnonymousSid",
		L"WinProxySid",
		L"WinEnterpriseControllersSid",
		L"WinSelfSid",
		L"WinAuthenticatedUserSid",
		L"WinRestrictedCodeSid",
		L"WinTerminalServerSid",
		L"WinRemoteLogonIdSid",
		L"WinLogonIdsSid",
		L"WinLocalSystemSid",
		L"WinLocalServiceSid",
		L"WinNetworkServiceSid",
		L"WinBuiltinDomainSid",
		L"WinBuiltinAdministratorsSid",
		L"WinBuiltinUsersSid",
		L"WinBuiltinGuestsSid",
		L"WinBuiltinPowerUsersSid",
		L"WinBuiltinAccountOperatorsSid",
		L"WinBuiltinSystemOperatorsSid",
		L"WinBuiltinPrintOperatorsSid",
		L"WinBuiltinBackupOperatorsSid",
		L"WinBuiltinReplicatorSid",
		L"WinBuiltinPreWindows2000CompatibleAccessSid",
		L"WinBuiltinRemoteDesktopUsersSid",
		L"WinBuiltinNetworkConfigurationOperatorsSid",
		L"WinAccountAdministratorSid",
		L"WinAccountGuestSid",
		L"WinAccountKrbtgtSid",
		L"WinAccountDomainAdminsSid",
		L"WinAccountDomainUsersSid",
		L"WinAccountDomainGuestsSid",
		L"WinAccountComputersSid",
		L"WinAccountControllersSid",
		L"WinAccountCertAdminsSid",
		L"WinAccountSchemaAdminsSid",
		L"WinAccountEnterpriseAdminsSid",
		L"WinAccountPolicyAdminsSid",
		L"WinAccountRasAndIasServersSid",
		L"WinNTLMAuthenticationSid",
		L"WinDigestAuthenticationSid",
		L"WinSChannelAuthenticationSid",
		L"WinThisOrganizationSid",
		L"WinOtherOrganizationSid",
		L"WinBuiltinIncomingForestTrustBuildersSid",
		L"WinBuiltinPerfMonitoringUsersSid",
		L"WinBuiltinPerfLoggingUsersSid",
		L"WinBuiltinAuthorizationAccessSid",
		L"WinBuiltinTerminalServerLicenseServersSid",
		L"WinBuiltinDCOMUsersSid",
		L"WinBuiltinIUsersSid",
		L"WinIUserSid",
		L"WinBuiltinCryptoOperatorsSid",
		L"WinUntrustedLabelSid",
		L"WinLowLabelSid",
		L"WinMediumLabelSid",
		L"WinHighLabelSid",
		L"WinSystemLabelSid",
		L"WinWriteRestrictedCodeSid",
		L"WinCreatorOwnerRightsSid",
		L"WinCacheablePrincipalsGroupSid",
		L"WinNonCacheablePrincipalsGroupSid",
		L"WinEnterpriseReadonlyControllersSid",
		L"WinAccountReadonlyControllersSid",
		L"WinBuiltinEventLogReadersGroup",
		L"WinNewEnterpriseReadonlyControllersSid",
		L"WinBuiltinCertSvcDComAccessGroup",
		L"WinMediumPlusLabelSid",
		L"WinLocalLogonSid",
		L"WinConsoleLogonSid",
		L"WinThisOrganizationCertificateSid",
		L"WinApplicationPackageAuthoritySid",
		L"WinBuiltinAnyPackageSid",
		L"WinCapabilityInternetClientSid",
		L"WinCapabilityInternetClientServerSid",
		L"WinCapabilityPrivateNetworkClientServerSid",
		L"WinCapabilityPicturesLibrarySid",
		L"WinCapabilityVideosLibrarySid",
		L"WinCapabilityMusicLibrarySid",
		L"WinCapabilityDocumentsLibrarySid",
		L"WinCapabilitySharedUserCertificatesSid",
		L"WinCapabilityEnterpriseAuthenticationSid",
		L"WinCapabilityRemovableStorageSid",
		L"WinBuiltinRDSRemoteAccessServersSid",
		L"WinBuiltinRDSEndpointServersSid",
		L"WinBuiltinRDSManagementServersSid",
		L"WinUserModeDriversSid",
		L"WinBuiltinHyperVAdminsSid",
		L"WinAccountCloneableControllersSid",
		L"WinBuiltinAccessControlAssistanceOperatorsSid",
		L"WinBuiltinRemoteManagementUsersSid",
		L"WinAuthenticationAuthorityAssertedSid",
		L"WinAuthenticationServiceAssertedSid",
		L"WinLocalAccountSid",
		L"WinLocalAccountAndAdministratorSid",
		L"WinAccountProtectedUsersSid",
		L"WinCapabilityAppointmentsSid",
		L"WinCapabilityContactsSid",
		L"WinAccountDefaultSystemManagedSid",
		L"WinBuiltinDefaultSystemManagedGroupSid",
		L"WinBuiltinStorageReplicaAdminsSid",
		L"WinAccountKeyAdminsSid",
		L"WinAccountEnterpriseKeyAdminsSid",
		L"WinAuthenticationKeyTrustSid",
		L"WinAuthenticationKeyPropertyMFASid",
		L"WinAuthenticationKeyPropertyAttestationSid",
		L"WinAuthenticationFreshKeyAuthSid",
		L"WinBuiltinDeviceOwnersSid"
	};
	SysHandler sys;
	::PSID tpsid = 0;
	unsigned long sidlen = SECURITY_MAX_SID_SIZE;
	for (size_t i = 0; i < 120; ++i) {
		sidlen = SECURITY_MAX_SID_SIZE;
		tpsid = malloc(sidlen);
		if (tpsid) {
			if (::CreateWellKnownSid((WELL_KNOWN_SID_TYPE)wksids[i], domainSID, tpsid, &sidlen)) {
				ret.emplace_back(sys.StrSIDFromSID(tpsid), sys.GetAccountNameFromSID(tpsid), wksidnames[i]);
			}
			SAFE_FREE(tpsid);
		} else {
			return ret;
		}
	}
	return ret;
}

WKSid::WKSid() {}

WKSid::WKSid(std::wstring strsid,
	std::wstring sidname, std::wstring sidnameorig) {
	StrSID = strsid;
	SIDName = sidname;
	SIDNameOrig = sidnameorig;
}

WKSid::WKSid(const WKSid& other) {
	StrSID = other.StrSID;
	SIDName = other.SIDName;
	SIDNameOrig = other.SIDNameOrig;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
WKSid::WKSid(WKSid &&other) noexcept {
	StrSID = std::move(other.StrSID);
	SIDName = std::move(other.SIDName);
	SIDNameOrig = std::move(other.SIDNameOrig);
}
#endif

WKSid::~WKSid() {}

WKSid& WKSid::operator=(const WKSid &other) {
	if (this != &other) {
		StrSID = other.StrSID;
		SIDName = other.SIDName;
		SIDNameOrig = other.SIDNameOrig;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
WKSid& WKSid::operator=(WKSid &&other) noexcept {
	if (this != &other) {
		StrSID = std::move(other.StrSID);
		SIDName = std::move(other.SIDName);
		SIDNameOrig = std::move(other.SIDNameOrig);
	}
	return *this;
}
#endif

bool WKSid::operator==(const WKSid& other) const {
	if (this != &other) {
		return (lower_copy(StrSID) == lower_copy(other.StrSID) &&
				lower_copy(SIDName) == lower_copy(other.SIDName) &&
				lower_copy(SIDNameOrig) == lower_copy(other.SIDNameOrig));
	} else {
		return true;
	}
}

bool WKSid::operator!=(const WKSid& other) const {
	if (this != &other) {
		return (lower_copy(StrSID) != lower_copy(other.StrSID) ||
				lower_copy(SIDName) != lower_copy(other.SIDName) ||
				lower_copy(SIDNameOrig) != lower_copy(other.SIDNameOrig));
	} else {
		return false;
	}
}

AccountDesc::AccountDesc() {
	ncNormalAcc = false;
	ncTempDupAcc = false;
	ncWorkstationTrustAcc = false;
	ncServerTrustAcc = false;
	ncInterdomainTrustAcc = false;
	accountLocked = false;
	accountDisabled = false;
	accountNotDelegated = false;
	accountDESKeysOnly = false;
	accountNoKerberosPreauth = false;
	accountTrustedToAuthForDelegation = false;
	accountTrustedForDelegation = false;
	logonScriptExecuted = false;
	passwordNotNeeded = false;
	passwordCantChange = false;
	passwordNoExpire = false;
	passwordReverseEncryption = false;
	passwordSmartCardNeeded = false;
	passwordExpired = false;
	passwordEncrypted = false;
	authPrintOp = false;
	authCommOp = false;
	authSrvOp = false;
	authAccOp = false;
	privileges = 0;
	passwordAge = 0;
	passwordBadCount = 0;
	lastLogOn = 0;
	lastLogOff = 0;
	expireDate = 0;
	maxStorage = 0;
	logonCount = 0;
	countryCode = 0;
	codePage = 0;
	primaryGroupId = 0;
}

AccountDesc::AccountDesc(const AccountDesc &other) {
	if (this != &other) {
		ncNormalAcc = other.ncNormalAcc;
		ncTempDupAcc = other.ncTempDupAcc;
		ncWorkstationTrustAcc = other.ncWorkstationTrustAcc;
		ncServerTrustAcc = other.ncServerTrustAcc;
		ncInterdomainTrustAcc = other.ncInterdomainTrustAcc;
		accountLocked = other.accountLocked;
		accountDisabled = other.accountDisabled;
		accountNotDelegated = other.accountNotDelegated;
		accountDESKeysOnly = other.accountDESKeysOnly;
		accountNoKerberosPreauth = other.accountNoKerberosPreauth;
		accountTrustedToAuthForDelegation = other.accountTrustedToAuthForDelegation;
		accountTrustedForDelegation = other.accountTrustedForDelegation;
		logonScriptExecuted = other.logonScriptExecuted;
		passwordNotNeeded = other.passwordNotNeeded;
		passwordCantChange = other.passwordCantChange;
		passwordNoExpire = other.passwordNoExpire;
		passwordReverseEncryption = other.passwordReverseEncryption;
		passwordSmartCardNeeded = other.passwordSmartCardNeeded;
		passwordExpired = other.passwordExpired;
		passwordEncrypted = other.passwordEncrypted;
		authPrintOp = other.authPrintOp;
		authCommOp = other.authCommOp;
		authSrvOp = other.authSrvOp;
		authAccOp = other.authAccOp;
		passwordAge = other.passwordAge;
		passwordBadCount = other.passwordBadCount;
		lastLogOn = other.lastLogOn;
		lastLogOff = other.lastLogOff;
		privileges = other.privileges;
		expireDate = other.expireDate;
		maxStorage = other.maxStorage;
		logonCount = other.logonCount;
		countryCode = other.countryCode;
		codePage = other.codePage;
		primaryGroupId = other.primaryGroupId;
		sid = other.sid;
		comment = other.comment;
		accountName = other.accountName;
		accountFullname = other.accountFullname;
		logonScriptPath = other.logonScriptPath;
		homeDirPath = other.homeDirPath;
		homeDirDrive = other.homeDirDrive;
		profileDirPath = other.profileDirPath;
		params = other.params;
		allowedWorkstations = other.allowedWorkstations;
		logonServer = other.logonServer;
		passwordSetStr = other.passwordSetStr;
		lastLogOnStr = other.lastLogOnStr;
		lastLogOffStr = other.lastLogOffStr;
		privilegesStr = other.privilegesStr;
		expireDateStr = other.expireDateStr;
		maxStorageStr = other.maxStorageStr;
		groups = other.groups;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
AccountDesc::AccountDesc(AccountDesc &&other) noexcept {
	if (this != &other) {
		ncNormalAcc = std::exchange(other.ncNormalAcc, false);
		ncTempDupAcc = std::exchange(other.ncTempDupAcc, false);
		ncWorkstationTrustAcc = std::exchange(other.ncWorkstationTrustAcc, false);
		ncServerTrustAcc = std::exchange(other.ncServerTrustAcc, false);
		ncInterdomainTrustAcc = std::exchange(other.ncInterdomainTrustAcc, false);
		accountLocked = std::exchange(other.accountLocked, false);
		accountDisabled = std::exchange(other.accountDisabled, false);
		accountNotDelegated = std::exchange(other.accountNotDelegated, false);
		accountDESKeysOnly = std::exchange(other.accountDESKeysOnly, false);
		accountNoKerberosPreauth = std::exchange(other.accountNoKerberosPreauth, false);
		accountTrustedToAuthForDelegation = std::exchange(other.accountTrustedToAuthForDelegation, false);
		accountTrustedForDelegation = std::exchange(other.accountTrustedForDelegation, false);
		logonScriptExecuted = std::exchange(other.logonScriptExecuted, false);
		passwordNotNeeded = std::exchange(other.passwordNotNeeded, false);
		passwordCantChange = std::exchange(other.passwordCantChange, false);
		passwordNoExpire = std::exchange(other.passwordNoExpire, false);
		passwordReverseEncryption = std::exchange(other.passwordReverseEncryption, false);
		passwordSmartCardNeeded = std::exchange(other.passwordSmartCardNeeded, false);
		passwordExpired = std::exchange(other.passwordExpired, false);
		passwordEncrypted = std::exchange(other.passwordEncrypted, false);
		authPrintOp = std::exchange(other.authPrintOp, false);
		authCommOp = std::exchange(other.authCommOp, false);
		authSrvOp = std::exchange(other.authSrvOp, false);
		authAccOp = std::exchange(other.authAccOp, false);
		passwordAge = std::exchange(other.authAccOp, 0);
		passwordBadCount = std::exchange(other.passwordBadCount, 0);
		lastLogOn = std::exchange(other.lastLogOn, 0);
		lastLogOff = std::exchange(other.lastLogOff, 0);
		privileges = std::exchange(other.privileges, 0);
		expireDate = std::exchange(other.expireDate, 0);
		maxStorage = std::exchange(other.maxStorage, 0);
		logonCount = std::exchange(other.logonCount, 0);
		countryCode = std::exchange(other.countryCode, 0);
		codePage = std::exchange(other.codePage, 0);
		primaryGroupId = std::exchange(other.primaryGroupId, 0);
		sid = std::move(other.sid);
		comment = std::move(other.comment);
		accountName = std::move(other.accountName);
		accountFullname = std::move(other.accountFullname);
		logonScriptPath = std::move(other.logonScriptPath);
		homeDirPath = std::move(other.homeDirPath);
		homeDirDrive = std::move(other.homeDirDrive);
		profileDirPath = std::move(other.profileDirPath);
		params = std::move(other.params);
		allowedWorkstations = std::move(other.allowedWorkstations);
		logonServer = std::move(other.logonServer);
		passwordSetStr = std::move(other.passwordSetStr);
		lastLogOnStr = std::move(other.lastLogOnStr);
		lastLogOffStr = std::move(other.lastLogOffStr);
		privilegesStr = std::move(other.privilegesStr);
		expireDateStr = std::move(other.expireDateStr);
		maxStorageStr = std::move(other.maxStorageStr);
		groups = std::move(other.groups);
	}
}
#endif

AccountDesc::~AccountDesc() {}

AccountDesc& AccountDesc::operator=(const AccountDesc &other) {
	if (this != &other) {
		ncNormalAcc = other.ncNormalAcc;
		ncTempDupAcc = other.ncTempDupAcc;
		ncWorkstationTrustAcc = other.ncWorkstationTrustAcc;
		ncServerTrustAcc = other.ncServerTrustAcc;
		ncInterdomainTrustAcc = other.ncInterdomainTrustAcc;
		accountLocked = other.accountLocked;
		accountDisabled = other.accountDisabled;
		accountNotDelegated = other.accountNotDelegated;
		accountDESKeysOnly = other.accountDESKeysOnly;
		accountNoKerberosPreauth = other.accountNoKerberosPreauth;
		accountTrustedToAuthForDelegation = other.accountTrustedToAuthForDelegation;
		accountTrustedForDelegation = other.accountTrustedForDelegation;
		logonScriptExecuted = other.logonScriptExecuted;
		passwordNotNeeded = other.passwordNotNeeded;
		passwordCantChange = other.passwordCantChange;
		passwordNoExpire = other.passwordNoExpire;
		passwordReverseEncryption = other.passwordReverseEncryption;
		passwordSmartCardNeeded = other.passwordSmartCardNeeded;
		passwordExpired = other.passwordExpired;
		passwordEncrypted = other.passwordEncrypted;
		authPrintOp = other.authPrintOp;
		authCommOp = other.authCommOp;
		authSrvOp = other.authSrvOp;
		authAccOp = other.authAccOp;
		passwordAge = other.passwordAge;
		passwordBadCount = other.passwordBadCount;
		lastLogOn = other.lastLogOn;
		lastLogOff = other.lastLogOff;
		privileges = other.privileges;
		expireDate = other.expireDate;
		maxStorage = other.maxStorage;
		logonCount = other.logonCount;
		countryCode = other.countryCode;
		codePage = other.codePage;
		primaryGroupId = other.primaryGroupId;
		sid = other.sid;
		comment = other.comment;
		accountName = other.accountName;
		accountFullname = other.accountFullname;
		logonScriptPath = other.logonScriptPath;
		homeDirPath = other.homeDirPath;
		homeDirDrive = other.homeDirDrive;
		profileDirPath = other.profileDirPath;
		params = other.params;
		allowedWorkstations = other.allowedWorkstations;
		logonServer = other.logonServer;
		passwordSetStr = other.passwordSetStr;
		lastLogOnStr = other.lastLogOnStr;
		lastLogOffStr = other.lastLogOffStr;
		privilegesStr = other.privilegesStr;
		expireDateStr = other.expireDateStr;
		maxStorageStr = other.maxStorageStr;
		groups = other.groups;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
AccountDesc& AccountDesc::operator=(AccountDesc &&other) noexcept {
	if (this != &other) {
		ncNormalAcc = std::exchange(other.ncNormalAcc, false);
		ncTempDupAcc = std::exchange(other.ncTempDupAcc, false);
		ncWorkstationTrustAcc = std::exchange(other.ncWorkstationTrustAcc, false);
		ncServerTrustAcc = std::exchange(other.ncServerTrustAcc, false);
		ncInterdomainTrustAcc = std::exchange(other.ncInterdomainTrustAcc, false);
		accountLocked = std::exchange(other.accountLocked, false);
		accountDisabled = std::exchange(other.accountDisabled, false);
		accountNotDelegated = std::exchange(other.accountNotDelegated, false);
		accountDESKeysOnly = std::exchange(other.accountDESKeysOnly, false);
		accountNoKerberosPreauth = std::exchange(other.accountNoKerberosPreauth, false);
		accountTrustedToAuthForDelegation = std::exchange(other.accountTrustedToAuthForDelegation, false);
		accountTrustedForDelegation = std::exchange(other.accountTrustedForDelegation, false);
		logonScriptExecuted = std::exchange(other.logonScriptExecuted, false);
		passwordNotNeeded = std::exchange(other.passwordNotNeeded, false);
		passwordCantChange = std::exchange(other.passwordCantChange, false);
		passwordNoExpire = std::exchange(other.passwordNoExpire, false);
		passwordReverseEncryption = std::exchange(other.passwordReverseEncryption, false);
		passwordSmartCardNeeded = std::exchange(other.passwordSmartCardNeeded, false);
		passwordExpired = std::exchange(other.passwordExpired, false);
		passwordEncrypted = std::exchange(other.passwordEncrypted, false);
		authPrintOp = std::exchange(other.authPrintOp, false);
		authCommOp = std::exchange(other.authCommOp, false);
		authSrvOp = std::exchange(other.authSrvOp, false);
		authAccOp = std::exchange(other.authAccOp, false);
		passwordAge = std::exchange(other.authAccOp, 0);
		passwordBadCount = std::exchange(other.passwordBadCount, 0);
		lastLogOn = std::exchange(other.lastLogOn, 0);
		lastLogOff = std::exchange(other.lastLogOff, 0);
		privileges = std::exchange(other.privileges, 0);
		expireDate = std::exchange(other.expireDate, 0);
		maxStorage = std::exchange(other.maxStorage, 0);
		logonCount = std::exchange(other.logonCount, 0);
		countryCode = std::exchange(other.countryCode, 0);
		codePage = std::exchange(other.codePage, 0);
		primaryGroupId = std::exchange(other.primaryGroupId, 0);
		sid = std::move(other.sid);
		comment = std::move(other.comment);
		accountName = std::move(other.accountName);
		accountFullname = std::move(other.accountFullname);
		logonScriptPath = std::move(other.logonScriptPath);
		homeDirPath = std::move(other.homeDirPath);
		homeDirDrive = std::move(other.homeDirDrive);
		profileDirPath = std::move(other.profileDirPath);
		params = std::move(other.params);
		allowedWorkstations = std::move(other.allowedWorkstations);
		logonServer = std::move(other.logonServer);
		passwordSetStr = std::move(other.passwordSetStr);
		lastLogOnStr = std::move(other.lastLogOnStr);
		lastLogOffStr = std::move(other.lastLogOffStr);
		privilegesStr = std::move(other.privilegesStr);
		expireDateStr = std::move(other.expireDateStr);
		maxStorageStr = std::move(other.maxStorageStr);
		groups = std::move(other.groups);
	}
	return *this;
}
#endif

bool AccountDesc::operator==(const AccountDesc &other) const {
	if (this != &other) {
		return (ncNormalAcc == other.ncNormalAcc &&
				ncTempDupAcc == other.ncTempDupAcc &&
				ncWorkstationTrustAcc == other.ncWorkstationTrustAcc &&
				ncServerTrustAcc == other.ncServerTrustAcc &&
				ncInterdomainTrustAcc == other.ncInterdomainTrustAcc &&
				accountLocked == other.accountLocked &&
				accountDisabled == other.accountDisabled &&
				accountNotDelegated == other.accountNotDelegated &&
				accountDESKeysOnly == other.accountDESKeysOnly &&
				accountNoKerberosPreauth == other.accountNoKerberosPreauth &&
				accountTrustedToAuthForDelegation == other.accountTrustedToAuthForDelegation &&
				accountTrustedForDelegation == other.accountTrustedForDelegation &&
				logonScriptExecuted == other.logonScriptExecuted &&
				passwordNotNeeded == other.passwordNotNeeded &&
				passwordCantChange == other.passwordCantChange &&
				passwordNoExpire == other.passwordNoExpire &&
				passwordReverseEncryption == other.passwordReverseEncryption &&
				passwordSmartCardNeeded == other.passwordSmartCardNeeded &&
				passwordExpired == other.passwordExpired &&
				authPrintOp == other.authPrintOp &&
				authCommOp == other.authCommOp &&
				authSrvOp == other.authSrvOp &&
				authAccOp == other.authAccOp &&
				passwordEncrypted == other.passwordEncrypted &&
				passwordAge == other.passwordAge &&
				passwordBadCount == other.passwordBadCount &&
				lastLogOn == other.lastLogOn &&
				lastLogOff == other.lastLogOff &&
				privileges == other.privileges &&
				expireDate == other.expireDate &&
				maxStorage == other.maxStorage &&
				logonCount == other.logonCount &&
				countryCode == other.countryCode &&
				codePage == other.codePage &&
				primaryGroupId == other.primaryGroupId &&
				groups == other.groups &&
				lower_copy(sid) == lower_copy(other.sid) &&
				lower_copy(comment) == lower_copy(other.comment) &&
				lower_copy(accountName) == lower_copy(other.accountName) &&
				lower_copy(accountFullname) == lower_copy(other.accountFullname) &&
				lower_copy(logonScriptPath) == lower_copy(other.logonScriptPath) &&
				lower_copy(homeDirPath) == lower_copy(other.homeDirPath) &&
				lower_copy(homeDirDrive) == lower_copy(other.homeDirDrive) &&
				lower_copy(profileDirPath) == lower_copy(other.profileDirPath) &&
				lower_copy(params) == lower_copy(other.params) &&
				lower_copy(allowedWorkstations) == lower_copy(other.allowedWorkstations) &&
				lower_copy(logonServer) == lower_copy(other.logonServer) &&
				lower_copy(passwordSetStr) == lower_copy(other.passwordSetStr) &&
				lower_copy(lastLogOnStr) == lower_copy(other.lastLogOnStr) &&
				lower_copy(lastLogOffStr) == lower_copy(other.lastLogOffStr) &&
				lower_copy(privilegesStr) == lower_copy(other.privilegesStr) &&
				lower_copy(expireDateStr) == lower_copy(other.expireDateStr) &&
				lower_copy(maxStorageStr) == lower_copy(other.maxStorageStr));
	} else {
		return true;
	}
}

bool AccountDesc::operator!=(const AccountDesc &other) const {
	if (this != &other) {
		return (ncNormalAcc != other.ncNormalAcc ||
				ncTempDupAcc != other.ncTempDupAcc ||
				ncWorkstationTrustAcc != other.ncWorkstationTrustAcc ||
				ncServerTrustAcc != other.ncServerTrustAcc ||
				ncInterdomainTrustAcc != other.ncInterdomainTrustAcc ||
				accountLocked != other.accountLocked ||
				accountDisabled != other.accountDisabled ||
				accountNotDelegated != other.accountNotDelegated ||
				accountDESKeysOnly != other.accountDESKeysOnly ||
				accountNoKerberosPreauth != other.accountNoKerberosPreauth ||
				accountTrustedToAuthForDelegation != other.accountTrustedToAuthForDelegation ||
				accountTrustedForDelegation != other.accountTrustedForDelegation ||
				logonScriptExecuted != other.logonScriptExecuted ||
				passwordNotNeeded != other.passwordNotNeeded ||
				passwordCantChange != other.passwordCantChange ||
				passwordNoExpire != other.passwordNoExpire ||
				passwordReverseEncryption != other.passwordReverseEncryption ||
				passwordSmartCardNeeded != other.passwordSmartCardNeeded ||
				passwordExpired != other.passwordExpired ||
				passwordEncrypted != other.passwordEncrypted ||
				authPrintOp != other.authPrintOp ||
				authCommOp != other.authCommOp ||
				authSrvOp != other.authSrvOp ||
				authAccOp != other.authAccOp ||
				passwordAge != other.passwordAge ||
				passwordBadCount != other.passwordBadCount ||
				lastLogOn != other.lastLogOn ||
				lastLogOff != other.lastLogOff ||
				privileges != other.privileges ||
				expireDate != other.expireDate ||
				maxStorage != other.maxStorage ||
				logonCount != other.logonCount ||
				countryCode != other.countryCode ||
				codePage != other.codePage ||
				primaryGroupId != other.primaryGroupId ||
				groups != other.groups ||
				lower_copy(sid) != lower_copy(other.sid) ||
				lower_copy(comment) != lower_copy(other.comment) ||
				lower_copy(accountName) != lower_copy(other.accountName) ||
				lower_copy(accountFullname) != lower_copy(other.accountFullname) ||
				lower_copy(logonScriptPath) != lower_copy(other.logonScriptPath) ||
				lower_copy(homeDirPath) != lower_copy(other.homeDirPath) ||
				lower_copy(homeDirDrive) != lower_copy(other.homeDirDrive) ||
				lower_copy(profileDirPath) != lower_copy(other.profileDirPath) ||
				lower_copy(params) != lower_copy(other.params) ||
				lower_copy(allowedWorkstations) != lower_copy(other.allowedWorkstations) ||
				lower_copy(logonServer) != lower_copy(other.logonServer) ||
				lower_copy(passwordSetStr) != lower_copy(other.passwordSetStr) ||
				lower_copy(lastLogOnStr) != lower_copy(other.lastLogOnStr) ||
				lower_copy(lastLogOffStr) != lower_copy(other.lastLogOffStr) ||
				lower_copy(privilegesStr) != lower_copy(other.privilegesStr) ||
				lower_copy(expireDateStr) != lower_copy(other.expireDateStr) ||
				lower_copy(maxStorageStr) != lower_copy(other.maxStorageStr));
	} else {
		return false;
	}
}

GroupDesc::GroupDesc() {
	IsADGroup = false;
}

GroupDesc::GroupDesc(const bool isADGroup, const std::wstring groupName, const std::wstring groupStrSid,
	const std::wstring comment, const std::vector<AccountDesc> accounts) {
	IsADGroup = isADGroup;
	GroupName = groupName;
	GroupStrSid = groupStrSid;
	Comment = comment;
	Accounts = accounts;
}

GroupDesc::GroupDesc(const GroupDesc &other) {
	if (this != &other) {
		IsADGroup = other.IsADGroup;
		GroupName = other.GroupName;
		GroupStrSid = other.GroupStrSid;
		Comment = other.Comment;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
GroupDesc::GroupDesc(GroupDesc &&other) noexcept {
	if (this != &other) {
		IsADGroup = std::exchange(other.IsADGroup, false);
		GroupName = std::move(other.GroupName);
		GroupStrSid = std::move(other.GroupStrSid);
		Comment = std::move(other.Comment);
	}
}
#endif

GroupDesc::~GroupDesc() {}

GroupDesc& GroupDesc::operator=(const GroupDesc &other) {
	if (this != &other) {
		IsADGroup = other.IsADGroup;
		GroupName = other.GroupName;
		GroupStrSid = other.GroupStrSid;
		Comment = other.Comment;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
GroupDesc& GroupDesc::operator=(GroupDesc &&other) noexcept {
	if (this != &other) {
		IsADGroup = std::exchange(other.IsADGroup, false);
		GroupName = std::move(other.GroupName);
		GroupStrSid = std::move(other.GroupStrSid);
		Comment = std::move(other.Comment);
	}
	return *this;
}
#endif

bool GroupDesc::operator==(const GroupDesc& other) const {
	if (this != &other) {
		return (lower_copy(GroupName) == lower_copy(other.GroupName) &&
				lower_copy(GroupName) == lower_copy(other.GroupName) &&
				lower_copy(Comment) == lower_copy(other.Comment) &&
				IsADGroup == other.IsADGroup &&
				Accounts == other.Accounts);
	} else {
		return true;
	}
}

bool GroupDesc::operator!=(const GroupDesc& other) const {
	if (this != &other) {
		return (lower_copy(GroupName) != lower_copy(other.GroupName) ||
				lower_copy(GroupName) != lower_copy(other.GroupName) ||
				lower_copy(Comment) != lower_copy(other.Comment) ||
				IsADGroup != other.IsADGroup ||
				Accounts != other.Accounts);
	} else {
		return false;
	}
}

SysHandler::SysHandler() {}

SysHandler::~SysHandler() {}

std::wstring SysHandler::GetMachineName() const {
	unsigned long nameLen = MAX_COMPUTERNAME_LENGTH * sizeof(wchar_t);
	wchar_t compName[(MAX_COMPUTERNAME_LENGTH * sizeof(wchar_t)) + sizeof(wchar_t)] = { 0 };
	if (::GetComputerName(compName, &nameLen)) {
		return compName;
	} else {
		return L"";
	}
}

SysArch SysHandler::GetMachineArch() const {
	unsigned int x64test = ::GetSystemWow64DirectoryA(0, 0);
	if (ERROR_CALL_NOT_IMPLEMENTED == getLastErrorCode()) {
		return SysArch::X32;
	} else {
		return SysArch::X64;
	}
}

SysOpResult SysHandler::GetResourceIDByName(unsigned long &resourceID, const std::wstring resourceName,
	const ::HMODULE hModule, const ResourceType resourceType) const {
	if (ResourceType::Any == resourceType) {
		std::vector<ResourceType> resTypes;
		resTypes.push_back(ResourceType::Accelerator);
		resTypes.push_back(ResourceType::AniIcon);
		resTypes.push_back(ResourceType::Bitmap);
		resTypes.push_back(ResourceType::Cursor);
		resTypes.push_back(ResourceType::Dialog);
		resTypes.push_back(ResourceType::DialogInculde);
		resTypes.push_back(ResourceType::Font);
		resTypes.push_back(ResourceType::FontDirectory);
		resTypes.push_back(ResourceType::GroupCursor);
		resTypes.push_back(ResourceType::GroupIcon);
		resTypes.push_back(ResourceType::HTML);
		resTypes.push_back(ResourceType::Icon);
		resTypes.push_back(ResourceType::Manifest);
		resTypes.push_back(ResourceType::Menu);
		resTypes.push_back(ResourceType::MessageTable);
		resTypes.push_back(ResourceType::PlugNPlay);
		resTypes.push_back(ResourceType::RCData);
		resTypes.push_back(ResourceType::StringTable);
		resTypes.push_back(ResourceType::Version);
		resTypes.push_back(ResourceType::VXD);
		for (size_t i = 0; i < resTypes.size(); ++i) {

		}
	} else {

	}
	return SysOpResult::Success;
}

SysOpResult SysHandler::GetResourceNameByID(std::wstring &recourceName, const unsigned long resourceID) const {
	wchar_t* resName = MAKEINTRESOURCE(resourceID);
	if (!IsBadReadPtr(resName)) {
		wchar_t* nameBuf = (wchar_t*)malloc(256 * sizeof(wchar_t));
		if (!nameBuf) {
			return SysOpResult::Fail;
		}
		wsprintf(nameBuf, L"%s", resName);
		recourceName = nameBuf;
		SAFE_FREE(nameBuf);
		return SysOpResult::Success;
	} else {
		recourceName = L"";
		return SysOpResult::Fail;
	}
}

bool SysHandler::IsWow64Proc () const {
	int isWow64 = 0;
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)::GetProcAddress(
		::GetModuleHandle(L"kernel32"), "IsWow64Process");
	if(fnIsWow64Process) {
		if (fnIsWow64Process(::GetCurrentProcess(), isWow64)) {
			if (isWow64) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
	return false;
}

bool SysHandler::IsWow64Proc(const unsigned long pid, const unsigned long desiredProcRights) const {
	int isWow64 = 0;
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)::GetProcAddress(
		::GetModuleHandle(L"kernel32"), "IsWow64Process");
	if (fnIsWow64Process) {
		::HANDLE hProc = ::OpenProcess(desiredProcRights, true, pid);
		if (hProc && INVALID_HANDLE_VALUE != hProc && hProc) {
			if (fnIsWow64Process(hProc, isWow64)) {
				if (isWow64) {
					::CloseHandle(hProc);
					return true;
				} else {
					::CloseHandle(hProc);
					return false;
				}
			} else {
				::CloseHandle(hProc);
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
	return false;
}

bool SysHandler::ExtractResource(const std::wstring extractPath, const unsigned long resId) const {
	::HRSRC hResource = 0;
	hResource = ::FindResource(0, MAKEINTRESOURCE(resId), RT_RCDATA);
	if (hResource) {
		::HGLOBAL hGlobal = ::LoadResource(0, hResource);
		if (hGlobal) {
			unsigned long exeSize = ::SizeofResource(0, hResource);
			if (exeSize) {
				void* exeBuf = ::LockResource(hGlobal);
				if (exeBuf) {
					std::ofstream outfile(extractPath.c_str(), std::ios::binary);
					if (outfile.is_open()) {
						int res = (outfile.write((char*)exeBuf, exeSize)) ? 0 : 7;
						outfile.close();
						if (!res) {
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

std::wstring SysHandler::GetStrSIDFromAccountName(const std::wstring accName, const std::wstring machineName,
	const bool isDomainAcc) const {
	unsigned long domainNameLen = 256, sidlen = 128;
	NEW_ARR_NULLIFY(buf, wchar_t, sidlen);
	NEW_ARR_NULLIFY(domainName, wchar_t, domainNameLen);
	// wchar_t buf[128] = { 0 };
	// wchar_t domainName[256] = { 0 };
	wchar_t* sidstr = { 0 };
	SID_NAME_USE sidType;
	/*std::wstring fullName = machineName.length() ?
		machineName + L"\\" + accName : L".\\" + accName;*/
	if (::LookupAccountName(machineName.c_str(), accName.c_str(), buf, &sidlen, domainName,
		&domainNameLen, &sidType)) {
		SID* sid = (SID*)buf;
		if (::ConvertSidToStringSid(sid, &sidstr)) {
			std::wstring ret = sidstr;
			SAFE_NetApiBufferFree(sidstr);
			SAFE_ARR_DELETE(buf);
			SAFE_ARR_DELETE(domainName);
			return ret;
		} else {
			return L"";
		}
	} else {
		return L"";
	}
}

::PSID SysHandler::GetSIDFromAccountName(const std::wstring accName,
	const std::wstring machineName, const bool isDomainAcc) const {
	// unsigned char* buf = (unsigned char*)malloc(128 * sizeof(unsigned char));
	NEW_ARR_NULLIFY(buf, unsigned char, 128);
	// unsigned char buf[128] = { 0 };
	wchar_t domainName[128] = { 0 };
	SID_NAME_USE sidType;
	unsigned long domainNameLen = 128, sidlen = 128;
	/*std::wstring fullName = machineName.length() ?
		machineName + L"\\" + accName : L".\\" + accName;*/
	if (::LookupAccountName(machineName.c_str(), accName.c_str(), (::PSID)buf, &sidlen, domainName,
		&domainNameLen, &sidType)) {
		return (::PSID)buf;
	} else {
		return 0;
	}
}

unsigned long long SysHandler::CurrentTime2Unixtime() const {
	time_t ltime;
	return time(&ltime);
}

long SysHandler::GetTZOffset(time_t when) const {
	if (when == NULL_TIME) {
		when = std::time(nullptr);
	}
	auto const tm = *std::localtime(&when);
	std::ostringstream os;
	os << std::put_time(&tm, "%z");
	std::string s = os.str();
	// s is in ISO 8601 format: "±HHMM"
	char h = std::stoi(s.substr(0, 3), nullptr, 10);
	char m = std::stoi(s[0] + s.substr(3), nullptr, 10);
	return ((3600 * h) + (60 * m));
}

std::wstring SysHandler::UnixTime2StrTime(const time_t& timestamp) const {
	enum { numCStringSize = 0x80 };
	const char strTimestampFmt[] = "%a, %Y-%b-%d, %H:%M:%S";
	char timeCStr[numCStringSize];
	std::tm* tmInfo = gmtime(&timestamp);
	std::strftime(&timeCStr[0], numCStringSize, strTimestampFmt, tmInfo);
	return str2wstr(std::string(timeCStr));
}

unsigned char SysHandler::GetCPUCoreNum() const {
	return (unsigned char)std::thread::hardware_concurrency();
}

unsigned char SysHandler::GetThreadNum() const {
	return (unsigned char)(std::thread::hardware_concurrency() - 1);
}

SysOpResult SysHandler::GetSIDType(const ::PSID sid, SidType &sidType, const std::wstring machineName, const std::wstring domainName) const {
	if (sid) {
		unsigned long nameLen = 256, domainNameLen = 256;
		NEW_ARR_NULLIFY(nameBuf, wchar_t, nameLen);
		if (nameBuf) {
			NEW_ARR_NULLIFY(domainNameBuf, wchar_t, domainNameLen);
			if (domainNameBuf) {
				swprintf(domainNameBuf, L"%s", domainName.c_str());
				SID_NAME_USE sidUse;
				if (::LookupAccountSid(machineName.c_str(), sid, nameBuf, &nameLen, domainNameBuf, &domainNameLen, &sidUse)) {
					SAFE_ARR_DELETE(nameBuf);
					SAFE_ARR_DELETE(domainNameBuf);
					sidType = (SidType)sidUse;
					return SysOpResult::Success;
				} else {
					SAFE_ARR_DELETE(nameBuf);
					SAFE_ARR_DELETE(domainNameBuf);
					sidType = SidType::SidTypeUnknown;
					return SysOpResult::Fail;
				}
			} else {
				SAFE_ARR_DELETE(nameBuf);
				return SysOpResult::Fail;
			}
		} else {
			return SysOpResult::Fail;
		}
	} else {
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::GetPhysRAM(unsigned long long &ramAmount) const {
	if (::GetPhysicallyInstalledSystemMemory(&ramAmount)) {
		return SysOpResult::Success;
	} else {
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::GetRam(unsigned long long &freeRam,
	unsigned long long &totalRam, unsigned char &precentInUse) const {
	MEMORYSTATUSEX statex;
	memset(&statex, 0, sizeof(MEMORYSTATUSEX));
	statex.dwLength = sizeof(statex);
	if (::GlobalMemoryStatusEx(&statex)) {
		freeRam = statex.ullAvailPhys;
		totalRam = statex.ullTotalPhys;
		precentInUse = statex.dwMemoryLoad;
		return SysOpResult::Success;
	} else {
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::GetPageFile(unsigned long long &freePageFile,
	unsigned long long &totalPageFile) const {
	MEMORYSTATUSEX statex;
	memset(&statex, 0, sizeof(MEMORYSTATUSEX));
	statex.dwLength = sizeof(statex);
	if (::GlobalMemoryStatusEx(&statex)) {
		freePageFile = statex.ullAvailPageFile;
		totalPageFile = statex.ullTotalPageFile;
		return SysOpResult::Success;
	} else {
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::GetVirtualMem(unsigned long long &freeVirtMem,
	unsigned long long &totalVirtMem) const {
	MEMORYSTATUSEX statex;
	memset(&statex, 0, sizeof(MEMORYSTATUSEX));
	statex.dwLength = sizeof(statex);
	if (::GlobalMemoryStatusEx(&statex)) {
		freeVirtMem = statex.ullAvailVirtual;
		totalVirtMem = statex.ullTotalVirtual;
		return SysOpResult::Success;
	} else {
		return SysOpResult::Fail;
	}
}

::PSID SysHandler::SIDFromStrSid(const std::wstring sidstr) const {
	if (sidstr.length()) {
		::PSID tsid = 0;
		if (::ConvertStringSidToSid(sidstr.c_str(), &tsid)) {
			return tsid;
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}

std::wstring SysHandler::StrSIDFromSID(const ::PSID sid) const {
	wchar_t* sbuf = 0;
	std::wstring ret;
	if (::ConvertSidToStringSid(sid, &sbuf)) {
		ret = sbuf;
		SAFE_LOCALFREE(sbuf);
		return ret;
	}
	return L"";
}

std::wstring SysHandler::GetAccountNameFromSID(const ::PSID sid, const std::wstring machineName) const {
	if (sid) {
		unsigned long nameLen = MAX_PATH, domainNameLen = MAX_PATH;
		SID_NAME_USE sidUse;
		NEW_ARR_NULLIFY(nameBuf, wchar_t, nameLen + 1);
		NEW_ARR_NULLIFY(domainNameBuf, wchar_t, domainNameLen + 1);
		wchar_t* machineNameBuf = 0;
		if (nameBuf && domainNameBuf) {
			if (L"." != machineName) {
				NEW_ARR_NULLIFY_NO_REDEFINE(machineNameBuf, wchar_t, 128);
				wsprintf(machineNameBuf, L"%s", machineName.c_str());
			}
			if (::LookupAccountSid(machineNameBuf, sid, nameBuf, &nameLen,
				domainNameBuf, &domainNameLen, &sidUse)) {
				std::wstring ret = nameBuf;
				SAFE_ARR_DELETE(nameBuf);
				SAFE_ARR_DELETE(domainNameBuf);
				if (machineNameBuf) {
					SAFE_ARR_DELETE(machineNameBuf);
				}
				return ret;
			} else {
				SAFE_ARR_DELETE(nameBuf);
				SAFE_ARR_DELETE(domainNameBuf);
				return L"";
			}
		}
		SAFE_ARR_DELETE(nameBuf);
		SAFE_ARR_DELETE(domainNameBuf);
		return L"";
	} else {
		return L"";
	}
}

std::wstring SysHandler::GetAccountNameFromStrSID(const std::wstring strSid,
	const std::wstring  machineName) const {
	std::wstring sid;
	if (strSid.length()) {
		if (startsWith(strSid, L"{")) {
			sid = removeFromStart_copy(strSid, L"{");
		}
		if (endsWith(strSid, L"}")) {
			sid = removeFromEnd_copy(strSid, L"}");
		}
		::PSID tpsid = SIDFromStrSid(strSid);
		unsigned long nameLen = MAX_PATH, domainNameLen = MAX_PATH;
		SID_NAME_USE sidUse;
		NEW_ARR_NULLIFY(nameBuf, wchar_t, nameLen + 1);
		NEW_ARR_NULLIFY(domainNameBuf, wchar_t, domainNameLen + 1);
		// wchar_t* nameBuf = (wchar_t*)calloc(nameLen + 1, (nameLen + 1) * sizeof(wchar_t));
		// wchar_t* domainNameBuf = (wchar_t*)calloc(domainNameLen + 1, (domainNameLen + 1) * sizeof(wchar_t));
		if (nameBuf && domainNameBuf) {
			if (::LookupAccountSid(machineName.c_str(), tpsid, nameBuf, &nameLen,
				domainNameBuf, &domainNameLen, &sidUse)) {
				std::wstring ret = nameBuf;
				SAFE_LOCALFREE(tpsid);
				SAFE_ARR_DELETE(nameBuf);
				SAFE_ARR_DELETE(domainNameBuf);
				return ret;
			} else {
				SAFE_LOCALFREE(tpsid);
				SAFE_ARR_DELETE(nameBuf);
				SAFE_ARR_DELETE(domainNameBuf);
				return L"";
			}
		}
		SAFE_LOCALFREE(tpsid);
		SAFE_ARR_DELETE(nameBuf);
		SAFE_ARR_DELETE(domainNameBuf);
		return L"";
	} else {
		return L"";
	}
}

SysOpResult SysHandler::LocalGroupListFromUsername(std::vector<GroupDesc> &outGroupList,
	const std::wstring userName, const std::wstring machineName) const {
	size_t i = 0;
	unsigned char* buf = 0;
	unsigned char* tmpbuf = 0;
	unsigned long entriesRead = 0, totalEntries = 0, res = 0;
	do {
		res = ::NetUserGetLocalGroups(machineName.c_str(), userName.c_str(), 0, LG_INCLUDE_INDIRECT , &buf,
			MAX_PREFERRED_LENGTH, &entriesRead, &totalEntries);
		if (NERR_Success == res || ERROR_MORE_DATA == res) {
			if ((tmpbuf = buf) != 0) {
				LOCALGROUP_USERS_INFO_0* groups = (LOCALGROUP_USERS_INFO_0*)tmpbuf;
				for (i = 0; i < entriesRead; ++i) {
					GroupDesc elem;
					elem.IsADGroup = false;
					elem.GroupName = groups[i].lgrui0_name;
					elem.GroupStrSid = GetStrSIDFromAccountName(elem.GroupName, machineName);
					outGroupList.push_back(elem);
					++tmpbuf;
					++totalEntries;
				}
			}
			SAFE_NetApiBufferFree(buf);
		}
	} while (ERROR_MORE_DATA == res || NERR_Success != res);
	SAFE_NetApiBufferFree(buf);
	return SysOpResult::Success;
}

SysOpResult SysHandler::LocalGroupListFromStrSID(std::vector<GroupDesc> &outGroupList, const std::wstring strSID,
	const std::wstring machineName) const {
	std::wstring username = GetAccountNameFromStrSID(strSID, machineName);
	if (username.length()) {
		SysOpResult res = SysOpResult::Success;
		res = LocalGroupListFromUsername(outGroupList, username, machineName);
		if (SysOpResult::Success != res) {
			return res;
		}
	}
	return SysOpResult::Success;
}

SysOpResult SysHandler::UserLogon(::HANDLE &token, std::wstring userName, std::wstring password,
	std::wstring domain) const {
	if (LogonUser(userName.c_str(), domain.c_str(), password.c_str(), LOGON32_LOGON_NEW_CREDENTIALS,
		LOGON32_PROVIDER_DEFAULT, &token)) {
		return SysOpResult::Success;
	} else {
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::ImpersonateUser(::HANDLE &token) const {
	if (::ImpersonateLoggedOnUser(token)) {
		return SysOpResult::Success;
	} else {
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::GetSysTempFolderPath(std::wstring &tempFldPath, const HKEY *root) const {
	std::wstring tempfld;
	RegHandler reg;
	if (RegOpResult::Success == reg.GetSysTempPath(tempfld, root)) {
		if (std::wstring::npos != tempfld.find(L"%")) {
			std::map<size_t, std::wstring> tmap = tokenFromString(tempfld, L"%", true);
			std::vector<std::wstring> envvalvec;
			for (auto& it : tmap) {
				envvalvec.push_back(it.second);
			}
			if (!envvalvec.size()) {
				return SysOpResult::Fail;
			}
			std::wstring ttokval;
			for (size_t i = 0; i < envvalvec.size(); ++i) {
				if (SysOpResult::Success != ExpandToken(ttokval, envvalvec[i])) {
					return SysOpResult::Fail;
				}
				tempfld = replaceAll_copy(tempfld, envvalvec[i], ttokval);
			}
		}
		if (INVALID_FILE_ATTRIBUTES != ::GetFileAttributes(tempfld.c_str())) {
			tempFldPath = tempfld;
			return SysOpResult::Success;
		}
	}
	std::wstring sysroottoken;
	if (SysOpResult::Success == ExpandToken(sysroottoken, L"%SystemRoot%")) {
		tempfld = sysroottoken + L"\\Temp";
		if (INVALID_FILE_ATTRIBUTES != ::GetFileAttributes(tempfld.c_str())) {
			tempFldPath = tempfld;
			return SysOpResult::Success;
		} else {
			return SysOpResult::Fail;
		}
	} else {
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::RunVBSScript(std::wstring &output, std::wstring &erroutput, const std::wstring scriptPath,
	const std::wstring scriptArgs) const {
	bool avail = false;
	if (SysOpResult::Success == IsCScriptAvailable(avail)) {
		if (avail) {
			if (pathExists(scriptPath)) {
				std::wstring fullpath = trim_copy(scriptPath) + L" " + trim_copy(scriptArgs);
				ProcessHandler proc;
				if (ProcOpResult::Success == proc.RunCommandPiped(L"cscript /nologo \"" + fullpath + L"\"", output,
					erroutput)) {
					return SysOpResult::Success;
				} else {
					return SysOpResult::Fail;
				}
			} else {
				return SysOpResult::Fail;
			}
		} else {
			return SysOpResult::Fail;
		}
	} else {
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::RunPSScript(std::wstring &output, std::wstring &erroutput, const std::wstring scriptPath,
	const std::wstring scriptArgs) const {
	bool avail = false;
	if (SysOpResult::Success == IsPowershellAvailable(avail)) {
		if (avail) {
			if (pathExists(scriptPath)) {
				std::wstring fullpath = trim_copy(scriptPath) + L" " + trim_copy(scriptArgs);
				ProcessHandler proc;
				if (ProcOpResult::Success == proc.RunCommandPiped(L"powershell -File \"" + fullpath + L"\"", output,
					erroutput)) {
					return SysOpResult::Success;
				} else {
					return SysOpResult::Fail;
				}
			} else {
				return SysOpResult::Fail;
			}
		} else {
			return SysOpResult::Fail;
		}
	} else {
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::ExpandToken(std::wstring &outStr, const std::wstring token) const {
	NEW_ARR_NULLIFY(tBuf, wchar_t, 4096);
	if (!tBuf) {
		return SysOpResult::Fail;
	}
	if (::ExpandEnvironmentStrings(token.c_str(), tBuf, wcslen_c(token.c_str()))) {
		outStr = tBuf;
		SAFE_ARR_DELETE(tBuf);
		return SysOpResult::Success;
	} else {
		SAFE_ARR_DELETE(tBuf);
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::IsSysTempFolderAvailable(bool &available, const HKEY *root) const {
	std::wstring tempfld;
	RegHandler reg;
	if (RegOpResult::Success == reg.GetSysTempPath(tempfld, root)) {
		if (std::wstring::npos != tempfld.find(L"%")) {
			std::map<size_t, std::wstring> tmap = tokenFromString(tempfld, L"%", true);
			std::vector<std::wstring> envvalvec;
			for (auto &it : tmap) {
				envvalvec.push_back(it.second);
			}
			if (!envvalvec.size()) {
				return SysOpResult::Fail;
			}
			std::wstring ttokval;
			for (size_t i = 0; i < envvalvec.size(); ++i) {
				if(SysOpResult::Success != ExpandToken(ttokval, envvalvec[i])) {
					return SysOpResult::Fail;
				}
				tempfld = replaceAll_copy(tempfld, envvalvec[i], ttokval);
			}
		}
		if (INVALID_FILE_ATTRIBUTES != ::GetFileAttributes(tempfld.c_str())) {
			std::wstring fpath = tempfld + genRandomString(L"\\tfile_") + L".tmp";
			if (INVALID_FILE_ATTRIBUTES != ::GetFileAttributes(fpath.c_str())) {
				!::DeleteFile(fpath.c_str());
			}
			::HANDLE hFile = ::CreateFile(fpath.c_str(), GENERIC_WRITE,
				FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (INVALID_HANDLE_VALUE != hFile) {
				unsigned long bytesWritten = 0;
				wchar_t tmpbuf[] = L"tmpbuf";
				if (::WriteFile(hFile, tmpbuf, wcslen_c(tmpbuf) * sizeof(wchar_t), &bytesWritten, 0)) {
					if (::CloseHandle(hFile)) {
						if (::DeleteFile(fpath.c_str())) {
							available = true;
							return SysOpResult::Success;
						}
					}
				}
			}
		}
	}
	size_t bufsz = SH_STRBUFSZ * sizeof(wchar_t);
	wchar_t* wbuf = (wchar_t*)malloc(bufsz);
	if (!wbuf) {
		return SysOpResult::Fail;
	}
	memset(wbuf, 0, bufsz);
	if (::ExpandEnvironmentStrings(L"%SystemRoot%", wbuf, wcslen_c(L"%SystemRoot%"))) {
		std::wstring tempFolder = std::wstring(wbuf) + L"\\Temp";
		if (INVALID_FILE_ATTRIBUTES != ::GetFileAttributes(wbuf)) {
			std::wstring tempFilename = tempFolder + genRandomString(L"\\tfile_") + L".tmp";
			::HANDLE hFile = ::CreateFile(tempFilename.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL, 0);
			if (INVALID_HANDLE_VALUE != hFile) {
				if (::CloseHandle(hFile)) {
					if (::DeleteFile(tempFilename.c_str())) {
						available = true;
						SAFE_FREE(wbuf);
						return SysOpResult::Success;
					} else {
						available = true;
						SAFE_FREE(wbuf);
						return SysOpResult::Fail;
					}
				} else {
					available = true;
					SAFE_FREE(wbuf);
					return SysOpResult::Fail;
				}
			} else {
				available = false;
				SAFE_FREE(wbuf);
				return SysOpResult::Success;
			}
		} else {
			available = false;
			SAFE_FREE(wbuf);
			return SysOpResult::Success;
		}
	} else {
		available = false;
		SAFE_FREE(wbuf);
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::IsPowershellAvailable(bool &available) const {
	bool tfldavail = false;
	IsSysTempFolderAvailable(tfldavail);
	if (tfldavail) {
		size_t bufsz = SH_STRBUFSZ * sizeof(wchar_t);
		wchar_t* tBuf = (wchar_t*)malloc(bufsz);
		if (!tBuf) {
			return SysOpResult::Fail;
		}
		memset(tBuf, 0, bufsz);
		std::wstring tscriptPath;
		if (::ExpandEnvironmentStrings(L"%SystemRoot%", tBuf, wcslen_c(L"%SystemRoot%"))) {
			tscriptPath = std::wstring(tBuf) + genRandomString(L"\\Temp\\tscript_") + L".ps1";
			wchar_t scriptbody[] = L"Write-Host \"Test Output\"";
			FSHandler fsh;
			if (FSOpResult::Success != fsh.WriteToTextFile(tscriptPath, scriptbody, TextFileEnc::UTF8)) {
				return SysOpResult::Fail;
			}
			ProcessHandler proc;
			std::wstring out, errout;
			if (ProcOpResult::Success == proc.RunCommandPiped(L"powershell -File \"" + tscriptPath + L"\"",
				out, errout)) {
				if (partialMatch(out, L"Test Output")) {
					available = true;
				} else {
					available = false;
				}
				if (::DeleteFile(tscriptPath.c_str())) {
					return SysOpResult::Success;
				} else {
					return SysOpResult::Fail;
				}
			} else {
				available = false;
				return SysOpResult::Fail;
			}
		}
	} else {
		available = false;
		return SysOpResult::Fail;
	}
	return SysOpResult::Success;
}

SysOpResult SysHandler::IsCScriptAvailable(bool& available) const {
	bool tfldavail = false;
	IsSysTempFolderAvailable(tfldavail);
	if (tfldavail) {
		size_t bufsz = SH_STRBUFSZ * sizeof(wchar_t);
		wchar_t* tBuf = (wchar_t*)malloc(bufsz);
		if (!tBuf) {
			return SysOpResult::Fail;
		}
		memset(tBuf, 0, bufsz);
		std::wstring tscriptPath;
		if (::ExpandEnvironmentStrings(L"%SystemRoot%", tBuf, wcslen_c(L"%SystemRoot%"))) {
			tscriptPath = std::wstring(tBuf) + genRandomString(L"\\Temp\\tscript_") + L".vbs";
			wchar_t scriptbody[] = L"WScript.Echo \"Test Output\"";
			FSHandler fsh;
			if (FSOpResult::Success != fsh.WriteToTextFile(tscriptPath, scriptbody)) {
				return SysOpResult::Fail;
			}
			ProcessHandler proc;
			std::wstring out, errout;
			if (ProcOpResult::Success == proc.RunCommandPiped(L"cscript /nologo " + tscriptPath, out, errout)) {
				if (partialMatch(out, L"Test Output")) {
					available = true;
				} else {
					available = false;
				}
				if (::DeleteFile(tscriptPath.c_str())) {
					return SysOpResult::Success;
				} else {
					return SysOpResult::Fail;
				}
			} else {
				available = false;
				return SysOpResult::Fail;
			}
		}
	} else {
		available = false;
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::GetLocalSystemSID(::PSID &localSysSID) const {
	::SID_IDENTIFIER_AUTHORITY sidAuth = SECURITY_NT_AUTHORITY;
	::PSID psid = 0;
	if (::AllocateAndInitializeSid(&sidAuth, 1, SECURITY_LOCAL_SYSTEM_RID, 0, 0, 0, 0, 0, 0, 0, &psid)) {
		localSysSID = psid;
		return SysOpResult::Success;
	} else {
		localSysSID = 0;
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::GetAdministratorsSID(::PSID &adminSID) const {
	::SID_IDENTIFIER_AUTHORITY sidAuth = SECURITY_NT_AUTHORITY;
	::PSID psid = 0;
	if (::AllocateAndInitializeSid(&sidAuth, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
		&psid)) {
		adminSID = psid;
		return SysOpResult::Success;
	} else {
		adminSID = 0;
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::GetEveryoneSID(::PSID &everyoneSID) const {
	SID_IDENTIFIER_AUTHORITY ntauth = SECURITY_WORLD_SID_AUTHORITY;
	PSID psid = 0;
	if (::AllocateAndInitializeSid(&ntauth, 1, 0, 0, 0, 0, 0, 0, 0, 0, &psid)) {
		everyoneSID = psid;
		return SysOpResult::Success;
	} else {
		everyoneSID = 0;
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::GetAuthedUsersSID(::PSID &authedUsersSID) const {
	::SID_IDENTIFIER_AUTHORITY sidAuth = SECURITY_NT_AUTHORITY;
	::PSID psid = 0;
	if (::AllocateAndInitializeSid(&sidAuth, 1, SECURITY_AUTHENTICATED_USER_RID, 0, 0, 0, 0, 0, 0, 0, &psid)) {
		authedUsersSID = psid;
		return SysOpResult::Success;
	} else {
		authedUsersSID = 0;
		return SysOpResult::Fail;
	}
}

SysOpResult SysHandler::CreateTokenStatistics(
	::PTOKEN_STATISTICS &tokenStats,						// Mandatiry / Out
	unsigned long* lpGroupCount,							// Mandatory
	unsigned long* lpPrivilegeCount,						// Mandatory
	::SECURITY_IMPERSONATION_LEVEL* lpImpersonationLevel,	// Only when ImpersonteToken
	::PLUID lpTokenId,										// Optional
	::PLUID lpAuthenticationId,								// Optional 
	::PLARGE_INTEGER lpExpirationTime,						// Optional
	::TOKEN_TYPE* lpTokenType,								// Primary
	unsigned long* lpDynamicCharged,						// Optional
	unsigned long* lpDynamicAvailable,						// Optional
	::LUID* lpModifiedId									// Optional
) const {
	// check http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnsecure/html/msdn_secguts.asp
	bool bSuccess = false;
	unsigned long dwSize = sizeof(::TOKEN_STATISTICS);
	void* lpData = 0;
	::TOKEN_STATISTICS* lpStatisticsToken = 0;
	::LUID luidFakeTokenId = { 0 };
	::LUID luidFakeAuthenticationId = { 0 };
	::LUID luidFakeModifiedId = { 0 };
	__try {
		if (!::AllocateLocallyUniqueId(&luidFakeTokenId) ||
			!::AllocateLocallyUniqueId(&luidFakeAuthenticationId) ||
			!::AllocateLocallyUniqueId(&luidFakeModifiedId)) {
			__leave;
		}
		lpData = ::LocalAlloc(LPTR, dwSize);
		if (!lpData) {
			return SysOpResult::Fail;
		}
		lpStatisticsToken = (::TOKEN_STATISTICS*)lpData;
		if (lpTokenId) {
			lpStatisticsToken->TokenId.HighPart = lpTokenId->HighPart;
			lpStatisticsToken->TokenId.LowPart = lpTokenId->LowPart;
		} else {
			lpStatisticsToken->TokenId.HighPart = luidFakeTokenId.HighPart;
			lpStatisticsToken->TokenId.LowPart = luidFakeTokenId.LowPart;
		}
		if (lpAuthenticationId) {
			lpStatisticsToken->AuthenticationId.HighPart = lpAuthenticationId->HighPart;
			lpStatisticsToken->AuthenticationId.LowPart = lpAuthenticationId->LowPart;
		} else {
			lpStatisticsToken->TokenId.HighPart = luidFakeAuthenticationId.HighPart;
			lpStatisticsToken->TokenId.LowPart = luidFakeAuthenticationId.LowPart;
		}
		if (lpExpirationTime) {
			lpStatisticsToken->ExpirationTime.HighPart = lpExpirationTime->HighPart;
			lpStatisticsToken->ExpirationTime.LowPart = lpExpirationTime->LowPart;
		} else {
			lpStatisticsToken->ExpirationTime.HighPart = 0x7FFFFFFF;
			lpStatisticsToken->ExpirationTime.LowPart = 0xFFFFFFFF;
		}
		if (lpTokenType) {
			lpStatisticsToken->TokenType = *lpTokenType;
		} else {
			lpStatisticsToken->TokenType = TokenPrimary;
		}
		if (lpImpersonationLevel) {
			lpStatisticsToken->ImpersonationLevel = *lpImpersonationLevel;
		} else {
			lpStatisticsToken->ImpersonationLevel = ::SECURITY_IMPERSONATION_LEVEL::SecurityAnonymous;
		}
		if (lpDynamicCharged) {
			lpStatisticsToken->DynamicCharged = *lpDynamicCharged;
		} else {
			lpStatisticsToken->DynamicCharged = 500; //0x300? //fake fake fake
		}
		if (lpDynamicAvailable) {
			lpStatisticsToken->DynamicAvailable = *lpDynamicAvailable;
		} else {
			lpStatisticsToken->DynamicAvailable = 420;
		}
		if (lpGroupCount) {
			lpStatisticsToken->GroupCount = *lpGroupCount;
		} else {
			lpStatisticsToken->GroupCount = 0; //err?
		}
		if (lpPrivilegeCount) {
			lpStatisticsToken->PrivilegeCount = *lpPrivilegeCount;
		} else {
			lpStatisticsToken->PrivilegeCount = 0;
		}
		if (lpModifiedId) {
			lpStatisticsToken->ModifiedId.HighPart = lpModifiedId->HighPart;
			lpStatisticsToken->ModifiedId.LowPart = lpModifiedId->LowPart;
		} else {
			//make it original
			lpStatisticsToken->TokenId.HighPart = luidFakeTokenId.HighPart; //luidFakeModifiedId.HighPart;
			lpStatisticsToken->TokenId.LowPart = luidFakeTokenId.LowPart; //luidFakeModifiedId.LowPart;
		}
		bSuccess = true;
	} __finally {
		if (bSuccess) {
			tokenStats = (::PTOKEN_STATISTICS)lpData;
			return SysOpResult::Success;
		} else {
			if (lpData) {
				SAFE_LOCALFREE(lpData);
			}
			tokenStats = 0;
			return SysOpResult::Fail;
		}
	}
	return SysOpResult::Success;
}

SysOpResult SysHandler::CreateTokenGroups(::PTOKEN_GROUPS &tokenGroups, ::PSID_AND_ATTRIBUTES lpPSIDGroupsAttr) const {
	int bRet = 0, dim = 0, i = 0;
	while ((lpPSIDGroupsAttr[dim].Sid) && dim < 64) {
		++dim;
	}
	// too many recs
	if (dim >= 64) {
		return SysOpResult::Fail; //Arg Error!
	}
	unsigned char* lpBase = 0;
	unsigned long dwGroupTokenSize = 0;
	::PTOKEN_GROUPS lpGroupToken = 0;
	::SID_AND_ATTRIBUTES* lpEachGrp = 0;
	__try {
		dwGroupTokenSize = sizeof(unsigned long) + sizeof(::SID_AND_ATTRIBUTES) * dim;
		for (i = 0; i < dim; ++i) {
			dwGroupTokenSize += ::GetLengthSid(lpPSIDGroupsAttr[i].Sid);
		}
		// Copying is boring
		lpGroupToken = (::PTOKEN_GROUPS)::LocalAlloc(LPTR, dwGroupTokenSize);
		if (!lpGroupToken) {
			__leave;
		}
		lpGroupToken->GroupCount = dim;
		lpEachGrp = (::SID_AND_ATTRIBUTES*)lpGroupToken->Groups;
		lpBase = (unsigned char*)lpGroupToken;
		lpBase += sizeof(unsigned long) + sizeof(::SID_AND_ATTRIBUTES) * dim;
		for (i = 0; i < dim; ++i) {
			lpEachGrp[i].Attributes = lpPSIDGroupsAttr[i].Attributes;
			CopyMemory(lpBase, lpPSIDGroupsAttr[i].Sid, ::GetLengthSid(lpPSIDGroupsAttr[i].Sid));
			lpEachGrp[i].Sid = (::PSID)lpBase;
			lpBase += ::GetLengthSid(lpPSIDGroupsAttr[i].Sid);
		}
		bRet = 1;
	} __finally {
		if (bRet) {
			tokenGroups = lpGroupToken;
			return SysOpResult::Success;
		} else {
			if (lpGroupToken) {
				SAFE_LOCALFREE(lpGroupToken);
			}
			tokenGroups = 0;
			return SysOpResult::Fail;
		}
	}
}

SysOpResult SysHandler::CreateTokenPriv(::PTOKEN_PRIVILEGES &tokenPriv, unsigned long &dwPrivGranted, LPCTSTR* lpszPriv,
	const bool enableAll) const {
	const wchar_t* _lpszPriv[] = {
		//////////////     Priviledge      /////////////////
		SE_CREATE_TOKEN_NAME, SE_ASSIGNPRIMARYTOKEN_NAME,
		SE_LOCK_MEMORY_NAME, SE_CREATE_TOKEN_NAME,
		SE_ASSIGNPRIMARYTOKEN_NAME, SE_LOCK_MEMORY_NAME, SE_INCREASE_QUOTA_NAME,
		//SE_UNSOLICITED_INPUT_NAME, // failure to find in Win2k, XP
		//SE_MACHINE_ACCOUNT_NAME,  // leading to Zw failure in WinXP
		SE_TCB_NAME, SE_SECURITY_NAME, SE_TAKE_OWNERSHIP_NAME,
		SE_LOAD_DRIVER_NAME, SE_SYSTEM_PROFILE_NAME,
		SE_SYSTEMTIME_NAME, SE_PROF_SINGLE_PROCESS_NAME,
		SE_INC_BASE_PRIORITY_NAME, SE_CREATE_PAGEFILE_NAME,
		SE_CREATE_PERMANENT_NAME, SE_BACKUP_NAME,
		SE_RESTORE_NAME, SE_SHUTDOWN_NAME,
		SE_DEBUG_NAME, SE_AUDIT_NAME,
		SE_SYSTEM_ENVIRONMENT_NAME, SE_CHANGE_NOTIFY_NAME,
		SE_REMOTE_SHUTDOWN_NAME, SE_UNDOCK_NAME,
		SE_SYNC_AGENT_NAME, SE_ENABLE_DELEGATION_NAME,
		SE_MANAGE_VOLUME_NAME,  //failure to find on Win2k but ok when Xp+
		0
		////////////////     NT Rights      ////////////////
		//SE_BATCH_LOGON_NAME, SE_DENY_BATCH_LOGON_NAME,
		//SE_DENY_INTERACTIVE_LOGON_NAME, SE_DENY_NETWORK_LOGON_NAME,
		//SE_DENY_SERVICE_LOGON_NAME, SE_INTERACTIVE_LOGON_NAME ,
		//SE_NETWORK_LOGON_NAME, SE_SERVICE_LOGON_NAME, 0
		///////////////////////////////////////////////////
	};
	int dim = 0;
	const wchar_t** pPriv = 0;
	if (enableAll) {
		pPriv = _lpszPriv;
		while (pPriv[dim]) {
			++dim;
		}
	} else {
		pPriv = lpszPriv;
		if (lpszPriv) while (pPriv[dim] != NULL)  dim++;
	}
	bool bSuccess = false;
	int i = 0;
	unsigned long dwSize = sizeof(unsigned long) + dim * sizeof(::LUID_AND_ATTRIBUTES),
		dwIndex = 0; // real number of priviledes;
	::TOKEN_PRIVILEGES* lpPrivToken = { 0 };
	void* lpData = 0;
	void* lpData2 = 0;
	__try {
		lpData = ::LocalAlloc(LPTR, dwSize);
		lpPrivToken = (::TOKEN_PRIVILEGES*)(lpData);
		lpPrivToken->PrivilegeCount = dim;
		::LUID luid = { 0 };
		for (i = 0; i < dim; ++i) {
			const wchar_t* szPrivAdd = pPriv[i];
			memset(&luid, 0, sizeof(::LUID));
			if (!::LookupPrivilegeValue(0, szPrivAdd, &luid)) {
				// If the name is bogus... SE_UNSOLICITED_INPUT_NAME brrr go on
				//__leave;
				continue;
			}
			lpPrivToken->Privileges[dwIndex].Luid.HighPart = luid.HighPart;
			lpPrivToken->Privileges[dwIndex].Luid.LowPart = luid.LowPart;
			lpPrivToken->Privileges[dwIndex].Attributes = SE_PRIVILEGE_ENABLED | SE_PRIVILEGE_ENABLED_BY_DEFAULT;
			dwIndex++;
		}
		if (dwIndex == (unsigned long)dim) {
			dwPrivGranted = dim;
			lpData2 = lpData;
			bSuccess = true;
		} else {
			dwPrivGranted = dwIndex;
			dwSize = sizeof(unsigned long) + dwIndex * sizeof(::LUID_AND_ATTRIBUTES);
			lpData2 = ::LocalAlloc(LPTR, dwSize);
			if (!lpData2) {
				__leave;
			}
			CopyMemory(lpData2, lpData, dwSize);
			lpPrivToken = (::TOKEN_PRIVILEGES*)(lpData2);
			lpPrivToken->PrivilegeCount = dwIndex;
			bSuccess = true;
		}
	} __finally {
		if (bSuccess) {
			if (lpData != lpData2 && lpData) {
				SAFE_LOCALFREE(lpData);
			}
			tokenPriv = (::TOKEN_PRIVILEGES*)lpData2;
			return SysOpResult::Success;
		} else {
			if (lpData) {
				SAFE_LOCALFREE(lpData);
			}
			tokenPriv = 0;
			return SysOpResult::Fail;
		}
	}
}

SysOpResult SysHandler::CreatePureSystemToken(::HANDLE &hToken) const {
	hToken = 0;
	int bRet = 0;
	ProcessHandler proc;
	unsigned long procpid = proc.GetCurrentProcPid();
	std::vector<std::wstring> privs = proc.GetProcPrivileges(procpid);
	if (privs.size()) {
		if (!valInList(privs, L"SeCreateTokenPrivilge")) {
			if (!proc.EnableCreateTokenPrivilege(procpid)) {
				return SysOpResult::Fail;
			}
		}
	} else {
		return SysOpResult::Fail;
	}
	::PTOKEN_STATISTICS lpStatsToken = { 0 };
	::PSID lpSidOwner = 0;
	if (SysOpResult::Success != GetLocalSystemSID(lpSidOwner)) {
		return SysOpResult::Fail;
	}
	if (lpSidOwner) {
		::NTSTATUS ntStatus = 0;
		// SYSTEM Token has 3 group SIDs
		// SID_AND_ATTRIBUTES grpSIDAttr[3];    
		::PTOKEN_GROUPS lpGroupToken = 0;
		unsigned long dwGroupNumber = 3;
		::PTOKEN_PRIVILEGES lpPrivToken = { 0 };
		unsigned long dwPrivGranted = 0;
		::TOKEN_OWNER ownerToken = { 0 };
		ownerToken.Owner = 0;
		::TOKEN_PRIMARY_GROUP primGroupToken = { 0 };
		if (SysOpResult::Success != GetLocalSystemSID(primGroupToken.PrimaryGroup)) {
			return SysOpResult::Fail;
		}
		::PTOKEN_DEFAULT_DACL lpDaclToken = { 0 };
		::SID_AND_ATTRIBUTES lpEachGrp[4] = { 0 };
		lpEachGrp[0].Sid = 0;
		lpEachGrp[1].Sid = 0;
		lpEachGrp[2].Sid = 0;
		if (SysOpResult::Success != GetAdministratorsSID(lpEachGrp[0].Sid)) {
			return SysOpResult::Fail;
		}
		lpEachGrp[0].Attributes = SE_GROUP_ENABLED | SE_GROUP_ENABLED_BY_DEFAULT | SE_GROUP_OWNER;
		if (SysOpResult::Success != GetEveryoneSID(lpEachGrp[1].Sid)) {
			return SysOpResult::Fail;
		}
		lpEachGrp[1].Attributes = SE_GROUP_ENABLED | SE_GROUP_ENABLED_BY_DEFAULT | SE_GROUP_MANDATORY;
		if (SysOpResult::Success != GetAuthedUsersSID(lpEachGrp[2].Sid)) {
			return SysOpResult::Fail;
		}
		lpEachGrp[2].Attributes = SE_GROUP_ENABLED | SE_GROUP_ENABLED_BY_DEFAULT | SE_GROUP_MANDATORY;
		lpEachGrp[3].Sid = 0;
		lpEachGrp[3].Attributes = 0;
		if (SysOpResult::Success != CreateTokenGroups(lpGroupToken, lpEachGrp)) {
			return SysOpResult::Fail;
		}
		if (SysOpResult::Success != CreateTokenPriv(lpPrivToken, dwPrivGranted, 0, true)) {
			return SysOpResult::Fail;
		}
		if (SysOpResult::Success != GetLocalSystemSID(ownerToken.Owner)) {
			return SysOpResult::Fail;
		}
		::PSID lpSidOwner = 0;
		::TOKEN_USER userToken = { 0 };
		if (SysOpResult::Success != GetLocalSystemSID(userToken.User.Sid)) {
			return SysOpResult::Fail;
		}
		userToken.User.Attributes = 0;    //no use
		//this luid only unique in this session
		::LUID luid = { 0 };
		if (!::AllocateLocallyUniqueId(&luid)) {
			return SysOpResult::Fail;
		}
		::TOKEN_SOURCE sourceToken = { {'*', 'S', 'Y', 'S', 'T', 'E', 'M', '*'}, {luid.LowPart, luid.HighPart} };
		// Allocate the System Luid.  The first 1000 LUIDs are reserved.
		// Use #999 here (0x3E7 = 999)
		//
		//#define SYSTEM_LUID                     { 0x3E7, 0x0 }
		//#define ANONYMOUS_LOGON_LUID            { 0x3e6, 0x0 }
		//#define LOCALSERVICE_LUID               { 0x3e5, 0x0 }
		//#define NETWORKSERVICE_LUID             { 0x3e4, 0x0 }
		::LUID authid = SYSTEM_LUID;
		// PTOKEN_STATISTICS lpStatsToken = { 0 };
		if (SysOpResult::Success != CreateTokenStatistics(lpStatsToken, 0, 0, 0, 0, 0, 0, 0,
			&dwGroupNumber, &dwPrivGranted, 0)) {
			return SysOpResult::Fail;
		}
		//typedef enum _SECURITY_IMPERSONATION_LEVEL {
		//SecurityAnonymous,
		//SecurityIdentification,
		//SecurityImpersonation,
		//SecurityDelegation
		//} SECURITY_IMPERSONATION_LEVEL, *
		//  PSECURITY_IMPERSONATION_LEVEL;

		//typedef struct _SECURITY_QUALITY_OF_SERVICE {
		//DWORD Length;
		//SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
		//SECURITY_CONTEXT_TRACKING_MODE ContextTrackingMode;
		//BOOLEAN EffectiveOnly;
		//} SECURITY_QUALITY_OF_SERVICE, * PSECURITY_QUALITY_OF_SERVICE;

		//#define SECURITY_DYNAMIC_TRACKING      (TRUE)
		//#define SECURITY_STATIC_TRACKING       (FALSE)

		//typedef BOOLEAN SECURITY_CONTEXT_TRACKING_MODE,
		//              * PSECURITY_CONTEXT_TRACKING_MODE;
		//fake value,,,
		::SECURITY_QUALITY_OF_SERVICE sqos = { sizeof(sqos),
			::SECURITY_IMPERSONATION_LEVEL::SecurityAnonymous, SECURITY_STATIC_TRACKING, 0 };
		// ::OBJECT_ATTRIBUTES oa = { sizeof(oa), 0, 0, 0, 0, &sqos };
		ObjectAttributes oa;
		memset(&oa, 0, sizeof(ObjectAttributes));
		oa.uLength = sizeof(ObjectAttributes);
		oa.pSecurityQualityOfService = &sqos;
		::HMODULE hModule = ::LoadLibrary(L"ntdll.dll");
		if (!hModule) {
			return SysOpResult::Fail;
		}
		pNtCreateToken NtCreateToken = (pNtCreateToken)
			::GetProcAddress(hModule, "NtCreateToken");
		if (0 == NtCreateToken) {
			return SysOpResult::Fail;
		}
		ntStatus = NtCreateToken(&hToken, TOKEN_ALL_ACCESS, &oa, TokenPrimary, (::LUID*)(&authid),
			// NT::PLUID(&stats->AuthenticationId),
			(::LARGE_INTEGER*)(&lpStatsToken->ExpirationTime),
			&userToken, lpGroupToken, lpPrivToken, &ownerToken,
			&primGroupToken, lpDaclToken, &sourceToken);
		/* if (!::FreeLibrary(hModule)) {
			return SysOpResult::Fail;
		} */
		//You may get 0xC0000061 which is STATUS_PRIVILEDGE_NOT_HELD,
		//If so, try to GRANT SeCreateTokenPrivilege and relog once...
		//STATUS_PRIVILEDGE_NOT_HELD;
		//#define STATUS_ACCESS_VIOLATION     ((NTSTATUS)0xC0000005L)    // winnt
		//An invalid parameter was passed to a service or function.
		//#define STATUS_INVALID_PARAMETER         ((NTSTATUS)0xC000000DL)
		if (NT_SUCCESS(ntStatus)) {
			bRet = true;
		} else {
			return SysOpResult::Fail;
		}
		if (lpSidOwner) {
			::FreeSid(lpSidOwner);
			lpSidOwner = 0;
		}
		if (primGroupToken.PrimaryGroup) {
			::FreeSid(primGroupToken.PrimaryGroup);
			primGroupToken.PrimaryGroup = 0;
		}
		// free DACL if you allocate it yourself inside this Func
		if (lpDaclToken) {}
		if (lpEachGrp[0].Sid) {
			::FreeSid(lpEachGrp[0].Sid);
			lpEachGrp[0].Sid = 0;
		}
		if (lpEachGrp[1].Sid) {
			::FreeSid(lpEachGrp[1].Sid);
			lpEachGrp[1].Sid = 0;
		}
		if (lpEachGrp[2].Sid) {
			::FreeSid(lpEachGrp[2].Sid);
			lpEachGrp[2].Sid = 0;
		}
		// Free Inside SID
		if (lpGroupToken && lpGroupToken->GroupCount > 0) {
			::SID_AND_ATTRIBUTES* lpEachGrp = 0;
			for (int i = 0; i < (int)lpGroupToken->GroupCount; ++i) {
				lpEachGrp = lpGroupToken[i].Groups;
				::FreeSid(lpEachGrp->Sid);
				++lpEachGrp;
			}
		}
		if (lpGroupToken) {
			SAFE_LOCALFREE(lpGroupToken);
		}
		if (lpStatsToken) {
			SAFE_LOCALFREE(lpStatsToken);
		}
		if (lpPrivToken) {
			SAFE_LOCALFREE(lpPrivToken);
		}
		return SysOpResult::Success;
	}
	return SysOpResult::Success;
}

SysOpResult SysHandler::CreatePureUserToken(::HANDLE &hToken) const {
	return SysOpResult::Success;
}

SysOpResult SysHandler::ImpersonateIfConformToken(long &operatonStatus, ::HANDLE &hToken) {
	static volatile unsigned char guz = 0;
	// ::OBJECT_ATTRIBUTES zoa = { sizeof(::OBJECT_ATTRIBUTES) };
	ObjectAttributes zoa;
	memset(&zoa, 0, sizeof(ObjectAttributes));
	zoa.uLength = sizeof(ObjectAttributes);
	unsigned long cb = 0, rcb = 0x200;
	void* stack = alloca(sizeof(guz));
	if (!stack) {
		return SysOpResult::Fail;
	}
	union {
		void* buf;
		::TOKEN_PRIVILEGES* ptp;
	};
	::HMODULE hModule = ::LoadLibrary(L"ntdll.dll");
	if (!hModule) {
		return SysOpResult::Fail;
	}
	pNtQueryInformationToken NtQueryInformationToken = (pNtQueryInformationToken)
		::GetProcAddress(hModule, "NtQueryInformationToken");
	pNtSetInformationThread NtSetInformationThread = (pNtSetInformationThread)
		::GetProcAddress(hModule, "NtSetInformationThread");
	pNtDuplicateToken NtDuplicateToken = (pNtDuplicateToken)
		::GetProcAddress(hModule, "NtDuplicateToken");
	pNtAdjustPrivilegesToken NtAdjustPrivilegesToken = (pNtAdjustPrivilegesToken)
		::GetProcAddress(hModule, "NtAdjustPrivilegesToken");
	pNtClose NtClose = (pNtClose)
		::GetProcAddress(hModule, "NtClose");
	if (0 == NtQueryInformationToken) {
		return SysOpResult::Fail;
	}
	if (0 == NtSetInformationThread) {
		return SysOpResult::Fail;
	}
	if (0 == NtDuplicateToken) {
		return SysOpResult::Fail;
	}
	if (0 == NtAdjustPrivilegesToken) {
		return SysOpResult::Fail;
	}
	if (0 == NtClose) {
		return SysOpResult::Fail;
	}
	::NTSTATUS status = 0;
	do {
		if (cb < rcb) {
			cb = RtlPointerToOffset(buf = alloca(rcb - cb), stack);
		}
		if (0 <= (status = NtQueryInformationToken(&hToken, TokenPrivileges, buf, cb, &rcb))) {
			if (unsigned long PrivilegeCount = ptp->PrivilegeCount) {
				unsigned long n = 1;
				int bNeedAdjust = 0;
				::PLUID_AND_ATTRIBUTES Privileges = ptp->Privileges;
				do {
					if (!Privileges->Luid.HighPart) {
						switch (Privileges->Luid.LowPart) {
						case 0: // SE_CREATE_TOKEN_NAME:
							if (!(Privileges->Attributes & SE_PRIVILEGE_ENABLED)) {
								Privileges->Attributes |= SE_PRIVILEGE_ENABLED;
								bNeedAdjust = 1;
							}
							if (!--n) {
								static SECURITY_QUALITY_OF_SERVICE sqos = {
									sizeof(sqos), SecurityImpersonation, SECURITY_STATIC_TRACKING, FALSE
								};
								// static OBJECT_ATTRIBUTES soa = { sizeof(soa), 0, 0, 0, 0, &sqos };
								ObjectAttributes soa;
								memset(&soa, 0, sizeof(ObjectAttributes));
								soa.uLength = sizeof(ObjectAttributes);
								soa.pSecurityQualityOfService = &sqos;
								if (0 <= (status = NtDuplicateToken(hToken, TOKEN_ADJUST_PRIVILEGES |
									TOKEN_IMPERSONATE, &soa, SECURITY_IMPERSONATION_LEVEL::SecurityAnonymous,
									::TOKEN_TYPE::TokenImpersonation, &hToken))) {
									if (bNeedAdjust) {
										status = NtAdjustPrivilegesToken(hToken, 0, ptp, 0, 0, 0);
									}
									if (status == STATUS_SUCCESS) {
										::HANDLE cThread = ::GetCurrentThread();
										status = NtSetInformationThread(&cThread,
											::THREAD_INFORMATION_CLASS::ThreadAbsoluteCpuPriority,
											&hToken, sizeof(::HANDLE));
									}
									NtClose(hToken);
								}
								operatonStatus = status;
								return SysOpResult::Success;
							}
							break;
						}
					}
				} while (Privileges++, --PrivilegeCount);
			}
			operatonStatus = STATUS_PRIVILEGE_NOT_HELD;
			return SysOpResult::Fail;
		}
	} while (status == STATUS_BUFFER_TOO_SMALL);
	if (!::FreeLibrary(hModule)) {
		return SysOpResult::Fail;
	}
	return SysOpResult::Success;
}

SysOpResult SysHandler::EnumLocalGroups(std::vector<GroupDesc> &groupList,
	const std::wstring machineName, const bool enumAccs,
	const std::vector<AccountDesc> *accList) const {
	size_t i = 0;
	unsigned char* buf = 0;
	unsigned char* tmpbuf = 0;
	unsigned long entriesRead = 0, totalEntries = 0, res = 0;
#ifdef _WIN64
	unsigned long long resptr = 0;
#else
	unsigned long resptr = 0;
#endif
	do {
		res = ::NetLocalGroupEnum(machineName.c_str(), 1, &buf, MAX_PREFERRED_LENGTH, &entriesRead,
			&totalEntries, &resptr);
		if (NERR_Success == res || ERROR_MORE_DATA == res) {
			if ((tmpbuf = buf) != 0) {
				LOCALGROUP_INFO_1* groups = (LOCALGROUP_INFO_1*)tmpbuf;
				for (i = 0; i < entriesRead; ++i) {
					GroupDesc elem;
					elem.IsADGroup = false;
					elem.GroupName = groups[i].lgrpi1_name;
					elem.Comment = groups[i].lgrpi1_comment;
					elem.GroupStrSid = GetStrSIDFromAccountName(elem.GroupName, machineName);
					groupList.push_back(elem);
					++tmpbuf;
					++totalEntries;
				}
			}
		}
		SAFE_NetApiBufferFree(buf);
	} while (ERROR_MORE_DATA == res || NERR_Success != res);
	SAFE_NetApiBufferFree(buf);
	if (enumAccs) {
		std::vector<AccountDesc> accs;
		if (enumAccs) {
			if (accList) {
				accs = *accList;
			} else {
				SysOpResult res = SysOpResult::Success;
				res = EnumAccounts(accs, machineName);
				if (SysOpResult::Success != res) {
					return res;
				}
			}
		}
		size_t j = 0, k = 0;
		buf = 0;
		tmpbuf = 0;
		entriesRead = 0;
		totalEntries = 0;
		res = 0;
		resptr = 0;
		for (i = 0; i < groupList.size(); ++i) {
			do {
				res = ::NetLocalGroupGetMembers(machineName.c_str(), groupList[i].GroupName.c_str(), 1,
					&buf, MAX_PREFERRED_LENGTH, &entriesRead, &totalEntries, &resptr);
				if (NERR_Success == res || ERROR_MORE_DATA == res) {
					if ((tmpbuf = buf) != 0) {
						LOCALGROUP_MEMBERS_INFO_1* groupmembers = (LOCALGROUP_MEMBERS_INFO_1*)tmpbuf;
						for (j = 0; j < entriesRead; ++j) {
							for (k = 0; k < accs.size(); ++k) {
								if (StrSIDFromSID(groupmembers[j].lgrmi1_sid) == accs[k].sid) {
									if (std::find(groupList[i].Accounts.begin(), groupList[i].Accounts.end(),
										accs[k]) == groupList[i].Accounts.end()) {
										groupList[i].Accounts.push_back(accs[k]);
									}
								}
							}
						}
						++tmpbuf;
						++totalEntries;
					}
				}
				SAFE_NetApiBufferFree(buf);
			} while (ERROR_MORE_DATA == res || NERR_Success != res);
			SAFE_NetApiBufferFree(buf);
		}
	}
	return SysOpResult::Success;
}

SysOpResult SysHandler::EnumAccounts(std::vector<AccountDesc> &accountList,
	const std::wstring machineName, const bool enumGroups) const {
	unsigned long entriesRead = 0, totalEntries = 0, resHandle = 0, res = 0;
	::USER_INFO_20* buf = 0;
	::USER_INFO_20* tmpbuf = 0;
	do {
		SAFE_NetApiBufferFree(buf);
		res = ::NetUserEnum(machineName.c_str(), 20,
			FILTER_TEMP_DUPLICATE_ACCOUNT | FILTER_NORMAL_ACCOUNT | FILTER_INTERDOMAIN_TRUST_ACCOUNT |
			FILTER_WORKSTATION_TRUST_ACCOUNT | FILTER_SERVER_TRUST_ACCOUNT,
			(unsigned char**)&buf, MAX_PREFERRED_LENGTH, &entriesRead, &totalEntries, &resHandle);
		if (NERR_Success == res || ERROR_MORE_DATA == res) {
			if ((tmpbuf = buf) != 0) {
				for (size_t i = 0; i < entriesRead; ++i) {
					AccountDesc elem;
					elem.ncNormalAcc = tmpbuf->usri20_flags & UF_NORMAL_ACCOUNT;
					elem.ncTempDupAcc = tmpbuf->usri20_flags & UF_TEMP_DUPLICATE_ACCOUNT;
					elem.ncWorkstationTrustAcc = tmpbuf->usri20_flags & UF_WORKSTATION_TRUST_ACCOUNT;
					elem.ncServerTrustAcc = tmpbuf->usri20_flags & UF_SERVER_TRUST_ACCOUNT;
					elem.ncInterdomainTrustAcc = tmpbuf->usri20_flags & UF_INTERDOMAIN_TRUST_ACCOUNT;
					elem.logonScriptExecuted = tmpbuf->usri20_flags & UF_SCRIPT;
					elem.accountDisabled = tmpbuf->usri20_flags & UF_ACCOUNTDISABLE;
					elem.passwordNotNeeded = tmpbuf->usri20_flags & UF_PASSWD_NOTREQD;
					elem.passwordCantChange = tmpbuf->usri20_flags & UF_PASSWD_CANT_CHANGE;
					elem.accountLocked = tmpbuf->usri20_flags & UF_LOCKOUT;
					elem.passwordNoExpire = tmpbuf->usri20_flags & UF_DONT_EXPIRE_PASSWD;
					elem.passwordEncrypted = tmpbuf->usri20_flags & UF_ENCRYPTED_TEXT_PASSWORD_ALLOWED;
					elem.accountNotDelegated = tmpbuf->usri20_flags & UF_NOT_DELEGATED;
					elem.passwordSmartCardNeeded = tmpbuf->usri20_flags & UF_SMARTCARD_REQUIRED;
					elem.accountDESKeysOnly = tmpbuf->usri20_flags & UF_USE_DES_KEY_ONLY;
					elem.accountNoKerberosPreauth = tmpbuf->usri20_flags & UF_DONT_REQUIRE_PREAUTH;
					elem.accountTrustedForDelegation = tmpbuf->usri20_flags & UF_TRUSTED_FOR_DELEGATION;
					elem.passwordExpired = tmpbuf->usri20_flags & UF_PASSWORD_EXPIRED;
					elem.accountTrustedToAuthForDelegation = tmpbuf->usri20_flags & UF_TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION;
					if (!IsBadReadPtr(tmpbuf->usri20_name)) {
						elem.accountName = tmpbuf->usri20_name;
						elem.sid = GetStrSIDFromAccountName(elem.accountName, machineName);
					}
					/*if (elem.sid.length() && !elem.accountName.length()) {
						elem.accountName = GetAccountNameFromSID(SIDFromStrSid(elem.sid), machineName);
					}*/
					if (!IsBadReadPtr(tmpbuf->usri20_full_name)) {
						elem.accountFullname = tmpbuf->usri20_full_name;
					}
					if (!IsBadReadPtr(tmpbuf->usri20_comment)) {
						elem.comment = tmpbuf->usri20_comment;
					}
					if (enumGroups) {
						if (SysOpResult::Success != LocalGroupListFromUsername(elem.groups,
							elem.accountName, machineName)) {
							return SysOpResult::Fail;
						}
					}
					accountList.push_back(elem);
					++tmpbuf;
					++totalEntries;
				}
			}
		}
	} while (ERROR_MORE_DATA == res || NERR_Success != res);
	SAFE_NetApiBufferFree(buf);
	::USER_INFO_4* buf4 = 0;
	AccountDesc* rec = 0;
	for (size_t i = 0; i < accountList.size(); ++i) {
		res = ::NetUserGetInfo(machineName.c_str(), accountList[i].accountName.c_str(), 4,
			(unsigned char**)&buf4);
		if (NERR_Success == res) {
			if (accountList[i].accountName == buf4->usri4_name) {
				rec = &accountList[i];
				rec->privileges = buf4->usri4_priv;
				if (0 == rec->privileges) {
					rec->privilegesStr = L"Guest (" + std::to_wstring(rec->privileges) + L")";
				} else if (1 == rec->privileges) {
					rec->privilegesStr = L"User (" + std::to_wstring(rec->privileges) + L")";
				} else if (2 == rec->privileges) {
					rec->privilegesStr = L"Admin (" + std::to_wstring(rec->privileges) + L")";
				}
				rec->logonScriptPath = buf4->usri4_script_path;
				rec->homeDirPath = buf4->usri4_home_dir;
				rec->homeDirPath = buf4->usri4_home_dir_drive;
				rec->profileDirPath = buf4->usri4_profile;
				rec->params = buf4->usri4_parms;
				rec->allowedWorkstations = buf4->usri4_workstations;
				/*if (!wcslen_c(rec->allowedWorkstations.c_str())) {
					rec->allowedWorkstations = L"\\\\*";
				}*/
				rec->passwordAge = buf4->usri4_password_age;
				if (rec->passwordAge) {
					rec->passwordSetStr = UnixTime2StrTime((CurrentTime2Unixtime() - rec->passwordAge) + GetTZOffset());
				} else {
					rec->passwordSetStr = L"Password never changed";
				}
				rec->lastLogOn = buf4->usri4_last_logon;
				if (rec->lastLogOn) {
					rec->lastLogOnStr = UnixTime2StrTime(rec->lastLogOn + GetTZOffset());
				} else {
					rec->lastLogOnStr = L"User never logged on";
				}
				rec->lastLogOff = buf4->usri4_last_logoff;
				if (rec->lastLogOff) {
					rec->lastLogOffStr = UnixTime2StrTime(rec->lastLogOff);
				} else {
					// rec->lastLogOffStr = UnixTime2StrTime(CurrentTime2Unixtime() + GetTZOffset());
					if (rec->lastLogOn) {
						// Is user session running
						rec->lastLogOffStr = L"Session running";
					} else {
						rec->lastLogOffStr = L"User never logged on";
					}
				}
				rec->authPrintOp = buf4->usri4_auth_flags & AF_OP_PRINT;
				rec->authCommOp = buf4->usri4_auth_flags & AF_OP_COMM;
				rec->authSrvOp = buf4->usri4_auth_flags & AF_OP_SERVER;
				rec->authAccOp = buf4->usri4_auth_flags & AF_OP_ACCOUNTS;
				rec->expireDate = buf4->usri4_acct_expires;
				if ((rec->expireDate != TIMEQ_FOREVER) && (rec->expireDate > 0) && rec->expireDate) {
					rec->expireDateStr = UnixTime2StrTime(rec->expireDate);
				} else {
					rec->expireDateStr = L"Never";
				}
				rec->maxStorage = buf4->usri4_max_storage;
				if (USER_MAXSTORAGE_UNLIMITED == rec->maxStorage) {
					rec->maxStorageStr = L"Unlimited";
				} else {
					rec->maxStorageStr = std::to_wstring(rec->maxStorage / 1048576) + L" MB";
				}
				rec->passwordBadCount = buf4->usri4_bad_pw_count;
				rec->logonCount = buf4->usri4_num_logons;
				rec->countryCode = buf4->usri4_country_code;
				rec->codePage = buf4->usri4_code_page;
				rec->primaryGroupId = buf4->usri4_primary_group_id;
				rec->logonServer = buf4->usri4_logon_server;
			}
		}
		SAFE_NetApiBufferFree(buf4);
	}
	return SysOpResult::Success;
}

SysOpResult SysHandler::IsAccountMemberOfGroup(const ::PSID groupSID, const ::PSID testSID, bool &isMember,
	const std::wstring machineName) const {
	isMember = false;
	std::vector<GroupDesc> groups;
	std::wstring teststrsid = StrSIDFromSID(testSID);
	std::wstring groupstrsid = StrSIDFromSID(groupSID);
	if (SysOpResult::Success == LocalGroupListFromStrSID(groups, groupstrsid, machineName)) {
		for (size_t i = 0; i < groups.size(); ++i) {
			if (lower_copy(teststrsid) == lower_copy(groups[i].GroupStrSid)) {
				isMember = true;
				return SysOpResult::Success;
			}
		}
	} else {
		return SysOpResult::Fail;
	}
	return SysOpResult::Success;
}

SysOpResult SysHandler::IsAccountMemberOfGroup(const std::wstring groupName, const std::wstring testAccName,
	bool &isMember, const std::wstring machineName) const {
	isMember = false;
	std::vector<GroupDesc> groups;
	if (SysOpResult::Success == LocalGroupListFromUsername(groups, testAccName, machineName)) {
		for (size_t i = 0; i < groups.size(); ++i) {
			if (lower_copy(groupName) == lower_copy(groups[i].GroupName)) {
				isMember = true;
				return SysOpResult::Success;
			}
		}
	} else {
		return SysOpResult::Fail;
	}
	return SysOpResult::Success;
}