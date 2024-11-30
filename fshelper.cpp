#include <stdio.h>
#include <iostream>
#include "fshelper.h"
#include "ntapi.h"
#include "crypt.h"

inline bool pathExists(const std::wstring path) {
	return (path.length() && (INVALID_FILE_ATTRIBUTES != ::GetFileAttributes(path.c_str())));
}

FSOpResult isFolder(bool &isFolder, const std::wstring path) {
	FSHandler fsh;
	return fsh.attrAnalyzer(isFolder, FILE_ATTRIBUTE_DIRECTORY, path);
}

FSOpResult isTemporary(bool &isTemporary, const std::wstring path) {
	FSHandler fsh;
	return fsh.attrAnalyzer(isTemporary, FILE_ATTRIBUTE_TEMPORARY, path);
}

FSOpResult isNormal(bool &isNormal, const std::wstring path) {
	FSHandler fsh;
	return fsh.attrAnalyzer(isNormal, FILE_ATTRIBUTE_NORMAL, path);
}

FSOpResult isArchive(bool &isArchive, const std::wstring path) {
	FSHandler fsh;
	return fsh.attrAnalyzer(isArchive, FILE_ATTRIBUTE_ARCHIVE, path);
}

FSOpResult isCompressed(bool &isCompressed, const std::wstring path) {
	FSHandler fsh;
	return fsh.attrAnalyzer(isCompressed, FILE_ATTRIBUTE_COMPRESSED, path);
}

FSOpResult isHidden(bool &isHidden, const std::wstring path) {
	FSHandler fsh;
	return fsh.attrAnalyzer(isHidden, FILE_ATTRIBUTE_HIDDEN, path);
}

FSOpResult isEncrypted(bool &isEncrypted, const std::wstring path) {
	FSHandler fsh;
	return fsh.attrAnalyzer(isEncrypted, FILE_ATTRIBUTE_ENCRYPTED, path);
}

FSOpResult isVirtual(bool& isVirtual, const std::wstring path) {
	FSHandler fsh;
	return fsh.attrAnalyzer(isVirtual, FILE_ATTRIBUTE_VIRTUAL, path);
}

