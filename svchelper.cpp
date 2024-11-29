#include "svchelper.h"
#include "winerrhelper.h"

SvcRunState::SvcRunState() {
	svcRunning = false;
	svcProcRunning = false;
}

SvcRunState::SvcRunState(const SvcRunState& other) {
	if (this != &other) {
		svcRunning = other.svcRunning;
		svcProcRunning = other.svcProcRunning;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
SvcRunState::SvcRunState(SvcRunState &&other) noexcept {
	if (this != &other) {
		svcRunning = std::exchange(other.svcRunning, false);
		svcProcRunning = std::exchange(other.svcProcRunning, false);
	}
}
#endif

SvcRunState::~SvcRunState() {}

SvcRunState& SvcRunState::operator=(const SvcRunState &other) {
	if (this != &other) {
		svcRunning = other.svcRunning;
		svcProcRunning = other.svcProcRunning;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
SvcRunState& SvcRunState::operator=(SvcRunState &&other) noexcept {
	if (this != &other) {
		svcRunning = std::exchange(other.svcRunning, false);
		svcProcRunning = std::exchange(other.svcProcRunning, false);
	}
	return *this;
}
#endif

bool SvcRunState::operator==(const SvcRunState& other) const {
	if (this != &other) {
		return (svcRunning == other.svcRunning &&
				svcProcRunning == other.svcProcRunning);
	} else {
		return true;
	}
}

bool SvcRunState::operator!=(const SvcRunState& other) const {
	if (this != &other) {
		return (svcRunning != other.svcRunning ||
				svcProcRunning != other.svcProcRunning);
	} else {
		return false;
	}
}

SvcTriggerData::SvcTriggerData() {
	triggerDataSize = 0;
	triggerDataType = 0;
}

SvcTriggerData::SvcTriggerData(const unsigned short triggerdatasize,
	const unsigned long triggerdatatype,
	std::vector<unsigned char> triggerdata) {
	triggerDataSize = triggerdatasize;
	triggerDataType = triggerdatatype;
	triggerData = triggerdata;
}

SvcTriggerData::SvcTriggerData(const SvcTriggerData &other) {
	if (this != &other) {
		triggerDataSize = other.triggerDataSize;
		triggerDataType = other.triggerDataType;
		triggerData = other.triggerData;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
SvcTriggerData::SvcTriggerData(SvcTriggerData &&other) noexcept {
	if (this != &other) {
		triggerDataSize = std::exchange(other.triggerDataSize, 0);
		triggerDataType = std::exchange(other.triggerDataType, 0);
		triggerData = std::move(other.triggerData);
	}
}
#endif

SvcTriggerData::~SvcTriggerData() {}

SvcTriggerData& SvcTriggerData::operator=(const SvcTriggerData& other) {
	if (this != &other) {
		triggerDataSize = other.triggerDataSize;
		triggerDataType = other.triggerDataType;
		triggerData = other.triggerData;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
SvcTriggerData& SvcTriggerData::operator=(SvcTriggerData&& other) noexcept {
	if (this != &other) {
		triggerDataSize = std::exchange(other.triggerDataSize, 0);
		triggerDataType = std::exchange(other.triggerDataType, 0);
		triggerData = std::move(other.triggerData);
	}
	return *this;
}
#endif

bool SvcTriggerData::operator==(const SvcTriggerData& other) const {
	if (this != &other) {
		std::string origstr = std::string(triggerData.begin(), triggerData.end());
		std::string otherstr = std::string(other.triggerData.begin(), other.triggerData.end());
		return (triggerDataSize == other.triggerDataSize &&
			triggerDataType == other.triggerDataType &&
			(origstr == otherstr &&
				lower_copy(origstr) == lower_copy(otherstr)));
	} else {
		return true;
	}
}
bool SvcTriggerData::operator!=(const SvcTriggerData& other) const {
	if (this != &other) {
		std::string origstr = std::string(triggerData.begin(), triggerData.end());
		std::string otherstr = std::string(other.triggerData.begin(), other.triggerData.end());
		return (triggerDataSize != other.triggerDataSize ||
			triggerDataType != other.triggerDataType ||
			(origstr != otherstr ||
				lower_copy(origstr) != lower_copy(otherstr)));
	} else {
		return false;
	}
}

SvcTrigger::SvcTrigger() {
	triggerType = 0;
	triggerAction = 0;
}

SvcTrigger::SvcTrigger(const unsigned long triggertype,
	const unsigned long triggeraction,
	const std::wstring triggerguid,
	std::vector <SvcTriggerData> trigdata) {
	triggerType = triggertype;
	triggerAction = triggeraction;
	triggerGuid = triggerguid;
	triggerData = trigdata;
}

SvcTrigger::SvcTrigger(const SvcTrigger &other) {
	if (this != &other) {
		triggerType = other.triggerType;
		triggerAction = other.triggerAction;
		triggerGuid = other.triggerGuid;
		triggerData = other.triggerData;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
SvcTrigger::SvcTrigger(SvcTrigger &&other) noexcept {
	if (this != &other) {
		triggerType = std::exchange(other.triggerType, 0);
		triggerAction = std::exchange(other.triggerAction, 0);
		triggerGuid = std::move(other.triggerGuid);
		triggerData = std::move(other.triggerData);
	}
}
#endif

SvcTrigger::~SvcTrigger() {}

SvcTrigger& SvcTrigger::operator=(const SvcTrigger &other) {
	if (this != &other) {
		triggerType = other.triggerType;
		triggerAction = other.triggerAction;
		triggerGuid = other.triggerGuid;
		triggerData = other.triggerData;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
SvcTrigger& SvcTrigger::operator=(SvcTrigger &&other) noexcept {
	if (this != &other) {
		if (this != &other) {
			triggerType = std::exchange(other.triggerType, 0);
			triggerAction = std::exchange(other.triggerAction, 0);
			triggerGuid = std::move(other.triggerGuid);
			triggerData = std::move(other.triggerData);
		}
	}
	return *this;
}
#endif

bool SvcTrigger::operator==(const SvcTrigger &other) const {
	if (this != &other) {
		return (triggerType == other.triggerType &&
				triggerAction == other.triggerAction &&
				lower_copy(triggerGuid) == lower_copy(other.triggerGuid) &&
				triggerData == other.triggerData);
	} else {
		return true;
	}
}

bool SvcTrigger::operator!=(const SvcTrigger &other) const {
	if (this != &other) {
		return (triggerType != other.triggerType ||
				triggerAction != other.triggerAction ||
				lower_copy(triggerGuid) != lower_copy(other.triggerGuid) ||
				triggerData != other.triggerData);
	} else {
		return false;
	}
}

SvcRecord::SvcRecord() {
	serviceType = 0;
	startType = 0;
	errorControl = 0;
	tagId = 0;
	failResetPeriod = 0;
	failActions = 0;
	failActionDelay = 0;
	preshutdownTimeout = 0;
	svcSidType = 0;
	launchProtected = 0;
	preferredNode = 0;
	preferredNodeDeleteFlag = false;
	delayedAutoStart = false;
	failureActionsOnNonCrashFailures = false;
	failActionType = SC_ACTION_NONE;
	memset(&svcProcDetail, 0, sizeof(SERVICE_STATUS_PROCESS));
}

SvcRecord::SvcRecord(const std::wstring svcname,
	const std::wstring svcdisplayname,
	const std::wstring svcdesc,
	const std::wstring binarypath,
	const std::wstring startsfrom,
	const std::wstring loadordergroup,
	const std::wstring failresetmsg,
	const std::wstring failcommand,
	const std::wstring reqprivileges,
	const unsigned long servicetype,
	const unsigned long starttype,
	const unsigned long errorcontrol,
	const unsigned long tagid,
	const unsigned long failresetperiod,
	const unsigned long failactions,
	const unsigned long failactiondelay,
	const unsigned long preshutdowntimeout,
	const unsigned long svcsidtype,
	const unsigned long launchprotected,
	const unsigned short preferrednode,
	const bool preferrednodedeleteflag,
	const bool failureactionsonnoncrashfailures,
	const bool delayedautostart) {
	svcName = svcname;
	svcDisplayName = svcdisplayname;
	svcDisplayName = svcdesc;
	binaryPath = binarypath;
	startsFrom = startsfrom;
	loadOrderGroup = loadordergroup;
	failResetMsg = failresetmsg;
	failCommand = failcommand;
	privileges = reqprivileges;
	serviceType = servicetype;
	startType = starttype;
	errorControl = errorcontrol;
	tagId = tagid;
	failResetPeriod = failresetperiod;
	failActions = failactions;
	preshutdownTimeout = preshutdowntimeout;
	svcSidType = svcsidtype;
	launchProtected = launchprotected;
	preferredNode = preferrednode;
	preferredNodeDeleteFlag = preferrednodedeleteflag;
	failureActionsOnNonCrashFailures = failureactionsonnoncrashfailures;
	delayedAutoStart = delayedautostart;
	failActionDelay = failactiondelay;
	failActionType = SC_ACTION_NONE;
	memset(&svcProcDetail, 0, sizeof(SERVICE_STATUS_PROCESS));
}

SvcRecord::SvcRecord(const std::wstring svcname,
	const std::wstring svcdisplayname,
	const std::wstring svcdesc,
	const std::wstring binarypath,
	const std::wstring startsfrom,
	const std::wstring loadordergroup,
	const std::wstring failresetmsg,
	const std::wstring failcommand,
	const std::wstring reqprivileges,
	const unsigned long servicetype,
	const unsigned long starttype,
	const unsigned long errorcontrol,
	const unsigned long tagid,
	const unsigned long failresetperiod,
	const unsigned long failactions,
	const unsigned long failactiondelay,
	const unsigned long preshutdowntimeout,
	const unsigned long svcsidtype,
	const unsigned long launchprotected,
	const unsigned short preferrednode,
	const bool preferrednodedeleteflag,
	const bool delayedautostart,
	const bool failureactionsonnoncrashfailures,
	const std::vector<std::wstring> deps,
	const std::vector<SvcTrigger> trgs,
	const SC_ACTION_TYPE failctiontype,
	const SERVICE_STATUS_PROCESS svcprocdetial) {
	svcName = svcname;
	svcDisplayName = svcdisplayname;
	svcDisplayName = svcdesc;
	binaryPath = binarypath;
	startsFrom = startsfrom;
	loadOrderGroup = loadordergroup;
	failResetMsg = failresetmsg;
	failCommand = failcommand;
	privileges = reqprivileges;
	serviceType = servicetype;
	startType = starttype;
	errorControl = errorcontrol;
	tagId = tagid;
	failResetPeriod = failresetperiod;
	failActions = failactions;
	failActionDelay = failactiondelay;
	preshutdownTimeout = preshutdowntimeout;
	svcSidType = svcsidtype;
	launchProtected = launchprotected;
	preferredNode = preferrednode;
	preferredNodeDeleteFlag = preferrednodedeleteflag;
	delayedAutoStart = delayedautostart;
	failureActionsOnNonCrashFailures = failureactionsonnoncrashfailures;
	dependencies = deps;
	triggers = trgs;
	failActionType = failctiontype;
	svcProcDetail = svcprocdetial;
}

SvcRecord::SvcRecord(const SvcRecord &other) {
	if (this != &other) {
		svcName = other.svcName;
		svcDisplayName = other.svcDisplayName;
		svcDesc = other.svcDesc;
		binaryPath = other.binaryPath;
		startsFrom = other.startsFrom;
		dependencies = other.dependencies;
		loadOrderGroup = other.loadOrderGroup;
		failResetMsg = other.failResetMsg;
		failCommand = other.failCommand;
		privileges = other.privileges;
		serviceType = other.serviceType;
		startType = other.startType;
		errorControl = other.errorControl;
		tagId = other.tagId;
		failResetPeriod = other.failResetPeriod;
		failActions = other.failActions;
		failActionDelay = other.failActionDelay;
		preshutdownTimeout = other.preshutdownTimeout;
		svcSidType = other.svcSidType;
		launchProtected = other.launchProtected;
		preferredNode = other.preferredNode;
		preferredNodeDeleteFlag = other.preferredNodeDeleteFlag;
		delayedAutoStart = other.delayedAutoStart;
		failureActionsOnNonCrashFailures = other.failureActionsOnNonCrashFailures;
		triggers = other.triggers;
		failActionType = other.failActionType;
		memcpy(&svcProcDetail, &other.svcProcDetail, sizeof(SERVICE_STATUS_PROCESS));
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
SvcRecord::SvcRecord(SvcRecord &&other) noexcept {
	if (this != &other) {
		serviceType = std::exchange(other.serviceType, 0);
		startType = std::exchange(other.startType, 0);
		errorControl = std::exchange(other.errorControl, 0);
		tagId = std::exchange(other.tagId, 0);
		failResetPeriod = std::exchange(other.failResetPeriod, 0);
		failActions = std::exchange(other.failActions, 0);
		failActionDelay = std::exchange(other.failActionDelay, 0);
		preshutdownTimeout = std::exchange(other.preshutdownTimeout, 0);
		svcSidType = std::exchange(other.svcSidType, 0);
		launchProtected = std::exchange(other.launchProtected, 0);
		preferredNode = std::exchange(other.preferredNode, 0);
		preferredNodeDeleteFlag = std::exchange(other.preferredNodeDeleteFlag, false);
		delayedAutoStart = std::exchange(other.delayedAutoStart, false);
		failureActionsOnNonCrashFailures = std::exchange(other.failureActionsOnNonCrashFailures, false);
		svcName = std::move(other.svcName);
		svcDisplayName = std::move(other.svcDisplayName);
		svcDesc = std::move(other.svcDesc);
		binaryPath = std::move(other.binaryPath);
		startsFrom = std::move(other.startsFrom);
		dependencies = std::move(other.dependencies);
		loadOrderGroup = std::move(other.loadOrderGroup);
		failResetMsg = std::move(other.failResetMsg);
		failCommand = std::move(other.failCommand);
		privileges = std::move(other.privileges);
		triggers = std::move(other.triggers);
		failActionType = std::move(other.failActionType);
		memset(&other.failActionType, 0, sizeof(SC_ACTION_TYPE));
		svcProcDetail = std::move(other.svcProcDetail);
		memset(&other.svcProcDetail, 0, sizeof(SERVICE_STATUS_PROCESS));
	}
}
#endif

SvcRecord::~SvcRecord() {}

SvcRecord& SvcRecord::operator=(const SvcRecord &other) {
	if (this != &other) {
		svcName = other.svcName;
		svcDisplayName = other.svcDisplayName;
		svcDesc = other.svcDesc;
		binaryPath = other.binaryPath;
		startsFrom = other.startsFrom;
		dependencies = other.dependencies;
		loadOrderGroup = other.loadOrderGroup;
		failResetMsg = other.failResetMsg;
		failCommand = other.failCommand;
		privileges = other.privileges;
		serviceType = other.serviceType;
		startType = other.startType;
		errorControl = other.errorControl;
		tagId = other.tagId;
		failResetPeriod = other.failResetPeriod;
		failActions = other.failActions;
		failActionDelay = other.failActionDelay;
		preshutdownTimeout = other.preshutdownTimeout;
		svcSidType = other.svcSidType;
		launchProtected = other.launchProtected;
		preferredNode = other.preferredNode;
		preferredNodeDeleteFlag = other.preferredNodeDeleteFlag;
		delayedAutoStart = other.delayedAutoStart;
		failureActionsOnNonCrashFailures = other.failureActionsOnNonCrashFailures;
		triggers = other.triggers;
		failActionType = other.failActionType;
		memcpy(&svcProcDetail, &other.svcProcDetail, sizeof(SERVICE_STATUS_PROCESS));
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
SvcRecord& SvcRecord::SvcRecord::operator=(SvcRecord &&other) noexcept {
	if (this != &other) {
		serviceType = std::exchange(other.serviceType, 0);
		startType = std::exchange(other.startType, 0);
		errorControl = std::exchange(other.errorControl, 0);
		tagId = std::exchange(other.tagId, 0);
		failResetPeriod = std::exchange(other.failResetPeriod, 0);
		failActions = std::exchange(other.failActions, 0);
		failActionDelay = std::exchange(other.failActionDelay, 0);
		preshutdownTimeout = std::exchange(other.preshutdownTimeout, 0);
		svcSidType = std::exchange(other.svcSidType, 0);
		launchProtected = std::exchange(other.launchProtected, 0);
		preferredNode = std::exchange(other.preferredNode, 0);
		preferredNodeDeleteFlag = std::exchange(other.preferredNodeDeleteFlag, false);
		delayedAutoStart = std::exchange(other.delayedAutoStart, false);
		failureActionsOnNonCrashFailures = std::exchange(other.failureActionsOnNonCrashFailures, false);
		svcName = std::move(other.svcName);
		svcDisplayName = std::move(other.svcDisplayName);
		svcDesc = std::move(other.svcDesc);
		binaryPath = std::move(other.binaryPath);
		startsFrom = std::move(other.startsFrom);
		dependencies = std::move(other.dependencies);
		loadOrderGroup = std::move(other.loadOrderGroup);
		failResetMsg = std::move(other.failResetMsg);
		failCommand = std::move(other.failCommand);
		privileges = std::move(other.privileges);
		triggers = std::move(other.triggers);
		failActionType = std::move(other.failActionType);
		memset(&other.failActionType, 0, sizeof(SC_ACTION_TYPE));
		svcProcDetail = std::move(other.svcProcDetail);
		memset(&other.svcProcDetail, 0, sizeof(SERVICE_STATUS_PROCESS));
	}
	return *this;
}
#endif

bool SvcRecord::operator ==(const SvcRecord& other) const {
	if (this != &other) {
		return (lower_copy(svcName) == lower_copy(other.svcName) &&
				lower_copy(svcDisplayName) == lower_copy(other.svcDisplayName) &&
				lower_copy(svcDesc) == lower_copy(other.svcDesc) &&
				lower_copy(binaryPath) == lower_copy(other.binaryPath) &&
				lower_copy(startsFrom) == lower_copy(other.startsFrom) &&
				lower_copy(dependencies) == lower_copy(other.dependencies) &&
				lower_copy(loadOrderGroup) == lower_copy(other.loadOrderGroup) &&
				lower_copy(failResetMsg) == lower_copy(other.failResetMsg) &&
				lower_copy(failResetMsg) == lower_copy(other.failResetMsg) &&
				privileges == other.privileges &&
				serviceType == other.serviceType &&
				startType == other.startType &&
				errorControl == other.errorControl &&
				tagId == other.tagId &&
				failResetPeriod == other.failResetPeriod &&
				failActions == other.failActions &&
				failActionDelay == other.failActionDelay &&
				preshutdownTimeout == other.preshutdownTimeout &&
				svcSidType == other.svcSidType &&
				launchProtected == other.launchProtected &&
				preferredNode == other.preferredNode &&
				preferredNodeDeleteFlag == other.preferredNodeDeleteFlag &&
				delayedAutoStart == other.delayedAutoStart &&
				failureActionsOnNonCrashFailures == other.failureActionsOnNonCrashFailures &&
				triggers == other.triggers &&
				!memcmp(&failActionType, &other.failActionType, sizeof(SC_ACTION_TYPE)) &&
				!memcmp(&svcProcDetail, &other.svcProcDetail, sizeof(SERVICE_STATUS_PROCESS)));
	} else {
		return true;
	}
}

bool SvcRecord::operator !=(const SvcRecord& other) const {
	if (this != &other) {
		return (lower_copy(svcName) != lower_copy(other.svcName) ||
				lower_copy(svcDisplayName) != lower_copy(other.svcDisplayName) ||
				lower_copy(svcDesc) != lower_copy(other.svcDesc) ||
				lower_copy(binaryPath) != lower_copy(other.binaryPath) ||
				lower_copy(startsFrom) != lower_copy(other.startsFrom) ||
				lower_copy(dependencies) != lower_copy(other.dependencies) ||
				lower_copy(loadOrderGroup) != lower_copy(other.loadOrderGroup) ||
				lower_copy(failResetMsg) != lower_copy(other.failResetMsg) ||
				lower_copy(failResetMsg) != lower_copy(other.failResetMsg) ||
				privileges != other.privileges ||
				serviceType != other.serviceType ||
				startType != other.startType ||
				errorControl != other.errorControl ||
				tagId != other.tagId ||
				failResetPeriod != other.failResetPeriod ||
				failActions != other.failActions ||
				failActionDelay != other.failActionDelay ||
				preshutdownTimeout != other.preshutdownTimeout ||
				svcSidType != other.svcSidType ||
				launchProtected != other.launchProtected ||
				preferredNode != other.preferredNode ||
				preferredNodeDeleteFlag != other.preferredNodeDeleteFlag ||
				delayedAutoStart != other.delayedAutoStart ||
				failureActionsOnNonCrashFailures != other.failureActionsOnNonCrashFailures ||
				triggers != other.triggers ||
				failActionType != other.failActionType ||
				memcmp(&failActionType, &other.failActionType, sizeof(SC_ACTION_TYPE)) ||
				memcmp(&svcProcDetail, &other.svcProcDetail, sizeof(SERVICE_STATUS_PROCESS)));
	} else {
		return false;
	}
}

SvcHandler::SvcHandler() {
	m_scmHandle = 0;
	m_scmRights = 0;
	m_scmOpen = false;
}

SvcHandler::SvcHandler(const ::SC_HANDLE schandle, const unsigned long scmrights, const bool scmopen) {
	m_scmHandle = schandle;
	m_scmRights = scmrights;
	m_scmOpen = scmopen;
}

SvcHandler::SvcHandler(const SvcHandler &other) {
	if (this != &other) {
		m_scmHandle = other.m_scmHandle;
		m_scmRights = other.m_scmRights;
		m_scmOpen = other.m_scmOpen;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
SvcHandler::SvcHandler(SvcHandler &&other) noexcept {
	if (this != &other) {
		m_scmRights = std::exchange(other.m_scmRights, 0);
		m_scmOpen = std::exchange(other.m_scmOpen, false);
		m_scmHandle = std::move(other.m_scmHandle);
		other.m_scmHandle = 0;
	}
}
#endif

SvcHandler::~SvcHandler() {
	if (m_scmHandle) {
		SCM_CloseManager();
	}
}

SvcHandler& SvcHandler::operator=(const SvcHandler &other) {
	if (this != &other) {
		m_scmHandle = other.m_scmHandle;
		m_scmRights = other.m_scmRights;
		m_scmOpen = other.m_scmOpen;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
SvcHandler& SvcHandler::operator=(SvcHandler &&other) noexcept {
	if (this != &other) {
		m_scmRights = std::exchange(other.m_scmRights, 0);
		m_scmOpen = std::exchange(other.m_scmOpen, false);
		m_scmHandle = std::move(other.m_scmHandle);
		other.m_scmHandle = 0;
	}
	return *this;
}
#endif

bool SvcHandler::operator==(const SvcHandler &other) const {
	if (this != &other) {
		return (m_scmHandle == other.m_scmHandle &&
				m_scmRights == other.m_scmRights &&
				m_scmOpen == other.m_scmOpen);
	} else {
		return true;
	}
}

bool SvcHandler::operator!=(const SvcHandler &other) const {
	if (this != &other) {
		return (m_scmHandle != other.m_scmHandle ||
				m_scmRights != other.m_scmRights ||
				m_scmOpen != other.m_scmOpen);
	} else {
		return false;
	}
}

ScmOpResult SvcHandler::SCM_OpenManager(unsigned long desiredRghts) {
	if (!desiredRghts) {
		desiredRghts = GENERIC_ALL;
	}
	::SC_HANDLE sch = OpenSCManager(NULL, NULL, desiredRghts);
	if (0 == sch) {
		return ScmOpResult::OpenManagerFail;
	}
	m_scmHandle = sch;
	m_scmRights = desiredRghts;
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::SCM_CloseManager() {
	if (!::CloseServiceHandle(m_scmHandle)) {
		return ScmOpResult::CloseManagerFail;
	}
	m_scmHandle = 0;
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::InstallSvc(const std::wstring svcName, const std::wstring displayName,
	const std::wstring binPath, const unsigned long desiredRights, const unsigned long tagId,
	const SvcType svcType, const SvcStartUpType startupType, const SvcErrControl errControlType,
	std::wstring loadOrderGroup, std::wstring startupAccLogin, std::wstring startupAccPwd,
	const std::vector<std::wstring> *depenendencies) {
	wchar_t* loadOrderGroupPtr = (loadOrderGroup.length()) ? ((wchar_t*)loadOrderGroup.c_str()) : 0;
	wchar_t* startupAccLoginPtr = (startupAccLogin.length()) ? ((wchar_t*)startupAccLogin.c_str()) : 0;
	wchar_t* startupAccPwdPtr = (startupAccPwd.length()) ? ((wchar_t*)startupAccPwd.c_str()) : 0;
	wchar_t* depenendenciesPtr = 0;
	size_t i = 0, bufSz = 0;
	if (!svcName.length()) {
		return ScmOpResult::InstallFail;
	}
	if (!displayName.length()) {
		return ScmOpResult::InstallFail;
	}
	if (binPath.length()) {
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(binPath.c_str())) {
			return ScmOpResult::InstallFail;
		}
	} else {
		return ScmOpResult::InstallFail;
	}
	if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
		return ScmOpResult::InstallFail;
	}
	if (depenendencies) {
		if ((*depenendencies).size()) {
			for (i = 0; i < (*depenendencies).size(); ++i) {
				bufSz += (*depenendencies)[i].length() + 1;
			}
			++bufSz;
			NEW_ARR_NULLIFY_NO_REDEFINE(depenendenciesPtr, wchar_t, bufSz);
			if (!depenendenciesPtr) {
				return ScmOpResult::InstallFail;
			}
			unsigned long bufPtr = 0;
			for (i = 0; i < (*depenendencies).size(); ++i) {
				swprintf(&depenendenciesPtr[bufPtr], L"%s", (*depenendencies)[i].c_str());
				bufPtr += (*depenendencies)[i].length() + 1;
			}
		}
	}
	auto& tagid_unconst = const_cast<unsigned long&>(tagId);
	if (CreateService(
		m_scmHandle,								// SCM manager
		svcName.c_str(),							// name of service
		displayName.c_str(),						// service name to display
		SERVICE_ALL_ACCESS,							// desired access
		static_cast<unsigned long>(svcType),		// service type
		static_cast<unsigned long>(startupType),	// start type
		static_cast<unsigned long>(errControlType),	// error control type
		binPath.c_str(),							// path to service's binary
		loadOrderGroupPtr,							// load ordering group
		&tagid_unconst,								// tag identifier
		depenendenciesPtr,							// dependencies
		startupAccLoginPtr,							// 0 for LocalSystem account
		startupAccPwdPtr)							// password / LocalSystem acc needs no pwd (0)
		) {
		return ScmOpResult::Success;
	}
	return ScmOpResult::InstallFail;
}

ScmOpResult SvcHandler::RemoveSvc(const std::wstring svcName, const StrCompareType compareType,
	const unsigned long desiredRights, const std::vector<SvcRecord>* svcRecords) {
	std::vector<SvcRecord> vec;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights, ServiceType::Any)) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	std::vector<SvcRecord> foundsvcs;
	foundsvcs = GetSvcBySvcName(svcName, compareType, true, desiredRights, &vec);
	if (foundsvcs.size() != 1) {
		foundsvcs = GetSvcByDisplayName(svcName, compareType, true, desiredRights, &vec);
		if (foundsvcs.size() != 1) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
		return ScmOpResult::ChangeServiceFail;
	}
	::SC_HANDLE hSvc = ::OpenService(m_scmHandle, foundsvcs[0].svcName.c_str(), SERVICE_ALL_ACCESS);
	if (hSvc && INVALID_HANDLE_VALUE != hSvc) {
		if (DeleteService(hSvc)) {
			return ScmOpResult::Success;
		}
		return ScmOpResult::RemoveFail;
	}
	return ScmOpResult::RemoveFail;
}

ScmOpResult SvcHandler::StartSvc(const std::wstring displayName,
	const unsigned long desiredRights,
	const std::vector<std::wstring> *svcArgs,
	const std::vector<SvcRecord> *svcRecords) {
	std::vector<SvcRecord> svcrecs;
	ScmOpResult ret = ScmOpResult::StartSvcFail;
	if (svcRecords) {
		svcrecs = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(svcrecs, desiredRights)) {
			return ret;
		}
	}
	bool found = false;
	std::vector<SvcRecord> foundsvcs = GetSvcByDisplayName(displayName);
	if (foundsvcs.size() != 1) {
		foundsvcs = GetSvcBySvcName(displayName);
		if (foundsvcs.size() == 1) {
			found = true;
		}
	} else {
		found = true;
	}
	if (found) {
		if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
			return ScmOpResult::StartSvcFail;
		}
		::SC_HANDLE hSvc = ::OpenService(m_scmHandle, foundsvcs[0].svcName.c_str(), SERVICE_ALL_ACCESS);
		if (hSvc && INVALID_HANDLE_VALUE != hSvc) {
			SERVICE_STATUS_PROCESS svcStatus = { 0 };
			unsigned long dwBytesNeeded = 0, dwWaitTime = 0;
			if (QueryServiceStatusEx(hSvc, SC_STATUS_PROCESS_INFO, (unsigned char*)&svcStatus,
				sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
				if (svcStatus.dwCurrentState == SERVICE_STOPPED || svcStatus.dwCurrentState == SERVICE_STOP_PENDING) {
					unsigned long dwStartTickCount = GetTickCount64(), dwOldCheckPoint = svcStatus.dwCheckPoint;
					while (svcStatus.dwCurrentState == SERVICE_STOP_PENDING) {
						dwWaitTime = svcStatus.dwWaitHint / 10;
						if (dwWaitTime < 1000) {
							dwWaitTime = 1000;
						} else if (dwWaitTime > 10000) {
							dwWaitTime = 10000;
						}
					}
					Sleep(dwWaitTime);
					if (QueryServiceStatusEx(hSvc, SC_STATUS_PROCESS_INFO,
						(unsigned char*)&svcStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
						if (svcStatus.dwCheckPoint > dwOldCheckPoint) {
							dwStartTickCount = GetTickCount64();
							dwOldCheckPoint = svcStatus.dwCheckPoint;
						} else if ((dwStartTickCount && svcStatus.dwWaitHint) &&
							(GetTickCount64() - dwStartTickCount > svcStatus.dwWaitHint)) {
							::CloseServiceHandle(hSvc);
							return ret;
						}
						size_t argCount = 0;
						wchar_t** svcArgsBuf = 0;
						if (svcArgs) {
							argCount = (*svcArgs).size();
							if (argCount) {
								svcArgsBuf = (wchar_t**)calloc(argCount, argCount * sizeof(wchar_t*));
								if (!svcArgsBuf) {
									::CloseServiceHandle(hSvc);
									return ret;
								}
								size_t slen = 0;
								for (size_t i = 0; i < argCount; ++i) {
									svcArgsBuf[i] = (wchar_t*)calloc((*svcArgs)[i].length() + 1,
										((*svcArgs)[i].length() + 1) * sizeof(wchar_t));
									slen = (*svcArgs)[i].length();
									memcpy(&svcArgsBuf[i][0], (*svcArgs)[i].c_str(), slen * sizeof(wchar_t));
								}
							}
						}
						if (StartService(hSvc, argCount, (const wchar_t**)svcArgsBuf)) {
							for (size_t i = 0; i < argCount; ++i) {
								free(svcArgsBuf[i]);
								svcArgsBuf[i] = 0;
							}
							free(svcArgsBuf);
							svcArgsBuf = 0;
							if (QueryServiceStatusEx(hSvc, SC_STATUS_PROCESS_INFO,
								(unsigned char*)&svcStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))  {
								dwStartTickCount = GetTickCount64();
								dwOldCheckPoint = svcStatus.dwCheckPoint;
								while (svcStatus.dwCurrentState == SERVICE_START_PENDING) {
									dwWaitTime = svcStatus.dwWaitHint / 10;
									if (dwWaitTime < 1000) {
										dwWaitTime = 1000;
									} else if (dwWaitTime > 10000) {
										dwWaitTime = 10000;
									}
									Sleep(dwWaitTime);
									if (!QueryServiceStatusEx(hSvc, SC_STATUS_PROCESS_INFO,
										(unsigned char*)&svcStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
										::CloseServiceHandle(hSvc);
										return ret;
									}
									if (svcStatus.dwCheckPoint > dwOldCheckPoint) {
										dwStartTickCount = GetTickCount64();
										dwOldCheckPoint = svcStatus.dwCheckPoint;
									} else if ((dwStartTickCount && svcStatus.dwWaitHint) &&
										(GetTickCount64() - dwStartTickCount > svcStatus.dwWaitHint)) {
										::CloseServiceHandle(hSvc);
										return ret;
									}
								}
							}
						} else {
							::CloseServiceHandle(hSvc);
							return ret;
						}
					}
				} else {
					::CloseServiceHandle(hSvc);
					return ScmOpResult::StartSvcFailAlreadyStarted;
				}
			}
			::CloseServiceHandle(hSvc);
			return ScmOpResult::Success;
		}
	}
	return ret;
}

ScmOpResult SvcHandler::StopSvc(const std::wstring displayName, const bool stopDependentSvcs,
	const bool stopDrivers, const unsigned long desiredRights, const unsigned long timeout,
	const std::vector<SvcRecord>* svcRecords) {
	std::vector<SvcRecord> svcrecs;
	if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
		return ScmOpResult::StopSvcFail;
	}
	if (svcRecords) {
		svcrecs = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(svcrecs, desiredRights)) {
			return ScmOpResult::StopSvcFail;
		}
	}
	bool found = false;
	std::vector<SvcRecord> foundsvcs = GetSvcByDisplayName(displayName);
	if (foundsvcs.size() != 1) {
		foundsvcs = GetSvcBySvcName(displayName);
		if (foundsvcs.size() == 1) {
			found = true;
		}
	} else {
		found = true;
	}
	if (found) {
		if (!stopDrivers && IsSvcDriver(foundsvcs[0])) {
			return ScmOpResult::StopSvcFail;
		}
		if (stopDependentSvcs) {
			for (size_t i = 0; i < foundsvcs[0].dependencies.size(); ++i) {
				std::vector<SvcRecord> depsvc = GetSvcBySvcName(foundsvcs[0].dependencies[i],
					StrCompareType::equalIgnoreCase, true, 0, &svcrecs);
				if (stopDrivers && IsSvcDriver(depsvc[0])) {
					StopDependentSvc(depsvc[0], stopDrivers, timeout, desiredRights, &svcrecs);
				}
			}
		}
		::SC_HANDLE hSvc = ::OpenService(m_scmHandle, foundsvcs[0].svcName.c_str(),
			SERVICE_STOP | SERVICE_QUERY_STATUS);
		if (hSvc && INVALID_HANDLE_VALUE != hSvc) {
			SERVICE_STATUS_PROCESS svcStatus = { 0 };
			unsigned long dwBytesNeeded = 0, dwWaitTime = 0;
			if (QueryServiceStatusEx(hSvc, SC_STATUS_PROCESS_INFO, (unsigned char*)&svcStatus,
				sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
				if (svcStatus.dwCurrentState != SERVICE_STOPPED) {
					while (svcStatus.dwCurrentState == SERVICE_STOP_PENDING) {
						dwWaitTime = svcStatus.dwWaitHint / 10;
						if (dwWaitTime < 1000) {
							dwWaitTime = 1000;
						} else if (dwWaitTime > 10000) {
							dwWaitTime = 10000;
						}
						Sleep(dwWaitTime);
					}
					if (QueryServiceStatusEx(hSvc, SC_STATUS_PROCESS_INFO,
						(unsigned char*)&svcStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
						if (svcStatus.dwCurrentState == SERVICE_STOPPED) {
							::CloseServiceHandle(hSvc);
							return ScmOpResult::Success;
						}
					}
					if (ControlService(hSvc, SERVICE_CONTROL_STOP,
						(LPSERVICE_STATUS)&svcStatus)) {
						unsigned long dwStartTime = GetTickCount64();
						while (svcStatus.dwCurrentState == SERVICE_STOP_PENDING) {
							dwWaitTime = svcStatus.dwWaitHint / 10;
							if (dwWaitTime < 1000) {
								dwWaitTime = 1000;
							} else if (dwWaitTime > 10000) {
								dwWaitTime = 10000;
							}
							Sleep(dwWaitTime);
							if (QueryServiceStatusEx(hSvc, SC_STATUS_PROCESS_INFO,
								(unsigned char*)&svcStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
								if (svcStatus.dwCurrentState == SERVICE_STOPPED) {
									::CloseServiceHandle(hSvc);
									return ScmOpResult::Success;
								} else if (GetTickCount64() - dwStartTime > timeout) {
									::CloseServiceHandle(hSvc);
									return ScmOpResult::StopSvcFail;
								}
							} else {
								::CloseServiceHandle(hSvc);
								return ScmOpResult::StopSvcFail;
							}
						}
						if (QueryServiceStatusEx(hSvc, SC_STATUS_PROCESS_INFO,
							(unsigned char*)&svcStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
							if (svcStatus.dwCurrentState == SERVICE_STOPPED) {
								::CloseServiceHandle(hSvc);
								return ScmOpResult::Success;
							}
						}
					} else {
						::CloseServiceHandle(hSvc);
						return ScmOpResult::StopSvcFail;
					}
				} else {
					::CloseServiceHandle(hSvc);
					return ScmOpResult::StopSvcFailAlreadyStopped;
				}
			} else {
				::CloseServiceHandle(hSvc);
				return ScmOpResult::StopSvcFail;
			}
		} else {
			return ScmOpResult::StopSvcFail;
		}
	}
	return ScmOpResult::StopSvcFail;
}

bool SvcHandler::IsSvcDriver(const SvcRecord &svcrec) {
	return (svcrec.serviceType & (SERVICE_FILE_SYSTEM_DRIVER | SERVICE_KERNEL_DRIVER));
}

bool SvcHandler::IsSvcDriver(const std::wstring serviceName, const unsigned long desiredRights,
	const std::vector<SvcRecord>* svcRecords) {
	std::vector<SvcRecord> svcrecs;
	if (svcRecords) {
		svcrecs = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(svcrecs, desiredRights)) {
			return false;
		}
	}
	bool found = false;
	SvcHandler svch;
	std::vector<SvcRecord> ret = svch.GetSvcBySvcName(serviceName, StrCompareType::equalIgnoreCase, true, 0, svcRecords);
	if (ret.size() != 1) {
		ret = svch.GetSvcByDisplayName(serviceName, StrCompareType::equalIgnoreCase, true, 0, svcRecords);
		if (ret.size() == 1) {
			found = true;
		} else {
			return false;
		}
	} else {
		found = true;
	}
	if (found) {
		return ret[0].serviceType &
			(SERVICE_FILE_SYSTEM_DRIVER | SERVICE_KERNEL_DRIVER);
	} else {
		return false;
	}
}

ScmOpResult SvcHandler::EnumSvcsStatus(std::vector<SvcRecord>& svcrecs,
	const unsigned long desiredRights, ServiceType svcType, ServiceState svcState) {
	if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
		return ScmOpResult::EnumSvcsFail;
	}
	SvcRecord rec;
	SvcTrigger trg;
	size_t trgDataSz = 0;
	::SC_HANDLE svcHandle = 0;
	void* buf = NULL;
	unsigned long bufSize = 0, moreBytesNeeded = 0, serviceCount = 0;
	for (;;) {
		if (EnumServicesStatusEx(
			m_scmHandle,
			SC_ENUM_PROCESS_INFO,
			(unsigned long)svcType,
			(unsigned long)svcState,
			(unsigned char*)buf,
			bufSize,
			&moreBytesNeeded,
			&serviceCount,
			NULL,
			NULL)) {
			ENUM_SERVICE_STATUS_PROCESS* services = (ENUM_SERVICE_STATUS_PROCESS*)buf;
			if (!serviceCount || !services) {
				SAFE_FREE(buf);
				return ScmOpResult::EnumSvcsFail;
			}
			for (unsigned long i = 0; i < serviceCount; ++i) {
				rec.triggers.clear();
				rec.svcName = services[i].lpServiceName;
				rec.svcDisplayName = services[i].lpDisplayName;
				rec.svcProcDetail = services[i].ServiceStatusProcess;
				svcHandle = ::OpenService(m_scmHandle, rec.svcName.c_str(), SERVICE_QUERY_CONFIG);
				if (svcHandle) {
					WinVerAdvanced wver = getWinVersionAdvanced();
					QUERY_SERVICE_CONFIG* configBuf = 0;
					unsigned long configBufSz = 0, dwBN = sizeof(QUERY_SERVICE_CONFIG);
					do {
						SAFE_FREE(configBuf);
						configBufSz = dwBN;
						configBuf = (QUERY_SERVICE_CONFIG*)malloc(configBufSz);
						if (!configBuf) {
							return ScmOpResult::EnumSvcsFail;
						}
						if (QueryServiceConfig(svcHandle, (LPQUERY_SERVICE_CONFIG)configBuf, configBufSz, &dwBN)) {
							QUERY_SERVICE_CONFIG* svcconfig = (QUERY_SERVICE_CONFIG*)configBuf;
							if (wcslen_c(svcconfig->lpBinaryPathName)) {
								rec.binaryPath = svcconfig->lpBinaryPathName;
							}
							if (svcconfig->lpDependencies[0]) {
								size_t buflen = 0;
								for (size_t i = 1; i < 4096; ++i) {
									if(!svcconfig->lpDependencies[i] && !svcconfig->lpDependencies[i -1]) {
										buflen = i - 1;
										break;
									}
								}
								if (buflen) {
									wchar_t* newBuf = replaceChars(svcconfig->lpDependencies, L"\0", L"\n", buflen, 1);
									if (!newBuf) {
										return ScmOpResult::EnumSvcsFail;
									}
									std::vector<std::wstring> depspl = splitStr(newBuf, L"\n");
									rec.dependencies = depspl;
									SAFE_ARR_DELETE(newBuf);
									// free(newBuf);
									// newBuf = 0;
								} else {
									rec.dependencies = std::vector<std::wstring>();
								}
							}
							rec.errorControl = svcconfig->dwErrorControl;
							if (wcslen_c(svcconfig->lpLoadOrderGroup)) {
								rec.loadOrderGroup = svcconfig->lpLoadOrderGroup;
							}
							if (wcslen_c(svcconfig->lpServiceStartName)) {
								rec.startsFrom = svcconfig->lpServiceStartName;
							}
							rec.serviceType = svcconfig->dwServiceType;
							rec.startType = svcconfig->dwStartType;
							rec.tagId = svcconfig->dwTagId;
							break;
						}
						SAFE_FREE(configBuf);
					} while (getLastErrorCode() == ERROR_INSUFFICIENT_BUFFER);
					SAFE_FREE(configBuf);
					// free(configBuf);
					// configBuf = 0;
					if (ScmOpResult::Success != QuerySvcDesc(svcHandle, rec)) {
						return ScmOpResult::EnumSvcsFail;
					}
					if (WinVerAdvanced::WinSrv2003 < wver) {
						if (ScmOpResult::Success != QuerySvcDelayedAutostart(svcHandle, rec)) {
							return ScmOpResult::EnumSvcsFail;
						}
					}
					if (ScmOpResult::Success != QuerySvcFailureActions(svcHandle, rec)) {
						return ScmOpResult::EnumSvcsFail;
					}
					if (WinVerAdvanced::WinSrv2003 < wver) {
						if (ScmOpResult::Success != QuerySvcFailureActionsFlag(svcHandle, rec)) {
							return ScmOpResult::EnumSvcsFail;
						}
					}
					if (WinVerAdvanced::WinSrv2008 < wver) {
						if (ScmOpResult::Success != QuerySvcPreferredNode(svcHandle, rec)) {
							return ScmOpResult::EnumSvcsFail;
						}
					}
					if (WinVerAdvanced::WinSrv2003 < wver) {
						if (ScmOpResult::Success != QuerySvcPreshutdownInfo(svcHandle, rec)) {
							return ScmOpResult::EnumSvcsFail;
						}
					}
					if (WinVerAdvanced::WinSrv2003 < wver) {
						if (ScmOpResult::Success != QuerySvcPrivileges(svcHandle, rec)) {
							return ScmOpResult::EnumSvcsFail;
						}
					}
					if (WinVerAdvanced::WinSrv2003 < wver) {
						if (ScmOpResult::Success != QuerySvcSIDInfo(svcHandle, rec)) {
							return ScmOpResult::EnumSvcsFail;
						}
					}
					if (WinVerAdvanced::WinSrv2008 < wver) {
						if (ScmOpResult::Success != QuerySvcTriggerInfo(svcHandle, rec)) {
							return ScmOpResult::EnumSvcsFail;
						}
					}
					if (WinVerAdvanced::Win81 <= wver) {
						if (ScmOpResult::Success != QuerySvcLaunchProtected(svcHandle, rec)) {
							return ScmOpResult::EnumSvcsFail;
						}
					}
					::CloseServiceHandle(svcHandle);
				}
				svcrecs.push_back(rec);
			}
			SAFE_FREE(buf);
			// free(buf);
			// buf = 0;
			return ScmOpResult::Success;
		}
		int err = GetLastError();
		if (ERROR_MORE_DATA != err) {
			SAFE_FREE(buf);
			return ScmOpResult::EnumSvcsFail;
		}
		bufSize += moreBytesNeeded;
		SAFE_FREE(buf);
		// free(buf);
		// buf = 0;
		buf = malloc(bufSize);
		if (!buf) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
}

ScmOpResult SvcHandler::SetSvcStartupType(const std::wstring svcName, SvcStartUpType startupType,
	const StrCompareType compareType, const unsigned long desiredRights,
	const std::vector<SvcRecord> *svcRecords) {
	std::vector<SvcRecord> vec;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights, ServiceType::Any)) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	std::vector<SvcRecord> foundsvcs;
	foundsvcs = GetSvcBySvcName(svcName, compareType, true, desiredRights, &vec);
	if (foundsvcs.size() != 1) {
		foundsvcs = GetSvcByDisplayName(svcName, compareType, true, desiredRights, &vec);
		if (foundsvcs.size() != 1) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	if (foundsvcs[0].startType != static_cast<unsigned long>(startupType)) {
		if ((startupType == SvcStartUpType::BootStart ||
			startupType == SvcStartUpType::SystemStart) && !IsSvcDriver(foundsvcs[0])) {
			return ScmOpResult::ChangeServiceFail;
		}
		if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
			return ScmOpResult::ChangeServiceFail;
		}
		::SC_HANDLE hSvc = ::OpenService(m_scmHandle, foundsvcs[0].svcName.c_str(), SERVICE_ALL_ACCESS);
		if (hSvc && INVALID_HANDLE_VALUE != hSvc) {
			if (ChangeServiceConfig(hSvc, SERVICE_NO_CHANGE, static_cast<unsigned long>(startupType),
				SERVICE_NO_CHANGE, 0, 0, 0, 0, 0, 0, 0)) {
				return ScmOpResult::Success;
			} else {
				return ScmOpResult::ChangeServiceFail;
			}
		}
	}
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::SetSvcStartName(const std::wstring svcName, const std::wstring accLogin,
	const std::wstring accPwd, const StrCompareType compareType, const unsigned long desiredRights,
	const std::vector<SvcRecord> *svcRecords) {
	std::vector<SvcRecord> vec;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights, ServiceType::Any)) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	std::vector<SvcRecord> foundsvcs;
	foundsvcs = GetSvcBySvcName(svcName, compareType, true, desiredRights, &vec);
	if (foundsvcs.size() != 1) {
		foundsvcs = GetSvcByDisplayName(svcName, compareType, true, desiredRights, &vec);
		if (foundsvcs.size() != 1) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
		return ScmOpResult::ChangeServiceFail;
	}
	::SC_HANDLE hSvc = ::OpenService(m_scmHandle, foundsvcs[0].svcName.c_str(), SERVICE_ALL_ACCESS);
	if (hSvc && INVALID_HANDLE_VALUE != hSvc) {
		if (ChangeServiceConfig(hSvc, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE,
			SERVICE_NO_CHANGE, 0, 0, 0, 0, accLogin.c_str(), accPwd.c_str(), 0)) {
			return ScmOpResult::Success;
		} else {
			return ScmOpResult::ChangeServiceFail;
		}
	}
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::SetSvcDisplayName(const std::wstring svcName, const std::wstring newDisplayName,
	const StrCompareType compareType, const unsigned long desiredRights,
	const std::vector<SvcRecord> *svcRecords) {
	std::vector<SvcRecord> vec;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights, ServiceType::Any)) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	std::vector<SvcRecord> foundsvcs;
	foundsvcs = GetSvcBySvcName(svcName, compareType, true, desiredRights, &vec);
	if (foundsvcs.size() != 1) {
		foundsvcs = GetSvcByDisplayName(svcName, compareType, true, desiredRights, &vec);
		if (foundsvcs.size() != 1) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	if (newDisplayName.length() > 255) {
		return ScmOpResult::ChangeServiceFail;
	}
	if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
		return ScmOpResult::ChangeServiceFail;
	}
	::SC_HANDLE hSvc = ::OpenService(m_scmHandle, foundsvcs[0].svcName.c_str(), SERVICE_ALL_ACCESS);
	if (hSvc && INVALID_HANDLE_VALUE != hSvc) {
		if (ChangeServiceConfig(hSvc, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE,
			SERVICE_NO_CHANGE, 0, 0, 0, 0, 0, 0, newDisplayName.c_str())) {
			return ScmOpResult::Success;
		} else {
			return ScmOpResult::ChangeServiceFail;
		}
	}
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::SetSvcDependencies(const std::wstring svcName,
	const std::vector<std::wstring> dependencies, const StrCompareType compareType,
	const unsigned long desiredRights, const std::vector<SvcRecord> *svcRecords) {
	std::vector<SvcRecord> vec;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights, ServiceType::Any)) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	std::vector<SvcRecord> foundsvcs;
	foundsvcs = GetSvcBySvcName(svcName, compareType, true, desiredRights, &vec);
	if (foundsvcs.size() != 1) {
		foundsvcs = GetSvcByDisplayName(svcName, compareType, true, desiredRights, &vec);
		if (foundsvcs.size() != 1) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	size_t i = 0;
	unsigned long bufSz = 0, bufPtr = 0;
	for (i = 0; i < dependencies.size(); ++i) {
		bufSz += dependencies[i].length() + 1;
	}
	++bufSz;
	NEW_ARR_NULLIFY(buf, wchar_t, bufSz);
	if (!buf) {
		return ScmOpResult::ChangeServiceFail;
	}
	for (i = 0; i < dependencies.size(); ++i) {
		swprintf(&buf[bufPtr], L"%s", dependencies[i].c_str());
		bufPtr += (dependencies[i].length() + 1) * sizeof(wchar_t);
	}
	if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
		return ScmOpResult::ChangeServiceFail;
	}
	::SC_HANDLE hSvc = ::OpenService(m_scmHandle, foundsvcs[0].svcName.c_str(), SERVICE_ALL_ACCESS);
	if (hSvc && INVALID_HANDLE_VALUE != hSvc) {
		if (ChangeServiceConfig(hSvc, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE,
			SERVICE_NO_CHANGE, 0, 0, 0, buf, 0, 0, 0)) {
			SAFE_ARR_DELETE(buf);
			return ScmOpResult::Success;
		} else {
			SAFE_ARR_DELETE(buf);
			return ScmOpResult::ChangeServiceFail;
		}
	}
	SAFE_ARR_DELETE(buf);
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::SetSvcDelayedAutostart(const std::wstring svcName, const bool enableDelayedAutostart,
	const StrCompareType compareType, const unsigned long desiredRights,
	const std::vector<SvcRecord> *svcRecords) {
	std::vector<SvcRecord> vec;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights, ServiceType::Any)) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	std::vector<SvcRecord> foundsvcs;
	foundsvcs = GetSvcBySvcName(svcName, compareType, true, desiredRights, &vec);
	if (foundsvcs.size() != 1) {
		foundsvcs = GetSvcByDisplayName(svcName, compareType, true, desiredRights, &vec);
		if (foundsvcs.size() != 1) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	SERVICE_DELAYED_AUTO_START_INFO delayedAuotoStartInfo;
	delayedAuotoStartInfo.fDelayedAutostart = enableDelayedAutostart;
	if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
		return ScmOpResult::ChangeServiceFail;
	}
	::SC_HANDLE hSvc = ::OpenService(m_scmHandle, foundsvcs[0].svcName.c_str(), SERVICE_ALL_ACCESS);
	if (hSvc && INVALID_HANDLE_VALUE != hSvc) {
		if (ChangeServiceConfig2(hSvc, SERVICE_CONFIG_DELAYED_AUTO_START_INFO,
			(void*)&delayedAuotoStartInfo)) {
			return ScmOpResult::Success;
		} else {
			return ScmOpResult::ChangeServiceFail;
		}
	}
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::SetSvcDescription(const std::wstring svcName, const std::wstring svcDescription,
	const StrCompareType compareType, const unsigned long desiredRights,
	const std::vector<SvcRecord> *svcRecords) {
	std::vector<SvcRecord> vec;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights, ServiceType::Any)) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	std::vector<SvcRecord> foundsvcs;
	foundsvcs = GetSvcBySvcName(svcName, compareType, true, desiredRights, &vec);
	if (foundsvcs.size() != 1) {
		foundsvcs = GetSvcByDisplayName(svcName, compareType, true, desiredRights, &vec);
		if (foundsvcs.size() != 1) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	SERVICE_DESCRIPTIONA svcDescInfo;
	char* buf = wchar2char(svcDescription.c_str());
	std::string tstr = buf;
	SAFE_ARR_DELETE(buf);
	svcDescInfo.lpDescription = (char*)tstr.c_str();
	if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
		return ScmOpResult::ChangeServiceFail;
	}
	::SC_HANDLE hSvc = ::OpenService(m_scmHandle, foundsvcs[0].svcName.c_str(), SERVICE_ALL_ACCESS);
	if (hSvc && INVALID_HANDLE_VALUE != hSvc) {
		if (ChangeServiceConfig2(hSvc, SERVICE_CONFIG_DESCRIPTION,
			(void*)&svcDescInfo)) {
			return ScmOpResult::Success;
		} else {
			return ScmOpResult::ChangeServiceFail;
		}
	}
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::SetSvcType(const std::wstring svcName, const SvcType svcType,
	const StrCompareType compareType, const unsigned long desiredRights,
	const std::vector<SvcRecord> *svcRecords) {
	std::vector<SvcRecord> vec;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights, ServiceType::Any)) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	std::vector<SvcRecord> foundsvcs;
	foundsvcs = GetSvcBySvcName(svcName, compareType, true, desiredRights, &vec);
	if (foundsvcs.size() != 1) {
		foundsvcs = GetSvcByDisplayName(svcName, compareType, true, desiredRights, &vec);
		if (foundsvcs.size() != 1) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
		return ScmOpResult::ChangeServiceFail;
	}
	::SC_HANDLE hSvc = ::OpenService(m_scmHandle, foundsvcs[0].svcName.c_str(), SERVICE_ALL_ACCESS);
	if (hSvc && INVALID_HANDLE_VALUE != hSvc) {
		if (ChangeServiceConfig(hSvc, static_cast<unsigned long>(svcType), SERVICE_NO_CHANGE,
			SERVICE_NO_CHANGE, 0, 0, 0, 0, 0, 0, 0)) {
			return ScmOpResult::Success;
		} else {
			return ScmOpResult::ChangeServiceFail;
		}
	}
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::SetSvcBinary(const std::wstring svcName, const std::wstring svcBinPath,
	const StrCompareType compareType, const unsigned long desiredRights,
	const std::vector<SvcRecord> *svcRecords) {
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(svcBinPath.c_str())) {
		return ScmOpResult::ChangeServiceFail;
	}
	std::vector<SvcRecord> vec;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights, ServiceType::Any)) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	std::vector<SvcRecord> foundsvcs;
	foundsvcs = GetSvcBySvcName(svcName, compareType, true, desiredRights, &vec);
	if (foundsvcs.size() != 1) {
		foundsvcs = GetSvcByDisplayName(svcName, compareType, true, desiredRights, &vec);
		if (foundsvcs.size() != 1) {
			return ScmOpResult::EnumSvcsFail;
		}
	}
	if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
		return ScmOpResult::ChangeServiceFail;
	}
	::SC_HANDLE hSvc = ::OpenService(m_scmHandle, foundsvcs[0].svcName.c_str(), SERVICE_ALL_ACCESS);
	if (hSvc && INVALID_HANDLE_VALUE != hSvc) {
		if (ChangeServiceConfig(hSvc, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE,
			SERVICE_NO_CHANGE, (L"\"" + svcBinPath + L"\"").c_str(), 0, 0, 0, 0, 0, 0)) {
			return ScmOpResult::Success;
		} else {
			return ScmOpResult::ChangeServiceFail;
		}
	}
	return ScmOpResult::Success;
}

SvcRunState SvcHandler::IsServiceRunning(const std::wstring svcDisplayName,
	const unsigned long desiredRights, const std::vector<SvcRecord> *svcRecords) {
	std::vector<SvcRecord> vec;
	SvcRunState ret;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights, ServiceType::Service)) {
			return ret;
		}
	}
	std::vector<SvcRecord> svcNeeded = GetSvcByDisplayName(svcDisplayName,
		StrCompareType::equalIgnoreCase,
		true, desiredRights);
	if (svcNeeded.size() != 1) {
		return ret;
	}
	if((svcNeeded[0].svcProcDetail.dwCurrentState == SERVICE_CONTINUE_PENDING) ||
		(svcNeeded[0].svcProcDetail.dwCurrentState == SERVICE_PAUSE_PENDING) ||
		(svcNeeded[0].svcProcDetail.dwCurrentState == SERVICE_PAUSED) ||
		(svcNeeded[0].svcProcDetail.dwCurrentState == SERVICE_RUNNING) ||
		(svcNeeded[0].svcProcDetail.dwCurrentState == SERVICE_START_PENDING)) {
		ret.svcRunning = true;
	}
	std::wstring tstr = lower_copy(svcNeeded[0].binaryPath);
	size_t markPos = tstr.find(L".exe");
	if (std::string::npos != markPos) {
		tstr = tstr.substr(0, markPos + 4);
	}
	ProcessHandler procHandler;
	ret.svcProcRunning = procHandler.IsProcRunning(tstr);
	return ret;
}

std::vector<SvcRecord> SvcHandler::GetSvcsWithNTriggers(size_t trigCount,
	const DepCompareType depCompareType, const bool uniqueResults, const unsigned long desiredRights,
	const std::vector<SvcRecord>* svcRecords) {
	std::vector<SvcRecord> vec;
	std::vector<SvcRecord> ret;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights)) {
			return ret;
		}
	}
	/*size_t triggerCount = 0;
	if (trigCount) {
		triggerCount = trigCount;
	} else {
		triggerCount = 1;
	}*/
	for (size_t i = 0; i < vec.size(); ++i) {
		if (DepCompareType::equal == depCompareType) {
			if (vec[i].triggers.size() == trigCount) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (DepCompareType::notEqual == depCompareType) {
			if (vec[i].triggers.size() != trigCount) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (DepCompareType::greater == depCompareType) {
			if (vec[i].triggers.size() > trigCount) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (DepCompareType::less == depCompareType) {
			if (vec[i].triggers.size() < trigCount) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (DepCompareType::greaterOrEqual == depCompareType) {
			if (vec[i].triggers.size() >= trigCount) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (DepCompareType::lessOrEqual <= depCompareType) {
			if (vec[i].triggers.size() <= trigCount) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		}
	}
	return ret;
}

std::vector<SvcRecord> SvcHandler::GetSvcsWithNDependencies(size_t depCount, const DepCompareType depCompareType,
	bool uniqueResults, const unsigned long desiredRights, const std::vector<SvcRecord>* svcRecords) {
	std::vector<SvcRecord> vec;
	std::vector<SvcRecord> ret;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights)) {
			return ret;
		}
	}
	/*size_t dependsCount = 0;
	if (depCount) {
		dependsCount = depCount;
	} else {
		dependsCount = 1;
	}*/
	for (size_t i = 0; i < vec.size(); ++i) {
		if (DepCompareType::equal == depCompareType) {
			if (vec[i].dependencies.size() == depCount) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (DepCompareType::notEqual == depCompareType) {
			if (vec[i].dependencies.size() != depCount) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (DepCompareType::greater == depCompareType) {
			if (vec[i].dependencies.size() > depCount) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (DepCompareType::less == depCompareType) {
			if (vec[i].dependencies.size() < depCount) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (DepCompareType::greaterOrEqual == depCompareType) {
			if (vec[i].dependencies.size() >= depCount) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (DepCompareType::lessOrEqual <= depCompareType) {
			if (vec[i].dependencies.size() <= depCount) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		}
	}
	return ret;
}

std::vector<SvcRecord> SvcHandler::GetSvcByDisplayName(const std::wstring displayName,
	const StrCompareType compareType, const bool uniqueResults, const unsigned long desiredRights,
	const std::vector<SvcRecord>* svcRecords) {
	std::vector<SvcRecord> vec;
	std::vector<SvcRecord> ret;
	bool addFlag = true;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		ScmOpResult res = EnumSvcsStatus(vec, desiredRights);
		if (ScmOpResult::Success != res) {
			return ret;
		}
	}
	for (size_t i = 0; i < vec.size(); ++i) {
		if (StrCompareType::equal == compareType) {
			if (vec[i].svcDisplayName == displayName) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (StrCompareType::equalIgnoreCase == compareType) {
			if (lower_copy(vec[i].svcDisplayName) == lower_copy(displayName)) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (StrCompareType::partialMatch == compareType) {
			if (partialMatch(vec[i].svcDisplayName, displayName, false)) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (StrCompareType::partialMatchIgnoreCase == compareType) {
			if (partialMatch(vec[i].svcDisplayName, displayName)) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		}
	}
	return ret;
}

std::vector<SvcRecord> SvcHandler::GetSvcBySvcName(const std::wstring svcName,
	const StrCompareType compareType, const bool uniqueResults, const unsigned long desiredRights,
	const std::vector<SvcRecord>* svcRecords) {
	std::vector<SvcRecord> vec;
	std::vector<SvcRecord> ret;
	bool addFlag = true;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights)) {
			return ret;
		}
	}
	for (size_t i = 0; i < vec.size(); ++i) {
		if (StrCompareType::equal == compareType) {
			if (vec[i].svcName == svcName) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (StrCompareType::equalIgnoreCase == compareType) {
			if (lower_copy(vec[i].svcName) == lower_copy(svcName)) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (StrCompareType::partialMatch == compareType) {
			if (partialMatch(vec[i].svcName, svcName)) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		} else if (StrCompareType::partialMatchIgnoreCase == compareType) {
			if (partialMatch(vec[i].svcName, svcName)) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						vec[i]) == ret.end()) {
						ret.push_back(vec[i]);
					}
				} else {
					ret.push_back(vec[i]);
				}
			}
		}
	}
	return ret;
}

std::vector<std::wstring> SvcHandler::GetSvcExecutableByDisplayName(const std::wstring displayName,
	const StrCompareType compareType, const bool uniqueResults, const bool removeParameters,
	const unsigned long desiredRights, const std::vector<SvcRecord>* svcRecords) {
	std::vector<SvcRecord> vec;
	std::vector<std::wstring> ret;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights)) {
			return ret;
		}
	}
	for (size_t i = 0; i < vec.size(); ++i) {
		if (StrCompareType::equal == compareType) {
			if (vec[i].svcDisplayName == displayName) {
				std::wstring tempstr = vec[i].binaryPath;
				if (removeParameters) {
					size_t extPos = lower_copy(tempstr).find(L".exe");
					if (std::wstring::npos != extPos) {
						tempstr = tempstr.substr(0, extPos + 4);
					}
				}
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(tempstr)) == ret.end()) {
						ret.push_back(tempstr);
					}
				} else {
					ret.push_back(tempstr);
				}
			}
		} else if (StrCompareType::equalIgnoreCase == compareType) {
			if (lower_copy(vec[i].svcDisplayName) == lower_copy(displayName)) {
				std::wstring tempstr = vec[i].binaryPath;
				if (removeParameters) {
					size_t extPos = lower_copy(tempstr).find(L".exe");
					if (std::wstring::npos != extPos) {
						tempstr = tempstr.substr(0, extPos + 4);
					}
				}
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(tempstr)) == ret.end()) {
						ret.push_back(tempstr);
					}
				} else {
					ret.push_back(tempstr);
				}
			}
		} else if (StrCompareType::partialMatch == compareType) {
			if (partialMatch(vec[i].svcDisplayName, displayName, false)) {
				std::wstring tempstr = vec[i].binaryPath;
				if (removeParameters) {
					size_t extPos = lower_copy(tempstr).find(L".exe");
					if (std::wstring::npos != extPos) {
						tempstr = tempstr.substr(0, extPos + 4);
					}
				}
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(tempstr)) == ret.end()) {
						ret.push_back(tempstr);
					}
				} else {
					ret.push_back(tempstr);
				}
			}
		} else if (StrCompareType::partialMatchIgnoreCase == compareType) {
			if (partialMatch(vec[i].svcDisplayName, displayName)) {
				std::wstring tempstr = vec[i].binaryPath;
				if (removeParameters) {
					size_t extPos = lower_copy(tempstr).find(L".exe");
					if (std::wstring::npos != extPos) {
						tempstr = tempstr.substr(0, extPos + 4);
					}
				}
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(tempstr)) == ret.end()) {
						ret.push_back(tempstr);
					}
				} else {
					ret.push_back(tempstr);
				}
			}
		}
	}
	return ret;
}

