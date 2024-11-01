#include <iostream>
#include "aclhelper.h"

SecDesc::SecDesc() {
    daclInfoSz = 0;
    daclAbsInfoSz = 0;
    saclInfoSz = 0;
    saclAbsInfoSz = 0;
    ownerInfoSz = 0;
    primaryGroupInfoSz = 0;
    absoluteSDInfoSz = 0;
    selfRelativeSDInfoSz = 0;
    daclInfo = 0;
    daclAbsInfo = 0;
    saclInfo = 0;
    saclAbsInfo = 0;
    absoluteSDInfo = 0;
    selfRelativeSDInfo = 0;
}

SecDesc::SecDesc(const SecDesc &other) {
    if (this != &other) {
        ownerInfo = other.ownerInfo;
        primaryGroupInfo = other.primaryGroupInfo;
        daclInfoSz = other.daclInfoSz;
        daclAbsInfoSz = other.daclAbsInfoSz;
        saclInfoSz = other.saclInfoSz;
        saclAbsInfoSz = other.saclAbsInfoSz;
        ownerInfoSz = other.ownerInfoSz;
        primaryGroupInfoSz = other.primaryGroupInfoSz;
        absoluteSDInfoSz = other.absoluteSDInfoSz;
        selfRelativeSDInfoSz = other.selfRelativeSDInfoSz;
        if (other.daclInfo) {
            daclInfo = ::LocalAlloc(LPTR, other.daclInfoSz);
            if (daclInfo) {
                memcpy(daclInfo, other.daclInfo, other.daclInfoSz);
            }
        } else {
            if (daclInfo && daclInfoSz && !IsBadReadPtr(daclInfo)) {
                SAFE_LOCALFREE(daclInfo);
            } else {
                daclInfo = 0;
            }
        }
        if (other.daclAbsInfo) {
            daclAbsInfo = ::LocalAlloc(LPTR, other.daclAbsInfoSz);
            if (daclAbsInfo) {
                memcpy(daclAbsInfo, other.daclAbsInfo, other.daclAbsInfoSz);
            }
        } else {
            if (daclAbsInfo && daclAbsInfoSz) {
                SAFE_LOCALFREE(daclAbsInfo);
            } else {
                daclAbsInfo = 0;
            }
        }
        if (other.saclInfo) {
            saclInfo = ::LocalAlloc(LPTR, other.saclInfoSz);
            if (saclInfo) {
                memcpy(saclInfo, other.saclInfo, other.saclInfoSz);
            }
        } else {
            if (saclInfo && saclInfoSz) {
                SAFE_LOCALFREE(saclInfo);
            } else {
                saclInfo = 0;
            }
        }
        if (other.saclAbsInfo) {
            saclAbsInfo = ::LocalAlloc(LPTR, other.saclAbsInfoSz);
            if (saclAbsInfo) {
                memcpy(saclAbsInfo, other.saclAbsInfo, other.saclAbsInfoSz);
            }
        } else {
            if (saclAbsInfo && saclAbsInfoSz) {
                SAFE_LOCALFREE(saclAbsInfo);
            } else {
                saclAbsInfo = 0;
            }
        }
        if (other.absoluteSDInfo) {
            absoluteSDInfo = ::LocalAlloc(LPTR, other.absoluteSDInfoSz);
            if (absoluteSDInfo) {
                memcpy(absoluteSDInfo, other.absoluteSDInfo, other.absoluteSDInfoSz);
            }
        } else {
            if (absoluteSDInfo && absoluteSDInfoSz) {
                SAFE_LOCALFREE(absoluteSDInfo);
            } else {
                absoluteSDInfo = 0;
            }
        }
        if (other.selfRelativeSDInfo) {
            selfRelativeSDInfo = ::LocalAlloc(LPTR, other.selfRelativeSDInfoSz);
            if (selfRelativeSDInfo) {
                memcpy(selfRelativeSDInfo, other.selfRelativeSDInfo, other.selfRelativeSDInfoSz);
            }
        } else {
            if (selfRelativeSDInfo && selfRelativeSDInfoSz) {
                SAFE_LOCALFREE(selfRelativeSDInfo);
            } else {
                selfRelativeSDInfo = 0;
            }
        }
    }
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
SecDesc::SecDesc(SecDesc &&other) noexcept {
    if (this != &other) {
        daclInfoSz = std::exchange(other.daclInfoSz, 0);
        daclAbsInfoSz = std::exchange(other.daclAbsInfoSz, 0);
        saclInfoSz = std::exchange(other.saclInfoSz, 0);
        saclAbsInfoSz = std::exchange(other.saclAbsInfoSz, 0);
        ownerInfoSz = std::exchange(other.ownerInfoSz, 0);
        primaryGroupInfoSz = std::exchange(other.primaryGroupInfoSz, 0);
        absoluteSDInfoSz = std::exchange(other.absoluteSDInfoSz, 0);
        selfRelativeSDInfoSz = std::exchange(other.selfRelativeSDInfoSz, 0);
        ownerInfo = std::move(other.ownerInfo);
        primaryGroupInfo = std::move(other.primaryGroupInfo);
        daclInfo = std::move(other.daclInfo);
        other.daclInfo = 0;
        daclAbsInfo = std::move(other.daclAbsInfo);
        other.daclAbsInfo = 0;
        saclInfo = std::move(other.saclInfo);
        other.saclInfo = 0;
        saclAbsInfo = std::move(other.saclAbsInfo);
        other.saclAbsInfo = 0;
        absoluteSDInfo = std::move(other.absoluteSDInfo);
        other.absoluteSDInfo = 0;
        selfRelativeSDInfo = std::move(other.selfRelativeSDInfo);
        other.selfRelativeSDInfo = 0;
    }
}
#endif

SecDesc::~SecDesc() {
    if (daclInfo) {
        SAFE_LOCALFREE(daclInfo);
    }
    if (daclAbsInfo) {
        SAFE_LOCALFREE(daclAbsInfo);
    }
    if (saclInfo) {
        SAFE_LOCALFREE(saclInfo);
    }
    if (saclAbsInfo) {
        SAFE_LOCALFREE(saclAbsInfo);
    }
    if (absoluteSDInfo) {
        SAFE_LOCALFREE(absoluteSDInfo);
    }
    if (selfRelativeSDInfo) {
        SAFE_LOCALFREE(selfRelativeSDInfo);
    }
}

SecDesc& SecDesc::operator=(const SecDesc &other) {
    if (this != &other) {
        ownerInfo = other.ownerInfo;
        primaryGroupInfo = other.primaryGroupInfo;
        daclInfoSz = other.daclInfoSz;
        daclAbsInfoSz = other.daclAbsInfoSz;
        saclInfoSz = other.saclInfoSz;
        saclAbsInfoSz = other.saclAbsInfoSz;
        ownerInfoSz = other.ownerInfoSz;
        primaryGroupInfoSz = other.primaryGroupInfoSz;
        absoluteSDInfoSz = other.absoluteSDInfoSz;
        selfRelativeSDInfoSz = other.selfRelativeSDInfoSz;
        if (other.daclInfo) {
            daclInfo = ::LocalAlloc(LPTR, other.daclInfoSz);
            if (daclInfo) {
                memcpy(daclInfo, other.daclInfo, other.daclInfoSz);
            }
        } else {
            if (daclInfo && daclInfoSz) {
                SAFE_LOCALFREE(daclInfo);
            } else {
                daclInfo = 0;
            }
        }
        if (other.daclAbsInfo) {
            daclAbsInfo = ::LocalAlloc(LPTR, other.daclAbsInfoSz);
            if (daclAbsInfo) {
                memcpy(daclAbsInfo, other.daclAbsInfo, other.daclAbsInfoSz);
            }
        } else {
            if (daclAbsInfo && daclAbsInfoSz) {
                SAFE_LOCALFREE(daclAbsInfo);
            } else {
                daclAbsInfo = 0;
            }
        }
        if (other.saclInfo) {
            saclInfo = ::LocalAlloc(LPTR, other.saclInfoSz);
            if (saclInfo) {
                memcpy(saclInfo, other.saclInfo, other.saclInfoSz);
            }
        } else {
            if (saclInfo && saclInfoSz) {
                SAFE_LOCALFREE(saclInfo);
            } else {
                saclInfo = 0;
            }
        }
        if (other.saclAbsInfo) {
            saclAbsInfo = ::LocalAlloc(LPTR, other.saclAbsInfoSz);
            if (saclAbsInfo) {
                memcpy(saclAbsInfo, other.saclAbsInfo, other.saclAbsInfoSz);
            }
        } else {
            if (saclAbsInfo && saclAbsInfoSz) {
                SAFE_LOCALFREE(saclAbsInfo);
            } else {
                saclAbsInfo = 0;
            }
        }
        if (other.absoluteSDInfo) {
            absoluteSDInfo = ::LocalAlloc(LPTR, other.absoluteSDInfoSz);
            if (absoluteSDInfo) {
                memcpy(absoluteSDInfo, other.absoluteSDInfo, other.absoluteSDInfoSz);
            }
        } else {
            if (absoluteSDInfo && absoluteSDInfoSz) {
                SAFE_LOCALFREE(absoluteSDInfo);
            } else {
                absoluteSDInfo = 0;
            }
        }
        if (other.selfRelativeSDInfo) {
            selfRelativeSDInfo = ::LocalAlloc(LPTR, other.selfRelativeSDInfoSz);
            if (selfRelativeSDInfo) {
                memcpy(selfRelativeSDInfo, other.selfRelativeSDInfo, other.selfRelativeSDInfoSz);
            }
        } else {
            if (selfRelativeSDInfo && selfRelativeSDInfoSz) {
                SAFE_LOCALFREE(selfRelativeSDInfo);
            } else {
                selfRelativeSDInfo = 0;
            }
        }
    }
    return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
SecDesc& SecDesc::operator=(SecDesc &&other) noexcept {
    if (this != &other) {
        daclInfoSz = std::exchange(other.daclInfoSz, 0);
        daclAbsInfoSz = std::exchange(other.daclAbsInfoSz, 0);
        saclInfoSz = std::exchange(other.saclInfoSz, 0);
        saclAbsInfoSz = std::exchange(other.saclAbsInfoSz, 0);
        ownerInfoSz = std::exchange(other.ownerInfoSz, 0);
        primaryGroupInfoSz = std::exchange(other.primaryGroupInfoSz, 0);
        absoluteSDInfoSz = std::exchange(other.absoluteSDInfoSz, 0);
        selfRelativeSDInfoSz = std::exchange(other.selfRelativeSDInfoSz, 0);
        ownerInfo = std::move(other.ownerInfo);
        primaryGroupInfo = std::move(other.primaryGroupInfo);
        daclInfo = std::move(other.daclInfo);
        other.daclInfo = 0;
        daclAbsInfo = std::move(other.daclAbsInfo);
        other.daclAbsInfo = 0;
        saclInfo = std::move(other.saclInfo);
        other.saclInfo = 0;
        saclAbsInfo = std::move(other.saclAbsInfo);
        other.saclAbsInfo = 0;
        absoluteSDInfo = std::move(other.absoluteSDInfo);
        other.absoluteSDInfo = 0;
        selfRelativeSDInfo = std::move(other.selfRelativeSDInfo);
        other.selfRelativeSDInfo = 0;
    }
    return *this;
}
#endif

bool SecDesc::operator==(const SecDesc& other) const {
    bool bufeq = false, otheq = false, sideq = false;
    if (daclInfo && other.daclInfo) {
        if (daclInfoSz && other.daclInfoSz) {
            bufeq = !memcmp(daclInfo, other.daclInfo, other.daclInfoSz);
        } else {
            bufeq = false;
        }
    } else {
        if (daclInfo && !other.daclInfo) {
            bufeq = false;
        } else if (!daclInfo && other.daclInfo) {
            bufeq = false;
        } else if (!daclInfo && !other.daclInfo) {
            bufeq = true;
        }
    }
    if (daclAbsInfo && other.daclAbsInfo) {
        if (daclAbsInfoSz && other.daclAbsInfoSz) {
            bufeq &= !memcmp(daclAbsInfo, other.daclAbsInfo, other.daclAbsInfoSz);
        } else {
            bufeq &= false;
        }
    } else {
        if (daclAbsInfo && !other.daclAbsInfo) {
            bufeq &= false;
        } else if (!daclAbsInfo && other.daclAbsInfo) {
            bufeq &= false;
        } else if (!daclAbsInfo && !other.daclAbsInfo) {
            bufeq &= true;
        }
    }
    if (saclInfo && other.saclInfo) {
        if (saclInfoSz && other.saclInfoSz) {
            bufeq &= !memcmp(saclInfo, other.saclInfo, other.saclInfoSz);
        } else {
            bufeq &= false;
        }
    } else {
        if (saclInfo && !other.saclInfo) {
            bufeq &= false;
        } else if (!saclInfo && other.saclInfo) {
            bufeq &= false;
        } else if (!saclInfo && !other.saclInfo) {
            bufeq &= true;
        }
    }
    if (saclAbsInfo && other.saclAbsInfo) {
        if (saclAbsInfoSz && other.saclAbsInfoSz) {
            bufeq &= !memcmp(saclAbsInfo, other.saclAbsInfo, other.saclAbsInfoSz);
        } else {
            bufeq &= false;
        }
    } else {
        if (saclAbsInfo && !other.saclAbsInfo) {
            bufeq &= false;
        } else if (!saclAbsInfo && other.saclAbsInfo) {
            bufeq &= false;
        } else if (!saclAbsInfo && !other.saclAbsInfo) {
            bufeq &= true;
        }
    }
    if (absoluteSDInfo && other.absoluteSDInfo) {
        if (absoluteSDInfoSz && other.absoluteSDInfoSz) {
            bufeq &= !memcmp(absoluteSDInfo, other.absoluteSDInfo, other.absoluteSDInfoSz);
        } else {
            bufeq &= false;
        }
    } else {
        if (absoluteSDInfo && !other.absoluteSDInfo) {
            bufeq &= false;
        } else if (!absoluteSDInfo && other.absoluteSDInfo) {
            bufeq &= false;
        } else if (!absoluteSDInfo && !other.absoluteSDInfo) {
            bufeq &= true;
        }
    }
    if (selfRelativeSDInfo && other.selfRelativeSDInfo) {
        if (selfRelativeSDInfoSz && other.selfRelativeSDInfoSz) {
            bufeq &= !memcmp(selfRelativeSDInfo, other.selfRelativeSDInfo, other.selfRelativeSDInfoSz);
        } else {
            bufeq &= false;
        }
    } else {
        if (selfRelativeSDInfo && !other.selfRelativeSDInfo) {
            bufeq &= false;
        } else if (!selfRelativeSDInfo && other.selfRelativeSDInfo) {
            bufeq &= false;
        } else if (!selfRelativeSDInfo && !other.selfRelativeSDInfo) {
            bufeq &= true;
        }
    }
    otheq = (daclInfoSz == other.daclInfoSz &&
        saclInfoSz == other.saclInfoSz &&
        ownerInfoSz == other.ownerInfoSz &&
        primaryGroupInfoSz == other.primaryGroupInfoSz &&
        absoluteSDInfoSz == other.absoluteSDInfoSz &&
        selfRelativeSDInfoSz == other.selfRelativeSDInfoSz &&
        lower_copy(ownerInfo) == lower_copy(other.ownerInfo) &&
        lower_copy(primaryGroupInfo) == lower_copy(other.primaryGroupInfo));
    return (bufeq && otheq);
}

bool SecDesc::operator!=(const SecDesc& other) const {
    bool bufneq = false, othneq = false, sidneq = false;
    if (daclInfo && other.daclInfo) {
        if (daclInfoSz && other.daclInfoSz) {
            bufneq = memcmp(daclInfo, other.daclInfo, other.daclInfoSz);
        } else {
            bufneq = true;
        }
    } else {
        if (daclInfo && !other.daclInfo) {
            bufneq = true;
        } else if (!daclInfo && other.daclInfo) {
            bufneq = true;
        } else if (!daclInfo && !other.daclInfo) {
            bufneq = false;
        }
    }
    if (daclAbsInfo && other.daclAbsInfo) {
        if (daclAbsInfoSz && other.daclAbsInfoSz) {
            bufneq |= memcmp(daclAbsInfo, other.daclAbsInfo, other.daclAbsInfoSz);
        } else {
            bufneq |= true;
        }
    } else {
        if (daclAbsInfo && !other.daclAbsInfo) {
            bufneq |= true;
        } else if (!daclAbsInfo && other.daclAbsInfo) {
            bufneq |= true;
        } else if (!daclAbsInfo && !other.daclAbsInfo) {
            bufneq |= false;
        }
    }
    if (saclInfo && other.saclInfo) {
        if (saclInfoSz && other.saclInfoSz) {
            bufneq |= memcmp(saclInfo, other.saclInfo, other.saclInfoSz);
        } else {
            bufneq |= true;
        }
    } else {
        if (saclInfo && !other.saclInfo) {
            bufneq |= true;
        } else if (!saclInfo && other.saclInfo) {
            bufneq |= true;
        } else if (!saclInfo && !other.saclInfo) {
            bufneq |= false;
        }
    }
    if (saclAbsInfo && other.saclAbsInfo) {
        if (saclAbsInfoSz && other.saclAbsInfoSz) {
            bufneq |= memcmp(saclAbsInfo, other.saclAbsInfo, other.saclAbsInfoSz);
        } else {
            bufneq |= true;
        }
    } else {
        if (saclAbsInfo && !other.saclAbsInfo) {
            bufneq |= true;
        } else if (!saclAbsInfo && other.saclAbsInfo) {
            bufneq |= true;
        } else if (!saclAbsInfo && !other.saclAbsInfo) {
            bufneq |= false;
        }
    }
    if (absoluteSDInfo && other.absoluteSDInfo) {
        if (absoluteSDInfoSz && other.absoluteSDInfoSz) {
            bufneq |= memcmp(absoluteSDInfo, other.absoluteSDInfo, other.absoluteSDInfoSz);
        } else {
            bufneq |= true;
        }
    } else {
        if (absoluteSDInfo && !other.absoluteSDInfo) {
            bufneq |= true;
        } else if (!absoluteSDInfo && other.absoluteSDInfo) {
            bufneq |= true;
        } else if (!absoluteSDInfo && !other.absoluteSDInfo) {
            bufneq |= false;
        }
    }
    if (selfRelativeSDInfo && other.selfRelativeSDInfo) {
        if (selfRelativeSDInfoSz && other.selfRelativeSDInfoSz) {
            bufneq |= memcmp(selfRelativeSDInfo, other.selfRelativeSDInfo, other.selfRelativeSDInfoSz);
        } else {
            bufneq |= true;
        }
    } else {
        if (selfRelativeSDInfo && !other.selfRelativeSDInfo) {
            bufneq |= true;
        } else if (!selfRelativeSDInfo && other.selfRelativeSDInfo) {
            bufneq |= true;
        } else if (!selfRelativeSDInfo && !other.selfRelativeSDInfo) {
            bufneq |= false;
        }
    }
    othneq = (daclInfoSz != other.daclInfoSz ||
        saclInfoSz != other.saclInfoSz ||
        ownerInfoSz != other.ownerInfoSz ||
        primaryGroupInfoSz != other.primaryGroupInfoSz ||
        absoluteSDInfoSz != other.absoluteSDInfoSz ||
        selfRelativeSDInfoSz != other.selfRelativeSDInfoSz ||
        lower_copy(ownerInfo) != lower_copy(other.ownerInfo) ||
        lower_copy(primaryGroupInfo) != lower_copy(other.primaryGroupInfo));
    return (bufneq || othneq);
}

ACLHandler::ACLHandler() {}

ACLHandler::~ACLHandler() {}

ACLOpResult ACLHandler::SecurityDescriptor2StringSecurityDescriptor(unsigned char* &secDesc,
	std::wstring &textSecDesc, SecInfo secInfo) const  {
	wchar_t* buf = 0;
	if (::ConvertSecurityDescriptorToStringSecurityDescriptor((::SECURITY_DESCRIPTOR*)secDesc,
		SDDL_REVISION_1, static_cast<unsigned long>(secInfo), &buf, 0)) {
		textSecDesc = buf;
        SAFE_LOCALFREE(buf);
		return ACLOpResult::Success;
	}
	return ACLOpResult::Fail;
}

ACLOpResult ACLHandler::StringSecurityDescriptor2SecurityDescriptor(const std::wstring textSecDesc,
    unsigned char* &secDesc, unsigned long &secDeszSz) const {
    PSECURITY_DESCRIPTOR sd = 0;
    if (::ConvertStringSecurityDescriptorToSecurityDescriptor(textSecDesc.c_str(),
        SDDL_REVISION_1, &sd, &secDeszSz)) {
        if (!secDesc) {
            NEW_ARR_NULLIFY_NO_REDEFINE(secDesc, unsigned char, secDeszSz);
            if (secDesc) {
                memcpy(secDesc, sd, secDeszSz);
                SAFE_LOCALFREE(sd);
                return ACLOpResult::Success;
            } else {
                SAFE_LOCALFREE(sd);
                return ACLOpResult::Fail;
            }
        } else {
            memcpy(secDesc, sd, secDeszSz);
            SAFE_LOCALFREE(sd);
            return ACLOpResult::Success;
        }
    }
    return ACLOpResult::Fail;
}

ACLOpResult ACLHandler::DACLReadAllowed(bool &allowed, const ::ACL* testACL, const ::PSID sid) const {
    return DACLPermissionGetter(allowed, testACL, sid, READ_CONTROL);
}

ACLOpResult ACLHandler::DACLWriteAllowed(bool &allowed, const ::ACL* testACL, const ::PSID sid) const {
    return DACLPermissionGetter(allowed, testACL, sid, WRITE_DAC);
}

ACLOpResult ACLHandler::DACLExecuteAllowed(bool &allowed, const ::ACL* testACL, const ::PSID sid) const {
    return DACLPermissionGetter(allowed, testACL, sid, GENERIC_EXECUTE);
}

ACLOpResult ACLHandler::DACLDeleteAllowed(bool &allowed, const ::ACL* testACL, const ::PSID sid) const {
    return DACLPermissionGetter(allowed, testACL, sid, DELETE);
}

ACLOpResult ACLHandler::DACLFullControlAllowed(bool& allowed, const ::ACL* testACL, const ::PSID sid) const {
    return DACLPermissionGetter(allowed, testACL, sid, FILE_ALL_ACCESS);
}

ACLOpResult ACLHandler::DACLGetSIDsByAceType(::ACL* dacl, AceType reqAceType, std::vector<PERMISSION> &permissions,
    const std::wstring machineName) const {
    void* testace = 0;
    ::ACE_HEADER* vace = 0;
    SysHandler sys;
    for (size_t i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        vace = (::ACE_HEADER*)testace;
        if (vace->AceType == static_cast<unsigned char>(reqAceType)) {
            std::wstring twsid = sys.StrSIDFromSID((::PSID)&((::ACCESS_ALLOWED_ACE*)testace)->SidStart);
            permissions.emplace_back(std::pair<std::wstring, std::wstring>(
                sys.GetAccountNameFromStrSID(twsid, machineName), twsid), (AceType)vace->AceType);
        }
    }
    return ACLOpResult::Success;
}

ACLOpResult ACLHandler::DACLGetPermissionBySID(::ACL* dacl, ::PSID sid, std::vector<PERMISSION> &permissions,
    const bool includeGroups, const std::wstring machineName) const {
    void* testace = 0;
    ::PSID accsid = 0;
    ::ACCESS_ALLOWED_ACE* aceAllowed = 0;
    SysHandler sys;
    std::vector<GroupDesc> groups;
    if (includeGroups) {
        if (SysOpResult::Success != sys.LocalGroupListFromStrSID(groups, sys.StrSIDFromSID(sid))) {
            return ACLOpResult::Fail;
        }
    }
    for (size_t i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
        accsid = (::PSID)&aceAllowed->SidStart;
        if (includeGroups) {
            for (size_t j = 0; j < groups.size(); ++j) {
                ::PSID tpsid = sys.SIDFromStrSid(groups[j].GroupStrSid);
                if (::EqualSid(accsid, tpsid)) {
                    /*PERMISSION perm;
                    perm.first.first = sys.GetAccountNameFromStrSID(groups[j].GroupStrSid, machineName);
                    perm.first.second = groups[j].GroupStrSid;
                    perm.second = (AceType)vace->AceType;*/
                    permissions.emplace_back(std::pair<std::wstring, std::wstring>(
                        sys.GetAccountNameFromStrSID(groups[j].GroupStrSid, machineName), groups[j].GroupStrSid),
                        (AceType)vace->AceType);
                }
                SAFE_LOCALFREE(tpsid);
            }
        }
        if (::EqualSid(accsid, sid)) {
            std::wstring twsid = sys.StrSIDFromSID(sid);
            permissions.emplace_back(std::pair<std::wstring, std::wstring>(
                sys.GetAccountNameFromStrSID(twsid, machineName), twsid), (AceType)vace->AceType);
        }
    }
    return ACLOpResult::Success;
}

ACLOpResult ACLHandler::DACLGetPermissionMaskBySID(::ACL* dacl, ::PSID sid, std::vector<PERMISSIONFULL> &permissions,
    const bool includeGroups, const std::wstring machineName) const {
    void* testace = 0;
    ::PSID accsid = 0;
    ::ACCESS_ALLOWED_ACE* aceAllowed = 0;
    SysHandler sys;
    std::vector<GroupDesc> groups;
    if (includeGroups) {
        if (SysOpResult::Success != sys.LocalGroupListFromStrSID(groups, sys.StrSIDFromSID(sid))) {
            return ACLOpResult::Fail;
        }
    }
    for (size_t i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
        accsid = (::PSID)&aceAllowed->SidStart;
        if (includeGroups) {
            for (size_t j = 0; j < groups.size(); ++j) {
                ::PSID tpsid = sys.SIDFromStrSid(groups[j].GroupStrSid);
                if (::EqualSid(accsid, tpsid)) {
                    /*PERMISSION perm;
                    perm.first.first = sys.GetAccountNameFromStrSID(groups[j].GroupStrSid, machineName);
                    perm.first.second = groups[j].GroupStrSid;
                    perm.second = (AceType)vace->AceType;*/
                    permissions.emplace_back(std::pair<std::wstring, std::wstring>(
                        sys.GetAccountNameFromStrSID(groups[j].GroupStrSid, machineName), groups[j].GroupStrSid),
                        std::pair<AceType, unsigned long>((AceType)vace->AceType, aceAllowed->Mask));
                }
                SAFE_LOCALFREE(tpsid);
            }
        }
        if (::EqualSid(accsid, sid)) {
            std::wstring twsid = sys.StrSIDFromSID(sid);
            permissions.emplace_back(std::pair<std::wstring, std::wstring>(
                sys.GetAccountNameFromStrSID(twsid, machineName), twsid), std::pair<AceType, unsigned long>(
                (AceType)vace->AceType, aceAllowed->Mask));
        }
    }
    return ACLOpResult::Success;
}

ACLOpResult ACLHandler::DACLFromSecurityDescriptor(::SECURITY_DESCRIPTOR* secDesc, ::ACL* &dacl) const {
    int daclPresent = 0, daclDefaulted = 0;
    if (::GetSecurityDescriptorDacl(secDesc, &daclPresent, &dacl, &daclDefaulted)) {
        if (daclPresent && dacl) {
            return ACLOpResult::Success;
        } else {
            if (!daclPresent) {
                if (dacl) {
                    SAFE_LOCALFREE(dacl);
                }
                return ACLOpResult::Success;
            } else {
                if (dacl) {
                    SAFE_LOCALFREE(dacl);
                }
                return ACLOpResult::Fail;
            }
        }
    } else {
        return ACLOpResult::Fail;
    }
}

ACLOpResult ACLHandler::SACLFromSecurityDescriptor(::SECURITY_DESCRIPTOR* secDesc, ::ACL* &sacl) const {
    int saclPresent = 0, saclDefaulted = 0;
    if (::GetSecurityDescriptorSacl(secDesc, &saclPresent, &sacl, &saclDefaulted)) {
        if (saclPresent && sacl) {
            return ACLOpResult::Success;
        } else {
            if (!saclPresent) {
                if (sacl) {
                    SAFE_LOCALFREE(sacl);
                }
                return ACLOpResult::Success;
            } else {
                if (sacl) {
                    SAFE_LOCALFREE(sacl);
                }
                return ACLOpResult::Fail;
            }
        }
    } else {
        return ACLOpResult::Fail;
    }
}

ACLOpResult ACLHandler::OwnerSIDFromSecurityDescriptor(::SECURITY_DESCRIPTOR* secDesc, ::PSID &sid) const {
    int defaulted = 0;
    if (!::GetSecurityDescriptorOwner(secDesc, &sid, &defaulted)) {
        return ACLOpResult::Fail;
    }
    return ACLOpResult::Success;
}

ACLOpResult ACLHandler::PrimaryGroupSIDFromSecurityDescriptor(::SECURITY_DESCRIPTOR* secDesc, ::PSID &sid) const {
    int defaulted = 0;
    if (!::GetSecurityDescriptorGroup(secDesc, &sid, &defaulted)) {
        return ACLOpResult::Fail;
    }
    return ACLOpResult::Success;
}

ACLOpResult ACLHandler::CreateAbsoluteSecDesc(SecDesc &secDesc) const {
    if (!secDesc.absoluteSDInfo) {
        secDesc.absoluteSDInfo = ::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
        if (!secDesc.absoluteSDInfo) {
            return ACLOpResult::Fail;
        }
        secDesc.absoluteSDInfoSz = SECURITY_DESCRIPTOR_MIN_LENGTH;
    }
    unsigned long ownerinfosz = secDesc.ownerInfoSz, primgroupinfosz = secDesc.primaryGroupInfoSz;
    int defaulted = 0;
    ::PSID townersid = 0, tprimgroupsid = 0;
    SysHandler sys;
    ::SECURITY_DESCRIPTOR* ownerSecDesc = (::SECURITY_DESCRIPTOR*)::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    if (!ownerSecDesc) {
        SAFE_LOCALFREE(secDesc.absoluteSDInfo);
        return ACLOpResult::Fail;
    }
    if (!::InitializeSecurityDescriptor(ownerSecDesc, SECURITY_DESCRIPTOR_REVISION)) {
        SAFE_LOCALFREE(ownerSecDesc);
        SAFE_LOCALFREE(secDesc.absoluteSDInfo);
        return ACLOpResult::Fail;
    }
    if (secDesc.ownerInfo.length()) {
        townersid = sys.SIDFromStrSid(secDesc.ownerInfo);
        if (!townersid) {
            SAFE_LOCALFREE(ownerSecDesc);
            SAFE_LOCALFREE(secDesc.absoluteSDInfo);
            return ACLOpResult::Fail;
        }
        if (!::SetSecurityDescriptorOwner(ownerSecDesc, townersid, defaulted)) {
            SAFE_LOCALFREE(secDesc.absoluteSDInfo);
            return ACLOpResult::Fail;
        }
    }
    ::SECURITY_DESCRIPTOR* primaryGroupSecDesc = (::SECURITY_DESCRIPTOR*)::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    if (!primaryGroupSecDesc) {
        SAFE_LOCALFREE(ownerSecDesc);
        SAFE_LOCALFREE(secDesc.absoluteSDInfo);
        return ACLOpResult::Fail;
    }
    if (!::InitializeSecurityDescriptor(primaryGroupSecDesc, SECURITY_DESCRIPTOR_REVISION)) {
        SAFE_LOCALFREE(ownerSecDesc);
        SAFE_LOCALFREE(secDesc.absoluteSDInfo);
        return ACLOpResult::Fail;
    }
    if (secDesc.primaryGroupInfo.length()) {
        tprimgroupsid = sys.SIDFromStrSid(secDesc.primaryGroupInfo);
        if (!tprimgroupsid) {
            SAFE_LOCALFREE(ownerSecDesc);
            SAFE_LOCALFREE(primaryGroupSecDesc);
            SAFE_LOCALFREE(secDesc.absoluteSDInfo);
            return ACLOpResult::Fail;
        }
        if (!::SetSecurityDescriptorGroup(primaryGroupSecDesc, tprimgroupsid, defaulted)) {
            SAFE_LOCALFREE(ownerSecDesc);
            SAFE_LOCALFREE(primaryGroupSecDesc);
            SAFE_LOCALFREE(secDesc.absoluteSDInfo);
            return ACLOpResult::Fail;
        }
    }
    if (::MakeAbsoluteSD((::SECURITY_DESCRIPTOR*)secDesc.daclInfo, (::SECURITY_DESCRIPTOR*)secDesc.absoluteSDInfo,
        &secDesc.absoluteSDInfoSz, (::ACL*)secDesc.daclAbsInfo, &secDesc.daclAbsInfoSz, (::ACL*)secDesc.saclAbsInfo,
        &secDesc.saclAbsInfoSz, ownerSecDesc, &ownerinfosz, primaryGroupSecDesc, &primgroupinfosz)) {
        return ACLOpResult::Success;
    } else {
        if (ERROR_INSUFFICIENT_BUFFER == getLastErrorCode()) {
            if (secDesc.daclAbsInfoSz) {
                secDesc.daclAbsInfo = ::LocalAlloc(LPTR, secDesc.daclAbsInfoSz);
                if (!secDesc.daclAbsInfo) {
                    SAFE_LOCALFREE(ownerSecDesc);
                    SAFE_LOCALFREE(primaryGroupSecDesc);
                    SAFE_LOCALFREE(secDesc.absoluteSDInfo);
                    return ACLOpResult::Fail;
                }
            } else {
                secDesc.daclAbsInfo = 0;
            }
            if (secDesc.saclAbsInfoSz) {
                secDesc.saclAbsInfo = ::LocalAlloc(LPTR, secDesc.saclAbsInfoSz);
                if (!secDesc.saclAbsInfo) {
                    SAFE_LOCALFREE(ownerSecDesc);
                    SAFE_LOCALFREE(primaryGroupSecDesc);
                    SAFE_LOCALFREE(secDesc.absoluteSDInfo);
                    SAFE_LOCALFREE(secDesc.daclAbsInfo);
                    return ACLOpResult::Fail;
                }
            } else {
                secDesc.saclAbsInfo = 0;
            }
            void* absSDinfoBuf = LocalAlloc(LPTR, secDesc.absoluteSDInfoSz);
            if (!absSDinfoBuf) {
                return ACLOpResult::Fail;
            }
            if (secDesc.absoluteSDInfo) {
                SAFE_LOCALFREE(secDesc.absoluteSDInfo);
            }
            secDesc.absoluteSDInfo = ::LocalAlloc(LPTR, secDesc.absoluteSDInfoSz);
            if (!secDesc.absoluteSDInfo) {
                return ACLOpResult::Fail;
            }
            if (::MakeAbsoluteSD((::SECURITY_DESCRIPTOR*)secDesc.daclInfo, (::SECURITY_DESCRIPTOR*)absSDinfoBuf,
                &secDesc.absoluteSDInfoSz, (::ACL*)secDesc.daclAbsInfo, &secDesc.daclAbsInfoSz,
                (::ACL*)secDesc.saclAbsInfo, &secDesc.saclAbsInfoSz, ownerSecDesc, &ownerinfosz,
                primaryGroupSecDesc, &primgroupinfosz)) {
                memcpy(secDesc.absoluteSDInfo, absSDinfoBuf, secDesc.absoluteSDInfoSz);
                if (::SetSecurityDescriptorOwner(secDesc.absoluteSDInfo, townersid, false)) {
                    if (::SetSecurityDescriptorGroup(secDesc.absoluteSDInfo, tprimgroupsid, false)) {
                        if (::MakeSelfRelativeSD(secDesc.absoluteSDInfo,
                            secDesc.selfRelativeSDInfo, &secDesc.selfRelativeSDInfoSz)) {
                            return ACLOpResult::Success;
                        } else {
                            if (ERROR_INSUFFICIENT_BUFFER == getLastErrorCode()) {
                                secDesc.selfRelativeSDInfo = ::LocalAlloc(LPTR, secDesc.selfRelativeSDInfoSz);
                                if (!secDesc.selfRelativeSDInfo) {
                                    SAFE_LOCALFREE(ownerSecDesc);
                                    SAFE_LOCALFREE(primaryGroupSecDesc);
                                    SAFE_LOCALFREE(secDesc.absoluteSDInfo);
                                    SAFE_LOCALFREE(secDesc.daclAbsInfo);
                                    SAFE_LOCALFREE(secDesc.saclAbsInfo);
                                    return ACLOpResult::Fail;
                                }
                                if (::MakeSelfRelativeSD(secDesc.absoluteSDInfo, secDesc.selfRelativeSDInfo,
                                    &secDesc.selfRelativeSDInfoSz)) {
                                    SAFE_LOCALFREE(ownerSecDesc);
                                    SAFE_LOCALFREE(primaryGroupSecDesc);
                                    return ACLOpResult::Success;
                                } else {
                                    SAFE_LOCALFREE(ownerSecDesc);
                                    SAFE_LOCALFREE(primaryGroupSecDesc);
                                    SAFE_LOCALFREE(secDesc.absoluteSDInfo);
                                    SAFE_LOCALFREE(secDesc.daclAbsInfo);
                                    SAFE_LOCALFREE(secDesc.saclAbsInfo);
                                    if(secDesc.selfRelativeSDInfo) {
                                        SAFE_LOCALFREE(secDesc.selfRelativeSDInfo);
                                    }
                                    return ACLOpResult::Fail;
                                }
                            }
                        }
                    } else {
                        SAFE_LOCALFREE(ownerSecDesc);
                        SAFE_LOCALFREE(primaryGroupSecDesc);
                        SAFE_LOCALFREE(secDesc.absoluteSDInfo);
                        SAFE_LOCALFREE(secDesc.daclAbsInfo);
                        SAFE_LOCALFREE(secDesc.saclAbsInfo);
                        return ACLOpResult::Fail;
                    }
                } else {
                    SAFE_LOCALFREE(ownerSecDesc);
                    SAFE_LOCALFREE(primaryGroupSecDesc);
                    SAFE_LOCALFREE(secDesc.absoluteSDInfo);
                    SAFE_LOCALFREE(secDesc.daclAbsInfo);
                    SAFE_LOCALFREE(secDesc.saclAbsInfo);
                    return ACLOpResult::Fail;
                }
            }
        }
    }
    return ACLOpResult::Fail;
}

ACLOpResult ACLHandler::DACLRegAddFullControlAllowed(::ACL* &dacl, const ::PSID sid,
    const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, KEY_ALL_ACCESS);
}

ACLOpResult ACLHandler::DACLRegAddQueryValueAllowed(::ACL* &dacl, const ::PSID sid,
    const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, KEY_QUERY_VALUE);
}

