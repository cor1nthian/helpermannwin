#include <iostream>
#include "ntapi.h"

UnicodeString::UnicodeString() {
	Length = 0;
	MaximumLength = 0;
	Buffer = 0;
}

UnicodeString::UnicodeString(const unsigned short length, const unsigned short maxlength) {
	Length = length;
	MaximumLength = maxlength;
	Buffer = (wchar_t*)malloc(Length * sizeof(wchar_t));
	// Buffer = (wchar_t*)::LocalAlloc(LPTR, Length);
	// NEW_ARR_NULLIFY_NO_REDEFINE(Buffer, wchar_t, Length);
}

UnicodeString::UnicodeString(const unsigned short length, const unsigned short maxlength, const wchar_t* buffer) {
	Length = length;
	MaximumLength = maxlength;
	Buffer = (wchar_t*)malloc(Length * sizeof(wchar_t));
	// Buffer = (wchar_t*)::LocalAlloc(LPTR, Length);
	// NEW_ARR_NULLIFY_NO_REDEFINE(Buffer, wchar_t, Length);
	if (Buffer) {
		memset(Buffer, 0, Length * sizeof(wchar_t));
		wsprintf(Buffer, L"%s", buffer);
	}
}

UnicodeString::UnicodeString(const UnicodeString &other) {
	if (this != &other) {
		Length = other.Length;
		MaximumLength = other.MaximumLength;
		if (Buffer) {
			SAFE_FREE(Buffer);
		}
		if (other.Buffer) {
			Buffer = (wchar_t*)malloc(MaximumLength * sizeof(char));
			if (Buffer) {
				memset(Buffer, 0, MaximumLength * sizeof(wchar_t));
				wsprintf(Buffer, L"%s", other.Buffer);
			}
		}
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
UnicodeString::UnicodeString(UnicodeString &&other) noexcept {
	if (this != &other) {
		Length = std::exchange(other.Length, 0);
		MaximumLength = std::exchange(other.MaximumLength, 0);
		Buffer = std::move(other.Buffer);
		other.Buffer = 0;
	}
}
#endif

UnicodeString::~UnicodeString() {
	/*if (Buffer) {
		SAFE_FREE(Buffer);
	}*/
}

UnicodeString& UnicodeString::operator=(const UnicodeString &other) {
	if (this != &other) {
		Length = other.Length;
		MaximumLength = other.MaximumLength;
		if (Buffer) {
			SAFE_FREE(Buffer);
		}
		if (other.Buffer) {
			Buffer = (wchar_t*)malloc(MaximumLength * sizeof(char));
			if (Buffer) {
				memset(Buffer, 0, MaximumLength * sizeof(wchar_t));
				wsprintf(Buffer, L"%s", other.Buffer);
			}
		}
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
UnicodeString& UnicodeString::operator=(UnicodeString &&other) noexcept {
	if (this != &other) {
		Length = std::exchange(other.Length, 0);
		MaximumLength = std::exchange(other.MaximumLength, 0);
		Buffer = std::move(other.Buffer);
		other.Buffer = 0;
	}
	return *this;
}
#endif

bool UnicodeString::operator==(const UnicodeString &other) const {
	if (this != &other) {
		return (Length == other.Length &&
			MaximumLength == other.MaximumLength &&
			!memcmp(Buffer, other.Buffer, Length * sizeof(wchar_t)));
	} else {
		return true;
	}
}

bool UnicodeString::operator!=(const UnicodeString &other) const {
	if (this != &other) {
		return (Length != other.Length ||
			MaximumLength != other.MaximumLength ||
			memcmp(Buffer, other.Buffer, Length * sizeof(wchar_t)));
	} else {
		return false;
	}
}

ANSIString::ANSIString() {
	Length = 0;
	MaximumLength = 0;
	Buffer = 0;
}

ANSIString::ANSIString(const unsigned short length, const unsigned short maxlength) {
	Length = length;
	MaximumLength = maxlength;
	Buffer = (char*)malloc(Length * sizeof(char));
	if (Buffer) {
		memset(Buffer, 0, Length * sizeof(wchar_t));
	}
	// NEW_ARR_NULLIFY_NO_REDEFINE(Buffer, char, Length);
}

ANSIString::ANSIString(const unsigned short length, const unsigned short maxlength, const char* buffer) {
	Length = length;
	MaximumLength = maxlength;
	Buffer = (char*)malloc(Length * sizeof(char));
	// NEW_ARR_NULLIFY_NO_REDEFINE(Buffer, char, Length);
	if (Buffer) {
		memset(Buffer, 0, Length * sizeof(wchar_t));
		sprintf(Buffer, "%s", buffer);
	}
}

ANSIString::ANSIString(const ANSIString &other) {
	if (this != &other) {
		Length = other.Length;
		MaximumLength = other.MaximumLength;
		if (Buffer) {
			SAFE_FREE(Buffer);
		}
		if (other.Buffer) {
			Buffer = (char*)malloc(MaximumLength * sizeof(char));
			if (Buffer) {
				memset(Buffer, 0, MaximumLength * sizeof(wchar_t));
				sprintf(Buffer, "%s", other.Buffer);
			}
		}
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
ANSIString::ANSIString(ANSIString &&other) noexcept {
	if (this != &other) {
		Length = std::exchange(other.Length, 0);
		MaximumLength = std::exchange(other.MaximumLength, 0);
		Buffer = std::move(other.Buffer);
		other.Buffer = 0;
	}
}
#endif

ANSIString::~ANSIString() {
	/*if (Buffer) {
		SAFE_FREE(Buffer);
	}*/
}

ANSIString& ANSIString::operator=(const ANSIString &other) {
	if (this != &other) {
		Length = other.Length;
		MaximumLength = other.MaximumLength;
		if (Buffer) {
			SAFE_FREE(Buffer);
		}
		if (other.Buffer) {
			Buffer = (char*)malloc(MaximumLength * sizeof(char));
			if (Buffer) {
				memset(Buffer, 0, MaximumLength * sizeof(wchar_t));
				sprintf(Buffer, "%s", other.Buffer);
			}
		}
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
ANSIString& ANSIString::operator=(ANSIString &&other) noexcept {
	if (this != &other) {
		Length = std::exchange(other.Length, 0);
		MaximumLength = std::exchange(other.MaximumLength, 0);
		Buffer = std::move(other.Buffer);
		other.Buffer = 0;
	}
	return *this;
}
#endif

bool ANSIString::operator==(const ANSIString &other) const {
	if (this != &other) {
		return (Length == other.Length &&
			MaximumLength == other.MaximumLength &&
			!memcmp(Buffer, other.Buffer, Length * sizeof(char)));
	} else {
		return true;
	}
}

bool ANSIString::operator!=(const ANSIString &other) const {
	if (this != &other) {
		return (Length != other.Length ||
			MaximumLength != other.MaximumLength ||
			memcmp(Buffer, other.Buffer, Length * sizeof(char)));
	} else {
		return false;
	}
}

ObjectAttributes::ObjectAttributes() {
	uLength = 0;
	uAttributes = 0;
	pSecurityDescriptor = 0;
	pSecurityQualityOfService = 0;
	hRootDirectory = 0;
	pObjectName = 0;
}

ObjectAttributes::ObjectAttributes(const ObjectAttributes &other) {
	if (this != &other) {
		uLength = other.uLength;
		uAttributes = other.uAttributes;
		if (other.pSecurityDescriptor) {
			if (pSecurityDescriptor) {
				SAFE_FREE(pSecurityDescriptor);
			}
			pSecurityDescriptor = malloc(SECURITY_DESCRIPTOR_MIN_LENGTH);
			if (pSecurityDescriptor) {
				memcpy(pSecurityDescriptor, other.pSecurityDescriptor, sizeof(SECURITY_DESCRIPTOR));
			}
		} else {
			if (pSecurityDescriptor) {
				SAFE_FREE(pSecurityDescriptor);
			}
		}
		pSecurityQualityOfService = other.pSecurityQualityOfService;
		hRootDirectory = other.hRootDirectory;
		if (other.pObjectName) {
			if (pObjectName) {
				SAFE_DELETE(pObjectName);
			}
			pObjectName = new UnicodeString;
			if (pObjectName) {
				memcpy(pObjectName, other.pObjectName, sizeof(UnicodeString));
			}
		} else {
			if (pObjectName) {
				SAFE_DELETE(pObjectName);
			}
		}
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
ObjectAttributes::ObjectAttributes(ObjectAttributes &&other) noexcept {
	if (this != &other) {
		uLength = std::exchange(other.uLength, 0);
		uAttributes = std::exchange(other.uAttributes, 0);
		hRootDirectory = std::move(other.hRootDirectory);
		other.hRootDirectory = 0;
		pSecurityDescriptor = std::move(other.pSecurityDescriptor);
		other.pSecurityDescriptor = 0;
		pSecurityQualityOfService = std::move(other.pSecurityQualityOfService);
		other.pSecurityQualityOfService = 0;
		pObjectName = std::move(other.pObjectName);
		other.pObjectName = 0;
	}
}
#endif

ObjectAttributes::~ObjectAttributes() {
	/*if (pObjectName) {
		SAFE_DELETE(pObjectName);
	}*/
}

ObjectAttributes& ObjectAttributes::operator=(const ObjectAttributes &other) {
	if (this != &other) {
		uLength = other.uLength;
		uAttributes = other.uAttributes;
		if (other.pSecurityDescriptor) {
			if (pSecurityDescriptor) {
				SAFE_FREE(pSecurityDescriptor);
			}
			pSecurityDescriptor = malloc(SECURITY_DESCRIPTOR_MIN_LENGTH);
			if (pSecurityDescriptor) {
				memcpy(pSecurityDescriptor, other.pSecurityDescriptor, sizeof(SECURITY_DESCRIPTOR));
			}
		} else {
			if (pSecurityDescriptor) {
				SAFE_FREE(pSecurityDescriptor);
			}
		}
		pSecurityQualityOfService = other.pSecurityQualityOfService;
		hRootDirectory = other.hRootDirectory;
		if (other.pObjectName) {
			if (pObjectName) {
				SAFE_DELETE(pObjectName);
			}
			pObjectName = new UnicodeString;
			if (pObjectName) {
				memcpy(pObjectName, other.pObjectName, sizeof(UnicodeString));
			}
		} else {
			if (pObjectName) {
				SAFE_DELETE(pObjectName);
			}
		}
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
ObjectAttributes& ObjectAttributes::operator=(ObjectAttributes &&other) noexcept {
	if (this != &other) {
		uLength = std::exchange(other.uLength, 0);
		uAttributes = std::exchange(other.uAttributes, 0);
		hRootDirectory = std::move(other.hRootDirectory);
		other.hRootDirectory = 0;
		pSecurityDescriptor = std::move(other.pSecurityDescriptor);
		other.pSecurityDescriptor = 0;
		pSecurityQualityOfService = std::move(other.pSecurityQualityOfService);
		other.pSecurityQualityOfService = 0;
		pObjectName = std::move(other.pObjectName);
		other.pObjectName = 0;
	}
	return *this;
}
#endif

bool ObjectAttributes::operator==(const ObjectAttributes &other) const {
	if (this != &other) {
		return (uLength == other.uLength &&
			uAttributes == other.uAttributes &&
			hRootDirectory == other.hRootDirectory &&
			// pSecurityQualityOfService == other.pSecurityQualityOfService &&
			!memcmp(pSecurityDescriptor, other.pSecurityDescriptor, sizeof(SECURITY_DESCRIPTOR)) &&
			!memcmp(pObjectName, other.pObjectName, sizeof(UnicodeString)));
	} else {
		return true;
	}
}

bool ObjectAttributes::operator!=(const ObjectAttributes &other) const {
	if (this != &other) {
		return (uLength != other.uLength ||
			uAttributes != other.uAttributes ||
			hRootDirectory != other.hRootDirectory ||
			// pSecurityQualityOfService != other.pSecurityQualityOfService ||
			memcmp(pSecurityDescriptor, other.pSecurityDescriptor, sizeof(SECURITY_DESCRIPTOR)) ||
			memcmp(pObjectName, other.pObjectName, sizeof(UnicodeString)));
	} else {
		return false;
	}
}

IOStatusBlock::IOStatusBlock() {
	Status = 0;
	Pointer = 0;
	Information = 0;
}

IOStatusBlock::IOStatusBlock(const NTSTATUS status, const unsigned long long info) {
	Status = status;
	Information = info;
	Pointer = 0;
}

IOStatusBlock::IOStatusBlock(const IOStatusBlock &other) {
	if (this != &other) {
		Status = other.Status;
		Pointer = other.Pointer;
		Information = other.Information;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
IOStatusBlock::IOStatusBlock(IOStatusBlock &&other) noexcept {
	if (this != &other) {
		Status = std::exchange(other.Status, 0);
		Information = std::exchange(other.Information, 0);
		Pointer = std::move(other.Pointer);
		other.Pointer = 0;
	}
}
#endif

IOStatusBlock::~IOStatusBlock() {}

IOStatusBlock& IOStatusBlock::operator=(const IOStatusBlock &other) {
	if (this != &other) {
		Status = other.Status;
		Pointer = other.Pointer;
		Information = other.Information;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
IOStatusBlock& IOStatusBlock::operator=(IOStatusBlock &&other) {
	if (this != &other) {
		Status = std::exchange(other.Status, 0);
		Information = std::exchange(other.Information, 0);
		Pointer = std::move(other.Pointer);
		other.Pointer = 0;
	}
	return *this;
}
#endif

bool IOStatusBlock::operator==(const IOStatusBlock &other) const {
	if (this != &other) {
		return (Status == other.Status &&
			Information == other.Information);
	} else {
		return true;
	}
}

bool IOStatusBlock::operator!=(const IOStatusBlock &other) const {
	if (this != &other) {
		return (Status != other.Status ||
			Information != other.Information);
	} else {
		return false;
	}
}

FileDirBothInformation::FileDirBothInformation() {
	NextEntryOffset = 0;
	FileIndex = 0;
	FileAttributes = 0;
	FileNameLength = 0;
	EaSize = 0;
	ShortNameLength = 0;
	CreationTime = { 0 };
	LastAccessTime = { 0 };
	LastWriteTime = { 0 };
	ChangeTime = { 0 };
	EndOfFile = { 0 };
	AllocationSize = { 0 };
	memset(ShortName, 0, FSH_SHORTNAMELENGTH * sizeof(wchar_t));
	memset(FileName, 0, sizeof(wchar_t));
}

FileDirBothInformation::FileDirBothInformation(const FileDirBothInformation &other) {
	if (this != &other) {
		NextEntryOffset = other.NextEntryOffset;
		FileIndex = other.FileIndex;
		FileAttributes = other.FileAttributes;
		FileNameLength = other.FileNameLength;
		EaSize = other.EaSize;
		ShortNameLength = other.ShortNameLength;
		CreationTime.LowPart = other.CreationTime.LowPart;
		CreationTime.HighPart = other.CreationTime.HighPart;
		LastAccessTime.LowPart = other.LastAccessTime.LowPart;
		LastAccessTime.HighPart = other.LastAccessTime.HighPart;
		LastWriteTime.LowPart = other.LastWriteTime.LowPart;
		LastWriteTime.HighPart = other.LastWriteTime.HighPart;
		ChangeTime.LowPart = other.ChangeTime.LowPart;
		ChangeTime.HighPart = other.ChangeTime.HighPart;
		EndOfFile.LowPart = other.EndOfFile.LowPart;
		EndOfFile.HighPart = other.EndOfFile.HighPart;
		AllocationSize.LowPart = other.AllocationSize.LowPart;
		AllocationSize.HighPart = other.AllocationSize.HighPart;
		wsprintf(ShortName, L"%s", other.ShortName);
		wsprintf(FileName, L"%s", other.FileName);
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
FileDirBothInformation::FileDirBothInformation(FileDirBothInformation &&other) noexcept {
	if (this != &other) {
		NextEntryOffset = std::exchange(other.NextEntryOffset, 0);
		FileIndex = std::exchange(other.FileIndex, 0);
		FileAttributes = std::exchange(other.FileAttributes, 0);
		FileNameLength = std::exchange(other.FileNameLength, 0);
		EaSize = std::exchange(other.EaSize, 0);
		ShortNameLength = std::exchange(other.ShortNameLength, 0);
		CreationTime.LowPart = std::exchange(other.CreationTime.LowPart, 0);
		CreationTime.HighPart = std::exchange(other.CreationTime.HighPart, 0);
		LastAccessTime.LowPart = std::exchange(other.LastAccessTime.LowPart, 0);
		LastAccessTime.HighPart = std::exchange(other.LastAccessTime.HighPart, 0);
		LastWriteTime.LowPart = std::exchange(other.LastWriteTime.LowPart, 0);
		LastWriteTime.HighPart = std::exchange(other.LastWriteTime.HighPart, 0);
		ChangeTime.LowPart = std::exchange(other.ChangeTime.LowPart, 0);
		ChangeTime.HighPart = std::exchange(other.ChangeTime.HighPart, 0);
		EndOfFile.LowPart = std::exchange(other.EndOfFile.LowPart, 0);
		EndOfFile.HighPart = std::exchange(other.EndOfFile.HighPart, 0);
		AllocationSize.LowPart = std::exchange(other.AllocationSize.LowPart, 0);
		AllocationSize.HighPart = std::exchange(other.AllocationSize.HighPart, 0);
		wsprintf(ShortName, L"%s", other.ShortName);
		memset(other.ShortName, 0, FSH_SHORTNAMELENGTH * sizeof(wchar_t));
		wsprintf(FileName, L"%s", other.FileName);
		memset(other.FileName, 0, sizeof(wchar_t));
	}
}
#endif

FileDirBothInformation::~FileDirBothInformation() {}

FileDirBothInformation& FileDirBothInformation::operator=(const FileDirBothInformation &other) {
	if (this != &other) {
		NextEntryOffset = other.NextEntryOffset;
		FileIndex = other.FileIndex;
		FileAttributes = other.FileAttributes;
		FileNameLength = other.FileNameLength;
		EaSize = other.EaSize;
		ShortNameLength = other.ShortNameLength;
		CreationTime.LowPart = other.CreationTime.LowPart;
		CreationTime.HighPart = other.CreationTime.HighPart;
		LastAccessTime.LowPart = other.LastAccessTime.LowPart;
		LastAccessTime.HighPart = other.LastAccessTime.HighPart;
		LastWriteTime.LowPart = other.LastWriteTime.LowPart;
		LastWriteTime.HighPart = other.LastWriteTime.HighPart;
		ChangeTime.LowPart = other.ChangeTime.LowPart;
		ChangeTime.HighPart = other.ChangeTime.HighPart;
		EndOfFile.LowPart = other.EndOfFile.LowPart;
		EndOfFile.HighPart = other.EndOfFile.HighPart;
		AllocationSize.LowPart = other.AllocationSize.LowPart;
		AllocationSize.HighPart = other.AllocationSize.HighPart;
		wsprintf(ShortName, L"%s", other.ShortName);
		wsprintf(FileName, L"%s", other.FileName);
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
FileDirBothInformation& FileDirBothInformation::operator=(FileDirBothInformation &&other) noexcept {
	if (this != &other) {
		NextEntryOffset = std::exchange(other.NextEntryOffset, 0);
		FileIndex = std::exchange(other.FileIndex, 0);
		FileAttributes = std::exchange(other.FileAttributes, 0);
		FileNameLength = std::exchange(other.FileNameLength, 0);
		EaSize = std::exchange(other.EaSize, 0);
		ShortNameLength = std::exchange(other.ShortNameLength, 0);
		CreationTime.LowPart = std::exchange(other.CreationTime.LowPart, 0);
		CreationTime.HighPart = std::exchange(other.CreationTime.HighPart, 0);
		LastAccessTime.LowPart = std::exchange(other.LastAccessTime.LowPart, 0);
		LastAccessTime.HighPart = std::exchange(other.LastAccessTime.HighPart, 0);
		LastWriteTime.LowPart = std::exchange(other.LastWriteTime.LowPart, 0);
		LastWriteTime.HighPart = std::exchange(other.LastWriteTime.HighPart, 0);
		ChangeTime.LowPart = std::exchange(other.ChangeTime.LowPart, 0);
		ChangeTime.HighPart = std::exchange(other.ChangeTime.HighPart, 0);
		EndOfFile.LowPart = std::exchange(other.EndOfFile.LowPart, 0);
		EndOfFile.HighPart = std::exchange(other.EndOfFile.HighPart, 0);
		AllocationSize.LowPart = std::exchange(other.AllocationSize.LowPart, 0);
		AllocationSize.HighPart = std::exchange(other.AllocationSize.HighPart, 0);
		wsprintf(ShortName, L"%s", other.ShortName);
		memset(other.ShortName, 0, FSH_SHORTNAMELENGTH * sizeof(wchar_t));
		wsprintf(FileName, L"%s", other.FileName);
		memset(other.FileName, 0, sizeof(wchar_t));
	}
	return *this;
}
#endif

bool FileDirBothInformation::operator==(const FileDirBothInformation &other) const {
	if (this != &other) {
		return (NextEntryOffset == other.NextEntryOffset &&
			FileIndex == other.FileIndex &&
			FileAttributes == other.FileAttributes &&
			FileNameLength == other.FileNameLength &&
			EaSize == other.EaSize &&
			ShortNameLength == other.ShortNameLength &&
			CreationTime.QuadPart == other.CreationTime.QuadPart &&
			LastAccessTime.QuadPart == other.LastAccessTime.QuadPart &&
			LastWriteTime.QuadPart == other.LastWriteTime.QuadPart &&
			ChangeTime.QuadPart == other.ChangeTime.QuadPart &&
			EndOfFile.QuadPart == other.EndOfFile.QuadPart &&
			AllocationSize.QuadPart == other.AllocationSize.QuadPart &&
			!memcmp(ShortName, other.ShortName, FSH_SHORTNAMELENGTH * sizeof(wchar_t)) &&
			!memcmp(FileName, other.FileName, FileNameLength * sizeof(wchar_t)));
	} else {
		return true;
	}
}

bool FileDirBothInformation::operator!=(const FileDirBothInformation &other) const {
	if (this != &other) {
		return (NextEntryOffset != other.NextEntryOffset ||
			FileIndex != other.FileIndex ||
			FileAttributes != other.FileAttributes ||
			FileNameLength != other.FileNameLength ||
			EaSize != other.EaSize ||
			ShortNameLength != other.ShortNameLength ||
			CreationTime.QuadPart != other.CreationTime.QuadPart ||
			LastAccessTime.QuadPart != other.LastAccessTime.QuadPart ||
			LastWriteTime.QuadPart != other.LastWriteTime.QuadPart ||
			ChangeTime.QuadPart != other.ChangeTime.QuadPart ||
			EndOfFile.QuadPart != other.EndOfFile.QuadPart ||
			AllocationSize.QuadPart != other.AllocationSize.QuadPart ||
			!memcmp(ShortName, other.ShortName, FSH_SHORTNAMELENGTH * sizeof(wchar_t)) ||
			!memcmp(FileName, other.FileName, FileNameLength * sizeof(wchar_t)));
	} else {
		return false;
	}
}