std::vector<std::wstring> SvcHandler::GetSvcExecutableBySvcName(const std::wstring svcName,
	const StrCompareType compareType, const bool uniqueResults, const bool removeParameters,
	const unsigned long desiredRights, const std::vector<SvcRecord>* svcRecords) {
	std::vector<SvcRecord> vec;
	std::vector<std::wstring> ret;
	if (svcRecords) {
		vec = *svcRecords;
	}
	else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights)) {
			return ret;
		}
	}
	for (size_t i = 0; i < vec.size(); ++i) {
		if (StrCompareType::equal == compareType) {
			if (vec[i].svcName == svcName) {
				std::wstring tempstr = vec[i].binaryPath;
				if (removeParameters) {
					size_t extPos = lower_copy(tempstr).find(L".exe");
					if (std::wstring::npos != extPos) {
						tempstr = tempstr.substr(0, extPos + 4);
					}
				}
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(tempstr)) == ret.end()) {
						ret.push_back(tempstr);
					}
				} else {
					ret.push_back(tempstr);
				}
			}
		}
		else if (StrCompareType::equalIgnoreCase == compareType) {
			if (lower_copy(vec[i].svcName) == lower_copy(svcName)) {
				std::wstring tempstr = vec[i].binaryPath;
				if (removeParameters) {
					size_t extPos = lower_copy(tempstr).find(L".exe");
					if (std::wstring::npos != extPos) {
						tempstr = tempstr.substr(0, extPos + 4);
					}
				}
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(tempstr)) == ret.end()) {
						ret.push_back(tempstr);
					}
				} else {
					ret.push_back(tempstr);
				}
			}
		} else if (StrCompareType::partialMatch == compareType) {
			if (partialMatch(vec[i].svcName, svcName, false)) {
				std::wstring tempstr = vec[i].binaryPath;
				if (removeParameters) {
					size_t extPos = lower_copy(tempstr).find(L".exe");
					if (std::wstring::npos != extPos) {
						tempstr = tempstr.substr(0, extPos + 4);
					}
				}
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(tempstr)) == ret.end()) {
						ret.push_back(tempstr);
					}
				} else {
					ret.push_back(tempstr);
				}
			}
		} else if (StrCompareType::partialMatchIgnoreCase == compareType) {
			if (partialMatch(vec[i].svcName, svcName)) {
				std::wstring tempstr = vec[i].binaryPath;
				if (removeParameters) {
					size_t extPos = lower_copy(tempstr).find(L".exe");
					if (std::wstring::npos != extPos) {
						tempstr = tempstr.substr(0, extPos + 4);
					}
				}
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(tempstr)) == ret.end()) {
						ret.push_back(tempstr);
					}
				} else {
					ret.push_back(tempstr);
				}
			}
		}
	}
	return ret;
}