ACLOpResult ACLHandler::DACLRegAddSetValueAllowed(::ACL* &dacl, const ::PSID sid,
    const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, KEY_SET_VALUE);
}

ACLOpResult ACLHandler::DACLRegAddCreateSubkeyAllowed(::ACL* &dacl, const ::PSID sid,
    const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, KEY_CREATE_SUB_KEY);
}

ACLOpResult ACLHandler::DACLRegAddEnumSubkeyAllowed(::ACL* &dacl, const ::PSID sid,
    const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, KEY_ENUMERATE_SUB_KEYS);
}

ACLOpResult ACLHandler::DACLRegAddNotifyAllowed(::ACL* &dacl, const ::PSID sid,
    const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, KEY_NOTIFY);
}

ACLOpResult ACLHandler::DACLRegAddCreateLinkAllowed(::ACL* &dacl, const ::PSID sid,
    const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, KEY_CREATE_LINK);
}

ACLOpResult ACLHandler::DACLAddCustomAllowedACE(::ACL* &dacl, const ::PSID sid, const unsigned long aclMask,
    const unsigned char aceFlags, const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, aclMask, aceFlags);
}

ACLOpResult ACLHandler::DACLAddCustomDeniedACE(::ACL* &dacl, const ::PSID sid, unsigned long aclMask,
    const unsigned char aceFlags) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, aclMask, aceFlags);
}

