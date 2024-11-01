#include <iostream>
#include "reghelper.h"

RegValDesc::RegValDesc() {
	valData = { 0 };
	valDataSz = 0;
	valType = RegValType::Str;
}

RegKeyDesc::RegKeyDesc() {}

RegKeyDesc::RegKeyDesc(const RegKeyDesc &other) {
	if (this != &other) {
		keyPath = other.keyPath;
		keyName = other.keyName;
		keys = other.keys;
		values = other.values;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
RegKeyDesc::RegKeyDesc(RegKeyDesc &&other) noexcept {
	if (this != &other) {
		keyPath = std::move(other.keyPath);
		keyName = std::move(other.keyName);
		keys = std::move(other.keys);
		values = std::move(other.values);
	}
}
#endif

RegKeyDesc::~RegKeyDesc() {}

void RegKeyDesc::FreeValues() {
	for (size_t i = 0; i < values.size(); ++i) {
		values[i].FreeData();
	}
}

RegKeyDesc& RegKeyDesc::operator=(const RegKeyDesc &other) {
	if (this != &other) {
		keyPath = other.keyPath;
		keyName = other.keyName;
		keys = other.keys;
		values = other.values;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
RegKeyDesc& RegKeyDesc::operator=(RegKeyDesc &&other) noexcept {
	if (this != &other) {
		keyPath = std::move(other.keyPath);
		keyName = std::move(other.keyName);
		keys = std::move(other.keys);
		values = std::move(other.values);
	}
	return *this;
}
#endif

bool RegKeyDesc::operator==(const RegKeyDesc& other) const {
	if (this != &other) {
		return (lower_copy(keyPath) == lower_copy(other.keyPath) &&
				lower_copy(keyName) == lower_copy(other.keyName) &&
				keys == other.keys &&
				values == other.values);
	} else {
		return true;
	}
}

bool RegKeyDesc::operator!=(const RegKeyDesc& other) const {
	if (this != &other) {
		return (lower_copy(keyPath) != lower_copy(other.keyPath) ||
				lower_copy(keyName) != lower_copy(other.keyName) ||
				keys != other.keys ||
				values != other.values);
	} else {
		return false;
	}
}

RegValDesc::RegValDesc(const RegValDesc &other) {
	/*size_t sz = 0;
	if (valType == RegValType::Str || valType == RegValType::MultiStr ||
		valType == RegValType::ExpandStr || valType == RegValType::Link) {
		sz = sizeof(wchar_t);
	} else if (valType == RegValType::DWord || valType == RegValType::DWordLE ||
		valType == RegValType::DWordBE) {
		sz = sizeof(unsigned long);
	} else if (valType == RegValType::QWord || valType == RegValType::QWordLE) {
		sz = sizeof(unsigned long long);
	} else if (other.valType == RegValType::Binary ||
		other.valType == RegValType::None) {
		sz = sizeof(unsigned char);
	}*/
	if (this != &other) {
		valDataSz = other.valDataSz;
		valData = { 0 };
		if (other.valDataSz && other.valData) {
			if (other.valType == RegValType::Str || other.valType == RegValType::MultiStr ||
				other.valType == RegValType::ExpandStr || other.valType == RegValType::Link) {
				NEW_ARR_NULLIFY(valData, wchar_t, (valDataSz / sizeof(wchar_t)) * sizeof(char));
			} else if (other.valType == RegValType::DWord || other.valType == RegValType::DWordLE ||
				other.valType == RegValType::DWordBE) {
				NEW_NULLIFY(valData, unsigned long, valDataSz / sizeof(unsigned long));
			} else if (other.valType == RegValType::QWord || other.valType == RegValType::QWordLE) {
				NEW_NULLIFY(valData, unsigned long long, valDataSz / sizeof(unsigned long long));
			} else if (other.valType == RegValType::Binary) {
				NEW_ARR_NULLIFY(valData, unsigned char, valDataSz / sizeof(unsigned char));
			} else if (other.valType == RegValType::None) {
			}
			if (valData && other.valType != RegValType::None) {
				memcpy(valData, other.valData, valDataSz); // *sz);
			}
		} else {
			valData = 0;
		}
		valType = other.valType;
		valPath = other.valPath;
		valName = other.valName;
		valDataHex = other.valDataHex;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
RegValDesc::RegValDesc(RegValDesc &&other) noexcept {
	if (this != &other) {
		valType = std::exchange(other.valType, RegValType::None);
		valDataSz = std::exchange(other.valDataSz, 0);
		valPath = std::move(other.valPath);
		valName = std::move(other.valName);
		valDataHex = std::move(other.valDataHex);
		valData = std::move(other.valData);
		other.valData = 0;
	}
}
#endif

RegValDesc& RegValDesc::operator=(const RegValDesc &other) {
	/*size_t sz = 0;
	if (valType == RegValType::Str || valType == RegValType::MultiStr ||
		valType == RegValType::ExpandStr || valType == RegValType::Link) {
		sz = sizeof(wchar_t);
	} else if (valType == RegValType::DWord || valType == RegValType::DWordLE ||
		valType == RegValType::DWordBE) {
		sz = sizeof(unsigned long);
	} else if (valType == RegValType::QWord || valType == RegValType::QWordLE) {
		sz = sizeof(unsigned long long);
	} else if (other.valType == RegValType::Binary ||
		other.valType == RegValType::None) {
		sz = sizeof(unsigned char);
	}*/
	if (this != &other) {
		valDataSz = other.valDataSz;
		valData = { 0 };
		if (other.valDataSz && other.valData && !IsBadReadPtr(other.valData)) {
			if (other.valType == RegValType::Str || other.valType == RegValType::MultiStr ||
				other.valType == RegValType::ExpandStr || other.valType == RegValType::Link) {
				NEW_ARR_NULLIFY(valData, wchar_t, (valDataSz / sizeof(wchar_t)) * sizeof(char));
			} else if (other.valType == RegValType::DWord ||
				other.valType == RegValType::DWordLE ||
				other.valType == RegValType::DWordBE) {
				NEW_NULLIFY(valData, unsigned long, valDataSz / sizeof(unsigned long));
			} else if (other.valType == RegValType::QWord ||
				other.valType == RegValType::QWordLE) {
				NEW_NULLIFY(valData, unsigned long long, valDataSz / sizeof(unsigned long long));
			} else if (other.valType == RegValType::Binary) {
				NEW_ARR_NULLIFY(valData, unsigned char, valDataSz / sizeof(unsigned char));
			} else if (other.valType == RegValType::None) {
			}
			if (valData && other.valType != RegValType::None) {
				memcpy(valData, other.valData, valDataSz); // *sz);
			}
		}
		valType = other.valType;
		valPath = other.valPath;
		valName = other.valName;
		valDataHex = other.valDataHex;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
RegValDesc& RegValDesc::operator=(RegValDesc &&other) noexcept {
	if (this != &other) {
		valType = std::exchange(other.valType, RegValType::None);
		valDataSz = std::exchange(other.valDataSz, 0);
		valData = std::move(other.valData);
		other.valData = 0;
		valPath = std::move(other.valPath);
		valName = std::move(other.valName);
		valDataHex = std::move(other.valDataHex);
	}
	return *this;
}
#endif

RegValDesc::~RegValDesc() {
	if (valType == RegValType::DWord || valType == RegValType::DWordLE ||
		valType == RegValType::DWordBE || valType == RegValType::QWord ||
		valType == RegValType::QWordLE) {
		if (valData) {
			SAFE_DELETE(valData);
		}
	} else {
		if (valDataSz) {
			if (valDataSz == 1) {
				SAFE_DELETE(valData);
			} else {
				SAFE_ARR_DELETE(valData);
			}
		}
	}
}

void RegValDesc::FreeData() {
	SAFE_ARR_DELETE(valData);
}

bool RegValDesc::operator==(const RegValDesc &other) const {
	/*size_t sz = 0;
	if (valType == RegValType::Str || valType == RegValType::MultiStr ||
		valType == RegValType::ExpandStr || valType == RegValType::Link) {
		sz = sizeof(wchar_t);
	} else if (valType == RegValType::DWord || valType == RegValType::DWordLE ||
		valType == RegValType::DWordBE) {
		sz = sizeof(unsigned long);
	} else if (valType == RegValType::QWord || valType == RegValType::QWordLE) {
		sz = sizeof(unsigned long long);
	} else if (valType == RegValType::Binary || valType == RegValType::None) {
		sz = sizeof(unsigned char);
	}*/
	if (this != &other) {
		bool bufeq = false, otheq = false;
		if (valData && other.valData) {
			if (memcmp(valData, other.valData, valDataSz)) {
				bufeq = false;
			} else {
				bufeq = true;
			}
		} else if ((!valData && other.valData) || (valData && !other.valData)) {
			bufeq = false;
		} else if (!valData && !other.valData) {
			bufeq = true;
		}
		otheq = (lower_copy(valPath) == lower_copy(other.valPath) &&
				lower_copy(valName) == lower_copy(other.valName) &&
				lower_copy(valDataHex) == lower_copy(other.valDataHex) &&
				valDataSz == other.valDataSz &&
				valType == other.valType);
		return (bufeq && otheq);
	} else {
		return true;
	}
}

bool RegValDesc::operator!=(const RegValDesc& other) const {
	/*size_t sz = 0;
	if (valType == RegValType::Str || valType == RegValType::MultiStr ||
		valType == RegValType::ExpandStr || valType == RegValType::Link) {
		sz = sizeof(wchar_t);
	} else if (valType == RegValType::DWord || valType == RegValType::DWordLE ||
		valType == RegValType::DWordBE) {
		sz = sizeof(unsigned long);
	} else if (valType == RegValType::QWord || valType == RegValType::QWordLE) {
		sz = sizeof(unsigned long long);
	} else if (valType == RegValType::Binary || valType == RegValType::None) {
		sz = sizeof(unsigned char);
	}*/
	if (this != &other) {
		bool bufneq = false, othneq = false;
		if (valData && other.valData) {
			if (memcmp(valData, other.valData, valDataSz)) {
				bufneq = true;
			} else {
				bufneq = false;
			}
		} else if ((!valData && other.valData) || (valData && !other.valData)) {
			bufneq = true;
		} else if (!valData && !other.valData) {
			bufneq = false;
		}
		othneq = (lower_copy(valPath) != lower_copy(other.valPath) ||
				lower_copy(valName) != lower_copy(other.valName) ||
				lower_copy(valDataHex) != lower_copy(other.valDataHex) ||
				valDataSz != other.valDataSz ||
				valType != other.valType);
		return (bufneq || othneq);
	} else {
		return false;
	}
}

RegHandler::RegHandler() {
	m_mountedHiveCount = 0;
}

RegHandler::RegHandler(const RegHandler &other) {
	if (this != &other) {
		m_mountedHiveCount = other.m_mountedHiveCount;
		m_mountedHives = other.m_mountedHives;
		m_connectedRegs = other.m_connectedRegs;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
RegHandler::RegHandler(RegHandler &&other) noexcept {
	if (this != &other) {
		m_mountedHiveCount = std::exchange(other.m_mountedHiveCount, 0);
		m_mountedHives = std::move(other.m_mountedHives);
		m_connectedRegs = std::move(other.m_connectedRegs);
	}
}
#endif

RegHandler::~RegHandler() {
	if (m_mountedHives.size()) {
		for (auto &it : m_mountedHives) {
			UnmountHive_UnloadKey(it.first, pickUnloadKey(it.second));
		}
	}
	if (m_connectedRegs.size()) {
		for (auto& it : m_connectedRegs) {
			DisconnectRegistry(it.second);
		}
	}
}

RegHandler& RegHandler::operator=(const RegHandler &other) {
	if (this != &other) {
		m_mountedHiveCount = other.m_mountedHiveCount;
		m_mountedHives = other.m_mountedHives;
		m_connectedRegs = other.m_connectedRegs;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
RegHandler& RegHandler::operator=(RegHandler &&other) noexcept {
	if (this != &other) {
		m_mountedHiveCount = std::exchange(other.m_mountedHiveCount, 0);
		m_mountedHives = std::move(other.m_mountedHives);
		m_connectedRegs = std::move(other.m_connectedRegs);
	}
	return *this;
}
#endif

bool RegHandler::operator==(const RegHandler &other) const {
	if (this != &other) {
		return (m_mountedHiveCount == other.m_mountedHiveCount &&
				m_mountedHives == other.m_mountedHives &&
				m_connectedRegs == other.m_connectedRegs);
	} else {
		return true;
	}
}

bool RegHandler::operator!=(const RegHandler &other) const {
	if (this != &other) {
		return (m_mountedHiveCount != other.m_mountedHiveCount ||
				m_mountedHives != other.m_mountedHives ||
				m_connectedRegs != other.m_connectedRegs);
	} else {
		return false;
	}
}

RegOpResult RegHandler::GetStrVal(const std::wstring valName, std::wstring &val,
	const bool cleanupString, const HKEY *root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 }, rootKeyPrep = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0, KEY_READ | getRightMod(), &keyHandle)) {
			unsigned long buflen = REG_READBUFSZ;
			if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0, 0, 0, &buflen)) {
				if (!buflen) {
					val = L"";
					return RegOpResult::Success;
				}
				wchar_t* readBuf = (wchar_t*)malloc(buflen);
				if (readBuf) {
					memset(readBuf, 0, buflen);
					if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0, 0, (unsigned char*)readBuf,
						&buflen)) {
						val = readBuf;
						if (cleanupString) {
							val.erase(remove_if(val.begin(), val.end(),
								[](wchar_t wc) { return !(wc >= 0 && wc <= 255); }),
								val.end());
						}
						SAFE_FREE(readBuf);
						CLOSEKEY_NULLIFY(keyHandle);
						return RegOpResult::Success;
					}
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				}
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Fail;
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::GetDWordVal(const std::wstring valName, unsigned long &val, const HKEY *root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0, KEY_READ | getRightMod(), &keyHandle)) {
			unsigned long buflen = sizeof(unsigned long);
			unsigned char readBuf[sizeof(unsigned long)] = { 0 };
			if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0, 0,
				reinterpret_cast<unsigned char*>(&val), &buflen)) {
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Success;
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::GetQWordVal(const std::wstring valName, unsigned long long& val, const HKEY* root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0, KEY_READ | getRightMod(), &keyHandle)) {
			unsigned long buflen = 16; // sizeof(unsigned long);
			unsigned char readBuf[sizeof(unsigned long long)] = { 0 };
			if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0, 0,
				reinterpret_cast<unsigned char*>(&val), &buflen)) {
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Success;
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::GetBinaryVal(const std::wstring valName, unsigned char* &buf,
	unsigned long &bufSz, const HKEY* root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0, KEY_READ | getRightMod(), &keyHandle)) {
			unsigned long type = KEY_READ, buflen = REG_READBUFSZ;
			if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0, 0,
				0, &buflen)) {
				bufSz = buflen;
				buf = new unsigned char[buflen / sizeof(unsigned char)];
				if (buf) {
					// memset(buf, 0, buflen);
					if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0, 0,
						(unsigned char*)buf, &buflen)) {
						CLOSEKEY_NULLIFY(keyHandle);
						return RegOpResult::Success;
					}
				} else {
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				}
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::GetMultiStrVal(const std::wstring valName, wchar_t* &buf, 
	unsigned long &bufSz, const HKEY* root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0, KEY_READ | getRightMod(), &keyHandle)) {
			unsigned long type = KEY_READ, buflen = REG_READBUFSZ;
			if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0, 0,
				0, &buflen)) {
				bufSz = buflen;
				NEW_ARR_NULLIFY_NO_REDEFINE(buf, wchar_t, bufSz / sizeof(wchar_t) + 1);
				if (buf) {
					if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0, 0,
						(unsigned char*)buf, &buflen)) {
						CLOSEKEY_NULLIFY(keyHandle);
						return RegOpResult::Success;
					}
				} else {
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				}
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::GetMultiStrVal(const std::wstring valName,
	std::vector<std::wstring> &val, const bool split,
	const wchar_t* splitter, const HKEY *root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0, KEY_READ | getRightMod(), &keyHandle)) {
			unsigned long buflen = 0;
			if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0, 0,
				0, &buflen)) {
				NEW_ARR_NULLIFY(readBuf, wchar_t, (buflen / sizeof(wchar_t)) + 1);
				if (readBuf) {
					if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0, 0,
						(unsigned char*)readBuf, &buflen)) {
						CLOSEKEY_NULLIFY(keyHandle);
						wchar_t* newBuf = 0;
						size_t spllen = wcslen_c(splitter);
						newBuf = replaceChars(readBuf, L"\0", splitter, buflen, spllen);
						if (!newBuf) {
							return RegOpResult::Fail;
						}
						std::wstring tmpstr = newBuf;
						/*tmpstr.erase(remove_if(tmpstr.begin(), tmpstr.end(),
							[](wchar_t wc) { return !(wc >= 0 && wc <= 255); }),
							tmpstr.end());*/
						for (size_t i = 0; i < tmpstr.length() - 1; ++i) {
							if (static_cast<int>(tmpstr[i]) == 10 &&
								static_cast<int>(tmpstr[i + 1]) == 10) {
								tmpstr.erase(i, tmpstr.length() - i);
								break;
							}
						}
						SAFE_ARR_DELETE(newBuf);
						SAFE_ARR_DELETE(readBuf);
						if (split) {
							std::vector<std::wstring> tvec = splitStr(tmpstr, splitter);
							val = tvec;
						} else {
							val.push_back(tmpstr);
						}
						return RegOpResult::Success;
					}
				} else {
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				}
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::GetResourceList(const std::wstring valName,
	unsigned long long &val, const HKEY* root) const {
	if (valName.length()) {
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_READ | getRightMod(), &keyHandle)) {
			unsigned long buflen = 0;
			if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0, 0,
				0, &buflen)) {
				NEW_ARR_NULLIFY(readBuf, unsigned char, buflen / sizeof(unsigned char));
				if (readBuf) {
					if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0, 0,
						(unsigned char*)readBuf, &buflen)) {
						CLOSEKEY_NULLIFY(keyHandle);
						unsigned long busCount = *(unsigned long*)readBuf;
						if (busCount == 1) {
							val = 0;
							unsigned long resCount = *(unsigned long*)(readBuf + 16);
							unsigned char* pmi = readBuf + 24;
							for (unsigned long i = 0; i < resCount; ++i) {
								val += *(unsigned long*)(pmi + 8);
								pmi += 16;
							}
						} else {
							SAFE_ARR_DELETE(readBuf);
							return RegOpResult::Fail;
						}
						SAFE_ARR_DELETE(readBuf);
						return RegOpResult::Success;
					}
					SAFE_ARR_DELETE(readBuf);
					return RegOpResult::Fail;
				}
				return RegOpResult::Fail;
			}
			return RegOpResult::Fail;
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::GetVal(const std::wstring valName, unsigned char* &buf,
	unsigned long &bufSz, RegValType &valType, const HKEY* root) const {
	HKEY keyHandle = { 0 }, rootKey = { 0 };
	if (valName.length()) {
		std::wstring keyPath, valueName = valName;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_READ | getRightMod(), &keyHandle)) {
			unsigned long dataType = 0;
			if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0,
				&dataType, 0, &bufSz)) {
				valType = (RegValType)dataType;
				NEW_ARR_NULLIFY_NO_REDEFINE(buf, unsigned char, bufSz / sizeof(unsigned char));
				if (buf) {
					if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0,
						0, buf, &bufSz)) {
						RegCloseKey(keyHandle);
						return RegOpResult::Success;
					}
				}
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Fail;
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::GetValueType(const std::wstring valName, RegValType &valtype,
	const HKEY* root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_READ | getRightMod(), &keyHandle)) {
			unsigned long dataType = 0;
			if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, valueName.c_str(), 0,
				&dataType, 0, 0)) {
				if (dataType == REG_SZ) {
					valtype = RegValType::Str;
				} else if (dataType == REG_MULTI_SZ) {
					valtype = RegValType::MultiStr;
				} else if (dataType == REG_DWORD) {
					valtype = RegValType::DWord;
				} else if (dataType == REG_DWORD_LITTLE_ENDIAN) {
					valtype = RegValType::DWordLE;
				} else if (dataType == REG_DWORD_BIG_ENDIAN) {
					valtype = RegValType::DWordBE;
				} else if (dataType == REG_QWORD) {
					valtype = RegValType::QWord;
				} else if (dataType == REG_QWORD_LITTLE_ENDIAN) {
					valtype = RegValType::QWordLE;
				} else if (dataType == REG_EXPAND_SZ) {
					valtype = RegValType::ExpandStr;
				} else if (dataType == REG_BINARY) {
					valtype = RegValType::Binary;
				} else if (dataType == REG_LINK) {
					valtype = RegValType::Link;
				}
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Success;
			}
			if (keyHandle) {
				CLOSEKEY_NULLIFY(keyHandle);
			}
			return RegOpResult::Fail;
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::GetCPUDesc(std::vector<std::wstring> &cpuDesc, const HKEY* root) const {
	RegOpResult ret = RegOpResult::Fail;
	std::wstring tval;
	if (RegOpResult::Success == GetStrVal(
		L"HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0\\ProcessorNameString",
		tval, false, root)) {
		ret = RegOpResult::Success;
		cpuDesc.push_back(L"Name: " + tval);
	} else {
		cpuDesc.push_back(L"Name: (null)");
	}
	if (RegOpResult::Success == GetStrVal(
		L"HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0\\Identifier",
		tval, false, root)) {
		ret = RegOpResult::Success;
		cpuDesc.push_back(L"Identifier: " + tval);
	} else {
		cpuDesc.push_back(L"Identifier: (null)");
	}
	if (RegOpResult::Success == GetStrVal(
		L"HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0\\VendorIdentifier",
		tval, false, root)) {
		ret = RegOpResult::Success;
		cpuDesc.push_back(L"Vendor: " + tval);
	} else {
		cpuDesc.push_back(L"Vendor: (null)");
	}
	return ret;
}