std::vector<std::wstring> SvcHandler::GetDisplayNameBySvcName(const std::wstring svcName,
	const StrCompareType compareType,
	const bool uniqueResults,
	const unsigned long desiredRights,
	const std::vector<SvcRecord>* svcRecords) {
	std::vector<SvcRecord> vec;
	std::vector<std::wstring> ret;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		if (ScmOpResult::Success != EnumSvcsStatus(vec, desiredRights)) {
			return ret;
		}
	}
	for (size_t i = 0; i < vec.size(); ++i) {
		if (StrCompareType::equal == compareType) {
			if (vec[i].svcName == svcName) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(vec[i].svcDisplayName)) == ret.end()) {
						ret.push_back(vec[i].svcDisplayName);
					}
				} else {
					ret.push_back(vec[i].svcDisplayName);
				}
			}
		} else if (StrCompareType::equalIgnoreCase == compareType) {
			if (lower_copy(vec[i].svcName) == lower_copy(svcName)) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(vec[i].svcDisplayName)) == ret.end()) {
						ret.push_back(vec[i].svcDisplayName);
					}
				} else {
					ret.push_back(vec[i].svcDisplayName);
				}
			}
		} else if (StrCompareType::partialMatch == compareType) {
			if (partialMatch(vec[i].svcName, svcName, false)) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(vec[i].svcDisplayName)) == ret.end()) {
						ret.push_back(vec[i].svcDisplayName);
					}
				} else {
					ret.push_back(vec[i].svcDisplayName);
				}
			}
		} else if (StrCompareType::partialMatchIgnoreCase == compareType) {
			if (partialMatch(vec[i].svcName, svcName)) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(vec[i].svcDisplayName)) == ret.end()) {
						ret.push_back(vec[i].svcDisplayName);
					}
				} else {
					ret.push_back(vec[i].svcDisplayName);
				}
			}
		}
	}
	return ret;
}