ACLOpResult ACLHandler::DACLAddCustomACE(::ACL* &dacl, const ::PSID sid, const AceType aceTyoe, unsigned long aclMask,
    const unsigned char aceFlags, const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    if (AceType::AccessAllowed == aceTyoe) {
        return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, aclMask, aceFlags);
    } else if (AceType::AccessDenied == aceTyoe) {
        return DACLDenyPermissionSetter(dacl, sid, aclMask, aceFlags);
    } else {
        return ACLOpResult::Fail;
    }
}

ACLOpResult ACLHandler::DACLAddFullControlAllowed(::ACL* &dacl, const ::PSID sid,
    const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, FILE_ALL_ACCESS);
}

ACLOpResult ACLHandler::DACLAddDeleteAllowed(::ACL* &dacl, ::PSID sid, const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, DELETE);
}

ACLOpResult ACLHandler::DACLAddExecuteAllowed(::ACL* &dacl, const ::PSID sid, const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, GENERIC_EXECUTE);
}

ACLOpResult ACLHandler::DACLAddWriteAllowed(::ACL* &dacl, ::PSID sid, const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, WRITE_OWNER | WRITE_DAC | GENERIC_WRITE);
}

ACLOpResult ACLHandler::DACLAddReadAllowed(::ACL* &dacl, ::PSID sid, const bool removeExistingBan) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLAllowPermissionSetter(dacl, sid, removeExistingBan, READ_CONTROL | GENERIC_READ);
}