RegOpResult RegHandler::CreateStrVal(const std::wstring valName, const std::wstring& val, const HKEY *root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_SET_VALUE | KEY_READ | getRightMod(), &keyHandle)) {
			if (ERROR_SUCCESS == RegSetValueEx(keyHandle, valueName.c_str(), 0,
				REG_SZ, (unsigned char*)val.c_str(), wcslen_c(val.c_str()) * sizeof(wchar_t))) {
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Success;

			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::CreateDWordVal(const std::wstring valName, const unsigned long& val, const HKEY *root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_SET_VALUE | KEY_READ | getRightMod(), &keyHandle)) {
			if (ERROR_SUCCESS == RegSetValueEx(keyHandle, valueName.c_str(), 0,
				REG_DWORD, (unsigned char*)&val, sizeof(unsigned long))) {
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Success;
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::CreateQWordVal(const std::wstring valName, const unsigned long long& val, const HKEY* root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_SET_VALUE | KEY_READ | getRightMod(), &keyHandle)) {
			if (ERROR_SUCCESS == RegSetValueEx(keyHandle, valueName.c_str(), 0,
				REG_DWORD, (unsigned char*)&val, sizeof(unsigned long))) {
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Success;
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::CreateBinaryVal(const std::wstring valName, unsigned char* &buf,
	unsigned long bufSz, const HKEY* root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_SET_VALUE | KEY_READ | getRightMod(), &keyHandle)) {
			if (ERROR_SUCCESS == RegSetValueEx(keyHandle, valueName.c_str(), 0,
				REG_BINARY, buf, bufSz)) {
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Success;
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::CreateMultiStrVal(const std::wstring valName,
	const std::vector<std::wstring>& val, const HKEY *root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_SET_VALUE | KEY_READ | getRightMod(), &keyHandle)) {
			size_t totalsz = 0;
			for (size_t i = 0; i < val.size(); ++i) {
				totalsz += wcslen_c(val[i].c_str()) + 1;
			}
			++totalsz;
			NEW_ARR_NULLIFY(writebuf, unsigned char, totalsz);
			// char* writebuf = (char*)calloc(totalsz, totalsz * sizeof(char));
			if (writebuf) {
				totalsz = 0;
				for (size_t i = 0; i < val.size(); ++i) {
					size_t clen = wcslen_c(val[i].c_str()) + 1;
					char* tbuf = wchar2char(val[i].c_str());
					if (tbuf) {
						memcpy(&writebuf[totalsz], tbuf, clen * sizeof(char));
						SAFE_FREE(tbuf);
					}
					totalsz += clen;
				}
				if (ERROR_SUCCESS == ::RegSetValueExA(keyHandle, wstr2str(valueName).c_str(), 0,
					REG_MULTI_SZ, (unsigned char*)writebuf, totalsz * sizeof(char))) {
					CLOSEKEY_NULLIFY(keyHandle);
					SAFE_ARR_DELETE(writebuf);
					return RegOpResult::Success;
				}
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::GetSysTempPath(std::wstring &path, const HKEY *root) {
	std::wstring regpath;
	if (RegOpResult::Success != GetStrVal(
		L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\TEMP", regpath, false,
		root)) {
		if (RegOpResult::Success != GetStrVal(
			L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\TMP", regpath,
			false, root)) {
			return RegOpResult::Fail;
		}
	}
	path = regpath;
	return RegOpResult::Success;
}

RegOpResult RegHandler::DeleteVal(const std::wstring valName, const HKEY *root) const {
	if (valName.length()) {
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, valName, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_ALL_ACCESS | getRightMod(), &keyHandle)) {
			if (ERROR_SUCCESS == ::RegDeleteValue(keyHandle, valueName.c_str())) {
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Success;
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::AddItem2ExplorerContextMenu(const std::wstring itemName, const std::wstring objectPath,
	const bool add2DirMenu, const bool add2DriveMenu, const HKEY *root) const {
	if ((!itemName.length() || !objectPath.length()) || (!add2DirMenu && !add2DriveMenu)) {
		return RegOpResult::Fail;
	}
	std::wstring itemNameMod = itemName;
	if (startsWith(itemNameMod, L"\\")) {
		itemNameMod = removeFromStart_copy(itemNameMod, L"\\");
	}
	if (endsWith(itemNameMod, L"\\")) {
		itemNameMod = removeFromEnd_copy(itemNameMod, L"\\");
	}
	std::wstring itemPath;
	std::wstring itemCommandPath;
	if (add2DirMenu) {
		itemPath = L"HKEY_CLASSES_ROOT\\Directory\\shell\\" + itemNameMod;
		itemCommandPath = L"HKEY_CLASSES_ROOT\\Directory\\shell\\" + itemNameMod + L"\\command";
		if (RegOpResult::Success == CreateKey(itemCommandPath, true, root)) {
			HKEY keyHandle = { 0 };
			std::wstring pathPrepped = joinStrs(splitStr(itemPath, L"\\"), L"\\", 1);
			std::wstring pathCommandPrepped = joinStrs(splitStr(itemCommandPath, L"\\"), L"\\", 1);
			if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CLASSES_ROOT, pathPrepped.c_str(), 0,
				KEY_SET_VALUE | getRightMod(), &keyHandle)) {
				if (ERROR_SUCCESS != ::RegSetValueEx(keyHandle, L"Icon", 0, REG_SZ,
					(unsigned char*)objectPath.c_str(), objectPath.length() * sizeof(wchar_t))) {
				}
				CLOSEKEY_NULLIFY(keyHandle);
			}
			if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CLASSES_ROOT, pathCommandPrepped.c_str(), 0,
				KEY_SET_VALUE | getRightMod(),
				&keyHandle)) {
				if (ERROR_SUCCESS != ::RegSetValueEx(keyHandle, 0, 0, REG_SZ,
					(unsigned char*)objectPath.c_str(), objectPath.length() * sizeof(wchar_t))) {
				}
				CLOSEKEY_NULLIFY(keyHandle);
			}
		}
	}
	if (add2DriveMenu) {
		itemPath = L"HKEY_CLASSES_ROOT\\Drive\\shell\\" + itemNameMod;
		itemCommandPath = L"HKEY_CLASSES_ROOT\\Drive\\shell\\" + itemNameMod + L"\\command";
		if (RegOpResult::Success == CreateKey(itemCommandPath, true, root)) {
			HKEY keyHandle = { 0 };
			std::wstring pathPrepped = joinStrs(splitStr(itemPath, L"\\"), L"\\", 1);
			std::wstring pathCommandPrepped = joinStrs(splitStr(itemCommandPath, L"\\"), L"\\", 1);
			if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CLASSES_ROOT, pathPrepped.c_str(), 0,
				KEY_SET_VALUE | getRightMod(), &keyHandle)) {
				if (ERROR_SUCCESS != ::RegSetValueEx(keyHandle, L"Icon", 0, REG_SZ,
					(unsigned char*)objectPath.c_str(), objectPath.length() * sizeof(wchar_t))) {
					return RegOpResult::Fail;
				}
				CLOSEKEY_NULLIFY(keyHandle);
			}
			if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CLASSES_ROOT, pathCommandPrepped.c_str(), 0,
				KEY_SET_VALUE | getRightMod(), &keyHandle)) {
				if (ERROR_SUCCESS != ::RegSetValueEx(keyHandle, 0, 0, REG_SZ,
					(unsigned char*)objectPath.c_str(), objectPath.length() * sizeof(wchar_t))) {
					return RegOpResult::Fail;
				}
				CLOSEKEY_NULLIFY(keyHandle);
			}
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::RemoveItemFromExplorerContextMenu(const std::wstring itemName, const bool removeFromDirMenu,
	const bool removeFromDriveMenu, const HKEY *root) const {
	if (!itemName.length()) {
		return RegOpResult::Fail;
	}
	std::wstring itemNameMod = itemName;
	if (startsWith(itemNameMod, L"\\")) {
		itemNameMod = removeFromStart_copy(itemNameMod, L"\\");
	}
	if (endsWith(itemNameMod, L"\\")) {
		itemNameMod = removeFromEnd_copy(itemNameMod, L"\\");
	}
	std::wstring itemPath;
	if (removeFromDirMenu) {
		itemPath = L"HKEY_CLASSES_ROOT\\Directory\\shell\\" + itemNameMod;
		if (RegOpResult::Success != DeleteKey(itemPath, true, root)) {
			return RegOpResult::Fail;
		}
	}
	if (removeFromDriveMenu) {
		itemPath = L"HKEY_CLASSES_ROOT\\Drive\\shell\\" + itemNameMod;
		if (RegOpResult::Success != DeleteKey(itemPath, true, root)) {
			return RegOpResult::Fail;
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::AddItem2DesktopContextMenu(const std::wstring itemName, const std::wstring objectPath,
	const std::wstring subMenuName, const std::wstring subCommandMenuName, const std::wstring subCommandMenuNameAdd,
	const std::wstring menuIconPath, const HKEY *root) const {
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(objectPath.c_str())) {
		return RegOpResult::Fail;
	}
	if (subMenuName.length()) {
		std::wstring subMenuNameMod = subCommandMenuName;
		if (startsWith(subMenuNameMod, L"\\")) {
			subMenuNameMod = removeFromStart_copy(subMenuNameMod, L"\\");
		}
		if (endsWith(subMenuNameMod, L"\\")) {
			subMenuNameMod = removeFromEnd_copy(subMenuNameMod, L"\\");
		}
		std::wstring subCommandMenuNameCombined = subCommandMenuName + subCommandMenuNameAdd;
		std::wstring indstr = pickMenuModNum(subCommandMenuNameCombined, root);
		if (L"0" == indstr) {
			return RegOpResult::Fail;
		}
		subCommandMenuNameCombined += indstr;
		unsigned char* buf = 0;
		unsigned long bufSz = 0;
		std::wstring cmName = L"HKEY_CLASSES_ROOT\\DesktopBackground\\Shell\\" + subMenuNameMod;
		SecDesc sd;
		if (RegOpResult::Success != GetKeySecurity(cmName, sd, root)) {
			if (buf) {
				SAFE_ARR_DELETE(buf);
			}
			if (RegOpResult::Success == CreateKey(cmName, true, root)) {
				if (RegOpResult::Success != CreateStrVal(cmName + L"\\Icon", menuIconPath.c_str(), root)) {
					return RegOpResult::Fail;
				}
				if (RegOpResult::Success != CreateStrVal(cmName + L"\\MUIVerb", subMenuName, root)) {
					return RegOpResult::Fail;
				}
				if (RegOpResult::Success != CreateStrVal(cmName + L"\\Position", L"Bottom", root)) {
					return RegOpResult::Fail;
				}
				if (RegOpResult::Success != CreateStrVal(cmName + L"\\SubCommands", subCommandMenuNameCombined,
					root)) {
					return RegOpResult::Fail;
				}
			} else {
				return RegOpResult::Fail;
			}
		} else {
			if (buf) {
				SAFE_ARR_DELETE(buf);
			}
			if (RegOpResult::Success != CreateStrVal(cmName + L"\\Icon", menuIconPath.c_str(), root)) {
				return RegOpResult::Fail;
			}
			if (RegOpResult::Success != CreateStrVal(cmName + L"\\MUIVerb", subMenuName, root)) {
				return RegOpResult::Fail;
			}
			if (RegOpResult::Success != CreateStrVal(cmName + L"\\Position", L"Bottom", root)) {
				return RegOpResult::Fail;
			}
			// SubCommands
			std::wstring subcomm;
			RegValType rvt = RegValType::Binary;
			if (RegOpResult::Success == GetValueType(cmName + L"\\SubCommands", rvt, root) &&
				RegValType::Str == rvt) {
				if (RegOpResult::Success == GetStrVal(cmName + L"\\SubCommands",
					subcomm, true, root)) {
					subcomm += L";" + subCommandMenuNameCombined;
					if (RegOpResult::Success != CreateStrVal(cmName + L"\\SubCommands", subcomm,
						root)) {
						return RegOpResult::Fail;
					}
				}
			} else {
				if (RegOpResult::Success != CreateStrVal(cmName + L"\\SubCommands",
					subCommandMenuNameCombined, root)) {
					return RegOpResult::Fail;
				}
			}
		}
		cmName = L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\" +
			subCommandMenuNameCombined;
		if (RegOpResult::Success != GetKeySecurity(cmName, sd, root)) {
			if (buf) {
				SAFE_ARR_DELETE(buf);
			}
			if (RegOpResult::Success == CreateKey(cmName, true, root)) {
				HKEY keyHandle = { 0 };
				if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\" +
						subCommandMenuNameCombined).c_str(),
 					0, KEY_SET_VALUE | getRightMod(), &keyHandle)) {
					if (ERROR_SUCCESS != ::RegSetValueEx(keyHandle, 0, 0, REG_SZ,
						(unsigned char*)(itemName.c_str()), (itemName.length() * sizeof(wchar_t)))) {
						CLOSEKEY_NULLIFY(keyHandle);
						return RegOpResult::Fail;
					}
					if (ERROR_SUCCESS != ::RegSetValueEx(keyHandle, L"Icon", 0, REG_SZ,
						(unsigned char*)(objectPath.c_str()), (objectPath.length() * sizeof(wchar_t)))) {
						CLOSEKEY_NULLIFY(keyHandle);
						return RegOpResult::Fail;
					}
					CLOSEKEY_NULLIFY(keyHandle);
					if (RegOpResult::Success != CreateKey(cmName + L"\\command", true, root)) {
						return RegOpResult::Fail;
					}
					if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
						(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\" +
							subCommandMenuNameCombined + L"\\command").c_str(),
						0, KEY_SET_VALUE | getRightMod(), &keyHandle)) {
						if (ERROR_SUCCESS != ::RegSetValueEx(keyHandle, 0, 0, REG_SZ,
							(unsigned char*)(objectPath.c_str()), (objectPath.length() * sizeof(wchar_t)))) {
							CLOSEKEY_NULLIFY(keyHandle);
							return RegOpResult::Fail;
						}
						CLOSEKEY_NULLIFY(keyHandle);
						return RegOpResult::Success;
					}
					if (keyHandle) {
						CLOSEKEY_NULLIFY(keyHandle);
					}
					return RegOpResult::Success;
				}
				if (keyHandle) {
					CLOSEKEY_NULLIFY(keyHandle);
				}
				return RegOpResult::Fail;
			} else {
				return RegOpResult::Fail;
			}
		} else {
			if (buf) {
				SAFE_ARR_DELETE(buf);
			}
			return RegOpResult::Fail;
		}
	} else {
		std::wstring itemNameMod = itemName;
		if (startsWith(itemNameMod, L"\\")) {
			itemNameMod = removeFromStart_copy(itemNameMod, L"\\");
		}
		if (endsWith(itemNameMod, L"\\")) {
			itemNameMod = removeFromEnd_copy(itemNameMod, L"\\");
		}
		std::wstring regPath = L"HKEY_CLASSES_ROOT\\Directory\\Background\\shell\\" + itemNameMod;
		std::wstring regPathCommand = L"HKEY_CLASSES_ROOT\\Directory\\Background\\shell\\" + itemNameMod +
			L"\\command";
		if (RegOpResult::Success == CreateKey(regPathCommand, true, root)) {
			HKEY keyHandle = { 0 };
			std::wstring pathPrepped = joinStrs(splitStr(regPath, L"\\"), L"\\", 1);
			std::wstring pathCommandPrepped = joinStrs(splitStr(regPathCommand, L"\\"), L"\\", 1);
			if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CLASSES_ROOT, pathPrepped.c_str(),
				0, KEY_SET_VALUE | getRightMod(), &keyHandle)) {
				if (ERROR_SUCCESS != ::RegSetValueEx(keyHandle, 0, 0, REG_SZ,
					(unsigned char*)(itemNameMod.c_str()), (objectPath.length() * sizeof(wchar_t)))) {
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				}
				if (ERROR_SUCCESS != ::RegSetValueEx(keyHandle, L"Icon", 0, REG_SZ,
					(unsigned char*)(menuIconPath.c_str()), (objectPath.length() * sizeof(wchar_t)))) {
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				}
				CLOSEKEY_NULLIFY(keyHandle);
				if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CLASSES_ROOT, pathCommandPrepped.c_str(),
					0, KEY_SET_VALUE | getRightMod(), &keyHandle)) {
					if (ERROR_SUCCESS != ::RegSetValueEx(keyHandle, 0, 0, REG_SZ,
						(unsigned char*)(objectPath.c_str()), (objectPath.length() * sizeof(wchar_t)))) {
						CLOSEKEY_NULLIFY(keyHandle);
						return RegOpResult::Fail;
					} else {
						CLOSEKEY_NULLIFY(keyHandle);
						return RegOpResult::Success;
					}
				}
			}
			if (keyHandle) {
				CLOSEKEY_NULLIFY(keyHandle);
			}
			return RegOpResult::Fail;
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::RemoveItemFromDesktopContextMenu(const std::wstring itemName,
	const std::wstring subMenuName, const std::wstring subCommandMenuName,
	const std::wstring subCommandMenuNameAdd, const HKEY *root) const {
	if (subMenuName.length()) {
		std::wstring subMenuNameMod = subCommandMenuName;
		if (startsWith(subMenuNameMod, L"\\")) {
			subMenuNameMod = removeFromStart_copy(subMenuNameMod, L"\\");
		}
		if (endsWith(subMenuNameMod, L"\\")) {
			subMenuNameMod = removeFromEnd_copy(subMenuNameMod, L"\\");
		}
		std::wstring subkeyName;
		std::wstring subCommandMenuNameCombined = subMenuNameMod + subCommandMenuNameAdd;
		wchar_t* buf = 0;
		HKEY keyHandle = { 0 };
		unsigned long maxind = std::stoul(wstr2str(pickMenuModNum(subCommandMenuNameCombined, root)));
		unsigned long type = 0, bufSz = 0, ind = maxind - 1;
		std::wstring pathPrepped, delOpt;
		std::vector<RegKeyDesc> searchres;
		if (RegOpResult::Success == SeekKey(prepRegexString_copy(L".*" + subCommandMenuNameCombined + L".*"),
			searchres,
			L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\",
			false, 0, true, root)) {
			if (searchres.size()) {
				for (size_t i = 0; i < searchres.size(); ++i) {
					pathPrepped = joinStrs(splitStr(searchres[i].keyPath, L"\\"), L"\\", 1);
					delOpt = splitStr(searchres[i].keyPath, L"\\").back();
					if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, pathPrepped.c_str(), 0,
						KEY_READ | KEY_QUERY_VALUE | getRightMod(), &keyHandle)) {
						if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, 0, 0, &type, 0, &bufSz)) {
							buf = (wchar_t*)malloc(bufSz);
							if (buf) {
								if (ERROR_SUCCESS == ::RegQueryValueEx(keyHandle, 0, 0, &type,
									(unsigned char*)buf, &bufSz)) {
									if (partialMatch(buf, itemName)) {
										CLOSEKEY_NULLIFY(keyHandle);
										SAFE_FREE(buf);
										if (searchres.size() > 1) {
											std::wstring val;
											std::wstring valpath = L"HKEY_CLASSES_ROOT\\DesktopBackground\\Shell\\" +
												subMenuNameMod + L"\\SubCommands";
											if (RegOpResult::Success == GetStrVal(valpath, val, true, root)) {
												size_t pos = lower_copy(val).find(lower_copy(delOpt));
												if (std::wstring::npos == pos) {
													return RegOpResult::Fail;
												}
												if ((pos + subCommandMenuNameCombined.length() +
													std::to_wstring(i).length()) == val.length()) {
													delOpt = L";" + delOpt;
												} else {
													delOpt += L";";
												}
												std::wstring modstr = removeSubstr_copy(val, delOpt);
												if (RegOpResult::Success != CreateStrVal(valpath,
													modstr, root)) {
													return RegOpResult::Fail;
												}
											}
										}
										if (RegOpResult::Success != DeleteKey(searchres[i].keyPath,
											true, root)) {
											return RegOpResult::Fail;
										}
										if (searchres.size() == 1) {
											if (RegOpResult::Success != DeleteKey(
												L"HKEY_CLASSES_ROOT\\DesktopBackground\\Shell\\" +
												subMenuNameMod, true, root)) {
												return RegOpResult::Fail;
											}
										}
										return RegOpResult::Success;
									}
								}
								SAFE_FREE(buf);
							} else {
								return RegOpResult::Fail;
							}
						} else {
							return RegOpResult::Fail;
						}
						if (keyHandle) {
							CLOSEKEY_NULLIFY(keyHandle);
						}
					} else {
						if (keyHandle) {
							CLOSEKEY_NULLIFY(keyHandle);
						}
						return RegOpResult::Fail;
					}
				}
				return RegOpResult::Fail;
			} else {
				return RegOpResult::Fail;
			}
		} else {
			return RegOpResult::Fail;
		}
	} else {
		std::wstring itemNameMod = itemName;
		if (startsWith(itemNameMod, L"\\")) {
			itemNameMod = removeFromStart_copy(itemNameMod, L"\\");
		}
		if (endsWith(itemNameMod, L"\\")) {
			itemNameMod = removeFromEnd_copy(itemNameMod, L"\\");
		}
		if (RegOpResult::Success != DeleteKey(L"HKEY_CLASSES_ROOT\\Directory\\Background\\shell\\" +
			itemNameMod, true, root)) {
			return RegOpResult::Fail;
		} else {
			return RegOpResult::Success;
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::GetSoftwareVersion(const std::wstring softwareName, std::vector<SOFTWAREVERSION> &versions,
	const bool anySoftware, const bool uniqueValues, const HKEY *root) const {
	size_t i = 0;
	bool matchFound = false;
	std::vector<std::wstring> regpaths;
	regpaths.push_back(L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
	regpaths.push_back(L"HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
	std::vector<RegValDesc> softwareKeys;
	std::wstring strbuf, verstr, searchline = softwareName;
	RegOpResult seekres;
	for (i = 0; i < regpaths.size(); ++i) {
		seekres = SeekVal(L"DisplayName", softwareKeys, regpaths[i], true, 0, false, root);
		if (RegOpResult::Success != seekres) {
			return seekres;
		}
	}
	if(!softwareKeys.size()) { return RegOpResult::Fail; }
	if (anySoftware) {
		if (!startsWith(searchline, L".*")) {
			searchline = L".*" + searchline;
		}
		if (!endsWith(searchline, L".*")) {
			searchline += L".*";
		}
	}
	searchline = prepRegexString_copy(searchline); 
	std::basic_regex<wchar_t> svr(searchline);
	std::basic_regex<wchar_t> svrlow(lower_copy(searchline));
	std::wstring softname;
	SOFTWAREVERSION val;
	for (i = 0; i < softwareKeys.size(); ++i) {
		strbuf = L"Couldnt identify version";
		matchFound = false;
		if (!IsBadReadPtr(softwareKeys[i].valData)) {
			if (!memcmp(softwareKeys[i].valData, &gc_wc_emptyVal, sizeof(wchar_t))) {
				continue;
			}
		}
		softname = reinterpret_cast<wchar_t*>(softwareKeys[i].valData);
		if (anySoftware) {
			if (lower_copy(softwareName) == lower_copy(softname) ||
				std::regex_match(softname, svr) ||
				std::regex_match(lower_copy(softname), svrlow)) {
				matchFound = true;
			}
		} else {
			if (lower_copy(softwareName) == lower_copy(softname)) {
				matchFound = true;
			}
		}
		if (matchFound) {
			verstr = replaceSubstr_copy(softwareKeys[i].valPath, L"DisplayName", L"DisplayVersion");
			if (RegOpResult::Success == GetStrVal(verstr, strbuf, false, root)) {
				val = { softname, strbuf };
				if (uniqueValues) {
					if (std::find(versions.begin(), versions.end(), val) == versions.end()) {
						versions.push_back(val);
					}
				} else {
					versions.push_back(val);
				}
			} else {
				verstr = replaceSubstr_copy(softwareKeys[i].valPath, L"DisplayName", L"Inno Setup: Setup Version");
				if (RegOpResult::Success == GetStrVal(verstr, strbuf, false, root)) {
					val = { softname, strbuf + L" (Inno Setup)" };
					if (uniqueValues) {
						if (std::find(versions.begin(), versions.end(), val) == versions.end()) {
							versions.push_back(val);
						}
					} else {
						versions.push_back(val);
					}
				} else {
					val = { softname, strbuf };
					if (uniqueValues) {
						if (std::find(versions.begin(), versions.end(), val) == versions.end()) {
							versions.push_back(val);
						}
					} else {
						versions.push_back(val);
					}
				}
			}
		}
	}
	if (RegOpResult::Success != FreeValues(softwareKeys)) {
		return RegOpResult::Fail;
	}
	if(versions.size()) {
		return RegOpResult::Success;
	} else {
		return RegOpResult::Fail;
	}
}

RegOpResult RegHandler::CopyVal(const std::wstring source, const std::wstring destKeyName, const std::wstring destValName,
	const HKEY *root) const {
	if (source.length()) {
		std::wstring compSrc, compDst;
		if (endsWith(source, L"\\")) {
			compSrc = source;
		} else {
			compSrc = source + L"\\";
		}
		if (endsWith(destKeyName, L"\\")) {
			compDst = destKeyName + destValName;
		} else {
			compDst = destKeyName + L"\\" + destValName;
		}
		if (lower_copy(compSrc) == lower_copy(compDst)) {
			return RegOpResult::Fail;
		}
		// REGHELPERVALBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = source;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, source, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_ALL_ACCESS | getRightMod(), &keyHandle)) {
			unsigned char* buf = 0;
			unsigned long bufSz = 0;
			RegValType valType;
			if (RegOpResult::Success == GetVal(source, buf, bufSz, valType)) {
				bool keyAvail = false;
				if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, destKeyName.c_str(), 0,
					KEY_READ | getRightMod(), &keyHandle)) {
					keyAvail = true;
					CLOSEKEY_NULLIFY(keyHandle);
				} else {
					if (RegOpResult::Success == CreateKey(destKeyName, true, root)) {
						keyAvail = true;
						CLOSEKEY_NULLIFY(keyHandle);
					} else {
						return RegOpResult::Fail;
					}
				}
				if (keyAvail) {
					prepHKEYKeyPath(rootKey, destKeyName, rootKey, keyPath);
					if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
						KEY_ALL_ACCESS | getRightMod(), &keyHandle)) {
						if (ERROR_SUCCESS == RegSetValueEx(keyHandle, destValName.c_str(),
							0, static_cast<unsigned long>(valType), buf, bufSz)) {
							SAFE_ARR_DELETE(buf);
							CLOSEKEY_NULLIFY(keyHandle);
							return RegOpResult::Success;
						}
						SAFE_ARR_DELETE(buf);
						CLOSEKEY_NULLIFY(keyHandle);
						return RegOpResult::Fail;
					} else {
						if (keyHandle) {
							CLOSEKEY_NULLIFY(keyHandle);
						}
						return RegOpResult::Fail;
					}
				}
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Fail;
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::MoveVal(const std::wstring source,
	const std::wstring destKeyName, const std::wstring destValName,
	const HKEY* root) const {
	if (source.length()) {
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = source;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPathValueName(rootKey, rootKey, source, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_ALL_ACCESS | getRightMod(), &keyHandle)) {
			if (RegOpResult::Success == CopyVal(source, destKeyName,
				destValName, root)) {
				if (RegOpResult::Success == DeleteVal(source, root)) {
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Success;
				} else {
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				}
			} else {
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Fail;
			}
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::SeekVal(const std::wstring valName,
	std::vector<RegValDesc> &results, const std::wstring startPath,
	const RegValType valType, const bool acquireVals, const unsigned long limitResults,
	const bool cleanupStrings, const HKEY* root) const {
	if (valName.length()) {
		unsigned long resAcquired = 0;
		std::wstring path = startPath;
		if (path == L"") {
			path = L"HKEY_LOCAL_MACHINE\\";
		}
		// REGHELPERKEYBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPath(rootKey, path, rootKey, keyPath);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_READ | KEY_QUERY_VALUE | getRightMod(), &keyHandle)) {
			wchar_t			achClass[MAX_PATH] = L"";
			wchar_t			achValue[REG_MAX_VALUE_NAME] = { 0 }; // buffer for value name
			wchar_t			achKey[256] = { 0 };				  // buffer for subkey name
			unsigned long	cbName = 0;							  // size of name string 
			unsigned long	cchValue = REG_MAX_VALUE_NAME;		  // buffer for class name 
			unsigned long	cchClassName = MAX_PATH;			  // size of class string 
			unsigned long	cSubKeys = 0;						  // number of subkeys 
			unsigned long	cbMaxSubKey = 0;					  // longest subkey size 
			unsigned long	cchMaxClass = 0;					  // longest class string 
			unsigned long	cValues = 0;					      // number of values for key 
			unsigned long	cchMaxValue = 0;					  // longest value name 
			unsigned long	cbMaxValueData = 0;					  // longest value data 
			unsigned long	cbSecurityDescriptor = 0;		      // size of security descriptor 
			// FILETIME		ftLastWriteTime;					  // last write time
			unsigned long	dataType = 0;
			std::basic_regex<wchar_t> fbr(prepRegexString_copy(valName));
			std::basic_regex<wchar_t> fbrlow(lower_copy(prepRegexString_copy(valName)));
			std::wstring seekpath;
			if (ERROR_SUCCESS == ::RegQueryInfoKey(
				keyHandle,				 // key handle 
				achClass,				 // buffer for class name 
				&cchClassName,			 // size of class string 
				NULL,				 	 // reserved 
				&cSubKeys,				 // number of subkeys 
				&cbMaxSubKey,            // longest subkey size 
				&cchMaxClass,            // longest class string 
				&cValues,                // number of values for this key 
				&cchMaxValue,            // longest value name 
				&cbMaxValueData,         // longest value data 
				&cbSecurityDescriptor,   // security descriptor 
				0 /* &ftLastWriteTime */ )		 // last write time
			) {
				if (cSubKeys) {
					for (size_t i = 0; i < cSubKeys; ++i) {
						cbName = REG_MAX_KEY_LENGTH;
						RegEnumKeyEx(keyHandle, i,
										achKey,
										&cbName,
										NULL,
										NULL,
										NULL,
										0 /* &ftLastWriteTime */ );
						seekpath = hkey2wstr(rootKey);
						if (keyPath.length()) {
							if (endsWith(keyPath, L"\\")) {
								seekpath += removeFromStart_copy(keyPath, L"\\") + achKey;
							} else {
								seekpath += removeFromStart_copy(keyPath, L"\\") + L"\\" + achKey;
							}
						} else {
							seekpath += removeFromStart_copy(keyPath, L"\\") + achKey;
						}
						// seekpath = rebuildSearchKeyPath(rootKey, keyPath, achKey);
						SeekValRecType(valName, results, valType, seekpath, root);
					}
				}
				if (cValues) {
					unsigned long buflen = REG_READBUFSZ;
					for (size_t i = 0; i < cValues; ++i) {
						cchValue = REG_MAX_VALUE_NAME;
						achValue[0] = 0;
						::RegEnumValue(keyHandle, i,
										achValue,
										&cchValue,
										0,
										&dataType,
										0,
										&buflen);
						if ((std::regex_match(achValue, fbr) ||
							std::regex_match(lower_copy(achValue), fbrlow) ||
							(lower_copy(valName) == lower_copy(achValue))) &&
							(dataType == static_cast<unsigned long>(valType))) {
							RegValDesc elem;
							elem.valPath = hkey2wstr(rootKey);
							if (wcslen_c(achValue)) {
								elem.valPath += keyPath + L"\\" + std::wstring(achValue);
								RegValType rvt;
								if(RegOpResult::Success == GetValueType(elem.valPath, rvt)) {
									elem.valName = achValue;
									elem.valDataSz = buflen;
									elem.valType = valType;
									if (std::find(results.begin(), results.end(), elem) == results.end()) {
										results.push_back(elem);
									}
								}
							}
						}
					}
				}
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Success;
		} else {
			if (keyHandle) {
				CLOSEKEY_NULLIFY(keyHandle);
			}
			return RegOpResult::Fail;
		}
		if (acquireVals) {
			return AcquireValues(results, limitResults, cleanupStrings, root);
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::SeekKey(const std::wstring keyName, std::vector<RegKeyDesc> &results,
	const std::wstring startPath, const bool acquireVals, const unsigned long limitResults,
	const bool cleanupStrings, const HKEY *root) const {
	if (keyName.length() && startPath.length()) {
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath = startPath;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPath(rootKey, keyPath, rootKey, keyPath);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_READ | KEY_QUERY_VALUE | getRightMod(), &keyHandle)) {
			wchar_t			achClass[MAX_PATH] = L"";
			wchar_t			achValue[REG_MAX_VALUE_NAME] = { 0 }; // buffer for value name
			wchar_t			achKey[256] = { 0 };				  // buffer for subkey name
			unsigned long	cbName = 0;							  // size of name string 
			unsigned long	cchValue = REG_MAX_VALUE_NAME;		  // buffer for class name 
			unsigned long	cchClassName = MAX_PATH;			  // size of class string 
			unsigned long	cSubKeys = 0;						  // number of subkeys 
			unsigned long	cbMaxSubKey = 0;					  // longest subkey size 
			unsigned long	cchMaxClass = 0;					  // longest class string 
			unsigned long	cValues = 0;					      // number of values for key 
			unsigned long	cchMaxValue = 0;					  // longest value name 
			unsigned long	cbMaxValueData = 0;					  // longest value data 
			unsigned long	cbSecurityDescriptor = 0;		      // size of security descriptor 
			// FILETIME		ftLastWriteTime;					  // last write time
			unsigned long	dataType = 0;
			std::basic_regex<wchar_t> kbr(prepRegexString_copy(keyName));
			std::basic_regex<wchar_t> kbrlow(prepRegexString_copy(lower_copy(keyName)));
			std::wstring seekpath;
			bool matchFlag = false;
			std::wstring checkName = splitStr(keyPath, L"\\").back();
			if (checkName == keyName ||
				lower_copy(checkName) == lower_copy(keyName) ||
				std::regex_match(checkName, kbr) ||
				std::regex_match(lower_copy(checkName), kbrlow)) {
				matchFlag = true;
				RegKeyDesc keyelem;
				keyelem.keyPath = hkey2wstr(rootKey) + keyPath;
				keyelem.keyName = checkName;
				results.push_back(keyelem);
			}
			if (ERROR_SUCCESS == ::RegQueryInfoKey(
				keyHandle,				 // key handle 
				achClass,				 // buffer for class name 
				&cchClassName,			 // size of class string 
				NULL,				 	 // reserved 
				&cSubKeys,				 // number of subkeys 
				&cbMaxSubKey,            // longest subkey size 
				&cchMaxClass,            // longest class string 
				&cValues,                // number of values for this key 
				&cchMaxValue,            // longest value name 
				&cbMaxValueData,         // longest value data 
				&cbSecurityDescriptor,   // security descriptor 
				0 /* &ftLastWriteTime */)		 // last write time
				) {
				if (cSubKeys) {
					for (size_t i = 0; i < cSubKeys; ++i) {
						cbName = REG_MAX_KEY_LENGTH;
						::RegEnumKeyEx(keyHandle, i,
							achKey,
							&cbName,
							NULL,
							NULL,
							NULL,
							0 /* &ftLastWriteTime */);
						seekpath = hkey2wstr(rootKey);
						if (keyPath.length()) {
							if (endsWith(keyPath, L"\\")) {
								seekpath += removeFromStart_copy(keyPath, L"\\") + achKey;
							} else {
								seekpath += removeFromStart_copy(keyPath, L"\\") + L"\\" + achKey;
							}
						} else {
							seekpath += achKey;
						}
						if (matchFlag) {
							RegKeyDesc tskey;
							tskey.keyPath = seekpath;
							tskey.keyName = achKey;
							results.back().keys.push_back(tskey);
						}
						SeekKeyRec(keyName, results, seekpath, root);
					}
				}
				if (cValues && matchFlag) {
					unsigned long buflen = REG_READBUFSZ;
					for (size_t i = 0; i < cValues; ++i) {
						cchValue = REG_MAX_VALUE_NAME;
						achValue[0] = 0;
						::RegEnumValue(keyHandle, i,
							achValue,
							&cchValue,
							0,
							&dataType,
							0,
							&buflen);
						RegValDesc elem;
						elem.valPath = hkey2wstr(rootKey);
						if (wcslen_c(achValue)) {
							elem.valPath += keyPath + L"\\" + std::wstring(achValue);
							elem.valName = achValue;
							elem.valDataSz = buflen;
							elem.valType = static_cast<RegValType>(dataType);
							results.back().values.push_back(elem);
						}
					}
				}
			}
		}
		CLOSEKEY_NULLIFY(keyHandle);
	}
	if (acquireVals) {
		return AcquireValues(results, limitResults, cleanupStrings, root);
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::EnumKey(std::wstring keyName, RegKeyDesc &keyDesc, const bool acquireVals,
	const unsigned long limitResults, const bool cleanupStrings, const HKEY *root) const {
	if (keyName.length()) {
		unsigned long resAcquired = 0;
		std::wstring path = keyName;
		if (path == L"") {
			path = L"HKEY_LOCAL_MACHINE\\";
		}
		// REGHELPERKEYBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = keyName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPath(rootKey, path, rootKey, keyPath);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_READ | KEY_QUERY_VALUE | getRightMod(), &keyHandle)) {
			keyDesc.keyPath = hkey2wstr(rootKey) + keyPath;
			keyDesc.keyName = splitStr(keyPath, L"\\").back();
			wchar_t			achClass[MAX_PATH] = L"";
			wchar_t			achValue[REG_MAX_VALUE_NAME] = { 0 }; // buffer for value name
			wchar_t			achKey[256] = { 0 };				  // buffer for subkey name
			unsigned long	cbName = 0;							  // size of name string 
			unsigned long	cchValue = REG_MAX_VALUE_NAME;		  // buffer for class name 
			unsigned long	cchClassName = MAX_PATH;			  // size of class string 
			unsigned long	cSubKeys = 0;						  // number of subkeys 
			unsigned long	cbMaxSubKey = 0;					  // longest subkey size 
			unsigned long	cchMaxClass = 0;					  // longest class string 
			unsigned long	cValues = 0;					      // number of values for key 
			unsigned long	cchMaxValue = 0;					  // longest value name 
			unsigned long	cbMaxValueData = 0;					  // longest value data 
			unsigned long	cbSecurityDescriptor = 0;		      // size of security descriptor 
			// FILETIME		ftLastWriteTime;					  // last write time
			unsigned long	dataType = 0;
			std::wstring seekpath;
			if (ERROR_SUCCESS == ::RegQueryInfoKey(
				keyHandle,				 // key handle 
				achClass,				 // buffer for class name 
				&cchClassName,			 // size of class string 
				NULL,				 	 // reserved 
				&cSubKeys,				 // number of subkeys 
				&cbMaxSubKey,            // longest subkey size 
				&cchMaxClass,            // longest class string 
				&cValues,                // number of values for this key 
				&cchMaxValue,            // longest value name 
				&cbMaxValueData,         // longest value data 
				&cbSecurityDescriptor,   // security descriptor 
				0 /* &ftLastWriteTime */)		 // last write time
				) {
				if (cSubKeys) {
					for (size_t i = 0; i < cSubKeys; ++i) {
						cbName = REG_MAX_KEY_LENGTH;
						::RegEnumKeyEx(keyHandle, i, achKey, &cbName, 0, 0, 0, 0 /* &ftLastWriteTime */);
						seekpath = hkey2wstr(rootKey);
						if (keyPath.length()) {
							if (endsWith(keyPath, L"\\")) {
								seekpath += removeFromStart_copy(keyPath, L"\\") + achKey;
							} else {
								seekpath += removeFromStart_copy(keyPath, L"\\") + L"\\" + achKey;
							}
						} else {
							seekpath += removeFromStart_copy(keyPath, L"\\") + achKey;
						}
						RegKeyDesc tkey;
						tkey.keyPath = seekpath;
						tkey.keyName = splitStr(achKey, L"\\").back();
						EnumKey(tkey.keyPath, tkey, acquireVals, limitResults, cleanupStrings, root);
						keyDesc.keys.push_back(tkey);
					}
				}
				if (cValues) {
					unsigned long buflen = REG_READBUFSZ;
					for (size_t i = 0; i < cValues; ++i) {
						cchValue = REG_MAX_VALUE_NAME;
						achValue[0] = 0;
						::RegEnumValue(keyHandle, i, achValue, &cchValue, 0, &dataType, 0, &buflen);
						RegValDesc elem;
						elem.valPath = hkey2wstr(rootKey);
						if (wcslen_c(achValue)) {
							elem.valPath += keyPath + L"\\" + std::wstring(achValue);
							elem.valName = achValue;
							elem.valDataSz = buflen;
							elem.valType = static_cast<RegValType>(dataType);
							if (std::find(keyDesc.values.begin(), keyDesc.values.end(), elem) ==
								keyDesc.values.end()) {
								keyDesc.values.push_back(elem);
							}
						}
					}
				}
			}
			CLOSEKEY_NULLIFY(keyHandle);
		} else {
			if (keyHandle) {
				CLOSEKEY_NULLIFY(keyHandle);
			}
			return RegOpResult::Fail;
		}
		if (acquireVals) {
			return AcquireValues(keyDesc, limitResults, cleanupStrings, root);
		}
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::SeekVal2(const std::wstring valName,
	std::vector<RegValDesc> &results, const std::wstring startPath,
	const bool acquireVals, const unsigned long limitResults ,
	const bool cleanupStrings, const HKEY* root) const {
	if (valName.length() && startPath.length()) {
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath = startPath;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPath(rootKey, keyPath, rootKey, keyPath);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_READ | KEY_QUERY_VALUE | getRightMod(), &keyHandle)) {	
			long keyret = 0;
			unsigned long keyIdx = 0, keyNameLen = 4096, keyClassLen = 512;
			FILETIME writeTime = { 0 };
			wchar_t lpKeyName[4096] = { 0 };
			wchar_t lpKeyClass[512] = { 0 };
			memset(lpKeyName, 0, 4096 * sizeof(wchar_t));
			while ((keyret = RegEnumKeyEx(keyHandle, keyIdx++,
				lpKeyName, &keyNameLen, 0, lpKeyClass, &keyClassLen,
				&writeTime)) != ERROR_NO_MORE_ITEMS) {
				keyNameLen = 4096;
				keyClassLen = 512;
				unsigned long size = MAX_PATH;
				wchar_t keynameBuf[MAX_PATH] = { 0 };
				FILETIME filetime = { 0 };
				std::wstring keyPathRec;
				if (ERROR_SUCCESS == keyret || ERROR_MORE_DATA == keyret) {
					keyPathRec = rebuildSearchKeyPath(rootKey, keyPath,
						lpKeyName);
					if (RegOpResult::Success != SeekVal2(valName,
						results, keyPathRec, false, 0,
						root)) {
						break;
					}
				}
			}
			RegValDesc elem;
			long valret = 0;
			unsigned long valIdx = 0, dwValSize = 4096, dwDataSize = 4096,
				type = 0;
			wchar_t* lpValname = new wchar_t[4096];
			wchar_t* lpDataname = new wchar_t[4096];
			memset(lpDataname, 0, 4096 * sizeof(wchar_t));
			memset(lpValname, 0, 4096 * sizeof(wchar_t));
			while ((valret = ::RegEnumValue(keyHandle, valIdx++,
				lpValname, &dwValSize, 0, &type, 0, 0)) !=
				ERROR_NO_MORE_ITEMS) {
				if (wcslen_c(lpValname)) {
					std::basic_regex<wchar_t> svr(valName);
					std::basic_regex<wchar_t> svrlow(lower_copy(valName));
					if (lower_copy(valName) == lower_copy(lpValname) ||
						std::regex_match(valName, svr) ||
						std::regex_match(lower_copy(valName), svrlow)) {
						if ((valret == ERROR_SUCCESS /* || (valret == ERROR_MORE_DATA */)) {
							if (HKEY_CURRENT_USER == rootKey) {
								elem.valPath = L"HKEY_CURRENT_USER\\";
							} else if (HKEY_LOCAL_MACHINE == rootKey) {
								elem.valPath = L"HKEY_LOCAL_MACHINE\\";
							} else if (HKEY_USERS == rootKey) {
								elem.valPath = L"HKEY_USERS\\";
							} else if (HKEY_CURRENT_CONFIG == rootKey) {
								elem.valPath = L"HKEY_CURRENT_CONFIG\\";
							} else if (HKEY_CLASSES_ROOT == rootKey) {
								elem.valPath = L"HKEY_CLASSES_ROOT\\";
							}
							elem.valPath += keyPath + L"\\" + lpValname;
							elem.valName = lpValname;
							elem.valType = (RegValType)type;
							elem.valDataSz = dwValSize;
							if (std::find(results.begin(), results.end(), elem) == results.end()) {
								results.push_back(elem);
							}
							// _tprintf(TEXT("(%d) %s"), reg_index, lpValname);

						}
					}
				}
				dwValSize = 4096;
				dwDataSize = 4096;
				memset(lpDataname, 0, 4096 * sizeof(wchar_t));
				memset(lpValname, 0, 4096 * sizeof(wchar_t));
			}
			delete[] lpDataname;
			delete[] lpValname;
			CLOSEKEY_NULLIFY(keyHandle);
		} else {
			errorMsgBox(getErrorDetails(getLastErrorCode(), L"RegOpenKey",
				L"Failed to open key: " + keyPath));
			if (keyHandle) {
				CLOSEKEY_NULLIFY(keyHandle);
			}
			return RegOpResult::Fail;
		}
	} else {
		return RegOpResult::Fail;
	}
	if (acquireVals) {
		return AcquireValues(results, limitResults, cleanupStrings, root);
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::SeekVal(const std::wstring valName, std::vector<RegValDesc> &results,
	const std::wstring startPath, const bool acquireVals, const unsigned long limitResults, const bool cleanupStrings,
	const HKEY *root) const {
	if (valName.length()) {
		unsigned long resAcquired = 0;
		std::wstring path = startPath;
		if (path == L"") {
			path = L"HKEY_LOCAL_MACHINE\\";
		}
		// REGHELPERKEYBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPath(rootKey, path, rootKey, keyPath);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_READ | KEY_QUERY_VALUE  | getRightMod(), &keyHandle)) {
			wchar_t			achClass[MAX_PATH] = L"";
			wchar_t			achValue[REG_MAX_VALUE_NAME] = { 0 }; // buffer for value name
			wchar_t			achKey[256] = { 0 };				  // buffer for subkey name
			unsigned long	cbName = 0;							  // size of name string 
			unsigned long	cchValue = REG_MAX_VALUE_NAME;		  // buffer for class name 
			unsigned long	cchClassName = MAX_PATH;			  // size of class string 
			unsigned long	cSubKeys = 0;						  // number of subkeys 
			unsigned long	cbMaxSubKey = 0;					  // longest subkey size 
			unsigned long	cchMaxClass = 0;					  // longest class string 
			unsigned long	cValues = 0;					      // number of values for key 
			unsigned long	cchMaxValue = 0;					  // longest value name 
			unsigned long	cbMaxValueData = 0;					  // longest value data 
			unsigned long	cbSecurityDescriptor = 0;		      // size of security descriptor 
			// FILETIME		ftLastWriteTime;					  // last write time
			unsigned long	dataType = 0;
			std::basic_regex<wchar_t> fbr(prepRegexString_copy(valName));
			std::basic_regex<wchar_t> fbrlow(lower_copy(prepRegexString_copy(valName)));
			std::wstring seekpath;
			if (ERROR_SUCCESS == ::RegQueryInfoKey(
				keyHandle,				 // key handle 
				achClass,				 // buffer for class name 
				&cchClassName,			 // size of class string 
				NULL,				 	 // reserved 
				&cSubKeys,				 // number of subkeys 
				&cbMaxSubKey,            // longest subkey size 
				&cchMaxClass,            // longest class string 
				&cValues,                // number of values for this key 
				&cchMaxValue,            // longest value name 
				&cbMaxValueData,         // longest value data 
				&cbSecurityDescriptor,   // security descriptor 
				0 /* &ftLastWriteTime */)		 // last write time
				) {
				if (cSubKeys) {
					for (size_t i = 0; i < cSubKeys; ++i) {
						cbName = REG_MAX_KEY_LENGTH;
						::RegEnumKeyEx(keyHandle, i,
							achKey,
							&cbName,
							NULL,
							NULL,
							NULL,
							0 /* &ftLastWriteTime */);
						seekpath = hkey2wstr(rootKey);
						if (keyPath.length()) {
							if (endsWith(keyPath, L"\\")) {
								seekpath += removeFromStart_copy(keyPath, L"\\") + achKey;
							} else {
								seekpath += removeFromStart_copy(keyPath, L"\\") + L"\\" + achKey;
							}
						} else {
							seekpath += removeFromStart_copy(keyPath, L"\\") + achKey;
						}
						SeekValRec(valName, results, seekpath, root);
					}
				}
				if (cValues) {
					unsigned long buflen = REG_READBUFSZ;
					for (size_t i = 0; i < cValues; ++i) {
						cchValue = REG_MAX_VALUE_NAME;
						achValue[0] = 0;
						::RegEnumValue(keyHandle, i,
							achValue,
							&cchValue,
							0,
							&dataType,
							0,
							&buflen);
						if ((std::regex_match(achValue, fbr) ||
							std::regex_match(lower_copy(achValue), fbrlow) ||
							(lower_copy(valName) == lower_copy(achValue)))) {
							RegValDesc elem;
							elem.valPath = hkey2wstr(rootKey);
							if (wcslen_c(achValue)) {
								elem.valPath += keyPath + L"\\" + std::wstring(achValue);
								elem.valName = achValue;
								elem.valDataSz = buflen;
								elem.valType = static_cast<RegValType>(dataType);
								if (std::find(results.begin(), results.end(), elem) == results.end()) {
									results.push_back(elem);
								}
							}
						}
					}
				}
			}
			CLOSEKEY_NULLIFY(keyHandle);
		} else {
			if (keyHandle) {
				CLOSEKEY_NULLIFY(keyHandle);
			}
			return RegOpResult::Fail;
		}
		if (acquireVals) {
			return AcquireValues(results, limitResults, cleanupStrings, root);
		}
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::SeekKeyRec(const std::wstring keyName, std::vector<RegKeyDesc> &results,
	const std::wstring startPath, const HKEY *root) const {
	HKEY keyHandle = { 0 }, rootKey = { 0 };
	std::wstring keyPath = startPath;
	if (root) {
		rootKey = *root;
	}
	else {
		rootKey = { 0 };
	}
	prepHKEYKeyPath(rootKey, keyPath, rootKey, keyPath);
	if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
		KEY_READ | KEY_QUERY_VALUE | getRightMod(), &keyHandle)) {
		wchar_t			achClass[MAX_PATH] = L"";
		wchar_t			achValue[REG_MAX_VALUE_NAME] = { 0 }; // buffer for value name
		wchar_t			achKey[256] = { 0 };				  // buffer for subkey name
		unsigned long	cbName = 0;							  // size of name string 
		unsigned long	cchValue = REG_MAX_VALUE_NAME;		  // buffer for class name 
		unsigned long	cchClassName = MAX_PATH;			  // size of class string 
		unsigned long	cSubKeys = 0;						  // number of subkeys 
		unsigned long	cbMaxSubKey = 0;					  // longest subkey size 
		unsigned long	cchMaxClass = 0;					  // longest class string 
		unsigned long	cValues = 0;					      // number of values for key 
		unsigned long	cchMaxValue = 0;					  // longest value name 
		unsigned long	cbMaxValueData = 0;					  // longest value data 
		unsigned long	cbSecurityDescriptor = 0;		      // size of security descriptor 
		// FILETIME		ftLastWriteTime;					  // last write time
		unsigned long	dataType = 0;
		std::basic_regex<wchar_t> kbr(prepRegexString_copy(keyName));
		std::basic_regex<wchar_t> kbrlow(prepRegexString_copy(lower_copy(keyName)));
		std::wstring seekpath;
		bool matchFlag = false;
		std::wstring checkName = splitStr(keyPath, L"\\").back();
		if (checkName == keyName ||
			lower_copy(checkName) == lower_copy(keyName) ||
			std::regex_match(checkName, kbr) ||
			std::regex_match(lower_copy(checkName), kbrlow)) {
			matchFlag = true;
			RegKeyDesc keyelem;
			keyelem.keyPath = hkey2wstr(rootKey) + keyPath;
			keyelem.keyName = checkName;
			results.push_back(keyelem);
		}
		if (ERROR_SUCCESS == ::RegQueryInfoKey(
			keyHandle,				 // key handle 
			achClass,				 // buffer for class name 
			&cchClassName,			 // size of class string 
			NULL,				 	 // reserved 
			&cSubKeys,				 // number of subkeys 
			&cbMaxSubKey,            // longest subkey size 
			&cchMaxClass,            // longest class string 
			&cValues,                // number of values for this key 
			&cchMaxValue,            // longest value name 
			&cbMaxValueData,         // longest value data 
			&cbSecurityDescriptor,   // security descriptor 
			0 /* &ftLastWriteTime */)		 // last write time
			) {
			if (cSubKeys) {
				for (size_t i = 0; i < cSubKeys; ++i) {
					cbName = REG_MAX_KEY_LENGTH;
					::RegEnumKeyEx(keyHandle, i,
						achKey,
						&cbName,
						NULL,
						NULL,
						NULL,
						0 /* &ftLastWriteTime */);
					seekpath = hkey2wstr(rootKey);
					if (keyPath.length()) {
						if (endsWith(keyPath, L"\\")) {
							seekpath += removeFromStart_copy(keyPath, L"\\") + achKey;
						} else {
							seekpath += removeFromStart_copy(keyPath, L"\\") + L"\\" + achKey;
						}
					} else {
						seekpath += achKey;
					}
					if (matchFlag) {
						RegKeyDesc tskey;
						tskey.keyPath = seekpath;
						tskey.keyName = achKey;
						results.back().keys.push_back(tskey);
					}
					SeekKeyRec(keyName, results, seekpath, root);
				}
			}
			if (cValues && matchFlag) {
				unsigned long buflen = REG_READBUFSZ;
				for (size_t i = 0; i < cValues; ++i) {
					cchValue = REG_MAX_VALUE_NAME;
					achValue[0] = 0;
					::RegEnumValue(keyHandle, i,
						achValue,
						&cchValue,
						0,
						&dataType,
						0,
						&buflen);
						RegValDesc elem;
						elem.valPath = hkey2wstr(rootKey);
						if (wcslen_c(achValue)) {
							elem.valPath += keyPath + L"\\" + std::wstring(achValue);
							elem.valName = achValue;
							elem.valDataSz = buflen;
							elem.valType = static_cast<RegValType>(dataType);
							results.back().values.push_back(elem);
						}
				}
			}
		}
	}
	CLOSEKEY_NULLIFY(keyHandle);
	return RegOpResult::Success;
}

RegOpResult RegHandler::SeekValRec(const std::wstring valName,
	std::vector<RegValDesc> &results, const std::wstring startPath,
	const HKEY* root) const {
	if (valName.length()) {
		std::wstring path = startPath;
		if (path == L"") {
			path = L"HKEY_LOCAL_MACHINE\\";
		}
		// REGHELPERKEYBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPath(rootKey, path, rootKey, keyPath);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_READ | KEY_QUERY_VALUE | getRightMod(), &keyHandle)) {
			wchar_t			achClass[MAX_PATH] = L"";
			wchar_t			achValue[REG_MAX_VALUE_NAME] = { 0 }; // buffer for value name
			wchar_t			achKey[256] = { 0 };				  // buffer for subkey name
			unsigned long	cbName = 0;							  // size of name string 
			unsigned long	cchValue = REG_MAX_VALUE_NAME;		  // buffer for class name 
			unsigned long	cchClassName = MAX_PATH;			  // size of class string 
			unsigned long	cSubKeys = 0;						  // number of subkeys 
			unsigned long	cbMaxSubKey = 0;					  // longest subkey size 
			unsigned long	cchMaxClass = 0;					  // longest class string 
			unsigned long	cValues = 0;					      // number of values for key 
			unsigned long	cchMaxValue = 0;					  // longest value name 
			unsigned long	cbMaxValueData = 0;					  // longest value data 
			unsigned long	cbSecurityDescriptor = 0;		      // size of security descriptor 
			// FILETIME		ftLastWriteTime;					  // last write time
			unsigned long	dataType = 0;
			std::basic_regex<wchar_t> fbr(prepRegexString_copy(valName));
			std::basic_regex<wchar_t> fbrlow(lower_copy(prepRegexString_copy(valName)));
			std::wstring seekpath;
			if (ERROR_SUCCESS == ::RegQueryInfoKey(
				keyHandle,				 // key handle 
				achClass,				 // buffer for class name 
				&cchClassName,			 // size of class string 
				NULL,				 	 // reserved 
				&cSubKeys,				 // number of subkeys 
				&cbMaxSubKey,            // longest subkey size 
				&cchMaxClass,            // longest class string 
				&cValues,                // number of values for this key 
				&cchMaxValue,            // longest value name 
				&cbMaxValueData,         // longest value data 
				&cbSecurityDescriptor,   // security descriptor 
				0 /* &ftLastWriteTime */)		 // last write time
				) {
				if (cSubKeys) {
					for (size_t i = 0; i < cSubKeys; ++i) {
						cbName = REG_MAX_KEY_LENGTH;
						RegEnumKeyEx(keyHandle, i,
							achKey,
							&cbName,
							NULL,
							NULL,
							NULL,
							0 /* &ftLastWriteTime */);
						seekpath = hkey2wstr(rootKey);
						if (keyPath.length()) {
							if (endsWith(keyPath, L"\\")) {
								seekpath += removeFromStart_copy(keyPath, L"\\") + achKey;
							} else {
								seekpath += removeFromStart_copy(keyPath, L"\\") + L"\\" + achKey;
							}
						} else {
							seekpath += removeFromStart_copy(keyPath, L"\\") + achKey;
						}
						SeekValRec(valName, results, seekpath, root);
					}
				}
				if (cValues) {
					unsigned long buflen = REG_READBUFSZ;
					for (size_t i = 0; i < cValues; ++i) {
						cchValue = REG_MAX_VALUE_NAME;
						achValue[0] = 0;
						::RegEnumValue(keyHandle, i,
							achValue,
							&cchValue,
							0,
							&dataType,
							0,
							&buflen);
						if ((std::regex_match(achValue, fbr) ||
							std::regex_match(lower_copy(achValue), fbrlow) ||
							(lower_copy(valName) == lower_copy(achValue)))) {
							RegValDesc elem;
							elem.valPath = hkey2wstr(rootKey);
							if (wcslen_c(achValue)) {
								elem.valPath += keyPath + L"\\" + std::wstring(achValue);
								elem.valName = achValue;
								elem.valDataSz = buflen;
								elem.valType = static_cast<RegValType>(dataType);
								if (std::find(results.begin(), results.end(), elem) == results.end()) {
									results.push_back(elem);
								}
							}
						}
					}
				}
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Success;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Success;
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::SeekValRecType(const std::wstring valName,
	std::vector<RegValDesc> &results, const RegValType valType,
	const std::wstring startPath, const HKEY* root) const {
	if (valName.length()) {
		std::wstring path = startPath;
		if (path == L"") {
			path = L"HKEY_LOCAL_MACHINE\\";
		}
		// REGHELPERKEYBEGIN
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath, valueName = valName;
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPath(rootKey, path, rootKey, keyPath);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_READ | KEY_QUERY_VALUE | getRightMod(), &keyHandle)) {
			wchar_t			achClass[MAX_PATH] = L"";
			wchar_t			achValue[REG_MAX_VALUE_NAME] = { 0 }; // buffer for value name
			wchar_t			achKey[256] = { 0 };						  // buffer for subkey name
			unsigned long	cbName = 0;							  // size of name string 
			unsigned long	cchValue = REG_MAX_VALUE_NAME;		  // buffer for class name 
			unsigned long	cchClassName = MAX_PATH;			  // size of class string 
			unsigned long	cSubKeys = 0;						  // number of subkeys 
			unsigned long	cbMaxSubKey = 0;					  // longest subkey size 
			unsigned long	cchMaxClass = 0;					  // longest class string 
			unsigned long	cValues = 0;					      // number of values for key 
			unsigned long	cchMaxValue = 0;					  // longest value name 
			unsigned long	cbMaxValueData = 0;					  // longest value data 
			unsigned long	cbSecurityDescriptor = 0;		      // size of security descriptor 
			// FILETIME		ftLastWriteTime;					  // last write time
			unsigned long	dataType = 0;
			std::basic_regex<wchar_t> fbr(prepRegexString_copy(valName));
			std::basic_regex<wchar_t> fbrlow(lower_copy(prepRegexString_copy(valName)));
			std::wstring seekpath;
			if (ERROR_SUCCESS == ::RegQueryInfoKey(
				keyHandle,				 // key handle 
				achClass,				 // buffer for class name 
				&cchClassName,			 // size of class string 
				NULL,				 	 // reserved 
				&cSubKeys,				 // number of subkeys 
				&cbMaxSubKey,            // longest subkey size 
				&cchMaxClass,            // longest class string 
				&cValues,                // number of values for this key 
				&cchMaxValue,            // longest value name 
				&cbMaxValueData,         // longest value data 
				&cbSecurityDescriptor,   // security descriptor 
				0 /* &ftLastWriteTime */)		 // last write time
				) {
				if (cSubKeys) {
					for (size_t i = 0; i < cSubKeys; ++i) {
						cbName = REG_MAX_KEY_LENGTH;
						RegEnumKeyEx(keyHandle, i,
							achKey,
							&cbName,
							NULL,
							NULL,
							NULL,
							0 /* &ftLastWriteTime */);
						seekpath = hkey2wstr(rootKey);
						if (keyPath.length()) {
							if (endsWith(keyPath, L"\\")) {
								seekpath += removeFromStart_copy(keyPath, L"\\") + achKey;
							} else {
								seekpath += removeFromStart_copy(keyPath, L"\\") + L"\\" + achKey;
							}
						} else {
							seekpath += removeFromStart_copy(keyPath, L"\\") + achKey;
						}
						RegValType rvt;
						if (RegOpResult::Success != GetValueType(seekpath, rvt)) {
							SeekValRecType(valName, results, valType, seekpath, root);
						}
					}
				}
				if (cValues) {
					if (partialMatch(keyPath, L"software\\classes\\local settings\\software\\microsoft\\windows\\shell\\bagmru\\27")) {
						std::cout << "!!!" << std::endl;
					}
					unsigned long buflen = REG_READBUFSZ;
					for (size_t i = 0; i < cValues; ++i) {
						cchValue = REG_MAX_VALUE_NAME;
						achValue[0] = 0;
						::RegEnumValue(keyHandle, i,
							achValue,
							&cchValue,
							0,
							&dataType,
							0,
							&buflen);
						if ((std::regex_match(achValue, fbr) ||
							std::regex_match(lower_copy(achValue), fbrlow) ||
							(lower_copy(valName) == lower_copy(achValue))) &&
							(dataType == static_cast<unsigned long>(valType))) {
							RegValDesc elem;
							elem.valPath = hkey2wstr(rootKey);
							if (wcslen_c(achValue)) {
								elem.valPath += keyPath + L"\\" + std::wstring(achValue);
								RegValType rvt;
								if (RegOpResult::Success == GetValueType(elem.valPath, rvt)) {
									elem.valName = achValue;
									elem.valDataSz = buflen;
									elem.valType = valType;
									if (std::find(results.begin(), results.end(), elem) == results.end()) {
										results.push_back(elem);
									}
								}
							}
						}
					}
				}
			}
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Success;
		}
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::AcquireValues(RegKeyDesc &keyDesc, const unsigned long limitVals, const bool cleanupStrings,
	const HKEY *root) const {
	RegOpResult opres;
	if (keyDesc.keys.size()) {
		opres = AcquireValues(keyDesc.keys, limitVals, cleanupStrings, root);
		if (RegOpResult::Success != opres) {
			return opres;
		}
	}
	if (keyDesc.values.size()) {
		opres = AcquireValues(keyDesc.values, limitVals, cleanupStrings, root);
		if (RegOpResult::Success != opres) {
			return opres;
		}
	}
}

RegOpResult RegHandler::AcquireValues(std::vector<RegKeyDesc> &keyList,
	const unsigned long limitVals, const bool cleanupStrings,
	const HKEY *root) const {
	RegOpResult opres;
	for (size_t i = 0; i < keyList.size(); ++i) {
		if (keyList[i].keys.size()) {
			opres = AcquireValues(keyList[i].keys, limitVals, cleanupStrings, root);
			if (RegOpResult::Success != opres) {
				return opres;
			}
		}
		if (keyList[i].values.size()) {
			opres = AcquireValues(keyList[i].values, limitVals, cleanupStrings, root);
			if (RegOpResult::Success != opres) {
				return opres;
			}
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::AcquireValues(std::vector<RegValDesc> &valList,
	const unsigned long limitVals, const bool cleanupStrings, const HKEY *root) const {
	unsigned long resAcquired = 0;
	for (size_t i = 0; i < valList.size(); ++i) {
		if (limitVals) {
			if (resAcquired >= limitVals) {
				continue;
			}
		}
		if ((valList[i].valType == RegValType::Str ||
			valList[i].valType == RegValType::ExpandStr) &&
			!valList[i].valData) {
			if (valList[i].valDataSz) {
				std::wstring tstr;
				if (RegOpResult::Success == GetStrVal(valList[i].valPath,
					tstr, cleanupStrings, root)) {
					if (tstr.length()) {
						size_t sz = (tstr.length() + 1) * sizeof(wchar_t);
						valList[i].valData = new wchar_t[tstr.length() + 1];
						if (valList[i].valData) {
							memset(valList[i].valData, 0, sz); // *sizeof(wchar_t));
							swprintf((wchar_t*)valList[i].valData, L"%s", tstr.c_str());
							valList[i].valDataSz = sz;
							// std::cout << "seekval str ok " << resAcquired << std::endl;
							++resAcquired;
						} else {
							valList[i].valData = new wchar_t;
							if (valList[i].valData) {
								memcpy(valList[i].valData, gc_wc_emptyVal, sizeof(wchar_t));
								// valList[i].valData = (void*)gc_wc_emptyVal;
								valList[i].valDataSz = sizeof(wchar_t);
							}
						}
					} else {
						valList[i].valData = new wchar_t;
						if (valList[i].valData) {
							memcpy(valList[i].valData, &gc_wc_emptyVal, sizeof(wchar_t));
							// valList[i].valData = (void*)gc_wc_emptyVal;
							valList[i].valDataSz = sizeof(wchar_t);
						}
					}
				} else {
					valList[i].valData = new wchar_t;
					if (valList[i].valData) {
						memcpy(valList[i].valData, &gc_wc_emptyVal, sizeof(wchar_t));
						// valList[i].valData = (void*)gc_wc_emptyVal;
						valList[i].valDataSz = sizeof(wchar_t);
					}
				}
			} else {
				valList[i].valData = new wchar_t;
				if (valList[i].valData) {
					memcpy(valList[i].valData, &gc_wc_emptyVal, sizeof(wchar_t));
					// valList[i].valData = (void*)gc_wc_emptyVal;
					valList[i].valDataSz = sizeof(wchar_t);
				}
			}
		} else if (valList[i].valType == RegValType::MultiStr &&
			!valList[i].valData) {
			if (RegOpResult::Success == GetMultiStrVal(valList[i].valPath,
				(wchar_t*&)valList[i].valData,
				valList[i].valDataSz, root)) {
				// std::cout << "seekval multistr ok " << resAcquired << std::endl;
				++resAcquired;
			} else {
				std::cout << "seekval multistr NOT OK " << resAcquired << std::endl;
				valList[i].valData = new wchar_t;
				if (valList[i].valData) {
					memcpy(valList[i].valData, &gc_wc_emptyVal, sizeof(wchar_t));
					// valList[i].valData = (void*)gc_wc_emptyVal;
					valList[i].valDataSz = sizeof(wchar_t);
				}
			}
		} else if ((valList[i].valType == RegValType::DWord ||
			valList[i].valType == RegValType::DWordLE) &&
			!valList[i].valData) {
			unsigned long tval = 0;
			if (RegOpResult::Success == GetDWordVal(valList[i].valPath, tval,
				root)) {
				valList[i].valData = new unsigned long;
				if (valList[i].valData) {
					*(unsigned long*)valList[i].valData = 0;
					memcpy(valList[i].valData, &tval, sizeof(unsigned long));
					valList[i].valDataHex = ul2hexwstr(*(unsigned long*)valList[i].valData);
					valList[i].valDataSz = sizeof(unsigned long);
					// std::cout << "seekval dword ok " << resAcquired << std::endl;
					++resAcquired;
				}
			} else {
				valList[i].valData = new unsigned long;
				if (valList[i].valData) {
					memcpy(valList[i].valData, &gc_ul_incorrectVal, sizeof(unsigned long));
					// valList[i].valData = (void*)gc_ul_incorrectVal;
					valList[i].valDataSz = sizeof(unsigned long);
				}
			}
		} else if (valList[i].valType == RegValType::DWordBE &&
			!valList[i].valData) {
			unsigned long tval = 0;
			if (RegOpResult::Success == GetDWordVal(valList[i].valPath, tval, root)) {
				BYTESWAP32(tval);
				valList[i].valData = new unsigned long;
				if (valList[i].valData) {
					*(unsigned long*)valList[i].valData = 0;
					memcpy(valList[i].valData, &tval, sizeof(unsigned long));
					valList[i].valDataHex = ul2hexwstr(*(unsigned long*)valList[i].valData);
					valList[i].valDataSz = sizeof(unsigned long);
					// std::cout << "seekval dwordbe ok " << resAcquired << std::endl;
					++resAcquired;
				}
			} else {
				valList[i].valData = new unsigned long;
				if (valList[i].valData) {
					memcpy(valList[i].valData, &gc_ul_incorrectVal, sizeof(unsigned long));
					// valList[i].valData = (void*)gc_ul_incorrectVal;
					valList[i].valDataSz = sizeof(unsigned long);
				}
			}
		} else if ((valList[i].valType == RegValType::QWord ||
			valList[i].valType == RegValType::QWordLE) &&
			!valList[i].valData) {
			unsigned long long tval = 0;
			if (RegOpResult::Success == GetQWordVal(valList[i].valPath, tval, root)) {
				valList[i].valData = new unsigned long long;
				if (valList[i].valData) {
					*(unsigned long long*)valList[i].valData = 0;
					memcpy(valList[i].valData, &tval, sizeof(unsigned long long));
					valList[i].valDataHex = ull2hexwstr(*(unsigned long long*)valList[i].valData);
					valList[i].valDataSz = sizeof(unsigned long long);
					// std::cout << "seekval qword ok " << resAcquired << std::endl;
					++resAcquired;
				}
			} else {
				valList[i].valData = new unsigned long long;
				if (valList[i].valData) {
					memcpy(valList[i].valData, &gc_ull_incorrectVal, sizeof(unsigned long long));
					// valList[i].valData = (void*)gc_ull_incorrectVal;
					valList[i].valDataSz = sizeof(unsigned long long);
				}
			}
		} else if ((valList[i].valType == RegValType::Binary ||
			valList[i].valType == RegValType::FullResourceDesc ||
			valList[i].valType == RegValType::ResourceRequirementsList ||
			valList[i].valType == RegValType::ResourceList ||
			valList[i].valType == RegValType::None) &&
			!valList[i].valData) {
			if (RegOpResult::Success == GetBinaryVal(valList[i].valPath,
				(unsigned char*&)valList[i].valData,
				valList[i].valDataSz, root)) {
				if (valList[i].valData) {
					// std::cout << "seekval bin ok " << resAcquired << std::endl;
					++resAcquired;
				}/* else {
					std::cout << "seekval bin NOT OK " << resAcquired << std::endl;
					std::wcout << valList[i].valPath << std::endl;
				}*/
			} else {
				valList[i].valData = new unsigned long;
				if (valList[i].valData) {
					memcpy(valList[i].valData, &gc_ul_incorrectVal, sizeof(unsigned long));
					// valList[i].valData = (void*)gc_ul_incorrectVal;
					valList[i].valDataSz = sizeof(unsigned long);
				}
			}
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::CreateValues(const RegKeyDesc &keyDesc, const HKEY* root) const {
	RegOpResult opres;
	if (keyDesc.keys.size()) {
		opres = CreateValues(keyDesc.keys, root);
		if (RegOpResult::Success != opres) {
			return opres;
		}
	}
	if (keyDesc.values.size()) {
		opres = CreateValues(keyDesc.values, root);
		if (RegOpResult::Success != opres) {
			return opres;
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::CreateValues(const std::vector<RegKeyDesc> &keyList, const HKEY *root) const {
	RegOpResult opres;
	for (size_t i = 0; i < keyList.size(); ++i) {
		if (keyList[i].keys.size()) {
			opres = CreateValues(keyList[i].keys, root);
			if (RegOpResult::Success != opres) {
				return opres;
			}
		}
		if (keyList[i].values.size()) {
			opres = CreateValues(keyList[i].values, root);
			if (RegOpResult::Success != opres) {
				return opres;
			}
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::CreateValues(const std::vector<RegValDesc> &valList,
	const HKEY *root) const {
	if (valList.size()) {
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::vector<std::wstring> strSpl;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		std::wstring valName, keyPath;
		for (size_t i = 0; i < valList.size(); ++i) {
			prepHKEYKeyPathValueName(rootKey, rootKey, valList[i].valPath, keyPath, valName);
			if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
				KEY_ALL_ACCESS | getRightMod(), &keyHandle)) {
				if (ERROR_SUCCESS != RegSetValueEx(keyHandle, valList[i].valName.c_str(),
					0, static_cast<unsigned long>(valList[i].valType),
					(unsigned char*)valList[i].valData, valList[i].valDataSz)) {
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				} else {
					CLOSEKEY_NULLIFY(keyHandle);
				}
			}
			if(keyHandle) {
				CLOSEKEY_NULLIFY(keyHandle);
			}
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::FreeValues(const RegKeyDesc &keyDesc, const HKEY *root) const {
	RegOpResult opres;
	if (keyDesc.keys.size()) {
		opres = FreeValues(keyDesc.keys, root);
		if (RegOpResult::Success != opres) {
			return opres;
		}
	}
	if (keyDesc.values.size()) {
		opres = FreeValues(keyDesc.values, root);
		if (RegOpResult::Success != opres) {
			return opres;
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::FreeValues(const std::vector<RegKeyDesc> &keyList, const HKEY *root) const {
	RegOpResult opres;
	for (size_t i = 0; i < keyList.size(); ++i) {
		if (keyList[i].keys.size()) {
			opres = FreeValues(keyList[i].keys, root);
			if (RegOpResult::Success != opres) {
				return opres;
			}
		}
		if (keyList[i].values.size()) {
			opres = FreeValues(keyList[i].values, root);
			if (RegOpResult::Success != opres) {
				return opres;
			}
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::FreeValues(const std::vector<RegValDesc> &valList, const HKEY *root) const {
	if (valList.size()) {
		for (size_t i = 0; i < valList.size(); ++i) {
			if (valList[i].valData) {
				auto &itemunconst = const_cast<RegValDesc&>(valList[i]);
				itemunconst.FreeData();
			}
		}
	} else {
		return RegOpResult::Fail;
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::GetKeySecurity(const std::wstring keyName, SecDesc &secDesc, const HKEY *root) const {
	HKEY keyHandle = { 0 }, rootKey = { 0 };
	std::wstring keyPath = keyName;
	if (root) {
		rootKey = *root;
	} else {
		rootKey = { 0 };
	}
	ProcessHandler proc;
	unsigned long procid = proc.GetCurrentProcPid();
	std::vector<std::wstring> privs = proc.GetProcPrivileges(procid);
	if (!valInList(privs, L"SeSecurityPrivilege")) {
		if (!proc.EnableSecurityPrivilege(procid)) {
			return RegOpResult::Fail;
		}
	}
	prepHKEYKeyPath(rootKey, keyPath, rootKey, keyPath);
	if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0, KEY_READ | ACCESS_SYSTEM_SECURITY | getRightMod(),
		&keyHandle)) {
		unsigned long secinfosz = 0;
		if (ERROR_INSUFFICIENT_BUFFER == ::RegGetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::DACLSecInfo),
			secDesc.daclInfo, &secinfosz)) {
			secDesc.daclInfo = LocalAlloc(LPTR, secinfosz);
			if (secDesc.daclInfo) {
				if (ERROR_SUCCESS != ::RegGetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::DACLSecInfo),
					secDesc.daclInfo, &secinfosz)) {
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				} else {
					secDesc.daclInfoSz = secinfosz;
				}
			} else {
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Fail;
			}
		} else {
			CLOSEKEY_NULLIFY(keyHandle); 
			return RegOpResult::Fail;
		}
		secinfosz = 0;
		unsigned long res = 0;
		res = ::RegGetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::SACLSecInfo),
			secDesc.saclInfo, &secinfosz);
		if (ERROR_INSUFFICIENT_BUFFER == res) {
			secDesc.saclInfo = LocalAlloc(LPTR, secinfosz);
			if (secDesc.saclInfo) {
				if (ERROR_SUCCESS != ::RegGetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::SACLSecInfo),
					secDesc.saclInfo, &secinfosz)) {
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				} else {
					secDesc.saclInfoSz = secinfosz;
				}
			} else {
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Fail;
			}
		} else if ((ERROR_ACCESS_DENIED == res) || (ERROR_SUCCESS == res)) {
			secDesc.saclInfoSz = 0;
		} else {
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		secinfosz = 0;
		ACLHandler aclh;
		SysHandler sys;
		::SECURITY_DESCRIPTOR* ownerSecDesc = 0;
		if (ERROR_INSUFFICIENT_BUFFER == ::RegGetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::OwnerSecInfo),
			ownerSecDesc, &secinfosz)) {
			::SECURITY_DESCRIPTOR* ownerSecDesc = (::SECURITY_DESCRIPTOR*)::LocalAlloc(LPTR, secinfosz);
			if (ownerSecDesc) {
				if (ERROR_SUCCESS != ::RegGetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::OwnerSecInfo),
					ownerSecDesc, &secinfosz)) {
					SAFE_LOCALFREE(ownerSecDesc);
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				} else {
					::PSID tsid = 0;
					if (ACLOpResult::Success != aclh.OwnerSIDFromSecurityDescriptor(ownerSecDesc, tsid)) {
						SAFE_LOCALFREE(ownerSecDesc);
						CLOSEKEY_NULLIFY(keyHandle);
						return RegOpResult::Fail;
					}
					secDesc.ownerInfo = sys.StrSIDFromSID(tsid);
					secDesc.ownerInfoSz = secDesc.ownerInfo.length() * sizeof(wchar_t);
					SAFE_LOCALFREE(ownerSecDesc);
				}
			} else {
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Fail;
			}
		} else {
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		secinfosz = 0;
		::SECURITY_DESCRIPTOR* primGroupSecDesc = 0;
		if (ERROR_INSUFFICIENT_BUFFER == ::RegGetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::GroupSecInfo),
			primGroupSecDesc, &secinfosz)) {
			primGroupSecDesc = (::SECURITY_DESCRIPTOR*)LocalAlloc(LPTR, secinfosz);
			if (primGroupSecDesc) {
				if (ERROR_SUCCESS != ::RegGetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::GroupSecInfo),
					primGroupSecDesc, &secinfosz)) {
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				} else {
					::PSID tsid = 0;
					if (ACLOpResult::Success != aclh.PrimaryGroupSIDFromSecurityDescriptor(primGroupSecDesc, tsid)) {
						SAFE_LOCALFREE(primGroupSecDesc);
						CLOSEKEY_NULLIFY(keyHandle);
						return RegOpResult::Fail;
					}
					secDesc.primaryGroupInfo = sys.StrSIDFromSID(tsid);
					secDesc.primaryGroupInfoSz = secDesc.primaryGroupInfo.length() * sizeof(wchar_t);
					SAFE_LOCALFREE(primGroupSecDesc);
				}
			}
			CLOSEKEY_NULLIFY(keyHandle);
			ACLHandler aclh;
			if (ACLOpResult::Success == aclh.CreateAbsoluteSecDesc(secDesc)) {
				return RegOpResult::Success;
			}
			return RegOpResult::Fail;
		} else {
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
	} else {
		if (keyHandle) {
			CLOSEKEY_NULLIFY(keyHandle);
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::SetKeySecurity(const std::wstring keyName, SecDesc &secDesc, const HKEY *root) const {
	HKEY keyHandle = { 0 }, rootKey = { 0 };
	std::wstring keyPath = keyName;
	if (root) {
		rootKey = *root;
	} else {
		rootKey = { 0 };
	}
	ACLHandler aclh;
	ProcessHandler proc;
	unsigned long procid = proc.GetCurrentProcPid();
	std::vector<std::wstring> privs = proc.GetProcPrivileges(procid);
	if (!valInList(privs, L"SeSecurityPrivilege")) {
		if (!proc.EnableSecurityPrivilege(procid)) {
			return RegOpResult::Fail;
		}
	}
	if (!valInList(privs, L"SeBackupPrivilege")) {
		if (!proc.EnableBackupPrivilege(procid)) {
			return RegOpResult::Fail;
		}
	}
	if (!valInList(privs, L"SeRestorePrivilege")) {
		if (!proc.EnableRestorePrivilege(procid)) {
			return RegOpResult::Fail;
		}
	}
	if (!valInList(privs, L"SeTakeOwnershipPrivilege")) {
		if (!proc.EnableTakeOwnershipPrivilege(procid)) {
			return RegOpResult::Fail;
		}
	}
	prepHKEYKeyPath(rootKey, keyPath, rootKey, keyPath);
	unsigned long opkeyres = ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
		KEY_ALL_ACCESS | ACCESS_SYSTEM_SECURITY | getRightMod(), &keyHandle);
	if (ERROR_SUCCESS == opkeyres) {
		unsigned long res = 0;
		SysHandler sys;
#ifdef _WIN64
		res = ::RegSetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::DACLSecInfo),
			secDesc.absoluteSDInfo);
#else
		::ACL* acllist = 0;
		if (ACLOpResult::Success != aclh.DACLFromSecurityDescriptor((::SECURITY_DESCRIPTOR*)secDesc.absoluteSDInfo,
			acllist)) {
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		::SECURITY_DESCRIPTOR* sd = (::SECURITY_DESCRIPTOR*)::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		if (!sd) {
			SAFE_LOCALFREE(acllist);
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (::InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION)) {
			if (!::SetSecurityDescriptorDacl(sd, true, acllist, false)) {
				SAFE_LOCALFREE(acllist);
				SAFE_LOCALFREE(sd);
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Fail;
			}
		} else {
			SAFE_LOCALFREE(acllist);
			SAFE_LOCALFREE(sd);
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		res = ::RegSetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::DACLSecInfo),
			sd);
		SAFE_LOCALFREE(acllist);
		SAFE_LOCALFREE(sd);
		if (ERROR_SUCCESS != res) {
			return RegOpResult::Fail;
		}
#endif
#ifdef _WIN64
		::ACL* acllist = 0;
#endif
		if (ACLOpResult::Success != aclh.SACLFromSecurityDescriptor((::SECURITY_DESCRIPTOR*)secDesc.absoluteSDInfo,
			acllist)) {
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
#ifndef _WIN64
		memset(sd, 0, SECURITY_DESCRIPTOR_MIN_LENGTH);
#else
		::SECURITY_DESCRIPTOR* sd = (::SECURITY_DESCRIPTOR*)::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
#endif // _WIN64
		if (!sd) {
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (::InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION)) {
			if (!::SetSecurityDescriptorSacl(sd, true, acllist, false)) {
				SAFE_LOCALFREE(sd);
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Fail;
			}
		} else {
			SAFE_LOCALFREE(sd);
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		res = ::RegSetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::SACLSecInfo), sd);
		SAFE_LOCALFREE(sd);
		if (ERROR_SUCCESS != res) {
			return RegOpResult::Fail;
		}
		sd = (::SECURITY_DESCRIPTOR*)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		if (!sd) {
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (::InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION)) {
			::PSID tsid = sys.SIDFromStrSid(secDesc.ownerInfo);
			if (tsid) {
				if (!::SetSecurityDescriptorOwner(sd, tsid, false)) {
					SAFE_LOCALFREE(sd);
					SAFE_LOCALFREE(tsid);
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				}
				res = ::RegSetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::OwnerSecInfo), sd);
				if (ERROR_SUCCESS != res) {
					SAFE_LOCALFREE(sd);
					SAFE_LOCALFREE(tsid);
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				}
				SAFE_LOCALFREE(sd);
				SAFE_LOCALFREE(tsid);
			} else {
				SAFE_LOCALFREE(sd);
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Fail;
			}
		} else {
			SAFE_LOCALFREE(sd);
			return RegOpResult::Fail;
		}
		res = ::RegSetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::OwnerSecInfo), sd);
		SAFE_LOCALFREE(sd);
		if (ERROR_SUCCESS != res) {
			return RegOpResult::Fail;
		}
		sd = (::SECURITY_DESCRIPTOR*)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		if (!sd) {
			CLOSEKEY_NULLIFY(keyHandle);
			return RegOpResult::Fail;
		}
		if (::InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION)) {
			::PSID tsid = sys.SIDFromStrSid(secDesc.primaryGroupInfo);
			if (tsid) {
				if (!::SetSecurityDescriptorOwner(sd, tsid, false)) {
					SAFE_LOCALFREE(sd);
					SAFE_LOCALFREE(tsid);
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				}
				res = ::RegSetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::GroupSecInfo), sd);
				if (ERROR_SUCCESS != res) {
					SAFE_LOCALFREE(sd);
					SAFE_LOCALFREE(tsid);
					CLOSEKEY_NULLIFY(keyHandle);
					return RegOpResult::Fail;
				}
				SAFE_LOCALFREE(sd);
				SAFE_LOCALFREE(tsid);
			} else {
				SAFE_LOCALFREE(sd);
				CLOSEKEY_NULLIFY(keyHandle);
				return RegOpResult::Fail;
			}
		} else {
			SAFE_LOCALFREE(sd);
			return RegOpResult::Fail;
		}
		res = ::RegSetKeySecurity(keyHandle, static_cast<unsigned long>(SecInfo::OwnerSecInfo), sd);
		SAFE_LOCALFREE(sd);
		if (ERROR_SUCCESS != res) {
			return RegOpResult::Fail;
		}
		CLOSEKEY_NULLIFY(keyHandle);
	}
	CLOSEKEY_NULLIFY(keyHandle);
	return RegOpResult::Success;
}

RegOpResult RegHandler::CreateKey(const std::wstring keyName, const bool createMissingKeys, const HKEY *root) const {
	if (keyName.length()) {
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath = keyName;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		if (createMissingKeys) {
			std::vector<std::wstring> keys = splitStr(keyName, L"\\");
			if (keys.size() > 1) {
				for (size_t i = 1; i < keys.size() - 1; ++i) {
					std::wstring pathJoined = joinStrs(keys, L"\\", 0, i + 1);
					prepHKEYKeyPath(rootKey, pathJoined, rootKey, pathJoined);
					if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, pathJoined.c_str(), 0,
						KEY_READ | getRightMod(), &keyHandle)) {
						HKEY tkey = { 0 };
						if (ERROR_SUCCESS == ::RegCreateKey(keyHandle, keys[i + 1].c_str(),
							&tkey)) {
							CLOSEKEY_NULLIFY(tkey);
						} else {
							if (tkey) {
								CLOSEKEY_NULLIFY(tkey);
							}
							CLOSEKEY_NULLIFY(keyHandle);
							return RegOpResult::Fail;
						}
					} else {
						if (keyHandle) {
							CLOSEKEY_NULLIFY(keyHandle);
						}
						return RegOpResult::Fail;
					}
					if (keyHandle) {
						CLOSEKEY_NULLIFY(keyHandle);
					}
				}
				return RegOpResult::Success;
			} else {
				return RegOpResult::Fail;
			}
		} else {
			prepHKEYKeyPath(rootKey, keyName, rootKey, keyPath);
			HKEY tkey = { 0 };
			if (ERROR_SUCCESS == ::RegCreateKey(rootKey, keyPath.c_str(), &tkey)) {
				CLOSEKEY_NULLIFY(tkey);
				return RegOpResult::Success;
			} else {
				if (tkey) {
					CLOSEKEY_NULLIFY(tkey);
				}
				return RegOpResult::Fail;
			}
		}
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::DeleteValues(const RegKeyDesc &keyDesc, const HKEY *root) const {
	RegOpResult opres;
	if (keyDesc.keys.size()) {
		opres = DeleteValues(keyDesc.keys, root);
		if (RegOpResult::Success != opres) {
			return opres;
		}
	}
	if (keyDesc.values.size()) {
		opres = DeleteValues(keyDesc.values, root);
		if (RegOpResult::Success != opres) {
			return opres;
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::DeleteValues(const std::vector<RegKeyDesc>& keyList, const HKEY* root) const {
	RegOpResult opres;
	for (size_t i = 0; i < keyList.size(); ++i) {
		if (keyList[i].keys.size()) {
			opres = DeleteValues(keyList[i].keys, root);
			if (RegOpResult::Success != opres) {
				return opres;
			}
		}
		if (keyList[i].values.size()) {
			opres = DeleteValues(keyList[i].values, root);
			if (RegOpResult::Success != opres) {
				return opres;
			}
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::DeleteValues(const std::vector<RegValDesc> &valList,
	const HKEY *root) const {
	HKEY keyHandle = { 0 }, rootKey = { 0 };
	std::wstring keyPath, valueName; // = valName;
	std::vector<std::wstring> strSpl;
	if (root) {
		rootKey = *root;
	} else {
		rootKey = { 0 };
	}
	for (size_t i = 0; i < valList.size(); ++i) {
		prepHKEYKeyPathValueName(rootKey, rootKey, valList[i].valPath, keyPath, valueName);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_ALL_ACCESS | getRightMod(), &keyHandle)) {
			if (ERROR_SUCCESS != ::RegDeleteValue(keyHandle, valueName.c_str())) {
				if (keyHandle) {
					CLOSEKEY_NULLIFY(keyHandle);
				}
				return RegOpResult::Fail;
			} else {
				CLOSEKEY_NULLIFY(keyHandle);
			}
			if(keyHandle) {
				CLOSEKEY_NULLIFY(keyHandle);
			}
		} else {
			if (keyHandle) {
				CLOSEKEY_NULLIFY(keyHandle);
			}
			return RegOpResult::Fail;
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::DeleteKey(const std::wstring keyName, const bool deleteSubKeys,
	const HKEY *root) const {
	if (keyName.length()) {
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath = keyName;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPath(rootKey, keyName, rootKey, keyPath);
		if (deleteSubKeys) {
			if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
				KEY_READ | getRightMod(), &keyHandle)) {
				std::wstring keyPathRec = rebuildSearchKeyPath(rootKey, keyPath, L"");
				long res = 0;
				unsigned long size = MAX_PATH;
				wchar_t keynameBuf[MAX_PATH] = { 0 };
				memcpy(keynameBuf, keyPathRec.c_str(), keyPath.length() * sizeof(wchar_t));
				FILETIME filetime = { 0 };
				res = RegEnumKeyEx(keyHandle, 0, keynameBuf, &size, NULL, NULL, NULL, &filetime);
				if (ERROR_SUCCESS == res) {
					do {
						keyPathRec = rebuildSearchKeyPath(rootKey, keyPath, keynameBuf);
						if (RegOpResult::Success != DeleteKey(keyPathRec, deleteSubKeys,
							root)) {
							break;
						}
						size = MAX_PATH;
						res = RegEnumKeyEx(keyHandle, 0, keynameBuf, &size,
							NULL, NULL, NULL, &filetime);
					} while (ERROR_SUCCESS == res);
				}
			} else {
				errorMsgBox(getErrorDetails(getLastErrorCode(), L"RegOpenKey",
					L"Failed to open key: " + keyPath));
				if (keyHandle) {
					CLOSEKEY_NULLIFY(keyHandle);
				}
				return RegOpResult::Fail;
			}
		}
		unsigned long mod = 0;
#ifdef _WIN64
		mod = 0;
#else
		mod = RIGHTMOD;
#endif // _WIN64
		std::vector<std::wstring> pathmatch;
		pathmatch.push_back(L"Wow6432Node");
		for (size_t i = 0; i < pathmatch.size(); ++i) {
			if (partialMatch(keyPath, pathmatch[i])) {
				mod = KEY_WOW64_32KEY;
				break;
			}
		}
		long res = RegDeleteKeyEx(rootKey, keyPath.c_str(), mod, 0);
		if (ERROR_SUCCESS != res) {
			errorMsgBox(getErrorDetails(getLastErrorCode(), L"RegDeleteKey",
				L"Delete operation result: " + std::to_wstring(res)));
			if (keyHandle) {
				CLOSEKEY_NULLIFY(keyHandle);
			}
			return RegOpResult::Fail;
		} else {
			if (keyHandle) {
				CLOSEKEY_NULLIFY(keyHandle);
			}
			return RegOpResult::Success;
		}
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::CopyKey(const std::wstring source,
	const std::wstring destination, const bool copyVals, const HKEY* root) const {
	if (source.length() && destination.length()) {
		HKEY keyHandle = { 0 }, rootKey = { 0 };
		std::wstring keyPath = source;
		if (root) {
			rootKey = *root;
		} else {
			rootKey = { 0 };
		}
		prepHKEYKeyPath(rootKey, source, rootKey, keyPath);
		if (ERROR_SUCCESS == ::RegOpenKeyEx(rootKey, keyPath.c_str(), 0,
			KEY_READ | getRightMod(), &keyHandle)) {
			CLOSEKEY_NULLIFY(keyHandle);
		} else {
			if (keyHandle) {
				CLOSEKEY_NULLIFY(keyHandle);
			}
			return RegOpResult::Fail;
		}
		if (RegOpResult::Success != CreateKey(destination, true, root)) {
			return RegOpResult::Fail;
		}
		if (copyVals) {
			std::vector<RegValDesc> vals;
			if (RegOpResult::Success == SeekVal(L".*", vals, source, true, 0, false, root)) {
				for (size_t i = 0; i < vals.size(); ++i) {
					vals[i].valPath = replaceSubstr_copy(vals[i].valPath, source, destination);
				}
				if (RegOpResult::Success == CreateValues(vals, root)) {
					FreeValues(vals);
					return RegOpResult::Success;
				} else {
					FreeValues(vals);
					return RegOpResult::Fail;
				}
			} else {
				FreeValues(vals);
				return RegOpResult::Fail;
			}
		}
	}
	return RegOpResult::Success;
}

RegOpResult RegHandler::MoveKey(const std::wstring source,
	const std::wstring destination, const bool moveValues, const HKEY *root) const {
	if (partialMatch(source, destination)) {
		return RegOpResult::Fail;
	}
	if (RegOpResult::Success == CopyKey(source, destination, moveValues, root)) {
		if (RegOpResult::Success == DeleteKey(source, true, root)) {
			return RegOpResult::Success;
		} 
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

unsigned long long RegHandler::ReadResourceList(const unsigned char* &resListBuf,
	const size_t bufSz) const {
	unsigned long busCount = *(unsigned long*)resListBuf;
	if (busCount == 1) {
		unsigned long resCount = *(unsigned long*)(resListBuf + 16);
		unsigned long long ret = 0;
		unsigned char* pmi = (unsigned char*)resListBuf + 24;
		for (unsigned long i = 0; i < resCount; ++i) {
			ret += *(unsigned long*)(pmi + 8);
			pmi += 16;
		}
		return ret;
	} else {
		return 0;
	}
}

std::vector<std::wstring> RegHandler::MultiStrBuf2Vector(const wchar_t* multiStrBuf,
	const size_t bufSz) const {
	wchar_t* newBuf = replaceChars((wchar_t*)multiStrBuf, L"\0", L"\n", bufSz, 1, 1);
	std::wstring tmpstr = newBuf;
	SAFE_ARR_DELETE(newBuf);
	for (size_t i = 0; i < tmpstr.length() - 1; ++i) {
		if (static_cast<int>(tmpstr[i]) == 10 &&
			static_cast<int>(tmpstr[i + 1]) == 10) {
			tmpstr.erase(i, tmpstr.length() - i);
			// break;
		}
	}
	std::vector<std::wstring> spl = splitStr(tmpstr, L"\n");
	return spl;
}

RegOpResult RegHandler::EnumUserProfileHives(std::vector<std::wstring> &hiveList,
	const bool mountHives) {
	std::vector<RegValDesc> vals;
	RegOpResult seekres = SeekVal(L"ProfileImagePath", vals,
		L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList",
		true, 0, false);
	if (RegOpResult::Success != seekres) {
		return seekres;
	}
	std::wstring profileHivePath;
	for (size_t i = 0; i < vals.size(); ++i) {
		profileHivePath = std::wstring((wchar_t*)vals[i].valData) + L"\\ntuser.dat";
		if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(profileHivePath.c_str())) {
			hiveList.push_back(profileHivePath);
			if (mountHives) {
				MountHive_LoadKey(hiveList.back());
			}
		}
	}
	FreeValues(vals);
	return RegOpResult::Success;
}

RegOpResult RegHandler::MountHive_LoadKey(const std::wstring hivePath,
	const RegLoadTarget loadTarget) {
	if (hivePath.length()) {
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(hivePath.c_str())) {
			return RegOpResult::Fail;
		}
		ProcessHandler proc;
		unsigned long procid = proc.GetCurrentProcPid();
		std::vector<std::wstring> privs = proc.GetProcPrivileges(procid);
		if (!valInList(privs, L"SeBackupPrivilege")) {
			if (!proc.EnableBackupPrivilege(procid)) {
				return RegOpResult::Fail;
			}
		}
		if (!valInList(privs, L"SeRestorePrivilege")) {
			if (!proc.EnableRestorePrivilege(procid)) {
				return RegOpResult::Fail;
			}
		}
		HKEY loadHiveTgt = pickLoadTarget(loadTarget);
		if (loadHiveTgt) {
			std::wstring loadHiveKey = pickLoadKey(hivePath);
			long res = ::RegLoadKey(loadHiveTgt, loadHiveKey.c_str(), hivePath.c_str());
			if (ERROR_SUCCESS == res) {
				m_mountedHives[loadHiveKey] = loadHiveTgt;
				++m_mountedHiveCount;
				return RegOpResult::Success;
			}
			errorMsgBox(getErrorDetails(getLastErrorCode(), L"RegOpenKey",
				L"Failed to mount hive: " + hivePath));
			return RegOpResult::Fail;
		}
		return RegOpResult::Fail;
	}
	return RegOpResult::Fail;
}

RegOpResult RegHandler::UnmountHive_UnloadKey(const std::wstring unloadKeyName,
	const RegLoadTarget unloadTarget) {
	if (unloadKeyName.length()) {
		ProcessHandler proc;
		unsigned long procid = proc.GetCurrentProcPid();
		std::vector<std::wstring> privs = proc.GetProcPrivileges(procid);
		if (!valInList(privs, L"SeBackupPrivilege")) {
			if (!proc.EnableBackupPrivilege(procid)) {
				return RegOpResult::Fail;
			}
		}
		if (!valInList(privs, L"SeRestorePrivilege")) {
			if (!proc.EnableRestorePrivilege(procid)) {
				return RegOpResult::Fail;
			}
		}
		HKEY unloadKey = pickUnloadKey(unloadKeyName);
		if (!unloadKey) {
			unloadKey = pickLoadTarget(unloadTarget);
		}
		if (unloadKey) {
			if (ERROR_SUCCESS == ::RegUnLoadKey(unloadKey, unloadKeyName.c_str())) {
				for (auto &it : m_mountedHives) {
					if (lower_copy(it.first) == lower_copy(unloadKeyName) && it.second == unloadKey) {
						m_mountedHives.erase(it.first);
						break;
					}
				}
				return RegOpResult::Success;
			} else {
				return RegOpResult::Fail;
			}
		} else {
			return RegOpResult::Fail;
		}
	} else {
		return RegOpResult::Fail;
	}
}

RegOpResult RegHandler::ConnectRegistry(HKEY &connectedReg, const std::wstring userName, const std::wstring password,
	const std::wstring remoteComputerName, const bool checkPing, const unsigned char pingAttempts) {
	std::wstring machineName = startsWith(remoteComputerName, L"\\\\") ?
		remoteComputerName : L"\\\\" + remoteComputerName;
	if (checkPing) {
		std::vector<PingResult> pingres;
		if (NetOpResult::Success != ping(pingres, remoteComputerName, pingAttempts)) {
			return RegOpResult::Fail;
		}
		bool pr = true;
		for (size_t i = 0; i < pingres.size(); ++i) {
			pr &= pingres[i].Result;
		}
		if (!pr) {
			return RegOpResult::Fail;
		}
	}
	::HANDLE token = 0;
	SysHandler sys;
	if (SysOpResult::Success == sys.UserLogon(token, userName, password, remoteComputerName)) {
		if (SysOpResult::Success == sys.ImpersonateUser(token)) {
			if (ERROR_SUCCESS == ::RegConnectRegistry(machineName.c_str(), HKEY_LOCAL_MACHINE, &connectedReg)) {
				if (connectedReg) {
					return RegOpResult::Success;
				} else {
					return RegOpResult::Fail;
				}
			} else {
				return RegOpResult::Fail;
			}
		} else {
			return RegOpResult::Fail;
		}
	} else {
		return RegOpResult::Fail;
	}
}

RegOpResult RegHandler::DisconnectRegistry(HKEY connectedReg) {
	long res = CLOSEKEY_NULLIFY(connectedReg);
	if (ERROR_SUCCESS == res) {
		return RegOpResult::Success;
	} else {
		return RegOpResult::Fail;
	}
}

std::wstring RegHandler::rebuildSearchKeyPath(const HKEY &root,
	const std::wstring &origPath, const std::wstring keyName) const {
	std::wstring ret;
	if (origPath.length()) {
		if (endsWith(origPath, L"\\")) {
			ret = origPath + keyName;
		} else {
			ret = origPath + L"\\" + keyName;
		}
	} else {
		ret = keyName;
	}
	if (root == HKEY_CURRENT_USER) {
		ret = L"HKEY_CURRENT_USER\\" + ret;
	} else if (root == HKEY_CLASSES_ROOT) {
		ret = L"HKEY_CLASSES_ROOT\\" + ret;
	} else if (root == HKEY_LOCAL_MACHINE) {
		ret = L"HKEY_LOCAL_MACHINE\\" + ret;
	} else if (root == HKEY_USERS) {
		ret = L"HKEY_USERS\\" + ret;
	} else if (root == HKEY_CURRENT_CONFIG) {
		ret = L"HKEY_CURRENT_CONFIG\\" + ret;
	}
	return ret;
}

unsigned long RegHandler::getRightMod() const {
	#ifdef _WIN64
		return 0;
		// return KEY_WOW64_32KEY;
	#else
		SysHandler sys;
		if (sys.IsWow64Proc()) {
			return RIGHTMOD;
		} else {
			return 0;
		}
	#endif
}

inline std::wstring RegHandler::pickLoadKey(const std::wstring hivePath) const {
	std::wstring pathPrepped = replaceAll(hivePath, L"\\", L"|");
	return (std::wstring(gc_loadKeyNamePrefix) + std::to_wstring(m_mountedHiveCount) +
		L" [" + pathPrepped + L"]");
	/*return (std::wstring(gc_loadKeyNamePrefix) + std::to_wstring(m_mountedHiveCount) +
		L" / " + hivePath);*/
}

inline HKEY RegHandler::pickLoadTarget(const RegLoadTarget loadTarget) const {
	if (RegLoadTarget::LocalMachine == loadTarget) {
		return HKEY_LOCAL_MACHINE;
	} else if (RegLoadTarget::Users == loadTarget || RegLoadTarget::None == loadTarget) {
		return HKEY_USERS;
	} else {
		return 0;
	}
}

HKEY RegHandler::pickUnloadKey(const std::wstring loadTarget) const {
	for (auto &it : m_mountedHives) {
		if(lower_copy(it.first) == lower_copy(loadTarget)) {
			return it.second;
		}
	}
	return 0;
}

RegLoadTarget RegHandler::pickUnloadKey(const HKEY tgtHKEY) const {
	if (tgtHKEY == HKEY_LOCAL_MACHINE) {
		return RegLoadTarget::LocalMachine;
	} else if (tgtHKEY == HKEY_USERS) {
		return RegLoadTarget::Users;
	} else {
		return RegLoadTarget::None;
	}
}

std::wstring RegHandler::pickMenuModNum(const std::wstring subMenuKey, const HKEY *root) const {
	std::wstring keyname, iconv;
	unsigned char* buf = 0;
	unsigned long bufSz = 0;
	for (size_t i = 1; i < gc_maxMenuItems; ++i) {
		iconv = std::to_wstring(i);
		keyname = L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\"
			+ subMenuKey + iconv;
		SecDesc sd;
		if (RegOpResult::Success != GetKeySecurity(keyname, sd, root)) {
			if (buf) {
				SAFE_ARR_DELETE(buf);
			}
			return iconv;
		}
	}
	if (buf) {
		SAFE_ARR_DELETE(buf);
	}
	return std::to_wstring(0);
}

inline std::wstring RegHandler::hkey2wstr(const HKEY key) const {
	if (key == HKEY_CURRENT_USER) {
		return L"HKEY_CURRENT_USER\\";
	} else if (key == HKEY_CURRENT_USER_LOCAL_SETTINGS) {
		return L"HKEY_CURRENT_USER_LOCAL_SETTINGS\\";
	} else if (key == HKEY_LOCAL_MACHINE) {
		return L"HKEY_LOCAL_MACHINE\\";
	} else if (key == HKEY_CLASSES_ROOT) {
		return L"HKEY_CLASSES_ROOT\\";
	} else if (key == HKEY_USERS) {
		return L"HKEY_USERS\\";
	} else if (key == HKEY_CURRENT_CONFIG) {
		return L"HKEY_CURRENT_CONFIG\\";
	} else if (key == HKEY_PERFORMANCE_NLSTEXT) {
		return L"HKEY_PERFORMANCE_NLSTEXT\\";
	} else if (key == HKEY_PERFORMANCE_TEXT) {
		return L"HKEY_PERFORMANCE_TEXT\\";
	} else if (key == HKEY_PERFORMANCE_DATA) {
		return L"HKEY_PERFORMANCE_DATA\\";
	} else if (key == HKEY_DYN_DATA) {
		return L"HKEY_DYN_DATA\\";
	} else {
		return L"";
	}
}

RegOpResult RegHandler::prepHKEYKeyPath(const HKEY &keyHandleSet, const std::wstring& keyPathSet,
	HKEY &keyHandle, std::wstring &keyPath) const {
	return assignHKEYKeyPath(keyPathSet, keyHandleSet, keyHandle, keyPath);
}

RegOpResult RegHandler::prepHKEYKeyPathValueName(HKEY &keyHandle, const HKEY &keyHandleSet, const std::wstring &valPath,
	std::wstring &keyPath, std::wstring &valName) const {
	bool spiltTrad = false;
	std::vector<std::wstring> strSpl;
	valName = removeFromStart_copy(valPath, L"\\");
	HKEY hktest = { 0 }, hkres = { 0 };
	std::vector<std::wstring> keyPathSpl = splitStr(valName, L"\\");
	std::wstring rootlow = lower_copy(keyPathSpl[0]);
	std::wstring keyPathSplStr = joinStrs(keyPathSpl, L"\\", 1);
	/*if (std::string::npos != valName.find(L"|")) {
		std::vector<std::wstring> keyPathSplProp = splitStr(valName, L"|");
		std::wstring keyPathProp = keyPathSplProp[0];
		valName = keyPathSplProp[1];
		keyPathSpl = splitStr(keyPathProp, L"\\");
		rootlow = lower_copy(keyPathSpl[0]);
		keyPathSplStr = joinStrs(keyPathSpl, L"\\", 1);
	} else {
	}*/
	std::vector<std::wstring> strSplTest = keyPathSpl;
	if (strSplTest.size() > 1) {
		strSplTest.erase(strSplTest.end() - 1);
	}
	if (keyHandleSet) {
		hktest = keyHandleSet;
	} else {
		if (L"hkey_local_machine" == rootlow || L"hklm" == rootlow) {
			hktest = HKEY_LOCAL_MACHINE;
			strSplTest.erase(strSplTest.begin());
		} else if (L"hkey_current_user" == rootlow || L"hkcu" == rootlow) {
			hktest = HKEY_CURRENT_USER;
			strSplTest.erase(strSplTest.begin());
		} else if (L"hkey_current_user_local_settings" == rootlow || L"hkculs" == rootlow) {
			hktest = HKEY_CURRENT_USER_LOCAL_SETTINGS;
			strSplTest.erase(strSplTest.begin());
		} else if (L"hkey_classes_root" == rootlow || L"hkcr" == rootlow) {
			hktest = HKEY_CLASSES_ROOT;
			strSplTest.erase(strSplTest.begin());
		} else if (L"hkey_current_config" == rootlow || L"hkcc" == rootlow) {
			hktest = HKEY_CURRENT_CONFIG;
			strSplTest.erase(strSplTest.begin());
		} else if (L"hkey_users" == rootlow || L"hku" == rootlow) {
			hktest = HKEY_USERS;
			strSplTest.erase(strSplTest.begin());
		} else if (L"hkey_dyn_data" == rootlow || L"hkdd" == rootlow) {
			hktest = HKEY_DYN_DATA;
			strSplTest.erase(strSplTest.begin());
		} else if (L"hkey_performance_data" == rootlow || L"hkpd" == rootlow) {
			hktest = HKEY_PERFORMANCE_DATA;
			strSplTest.erase(strSplTest.begin());
		} else if (L"hkey_performance_text" == rootlow || L"hkpt" == rootlow) {
			hktest = HKEY_PERFORMANCE_TEXT;
			strSplTest.erase(strSplTest.begin());
		} else if (L"hkey_performance_nlstext" == rootlow || L"hkpnlt" == rootlow) {
			hktest = HKEY_PERFORMANCE_NLSTEXT;
			strSplTest.erase(strSplTest.begin());
		} else {
			return RegOpResult::Fail;
		}
	}
	unsigned long res = 0;
	std::wstring tstr, tvalname;
	do {
		tstr = joinStrs(strSplTest, L"\\");
		res = ::RegOpenKeyEx(hktest, tstr.c_str(), 0, KEY_READ | getRightMod(), &hkres);
		if (ERROR_SUCCESS == res) {
			tvalname = removeFromStart_copy(removeFromStart_copy(valPath, rootlow + L"\\" + tstr), L"\\");
			if (ERROR_SUCCESS == ::RegQueryValueEx(hkres, tvalname.c_str(), 0, 0, 0, 0)) {
				keyHandle = hktest;
				keyPath = tstr;
				valName = tvalname;
				CLOSEKEY_NULLIFY(hkres);
				return RegOpResult::Success;
			} else {
				if (std::wstring::npos != tvalname.find(L"|")) {
					std::vector<std::wstring> tvalnameSpl = splitStr(tvalname, L"|");
					if (ERROR_SUCCESS == ::RegQueryValueEx(hkres, tvalnameSpl.front().c_str(), 0, 0, 0, 0)) {
						keyHandle = hktest;
						keyPath = tstr;
						valName = tvalnameSpl.front();
						CLOSEKEY_NULLIFY(hkres);
						return RegOpResult::Success;
					} else {
						if (ERROR_SUCCESS == ::RegQueryValueEx(hkres, tvalname.c_str(), 0, 0, 0, 0)) {
							keyHandle = hktest;
							keyPath = tstr;
							valName = tvalname;
							CLOSEKEY_NULLIFY(hkres);
							return RegOpResult::Success;
						}
					}
				} else {
					return RegOpResult::Fail;
				}
			}
		} else {
			if (hkres) {
				CLOSEKEY_NULLIFY(hkres);
			}
		}
		strSplTest.pop_back();
	} while (strSplTest.size() > 1);
	keyPath = tstr;
	valName = removeFromBothSides_copy(removeFromStart_copy(keyPathSplStr, tstr), L"\\");
	return RegOpResult::Success;
}

RegOpResult RegHandler::assignHKEYKeyPath(const std::wstring keyPath, const HKEY &hkeySet,
	HKEY &tgtHKEY, std::wstring &tgtKeyPath) const {
	std::wstring keyPathPrepped = removeFromBothSides_copy(keyPath, L"\\");
	std::vector<std::wstring> keyPathPreppedSpl = splitStr(keyPathPrepped, L"\\");
	std::wstring keyPathPreppedSplStr = joinStrs(keyPathPreppedSpl, L"\\", 1);
	std::wstring rootlow = lower_copy(keyPathPreppedSpl[0]);
	if (hkeySet) {
		tgtHKEY = hkeySet;
		if (std::wstring::npos != keyPath.find(L"\\")) {
			tgtKeyPath = joinStrs(keyPathPreppedSpl, L"\\", 1);
		} else {
			tgtKeyPath = keyPathPrepped;
		}
		return RegOpResult::Success;
	}
	if (std::wstring::npos != keyPath.find(L"\\")) {
		std::wstring keyPathPrepped = removeFromEnd_copy(removeFromStart_copy(keyPath, L"\\"), L"\\");
		std::vector<std::wstring> keyPathPreppedSpl = splitStr(keyPathPrepped, L"\\");
		rootlow = lower_copy(keyPathPreppedSpl[0]);
	} else {
		rootlow = lower_copy(keyPath);
		// rootlow = lower_copy(splitStr(removeFromStart_copy(keyPath, L"\\"), L"\\")[0]);
	}
	if (L"hkey_current_user" == rootlow || L"hkcu" == rootlow) {
		tgtKeyPath = keyPathPreppedSplStr;
		tgtHKEY = HKEY_CURRENT_USER;
	} else if (L"hkey_current_user_local_settings" == rootlow || L"hkculs" == rootlow) {
		tgtKeyPath = keyPathPreppedSplStr;
		tgtHKEY = HKEY_CURRENT_USER_LOCAL_SETTINGS;
	} else if (L"hkey_classes_root" == rootlow || L"hkcr" == rootlow) {
		tgtKeyPath = keyPathPreppedSplStr;
		tgtHKEY = HKEY_CLASSES_ROOT;
	} else if (L"hkey_local_machine" == rootlow || L"hklm" == rootlow) {
		tgtKeyPath = keyPathPreppedSplStr;
		tgtHKEY = HKEY_LOCAL_MACHINE;
	} else if (L"hkey_users" == rootlow || L"hku" == rootlow) {
		tgtKeyPath = keyPathPreppedSplStr;
		tgtHKEY = HKEY_USERS;
	} else if (L"hkey_current_config" == rootlow || L"hkcc" == rootlow) {
		tgtKeyPath = keyPathPreppedSplStr;
		tgtHKEY = HKEY_CURRENT_CONFIG;
	} else if (L"hkey_dyn_data" == rootlow || L"hkdd" == rootlow) {
		tgtKeyPath = keyPathPreppedSplStr;
		tgtHKEY = HKEY_DYN_DATA;
	} else if (L"hkey_performance_data" == rootlow || L"hkpd" == rootlow) {
		tgtKeyPath = keyPathPreppedSplStr;
		tgtHKEY = HKEY_PERFORMANCE_DATA;
	} else if (L"hkey_performance_text" == rootlow || L"hkpt" == rootlow) {
		tgtKeyPath = keyPathPreppedSplStr;
		tgtHKEY = HKEY_PERFORMANCE_TEXT;
	} else if (L"hkey_performance_nlstext" == rootlow || L"hkpnlt" == rootlow) {
		tgtKeyPath = keyPathPreppedSplStr;
		tgtHKEY = HKEY_PERFORMANCE_NLSTEXT;
	} else {
		if (hkeySet) {
			tgtHKEY = hkeySet;
		} else {
			return RegOpResult::Fail;
		}
	}
	return RegOpResult::Success;
}