std::vector<std::wstring> SvcHandler::GetSvcNameByDisplayName(const std::wstring displayName,
	const StrCompareType compareType,
	const bool uniqueResults,
	const unsigned long desiredRights,
	const std::vector<SvcRecord>* svcRecords) {
	std::vector<SvcRecord> vec;
	std::vector<std::wstring> ret;
	if (svcRecords) {
		vec = *svcRecords;
	} else {
		ScmOpResult res = EnumSvcsStatus(vec, desiredRights);
		if (ScmOpResult::Success != res) {
			return ret;
		}
	}
	for (size_t i = 0; i < vec.size(); ++i) {
		if (StrCompareType::equal == compareType) {
			if (vec[i].svcDisplayName == displayName) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(vec[i].svcName)) == ret.end()) {
						ret.push_back(vec[i].svcName);
					}
				} else {
					ret.push_back(vec[i].svcName);
				}
			}
		} else if (StrCompareType::equalIgnoreCase == compareType) {
			if (lower_copy(vec[i].svcDisplayName) == lower_copy(displayName)) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(vec[i].svcName)) == ret.end()) {
						ret.push_back(vec[i].svcName);
					}
				} else {
					ret.push_back(vec[i].svcName);
				}
			}
		} else if (StrCompareType::partialMatch == compareType) {
			if (partialMatch(vec[i].svcDisplayName, displayName, false)) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(vec[i].svcName)) == ret.end()) {
						ret.push_back(vec[i].svcName);
					}
				} else {
					ret.push_back(vec[i].svcName);
				}
			}
		} else if (StrCompareType::partialMatchIgnoreCase == compareType) {
			if (partialMatch(vec[i].svcDisplayName, displayName)) {
				if (uniqueResults) {
					if (std::find(ret.begin(), ret.end(),
						lower_copy(vec[i].svcName)) == ret.end()) {
						ret.push_back(vec[i].svcName);
					}
				} else {
					ret.push_back(vec[i].svcName);
				}
			}
		}
	}
	return ret;
}