ACLOpResult ACLHandler::DACLAddReadDenied(::ACL* &dacl, ::PSID sid) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, READ_CONTROL | GENERIC_READ);
}

ACLOpResult ACLHandler::DACLAddWriteDenied(::ACL* &dacl, ::PSID sid) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, WRITE_OWNER | WRITE_DAC | GENERIC_WRITE);
}

ACLOpResult ACLHandler::DACLAddExecuteDenied(::ACL*& dacl, const ::PSID sid) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, GENERIC_EXECUTE);
}

ACLOpResult ACLHandler::DACLAddDeleteDenied(::ACL*& dacl, ::PSID sid) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, DELETE);
}

ACLOpResult ACLHandler::DACLAddFullControlDenied(::ACL* &dacl, const ::PSID sid) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, FILE_ALL_ACCESS);
}

ACLOpResult ACLHandler::DACLRegAddCreateLinkDenied(::ACL*& dacl, const ::PSID sid) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, KEY_CREATE_LINK);
}

ACLOpResult ACLHandler::DACLRegAddNotifyDenied(::ACL*& dacl, const ::PSID sid) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, KEY_NOTIFY);
}

ACLOpResult ACLHandler::DACLRegAddEnumSubkeyDenied(::ACL*& dacl, const ::PSID sid) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, KEY_ENUMERATE_SUB_KEYS);
}