FSOpResult isPathAvailable(bool &available, const std::wstring path) {
	available = false;
	std::string testcontent = "test line";
	std::wstring pathPrepped = removeFromEnd_copy(path, L"\\");
	std::wstring tfilename = pathPrepped + L"\\tmp" + genRandomString(L"") + L".tmp";
	FILE* testFile = fopen((wstr2str(tfilename)).c_str(), "w");
	if (testFile) {
		char writeQuantity = 1;
		size_t writtenBytes = fwrite(testcontent.c_str(), testcontent.length() * sizeof(char),
			writeQuantity, testFile);
		if (!writtenBytes || writtenBytes != writeQuantity) {
			fclose(testFile);
			return FSOpResult::Fail;
		}
		int opres = fclose(testFile);
		if (EOF == opres) {
			return FSOpResult::Fail;
		}
		if (::DeleteFile(tfilename.c_str())) {
			available = true;
			return FSOpResult::Success;
		} else {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult getFileSize(unsigned long long &fileSize, const std::wstring filePath) {
	if (pathExists(filePath)) {
		bool isfld = false;
		if (FSOpResult::Success == isFolder(isfld, filePath)) {
			if (isfld) {
				return FSOpResult::Fail;
			}
		} else {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
	::HANDLE hFile = ::CreateFile(filePath.c_str(), READ_CONTROL | FILE_READ_ATTRIBUTES, FILE_SHARE_READ, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile && INVALID_HANDLE_VALUE != hFile) {
		::LARGE_INTEGER li = { 0 };
		unsigned long fszhigh = 0;
		li.LowPart = ::GetFileSize(hFile, &fszhigh);
		li.HighPart = fszhigh;
		fileSize = li.QuadPart;
		::CloseHandle(hFile);
		hFile = 0;
		return FSOpResult::Success;
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult getFolderSize(unsigned long long &fileSize, const std::wstring filePath) {
	if (pathExists(filePath)) {
		bool isfld = false;
		if (FSOpResult::Success == isFolder(isfld, filePath)) {
			if (!isfld) {
				return FSOpResult::Fail;
			}
		} else {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
	FSHandler fsh;
	std::vector<FileRecord> folderFiles = fsh.SeekFileInDir(filePath, L".*", L"", true, false);
	for (size_t i = 0; i < folderFiles.size(); ++i) {
		fileSize += folderFiles[i].size;
	}
	return FSOpResult::Success;
}

FSOpResult getShortPath(std::wstring &shorPath, const std::wstring originalPath) {
	unsigned long pathLen = 0;
	wchar_t* shortPathBuf = (wchar_t*)malloc(sizeof(wchar_t));
	if (!shortPathBuf) {
		return FSOpResult::Fail;
	}
	pathLen = ::GetShortPathName(originalPath.c_str(), shortPathBuf, 1);
	if (!pathLen) {
		SAFE_FREE(shortPathBuf);
		return FSOpResult::Fail;
	}
	SAFE_FREE(shortPathBuf);
	++pathLen;
	shortPathBuf = (wchar_t*)malloc(pathLen * sizeof(wchar_t));
	if (!shortPathBuf) {
		return FSOpResult::Fail;
	}
	memset(shortPathBuf, 0, pathLen * sizeof(wchar_t));
	pathLen = ::GetShortPathName(originalPath.c_str(), shortPathBuf, pathLen);
	if (!pathLen) {
		SAFE_FREE(shortPathBuf);
		return FSOpResult::Fail;
	}
	shorPath = shortPathBuf;
	SAFE_FREE(shortPathBuf);
	return FSOpResult::Success;
}

/*	C open file modes:
	r - read
	w - write
	a - append
	r+ - read + wrie
	w+ - read + wrie
	a+ - read + append
	rb - binary read
	wb - binary write */
/*	File pos type	fpos_t
	Get file pos	fgetpos()
	Set file pos	fsetpos() */
FSOpResult buffer2File(const unsigned char* buffer, const std::wstring filePath, const size_t bufferSize,
	const FileType fileType, const size_t writeBufSz, const bool purgeObjectIfExists) {
	if (!buffer) {
		return FSOpResult::Fail;
	}
	if (purgeObjectIfExists) {
		if(pathExists(filePath)) {
			if (FSOpResult::Success != removeObject(filePath)) {
				return FSOpResult::Fail;
			}

		}
	}
	std::wstring fullPath;
	if (partialMatch(filePath, L"\\")) {
		std::vector<std::wstring> pathSpl = splitStr(filePath, L"\\");
		if (pathSpl.size() > 1) {
			std::wstring shortPath, corrPath = joinStrs(pathSpl, L"\\", 0, pathSpl.size() - 1);
			if (FSOpResult::Success != getShortPath(shortPath, corrPath)) {
				return FSOpResult::Fail;
			}
			fullPath = shortPath + L"\\" + pathSpl.back();
		} else {
			fullPath = pathSpl[0];
		}
	} else {
		fullPath = filePath;
	}
	if (FileType::WinAPI_Buffered == fileType) {
		if (!writeBufSz) {
			return FSOpResult::Fail;
		}
		unsigned long bytesWritten = 0, remainSz = 0;
		unsigned long long bufSz = (bufferSize <= writeBufSz * sizeof(unsigned char)) ?
			bufferSize * sizeof(unsigned char) : writeBufSz * sizeof(unsigned char), writeSz = 0, writePos = 0;
		unsigned char* writeBuf = (unsigned char*)malloc(bufSz);
		if (!writeBuf) {
			return FSOpResult::Fail;
		}
		memset(writeBuf, 0, bufSz);
		::HANDLE hFile = ::CreateFile(filePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile) {
			SAFE_FREE(writeBuf);
			return FSOpResult::Fail;
		}
		while (true) {
			writePos += remainSz;
			writeSz = abs(long long(bufferSize - writePos));
			writeSz >= bufSz ? remainSz = bufSz : remainSz = writeSz;
			if (INVALID_SET_FILE_POINTER == ::SetFilePointer(hFile, 0, 0, FILE_END)) {
				SAFE_FREE(writeBuf);
				return FSOpResult::Fail;
			}
			memset(writeBuf, 0, bufSz);
			memcpy(writeBuf, &buffer[writePos], remainSz);
			if (!remainSz || !::WriteFile(hFile, (void*)writeBuf, remainSz, &bytesWritten, 0)) {
				break;
			}
			if (!bytesWritten) {
				break;
			}			
		}
		SAFE_FREE(writeBuf);
		if (!::CloseHandle(hFile)) {
			return FSOpResult::Fail;
		} else {
			return FSOpResult::Success;
		}
	} else if (FileType::WinAPI_Unbuffered == fileType) {
		unsigned long bytesWritten = 0, remainSz = 0;
		::HANDLE hFile = ::CreateFile(filePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile) {
			return FSOpResult::Fail;
		}
		FSOpResult writeOpRec = FSOpResult::Success;
		if (bufferSize) {
			if (!::WriteFile(hFile, (void*)buffer, bufferSize, &bytesWritten, 0)) {
				return FSOpResult::Fail;
			}
		} else {
			return FSOpResult::Fail;
		}
		if (!bytesWritten) {
			writeOpRec = FSOpResult::Fail;
		}
		if (!::CloseHandle(hFile)) {
			return FSOpResult::Fail;
		} else {
			return writeOpRec;
		}
	} else if (FileType::C_Binary == fileType) {
		FILE* writePtr = fopen(wstr2str(fullPath).c_str(), "wb");
		if (writePtr) {
			char writeQuantity = 1;
			size_t writtenBytes = fwrite(buffer, bufferSize, writeQuantity, writePtr);
			if (!writtenBytes || writtenBytes != writeQuantity) {
				fclose(writePtr);
				return FSOpResult::Fail;
			}
			int opres = fclose(writePtr);
			if (EOF == opres) {
				return FSOpResult::Fail;
			} else {
				return FSOpResult::Success;
			}
			return FSOpResult::Success;
		} else {
			return FSOpResult::Fail;
		}
	} else if (FileType::C_Text == fileType) {
		FILE* writePtr = fopen(wstr2str(fullPath).c_str(), "w");
		if (writePtr) {
			char writeQuantity = 1;
			size_t writtenBytes = fwrite(buffer, bufferSize, writeQuantity, writePtr);
			if (!writtenBytes || writtenBytes != writeQuantity) {
				fclose(writePtr);
				return FSOpResult::Fail;
			}
			int opres = fclose(writePtr);
			if (EOF == opres) {
				return FSOpResult::Fail;
			} else {
				return FSOpResult::Success;
			}
			return FSOpResult::Success;
		} else {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
}

/*	C open file modes:
	r - read
	w - write
	a - append
	r+ - read + wrie
	w+ - read + wrie
	a+ - read + append
	rb - binary read
	wb - binary write */
/*	File pos type	fpos_t
	Get file pos	fgetpos()
	Set file pos	fsetpos() */
FSOpResult file2Buffer(unsigned char* &buffer, const std::wstring filePath, const size_t bufferSize,
	const FileType fileType, const size_t readBufSz) {
	if (pathExists(filePath)) {
		bool isFld = false;
		if (FSOpResult::Success == isFolder(isFld, filePath)) {
			if (isFld) {
				return FSOpResult::Fail;
			}
		} else {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
	unsigned long long fileSize = 0;
	if (FSOpResult::Success != getFileSize(fileSize, filePath)) {
		return FSOpResult::Fail;
	}
	if(bufferSize < fileSize) {
		return FSOpResult::Fail;
	}
	std::wstring shortPath;
	if (FSOpResult::Success != getShortPath(shortPath, filePath)) {
		return FSOpResult::Fail;
	}
	if (FileType::WinAPI_Buffered == fileType) {
		if (!readBufSz) {
			return FSOpResult::Fail;
		}
		unsigned long bytesRead = 0, remainSz = 0;
		unsigned long long bufSz = (fileSize <= readBufSz) ?
			fileSize * sizeof(unsigned char) : readBufSz * sizeof(unsigned char), readSz = 0, readPos = 0;
		unsigned char* readBuf = (unsigned char*)malloc(bufSz);
		if (!readBuf) {
			return FSOpResult::Fail;
		}
		memset(readBuf, 0, bufSz);
		::HANDLE hFile = ::CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile) {
			SAFE_FREE(readBuf);
			return FSOpResult::Fail;
		}
		while (true) {
			readSz = abs(long long(fileSize - readPos));
			readSz >= bufSz ? remainSz = bufSz : remainSz = readSz;
			if (!remainSz || !::ReadFile(hFile, readBuf, remainSz, &bytesRead, 0)) {
				break;
			}
			if (!bytesRead) {
				break;
			}
			memcpy(&buffer[readPos], readBuf, remainSz);
			memset(readBuf, 0, bufSz);
			readPos += remainSz;
		}
		SAFE_FREE(readBuf);
		if (!::CloseHandle(hFile)) {
			return FSOpResult::Fail;
		} else {
			return FSOpResult::Success;
		}
	} else if (FileType::WinAPI_Unbuffered == fileType) {
		memset(buffer, 0, bufferSize);
		unsigned long bytesRead = 0;
		::HANDLE hFile = ::CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile) {
			return FSOpResult::Fail;
		}
		if (!::ReadFile(hFile, buffer, fileSize, &bytesRead, 0)) {
			return FSOpResult::Fail;
		}
		if (!::CloseHandle(hFile)) {
			return FSOpResult::Fail;
		} else {
			return FSOpResult::Success;
		}
	} else if (FileType::C_Binary == fileType) {
		FILE* readPtr = fopen(wstr2str(shortPath).c_str(), "rb");
		if (readPtr) {
			char readQuantity = 1;
			size_t readBytes = fread(buffer, fileSize, readQuantity, readPtr);
			if (!readBytes || readBytes  != readQuantity) {
				fclose(readPtr);
				return FSOpResult::Fail;
			}
			int opres = fclose(readPtr);
			if (EOF == opres) {
				return FSOpResult::Fail;
			} else {
				return FSOpResult::Success;
			}
		} else {
			return FSOpResult::Fail;
		}
	} else if (FileType::C_Text == fileType) {
		FILE* readPtr = fopen(wstr2str(shortPath).c_str(), "r");
		if (readPtr) {
			char readQuantity = 1;
			size_t readBytes = fread(buffer, fileSize, readQuantity, readPtr);
			if (!readBytes || readBytes != readQuantity) {
				fclose(readPtr);
				return FSOpResult::Fail;
			}
			int opres = fclose(readPtr);
			if (EOF == opres) {
				return FSOpResult::Fail;
			} else {
				return FSOpResult::Success;
			}
		} else {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
	return FSOpResult::Success;
}

FSOpResult removeObject(const std::wstring objectPath) {
	if (pathExists(objectPath)) {
		bool isFld = false;
		if (FSOpResult::Success == isFolder(isFld, objectPath)) {
			FSHandler fsh;
			if (isFld) {
				if (FSOpResult::Success == fsh.RemoveFolder(objectPath, true)) {
					return FSOpResult::Success;
				} else {
					return FSOpResult::Fail;
				}
			} else {
				if (FSOpResult::Success == fsh.RemoveFile(objectPath)) {
					return FSOpResult::Success;
				} else {
					return FSOpResult::Fail;
				}
			}
		} else {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult setFileSize(std::wstring filePath, ::LARGE_INTEGER newSize) {
	if (!filePath.length() || !(pathExists(filePath))) {
		return FSOpResult::Fail;
	}
	bool isFld = false;
	if (FSOpResult::Success != isFolder(isFld, filePath)) {
		if (isFld) {
			return FSOpResult::Fail;
		}
	}
	::HANDLE hFile = ::CreateFile(filePath.c_str(), GENERIC_WRITE | FILE_APPEND_DATA, FILE_SHARE_READ, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(INVALID_HANDLE_VALUE == hFile) {
		return FSOpResult::Fail;
	}
	if (::SetFilePointerEx(hFile, newSize, 0, FILE_BEGIN)) {
		if (!::SetEndOfFile(hFile)) {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
	if(::CloseHandle(hFile)) {
		return FSOpResult::Success;
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult setFileSize(::HANDLE hFile, ::LARGE_INTEGER newSize, const bool closeHandle) {
	if(!hFile || INVALID_HANDLE_VALUE == hFile){
		return FSOpResult::Fail;
	}
	if (::SetFilePointerEx(hFile, newSize, 0, FILE_BEGIN)) {
		if (!::SetEndOfFile(hFile)) {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
	if (closeHandle) {
		if (::CloseHandle(hFile)) {
			return FSOpResult::Success;
		} else {
			return FSOpResult::Fail;
		}
	}
	return FSOpResult::Success;
}

BinData::BinData() {
	Platform = BinPlatform::PlatformUnknown;
	BitDepth = BinBitDepth::BitDepthUnknown;
}

BinData::BinData(const BinBitDepth bitDepth, const BinPlatform binPlatform) {
	BitDepth = bitDepth;
	Platform = binPlatform;
}

BinData::BinData(const BinData &other) {
	if (this != &other) {
		Platform = other.Platform;
		BitDepth = other.BitDepth;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
BinData::BinData(BinData&& other) noexcept {
	if (this != &other) {
		Platform = std::exchange(other.Platform, BinPlatform::PlatformUnknown);
		BitDepth = std::exchange(other.BitDepth, BinBitDepth::BitDepthUnknown);
	}
}
#endif

BinData::~BinData() {}

BinData& BinData::operator=(const BinData &other) {
	if (this != &other) {
		BitDepth = other.BitDepth;
		Platform = other.Platform;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
BinData& BinData::operator=(BinData &&other) noexcept {
	if (this != &other) {
		Platform = std::exchange(other.Platform, BinPlatform::PlatformUnknown);
		BitDepth = std::exchange(other.BitDepth, BinBitDepth::BitDepthUnknown);
	}
	return *this;
}
#endif

bool BinData::operator==(const BinData& other) const {
	if (this != &other) {
		return (BitDepth == other.BitDepth && Platform == other.Platform);
	} else {
		return true;
	}
}

bool BinData::operator!=(const BinData& other) const {
	if (this != &other) {
		return (BitDepth != other.BitDepth || Platform != other.Platform);
	} else {
		return false;
	}
}

FileRecord::FileRecord() {
	size = 0;
}

FileRecord::FileRecord(const FileRecord &other) {
	if (this != &other) {
		fileName = other.fileName;
		filePath = other.filePath;
		hash = other.hash;
		size = other.size;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
FileRecord::FileRecord(FileRecord &&other) noexcept {
	if (this != &other) {
		fileName = std::move(other.fileName);
		filePath = std::move(other.filePath);
		hash = std::move(other.hash);
		size = std::exchange(other.size, 0);
	}
}
#endif

FileRecord::~FileRecord() {}

FileRecord& FileRecord::operator=(const FileRecord &other) {
	if (this != &other) {
		fileName = other.fileName;
		filePath = other.filePath;
		hash = other.hash;
		size = other.size;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
FileRecord& FileRecord::operator=(FileRecord &&other) noexcept {
	if (this != &other) {
		fileName = std::move(other.fileName);
		filePath = std::move(other.filePath);
		hash = std::move(other.hash);
		size = std::exchange(other.size, 0);
	}
	return *this;
}
#endif

bool FileRecord::operator==(const FileRecord &other) const {
	if (this != &other) {
		return (lower_copy(hash) == lower_copy(other.hash) &&
				size == other.size &&
#if defined (_WIN32) || defined (_WIN64)
				lower_copy(filePath) == lower_copy(other.filePath));
#else
				filePath == other.filePath);
#endif
	} else {
		return true;
	}
}

bool FileRecord::operator!=(const FileRecord &other) const {
	if (this != &other) {
		return (lower_copy(hash) != lower_copy(other.hash) ||
				size != other.size ||
#if defined (_WIN32) || defined (_WIN64)
				lower_copy(filePath) != lower_copy(other.filePath));
#else
				filePath != other.filePath);
#endif
	} else {
		return false;
	}
}

FolderRecord::FolderRecord() {}

FolderRecord::FolderRecord(const FolderRecord &other) {
	if (this != &other) {
		folderName = other.folderName;
		folderPath = other.folderPath;
		files = other.files;
		folders = other.folders;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
FolderRecord::FolderRecord(FolderRecord &&other) noexcept {
	if (this != &other) {
		folderName = std::move(other.folderName);
		folderPath = std::move(other.folderPath);
		files = std::move(other.files);
		folders = std::move(other.folders);
	}
}
#endif

FolderRecord::~FolderRecord() {}

FolderRecord& FolderRecord::operator=(const FolderRecord &other) {
	if (this != &other) {
		folderName = other.folderName;
		folderPath = other.folderPath;
		files = other.files;
		folders = other.folders;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
FolderRecord& FolderRecord::operator=(const FolderRecord &&other) noexcept {
	if (this != &other) {
		folderName = std::move(other.folderName);
		folderPath = std::move(other.folderPath);
		files = std::move(other.files);
		folders = std::move(other.folders);
	}
	return *this;
}
#endif

bool FolderRecord::operator==(const FolderRecord &other) const {
	if (this != &other) {
		return (files == other.files &&
				folders == other.folders &&
#if defined (_WIN32) || defined (_WIN64)
				lower_copy(folderName) == lower_copy(other.folderName) &&
#else
				folderName == other.folderName) &&
#endif
#if defined (_WIN32) || defined (_WIN64)
				lower_copy(folderPath) == lower_copy(other.folderPath));
#else
				folderPath == other.folderPath);
#endif
	} else {
		return true;
	}
}

bool FolderRecord::operator!=(const FolderRecord &other) const {
	if (this != &other) {
		return (files != other.files ||
				folders != other.folders ||
#if defined (_WIN32) || defined (_WIN64)
				lower_copy(folderName) != lower_copy(other.folderName) ||
#else
				folderName == other.folderName) ||
#endif
#if defined (_WIN32) || defined (_WIN64)
				lower_copy(folderPath) != lower_copy(other.folderPath));
#else
				folderPath != other.folderPath);
#endif
	} else {
		return false;
	}
}

DriveDesc::DriveDesc() {
	spaceFree = 0;
	spaceTotal = 0;
}

DriveDesc::DriveDesc(const unsigned long long freespace, const unsigned long long totalspace,
	const std::wstring drivephyspath, const std::wstring drivepath) {
	spaceFree = freespace;
	spaceTotal = totalspace;
	drivePhysPath = drivephyspath;
	drivePath = drivepath;
}

DriveDesc::DriveDesc(const unsigned long long freespace, const unsigned long long totalspace, const std::wstring drivephyspath,
	const std::wstring drivepath, const std::wstring serialnumber) {
	spaceFree = freespace;
	spaceTotal = totalspace;
	drivePhysPath = drivephyspath;
	drivePath = drivepath;
	serialNumber = serialnumber;
}

DriveDesc::DriveDesc(const DriveDesc &other) {
	if (this != &other) {
		spaceFree = other.spaceFree;
		spaceTotal = other.spaceTotal;
		drivePhysPath = other.drivePhysPath;
		drivePath = other.drivePath;
		serialNumber = other.serialNumber;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
DriveDesc::DriveDesc(DriveDesc &&other) noexcept {
	if (this != &other) {
		spaceFree = std::exchange(other.spaceFree, 0);
		spaceTotal = std::exchange(other.spaceTotal, 0);
		drivePhysPath = std::move(other.drivePhysPath);
		drivePath = std::move(other.drivePath);
		serialNumber = std::move(other.serialNumber);
	}
}
#endif

DriveDesc::~DriveDesc() {}

DriveDesc& DriveDesc::operator=(const DriveDesc &other) {
	if (this != &other) {
		spaceFree = other.spaceFree;
		spaceTotal = other.spaceTotal;
		drivePhysPath = other.drivePhysPath;
		drivePath = other.drivePath;
		serialNumber = other.serialNumber;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
DriveDesc& DriveDesc::operator=(DriveDesc &&other) noexcept {
	if (this != &other) {
		spaceFree = std::exchange(other.spaceFree, 0);
		spaceTotal = std::exchange(other.spaceTotal, 0);
		drivePhysPath = std::move(other.drivePhysPath);
		drivePath = std::move(other.drivePath);
		serialNumber = std::move(other.serialNumber);
	}
	return *this;
}
#endif

bool DriveDesc::operator==(const DriveDesc &other) const {
	if (this != &other) {
		return (spaceFree == other.spaceFree &&
				spaceTotal == other.spaceTotal &&
				lower_copy(drivePhysPath) == lower_copy(other.drivePhysPath) &&
				lower_copy(drivePath) == lower_copy(other.drivePath) &&
				lower_copy(serialNumber) == lower_copy(other.serialNumber));
	} else {
		return true;
	}
}

bool DriveDesc::operator!=(const DriveDesc &other) const {
	if (this != &other) {
		return (spaceFree != other.spaceFree ||
				spaceTotal != other.spaceTotal ||
				lower_copy(drivePhysPath) != lower_copy(other.drivePhysPath) ||
				lower_copy(drivePath) != lower_copy(other.drivePath) ||
				lower_copy(serialNumber) != lower_copy(other.serialNumber));
	} else {
		return false;
	}
}

PartitionDesc::PartitionDesc() {
	partitionStyle = PartitionStyle::GPT;
	rewritePartition = false;
	MBRPartitionType = 0;
	MBRBootIndicator = 0;
	MBRRecognizedPartitions = 0;
	GPTAttributes = 0;
	startOffset = 0;
	spaceFree = 0;
	spaceTotal = 0;
	partNumber = 0;
}

PartitionDesc::PartitionDesc(const PartitionDesc &other) {
	if (this != &other) {
		partitionStyle = other.partitionStyle;
		rewritePartition = other.rewritePartition;
		MBRPartitionType = other.MBRPartitionType;
		MBRBootIndicator = other.MBRBootIndicator;
		MBRRecognizedPartitions = other.MBRRecognizedPartitions;
		GPTAttributes = other.GPTAttributes;
		startOffset = other.startOffset;
		spaceFree = other.spaceFree;
		spaceTotal = other.spaceTotal;
		partNumber = other.partNumber;
		MBRID = other.MBRID;
		GPTID = other.GPTID;
		GPTType = other.GPTType;
		GPTName = other.GPTName;
		partitionPath = other.partitionPath;
		volumePath = other.volumePath;
		volumes = other.volumes;
		drives = other.drives;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
PartitionDesc::PartitionDesc(PartitionDesc &&other) noexcept {
	if (this != &other) {
		partitionStyle = std::exchange(other.partitionStyle, PartitionStyle::GPT);
		rewritePartition = std::exchange(other.rewritePartition, false);
		MBRPartitionType = std::exchange(other.MBRPartitionType, 0);
		MBRBootIndicator = std::exchange(other.MBRBootIndicator, 0);
		MBRRecognizedPartitions = std::exchange(other.MBRRecognizedPartitions, 0);
		GPTAttributes = std::exchange(other.GPTAttributes, 0);
		startOffset = std::exchange(other.startOffset, 0);
		spaceFree = std::exchange(other.spaceFree, 0);
		spaceTotal = std::exchange(other.spaceTotal, 0);
		partNumber = std::exchange(other.partNumber, 0);
		MBRID = std::move(other.MBRID);
		GPTID = std::move(other.GPTID);
		GPTType = std::move(other.GPTType);
		GPTName = std::move(other.GPTName);
		partitionPath = std::move(other.partitionPath);
		volumePath = std::move(other.volumePath);
		volumes = std::move(other.volumes);
		drives = std::move(other.drives);
	}
}
#endif

PartitionDesc::~PartitionDesc() {}

PartitionDesc& PartitionDesc:: operator=(const PartitionDesc &other) {
	if (this != &other) {
		partitionStyle = other.partitionStyle;
		rewritePartition = other.rewritePartition;
		MBRPartitionType = other.MBRPartitionType;
		MBRBootIndicator = other.MBRBootIndicator;
		MBRRecognizedPartitions = other.MBRRecognizedPartitions;
		GPTAttributes = other.GPTAttributes;
		startOffset = other.startOffset;
		spaceFree = other.spaceFree;
		spaceTotal = other.spaceTotal;
		partNumber = other.partNumber;
		MBRID = other.MBRID;
		GPTID = other.GPTID;
		GPTType = other.GPTType;
		GPTName = other.GPTName;
		partitionPath = other.partitionPath;
		volumePath = other.volumePath;
		volumes = other.volumes;
		drives = other.drives;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
PartitionDesc& PartitionDesc::operator=(PartitionDesc &&other) noexcept {
	if (this != &other) {
		partitionStyle = std::exchange(other.partitionStyle, PartitionStyle::GPT);
		rewritePartition = std::exchange(other.rewritePartition, false);
		MBRPartitionType = std::exchange(other.MBRPartitionType, 0);
		MBRBootIndicator = std::exchange(other.MBRBootIndicator, 0);
		MBRRecognizedPartitions = std::exchange(other.MBRRecognizedPartitions, 0);
		GPTAttributes = std::exchange(other.GPTAttributes, 0);
		startOffset = std::exchange(other.startOffset, 0);
		spaceFree = std::exchange(other.spaceFree, 0);
		partNumber = std::exchange(other.partNumber, 0);
		MBRID = std::move(other.MBRID);
		GPTID = std::move(other.GPTID);
		GPTType = std::move(other.GPTType);
		GPTName = std::move(other.GPTName);
		partitionPath = std::move(other.partitionPath);
		volumePath = std::move(other.volumePath);
		volumes = std::move(other.volumes);
		drives = std::move(other.drives);
	}
	return *this;
}
#endif

bool PartitionDesc::operator==(const PartitionDesc &other) const {
	if (this != &other) {
		return (partitionStyle == other.partitionStyle &&
				rewritePartition == other.rewritePartition &&
				MBRPartitionType == other.MBRPartitionType &&
				MBRBootIndicator == other.MBRBootIndicator &&
				MBRRecognizedPartitions == other.MBRRecognizedPartitions &&
				GPTAttributes == other.GPTAttributes &&
				startOffset == other.startOffset &&
				spaceFree == other.spaceFree &&
				spaceTotal == other.spaceTotal &&
				partNumber == other.partNumber &&
				lower_copy(MBRID) == lower_copy(other.MBRID) &&
				lower_copy(GPTID) == lower_copy(other.GPTID) &&
				lower_copy(GPTType) == lower_copy(other.GPTType) &&
				lower_copy(GPTName) == lower_copy(other.GPTName) &&
				lower_copy(partitionPath) == lower_copy(other.partitionPath) &&
				lower_copy(volumePath) == lower_copy(other.volumePath) &&
				lower_copy(volumes) == lower_copy(other.volumes) &&
				lower_copy(drives) == lower_copy(other.drives));
	} else {
		return true;
	}
}

bool PartitionDesc::operator!=(const PartitionDesc &other) const {
	if (this != &other) {
		return (partitionStyle != other.partitionStyle ||
				rewritePartition != other.rewritePartition ||
				MBRPartitionType != other.MBRPartitionType ||
				MBRBootIndicator != other.MBRBootIndicator ||
				MBRRecognizedPartitions != other.MBRRecognizedPartitions ||
				GPTAttributes != other.GPTAttributes ||
				startOffset != other.startOffset ||
				spaceFree != other.spaceFree ||
				spaceTotal != other.spaceTotal ||
				partNumber != other.partNumber ||
				lower_copy(MBRID) != lower_copy(other.MBRID) ||
				lower_copy(GPTID) != lower_copy(other.GPTID) ||
				lower_copy(GPTType) != lower_copy(other.GPTType) ||
				lower_copy(GPTName) != lower_copy(other.GPTName) ||
				lower_copy(partitionPath) != lower_copy(other.partitionPath) ||
				lower_copy(volumePath) != lower_copy(other.volumePath) ||
				lower_copy(volumes) != lower_copy(other.volumes) ||
				lower_copy(drives) != lower_copy(other.drives));
	} else {
		return false;
	}
}

VolumeDesc::VolumeDesc() {
	deviceType = DevType::Unknown;
	volumeSerial = 0;
	spaceFree = 0;
	spaceTotal = 0;
	maxComponentLen = 0;
	caseSensitiveSearch = false;
	casePreservedMames = false;
	unicodeFNames = false;
	persistentACLS = false;
	supportFileCompress = false;
	supportQuota = false;
	supportSparseFile = false;
	supportReparsePoints = false;
	supportRemoteStorage = false;
	supportPosixUnlinkRename = false;
	supportObjectIds = false;
	supportEncryption = false;
	supportNamedStreams = false;
	supportTransactions = false;
	supportHardLink = false;
	suportExtendAttrib = false;
	supportFileIdOpen = false;
	supportUSNJournal = false;
	supportIntegrityStream = false;
	supportBlockRefCount = false;
	supportSparseVdl = false;
	supportGhosting = false;
	returnCleanupResInfo = false;
	volumeCompressed = false;
	volumeReadOnly = false;
	volumeDax = false;
	seqWriteOnce = false;
}

VolumeDesc::VolumeDesc(const VolumeDesc &other) {
	if (this != &other) {
		deviceType = other.deviceType;
		physDrives = other.physDrives;
		partLetter = other.partLetter;
		volumeLabel = other.volumeLabel;
		volumePath = other.volumePath;
		drivePath = other.drivePath;
		volumeSerial = other.volumeSerial;
		volumeSerialStr = other.volumeSerialStr;
		spaceFree = other.spaceFree;
		spaceTotal = other.spaceTotal;
		fsName = other.fsName;
		maxComponentLen = other.maxComponentLen;
		caseSensitiveSearch = other.caseSensitiveSearch;
		casePreservedMames = other.casePreservedMames;
		unicodeFNames = other.unicodeFNames;
		persistentACLS = other.persistentACLS;
		supportFileCompress = other.supportFileCompress;
		supportQuota = other.supportQuota;
		supportSparseFile = other.supportSparseFile;
		supportReparsePoints = other.supportReparsePoints;
		supportRemoteStorage = other.supportRemoteStorage;
		supportPosixUnlinkRename = other.supportPosixUnlinkRename;
		supportObjectIds = other.supportObjectIds;
		supportEncryption = other.supportEncryption;
		supportNamedStreams = other.supportNamedStreams;
		supportTransactions = other.supportTransactions;
		supportHardLink = other.supportHardLink;
		suportExtendAttrib = other.suportExtendAttrib;
		supportFileIdOpen = other.supportFileIdOpen;
		supportUSNJournal = other.supportUSNJournal;
		supportIntegrityStream = other.supportIntegrityStream;
		supportBlockRefCount = other.supportBlockRefCount;
		supportSparseVdl = other.supportSparseVdl;
		supportGhosting = other.supportGhosting;
		returnCleanupResInfo = other.returnCleanupResInfo;
		volumeCompressed = other.volumeCompressed;
		volumeReadOnly = other.volumeReadOnly;
		volumeDax = other.volumeDax;
		seqWriteOnce = other.seqWriteOnce;
	}
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
VolumeDesc::VolumeDesc(VolumeDesc &&other) noexcept {
	if (this != &other) {
		maxComponentLen = std::exchange(other.maxComponentLen, 0);
		caseSensitiveSearch = std::exchange(other.caseSensitiveSearch, false);
		casePreservedMames = std::exchange(other.casePreservedMames, false);
		unicodeFNames = std::exchange(other.unicodeFNames, false);
		persistentACLS = std::exchange(other.persistentACLS, false);
		supportFileCompress = std::exchange(other.supportFileCompress, false);
		supportQuota = std::exchange(other.supportQuota, false);
		supportSparseFile = std::exchange(other.supportSparseFile, false);
		supportReparsePoints = std::exchange(other.supportReparsePoints, false);
		supportRemoteStorage = std::exchange(other.supportRemoteStorage, false);
		supportPosixUnlinkRename = std::exchange(other.supportPosixUnlinkRename, false);
		supportObjectIds = std::exchange(other.supportObjectIds, false);
		supportEncryption = std::exchange(other.supportEncryption, false);
		supportNamedStreams = std::exchange(other.supportNamedStreams, false);
		supportTransactions = std::exchange(other.supportTransactions, false);
		supportHardLink = std::exchange(other.supportHardLink, false);
		suportExtendAttrib = std::exchange(other.suportExtendAttrib, false);
		supportFileIdOpen = std::exchange(other.supportFileIdOpen, false);
		supportUSNJournal = std::exchange(other.supportUSNJournal, false);
		supportIntegrityStream = std::exchange(other.supportIntegrityStream, false);
		supportBlockRefCount = std::exchange(other.supportBlockRefCount, false);
		supportSparseVdl = std::exchange(other.supportSparseVdl, false);
		supportGhosting = std::exchange(other.supportGhosting, false);
		returnCleanupResInfo = std::exchange(other.returnCleanupResInfo, false);
		volumeCompressed = std::exchange(other.volumeCompressed, false);
		volumeReadOnly = std::exchange(other.volumeReadOnly, false);
		volumeDax = std::exchange(other.volumeDax, false);
		seqWriteOnce = std::exchange(other.seqWriteOnce, false);
		deviceType = std::exchange(other.deviceType, DevType::Unknown);
		volumeSerial = std::exchange(other.volumeSerial, 0);
		spaceFree = std::exchange(other.spaceFree, 0);
		spaceTotal = std::exchange(other.spaceTotal, 0);
		physDrives = std::move(other.physDrives);
		partLetter = std::move(other.partLetter);
		volumeLabel = std::move(other.volumeLabel);
		volumePath = std::move(other.volumePath);
		drivePath = std::move(other.drivePath);
		volumeSerialStr = std::move(other.volumeSerialStr);
		fsName = std::move(other.fsName);
	}
}
#endif

VolumeDesc& VolumeDesc::operator=(const VolumeDesc &other) {
	if (this != &other) {
		physDrives = other.physDrives;
		partLetter = other.partLetter;
		drivePath = other.drivePath;
		volumeSerial = other.volumeSerial;
		volumeSerialStr = other.volumeSerialStr;
		volumeLabel = other.volumeLabel;
		fsName = other.fsName;
		spaceFree = other.spaceFree;
		spaceTotal = other.spaceTotal;
		deviceType = other.deviceType;
		maxComponentLen = other.maxComponentLen;
		caseSensitiveSearch = other.caseSensitiveSearch;
		casePreservedMames = other.casePreservedMames;
		unicodeFNames = other.unicodeFNames;
		persistentACLS = other.persistentACLS;
		supportFileCompress = other.supportFileCompress;
		supportQuota = other.supportQuota;
		supportSparseFile = other.supportSparseFile;
		supportReparsePoints = other.supportReparsePoints;
		supportRemoteStorage = other.supportRemoteStorage;
		supportPosixUnlinkRename = other.supportPosixUnlinkRename;
		supportObjectIds = other.supportObjectIds;
		supportEncryption = other.supportEncryption;
		supportNamedStreams = other.supportNamedStreams;
		supportTransactions = other.supportTransactions;
		supportHardLink = other.supportHardLink;
		suportExtendAttrib = other.suportExtendAttrib;
		supportFileIdOpen = other.supportFileIdOpen;
		supportUSNJournal = other.supportUSNJournal;
		supportIntegrityStream = other.supportIntegrityStream;
		supportBlockRefCount = other.supportBlockRefCount;
		supportSparseVdl = other.supportSparseVdl;
		supportGhosting = other.supportGhosting;
		returnCleanupResInfo = other.returnCleanupResInfo;
		volumeCompressed = other.volumeCompressed;
		volumeReadOnly = other.volumeReadOnly;
		volumeDax = other.volumeDax;
		seqWriteOnce = other.seqWriteOnce;
	}
	return *this;
}

#if (defined(STDVER) && STDVER >= 11 && STDVER != 98)
VolumeDesc& VolumeDesc::operator=(VolumeDesc &&other) noexcept {
	if (this != &other) {
		maxComponentLen = std::exchange(other.maxComponentLen, 0);
		caseSensitiveSearch = std::exchange(other.caseSensitiveSearch, false);
		casePreservedMames = std::exchange(other.casePreservedMames, false);
		unicodeFNames = std::exchange(other.unicodeFNames, false);
		persistentACLS = std::exchange(other.persistentACLS, false);
		supportFileCompress = std::exchange(other.supportFileCompress, false);
		supportQuota = std::exchange(other.supportQuota, false);
		supportSparseFile = std::exchange(other.supportSparseFile, false);
		supportReparsePoints = std::exchange(other.supportReparsePoints, false);
		supportRemoteStorage = std::exchange(other.supportRemoteStorage, false);
		supportPosixUnlinkRename = std::exchange(other.supportPosixUnlinkRename, false);
		supportObjectIds = std::exchange(other.supportObjectIds, false);
		supportEncryption = std::exchange(other.supportEncryption, false);
		supportNamedStreams = std::exchange(other.supportNamedStreams, false);
		supportTransactions = std::exchange(other.supportTransactions, false);
		supportHardLink = std::exchange(other.supportHardLink, false);
		suportExtendAttrib = std::exchange(other.suportExtendAttrib, false);
		supportFileIdOpen = std::exchange(other.supportFileIdOpen, false);
		supportUSNJournal = std::exchange(other.supportUSNJournal, false);
		supportIntegrityStream = std::exchange(other.supportIntegrityStream, false);
		supportBlockRefCount = std::exchange(other.supportBlockRefCount, false);
		supportSparseVdl = std::exchange(other.supportSparseVdl, false);
		supportGhosting = std::exchange(other.supportGhosting, false);
		returnCleanupResInfo = std::exchange(other.returnCleanupResInfo, false);
		volumeCompressed = std::exchange(other.volumeCompressed, false);
		volumeReadOnly = std::exchange(other.volumeReadOnly, false);
		volumeDax = std::exchange(other.volumeDax, false);
		seqWriteOnce = std::exchange(other.seqWriteOnce, false);
		deviceType = std::exchange(other.deviceType, DevType::Unknown);
		volumeSerial = std::exchange(other.volumeSerial, 0);
		spaceFree = std::exchange(other.spaceFree, 0);
		spaceTotal = std::exchange(other.spaceTotal, 0);
		physDrives = std::move(other.physDrives);
		partLetter = std::move(other.partLetter);
		volumeLabel = std::move(other.volumeLabel);
		volumePath = std::move(other.volumePath);
		drivePath = std::move(other.drivePath);
		volumeSerialStr = std::move(other.volumeSerialStr);
		fsName = std::move(other.fsName);
	}
	return *this;
}
#endif

bool VolumeDesc::operator==(const VolumeDesc &other) const {
	if (this != &other) {
		return (lower_copy(partLetter) == lower_copy(other.partLetter) &&
				lower_copy(drivePath) == lower_copy(other.drivePath) &&
				lower_copy(volumePath) == lower_copy(other.volumePath) &&
				lower_copy(volumeSerialStr) == lower_copy(other.volumeSerialStr) &&
				lower_copy(volumeLabel) == lower_copy(other.volumeLabel) &&
				lower_copy(fsName) == lower_copy(other.fsName) &&
				lower_copy(physDrives) == lower_copy(other.physDrives) &&
				deviceType == other.deviceType &&
				volumeSerial == other.volumeSerial &&
				spaceFree == other.spaceFree &&
				spaceTotal == other.spaceTotal &&
				fsName == other.fsName &&
				maxComponentLen == other.maxComponentLen &&
				caseSensitiveSearch == other.caseSensitiveSearch &&
				casePreservedMames == other.casePreservedMames &&
				unicodeFNames == other.unicodeFNames &&
				persistentACLS == other.persistentACLS &&
				supportFileCompress == other.supportFileCompress &&
				supportQuota == other.supportQuota &&
				supportSparseFile == other.supportSparseFile &&
				supportReparsePoints == other.supportReparsePoints &&
				supportRemoteStorage == other.supportRemoteStorage &&
				supportPosixUnlinkRename == other.supportPosixUnlinkRename &&
				supportObjectIds == other.supportObjectIds &&
				supportEncryption == other.supportEncryption &&
				supportNamedStreams == other.supportNamedStreams &&
				supportTransactions == other.supportTransactions &&
				supportHardLink == other.supportHardLink &&
				suportExtendAttrib == other.suportExtendAttrib &&
				supportFileIdOpen == other.supportFileIdOpen &&
				supportUSNJournal == other.supportUSNJournal &&
				supportIntegrityStream == other.supportIntegrityStream &&
				supportBlockRefCount == other.supportBlockRefCount &&
				supportSparseVdl == other.supportSparseVdl &&
				supportGhosting == other.supportGhosting &&
				returnCleanupResInfo == other.returnCleanupResInfo &&
				volumeCompressed == other.volumeCompressed &&
				volumeReadOnly == other.volumeReadOnly &&
				volumeDax == other.volumeDax &&
				seqWriteOnce == other.seqWriteOnce);
	} else {
		return true;
	}
}

bool VolumeDesc::operator!=(const VolumeDesc &other) const {
	if (this != &other) {
		return (lower_copy(partLetter) != lower_copy(other.partLetter) ||
				lower_copy(drivePath) != lower_copy(other.drivePath) ||
				lower_copy(volumePath) != lower_copy(other.volumePath) ||
				lower_copy(volumeSerialStr) != lower_copy(other.volumeSerialStr) ||
				lower_copy(volumeLabel) != lower_copy(other.volumeLabel) ||
				lower_copy(fsName) != lower_copy(other.fsName) ||
				lower_copy(physDrives) != lower_copy(other.physDrives) ||
				deviceType != other.deviceType ||
				volumeSerial != other.volumeSerial ||
				spaceFree != other.spaceFree ||
				spaceTotal != other.spaceTotal ||
				fsName != other.fsName ||
				maxComponentLen != other.maxComponentLen ||
				caseSensitiveSearch != other.caseSensitiveSearch ||
				casePreservedMames != other.casePreservedMames ||
				unicodeFNames != other.unicodeFNames ||
				persistentACLS != other.persistentACLS ||
				supportFileCompress != other.supportFileCompress ||
				supportQuota != other.supportQuota ||
				supportSparseFile != other.supportSparseFile ||
				supportReparsePoints != other.supportReparsePoints ||
				supportRemoteStorage != other.supportRemoteStorage ||
				supportPosixUnlinkRename != other.supportPosixUnlinkRename ||
				supportObjectIds != other.supportObjectIds ||
				supportEncryption != other.supportEncryption ||
				supportNamedStreams != other.supportNamedStreams ||
				supportTransactions != other.supportTransactions ||
				supportHardLink != other.supportHardLink ||
				suportExtendAttrib != other.suportExtendAttrib ||
				supportFileIdOpen != other.supportFileIdOpen ||
				supportUSNJournal != other.supportUSNJournal ||
				supportIntegrityStream != other.supportIntegrityStream ||
				supportBlockRefCount != other.supportBlockRefCount ||
				supportSparseVdl != other.supportSparseVdl ||
				supportGhosting != other.supportGhosting ||
				returnCleanupResInfo != other.returnCleanupResInfo ||
				volumeCompressed != other.volumeCompressed ||
				volumeReadOnly != other.volumeReadOnly ||
				volumeDax != other.volumeDax ||
				seqWriteOnce != other.seqWriteOnce);
	} else {
		return false;
	}
}

VolumeDesc::~VolumeDesc() {}

FSHandler::FSHandler() {}

FSHandler::~FSHandler() {}

FSOpResult FSHandler::EnumVolumes(std::vector<VolumeDesc> &volumeList, const bool clearList) {
	if (clearList) {
		volumeList.clear();
	}
	unsigned long psz = 0, psn = 0;
	wchar_t plBuf[4] = { 0 };
	wchar_t drivePathBuf[MAX_PATH + 1] = { 0 };
	wchar_t vpnBuf[MAX_PATH + 1] = { 0 };
	std::vector<VolumeDesc> ret;
	// ::HANDLE hPart = FindFirstVolume(vol_name, ARRAYSIZE(vol_name))
	unsigned long drives = ::GetLogicalDrives();
	if (drives) {
		if (clearList) {
			volumeList.clear();
		}
		for (int i = 0; i < 26; ++i) {
			VolumeDesc elem;
			plBuf[0] = wchar_t(65 + i);
			plBuf[1] = L':';
			plBuf[2] = L'\\';
			plBuf[3] = L'\0';
			elem.partLetter = plBuf;
			if (::GetVolumeNameForVolumeMountPoint(plBuf, drivePathBuf, MAX_PATH)) {
				elem.deviceType = (DevType)::GetDriveType(plBuf);
				elem.volumePath = drivePathBuf;
				unsigned long fileSystemFlags = 0;
				wchar_t fsname[MAX_PATH + 1] = { 0 };
				if (::GetVolumeInformation(plBuf, drivePathBuf, MAX_PATH, &psn,
					&elem.maxComponentLen, &fileSystemFlags, fsname, MAX_PATH)) {
					wchar_t buf[16] = { 0 };
					elem.volumeSerial = psn;
					swprintf(buf, L"%X", psn);
					upper_chars(buf);
					elem.fsName = fsname;
					elem.volumeSerialStr = buf;
					elem.volumeLabel = drivePathBuf;
					elem.caseSensitiveSearch = fileSystemFlags & FILE_CASE_SENSITIVE_SEARCH;
					elem.casePreservedMames = fileSystemFlags & FILE_CASE_PRESERVED_NAMES;
					elem.unicodeFNames = fileSystemFlags & FILE_UNICODE_ON_DISK;
					elem.persistentACLS = fileSystemFlags & FILE_PERSISTENT_ACLS;
					elem.supportFileCompress = fileSystemFlags & FILE_FILE_COMPRESSION;
					elem.supportQuota = fileSystemFlags & FILE_VOLUME_QUOTAS;
					elem.supportSparseFile = fileSystemFlags & FILE_SUPPORTS_SPARSE_FILES;
					elem.supportReparsePoints = fileSystemFlags & FILE_SUPPORTS_REPARSE_POINTS;
					elem.supportRemoteStorage = fileSystemFlags & FILE_SUPPORTS_REMOTE_STORAGE;
					elem.supportPosixUnlinkRename = fileSystemFlags & FILE_SUPPORTS_POSIX_UNLINK_RENAME;
					elem.supportObjectIds = fileSystemFlags & FILE_SUPPORTS_OBJECT_IDS;
					elem.supportEncryption = fileSystemFlags & FILE_SUPPORTS_ENCRYPTION;
					elem.supportNamedStreams = fileSystemFlags & FILE_NAMED_STREAMS;
					elem.supportTransactions = fileSystemFlags & FILE_SUPPORTS_TRANSACTIONS;
					elem.supportHardLink = fileSystemFlags & FILE_SUPPORTS_HARD_LINKS;
					elem.suportExtendAttrib = fileSystemFlags & FILE_SUPPORTS_EXTENDED_ATTRIBUTES;
					elem.supportFileIdOpen = fileSystemFlags & FILE_SUPPORTS_OPEN_BY_FILE_ID;
					elem.supportUSNJournal = fileSystemFlags & FILE_SUPPORTS_USN_JOURNAL;
					elem.supportIntegrityStream = fileSystemFlags & FILE_SUPPORTS_INTEGRITY_STREAMS;
					elem.supportBlockRefCount = fileSystemFlags & FILE_SUPPORTS_BLOCK_REFCOUNTING;
					elem.supportSparseVdl = fileSystemFlags & FILE_SUPPORTS_SPARSE_VDL;
					elem.supportGhosting = fileSystemFlags & FILE_SUPPORTS_GHOSTING;
					elem.returnCleanupResInfo = fileSystemFlags & FILE_RETURNS_CLEANUP_RESULT_INFO;
					elem.volumeCompressed = fileSystemFlags & FILE_VOLUME_IS_COMPRESSED;
					elem.volumeReadOnly = fileSystemFlags & FILE_READ_ONLY_VOLUME;
					elem.volumeDax = fileSystemFlags & FILE_DAX_VOLUME;
					elem.seqWriteOnce = fileSystemFlags & FILE_SEQUENTIAL_WRITE_ONCE;
				} else {
					return FSOpResult::Fail;
				}
				plBuf[0] = wchar_t(65 + i);
				plBuf[1] = L':';
				plBuf[2] = L'\0';
				if(::QueryDosDevice(plBuf, vpnBuf, elem.volumePath.length())) {
					elem.drivePath = vpnBuf;
				}
				if (FSOpResult::Success != GetDriveSpace(elem.partLetter, elem.spaceFree, elem.spaceTotal)) {
					return FSOpResult::Fail;
				}
				std::wstring partpath = L"\\\\.\\" + std::wstring(plBuf);
				::HANDLE hFile = ::CreateFile(partpath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
					0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (INVALID_HANDLE_VALUE != hFile) {
					unsigned long bytes = 0, bytesReturned = 0;
					VOLUME_DISK_EXTENTS* volDiskExt = (VOLUME_DISK_EXTENTS*)malloc(sizeof(VOLUME_DISK_EXTENTS));
					if (!volDiskExt) {
						return FSOpResult::Fail;
					}
					bool ret = ::DeviceIoControl(hFile, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, 0, 0, volDiskExt,
						sizeof(VOLUME_DISK_EXTENTS), &bytes, 0);
					if (!ret) {
						if (ERROR_MORE_DATA == getLastErrorCode()) {
							unsigned long numDisks = volDiskExt->NumberOfDiskExtents;
							SAFE_FREE(volDiskExt);
							volDiskExt = (VOLUME_DISK_EXTENTS*)malloc((numDisks * sizeof(DISK_EXTENT)) +
								sizeof(VOLUME_DISK_EXTENTS));
							if (!volDiskExt) {
								return FSOpResult::Fail;
							}
							ret = ::DeviceIoControl(hFile, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, 0, 0, volDiskExt,
								(numDisks * sizeof(DISK_EXTENT)) + sizeof(VOLUME_DISK_EXTENTS), &bytes, 0);
							if (!ret) {
								SAFE_FREE(volDiskExt);
								return FSOpResult::Fail;
							}
							for (unsigned long i = 0; i < volDiskExt->NumberOfDiskExtents; ++i) {
#ifdef FSH_FULLPHYSDRIVESTRING
								elem.physDrives.push_back(L"\\\\.\\PhysicalDrive" + std::to_wstring(volDiskExt->Extents[i].DiskNumber));
#else
								elem.physDrives.push_back(L"PhysicalDrive" + std::to_wstring(volDiskExt));
#endif
							}
						}
					} else {
						for (unsigned long i = 0; i < volDiskExt->NumberOfDiskExtents; ++i) {
#ifdef FSH_FULLPHYSDRIVESTRING
							elem.physDrives.push_back(L"\\\\.\\PhysicalDrive" + std::to_wstring(volDiskExt->Extents[i].DiskNumber));
#else
							elem.physDrives.push_back(L"PhysicalDrive" + std::to_wstring(volDiskExt));
#endif
						}
					}
					::CloseHandle(hFile);
					SAFE_FREE(volDiskExt);
				} else {
					return FSOpResult::Fail;
				}
				volumeList.push_back(elem);
			}
			memset(plBuf, 0, 4 * sizeof(wchar_t));
			memset(drivePathBuf, 0, (MAX_PATH + 1) * sizeof(wchar_t));
			memset(vpnBuf, 0, (MAX_PATH + 1) * sizeof(wchar_t));
			psn = 0;
		}
	}
	if (volumeList.size()) {
		return FSOpResult::Success;
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult FSHandler::EnumVolumes2(std::vector<VolumeDesc> &volumeList, const bool clearList) {
	size_t Index = 0, bufSz = (MAX_PATH + 1) * sizeof(wchar_t);
	if (clearList) {
		volumeList.clear();
	}
	int c = 0;
	MALLOC_NULLIFY(VolumeName, wchar_t, bufSz);
	if (!VolumeName) {
		return FSOpResult::Fail;
	}
	MALLOC_NULLIFY(DeviceName, wchar_t, bufSz);
	if (!DeviceName) {
		SAFE_FREE(VolumeName);
		return FSOpResult::Fail;
	}
	MALLOC_NULLIFY(VolPathName, wchar_t, bufSz);
	if (!VolPathName) {
		SAFE_FREE(VolumeName);
		SAFE_FREE(DeviceName);
		return FSOpResult::Fail;
	}
	MALLOC_NULLIFY(fsname, wchar_t, bufSz);
	if (!fsname) {
		SAFE_FREE(VolumeName);
		SAFE_FREE(DeviceName);
		SAFE_FREE(VolPathName);
		return FSOpResult::Fail;
	}
	::HANDLE FindHandle = ::FindFirstVolume(VolumeName, MAX_PATH);
	if (INVALID_HANDLE_VALUE != FindHandle) {
		for (;;) {
			if (VolumeName[0] != L'\\' ||
				VolumeName[1] != L'\\' ||
				VolumeName[2] != L'?' ||
				VolumeName[3] != L'\\' ||
				VolumeName[Index] != L'\\') {
				SAFE_FREE(VolumeName);
				SAFE_FREE(DeviceName);
				SAFE_FREE(VolPathName);
				SAFE_FREE(fsname);
				return FSOpResult::Fail;
			}
			VolumeDesc elem;
			elem.volumePath = VolumeName;
			if (::QueryDosDevice(removeFromEnd_copy(removeFromStart_copy(elem.volumePath, L"\\\\?\\"),
				{ L'\\' }).c_str(), DeviceName, MAX_PATH)) {
				elem.drivePath = DeviceName;
			} else {
				SAFE_FREE(VolumeName);
				SAFE_FREE(DeviceName);
				SAFE_FREE(VolPathName);
				SAFE_FREE(fsname);
				return FSOpResult::Fail;
			}
			unsigned long charCount = MAX_PATH;
			if (::GetVolumePathNamesForVolumeName(elem.volumePath.c_str(), VolPathName, MAX_PATH, &charCount)) {
				elem.partLetter = VolPathName;
			} else {
				SAFE_FREE(VolumeName);
				SAFE_FREE(DeviceName);
				SAFE_FREE(VolPathName);
				SAFE_FREE(fsname);
				return FSOpResult::Fail;
			}
			memset(VolumeName, 0, bufSz);
			wchar_t plBuf[4] = { 0 };
			unsigned long fileSystemFlags = 0, psn = 0;
			if (::GetVolumeInformation(elem.volumePath.c_str(), VolumeName, MAX_PATH, &psn,
				&elem.maxComponentLen, &fileSystemFlags, fsname, MAX_PATH)) {
				elem.deviceType = (DevType)::GetDriveType(elem.volumePath.c_str());
				wchar_t buf[16] = { 0 };
				elem.volumeSerial = psn;
				swprintf(buf, L"%X", psn);
				upper_chars(buf);
				elem.fsName = fsname;
				elem.volumeSerialStr = buf;
				elem.volumeLabel = VolumeName;
				elem.caseSensitiveSearch = fileSystemFlags & FILE_CASE_SENSITIVE_SEARCH;
				elem.casePreservedMames = fileSystemFlags & FILE_CASE_PRESERVED_NAMES;
				elem.unicodeFNames = fileSystemFlags & FILE_UNICODE_ON_DISK;
				elem.persistentACLS = fileSystemFlags & FILE_PERSISTENT_ACLS;
				elem.supportFileCompress = fileSystemFlags & FILE_FILE_COMPRESSION;
				elem.supportQuota = fileSystemFlags & FILE_VOLUME_QUOTAS;
				elem.supportSparseFile = fileSystemFlags & FILE_SUPPORTS_SPARSE_FILES;
				elem.supportReparsePoints = fileSystemFlags & FILE_SUPPORTS_REPARSE_POINTS;
				elem.supportRemoteStorage = fileSystemFlags & FILE_SUPPORTS_REMOTE_STORAGE;
				elem.supportPosixUnlinkRename = fileSystemFlags & FILE_SUPPORTS_POSIX_UNLINK_RENAME;
				elem.supportObjectIds = fileSystemFlags & FILE_SUPPORTS_OBJECT_IDS;
				elem.supportEncryption = fileSystemFlags & FILE_SUPPORTS_ENCRYPTION;
				elem.supportNamedStreams = fileSystemFlags & FILE_NAMED_STREAMS;
				elem.supportTransactions = fileSystemFlags & FILE_SUPPORTS_TRANSACTIONS;
				elem.supportHardLink = fileSystemFlags & FILE_SUPPORTS_HARD_LINKS;
				elem.suportExtendAttrib = fileSystemFlags & FILE_SUPPORTS_EXTENDED_ATTRIBUTES;
				elem.supportFileIdOpen = fileSystemFlags & FILE_SUPPORTS_OPEN_BY_FILE_ID;
				elem.supportUSNJournal = fileSystemFlags & FILE_SUPPORTS_USN_JOURNAL;
				elem.supportIntegrityStream = fileSystemFlags & FILE_SUPPORTS_INTEGRITY_STREAMS;
				elem.supportBlockRefCount = fileSystemFlags & FILE_SUPPORTS_BLOCK_REFCOUNTING;
				elem.supportSparseVdl = fileSystemFlags & FILE_SUPPORTS_SPARSE_VDL;
				elem.supportGhosting = fileSystemFlags & FILE_SUPPORTS_GHOSTING;
				elem.returnCleanupResInfo = fileSystemFlags & FILE_RETURNS_CLEANUP_RESULT_INFO;
				elem.volumeCompressed = fileSystemFlags & FILE_VOLUME_IS_COMPRESSED;
				elem.volumeReadOnly = fileSystemFlags & FILE_READ_ONLY_VOLUME;
				elem.volumeDax = fileSystemFlags & FILE_DAX_VOLUME;
				elem.seqWriteOnce = fileSystemFlags & FILE_SEQUENTIAL_WRITE_ONCE;
			}else {
				SAFE_FREE(VolumeName);
				SAFE_FREE(DeviceName);
				SAFE_FREE(VolPathName);
				return FSOpResult::Fail;
			}
			if (FSOpResult::Success != GetDriveSpace(elem.volumePath, elem.spaceFree, elem.spaceTotal)) {
				SAFE_FREE(VolumeName);
				SAFE_FREE(DeviceName);
				SAFE_FREE(VolPathName);
				SAFE_FREE(fsname);
				return FSOpResult::Fail;
			}
			std::wstring volPath = removeFromEnd_copy(elem.volumePath, { L'\\' });
			::HANDLE hFile = ::CreateFile(volPath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
				0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (INVALID_HANDLE_VALUE != hFile) {
				unsigned long bytes = 0, bytesReturned = 0;
				::VOLUME_DISK_EXTENTS* volDiskExt = (::VOLUME_DISK_EXTENTS*)malloc(sizeof(::VOLUME_DISK_EXTENTS));
				if (!volDiskExt) {
					SAFE_FREE(VolumeName);
					SAFE_FREE(DeviceName);
					SAFE_FREE(VolPathName);
					SAFE_FREE(fsname);
					if (::FindVolumeClose(FindHandle)) {
						FindHandle = INVALID_HANDLE_VALUE;
					} else {
						// FindVolumeClose error
					}
					return FSOpResult::Fail;
				}
				bool ret = ::DeviceIoControl(hFile, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, 0, 0, volDiskExt,
					sizeof(::VOLUME_DISK_EXTENTS), &bytes, 0);
				if (!ret) {
					if (ERROR_MORE_DATA == getLastErrorCode()) {
						unsigned long numDisks = volDiskExt->NumberOfDiskExtents;
						SAFE_FREE(volDiskExt);
						volDiskExt = (::VOLUME_DISK_EXTENTS*)malloc((numDisks * sizeof(::DISK_EXTENT)) +
							sizeof(::VOLUME_DISK_EXTENTS));
						if (!volDiskExt) {
							SAFE_FREE(VolumeName);
							SAFE_FREE(DeviceName);
							SAFE_FREE(VolPathName);
							SAFE_FREE(fsname);
							if (::FindVolumeClose(FindHandle)) {
								FindHandle = INVALID_HANDLE_VALUE;
							} else {
								// FindVolumeClose error
							}
							return FSOpResult::Fail;
						}
						ret = ::DeviceIoControl(hFile, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, 0, 0, volDiskExt,
							(numDisks * sizeof(::DISK_EXTENT)) + sizeof(::VOLUME_DISK_EXTENTS), &bytes, 0);
						if (!ret) {
							SAFE_FREE(volDiskExt);
							SAFE_FREE(VolumeName);
							SAFE_FREE(DeviceName);
							SAFE_FREE(VolPathName);
							SAFE_FREE(fsname);
							if (::FindVolumeClose(FindHandle)) {
								FindHandle = INVALID_HANDLE_VALUE;
							} else {
								// FindVolumeClose error
							}
							return FSOpResult::Fail;
						}
						for (unsigned long i = 0; i < volDiskExt->NumberOfDiskExtents; ++i) {
#ifdef FSH_FULLPHYSDRIVESTRING
							elem.physDrives.push_back(L"\\\\.\\PhysicalDrive" +
								std::to_wstring(volDiskExt->Extents[i].DiskNumber));
#else
							elem.physDrives.push_back(L"PhysicalDrive" +
								std::to_wstring(volDiskExt->Extents[i].DiskNumber));
#endif
						}
					} else {
						SAFE_FREE(volDiskExt);
						SAFE_FREE(VolumeName);
						SAFE_FREE(DeviceName);
						SAFE_FREE(VolPathName);
						SAFE_FREE(fsname);
						if (::FindVolumeClose(FindHandle)) {
							FindHandle = INVALID_HANDLE_VALUE;
						} else {
							// FindVolumeClose error
						}
						return FSOpResult::Fail;
					}
				} else {
					for (unsigned long i = 0; i < volDiskExt->NumberOfDiskExtents; ++i) {
#ifdef FSH_FULLPHYSDRIVESTRING
						elem.physDrives.push_back(L"\\\\.\\PhysicalDrive" +
							std::to_wstring(volDiskExt->Extents[i].DiskNumber));
#else
						elem.physDrives.push_back(L"PhysicalDrive" +
							std::to_wstring(volDiskExt->Extents[i].DiskNumber));
#endif
					}
				}
				//::VOLUME_LOGICAL_OFFSET logicalOff = { 0 };
				//logicalOff.LogicalOffset = 0; // 16049504256;
				//::VOLUME_PHYSICAL_OFFSETS physicalOff = { 0 };
				//if (!::DeviceIoControl(hFile, IOCTL_VOLUME_LOGICAL_TO_PHYSICAL, &logicalOff,
				//	sizeof(::VOLUME_LOGICAL_OFFSET), &physicalOff, sizeof(::VOLUME_PHYSICAL_OFFSETS),
				//	&bytesReturned, 0)) {
				//	SAFE_FREE(VolumeName);
				//	SAFE_FREE(DeviceName);
				//	SAFE_FREE(VolPathName);
				//	SAFE_FREE(fsname);
				//	if (::FindVolumeClose(FindHandle)) {
				//		FindHandle = INVALID_HANDLE_VALUE;
				//	} else {
				//		// FindVolumeClose error
				//	}
				//	return FSOpResult::Fail;
				//}
				// unsigned long bytesReturned = 0;
				void* outBuf = malloc(512);
				::VOLUME_LOGICAL_OFFSET logicalOff = { 0 };
				logicalOff.LogicalOffset = 0; // 16049504256
				::VOLUME_PHYSICAL_OFFSETS physicalOff = { 0 };
				++c;
				if (!::DeviceIoControl(hFile, IOCTL_VOLUME_LOGICAL_TO_PHYSICAL, &logicalOff,
					sizeof(::VOLUME_LOGICAL_OFFSET), outBuf, 512,
					&bytesReturned, 0)) {
					return FSOpResult::Fail;
				}
				::VOLUME_PHYSICAL_OFFSETS* physPOff = (::VOLUME_PHYSICAL_OFFSETS*)outBuf;
				if (c > 3) {
					Sleep(1);
				}
				/*if (!::DeviceIoControl(hFile, IOCTL_VOLUME_LOGICAL_TO_PHYSICAL, &logicalOff,
					sizeof(::VOLUME_LOGICAL_OFFSET), &physicalOff, sizeof(::VOLUME_PHYSICAL_OFFSETS),
					&bytesReturned, 0)) {
					return FSOpResult::Fail;
				}*/
				if (::CloseHandle(hFile)) {
					hFile = INVALID_HANDLE_VALUE;
				} else {
					// CloseHandle error
				}
				SAFE_FREE(volDiskExt);
			} else {
				SAFE_FREE(VolumeName);
				SAFE_FREE(DeviceName);
				SAFE_FREE(VolPathName);
				SAFE_FREE(fsname);
				return FSOpResult::Fail;
			}
			volumeList.push_back(elem);
			memset(VolumeName, 0, bufSz);
			memset(DeviceName, 0, bufSz);
			memset(VolPathName, 0, bufSz);
			if (!::FindNextVolume(FindHandle, VolumeName, MAX_PATH)) {
				if (ERROR_NO_MORE_FILES != getLastErrorCode()) {
					SAFE_FREE(VolumeName);
					SAFE_FREE(DeviceName);
					SAFE_FREE(VolPathName);
					SAFE_FREE(fsname);
					if (::FindVolumeClose(FindHandle)) {
						FindHandle = INVALID_HANDLE_VALUE;
					} else {
						// FindVolumeClose error
					}
					return FSOpResult::Fail;
				} else {
					break;
				}
			}
		}
		if (::FindVolumeClose(FindHandle)) {
			FindHandle = INVALID_HANDLE_VALUE;
		} else {
			// FindVolumeClose error
		}
	} else {
		SAFE_FREE(VolumeName);
		SAFE_FREE(DeviceName);
		SAFE_FREE(VolPathName);
		SAFE_FREE(fsname);
		return FSOpResult::Fail;
	}
	SAFE_FREE(VolumeName);
	SAFE_FREE(DeviceName);
	SAFE_FREE(VolPathName);
	SAFE_FREE(fsname);
	return FSOpResult::Success;
}

FSOpResult FSHandler::EnumPartitions(std::vector<PartitionDesc> &partList, const bool clearList) {
	size_t Index = 0, bufSz = (MAX_PATH + 1) * sizeof(wchar_t);
	if (clearList) {
		partList.clear();
	}
	MALLOC_NULLIFY(VolumeName, wchar_t, bufSz);
	if (!VolumeName) {
		return FSOpResult::Fail;
	}
	MALLOC_NULLIFY(DeviceName, wchar_t, bufSz);
	if (!DeviceName) {
		SAFE_FREE(VolumeName);
		return FSOpResult::Fail;
	}
	MALLOC_NULLIFY(partLetters, wchar_t, bufSz);
	if (!partLetters) {
		SAFE_FREE(VolumeName);
		SAFE_FREE(DeviceName);
		return FSOpResult::Fail;
	}
	::HANDLE FindHandle = ::FindFirstVolume(VolumeName, bufSz);
	if (INVALID_HANDLE_VALUE != FindHandle) {
		for (;;) {
			PartitionDesc partdesc;
			Index = wcslen_c(VolumeName) - 1;
			if (VolumeName[0] != L'\\' || VolumeName[1] != L'\\' ||
				VolumeName[2] != L'?' || VolumeName[3] != L'\\' ||
				VolumeName[Index] != L'\\') {
				SAFE_FREE(VolumeName);
				SAFE_FREE(DeviceName);
				SAFE_FREE(partLetters);
				return FSOpResult::Fail;
			}
			VolumeName[Index] = L'\0';
			if (!::QueryDosDevice(&VolumeName[4], DeviceName, bufSz)) {
				SAFE_FREE(VolumeName);
				SAFE_FREE(DeviceName);
				SAFE_FREE(partLetters);
				return FSOpResult::Fail;
			}
			VolumeName[Index] = L'\\';
			partdesc.partitionPath = DeviceName;
			partdesc.volumePath = VolumeName;
			if (FSOpResult::Success != GetDriveSpace(partdesc.volumePath, partdesc.spaceFree, partdesc.spaceTotal)) {
				SAFE_FREE(VolumeName);
				SAFE_FREE(DeviceName);
				SAFE_FREE(partLetters);
				return FSOpResult::Fail;
			}
			unsigned long charCount = MAX_PATH + 1;
			for (;;) {
				if (::GetVolumePathNamesForVolumeName(VolumeName, partLetters, charCount, &charCount)) {
					if (charCount && wcslen_c(partLetters)) {
						std::wstring repl = replaceChars(partLetters, L"\0", L"\n", charCount, 1, 1);
						partdesc.volumes = splitStr(repl, L"\n");
					}
					memset(partLetters, 0, bufSz);
					break;
				} else {
					if (ERROR_MORE_DATA != getLastErrorCode()) {
						SAFE_FREE(VolumeName);
						SAFE_FREE(DeviceName);
						SAFE_FREE(partLetters);
						return FSOpResult::Fail;
					}
				}
			}
			std::wstring partpath = replaceAll_copy(partdesc.volumePath, fs_pathnolim, L"\\\\.\\");
			if (endsWith(partpath, L"\\")) {
				partpath = removeFromEnd_copy(partpath, L"\\");
			}
			::HANDLE hFile = ::CreateFile(partpath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
				0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (INVALID_HANDLE_VALUE != hFile) {
				unsigned long bytes = 0, bytesReturned = 0;
				::VOLUME_DISK_EXTENTS* volDiskExt = (::VOLUME_DISK_EXTENTS*)malloc(sizeof(::VOLUME_DISK_EXTENTS));
				if (!volDiskExt) {
					SAFE_FREE(VolumeName);
					SAFE_FREE(DeviceName);
					SAFE_FREE(partLetters);
					return FSOpResult::Fail;
				}
				bool ret = ::DeviceIoControl(hFile, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, 0, 0, volDiskExt,
					sizeof(VOLUME_DISK_EXTENTS), &bytes, 0);
				if (!ret) {
					if (ERROR_MORE_DATA == getLastErrorCode()) {
						unsigned long numDisks = volDiskExt->NumberOfDiskExtents;
						SAFE_FREE(volDiskExt);
						volDiskExt = (::VOLUME_DISK_EXTENTS*)malloc((numDisks * sizeof(::DISK_EXTENT)) +
							sizeof(::VOLUME_DISK_EXTENTS));
						if (!volDiskExt) {
							SAFE_FREE(VolumeName);
							SAFE_FREE(DeviceName);
							SAFE_FREE(partLetters);
							return FSOpResult::Fail;
						}
						ret = ::DeviceIoControl(hFile, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, 0, 0, volDiskExt,
							(numDisks * sizeof(DISK_EXTENT)) + sizeof(VOLUME_DISK_EXTENTS), &bytes, 0);
						if (!ret) {
							SAFE_FREE(volDiskExt);
							SAFE_FREE(VolumeName);
							SAFE_FREE(DeviceName);
							SAFE_FREE(partLetters);
							return FSOpResult::Fail;
						}
					}
				}
				::CloseHandle(hFile);
				SAFE_FREE(volDiskExt);
			} else {
				SAFE_FREE(VolumeName);
				SAFE_FREE(DeviceName);
				SAFE_FREE(partLetters);
				return FSOpResult::Fail;
			}
			partList.push_back(partdesc);
			if (!::FindNextVolume(FindHandle, VolumeName, bufSz)) {
				if (ERROR_NO_MORE_FILES != getLastErrorCode()) {
					SAFE_FREE(VolumeName);
					SAFE_FREE(DeviceName);
					SAFE_FREE(partLetters);
					return FSOpResult::Fail;
				}
				break;
			}
		}
		::FindVolumeClose(FindHandle);
	} else {
		SAFE_FREE(VolumeName);
		SAFE_FREE(DeviceName);
		SAFE_FREE(partLetters);
		return FSOpResult::Fail;
	}
	SAFE_FREE(VolumeName);
	SAFE_FREE(DeviceName);
	SAFE_FREE(partLetters);
	return FSOpResult::Success;
}

FSOpResult FSHandler::EnumDrives(std::vector<DriveDesc> &driveList, const bool trimSerialNumber, const bool clearList,
	const std::vector<VolumeDesc> *volumeList) {
	// HW_GetHardDrives();
	std::vector<VolumeDesc> vollist;
	if (volumeList) {
		vollist = *volumeList;
	} else {
		if (FSOpResult::Success != EnumVolumes2(vollist, false)) {
			return FSOpResult::Fail;
		}
	}
	if (clearList) {
		driveList.clear();
	}
#ifdef ARFUNX
	std::vector<std::wstring> physDrives = HW_GetHardDrives();
	if (physDrives.size() && 0 == physDrives.size() % HW_LINESDRIVE) {
		unsigned long long spacetotal = 0;
		size_t i = 0;
		for (i = 0; i < physDrives.size() - 1; i += HW_LINESDRIVE) {
			spacetotal = 0;
			if (FSOpResult::Success != GetDriveSpace_DriveGeometry(physDrives[i + 1], spacetotal)) {
				return FSOpResult::Fail;
			}
			driveList.emplace_back(0, spacetotal, physDrives[i], physDrives[i + 1]);
		}
		std::wstring dptrh;
		for (i = 0; i < driveList.size(); ++i) {
			if (startsWith(driveList[i].drivePath, L"\\\\.\\")) {
				dptrh = driveList[i].drivePath;
			} else {
				dptrh = L"\\\\.\\" + driveList[i].drivePath;
			}
			if (endsWith(dptrh, L"\\")) {
				dptrh = removeFromEnd_copy(dptrh, L"\\");
			}
			::HANDLE hFile = ::CreateFile(dptrh.c_str(), GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (INVALID_HANDLE_VALUE != hFile) {
				unsigned long bytesReturned = 0;
				// DRIVE_LAYOUT_INFORMATION_EX* outBuf = (DRIVE_LAYOUT_INFORMATION_EX*)malloc(outBudSz);
				void* outBuf = malloc(FSH_TEMPBUFSZ);
				if (!outBuf) {
					return FSOpResult::Fail;
				}
				if (::DeviceIoControl(hFile, IOCTL_DISK_GET_DRIVE_LAYOUT_EX, 0, 0, outBuf, FSH_TEMPBUFSZ, &bytesReturned,
					0)) {
					Sleep(1);
					// void* outBufUntyoed = (void*)outBuf;
					::DRIVE_LAYOUT_INFORMATION_EX* bufTyped = (::DRIVE_LAYOUT_INFORMATION_EX*)outBuf;
					// unsigned long offset = offsetof(DRIVE_LAYOUT_INFORMATION_EX, PartitionEntry[0]);
					unsigned long offset = 2 * sizeof(unsigned long);
					PartitionDesc pdesc;
					for (size_t j = 0; j < bufTyped->PartitionCount; ++j) {
						memset(&pdesc, 0, sizeof(PartitionDesc));
#if defined(FSH_FULLPHYSDRIVESTRING)
						if (!startsWith(driveList[i].drivePath, L"\\\\.\\")) {
							pdesc.drives.push_back(L"\\\\.\\" + driveList[i].drivePath);
						} else {
							pdesc.drives.push_back(driveList[i].drivePath);
						}
#else
						if (startsWith(driveList[i].drivePath, L"\\\\.\\")) {
							pdesc.drives.push_back(removeFromStart_copy(driveList[i].drivePath, L"\\\\.\\"));
						} else {
							pdesc.drives.push_back(driveList[i].drivePath);
						}
#endif
						::PARTITION_INFORMATION_EX* partdata =
							(::PARTITION_INFORMATION_EX*)&bufTyped->PartitionEntry[j];
						if(::PARTITION_STYLE::PARTITION_STYLE_GPT == partdata->PartitionStyle) {
							pdesc.partitionStyle = PartitionStyle::GPT;
						} else if (::PARTITION_STYLE::PARTITION_STYLE_MBR == partdata->PartitionStyle) {
							pdesc.partitionStyle = PartitionStyle::MBR;
						} else if (::PARTITION_STYLE::PARTITION_STYLE_RAW == partdata->PartitionStyle) {
							pdesc.partitionStyle = PartitionStyle::RAW;
						} else {
							return FSOpResult::Fail;
						}
						pdesc.spaceTotal = partdata->PartitionLength.QuadPart;
						pdesc.startOffset = partdata->StartingOffset.QuadPart;
						pdesc.partNumber = partdata->PartitionNumber;
						pdesc.rewritePartition = partdata->RewritePartition;
						pdesc.MBRBootIndicator = partdata->Mbr.BootIndicator;
						pdesc.MBRHiddenSectors = partdata->Mbr.HiddenSectors;
						pdesc.MBRPartitionType = partdata->Mbr.PartitionType;
						pdesc.MBRRecognizedPartitions = partdata->Mbr.RecognizedPartition;
						pdesc.MBRID = GUID2wstr(partdata->Mbr.PartitionId);
						pdesc.GPTAttributes = partdata->Gpt.Attributes;
						pdesc.GPTName = partdata->Gpt.Name;
						pdesc.GPTID = GUID2wstr(partdata->Gpt.PartitionId);
						pdesc.GPTType = GUID2wstr(partdata->Gpt.PartitionType);
						driveList[i].partitions.push_back(pdesc);
					}
				} else {
					return FSOpResult::Fail;
				}
				::STORAGE_PROPERTY_QUERY storagePropertyQuery;
				memset(&storagePropertyQuery, 0, sizeof(::STORAGE_PROPERTY_QUERY));
				storagePropertyQuery.PropertyId = ::STORAGE_PROPERTY_ID::StorageDeviceProperty;
				storagePropertyQuery.QueryType = ::STORAGE_QUERY_TYPE::PropertyStandardQuery;
				::STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader = { 0 };
				memset(outBuf, 0, FSH_TEMPBUFSZ);
				if (!::DeviceIoControl(hFile, IOCTL_STORAGE_QUERY_PROPERTY,
					&storagePropertyQuery, sizeof(::STORAGE_PROPERTY_QUERY), &storageDescriptorHeader,
					sizeof(::STORAGE_DESCRIPTOR_HEADER), &bytesReturned, 0)) {
					SAFE_FREE(outBuf);
					if (!::CloseHandle(hFile)) {
						// CloseHandle failed
					}
					return FSOpResult::Fail;
				}
				if (!::DeviceIoControl(hFile, IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery,
					sizeof(::STORAGE_PROPERTY_QUERY), outBuf, 8192, &bytesReturned, 0)) {
					SAFE_FREE(outBuf);
					if (!::CloseHandle(hFile)) {
						// CloseHandle failed
					}
					return FSOpResult::Fail;
				}
				::STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = (::STORAGE_DEVICE_DESCRIPTOR*)outBuf;
				if (pDeviceDescriptor->SerialNumberOffset) {
					MALLOC_NULLIFY(snbuf, char, FSH_SBBUFSZ);
					if (!snbuf) {
						SAFE_FREE(outBuf);
						if (!::CloseHandle(hFile)) {
							// CloseHandle failed
						}
						return FSOpResult::Fail;
					}
					sprintf(snbuf, "%s", ((char*)outBuf + pDeviceDescriptor->SerialNumberOffset));
					if (trimSerialNumber) {
						driveList[i].serialNumber = str2wstr(trim_copy(snbuf));
					} else {
						driveList[i].serialNumber = str2wstr(snbuf);
					}
					SAFE_FREE(snbuf);
				}
				SAFE_FREE(outBuf);
				if (!::CloseHandle(hFile)) {
					// CloseHandle failed
				}
			} else {
				return FSOpResult::Fail;
			}
		}
		return FSOpResult::Success;
	} else {
		return FSOpResult::Fail;
	}
#else
	return FSOpResult::Fail;
#endif
}

FSOpResult FSHandler::GetBinaryFileInfo(const std::wstring binaryPath, BinData &binaryData) const {
	if (INVALID_FILE_ATTRIBUTES != ::GetFileAttributes(binaryPath.c_str())) {
		unsigned long binType = 0;
		if (GetBinaryType(binaryPath.c_str(), &binType)) {
			if (SCS_32BIT_BINARY == binType) {
				binaryData.BitDepth = BinBitDepth::X32;
				binaryData.Platform = BinPlatform::Windows;
			} else if (SCS_64BIT_BINARY == binType) {
				binaryData.BitDepth = BinBitDepth::X64;
				binaryData.Platform = BinPlatform::Windows;
			} else if (SCS_DOS_BINARY == binType) {
				binaryData.BitDepth = BinBitDepth::X16;
				binaryData.Platform = BinPlatform::DOS;
			} else if (SCS_OS216_BINARY == binType) {
				binaryData.BitDepth = BinBitDepth::X16;
				binaryData.Platform = BinPlatform::OSdiv2;
			} else if (SCS_PIF_BINARY == binType) {
				binaryData.BitDepth = BinBitDepth::X16;
				binaryData.Platform = BinPlatform::Windows;
			} else if (SCS_POSIX_BINARY == binType) {
				binaryData.BitDepth = BinBitDepth::BitDepthUnknown;
				binaryData.Platform = BinPlatform::Unix;
			} else if (SCS_WOW_BINARY == binType) {
				binaryData.BitDepth = BinBitDepth::X16;
				binaryData.Platform = BinPlatform::Windows;
			} else {
				binaryData.BitDepth = BinBitDepth::BitDepthUnknown;
				binaryData.Platform = BinPlatform::PlatformUnknown;
			}
			return FSOpResult::Success;
		}
	}
	return FSOpResult::Fail;
}

FSOpResult FSHandler::CreateFolder(const std::wstring folderPath) const {
	if (!pathExists(folderPath)) {
		if (::CreateDirectory(folderPath.c_str(), 0)) {
			return FSOpResult::Success;
		} else {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult FSHandler::CreateFolder(const std::wstring folderPath, const SECURITY_ATTRIBUTES *secAttr) const {
	if (!pathExists(folderPath)) {
		if (::CreateDirectory(folderPath.c_str(), (SECURITY_ATTRIBUTES*)secAttr)) {
			return FSOpResult::Success;
		} else {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult FSHandler::CreateFolder(const std::wstring folderPath, const SecDesc secDesc) const {
	if (!pathExists(folderPath)) {
		if (!::CreateDirectory(folderPath.c_str(), 0)) {
			return FSOpResult::Fail;
		}
	}
	if (FSOpResult::Success == SetObjectSecurity(secDesc, folderPath)) {
		return FSOpResult::Success;
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult FSHandler::RemoveFolder_SHFileOp(const std::wstring folderPath, std::wstring *infoBuf) const {
	if (infoBuf && IsBadWritePtr(infoBuf)) {
		return FSOpResult::Fail;
	}
	if (!pathExists(folderPath)) {
		if (infoBuf) {
			*infoBuf = L"Specified path does not exist";
		}
		return FSOpResult::Fail;
	}
	bool isfld = true;
	if (FSOpResult::Success == isFolder(isfld, folderPath)) {
		if (!isfld) {
			if (infoBuf) {
				*infoBuf = L"Specified path is not a folder";
			}
			return FSOpResult::Fail;
		}
	}
	size_t folderPathLen = wcslen_c(folderPath.c_str()) + 2;
	wchar_t* folderPathBuf = (wchar_t*)malloc(folderPathLen * sizeof(wchar_t));
	if (!folderPathBuf) {
		return FSOpResult::Fail;
	}
	memset(folderPathBuf, 0, folderPathLen * sizeof(wchar_t));
	wsprintf(folderPathBuf, L"%s", folderPath.c_str());
	SHFILEOPSTRUCT fileOp = {
		0,
		FO_DELETE,
		folderPathBuf,
		L"",
		FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT,
		false,
		0,
		L"" };
	unsigned long opres = SHFileOperation(&fileOp);
	SAFE_FREE(folderPathBuf);
	if (opres) {
		if (infoBuf) {
			if (FSOpResult::Success != getSHFileOpDesc(opres, infoBuf)) {
				return FSOpResult::Fail;
			}
		}
		return FSOpResult::Fail;
	} else {
		return FSOpResult::Success;
	}
}

FSOpResult FSHandler::CustomFileOp_SHFileOp(const std::wstring folderPath, const std::wstring folderPathDest,
	const unsigned long operation, const unsigned long opCode, std::wstring *infoBuf) const {
	if (!pathExists(folderPath) && !pathExists(folderPathDest)) {
		if (infoBuf && !IsBadWritePtr(infoBuf)) {
			*infoBuf = L"Requested path does not exist";
		}
		return FSOpResult::Fail;
	}
	size_t folderPathLen = wcslen_c(folderPath.c_str()) + 2;
	wchar_t* folderPathBuf = 0;
	if (folderPathLen) {
		folderPathBuf = (wchar_t*)malloc(folderPathLen * sizeof(wchar_t));
		if (!folderPathBuf) {
			return FSOpResult::Fail;
		}
		memset(folderPathBuf, 0, folderPathLen * sizeof(wchar_t));
		wsprintf(folderPathBuf, L"%s", folderPath.c_str());
	}
	folderPathLen = wcslen_c(folderPathDest.c_str()) + 2;
	wchar_t* folderPathDestBuf = 0;
	if (folderPathLen) {
		folderPathDestBuf = (wchar_t*)malloc(folderPathLen * sizeof(wchar_t));
		if (!folderPathDestBuf) {
			return FSOpResult::Fail;
		}
		memset(folderPathDestBuf, 0, folderPathLen * sizeof(wchar_t));
		wsprintf(folderPathDestBuf, L"%s", folderPathDest.c_str());
	}
	SHFILEOPSTRUCT fileOp = {
		0,
		operation,
		folderPathBuf,
		folderPathDestBuf,
		opCode,
		false,
		0,
		L"" };
	unsigned long opres = SHFileOperation(&fileOp);
	SAFE_FREE(folderPathBuf);
	SAFE_FREE(folderPathDestBuf);
	if (!opres) {
		return FSOpResult::Success;
	} else {
		if (infoBuf) {
			if (FSOpResult::Success != getSHFileOpDesc(opres, infoBuf)) {
				return FSOpResult::Fail;
			}
		}
		return FSOpResult::Fail;
	}
}

FSOpResult FSHandler::RemoveFolder(const std::wstring folderPath, const bool includeFiles) {
	std::vector<FileRecord> folderFiles = SeekFileInDir(folderPath, L".*", L"", false, false);
	if (folderFiles.size()) {
		if (includeFiles) {
			for (size_t i = 0; i < folderFiles.size(); ++i) {
				if (FSOpResult::Success != RemoveFile(folderFiles[i].filePath)) {
					return FSOpResult::Fail;
				}
			}
		} else {
			return FSOpResult::Fail;
		}
	}
	if (::RemoveDirectory(folderPath.c_str())) {
		return FSOpResult::Success;
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult FSHandler::RenameFolder(const std::wstring folderPath, const std::wstring folderPathDest,
	const RenameBehaviour renameBehaviour) {
	if (lower_copy(folderPath) == lower_copy(folderPathDest)) {
		return FSOpResult::Fail;
	}
	if (RenameBehaviour::Rename == renameBehaviour) {
		if (!pathExists(folderPath)) {
			return FSOpResult::Fail;
		}
		if (pathExists(folderPathDest)) {
			return FSOpResult::Fail;
		}
		if (MAX_PATH <= folderPath.length()) {
			return FSOpResult::Fail;
		}
		if (MAX_PATH <= folderPathDest.length()) {
			return FSOpResult::Fail;
		}
		bool isfld = true;
		if (FSOpResult::Success == isFolder(isfld, folderPath)) {
			if (!isfld) {
				return FSOpResult::Fail;
			}
		}
		char* bufsrc = wchar2char(folderPath.c_str());
		if (!bufsrc) {
			return FSOpResult::Fail;
		}
		char* buftgt = wchar2char(folderPathDest.c_str());
		if (!buftgt) {
			return FSOpResult::Fail;
		}
		int renres = rename(bufsrc, buftgt);
		SAFE_ARR_DELETE(bufsrc);
		SAFE_ARR_DELETE(buftgt);
		if (!renres) {
			return FSOpResult::Success;
		} else {
			return FSOpResult::Fail;
		}
	} else if (RenameBehaviour::CopyDelete == renameBehaviour) {
		if (FSOpResult::Success != MoveFolder(folderPath, folderPathDest, true)) {
			return FSOpResult::Fail;
		}
		return FSOpResult::Success;
	} else {
		return FSOpResult::Fail;
	}
	
}

FSOpResult FSHandler::MoveFolder(const std::wstring folderPath, const std::wstring folderPathDest,
	const bool checkDestSpace) {
	if (!folderPath.length() || !folderPathDest.length()) {
		return FSOpResult::Fail;
	}
	if (lower_copy(trim_copy(folderPath)) == lower_copy(trim_copy(folderPathDest))) {
		return FSOpResult::Fail;
	}
	if (!pathExists(folderPath)) {
		return FSOpResult::Fail;
	}
	if (pathExists(folderPathDest)) {
		return FSOpResult::Fail;
	}
	bool isfld = true;
	if (FSOpResult::Success == isFolder(isfld, folderPath)) {
		if (!isfld) {
			return FSOpResult::Fail;
		}
	}
	if (checkDestSpace) {
		std::vector<std::wstring> pathSpl = splitStr(folderPathDest, L"\\");
		if (2 > pathSpl.size()) {
			return FSOpResult::Fail;
		}
		unsigned long long freeSpace = 0, totalSpace = 0;
		std::wstring partLetter = pathSpl.front();
		if (endsWith(partLetter, L"\\")) {
			partLetter = removeFromEnd_copy(partLetter, L"\\");
		}
		if (FSOpResult::Success != GetDriveSpace(partLetter, freeSpace, totalSpace)) {
			return FSOpResult::Fail;
		}
		unsigned long long foldersz = 0;
		if (FSOpResult::Success == GetFolderSizeOnDrive_NtQueryDir(foldersz, folderPath)) {
			return FSOpResult::Fail;
		}
		if (foldersz >= freeSpace) {
			return FSOpResult::Fail;
		}
	}
	if (!::CopyFile(folderPath.c_str(), folderPathDest.c_str(), true)) {
		return FSOpResult::Fail;
	}
	return RemoveFolder(folderPath);
}

FSOpResult FSHandler::CopyFolder(const std::wstring folderPath, const std::wstring folderPathDest,
	const bool checkDestSpace) {
	if (lower_copy(folderPath) == lower_copy(folderPathDest)) {
		return FSOpResult::Fail;
	}
	if (!pathExists(folderPath)) {
		return FSOpResult::Fail;
	}
	if (pathExists(folderPathDest)) {
		return FSOpResult::Fail;
	}
	bool isfld = true;
	if (FSOpResult::Success == isFolder(isfld, folderPath)) {
		if (!isfld) {
			return FSOpResult::Fail;
		}
	}
	if (checkDestSpace) {
		std::vector<std::wstring> pathSpl = splitStr(folderPathDest, L"\\");
		if (2 > pathSpl.size()) {
			return FSOpResult::Fail;
		}
		unsigned long long freeSpace = 0, totalSpace = 0;
		std::wstring partLetter = pathSpl.front();
		if (endsWith(partLetter, L"\\")) {
			partLetter = removeFromEnd_copy(partLetter, L"\\");
		}
		if (FSOpResult::Success != GetDriveSpace(partLetter, freeSpace, totalSpace)) {
			return FSOpResult::Fail;
		}
		unsigned long long foldersz = 0;
		if (FSOpResult::Success == GetFolderSizeOnDrive_NtQueryDir(foldersz, folderPath)) {
			return FSOpResult::Fail;
		}
		if (foldersz >= freeSpace) {
			return FSOpResult::Fail;
		}
	}
	std::wstring folderPathPrepped = fs_pathnolim + folderPath;
	std::wstring folderPathDestPrepped = fs_pathnolim + folderPathDest;
	if (!::CopyFile(folderPathPrepped.c_str(), folderPathDestPrepped.c_str(), true)) {
		return FSOpResult::Fail;
	}
	return FSOpResult::Success;
}

FSOpResult FSHandler::RenameFile(const std::wstring filePath, const std::wstring filePathDest,
	const RenameBehaviour renameBehaviour) {
	if (lower_copy(filePath) == lower_copy(filePathDest)) {
		return FSOpResult::Fail;
	}
	if (RenameBehaviour::Rename == renameBehaviour) {
		if (!pathExists(filePath)) {
			return FSOpResult::Fail;
		}
		if (pathExists(filePathDest)) {
			return FSOpResult::Fail;
		}
		if (MAX_PATH <= filePath.length()) {
			return FSOpResult::Fail;
		}
		if (MAX_PATH <= filePathDest.length()) {
			return FSOpResult::Fail;
		}
		bool isfld = true;
		if (FSOpResult::Success == isFolder(isfld, filePath)) {
			if (isfld) {
				return FSOpResult::Fail;
			}
		}
		char* bufsrc = wchar2char(filePath.c_str());
		if (!bufsrc) {
			return FSOpResult::Fail;
		}
		char* buftgt = wchar2char(filePathDest.c_str());
		if (!buftgt) {
			return FSOpResult::Fail;
		}
		int renres = rename(bufsrc, buftgt);
		SAFE_ARR_DELETE(bufsrc);
		SAFE_ARR_DELETE(buftgt);
		if (!renres) {
			return FSOpResult::Success;
		} else {
			return FSOpResult::Fail;
		}
	} else if (RenameBehaviour::CopyDelete == renameBehaviour) {
		if (FSOpResult::Success != FileMove(filePath, filePathDest, true)) {
			return FSOpResult::Fail;
		}
		return FSOpResult::Success;
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult FSHandler::FileMove(const std::wstring filePath, const std::wstring filePathDest,
	const bool checkDestSpace) {
	if (FSOpResult::Success != FileCopy(filePath, filePathDest, checkDestSpace)) {
		return FSOpResult::Fail;
	}
	return RemoveFile(filePath);
}

FSOpResult FSHandler::FileCopy(const std::wstring filePath, const std::wstring filePathDest,
	const bool checkDestSpace) {
	if (lower_copy(filePath) == lower_copy(filePathDest)) {
		return FSOpResult::Fail;
	}
	if (!pathExists(filePath)) {
		return FSOpResult::Fail;
	}
	if (pathExists(filePathDest)) {
		return FSOpResult::Fail;
	}
	bool isfld = true;
	if (FSOpResult::Success == isFolder(isfld, filePath)) {
		if (isfld) {
			return FSOpResult::Fail;
		}
	}
	if (checkDestSpace) {
		std::vector<std::wstring> pathSpl = splitStr(filePathDest, L"\\");
		if (2 > pathSpl.size()) {
			return FSOpResult::Fail;
		}
		unsigned long long freeSpace = 0, totalSpace = 0;
		std::wstring partLetter = pathSpl.front();
		if (endsWith(partLetter, L"\\")) {
			partLetter = removeFromEnd_copy(partLetter, L"\\");
		}
		if (FSOpResult::Success != GetDriveSpace(partLetter, freeSpace, totalSpace)) {
			return FSOpResult::Fail;
		}
		unsigned long long filesz = 0;
		if (FSOpResult::Success != GetFSizeOnDrive_NtQueryDir(filesz, filePath)) {
			return FSOpResult::Fail;
		}
		if (filesz >= freeSpace) {
			return FSOpResult::Fail;
		}
	}
	std::wstring filePathPrepped = fs_pathnolim + filePath;
	std::wstring filePathDestPrepped = fs_pathnolim + filePathDest;
	if (::CopyFile(filePathPrepped.c_str(), filePathDestPrepped.c_str(), true)) {
		return FSOpResult::Success;
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult FSHandler::RemoveFile_SHFileOp(const std::wstring filePath, std::wstring *infoBuf) const {
	if (infoBuf && IsBadWritePtr(infoBuf)) {
		return FSOpResult::Fail;
	}
	if (!pathExists(filePath)) {
		if (infoBuf) {
			*infoBuf = L"Specified path does not exist";
		}
		return FSOpResult::Fail;
	}
	bool isfld = true;
	if (FSOpResult::Success == isFolder(isfld, filePath)) {
		if (isfld) {
			if (infoBuf) {
				*infoBuf = L"Specified path is not a file";
			}
			return FSOpResult::Fail;
		}
	}
	size_t filePathLen = wcslen_c(filePath.c_str()) + 2;
	wchar_t* filePathBuf = (wchar_t*)malloc(filePathLen * sizeof(wchar_t));
	if (!filePathBuf) {
		if (infoBuf) {
			*infoBuf = L"Buffer allocation in RemoveFile_SHFileOp failed";
		}
		return FSOpResult::Fail;
	}
	memset(filePathBuf, 0, filePathLen * sizeof(wchar_t));
	wsprintf(filePathBuf, L"%s", filePath.c_str());
	SHFILEOPSTRUCT fileOp = {
		0,
		FO_DELETE,
		filePathBuf,
		L"",
		FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT,
		false,
		0,
		L"" };
	unsigned long opres = SHFileOperation(&fileOp);
	SAFE_FREE(filePathBuf);
	if (!opres) {
		return FSOpResult::Success;
	} else {
		if (infoBuf) {
			if (FSOpResult::Success != getSHFileOpDesc(opres, infoBuf)) {
				return FSOpResult::Fail;
			}
		}
		return FSOpResult::Fail;
	}
}

FSOpResult FSHandler::RemoveFile(const std::wstring filePath) const {
	if (::DeleteFile(filePath.c_str())) {
		return FSOpResult::Success;
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult FSHandler::GetFolderSizeOnDrive_NtQueryDir(unsigned long long &folderSize, const std::wstring folderPath) {
	if (!pathExists(folderPath)) {
		return FSOpResult::Fail;
	}
	bool isfld = false;
	if (FSOpResult::Success == isFolder(isfld, folderPath)) {
		if (!isfld) {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
	::HMODULE hModule = ::LoadLibrary(L"ntdll.dll");
	if (!hModule) {
		return FSOpResult::Fail;
	}
	pRtlInitUnicodeString RtlInitUnicodeString = (pRtlInitUnicodeString)
		::GetProcAddress(hModule, "RtlInitUnicodeString");
	pNtCreateFile NtCreateFile = (pNtCreateFile)
		::GetProcAddress(hModule, "NtCreateFile");
	pNtCreateEvent NtCreateEvent = (pNtCreateEvent)
		::GetProcAddress(hModule, "NtCreateEvent");
	pNtQueryDirectoryFile NtQueryDirectoryFile = (pNtQueryDirectoryFile)
		::GetProcAddress(hModule, "NtQueryDirectoryFile");
	pNtWaitForSingleobject NtWaitForSingleobject = (pNtWaitForSingleobject)
		::GetProcAddress(hModule, "NtWaitForSingleObject");
	pRtlUnicodeStringToAnsiString RtlUnicodeStringToAnsiString = (pRtlUnicodeStringToAnsiString)
		::GetProcAddress(hModule, "RtlUnicodeStringToAnsiString");
	pNtClearEvent NtClearEvent = (pNtClearEvent)
		::GetProcAddress(hModule, "NtClearEvent");
	pNtClose NtClose = (pNtClose)
		::GetProcAddress(hModule, "NtClose");
	if (0 == RtlInitUnicodeString) {
		return FSOpResult::Fail;
	}
	if (0 == RtlUnicodeStringToAnsiString) {
		return FSOpResult::Fail;
	}
	if (0 == NtCreateFile) {
		return FSOpResult::Fail;
	}
	if (0 == NtCreateEvent) {
		return FSOpResult::Fail;
	}
	if (0 == NtQueryDirectoryFile) {
		return FSOpResult::Fail;
	}
	if (0 == NtWaitForSingleobject) {
		return FSOpResult::Fail;
	}
	if (0 == NtClearEvent) {
		return FSOpResult::Fail;
	}
	if (0 == NtClose) {
		return FSOpResult::Fail;
	}
	::HANDLE RootDirectoryHandle = 0;
	::HANDLE Event = 0;
	NTSTATUS ntStatus = STATUS_SUCCESS;
	UnicodeString RootDirectoryName;
	ANSIString as;
	ObjectAttributes RootDirectoryAttributes;
	IOStatusBlock IOSB;
	FileDirBothInformation* DirInformation;
	wchar_t* wszBuffer = (wchar_t*)malloc(64 * sizeof(wchar_t));
	if (!wszBuffer) {
		return FSOpResult::Fail;
	}
	wchar_t* Buffer = (wchar_t*)malloc(65536 * sizeof(wchar_t));
	if (!Buffer) {
		return FSOpResult::Fail;
	}
	wsprintf(wszBuffer, L"\\??\\%s\\", folderPath.c_str());
	ntStatus = ((RtlInitUnicodeString)(&RootDirectoryName, wszBuffer));
	if (!NT_SUCCESS(ntStatus)) {
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	InitializeObjectAttributes(&RootDirectoryAttributes, &RootDirectoryName, OBJ_CASE_INSENSITIVE, 0, 0);
	ntStatus = ((NtCreateFile)(&RootDirectoryHandle,
		GENERIC_READ,
		&RootDirectoryAttributes,
		&IOSB,
		0,
		FILE_ATTRIBUTE_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_DIRECTORY_FILE,
		0, 0));
	if (!NT_SUCCESS(ntStatus)) {
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	ntStatus = ((NtCreateEvent)(&Event, GENERIC_ALL, 0, EvtType::NotificationEvent, 0));
	if (!NT_SUCCESS(ntStatus)) {
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	ntStatus = ((NtQueryDirectoryFile)(RootDirectoryHandle,
		Event, 0, 0,
		&IOSB,
		Buffer,
		65536 * sizeof(unsigned char),
		FolderInfo::FileDirectoryBothInformation,
		0, 0, 0));
	if (STATUS_PENDING == ntStatus) {
		ntStatus = ((NtWaitForSingleobject)(Event, 1, 0));
	}
	if (!NT_SUCCESS(ntStatus)) {
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	DirInformation = (FileDirBothInformation*)Buffer;
	UnicodeString EntryName;
	while (true) {
		EntryName.MaximumLength = EntryName.Length = (unsigned short)DirInformation->FileNameLength;
		EntryName.Buffer = &DirInformation->FileName[0];
		((pRtlUnicodeStringToAnsiString)(&as, &EntryName, 1));
		if (DirInformation->FileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (strcmp(as.Buffer, ".") && strcmp(as.Buffer, "..")) {
				wchar_t* tbuf = char2wchar(as.Buffer);
				std::wstring targpath = folderPath + L"\\" + tbuf;
				GetFolderSizeOnDriveRec(folderSize, targpath, hModule);
				SAFE_ARR_DELETE(tbuf);
			}
		} else {
			folderSize += DirInformation->AllocationSize.QuadPart;
		}
		if (!DirInformation->NextEntryOffset) {
			break;
		} else {
			DirInformation = (FileDirBothInformation*)(((unsigned char*)DirInformation) + DirInformation->NextEntryOffset);
		}
	}
	((NtClose)(RootDirectoryHandle));
	ntStatus = ((NtClearEvent)(Event));
	if (!NT_SUCCESS(ntStatus)) {
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	((NtClose)(Event));
	SAFE_FREE(wszBuffer);
	SAFE_FREE(Buffer);
	if (!::FreeLibrary(hModule)) {
		return FSOpResult::Fail;
	}
	return FSOpResult::Success;
}

FSOpResult FSHandler::GetFolderSize(unsigned long long &folderSize, const std::wstring folderPath) {
	if (!pathExists(folderPath)) {
		return FSOpResult::Fail;
	}
	bool isfld = false;
	if (FSOpResult::Success == isFolder(isfld, folderPath)) {
		if (!isfld) {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
	folderSize = 0;
	std::vector<FileRecord> filerecs = SeekFileInDir(folderPath, L".*", L"", true, false);
	for (size_t i = 0; i < filerecs.size(); ++i) {
		folderSize += filerecs[i].size;
	}
	return FSOpResult::Success;
}

FSOpResult FSHandler::GetFSizeOnDrive_NtQueryDir(unsigned long long &fileSize, const std::wstring filePath) const {
	if (!pathExists(filePath)) {
		return FSOpResult::Fail;
	}
	bool isfld = false;
	if (FSOpResult::Success == isFolder(isfld, filePath)) {
		if (isfld) {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
	std::vector<std::wstring> filePathSpl = splitStr(filePath, L"\\");
	if (2 > filePathSpl.size()) {
		return FSOpResult::Fail;
	}
	std::string filename = wstr2str(filePathSpl.back());
	filePathSpl.pop_back();
	std::wstring ownerfolder = joinStrs(filePathSpl, L"\\");
	::HMODULE hModule = ::LoadLibrary(L"ntdll.dll");
	if (!hModule) {
		return FSOpResult::Fail;
	}
	pRtlInitUnicodeString RtlInitUnicodeString = (pRtlInitUnicodeString)
		::GetProcAddress(hModule, "RtlInitUnicodeString");
	pNtCreateFile NtCreateFile = (pNtCreateFile)
		::GetProcAddress(hModule, "NtCreateFile");
	pNtCreateEvent NtCreateEvent = (pNtCreateEvent)
		::GetProcAddress(hModule, "NtCreateEvent");
	pNtQueryDirectoryFile NtQueryDirectoryFile = (pNtQueryDirectoryFile)
		::GetProcAddress(hModule, "NtQueryDirectoryFile");
	pNtWaitForSingleobject NtWaitForSingleobject = (pNtWaitForSingleobject)
		::GetProcAddress(hModule, "NtWaitForSingleObject");
	pRtlUnicodeStringToAnsiString RtlUnicodeStringToAnsiString = (pRtlUnicodeStringToAnsiString)
		::GetProcAddress(hModule, "RtlUnicodeStringToAnsiString");
	pNtClearEvent NtClearEvent = (pNtClearEvent) ::GetProcAddress(hModule, "NtClearEvent");
	pNtClose NtClose = (pNtClose) ::GetProcAddress(hModule, "NtClose");
	if (0 == RtlInitUnicodeString) {
		return FSOpResult::Fail;
	}
	if (0 == RtlUnicodeStringToAnsiString) {
		return FSOpResult::Fail;
	}
	if (0 == NtCreateFile) {
		return FSOpResult::Fail;
	}
	if (0 == NtCreateEvent) {
		return FSOpResult::Fail;
	}
	if (0 == NtQueryDirectoryFile) {
		return FSOpResult::Fail;
	}
	if (0 == NtWaitForSingleobject) {
		return FSOpResult::Fail;
	}
	if (0 == NtClearEvent) {
		return FSOpResult::Fail;
	}
	if (0 == NtClose) {
		return FSOpResult::Fail;
	}
	::HANDLE RootDirectoryHandle = 0;
	::HANDLE Event = 0;
	NTSTATUS ntStatus = STATUS_SUCCESS;
	UnicodeString RootDirectoryName;
	ANSIString as;
	ObjectAttributes RootDirectoryAttributes;
	IOStatusBlock IOSB;
	FileDirBothInformation* DirInformation;
	wchar_t* wszBuffer = (wchar_t*)malloc(64 * sizeof(wchar_t));
	if (!wszBuffer) {
		return FSOpResult::Fail;
	}
	wchar_t* Buffer = (wchar_t*)malloc(65536 * sizeof(wchar_t));
	if (!Buffer) {
		return FSOpResult::Fail;
	}
	wsprintf(wszBuffer, L"\\??\\%s\\", ownerfolder.c_str());
	ntStatus = ((RtlInitUnicodeString)(&RootDirectoryName, wszBuffer));
	if (!NT_SUCCESS(ntStatus)) {
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	InitializeObjectAttributes(&RootDirectoryAttributes, &RootDirectoryName, OBJ_CASE_INSENSITIVE, 0, 0);
	ntStatus = ((NtCreateFile)(&RootDirectoryHandle,
		GENERIC_READ,
		&RootDirectoryAttributes,
		&IOSB,
		0,
		FILE_ATTRIBUTE_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_DIRECTORY_FILE,
		0, 0));
	if (!NT_SUCCESS(ntStatus)) {
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	ntStatus = ((NtCreateEvent)(&Event, GENERIC_ALL, 0, EvtType::NotificationEvent, 0));
	if (!NT_SUCCESS(ntStatus)) {
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	ntStatus = ((NtQueryDirectoryFile)(RootDirectoryHandle,
		Event, 0, 0,
		&IOSB,
		Buffer,
		65536 * sizeof(unsigned char),
		FolderInfo::FileDirectoryBothInformation,
		0, 0, 0));
	if (STATUS_PENDING == ntStatus) {
		ntStatus = (NtWaitForSingleobject)(Event, 1, 0);
	}
	if (!NT_SUCCESS(ntStatus)) {
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	DirInformation = (FileDirBothInformation*)Buffer;
	UnicodeString EntryName;
	while (true) {
		EntryName.MaximumLength = EntryName.Length = (unsigned short)DirInformation->FileNameLength;
		EntryName.Buffer = &DirInformation->FileName[0];
		((pRtlUnicodeStringToAnsiString)(&as, &EntryName, 1));
		if (DirInformation->FileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		} else {
			if (!stricmp(as.Buffer, filename.c_str())) {
				fileSize = DirInformation->AllocationSize.QuadPart;
				break;
			}
		}
		if (!DirInformation->NextEntryOffset) {
			break;
		} else {
			DirInformation = (FileDirBothInformation*)(((unsigned char*)DirInformation) + DirInformation->NextEntryOffset);
		}
	}
	((pNtClose)(RootDirectoryHandle));
	ntStatus = ((NtClearEvent)(Event));
	if (!NT_SUCCESS(ntStatus)) {
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	((NtClose)(Event));
	SAFE_FREE(wszBuffer);
	SAFE_FREE(Buffer);
	if (!::FreeLibrary(hModule)) {
		return FSOpResult::Fail;
	}
	return FSOpResult::Success;
}

FSOpResult FSHandler::GetObjectSecurity(SecDesc &secDesc, const std::wstring objectPath) const {
	if (!pathExists(objectPath)) {
		return FSOpResult::Fail;
	}
	ProcessHandler proc;
	unsigned long procid = proc.GetCurrentProcPid();
	std::vector<std::wstring> privs = proc.GetProcPrivileges(procid);
	if (!valInList(privs, L"SeSecurityPrivilege")) {
		if (!proc.EnableSecurityPrivilege(procid)) {
			return FSOpResult::Fail;
		}
	}
	ACLHandler aclh;
	SysHandler sys;
	unsigned long secinfolen = 0;
	if (!::GetFileSecurity(objectPath.c_str(), static_cast<unsigned long>(SecInfo::DACLSecInfo), secDesc.daclInfo,
		secinfolen, &secinfolen)) {
		if (ERROR_INSUFFICIENT_BUFFER == getLastErrorCode()) {
			::SECURITY_DESCRIPTOR* tsd = (::SECURITY_DESCRIPTOR*)::LocalAlloc(LPTR, secinfolen);
			if (!tsd) {
				return FSOpResult::Fail;
			}
			secDesc.daclInfo = ::LocalAlloc(LPTR, secinfolen);
			if (secDesc.daclInfo) {
				if (!::GetFileSecurity(objectPath.c_str(), static_cast<unsigned long>(SecInfo::DACLSecInfo),
					tsd, secinfolen, &secinfolen)) {
					SAFE_LOCALFREE(secDesc.daclInfo);
					return FSOpResult::Fail;
				}
				memcpy(secDesc.daclInfo, tsd, secinfolen);
				SAFE_LOCALFREE(tsd);
				secDesc.daclInfoSz = secinfolen;
			} else {
				return FSOpResult::Fail;
			}
		}
	}
	secinfolen = 0;
	if (!::GetFileSecurity(objectPath.c_str(), static_cast<unsigned long>(SecInfo::SACLSecInfo), secDesc.saclInfo,
		secinfolen, &secinfolen)) {
		if (ERROR_INSUFFICIENT_BUFFER == getLastErrorCode()) {
			secDesc.saclInfo = ::LocalAlloc(LPTR, secinfolen);
			if (secDesc.saclInfo) {
				if (!::GetFileSecurity(objectPath.c_str(), static_cast<unsigned long>(SecInfo::SACLSecInfo),
					secDesc.saclInfo, secinfolen, &secinfolen)) {
					SAFE_LOCALFREE(secDesc.daclInfo);
					return FSOpResult::Fail;
				}
				secDesc.saclInfoSz = secinfolen;
			} else {
				return FSOpResult::Fail;
			}
		}
	}
	secinfolen = 0;
	::SECURITY_DESCRIPTOR* tsd = (::SECURITY_DESCRIPTOR*)::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (!tsd) {
		return FSOpResult::Fail;
	}
	if (!::GetFileSecurity(objectPath.c_str(), static_cast<unsigned long>(SecInfo::OwnerSecInfo), tsd, secinfolen,
		&secinfolen)) {
		if (ERROR_INSUFFICIENT_BUFFER == getLastErrorCode()) {
			if (!::GetFileSecurity(objectPath.c_str(),static_cast<unsigned long>(SecInfo::OwnerSecInfo),
				tsd, secinfolen, &secinfolen)) {
				SAFE_LOCALFREE(tsd);
				return FSOpResult::Fail;
			}
			::PSID tsid = 0;
			if (ACLOpResult::Success != aclh.OwnerSIDFromSecurityDescriptor(tsd, tsid)) {
				return FSOpResult::Fail;
			}
			secDesc.ownerInfo = sys.StrSIDFromSID(tsid);
			// std::wcout << L"xxx " << sys.GetAccountNameFromStrSID(secDesc.ownerInfo) << std::endl;
			secDesc.ownerInfoSz = secDesc.ownerInfo.length() * sizeof(wchar_t);
		}
	}
	SAFE_LOCALFREE(tsd);
	secinfolen = 0;
	tsd = (::SECURITY_DESCRIPTOR*)::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (!tsd) {
		return FSOpResult::Fail;
	}
	if (!::GetFileSecurity(objectPath.c_str(), static_cast<unsigned long>(SecInfo::GroupSecInfo), tsd, secinfolen,
		&secinfolen)) {
		if (ERROR_INSUFFICIENT_BUFFER == getLastErrorCode()) {
			if (!::GetFileSecurity(objectPath.c_str(), static_cast<unsigned long>(SecInfo::GroupSecInfo),
				tsd, secinfolen, &secinfolen)) {
				SAFE_LOCALFREE(tsd);
				return FSOpResult::Fail;
			}
			::PSID tsid = 0;
			if (ACLOpResult::Success != aclh.PrimaryGroupSIDFromSecurityDescriptor(tsd, tsid)) {
				return FSOpResult::Fail;
			}
			std::wstring sidname = sys.GetAccountNameFromSID(tsid);
			if((L"отсутствует" != lower_copy(sidname)) && (L"empty" != lower_copy(sidname)) &&
				(L"missing" != lower_copy(sidname))) {
				secDesc.primaryGroupInfo = sys.StrSIDFromSID(tsid);
				secDesc.primaryGroupInfoSz = secDesc.primaryGroupInfo.length() * sizeof(wchar_t);
			} else {
				secDesc.primaryGroupInfo = L"";
				secDesc.primaryGroupInfoSz = 0;
			}
			SAFE_LOCALFREE(tsd);
		}
	}
	if (ACLOpResult::Success != aclh.CreateAbsoluteSecDesc(secDesc)) {
		return FSOpResult::Fail;
	}
	return FSOpResult::Success;
}

FSOpResult FSHandler::SetObjectSecurity(const SecDesc secDesc, const std::wstring objectPath) const {
	if (!pathExists(objectPath)) {
		return FSOpResult::Fail;
	}
	ProcessHandler proc;
	unsigned long procid = proc.GetCurrentProcPid();
	std::vector<std::wstring> privs = proc.GetProcPrivileges(procid);
	if (!valInList(privs, L"SeBackupPrivilege")) {
		if (!proc.EnableBackupPrivilege(procid)) {
			return FSOpResult::Fail;
		}
	}
	if (!valInList(privs, L"SeRestorePrivilege")) {
		if (!proc.EnableRestorePrivilege(procid)) {
			return FSOpResult::Fail;
		}
	}
	if (!valInList(privs, L"SeSecurityPrivilege")) {
		if (!proc.EnableSecurityPrivilege(procid)) {
			return FSOpResult::Fail;
		}
	}
	if (!valInList(privs, L"SeTakeOwnershipPrivilege")) {
		if (!proc.EnableTakeOwnershipPrivilege(procid)) {
			return FSOpResult::Fail;
		}
	}
	ACLHandler aclh;
	if (secDesc.daclAbsInfo) {
		::ACL* acllist = 0;
		if (ACLOpResult::Success != aclh.DACLFromSecurityDescriptor(
			(::SECURITY_DESCRIPTOR*)secDesc.absoluteSDInfo, acllist)) {
			return FSOpResult::Fail;
		}
		::SECURITY_DESCRIPTOR* sd = (::SECURITY_DESCRIPTOR*)::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		if (!sd) {
			SAFE_LOCALFREE(acllist);
			return FSOpResult::Fail;
		}
		if (::InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION)) {
			if (!::SetSecurityDescriptorDacl(sd, true, acllist, false)) {
				SAFE_LOCALFREE(sd);
				SAFE_LOCALFREE(acllist);
				return FSOpResult::Fail;
			}
		} else {
			SAFE_LOCALFREE(sd);
			SAFE_LOCALFREE(acllist);
			return FSOpResult::Fail;
		}
		if (!::SetFileSecurity(objectPath.c_str(), static_cast<unsigned long>(SecInfo::DACLSecInfo), sd)) {
			SAFE_LOCALFREE(sd);
			SAFE_LOCALFREE(acllist);
			return FSOpResult::Fail;
		}
		SAFE_LOCALFREE(sd);
		SAFE_LOCALFREE(acllist);
	}
	if (secDesc.saclAbsInfo) {
		::ACL* acllist = 0;
		if (ACLOpResult::Success != aclh.SACLFromSecurityDescriptor((::SECURITY_DESCRIPTOR*)secDesc.absoluteSDInfo,
			acllist)) {
			return FSOpResult::Fail;
		}
		if (acllist) {
			::SECURITY_DESCRIPTOR* sd = (::SECURITY_DESCRIPTOR*)::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
			if (!sd) {
				SAFE_LOCALFREE(acllist);
				return FSOpResult::Fail;
			}
			if (::InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION)) {
				if (!::SetSecurityDescriptorSacl(sd, true, acllist, false)) {
					SAFE_LOCALFREE(sd);
					SAFE_LOCALFREE(acllist);
					return FSOpResult::Fail;
				}
			} else {
				SAFE_LOCALFREE(sd);
				SAFE_LOCALFREE(acllist);
				return FSOpResult::Fail;
			}
			if (!::SetFileSecurity(objectPath.c_str(), static_cast<unsigned long>(SecInfo::SACLSecInfo), sd)) {
				SAFE_LOCALFREE(sd);
				SAFE_LOCALFREE(acllist);
				return FSOpResult::Fail;
			}
			SAFE_LOCALFREE(sd);
			SAFE_LOCALFREE(acllist);
		}
	}
	if (secDesc.ownerInfo.length()) {
		::SECURITY_DESCRIPTOR* sd = (::SECURITY_DESCRIPTOR*)::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		if (!sd) {
			return FSOpResult::Fail;
		}
		::PSID tsid = 0;
		if (::InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION)) {
			int ownerDefaulted = 0;
			SysHandler sys;
			tsid = sys.SIDFromStrSid(secDesc.ownerInfo);
			if (!tsid) {
				return FSOpResult::Fail;
			}
			if (!::SetSecurityDescriptorOwner(sd, tsid, ownerDefaulted)) {
				SAFE_LOCALFREE(tsid);
				return FSOpResult::Fail;
			}
		} else {
			SAFE_LOCALFREE(sd);
			return FSOpResult::Fail;
		}
		if (!::SetFileSecurity(objectPath.c_str(), static_cast<unsigned long>(SecInfo::OwnerSecInfo), sd)) {
			return FSOpResult::Fail;
		}
		SAFE_LOCALFREE(sd);
		SAFE_LOCALFREE(tsid);
	}
	if (secDesc.primaryGroupInfo.length()) {
		::SECURITY_DESCRIPTOR* sd = (::SECURITY_DESCRIPTOR*)::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		if (!sd) {
			return FSOpResult::Fail;
		}
		::PSID tsid = 0;
		if (::InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION)) {
			int groupDefaulted = 0;
			SysHandler sys;
			tsid = sys.SIDFromStrSid(secDesc.primaryGroupInfo);
			if (!tsid) {
				return FSOpResult::Fail;
			}
			if (!::SetSecurityDescriptorOwner(sd, tsid, groupDefaulted)) {
				SAFE_LOCALFREE(tsid);
				return FSOpResult::Fail;
			}
		} else {
			SAFE_LOCALFREE(sd);
			return FSOpResult::Fail;
		}
		if (!::SetFileSecurity(objectPath.c_str(), static_cast<unsigned long>(SecInfo::GroupSecInfo), sd)) {
			return FSOpResult::Fail;
		}
		SAFE_LOCALFREE(sd);
		SAFE_LOCALFREE(tsid);
	}
	return FSOpResult::Success;
}

FSOpResult FSHandler::WriteToTextFile(const std::wstring textFilename, const std::wstring data,
	const TextFileEnc fileEncpding) {
	unsigned long cdisp = 0;
	if (pathExists(textFilename)) {
		if (!::DeleteFile(textFilename.c_str())) {
			return FSOpResult::Fail;
		}
	}
	::HANDLE hFile = ::CreateFile(textFilename.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile) {
		return FSOpResult::Fail;
	}
	char* outBuf = 0;
	if (TextFileEnc::UTF8 == fileEncpding) {
		outBuf = (char*)malloc((data.length() + 2) * sizeof(char));
	} else if (TextFileEnc::UTF16LE == fileEncpding) {
		outBuf = (char*)malloc((data.length() + 2) * sizeof(wchar_t));
	} else {
		return FSOpResult::Fail;
	}
	if (!outBuf) {
		return FSOpResult::Fail;
	}
	unsigned long numbytes2write = 0, numbyteswritten = 0;
	if (TextFileEnc::UTF8 == fileEncpding) {
		memset(outBuf, 0, (data.length() + 2) * sizeof(char));
		numbytes2write = data.length() * sizeof(char);
		sprintf(outBuf, "%s", wstr2str(data).c_str());
	} else if (TextFileEnc::UTF16LE == fileEncpding) {
		memset(outBuf, 0, (data.length() + 2) * sizeof(wchar_t));
		numbytes2write = data.length() * sizeof(wchar_t);
		wsprintf((wchar_t*)outBuf, L"%s", data.c_str());
		// memcpy(outBuf, data.c_str(), data.length() * sizeof(wchar_t));
	}
	if (!::WriteFile(hFile, outBuf, numbytes2write, &numbyteswritten, 0)) {
		SAFE_FREE(outBuf);
		::CloseHandle(hFile); 
		return FSOpResult::Fail;
	}
	SAFE_FREE(outBuf);
	::CloseHandle(hFile);
	return FSOpResult::Success;
}

FSOpResult FSHandler::EnumFolderContents(FolderRecord &folderInfo, const std::wstring folderPath,
	const bool getFileHashes, const HashType hashType, const bool getFilesize) {
	if (pathExists(folderPath)) {
		std::wstring seekpath = lower_copy(folderPath);
		if (endsWith(folderPath, L"\\")) {
			seekpath = removeFromEnd_copy(lower_copy(folderPath), L"\\");
		} else {
			seekpath = lower_copy(folderPath);
		}
		if (!startsWith(seekpath, fs_pathnolim)) {
			seekpath = fs_pathnolim + seekpath;
		}
		if (!endsWith(seekpath, fs_pathsall)) {
			seekpath = seekpath + fs_pathsall;
		}
		::WIN32_FIND_DATA fd;
		memset(&fd, 0, sizeof(::WIN32_FIND_DATA));
		::HANDLE hFind = FindFirstFile(seekpath.c_str(), &fd);
		if (INVALID_HANDLE_VALUE != hFind) {
			folderInfo.folderName = splitStr(lower_copy(folderPath), L"\\").back();
			folderInfo.folderPath = lower_copy(folderPath);
			std::wstring hashBuf;
			do {
				removeFromEnd(seekpath, fs_pathsall);
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					if (wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..") &&
						wcscmp(fd.cFileName, L"System Volume Information") &&
						wcscmp(fd.cFileName, L"$Recycle.Bin")) {
						FolderRecord folderrec;
						folderrec.folderName = lower_copy(fd.cFileName);
						folderrec.folderPath = removeFromStart_copy(seekpath, fs_pathnolim) + L"\\" +
							lower_copy(fd.cFileName);
						if (FSOpResult::Success != EnumFolderContents(folderrec, folderrec.folderPath, getFileHashes,
							hashType, getFileSize)) {
							return FSOpResult::Fail;
						}
						folderInfo.folders.push_back(folderrec);
					}
				} else {
					FileRecord filerec;
					filerec.fileName = lower_copy(fd.cFileName);
					filerec.filePath = removeFromStart_copy(seekpath, fs_pathnolim) + L"\\" +
						lower_copy(fd.cFileName);
					if (getFileHashes) {
						if (FSOpResult::Success == calcFileHash(hashBuf, filerec.filePath, hashType)) {
							filerec.hash = hashBuf;
						}
					}
					if (getFilesize) {
						if (FSOpResult::Success != getFileSize(filerec.size, filerec.filePath)) {
							return FSOpResult::Fail;
						}
					}
					folderInfo.files.push_back(filerec);
				}
			} while (FindNextFile(hFind, &fd));
			FindClose(hFind);
		}
	} else {
		return FSOpResult::Fail;
	}
	return FSOpResult::Success;
}

std::vector<FileRecord> FSHandler::SeekFile(const std::wstring filename, const bool getSize,
	const bool getControlSum, const bool excludeEmptyFiles, const HashType hash,
	const std::vector<std::wstring> exclusions, const std::vector<VolumeDesc>* parts) {
	std::vector<FileRecord> ret;
	std::vector<VolumeDesc> drives;
	if (parts) {
		drives = *parts;
	} else {
		if (FSOpResult::Success != EnumVolumes(drives)) {
			return ret;
		}
	}
	std::vector<std::wstring> exclLow = exclusions;
	for (size_t i = 0; i < exclLow.size(); ++i) {
		exclLow[i] = lower_copy(exclLow[i]);
		if (endsWith(exclLow[i], L"\\")) {
			removeFromEnd(exclLow[i], L"\\");
		}
	}
	std::basic_regex<wchar_t> fbr(prepRegexString_copy(filename));
	std::wstring path;
	std::wstring seekpath;
	std::wstring fnamelow = lower_copy(filename);
	::WIN32_FIND_DATA fd;
	::LARGE_INTEGER filesize;
	::HANDLE hFind = INVALID_HANDLE_VALUE;
	for (size_t i = 0; i < drives.size(); ++i) {
		memset(&fd, 0, sizeof(fd));
		if (endsWith(drives[i].partLetter, L"\\")) {
			path = removeFromEnd_copy(drives[i].partLetter, L"\\", false);
			seekpath = removeFromEnd_copy(lower_copy(drives[i].partLetter), L"\\");
		} else {
			path = drives[i].partLetter;
			seekpath = lower_copy(drives[i].partLetter);
		}
		if (!startsWith(seekpath, fs_pathnolim)) {
			seekpath = fs_pathnolim + seekpath;
		}
		if (!endsWith(seekpath, fs_pathsall)) {
			seekpath = seekpath + fs_pathsall;
		}
		std::wstring seekpathraw = removeFromEnd_copy(removeFromStart_copy(seekpath, fs_pathnolim), fs_pathsall);
		if (endsWith(seekpathraw, L"\\")) {
			removeFromEnd(seekpathraw, L"\\");
		}
		hFind = ::FindFirstFile(seekpath.c_str(), &fd);
		if (INVALID_HANDLE_VALUE != hFind) {
			std::wstring hashBuf;
			do {
				removeFromEnd(seekpath, fs_pathsall);
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					if (wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..") &&
						wcscmp(fd.cFileName, L"System Volume Information") &&
						wcscmp(fd.cFileName, L"$Recycle.Bin")) {
						if (!valInList(exclLow, seekpathraw + L"\\" + lower_copy(std::wstring(fd.cFileName))) ||
							!valInList(exclLow, seekpathraw)) {
							if (!startsWith(seekpath, fs_pathnolim)) {
								seekpath = fs_pathnolim + seekpath;
							}
							std::vector<FileRecord> searchRes = SeekFileRecursive(seekpath + L"\\" +
								lower_copy(fd.cFileName) + fs_pathsall, path + L"\\" + std::wstring(fd.cFileName),
								fnamelow, fbr, getSize, getControlSum, excludeEmptyFiles,
								hash, &exclLow);
							ret.insert(ret.end(), searchRes.begin(), searchRes.end());
						}
					}
				} else {
					if (std::regex_match(fd.cFileName, fbr) ||
						std::regex_match(lower_copy(fd.cFileName), fbr) ||
						(fnamelow == lower_copy(fd.cFileName))) {
						if (!valInList(exclLow, seekpathraw + L"\\" + lower_copy(std::wstring(fd.cFileName))) ||
							!valInList(exclLow, seekpathraw)) {
							FileRecord searchres;
							searchres.fileName = lower_copy(fd.cFileName);
							searchres.filePath = path + L"\\" + fd.cFileName;
							if (getSize) {
								getFileSize(searchres.size, searchres.filePath);
							}
							if (getControlSum) {
								if (FSOpResult::Success == calcFileHash(hashBuf, searchres.filePath, hash)) {
									searchres.hash = hashBuf;
								}
							}
							if (excludeEmptyFiles) {
								filesize.LowPart = fd.nFileSizeLow;
								filesize.HighPart = fd.nFileSizeHigh;
								if (filesize.QuadPart > 0) {
									ret.push_back(searchres);
								}
							} else {
								ret.push_back(searchres);
							}
						}
					}
				}
			} while (FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
	}
	return ret;
}

std::vector<FileRecord> FSHandler::SeekFileInDir(const std::wstring startPath,
	const std::wstring filename, const std::wstring pathOrig, const bool getSize, const bool getControlSum,
	const bool excludeEmptyFiles, const HashType hash, const std::vector<std::wstring> &exclusions) {
	std::vector<FileRecord> ret;
	std::basic_regex<wchar_t> fbr(prepRegexString_copy(filename));
	std::wstring path;
	if (pathOrig.length()) {
		path = pathOrig;
	} else {
		path = startPath;
	}
	std::wstring fnamelow = lower_copy(filename);
	std::wstring seekpath = lower_copy(startPath);
	::WIN32_FIND_DATA fd;
	::LARGE_INTEGER filesize;
	::HANDLE hFind = INVALID_HANDLE_VALUE;
	memset(&fd, 0, sizeof(fd));
	if (!startsWith(seekpath, fs_pathnolim)) {
		seekpath = fs_pathnolim + seekpath;
	}
	if (!endsWith(seekpath, fs_pathsall)) {
		seekpath = seekpath + fs_pathsall;
	}
	std::wstring seekpathraw = removeFromEnd_copy(removeFromStart_copy(seekpath, fs_pathnolim), fs_pathsall);
	if (endsWith(seekpathraw, L"\\")) {
		removeFromEnd(seekpathraw, L"\\");
	}
	hFind = ::FindFirstFile(seekpath.c_str(), &fd);
	if (INVALID_HANDLE_VALUE != hFind) {
		std::wstring hashBuf;
		do {
			removeFromEnd(seekpath, fs_pathsall);
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..") &&
					wcscmp(fd.cFileName, L"System Volume Information") &&
					wcscmp(fd.cFileName, L"$Recycle.Bin") && wcscmp(fd.cFileName, L"$RECYCLE.BIN")) {
					if (!valInList(exclusions, seekpathraw + L"\\" + lower_copy(std::wstring(fd.cFileName))) ||
						!valInList(exclusions, seekpathraw)) {
						if (!startsWith(seekpath, fs_pathnolim)) {
							seekpath = fs_pathnolim + seekpath;
						}
						std::vector<FileRecord> searchRes = SeekFileInDir(seekpath + L"\\" + lower_copy(fd.cFileName) + fs_pathsall,
							fnamelow, path + L"\\" + std::wstring(fd.cFileName), getSize, getControlSum, excludeEmptyFiles, hash, exclusions);
						ret.insert(ret.end(), searchRes.begin(), searchRes.end());
					}
				}
			} else {
				if (std::regex_match(fd.cFileName, fbr) ||
					std::regex_match(lower_copy(fd.cFileName), fbr) ||
					(fnamelow == lower_copy(fd.cFileName))) {
					if (!valInList(exclusions, seekpathraw + L"\\" + lower_copy(std::wstring(fd.cFileName))) ||
						!valInList(exclusions, seekpathraw)) {
						FileRecord searchres;
						searchres.fileName = lower_copy(fd.cFileName);
						searchres.filePath = path + L"\\" + std::wstring(fd.cFileName);
						if (getSize) {
							getFileSize(searchres.size, searchres.filePath);
						}
						if (getControlSum) {
							if (FSOpResult::Success == calcFileHash(hashBuf, searchres.filePath, hash)) {
								searchres.hash = hashBuf;
							}
						}
						if (excludeEmptyFiles) {
							filesize.LowPart = fd.nFileSizeLow;
							filesize.HighPart = fd.nFileSizeHigh;
							if (filesize.QuadPart > 0) {
								ret.push_back(searchres);
							}
						} else {
							ret.push_back(searchres);
						}
					}
				}
			}
		} while (FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return ret;
}

std::vector<FileRecord> FSHandler::SeekFileRecursive(const std::wstring startPath, const std::wstring pathOrig,
	const std::wstring filename, std::basic_regex<wchar_t> searchRegex, const bool getSize,
	const bool getControlSum, const bool excludeEmptyFiles, const HashType hash,
	std::vector<std::wstring>* exclusions) {
	std::vector<FileRecord> ret;
	std::wstring path = pathOrig;
	std::wstring fnamelow = lower_copy(filename);
	std::wstring seekpath = lower_copy(startPath);
	::WIN32_FIND_DATA fd;
	::LARGE_INTEGER filesize;
	::HANDLE hFind = INVALID_HANDLE_VALUE;
	memset(&fd, 0, sizeof(fd));
	if (!startsWith(seekpath, fs_pathnolim)) {
		seekpath = fs_pathnolim + seekpath;
	}
	if (!endsWith(seekpath, fs_pathsall)) {
		seekpath = seekpath + fs_pathsall;
	}
	std::wstring seekpathraw = removeFromEnd_copy(removeFromStart_copy(seekpath, fs_pathnolim), fs_pathsall);
	if (endsWith(seekpathraw, L"\\")) {
		removeFromEnd(seekpathraw, L"\\");
	}
	hFind = ::FindFirstFile(seekpath.c_str(), &fd);
	if (INVALID_HANDLE_VALUE != hFind) {
		std::wstring hashBuf;
		do {
		removeFromEnd(seekpath, fs_pathsall);
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..") &&
					wcscmp(fd.cFileName, L"System Volume Information") &&
					wcscmp(fd.cFileName, L"$Recycle.Bin")) {
					if (!valInList(*exclusions, seekpathraw + L"\\" + lower_copy(std::wstring(fd.cFileName))) ||
						!valInList(*exclusions, seekpathraw)) {
						if (!startsWith(seekpath, fs_pathnolim)) {
							seekpath = fs_pathnolim + seekpath;
						}
						std::vector<FileRecord> searchRes = SeekFileRecursive(seekpath + L"\\" + lower_copy(fd.cFileName) + fs_pathsall,
							path + L"\\" + std::wstring(fd.cFileName), fnamelow, searchRegex, getSize, getControlSum, excludeEmptyFiles, hash, exclusions);
						ret.insert(ret.end(), searchRes.begin(), searchRes.end());
					}
				}
			} else {
				if (std::regex_match(fd.cFileName, searchRegex) ||
					std::regex_match(lower_copy(fd.cFileName), searchRegex) ||
					(fnamelow == lower_copy(fd.cFileName))) {
					if (!valInList(*exclusions, seekpathraw + L"\\" + lower_copy(std::wstring(fd.cFileName))) ||
						!valInList(*exclusions, seekpathraw)) {
						FileRecord searchres;
						searchres.filePath = path + L"\\" + std::wstring(fd.cFileName);
						if (getSize) {
							getFileSize(searchres.size, searchres.filePath);
						}
						if (getControlSum) {
							if (FSOpResult::Success == calcFileHash(hashBuf, searchres.filePath, hash)) {
								searchres.hash = hashBuf;
							}
						}
						if (excludeEmptyFiles) {
							filesize.LowPart = fd.nFileSizeLow;
							filesize.HighPart = fd.nFileSizeHigh;
							if (filesize.QuadPart > 0) {
								ret.push_back(searchres);
							}
						} else {
							ret.push_back(searchres);
						}
					}
				}
			}
		} while (FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return ret;
}

std::wstring FSHandler::DrivePath2PartLetter(const std::wstring drivePath,
	const bool addSlash, const bool ignoreCase, std::vector<VolumeDesc> *parts) {
	std::vector<VolumeDesc> vec;
	if (parts) {
		vec = *parts;
	} else {
		if (FSOpResult::Fail == EnumVolumes(vec)) {
			return drivePath;
		}
	}
	for (size_t i = 0; i < vec.size(); ++i) {
		if (ignoreCase) {
			if ((lower_copy(vec[i].drivePath) == lower_copy(drivePath)) ||
				(lower_copy(vec[i].drivePath) == lower_copy(drivePath) + L"\\")) {
				return vec[i].partLetter;
			}
		} else {
			if ((vec[i].drivePath == drivePath) ||
				(vec[i].drivePath == drivePath + L"\\")) {
				return vec[i].partLetter;
			}
		}
	}
}

std::wstring FSHandler::ReplaceDrivePathWithPartLetter(const std::wstring path,
	const bool ignoreCase, std::vector<VolumeDesc> *parts) {
	std::vector<VolumeDesc> vec;
	if (parts) {
		vec = *parts;
	} else {
		if (FSOpResult::Fail == EnumVolumes(vec)) {
			return path;
		}
	}
	std::wstring ret = path;
	for (size_t i = 0; i < vec.size(); ++i) {
		if (ignoreCase) {
			if (startsWith(lower_copy(ret), lower_copy(vec[i].drivePath))) {
				if (endsWith(vec[i].partLetter, L"\\")) {
					std::wstring plr = removeSubstr_copy(vec[i].partLetter, L"\\");
					return replaceSubstr_copy(ret, vec[i].drivePath, plr);
				} else {
					return replaceSubstr_copy(ret, vec[i].drivePath, vec[i].partLetter);
				}
			}
		} else {
			if (startsWith(ret, vec[i].drivePath)) {
				if (endsWith(vec[i].partLetter, L"\\")) {
					std::wstring plr = removeSubstr_copy(vec[i].partLetter, L"\\", false);
					return replaceSubstr_copy(ret, vec[i].drivePath, plr, false);
				} else {
					return replaceSubstr_copy(ret, vec[i].drivePath, vec[i].partLetter, false);
				}
			}
		}
	}
	return ret;
}

FSOpResult FSHandler::GetFolderSizeOnDriveRec(unsigned long long &folderSize, const std::wstring folderPath,
	::HMODULE ntDLLModule) {
	if (!pathExists(folderPath)) {
		return FSOpResult::Fail;
	}
	bool isfld = false;
	if (FSOpResult::Success == isFolder(isfld, folderPath)) {
		if (!isfld) {
			return FSOpResult::Fail;
		}
	} else {
		return FSOpResult::Fail;
	}
	pRtlInitUnicodeString RtlInitUnicodeString = (pRtlInitUnicodeString)
		::GetProcAddress(ntDLLModule, "RtlInitUnicodeString");
	pNtCreateFile NtCreateFile = (pNtCreateFile)
		::GetProcAddress(ntDLLModule, "NtCreateFile");
	pNtCreateEvent NtCreateEvent = (pNtCreateEvent)
		::GetProcAddress(ntDLLModule, "NtCreateEvent");
	pNtQueryDirectoryFile NtQueryDirectoryFile = (pNtQueryDirectoryFile)
		::GetProcAddress(ntDLLModule, "NtQueryDirectoryFile");
	pNtWaitForSingleobject NtWaitForSingleobject = (pNtWaitForSingleobject)
		::GetProcAddress(ntDLLModule, "NtWaitForSingleObject");
	pRtlUnicodeStringToAnsiString RtlUnicodeStringToAnsiString = (pRtlUnicodeStringToAnsiString)
		::GetProcAddress(ntDLLModule, "RtlUnicodeStringToAnsiString");
	pNtClearEvent NtClearEvent = (pNtClearEvent)
		::GetProcAddress(ntDLLModule, "NtClearEvent");
	pNtClose NtClose = (pNtClose) ::GetProcAddress(ntDLLModule, "NtClose");
	if (0 == RtlInitUnicodeString) {
		return FSOpResult::Fail;
	}
	if (0 == RtlUnicodeStringToAnsiString) {
		return FSOpResult::Fail;
	}
	if (0 == NtCreateFile) {
		return FSOpResult::Fail;
	}
	if (0 == NtCreateEvent) {
		return FSOpResult::Fail;
	}
	if (0 == NtQueryDirectoryFile) {
		return FSOpResult::Fail;
	}
	if (0 == NtWaitForSingleobject) {
		return FSOpResult::Fail;
	}
	if (0 == NtClearEvent) {
		return FSOpResult::Fail;
	}
	if (0 == NtClose) {
		return FSOpResult::Fail;
	}
	::HANDLE RootDirectoryHandle = 0;
	::HANDLE Event = 0;
	NTSTATUS ntStatus = STATUS_SUCCESS;
	UnicodeString RootDirectoryName;
	ANSIString as;
	ObjectAttributes RootDirectoryAttributes;
	IOStatusBlock IOSB;
	FileDirBothInformation* DirInformation;
	wchar_t* wszBuffer = (wchar_t*)malloc(64 * sizeof(wchar_t));
	if (!wszBuffer) {
		return FSOpResult::Fail;
	}
	wchar_t* Buffer = (wchar_t*)malloc(65536 * sizeof(wchar_t));
	if (!Buffer) {
		return FSOpResult::Fail;
	}
	wsprintf(wszBuffer, L"\\??\\%s\\", folderPath.c_str());
	ntStatus = ((RtlInitUnicodeString)(&RootDirectoryName, wszBuffer));
	if (!NT_SUCCESS(ntStatus)) {
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	InitializeObjectAttributes(&RootDirectoryAttributes, &RootDirectoryName, OBJ_CASE_INSENSITIVE, 0, 0);
	ntStatus = ((NtCreateFile)(&RootDirectoryHandle,
		GENERIC_READ,
		&RootDirectoryAttributes,
		&IOSB,
		0,
		FILE_ATTRIBUTE_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_DIRECTORY_FILE,
		0, 0));
	if (!NT_SUCCESS(ntStatus)) {
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	ntStatus = ((NtCreateEvent)(&Event, GENERIC_ALL, 0, EvtType::NotificationEvent, 0));
	if (!NT_SUCCESS(ntStatus)) {
		((NtClose)(RootDirectoryHandle));
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	ntStatus = ((NtQueryDirectoryFile)(RootDirectoryHandle,
		Event, 0, 0,
		&IOSB,
		Buffer,
		65536 * sizeof(unsigned char),
		FolderInfo::FileDirectoryBothInformation,
		0, 0, 0));
	if (STATUS_PENDING == ntStatus) {
		ntStatus = ((NtWaitForSingleobject)(Event, 1, 0));
	}
	if (!NT_SUCCESS(ntStatus)) {
		((NtClose)(RootDirectoryHandle));
		((NtClearEvent)(Event));
		((NtClose)(Event));
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	DirInformation = (FileDirBothInformation*)Buffer;
	UnicodeString EntryName;
	while (true) {
		EntryName.MaximumLength = EntryName.Length = (unsigned short)DirInformation->FileNameLength;
		EntryName.Buffer = &DirInformation->FileName[0];
		((pRtlUnicodeStringToAnsiString)(&as, &EntryName, 1));
		if (DirInformation->FileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// printf("Directory name: ");
			if (strcmp(as.Buffer, ".") && strcmp(as.Buffer, "..")) {
				wchar_t* tbuf = char2wchar(as.Buffer);
				std::wstring targpath = folderPath + L"\\" + tbuf;
				GetFolderSizeOnDriveRec(folderSize, targpath, ntDLLModule);
				SAFE_ARR_DELETE(tbuf);
			}
		} else {
			// printf("Filename: ");
			folderSize += DirInformation->AllocationSize.QuadPart;
		}
		// printf("%s\n", as.Buffer);
		// printf("Directory size: %lld\n", DirInformation->AllocationSize.QuadPart);
		if (!DirInformation->NextEntryOffset) {
			break;
		} else {
			DirInformation = (FileDirBothInformation*)(((unsigned char*)DirInformation) + DirInformation->NextEntryOffset);
		}
	}
	((NtClose)(RootDirectoryHandle));
	ntStatus = ((NtClearEvent)(Event));
	if (!NT_SUCCESS(ntStatus)) {
		((NtClose)(Event));
		SAFE_FREE(wszBuffer);
		SAFE_FREE(Buffer);
		return FSOpResult::Fail;
	}
	((NtClose)(Event));
	SAFE_FREE(wszBuffer);
	SAFE_FREE(Buffer);
	return FSOpResult::Success;
}

FSOpResult FSHandler::GetDriveSpace(const std::wstring partLetter,
	unsigned long long &freeSpace, unsigned long long &totalSpace) {
	std::wstring partLetterMod = partLetter;
	if (!startsWith(partLetterMod, fs_pathnolim)) {
		partLetterMod = fs_pathnolim + partLetter;
	}
	if (!endsWith(partLetterMod, L"\\")) {
		partLetterMod = partLetterMod + L"\\";
	}
	ULARGE_INTEGER fSpace = { 0 }, tSpace = { 0 }, fSpacePU = { 0 };
	if (::GetDiskFreeSpaceEx(partLetterMod.c_str(), &fSpacePU, &tSpace, &fSpace)) {
		freeSpace = fSpace.QuadPart;
		totalSpace = tSpace.QuadPart;
		return FSOpResult::Success;
	} else {
		return FSOpResult::Fail;
	}
}

FSOpResult FSHandler::GetDriveSpace_DriveGeometry(const std::wstring partLetter, unsigned long long &totalSpace) const {
	std::wstring path = lower_copy(partLetter);
	if (endsWith(partLetter, L"\\")) {
		removeFromEnd(path, L"\\");
	}
	if (!startsWith(path, L"\\\\.\\")) {
		path = L"\\\\.\\" + path;
	}
	::HANDLE hDevice = ::CreateFile(path.c_str(),	// drive to open
		0,					// no access to the drive
		FILE_SHARE_READ |	// share mode
		FILE_SHARE_WRITE,
		0,					// default security attributes
		OPEN_ALWAYS,		// disposition
		0,					// file attributes
		NULL);				// do not copy file attributes
	if (INVALID_HANDLE_VALUE != hDevice) {
		unsigned long junk = 0;
		::DISK_GEOMETRY pdg = { 0 };
		bool result = ::DeviceIoControl(hDevice,	// device to be queried
			IOCTL_DISK_GET_DRIVE_GEOMETRY,			// operation to perform
			0, 0,									// no input buffer
			&pdg, sizeof(::DISK_GEOMETRY),			// output buffer
			&junk,									// # bytes returned
			(::LPOVERLAPPED)0);
		::CloseHandle(hDevice);
		if (result) {
			totalSpace = pdg.Cylinders.QuadPart * (unsigned long)pdg.TracksPerCylinder *
				(unsigned long)pdg.SectorsPerTrack * (unsigned long)pdg.BytesPerSector;
			return FSOpResult::Success;
		} else {
			return FSOpResult::Fail;
		}
	}
	return FSOpResult::Fail;
}

std::wstring FSHandler::calcHash(const std::wstring filePath, const HashType hashType, const bool hashUCase) {
	std::wstring ret;
	::HCRYPTPROV hProv = 0;
	if (::CryptAcquireContext(&hProv, 0, 0, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
		::HCRYPTPROV hHash = 0;
		if (::CryptCreateHash(hProv, (ALG_ID)hashType, 0, 0, &hHash)) {
			unsigned long long fsize = 0;
			if (FSOpResult::Success != getFileSize(fsize, filePath)) {
				return L"";
			}
			unsigned char* fileContent = (unsigned char*)malloc(fsize + 1);
			if (!fileContent) {
				return L"";
			}
			memset(fileContent, 0, (fsize + 1) * sizeof(unsigned char));
			if (FSOpResult::Success != file2Buffer(fileContent, filePath, fsize)) {
				return L"";
			}
			if (::CryptHashData(hHash, static_cast<const unsigned char*>(fileContent), fsize, 0)) {
				unsigned long cbHashSize = 0, dwCount = sizeof(DWORD);
				if (::CryptGetHashParam(hHash, HP_HASHSIZE, (unsigned char*)&cbHashSize, &dwCount, 0)) {
					std::vector<unsigned char> buffer(cbHashSize);
					if (::CryptGetHashParam(hHash, HP_HASHVAL, reinterpret_cast<unsigned char*>(&buffer[0]), &cbHashSize, 0)) {
						std::ostringstream oss;
						for (std::vector<unsigned char>::const_iterator iter = buffer.begin(); iter != buffer.end(); ++iter) {
							oss.fill('0');
							oss.width(2);
							oss << std::hex << static_cast<const int>(*iter);
						}
						::CryptDestroyHash(hHash);
						::CryptReleaseContext(hProv, 0);
						SAFE_FREE(fileContent);
						if (hashUCase) {
							return str2wstr(upper_copy(oss.str()));
						} else {
							return str2wstr(oss.str());
						}
					}
					SAFE_FREE(fileContent);
					::CryptDestroyHash(hHash);
					::CryptReleaseContext(hProv, 0);
				}
				SAFE_FREE(fileContent);
				::CryptDestroyHash(hHash);
				::CryptReleaseContext(hProv, 0);
			}
			SAFE_FREE(fileContent);
			::CryptDestroyHash(hHash);
			::CryptReleaseContext(hProv, 0);
		}
		::CryptReleaseContext(hProv, 0);
	}
	return L"";
}

FSOpResult FSHandler::getSHFileOpDesc(const unsigned long msgCode, std::wstring *msgStr) const {
	if (!msgStr || IsBadWritePtr(msgStr)) {
		return FSOpResult::Fail;
	}
	if (!msgCode) {
		*msgStr = L"Succesful operation";
	} else if (DE_SAMEFILE == msgCode) {
		*msgStr = L"The source and destination files are the same file";
	} else if (DE_MANYSRC1DEST == msgCode) {
		*msgStr = L"Multiple file paths were specified in the source buffer, but only one destination file path";
	} else if (DE_DIFFDIR == msgCode) {
		*msgStr = L"Rename operation was specified but the destination path is a different directory Use the move operation instead";
	} else if (DE_ROOTDIR == msgCode) {
		*msgStr = L"The source is a root directory, which cannot be moved or renamed";
	} else if (DE_OPCANCELLED == msgCode) {
		*msgStr = L"The operation was canceled by the user, or silently canceled if the appropriate flags were supplied to SHFileOperation";
	} else if (DE_DESTSUBTREE == msgCode) {
		*msgStr = L"The destination is a subtree of the source";
	} else if (DE_ACCESSDENIEDSRC == msgCode) {
		*msgStr = L"Security settings denied access to the source";
	} else if (DE_PATHTOODEEP == msgCode) {
		*msgStr = L"The source or destination path exceeded or would exceed MAX_PATH";
	} else if (DE_MANYDEST == msgCode) {
		*msgStr = L"The operation involved multiple destination paths, which can fail in the case of a move operation";
	} else if (DE_INVALIDFILES == msgCode) {
		*msgStr = L"The path in the source or destination or both was invalid";
	} else if (DE_DESTSAMETREE == msgCode) {
		*msgStr = L"The source and destination have the same parent folder";
	} else if (DE_FLDDESTISFILE == msgCode) {
		*msgStr = L"The destination path is an existing file";
	} else if (DE_FILEDESTISFLD == msgCode) {
		*msgStr = L"The destination path is an existing folder";
	} else if (DE_FILENAMETOOLONG == msgCode) {
		*msgStr = L"The name of the file exceeds MAX_PATH";
	} else if (DE_DEST_IS_CDROM == msgCode) {
		*msgStr = L"The destination is a read-only DVD, possibly unformatted";
	} else if (DE_DEST_IS_DVD == msgCode) {
		*msgStr = L"The destination is a writable CD-ROM, possibly unformatted";
	} else if (DE_DEST_IS_CDRECORD == msgCode) {
		*msgStr = L"The destination is a writable CD-ROM, possibly unformatted";
	} else if (DE_FILE_TOO_LARGE == msgCode) {
		*msgStr = L"The file involved in the operation is too large for the destination media or file system";
	} else if (DE_SRC_IS_DVD == msgCode) {
		*msgStr = L"The source is a read-only DVD, possibly unformatted";
	} else if (DE_SRC_IS_CDROM == msgCode) {
		*msgStr = L"The source is a read-only CD-ROM, possibly unformatted";
	} else if (DE_ERROR_MAX == msgCode) {
		*msgStr = L"MAX_PATH was exceeded during the operation";
	} else if (DE_UNKNOWN == msgCode) {
		// This error does not occur on Windows Vista and later
		*msgStr = L"An unknown error occurred. This is typically due to an invalid path in the source or destination";
	} else if (DE_ERRORONDEST == msgCode) {
		*msgStr = L"An unspecified error occurred on the destination";
	} else if (DE_CANTRENAME == msgCode) {
		*msgStr = L"Destination is a root directory and cannot be renamed";
	} else {
		*msgStr = L"Unknown code specifed";
	}
	return FSOpResult::Success;
}

FSOpResult FSHandler::attrAnalyzer(bool &isTrue, const unsigned long attr, const std::wstring path) {
	if (pathExists(path)) {
		unsigned long fileattr = ::GetFileAttributes(path.c_str());
		isTrue = attr & fileattr;
		return FSOpResult::Success;
	} else {
		isTrue = false;
		return FSOpResult::Fail;
	}
}