ScmOpResult SvcHandler::StopDependentSvc(const SvcRecord& svcRec, const bool stopDrivers,
	const unsigned long timeout, const unsigned long desiredRights, const std::vector<SvcRecord>* svcRecords) {
	for (size_t i = 0; i < svcRec.dependencies.size(); ++i) {
		std::vector<SvcRecord> depsvc = GetSvcBySvcName(svcRec.dependencies[i], StrCompareType::equalIgnoreCase,
			true, desiredRights, svcRecords);
		if (IsSvcDriver(depsvc[0])) {
			if (stopDrivers) {
				StopDependentSvc(depsvc[0], stopDrivers, timeout, desiredRights, svcRecords);
			} else {
				continue;
			}
		}
		if (!stopDrivers && IsSvcDriver(svcRec)) {
			return ScmOpResult::StopSvcFail;
		}
		if (ScmOpResult::Success != ObtainSCMHanele(desiredRights)) {
			return ScmOpResult::StopSvcFail;
		}
		::SC_HANDLE hSvc = ::OpenService(m_scmHandle, svcRec.svcName.c_str(),
			SERVICE_STOP | SERVICE_QUERY_STATUS);
		if (hSvc && INVALID_HANDLE_VALUE != hSvc) {
			SERVICE_STATUS_PROCESS svcStatus = { 0 };
			unsigned long dwBytesNeeded = 0, dwWaitTime = 0;
			if (QueryServiceStatusEx(hSvc, SC_STATUS_PROCESS_INFO, (unsigned char*)&svcStatus,
				sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
				if (svcStatus.dwCurrentState != SERVICE_STOPPED) {
					while (svcStatus.dwCurrentState == SERVICE_STOP_PENDING) {
						dwWaitTime = svcStatus.dwWaitHint / 10;
						if (dwWaitTime < 1000) {
							dwWaitTime = 1000;
						} else if (dwWaitTime > 10000) {
							dwWaitTime = 10000;
						}
						Sleep(dwWaitTime);
					}
					if (QueryServiceStatusEx(hSvc, SC_STATUS_PROCESS_INFO,
						(unsigned char*)&svcStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
						if (svcStatus.dwCurrentState == SERVICE_STOPPED) {
							::CloseServiceHandle(hSvc);
							return ScmOpResult::Success;
						}
					}
					if (ControlService(hSvc, SERVICE_CONTROL_STOP,
						(LPSERVICE_STATUS)&svcStatus)) {
						unsigned long dwStartTime = GetTickCount64();
						while (svcStatus.dwCurrentState == SERVICE_STOP_PENDING) {
							dwWaitTime = svcStatus.dwWaitHint / 10;
							if (dwWaitTime < 1000) {
								dwWaitTime = 1000;
							} else if (dwWaitTime > 10000) {
								dwWaitTime = 10000;
							}
							Sleep(dwWaitTime);
							if (QueryServiceStatusEx(hSvc, SC_STATUS_PROCESS_INFO,
								(unsigned char*)&svcStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
								if (svcStatus.dwCurrentState == SERVICE_STOPPED) {
									::CloseServiceHandle(hSvc);
									return ScmOpResult::Success;
								} else if (GetTickCount64() - dwStartTime > timeout) {
									::CloseServiceHandle(hSvc);
									return ScmOpResult::StopSvcFail;
								}
							} else {
								::CloseServiceHandle(hSvc);
								return ScmOpResult::StopSvcFail;
							}
						}
						if (QueryServiceStatusEx(hSvc, SC_STATUS_PROCESS_INFO,
							(unsigned char*)&svcStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
							if (svcStatus.dwCurrentState == SERVICE_STOPPED) {
								::CloseServiceHandle(hSvc);
								return ScmOpResult::Success;
							}
						}
					} else {
						::CloseServiceHandle(hSvc);
						return ScmOpResult::StopSvcFail;
					}
				} else {
					::CloseServiceHandle(hSvc);
					return ScmOpResult::StopSvcFailAlreadyStopped;
				}
			} else {
				::CloseServiceHandle(hSvc);
				return ScmOpResult::StopSvcFail;
			}
		} else {
			return ScmOpResult::StopSvcFail;
		}
	}
	return ScmOpResult::Success;
}

inline ScmOpResult SvcHandler::ObtainSCMHanele(const unsigned long desiredRights) {
	if (!m_scmHandle) {
		if (ScmOpResult::Success != SCM_OpenManager(desiredRights)) {
#ifdef _DEBUG
			errorMsgBox(getErrorDetails(getLastErrorCode(), L"OpenSCManager"));
#endif
			return ScmOpResult::OpenManagerFail;
		}
	}
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::QuerySvcDesc(::SC_HANDLE& svcHandle,  SvcRecord& rec) {
	DWORD BufSz = 0, dwBN = 0;
	void* Buf = 0;
	do {
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
		BufSz = dwBN;
		Buf = (SERVICE_DESCRIPTION*)malloc(BufSz);
		if (!Buf) {
			return ScmOpResult::EnumSvcsFail;
		}
		if (QueryServiceConfig2(svcHandle, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)Buf, BufSz, &dwBN)) {
			SERVICE_DESCRIPTION* BufTyped = (SERVICE_DESCRIPTION*)Buf;
			if (wcslen_c(BufTyped->lpDescription)) {
				rec.svcDesc = BufTyped->lpDescription;
			}
			SAFE_FREE(Buf);
			// free(Buf);
			// Buf = 0;
			BufTyped = 0;
			break;
		}
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
	} while (getLastErrorCode() == ERROR_INSUFFICIENT_BUFFER);
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::QuerySvcDelayedAutostart(::SC_HANDLE& svcHandle,  SvcRecord& rec) {
	DWORD BufSz = 0, dwBN = 0;
	void* Buf = 0;
	do {
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
		BufSz = dwBN;
		Buf = (SERVICE_DELAYED_AUTO_START_INFO*)malloc(BufSz);
		if (!Buf) {
			return ScmOpResult::EnumSvcsFail;
		}
		if (QueryServiceConfig2(svcHandle, SERVICE_CONFIG_DELAYED_AUTO_START_INFO, (LPBYTE)Buf, BufSz, &dwBN)) {
			SERVICE_DELAYED_AUTO_START_INFO* BufTyped = (SERVICE_DELAYED_AUTO_START_INFO*)Buf;
			rec.delayedAutoStart = BufTyped->fDelayedAutostart;
			SAFE_FREE(Buf);
			// free(Buf);
			// Buf = 0;
			BufTyped = 0;
			break;
		}
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
	} while (getLastErrorCode() == ERROR_INSUFFICIENT_BUFFER);
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::QuerySvcFailureActions(::SC_HANDLE& svcHandle,  SvcRecord& rec) {
	DWORD BufSz = 0, dwBN = 0;
	void* Buf = 0;
	do {
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
		BufSz = dwBN;
		Buf = (SERVICE_FAILURE_ACTIONS*)malloc(BufSz);
		if (!Buf) {
			return ScmOpResult::EnumSvcsFail;
		}
		if (QueryServiceConfig2(svcHandle, SERVICE_CONFIG_FAILURE_ACTIONS, (LPBYTE)Buf, BufSz, &dwBN)) {
			SERVICE_FAILURE_ACTIONS* svcfailact =
				(SERVICE_FAILURE_ACTIONS*)Buf;
			if (wcslen_c(svcfailact->lpCommand)) {
				rec.failCommand = svcfailact->lpCommand;
			}
			if (wcslen_c(svcfailact->lpRebootMsg)) {
				rec.failResetMsg = svcfailact->lpRebootMsg;
			}
			rec.failResetPeriod = svcfailact->dwResetPeriod;
			rec.failActions = svcfailact->cActions;
			if (nullptr != svcfailact->lpsaActions) {
				rec.failActionType = svcfailact->lpsaActions->Type;
				rec.failActionDelay = svcfailact->lpsaActions->Delay;
			}
			SAFE_FREE(Buf);
			// free(Buf);
			// Buf = 0;
			break;
		}
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
	} while (getLastErrorCode() == ERROR_INSUFFICIENT_BUFFER);
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::QuerySvcFailureActionsFlag(::SC_HANDLE& svcHandle,  SvcRecord& rec) {
	DWORD BufSz = 0, dwBN = 0;
	void* Buf = 0;
	do {
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
		BufSz = dwBN;
		Buf = (SERVICE_FAILURE_ACTIONS_FLAG*)malloc(BufSz);
		if (!Buf) {
			return ScmOpResult::EnumSvcsFail;
		}
		if (QueryServiceConfig2(svcHandle, SERVICE_CONFIG_FAILURE_ACTIONS_FLAG, (LPBYTE)Buf, BufSz, &dwBN)) {
			SERVICE_FAILURE_ACTIONS_FLAG* BufTyped = (SERVICE_FAILURE_ACTIONS_FLAG*)Buf;
			rec.failureActionsOnNonCrashFailures = BufTyped->fFailureActionsOnNonCrashFailures;
			SAFE_FREE(Buf);
			// free(Buf);
			// Buf = 0;
			BufTyped = 0;
			break;
		}
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
	} while (getLastErrorCode() == ERROR_INSUFFICIENT_BUFFER);
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::QuerySvcPreferredNode(::SC_HANDLE& svcHandle,  SvcRecord& rec) {
	DWORD BufSz = 0, dwBN = 0;;
	void* Buf = 0;
	do {
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
		BufSz = dwBN;
		Buf = (SERVICE_PREFERRED_NODE_INFO*)malloc(BufSz);
		if (!Buf) {
			return ScmOpResult::EnumSvcsFail;
		}
		if (QueryServiceConfig2(svcHandle, SERVICE_CONFIG_PREFERRED_NODE, (LPBYTE)Buf, BufSz, &dwBN)) {
			SERVICE_PREFERRED_NODE_INFO* BufTyped = (SERVICE_PREFERRED_NODE_INFO*)Buf;
			rec.preferredNode = BufTyped->usPreferredNode;
			rec.preferredNodeDeleteFlag = BufTyped->fDelete;
			SAFE_FREE(Buf);
			// free(Buf);
			// Buf = 0;
			BufTyped = 0;
			break;
		}
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
	} while (getLastErrorCode() == ERROR_INSUFFICIENT_BUFFER);
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::QuerySvcPreshutdownInfo(::SC_HANDLE& svcHandle,  SvcRecord& rec) {
	DWORD BufSz = 0, dwBN = 0;
	void* Buf = 0;
	do {
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
		BufSz = dwBN;
		Buf = (SERVICE_PRESHUTDOWN_INFO*)malloc(BufSz);
		if (!Buf) {
			return ScmOpResult::EnumSvcsFail;
		}
		if (QueryServiceConfig2(svcHandle, SERVICE_CONFIG_PRESHUTDOWN_INFO, (LPBYTE)Buf, BufSz, &dwBN)) {
			SERVICE_PRESHUTDOWN_INFO* BufTyped = (SERVICE_PRESHUTDOWN_INFO*)Buf;
			rec.preshutdownTimeout = BufTyped->dwPreshutdownTimeout;
			SAFE_FREE(Buf);
			// free(Buf);
			// Buf = 0;
			BufTyped = 0;
			break;
		}
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
	} while (getLastErrorCode() == ERROR_INSUFFICIENT_BUFFER);
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::QuerySvcPrivileges(::SC_HANDLE& svcHandle,  SvcRecord& rec) {
	DWORD BufSz = 0, dwBN = 0;
	void* Buf = 0;
	do {
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
		BufSz = dwBN;
		Buf = (SERVICE_REQUIRED_PRIVILEGES_INFO*)malloc(BufSz);
		if (!Buf) {
			return ScmOpResult::EnumSvcsFail;
		}
		if (QueryServiceConfig2(svcHandle, SERVICE_CONFIG_REQUIRED_PRIVILEGES_INFO, (LPBYTE)Buf, BufSz, &dwBN)) {
			SERVICE_REQUIRED_PRIVILEGES_INFO* BufTyped = (SERVICE_REQUIRED_PRIVILEGES_INFO*)Buf;
			if (wcslen_c(BufTyped->pmszRequiredPrivileges)) {
				NEW_ARR_NULLIFY(tmpBuf, wchar_t, BufSz);
				// wchar_t* tmpBuf = (wchar_t*)calloc(BufSz, BufSz * sizeof(wchar_t));
				if (tmpBuf) {
					memcpy(tmpBuf, BufTyped->pmszRequiredPrivileges, (BufSz - 1) /* * sizeof(wchar_t) */ );
					wchar_t* newBuf = 0;
					newBuf = replaceChars(tmpBuf, L"\0", L"\n", BufSz - 1, 1);
					if (!newBuf) {
						return ScmOpResult::OpenManagerFail;
					}
					std::wstring tmpstr = newBuf;
					tmpstr.erase(remove_if(tmpstr.begin(), tmpstr.end(),
						[](wchar_t wc) { return !(wc >= 0 && wc <= 128); }),
						tmpstr.end());
					for (size_t i = 0; i < tmpstr.length(); ++i) {
						if (static_cast<int>(tmpstr[i]) == 10 &&
							static_cast<int>(tmpstr[i + 1]) == 10) {
							tmpstr.erase(i, tmpstr.length() - i);
							break;
						}
					}
					rec.privileges = tmpstr;
					SAFE_ARR_DELETE(newBuf);
					SAFE_ARR_DELETE(tmpBuf);
				} else {
					return ScmOpResult::EnumSvcsFail;
				}
			}
			SAFE_FREE(Buf);
			// free(Buf);
			// Buf = 0;
			BufTyped = 0;
			break;
		}
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
	} while (getLastErrorCode() == ERROR_INSUFFICIENT_BUFFER);
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::QuerySvcSIDInfo(::SC_HANDLE& svcHandle,  SvcRecord& rec) {
	DWORD BufSz = 0, dwBN = 0;
	void* Buf = 0;
	do {
		free(Buf);
		Buf = 0;
		BufSz = dwBN;
		Buf = (SERVICE_SID_INFO*)malloc(BufSz);
		if (!Buf) {
			return ScmOpResult::EnumSvcsFail;
		}
		if (QueryServiceConfig2(svcHandle, SERVICE_CONFIG_SERVICE_SID_INFO, (LPBYTE)Buf, BufSz, &dwBN)) {
			SERVICE_SID_INFO* BufTyped = (SERVICE_SID_INFO*)Buf;
			rec.svcSidType = BufTyped->dwServiceSidType;
			BufTyped = 0;
			SAFE_FREE(Buf);
			// free(Buf);
			// Buf = 0;
			break;
		}
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
	} while (getLastErrorCode() == ERROR_INSUFFICIENT_BUFFER);
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::QuerySvcTriggerInfo(::SC_HANDLE& svcHandle,  SvcRecord& rec) {
	DWORD BufSz = 0, dwBN = 0;
	void* Buf = 0;
	do {
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
		BufSz = dwBN;
		Buf = (SERVICE_TRIGGER_INFO*)malloc(BufSz);
		if (!Buf) {
			return ScmOpResult::EnumSvcsFail;
		}
		if (QueryServiceConfig2(svcHandle, SERVICE_CONFIG_TRIGGER_INFO, (LPBYTE)Buf, BufSz, &dwBN)) {
			SERVICE_TRIGGER_INFO* BufTyped = (SERVICE_TRIGGER_INFO*)Buf;
			if (BufTyped->cTriggers) {
				SvcTrigger trg;
				for (size_t i = 0; i < BufTyped->cTriggers; ++i) {
					// trg.triggerData.clear();
					trg.triggerType = BufTyped->pTriggers[i].dwTriggerType;
					trg.triggerAction = BufTyped->pTriggers[i].dwAction;
					trg.triggerGuid = GUID2wstrMS(*BufTyped->pTriggers[i].pTriggerSubtype);
					unsigned long trgDataSz = BufTyped->pTriggers[i].cDataItems;
					if (trgDataSz) {
						SvcTriggerData svctrgDataItem;
						for (size_t j = 0; j < trgDataSz; ++j) {
							svctrgDataItem.triggerDataType = BufTyped->pTriggers[i].pDataItems[j].dwDataType;
							svctrgDataItem.triggerDataSize = BufTyped->pTriggers[i].pDataItems[j].cbData;
							svctrgDataItem.triggerData.reserve(svctrgDataItem.triggerDataSize);
							if (svctrgDataItem.triggerDataSize) {
								if (svctrgDataItem.triggerDataType == 2) {
									wchar_t* tmpBuf = (wchar_t*)calloc(svctrgDataItem.triggerDataSize, svctrgDataItem.triggerDataSize * sizeof(wchar_t));
									if (tmpBuf) {
										memcpy(tmpBuf,
											BufTyped->pTriggers[i].pDataItems[j].pData,
											svctrgDataItem.triggerDataSize * sizeof(wchar_t));
										wchar_t* newBuf = replaceChars(tmpBuf, L"\0", L"\n", svctrgDataItem.triggerDataSize - 1, 1);
										std::wstring tmpstr = newBuf;
										tmpstr.erase(remove_if(tmpstr.begin(), tmpstr.end(),
											[](wchar_t wc) { return !(wc >= 0 && wc <= 128); }),
											tmpstr.end());
										for (size_t k = 0; k < tmpstr.length(); ++k) {
											if (static_cast<int>(tmpstr[k]) == 10 &&
												static_cast<int>(tmpstr[k + 1]) == 10) {
												tmpstr.erase(k, tmpstr.length() - i);
												break;
											}
										}
										unsigned char* tbuf = (unsigned char*)wchar2char(tmpstr.c_str());
										if (tbuf) {
											svctrgDataItem.triggerData =
												std::vector<unsigned char>(
													tbuf, tbuf +
													BufTyped->pTriggers[i].pDataItems[j].cbData);
											SAFE_FREE(tbuf);
											// free(tbuf);
											// tbuf = 0;
										} else {
											SAFE_ARR_DELETE(newBuf);
											return ScmOpResult::EnumSvcsFail;
										}
										SAFE_FREE(newBuf);
										SAFE_FREE(tmpBuf);
									} else {
										return ScmOpResult::EnumSvcsFail;
									}
								} else {
									svctrgDataItem.triggerData =
										svctrgDataItem.triggerData =
										std::vector<unsigned char>(
											BufTyped->pTriggers[i].pDataItems[j].pData,
											BufTyped->pTriggers[i].pDataItems[j].pData +
											BufTyped->pTriggers[i].pDataItems[j].cbData);
								}
							}
						}
						trg.triggerData.push_back(svctrgDataItem);
					}
					rec.triggers.push_back(trg);
				}

			}
			SAFE_FREE(Buf);
			// free(Buf);
			// Buf = 0;
			BufTyped = 0;
			break;
		}
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
	} while (getLastErrorCode() == ERROR_INSUFFICIENT_BUFFER);
	return ScmOpResult::Success;
}

ScmOpResult SvcHandler::QuerySvcLaunchProtected(::SC_HANDLE& svcHandle,  SvcRecord& rec) {
	DWORD BufSz = 0, dwBN = 0;
	void* Buf = 0;
	SERVICE_LAUNCH_PROTECTED_INFO* launchProtBuf = { 0 };
	do {
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
		BufSz = dwBN;
		Buf = (SERVICE_LAUNCH_PROTECTED_INFO*)malloc(BufSz);
		if (!Buf) {
			return ScmOpResult::EnumSvcsFail;
		}
		if (QueryServiceConfig2(svcHandle, SERVICE_CONFIG_LAUNCH_PROTECTED, (LPBYTE)Buf, BufSz, &dwBN)) {
			SERVICE_LAUNCH_PROTECTED_INFO* BufTyped = (SERVICE_LAUNCH_PROTECTED_INFO*)Buf;
			rec.launchProtected = BufTyped->dwLaunchProtected;
			SAFE_FREE(Buf);
			// free(Buf);
			// Buf = 0;
			BufTyped = 0;
			break;
		}
		SAFE_FREE(Buf);
		// free(Buf);
		// Buf = 0;
	} while (getLastErrorCode() == ERROR_INSUFFICIENT_BUFFER);
	return ScmOpResult::Success;
}