ACLOpResult ACLHandler::DACLRegAddCreateSubkeyDenied(::ACL*& dacl, const ::PSID sid) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, KEY_CREATE_SUB_KEY);
}

ACLOpResult ACLHandler::DACLRegAddSetValueDenied(::ACL*& dacl, const ::PSID sid) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, KEY_SET_VALUE);
}

ACLOpResult ACLHandler::DACLRegAddQueryValueDenied(::ACL*& dacl, const ::PSID sid) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, KEY_QUERY_VALUE);
}

ACLOpResult ACLHandler::DACLRegAddFullControlDenied(::ACL*& dacl, const ::PSID sid) const {
    if (!dacl) {
        return ACLOpResult::Fail;
    }
    return DACLDenyPermissionSetter(dacl, sid, KEY_ALL_ACCESS);
}

// Sequence of ACE's must be:
//      not inherited, denied
//      not inherited, allowed
//      inherited, denied
//      inherited, allowed
ACLOpResult ACLHandler::DACLRemoveACESIDTypeMaskFlags(::ACL* &dacl, const ::PSID sid, const AceType aceType,
    const unsigned long aclMask, const unsigned char aclFlags, const bool includeGroups) const {
    if (!dacl->AceCount || !dacl) {
        return ACLOpResult::Fail;
    }
    size_t i = 0, j = 0;
    bool sideq = false;
    SysHandler sys;
    std::vector<GroupDesc> groups;
    std::vector<::PSID> groupsids;
    if (includeGroups) {
        if (SysOpResult::Success != sys.LocalGroupListFromStrSID(groups, sys.StrSIDFromSID(sid))) {
            return ACLOpResult::Fail;
        }
        for (i = 0; i < groups.size(); ++i) {
            ::PSID tpsid = sys.SIDFromStrSid(groups[i].GroupStrSid);
            if (tpsid) {
                groupsids.push_back(tpsid);
            }
        }
    }
    void* testace = 0;
    ::ACL* newList = 0;
    unsigned long aclSz = sizeof(::ACL);
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessAllowed == aceType &&
                        (aceAllowed->Mask & aclMask) && (vace->AceFlags & aclFlags)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (::EqualSid(accsid, sid) && AceType::AccessAllowed == aceType && (aceAllowed->Mask & aclMask) &&
                !(vace->AceFlags & aclFlags)) {
                aclSz += sizeof(::ACCESS_ALLOWED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            } else if (::EqualSid(accsid, sid) && AceType::AccessAllowed == aceType && !(aceAllowed->Mask & aclMask)) {
                aclSz += sizeof(::ACCESS_ALLOWED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            } else if (::EqualSid(accsid, sid) && AceType::AccessAllowed != aceType) {
                aclSz += sizeof(::ACCESS_ALLOWED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            } else if (!::EqualSid(accsid, sid)) {
                aclSz += sizeof(::ACCESS_ALLOWED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            }
        } else if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessDenied == aceType &&
                        (aceDenied->Mask & aclMask) && (vace->AceFlags & aclFlags)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (::EqualSid(accsid, sid) && AceType::AccessAllowed == aceType && (aceDenied->Mask & aclMask) &&
                !(vace->AceFlags & aclFlags)) {
                aclSz += sizeof(::ACCESS_DENIED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            } else if (::EqualSid(accsid, sid) && AceType::AccessDenied == aceType && !(aceDenied->Mask & aclMask)) {
                aclSz += sizeof(::ACCESS_DENIED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            } else if (::EqualSid(accsid, sid) && AceType::AccessDenied != aceType) {
                aclSz += sizeof(::ACCESS_DENIED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            } else if (!::EqualSid(accsid, sid)) {
                aclSz += sizeof(::ACCESS_DENIED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    aclSz = (aclSz + (sizeof(unsigned long) - 1)) & 0xfffffffc;
    newList = (::ACL*)LocalAlloc(LPTR, aclSz);
    if (!newList) {
        return ACLOpResult::Fail;
    }
    if (!::InitializeAcl(newList, aclSz, ACL_REVISION)) {
        return ACLOpResult::Fail;
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessDenied == aceType &&
                        (aceDenied->Mask & aclMask) && (vace->AceFlags & aclFlags)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (!(vace->AceFlags & INHERITED_ACE)) {
                if (::EqualSid(accsid, sid) && AceType::AccessDenied == aceType && (aceDenied->Mask & aclMask) &&
                    !(vace->AceFlags & aclFlags)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (::EqualSid(accsid, sid) && AceType::AccessDenied == aceType && !(aceDenied->Mask & aclMask)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (::EqualSid(accsid, sid) && AceType::AccessDenied != aceType) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    unsigned long aceCount = newList->AceCount + 1;
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessAllowed == aceType &&
                        (aceAllowed->Mask & aclMask) && (vace->AceFlags & aclFlags)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (!(vace->AceFlags & INHERITED_ACE)) {
                if (::EqualSid(accsid, sid) && AceType::AccessDenied == aceType && (aceAllowed->Mask & aclMask) &&
                    !(vace->AceFlags & aclFlags)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (::EqualSid(accsid, sid) && AceType::AccessAllowed == aceType && !(aceAllowed->Mask & aclMask)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (::EqualSid(accsid, sid) && AceType::AccessAllowed != aceType) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessDenied == aceType &&
                        (aceDenied->Mask & aclMask) && (vace->AceFlags & aclFlags)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (vace->AceFlags & INHERITED_ACE) {
                if (::EqualSid(accsid, sid) && AceType::AccessDenied == aceType && (aceDenied->Mask & aclMask) &&
                    !(vace->AceFlags & aclFlags)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (::EqualSid(accsid, sid) && AceType::AccessDenied == aceType && !(aceDenied->Mask & aclMask)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (::EqualSid(accsid, sid) && AceType::AccessDenied != aceType) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    aceCount = newList->AceCount + 1;
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessAllowed == aceType &&
                        (aceAllowed->Mask & aclMask) && (vace->AceFlags & aclFlags)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (vace->AceFlags & INHERITED_ACE) {
                if (::EqualSid(accsid, sid) && AceType::AccessDenied == aceType && (aceAllowed->Mask & aclMask) &&
                    !(vace->AceFlags & aclFlags)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (::EqualSid(accsid, sid) && AceType::AccessAllowed == aceType && !(aceAllowed->Mask & aclMask)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (::EqualSid(accsid, sid) && AceType::AccessAllowed != aceType) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    ::ACL_SIZE_INFORMATION asi;
    if (::GetAclInformation(newList, &asi, sizeof(asi), AclSizeInformation)) {
        newList->AclSize = (unsigned short)asi.AclBytesInUse;
    } else {
        return ACLOpResult::Fail;
    }
    for (i = 0; i < groupsids.size(); ++i) {
        SAFE_LOCALFREE(groupsids[i]);
    }
#if defined(_WIN64)
    ::LocalFree(&dacl);
#else if defined(_WIN32)
    ::LocalFree(dacl);
#endif
    dacl = newList;
    return ACLOpResult::Success;
}

// Sequence of ACE's must be:
//      not inherited, denied
//      not inherited, allowed
//      inherited, denied
//      inherited, allowed
ACLOpResult ACLHandler::DACLRemoveACESIDTypeMask(::ACL* &dacl, const ::PSID sid, const AceType aceType,
    const unsigned long aclMask, const bool includeGroups) const {
    if (!dacl->AceCount || !dacl) {
        return ACLOpResult::Fail;
    }
    size_t i = 0, j = 0;
    bool sideq = false;
    SysHandler sys;
    std::vector<GroupDesc> groups;
    std::vector<::PSID> groupsids;
    if (includeGroups) {
        if (SysOpResult::Success != sys.LocalGroupListFromStrSID(groups, sys.StrSIDFromSID(sid))) {
            return ACLOpResult::Fail;
        }
        for (i = 0; i < groups.size(); ++i) {
            ::PSID tpsid = sys.SIDFromStrSid(groups[i].GroupStrSid);
            if (tpsid) {
                groupsids.push_back(tpsid);
            }
        }
    }
    void* testace = 0;
    ::ACL* newList = 0;
    unsigned long aclSz = sizeof(::ACL);
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessAllowed == aceType &&
                        (aceAllowed->Mask & aclMask)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (::EqualSid(accsid, sid) && AceType::AccessAllowed == aceType && !(aceAllowed->Mask & aclMask)) {
                aclSz += sizeof(::ACCESS_ALLOWED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            } else if (::EqualSid(accsid, sid) && AceType::AccessAllowed != aceType) {
                aclSz += sizeof(::ACCESS_ALLOWED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            } else if (!::EqualSid(accsid, sid)) {
                aclSz += sizeof(::ACCESS_ALLOWED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            }
        } else if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessDenied == aceType &&
                        (aceDenied->Mask & aclMask)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (::EqualSid(accsid, sid) && AceType::AccessDenied == aceType && !(aceDenied->Mask & aclMask)) {
                aclSz += sizeof(::ACCESS_DENIED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            } else if (::EqualSid(accsid, sid) && AceType::AccessDenied != aceType) {
                aclSz += sizeof(::ACCESS_DENIED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            } else if (!::EqualSid(accsid, sid)) {
                aclSz += sizeof(::ACCESS_DENIED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    aclSz = (aclSz + (sizeof(unsigned long) - 1)) & 0xfffffffc;
    newList = (::ACL*)LocalAlloc(LPTR, aclSz);
    if (!newList) {
        return ACLOpResult::Fail;
    }
    if (!::InitializeAcl(newList, aclSz, ACL_REVISION)) {
        return ACLOpResult::Fail;
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessDenied == aceType &&
                        (aceDenied->Mask & aclMask)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (!(vace->AceFlags & INHERITED_ACE)) {
                if (::EqualSid(accsid, sid) && AceType::AccessDenied == aceType && !(aceDenied->Mask & aclMask)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (::EqualSid(accsid, sid) && AceType::AccessDenied != aceType) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    unsigned long aceCount = newList->AceCount + 1;
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessAllowed == aceType &&
                        (aceAllowed->Mask & aclMask)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (!(vace->AceFlags & INHERITED_ACE)) {
                if (::EqualSid(accsid, sid) && AceType::AccessAllowed == aceType && !(aceAllowed->Mask & aclMask)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (::EqualSid(accsid, sid) && AceType::AccessAllowed != aceType) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessDenied == aceType &&
                        (aceDenied->Mask & aclMask)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (vace->AceFlags & INHERITED_ACE) {
                if (::EqualSid(accsid, sid) && AceType::AccessDenied == aceType && !(aceDenied->Mask & aclMask)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (::EqualSid(accsid, sid) && AceType::AccessDenied != aceType) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    aceCount = newList->AceCount + 1;
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessAllowed == aceType &&
                        (aceAllowed->Mask & aclMask)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (vace->AceFlags & INHERITED_ACE) {
                if (::EqualSid(accsid, sid) && AceType::AccessAllowed == aceType && !(aceAllowed->Mask & aclMask)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (::EqualSid(accsid, sid) && AceType::AccessAllowed != aceType) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    ::ACL_SIZE_INFORMATION asi;
    if (::GetAclInformation(newList, &asi, sizeof(asi), AclSizeInformation)) {
        newList->AclSize = (unsigned short)asi.AclBytesInUse;
    } else {
        return ACLOpResult::Fail;
    }
    for (i = 0; i < groupsids.size(); ++i) {
        SAFE_LOCALFREE(groupsids[i]);
    }
#if defined(_WIN64)
    ::LocalFree(&dacl);
#else if defined(_WIN32)
    ::LocalFree(dacl);
#endif
    dacl = newList;
    return ACLOpResult::Success;
}

// Sequence of ACE's must be:
//      not inherited, denied
//      not inherited, allowed
//      inherited, denied
//      inherited, allowed
ACLOpResult ACLHandler::DACLRemoveACESIDType(::ACL* &dacl, const ::PSID sid, const AceType aceType,
    const bool includeGroups) const {
    if (!dacl->AceCount || !dacl) {
        return ACLOpResult::Fail;
    }
    size_t i = 0, j = 0;
    bool sideq = false;
    SysHandler sys;
    std::vector<GroupDesc> groups;
    std::vector<::PSID> groupsids;
    if (includeGroups) {
        if (SysOpResult::Success != sys.LocalGroupListFromStrSID(groups, sys.StrSIDFromSID(sid))) {
            return ACLOpResult::Fail;
        }
        for (i = 0; i < groups.size(); ++i) {
            ::PSID tpsid = sys.SIDFromStrSid(groups[i].GroupStrSid);
            if (tpsid) {
                groupsids.push_back(tpsid);
            }
        }
    }
    void* testace = 0;
    ::ACL* newList = 0;
    unsigned long aclSz = sizeof(::ACL);
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessAllowed == aceType) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (::EqualSid(accsid, sid) && AceType::AccessAllowed != aceType) {
                aclSz += sizeof(::ACCESS_ALLOWED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            } else if (!::EqualSid(accsid, sid)) {
                aclSz += sizeof(::ACCESS_ALLOWED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            }
        } else if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessDenied == aceType) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (::EqualSid(accsid, sid) && AceType::AccessDenied != aceType) {
                aclSz += sizeof(::ACCESS_DENIED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            } else if (!::EqualSid(accsid, sid)) {
                aclSz += sizeof(::ACCESS_DENIED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    aclSz = (aclSz + (sizeof(unsigned long) - 1)) & 0xfffffffc;
    newList = (::ACL*)LocalAlloc(LPTR, aclSz);
    if (!newList) {
        return ACLOpResult::Fail;
    }
    if (!::InitializeAcl(newList, aclSz, ACL_REVISION)) {
        return ACLOpResult::Fail;
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessDenied == aceType) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (!(vace->AceFlags & INHERITED_ACE)) {
                if (::EqualSid(accsid, sid) && AceType::AccessDenied != aceType) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if(!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    unsigned long aceCount = newList->AceCount + 1;
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessAllowed == aceType) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (!(vace->AceFlags & INHERITED_ACE)) {
                if (::EqualSid(accsid, sid) && AceType::AccessAllowed != aceType) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessDenied == aceType) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (vace->AceFlags & INHERITED_ACE) {
                if (::EqualSid(accsid, sid) && AceType::AccessDenied != aceType) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    aceCount = newList->AceCount + 1;
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j]) && AceType::AccessAllowed == aceType) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (vace->AceFlags & INHERITED_ACE) {
                if (::EqualSid(accsid, sid) && AceType::AccessAllowed != aceType) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                } else if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    ::ACL_SIZE_INFORMATION asi;
    if (::GetAclInformation(newList, &asi, sizeof(asi), AclSizeInformation)) {
        newList->AclSize = (unsigned short)asi.AclBytesInUse;
    } else {
        return ACLOpResult::Fail;
    }
    for (i = 0; i < groupsids.size(); ++i) {
        SAFE_LOCALFREE(groupsids[i]);
    }
#if defined(_WIN64)
    ::LocalFree(&dacl);
#else if defined(_WIN32)
    ::LocalFree(dacl);
#endif
    dacl = newList;
    return ACLOpResult::Success;
}

// Sequence of ACE's must be:
//      not inherited, denied
//      not inherited, allowed
//      inherited, denied
//      inherited, allowed
ACLOpResult ACLHandler::DACLRemoveACESID(::ACL* &dacl, const ::PSID sid, const bool includeGroups) const {
    if (!dacl->AceCount || !dacl) {
        return ACLOpResult::Fail;
    }
    size_t i = 0, j = 0;
    SysHandler sys;
    std::vector<GroupDesc> groups;
    std::vector<::PSID> groupsids;
    if (includeGroups) {
        if (SysOpResult::Success != sys.LocalGroupListFromStrSID(groups, sys.StrSIDFromSID(sid))) {
            return ACLOpResult::Fail;
        }
        for (i = 0; i < groups.size(); ++i) {
            ::PSID tpsid = sys.SIDFromStrSid(groups[i].GroupStrSid);
            if (tpsid) {
                groupsids.push_back(tpsid);
            }
        }
    }
    void* testace = 0;
    ::ACL* newList = 0;
    unsigned long aclSz = sizeof(::ACL);
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j])) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (!::EqualSid(accsid, sid)) {
                aclSz += sizeof(::ACCESS_ALLOWED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            }
        } else if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j])) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (!::EqualSid(accsid, sid)) {
                aclSz += sizeof(::ACCESS_DENIED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    aclSz = (aclSz + (sizeof(unsigned long) - 1)) & 0xfffffffc;
    newList = (::ACL*)LocalAlloc(LPTR, aclSz);
    if (!newList) {
        return ACLOpResult::Fail;
    }
    if (!::InitializeAcl(newList, aclSz, ACL_REVISION)) {
        return ACLOpResult::Fail;
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j])) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if(!(vace->AceFlags & INHERITED_ACE)) {
                if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, 0, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    unsigned long aceCount = newList->AceCount + 1;
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowedd = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowedd->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j])) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (!(vace->AceFlags & INHERITED_ACE)) {
                if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j])) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (vace->AceFlags & INHERITED_ACE) {
                if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, 0, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    aceCount = newList->AceCount + 1;
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowedd = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowedd->SidStart;
            if (includeGroups && groupsids.size()) {
                bool skip = false;
                for (j = 0; j < groupsids.size(); ++j) {
                    if (::EqualSid(accsid, groupsids[j])) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
            }
            if (vace->AceFlags & INHERITED_ACE) {
                if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    ::ACL_SIZE_INFORMATION asi;
    if (::GetAclInformation(newList, &asi, sizeof(asi), AclSizeInformation)) {
        newList->AclSize = (unsigned short)asi.AclBytesInUse;
    } else {
        return ACLOpResult::Fail;
    }
    for (i = 0; i < groupsids.size(); ++i) {
        SAFE_LOCALFREE(groupsids[i]);
    }
#if defined(_WIN64)
    ::LocalFree(&dacl);
#else if defined(_WIN32)
    ::LocalFree(dacl);
#endif
    dacl = newList;
    return ACLOpResult::Success;
}

ACLOpResult ACLHandler::DACL2AbsoluteSD(::SECURITY_DESCRIPTOR* secDesc, ::ACL* dacl) const {
    if (::SetSecurityDescriptorDacl(secDesc, true, dacl, false)) {
        return ACLOpResult::Success;
    } else {
        return ACLOpResult::Fail;
    }
}

ACLOpResult ACLHandler::SACL2AbsoluteSD(::SECURITY_DESCRIPTOR* secDesc, ::ACL* sacl) const {
    if (SetSecurityDescriptorSacl(secDesc, true, sacl, false)) {
        return ACLOpResult::Success;
    } else {
        return ACLOpResult::Fail;
    }
}

ACLOpResult ACLHandler::BuildACE(::ACE_HEADER* &ace, const ::PSID sid, const AceType aceType,
    const ACCESS_MASK accessMask, const unsigned char aceFlags) const {
    unsigned long aceLen = 0;
    unsigned long sidLen = GetLengthSid(sid);
    if (AceType::AccessAllowed == aceType) {
        aceLen = sizeof(::ACCESS_ALLOWED_ACE) - sizeof(unsigned long) + sidLen;
        ::ACCESS_ALLOWED_ACE* newace = (::ACCESS_ALLOWED_ACE*)LocalAlloc(LPTR, aceLen);
        if (newace) {
            memset(newace, 0, aceLen);
            newace->Header.AceType = static_cast<unsigned char>(aceType);
            newace->Header.AceFlags = aceFlags;
            newace->Header.AceSize = aceLen;
            newace->Mask = accessMask;
            memcpy(&newace->SidStart, sid, sidLen);
            ace = (::ACE_HEADER*)newace;
            return ACLOpResult::Success;
        }
    } else if (AceType::AccessDenied == aceType) {
        aceLen = sizeof(::ACCESS_DENIED_ACE) - sizeof(unsigned long) + sidLen;
        ::ACCESS_DENIED_ACE* newace = (::ACCESS_DENIED_ACE*)LocalAlloc(LPTR, aceLen);
        if (newace) {
            memset(newace, 0, aceLen);
            newace->Header.AceType = static_cast<unsigned char>(aceType);
            newace->Header.AceFlags = aceFlags;
            newace->Header.AceSize = aceLen;
            newace->Mask = accessMask;
            memcpy(&newace->SidStart, sid, sidLen);
            ace = (::ACE_HEADER*)newace;
            return ACLOpResult::Success;
        }
    }
    return ACLOpResult::Fail;
}

// Sequence of ACE's must be:
//      not inherited, denied
//      not inherited, allowed
//      inherited, denied
//      inherited, allowed
ACLOpResult ACLHandler::DACLAllowPermissionSetter(::ACL*& dacl, const ::PSID sid, const bool removeExistingBan,
    const unsigned long aclMask, const unsigned char aclFlags) const {
    if (removeExistingBan) {
        if (ACLOpResult::Success != DACLRemoveACESID(dacl, sid, false)) {
            return ACLOpResult::Fail;
        }
    }
    ::ACE_HEADER* newace = 0;
    if (ACLOpResult::Success != BuildACE(newace, sid, AceType::AccessAllowed, aclMask, aclFlags)) {
        return ACLOpResult::Fail;
    }
    size_t i = 0, j = 0;
    void* testace = 0;
    ::ACL* newList = 0;
    unsigned long aclSz = sizeof(::ACL);
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            aclSz += sizeof(::ACCESS_ALLOWED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
        } else  if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            aclSz += sizeof(::ACCESS_DENIED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
        }
#ifdef _WIN64
        LocalFree(&testace);
#endif
    }
    aclSz = newace->AceSize + (aclSz + (sizeof(unsigned long) - 1)) & 0xfffffffc;
    newList = (::ACL*)::LocalAlloc(LPTR, aclSz);
    if (!newList) {
        return ACLOpResult::Fail;
    }
    if (!::InitializeAcl(newList, aclSz, ACL_REVISION)) {
        return ACLOpResult::Fail;
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (!(vace->AceFlags & INHERITED_ACE)) {
                if (!::AddAce(newList, ACL_REVISION, 0, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                    return ACLOpResult::Fail;
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, newace, newace->AceSize)) {
        return ACLOpResult::Fail;
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowedd = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowedd->SidStart;
            if (!(vace->AceFlags & INHERITED_ACE)) {
                if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (vace->AceFlags & INHERITED_ACE) {
                if (!::AddAce(newList, ACL_REVISION, 0, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                    return ACLOpResult::Fail;
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    // aceCount = newList->AceCount + 1;
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowedd = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowedd->SidStart;
            if (vace->AceFlags & INHERITED_ACE) {
                if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                    return ACLOpResult::Fail;
                }
            }
        }
#ifdef _WIN64
        LocalFree(&testace);
#endif
    }
    ::ACL_SIZE_INFORMATION asi;
    if (::GetAclInformation(newList, &asi, sizeof(asi), AclSizeInformation)) {
        newList->AclSize = (unsigned short)asi.AclBytesInUse;
    } else {
        return ACLOpResult::Fail;
    }
#if defined(_WIN64)
    ::LocalFree(&dacl);
#else if defined(_WIN32)
    ::LocalFree(dacl);
#endif
    dacl = newList;
    return ACLOpResult::Success;
}

// Sequence of ACE's must be:
//      not inherited, denied
//      not inherited, allowed
//      inherited, denied
//      inherited, allowed
ACLOpResult ACLHandler::DACLDenyPermissionSetter(::ACL* &dacl, const ::PSID sid, const unsigned long aclMask,
    const unsigned char aclFlags) const {
    ::ACE_HEADER* newace = 0;
    if (ACLOpResult::Success != BuildACE(newace, sid, AceType::AccessDenied, aclMask, aclFlags)) {
        return ACLOpResult::Fail;
    }
    size_t i = 0, j = 0;
    void* testace = 0;
    ::ACL* newList = 0;
    unsigned long aclSz = sizeof(::ACL);
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            aclSz += sizeof(::ACCESS_ALLOWED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
        } else if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            aclSz += sizeof(::ACCESS_DENIED_ACE) + (GetLengthSid(accsid) - sizeof(unsigned long));
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    aclSz = newace->AceSize + (aclSz + (sizeof(unsigned long) - 1)) & 0xfffffffc;
    newList = (::ACL*)LocalAlloc(LPTR, aclSz);
    if (!newList) {
        return ACLOpResult::Fail;
    }
    if (!::InitializeAcl(newList, aclSz, ACL_REVISION)) {
        return ACLOpResult::Fail;
    }
    if (!::AddAce(newList, ACL_REVISION, 0, newace, newace->AceSize)) {
        return ACLOpResult::Fail;
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (!(vace->AceFlags & INHERITED_ACE)) {
                if (!::AddAce(newList, ACL_REVISION, 0, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                    return ACLOpResult::Fail;
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowedd = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowedd->SidStart;
            if (!(vace->AceFlags & INHERITED_ACE)) {
                if (!::EqualSid(accsid, sid)) {
                    if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                        return ACLOpResult::Fail;
                    }
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            if (vace->AceFlags & INHERITED_ACE) {
                if (!::AddAce(newList, ACL_REVISION, 0, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                    return ACLOpResult::Fail;
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    // aceCount = newList->AceCount + 1;
    for (i = 0; i < dacl->AceCount; ++i) {
        if (!::GetAce(dacl, i, (void**)&testace)) {
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            ::ACCESS_ALLOWED_ACE* aceAllowedd = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowedd->SidStart;
            if (vace->AceFlags & INHERITED_ACE) {
                if (!::AddAce(newList, ACL_REVISION, MAXDWORD, testace, ((::ACE_HEADER*)testace)->AceSize)) {
                    return ACLOpResult::Fail;
                }
            }
        }
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    ::ACL_SIZE_INFORMATION asi;
    if (::GetAclInformation(newList, &asi, sizeof(asi), AclSizeInformation)) {
        newList->AclSize = (unsigned short)asi.AclBytesInUse;
    } else {
        return ACLOpResult::Fail;
    }
#if defined(_WIN64)
    ::LocalFree(&dacl);
#else if defined(_WIN32)
    ::LocalFree(dacl);
#endif
    dacl = newList;
    return ACLOpResult::Success;
}

ACLOpResult ACLHandler::DACLPermissionGetter(bool &allowed, const ::ACL* testACL, const ::PSID sid,
    const unsigned long mask, const bool checkGroups) const {
    void* testace = 0;
    SysHandler sys;
    SidType specsidype;
    if (SysOpResult::Success != sys.GetSIDType(sid, specsidype)) {
        return ACLOpResult::Fail;
    }
    for (size_t i = 0; i < testACL->AceCount; ++i) {
        if (!::GetAce((::ACL*)testACL, i, (void**)&testace)) {
#ifdef _WIN64
            ::LocalFree(&testace);
#endif
            return ACLOpResult::Fail;
        }
        ::ACE_HEADER* vace = (::ACE_HEADER*)testace;
        if (vace->AceType == ACCESS_ALLOWED_ACE_TYPE) {
            std::cout << "(" << i << ")\t Access allowed ACE" << std::endl;
            ::ACCESS_ALLOWED_ACE* aceAllowed = (::ACCESS_ALLOWED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceAllowed->SidStart;
            SidType sidtype;
            if (SysOpResult::Success != sys.GetSIDType(accsid, sidtype)) {
#ifdef _WIN64
                ::LocalFree(&testace);
#endif
                continue;
            }
            if (SidType::User == specsidype) {
                if (SidType::User == sidtype) {
                    if (::EqualSid(accsid, sid)) {
                        if (mask & aceAllowed->Mask) {
                            allowed = true;
                        }
                    }
                } else if (SidType::Group == sidtype) {
                    if (checkGroups) {
                        bool ismember = false;
                        if (SysOpResult::Success == sys.IsAccountMemberOfGroup(accsid, sid, ismember)) {
                            if (ismember) {
                                if (mask & aceAllowed->Mask) {
                                    allowed = true;
                                }
                            }
                        } else {
                            return ACLOpResult::Fail;
                        }
                    }
                } else {
                    if (::EqualSid(accsid, sid)) {
                        if (mask & aceAllowed->Mask) {
                            allowed = true;
                        }
                    }
                }
            } else if (SidType::Group == specsidype) {
                if (SidType::User == sidtype) {
                    if (checkGroups) {
                        bool ismember = false;
                        if (SysOpResult::Success == sys.IsAccountMemberOfGroup(sid, accsid, ismember)) {
                            if (ismember) {
                                if (mask & aceAllowed->Mask) {
                                    allowed = true;
                                }
                            }
                        } else {
                            return ACLOpResult::Fail;
                        }
                    }
                } else if (SidType::Group == sidtype) {
                    if (::EqualSid(accsid, sid)) {
                        if (mask & aceAllowed->Mask) {
                            allowed = true;
                        }
                    }
                } else {
                    if (::EqualSid(accsid, sid)) {
                        if (mask & aceAllowed->Mask) {
                            allowed = true;
                        }
                    }
                }
            } else {
                if (::EqualSid(accsid, sid)) {
                    if (mask & aceAllowed->Mask) {
                        allowed = true;
                    }
                }
            }
        } else  if (vace->AceType == ACCESS_DENIED_ACE_TYPE) {
            std::cout << "(" << i << ")\t Access denied ACE" << std::endl;
            ::ACCESS_DENIED_ACE* aceDenied = (::ACCESS_DENIED_ACE*)testace;
            ::PSID accsid = (::PSID)&aceDenied->SidStart;
            SidType sidtype;
            if (SysOpResult::Success != sys.GetSIDType(accsid, sidtype)) {
#ifdef _WIN64
                ::LocalFree(&testace);
#endif
                continue;
            }
            if (SidType::User == specsidype) {
                if (SidType::User == sidtype) {
                    if (::EqualSid(accsid, sid)) {
                        if (mask & aceDenied->Mask) {
                            allowed = false;
                        }
                    }
                } else if (SidType::Group == sidtype) {
                    if (checkGroups) {
                        bool ismember = false;
                        if (SysOpResult::Success == sys.IsAccountMemberOfGroup(accsid, sid, ismember)) {
                            if (ismember) {
                                if (mask & aceDenied->Mask) {
                                    allowed = false;
                                }
                            }
                        } else {
                            return ACLOpResult::Fail;
                        }
                    }
                } else {
                    if (::EqualSid(accsid, sid)) {
                        if (mask & aceDenied->Mask) {
                            allowed = false;
                        }
                    }
                }
            } else if (SidType::Group == specsidype) {
                if (SidType::User == sidtype) {
                    if (checkGroups) {
                        bool ismember = false;
                        if (SysOpResult::Success == sys.IsAccountMemberOfGroup(sid, accsid, ismember)) {
                            if (ismember) {
                                if (mask & aceDenied->Mask) {
                                    allowed = false;
                                }
                            }
                        } else {
                            return ACLOpResult::Fail;
                        }
                    }
                } else if (SidType::Group == sidtype) {
                    if (::EqualSid(accsid, sid)) {
                        if (mask & aceDenied->Mask) {
                            allowed = false;
                        }
                    }
                } else {
                    if (::EqualSid(accsid, sid)) {
                        if (mask & aceDenied->Mask) {
                            allowed = false;
                        }
                    }
                }
            } else {
                if (::EqualSid(accsid, sid)) {
                    if (mask & aceDenied->Mask) {
                        allowed = false;
                    }
                }
            }
        } else if (vace->AceType == SYSTEM_AUDIT_ACE_TYPE) {
        } else if (vace->AceType == SYSTEM_ALARM_ACE_TYPE) {
        } else {
            std::cout << "(" << i << ")\t Undefined ACE" << std::endl;
        }
        vace = 0;
#ifdef _WIN64
        ::LocalFree(&testace);
#endif
    }
    return ACLOpResult::Success;
}