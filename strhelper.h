#ifndef _STRHELPER_H
#define _STRHELPER_H

/*       In loving memory of @pixo $oft       */
#if defined(_WIN32) || defined(_WIN64)
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
#endif

// #include <codecvt>
#if defined(_WIN32) || defined(_WIN64)
	#include <combaseapi.h>
#endif
#include <string>
#include <locale>
#include <algorithm>
#include <cctype>
#include <vector>
#include <map>
#include <regex>
#include "config.h"
#include "wmihelper.h"

size_t wcslen_c(const wchar_t* line);
bool contentInBuf(const char* buf, size_t bufSz);
bool contentInBuf(const wchar_t* buf, size_t bufSz);
wchar_t* char2wchar(const char* linee, wchar_t* buf = 0);
char* wchar2char(const wchar_t* linee, char* buf = 0);
std::string ul2hexstr(const unsigned long val);
std::wstring ul2hexwstr(const unsigned long val);
std::string ull2hexstr(const unsigned long long val);
std::wstring ull2hexwstr(const unsigned long long val);
unsigned long str2ul(const std::string val);
unsigned long wstr2ul(const std::wstring val);
unsigned long long str2ull(const std::string val);
unsigned long long wstr2ull(const std::wstring val);
char* replaceChars(const char* source, const char* replaceWhat, const char* replaceWith,
const size_t srclen = 0, const size_t replacewhatlen = 0, const size_t replacewithlen = 0);
wchar_t* replaceChars(const wchar_t* source, const wchar_t* replaceWhat, const wchar_t* replaceWith,
const size_t srclen = 0, const size_t replacewhatlen = 0, const size_t replacewithlen = 0);
void lower_chars(char* chrs);
void lower_chars(wchar_t* chrs);
char* lower_chars_copy(const char* chrs);
wchar_t* lower_chars_copy(const wchar_t* chrs);
void upper_chars(char* chrs);
void upper_chars(wchar_t* chrs);
char* upper_chars_copy(const char* chrs);
wchar_t* upper_chars_copy(const wchar_t* chrs);
std::string wstr2str(const std::wstring str);
std::wstring str2wstr(const std::string str);
std::string GUID2str(const GUID guid);
std::wstring GUID2wstr(const GUID guid);
#if defined(_WIN32) || defined(_WIN64)
	std::string GUID2strMS(const GUID guid);
	std::wstring GUID2wstrMS(const GUID guid);
#endif
GUID str2GUID(const std::string str);
GUID wstr2GUID(const std::wstring str);
#if defined(_WIN32) || defined(_WIN64)
	GUID str2GUIDMS(const std::string str);
	GUID wstr2GUIDMS(const std::wstring str);
#endif
void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);
std::string ltrim_copy(const std::string s);
std::string rtrim_copy(const std::string s);
std::string trim_copy(const std::string s);
void ltrim(std::wstring &s);
void rtrim(std::wstring &s);
void trim(std::wstring &s);
std::wstring ltrim_copy(const std::wstring s);
std::wstring rtrim_copy(const std::wstring s);
std::wstring trim_copy(const std::wstring s);
void lower(std::string &s);
void lower(std::wstring &s);
void lower(std::vector<std::string> &vec);
void lower(std::vector<std::wstring> &vec);
void upper(std::string &s);
void upper(std::wstring &s);
void upper(std::vector<std::string> &vec);
void upper(std::vector<std::wstring> &vec);
std::string lower_copy(const std::string s);
std::wstring lower_copy(const std::wstring s);
std::vector<std::string> lower_copy(const std::vector<std::string> vec);
std::vector<std::wstring> lower_copy(const std::vector<std::wstring> vec);
std::string upper_copy(const std::string s);
std::wstring upper_copy(const std::wstring s);
std::vector<std::string> upper_copy(const std::vector<std::string> vec);
std::vector<std::wstring> upper_copy(const std::vector<std::wstring> vec);
void prepRegexString(std::string &regexString);
void prepRegexString(std::wstring &regexString);
std::string prepRegexString_copy(const std::string regexString);
std::wstring prepRegexString_copy(const std::wstring regexString);
void removeFromStart(std::string& source, const std::string sequence, const bool ignoreCase = true);
void removeFromStart(std::wstring& source, const std::wstring sequence, const bool ignoreCase = true);
void removeFromEnd(std::string& source, const std::string sequence, const bool ignoreCase = true);
void removeFromEnd(std::wstring& source, const std::wstring sequence, const bool ignoreCase = true);
void removeFromBothSides(std::string& source, const std::string sequence, const bool ignoreCase = true);
void removeFromBothSides(std::wstring& source, const std::wstring sequence, const bool ignoreCase = true);
std::string removeFromStart_copy(const std::string source, const std::string sequence, const bool ignoreCase = true);
std::wstring removeFromStart_copy(const std::wstring source, const std::wstring sequence, const bool ignoreCase = true);
std::string removeFromEnd_copy(const std::string source, const std::string sequence, const bool ignoreCase = true);
std::wstring removeFromEnd_copy(const std::wstring source, const std::wstring sequence, const bool ignoreCase = true);
std::string removeFromBothSides_copy(const std::string source, const std::string sequence, const bool ignoreCase = true);
std::wstring removeFromBothSides_copy(const std::wstring source, const std::wstring sequence, const bool ignoreCase = true);
bool startsWith(const std::string source, const std::string sequence, const bool ignoreCase = true);
bool startsWith(const std::wstring source, const std::wstring sequence, const bool ignoreCase = true);
bool endsWith(const std::string source, const std::string sequence, const bool ignoreCase = true);
bool endsWith(const std::wstring source, const std::wstring sequence, const bool ignoreCase = true);
bool partialMatch(const std::string source, const std::string compare, const bool ignoreCase = true, size_t *position = 0);
bool partialMatch(const std::wstring source, const std::wstring compare, const bool ignoreCase = true,
	size_t *position = 0);
size_t countOccurence(const std::string source, const std::string sequence);
size_t countOccurence(const std::wstring source, const std::wstring sequence);
std::vector<std::string> splitStr(const std::string str, const char* delimiter = "?", const bool includeEmpty = false);
std::vector<std::wstring> splitStr(const std::wstring str, const wchar_t* delimiter = L"?", const bool includeEmpty = false);
std::map<std::string, std::vector<std::string>> splitStr(const std::string str,
	const std::vector<const char*> delimiters, const bool ignoreCase = true, const bool includeEmpty = false);
std::map<std::wstring, std::vector<std::wstring>> splitStr(const std::wstring str,
	const std::vector<const wchar_t*> delimiters, const bool ignoreCase = true, const bool includeEmpty = false);
std::map<size_t, std::string> tokenFromString(const std::string source, const std::string separator,
	const bool includeSeparators = true, const bool ignoreCase = true, const bool uniqueValues = false);
std::map<size_t, std::wstring> tokenFromString(const std::wstring source, const std::wstring separator,
	const bool includeSeparators = true, const bool ignoreCase = true, const bool uniqueValues = false);
std::string joinStrs(const std::vector<std::string> strs, const std::string delimiter = "",
	const size_t startIdx = 0, const size_t endIdx = 0);
std::wstring joinStrs(const std::vector<std::wstring> strs, const std::wstring delimiter = L"",
	const size_t startIdx = 0, const size_t endIdx = 0);
std::string reverseJoinStrs(const std::vector<std::string> strs, const std::string delimiter = "",
	const size_t startIdx = 0, const size_t endIdx = 0);
std::wstring reverseJoinStrs(const std::vector<std::wstring> strs, const std::wstring delimiter = L"",
	const size_t startIdx = 0, const size_t endIdx = 0);
void removeSubstr(std::string &str, const std::string substr, const bool ignoreCase = true);
void removeSubstr(std::wstring &str, const std::wstring substr, const bool ignoreCase = true);
void removeSubstr(std::string &str, const std::vector<std::string> sequences, const bool ignoreCase = true);
std::string removeSubstr_copy(const std::string str, const std::string substr, const bool ignoreCase = true);
std::wstring removeSubstr_copy(const std::wstring str, const std::wstring substr, const bool ignoreCase = true);
std::string removeSubstr_copy(const std::string str, const std::vector <std::string> sequences, const bool ignoreCase = true);
std::wstring removeSubstr_copy(const std::wstring str, const std::vector <std::wstring> sequences, const bool ignoreCase = true);
std::string replaceAll(const std::string source, const std::string replaceWhat, const  std::string replaceWith);
std::wstring replaceAll(const std::wstring source, const std::wstring replaceWhat, const  std::wstring replaceWith);
void replaceSubstr(std::string& source, const std::string replaceWhat, const std::string replaceWith, const bool ignoreCase = true);
void replaceSubstr(std::wstring& source, const std::wstring replaceWhat, const std::wstring replaceWith, const bool ignoreCase = true);
std::string replaceSubstr_copy(const std::string source, const std::string replaceWhat, const std::string replaceWith,
	const bool ignoreCase = true);
std::wstring replaceSubstr_copy(const std::wstring source, const std::wstring replaceWhat, const std::wstring replaceWith,
	const bool ignoreCase = true);
void reverseString(std::string &str);
void reverseString(std::wstring &str);
std::string reverseString_copy(const std::string str);
std::wstring reverseString_copy(const std::wstring str);
void reverseIPV4(std::string &ipaddrstr);
void reverseIPV4(std::wstring &ipaddrstr);
std::string reverseIPV4_copy(const std::string ipaddrstr);
std::wstring reverseIPV4_copy(const std::wstring ipaddrstr);
void reverseIPV6(std::string &ipaddrstr);
void reverseIPV6(std::wstring &ipaddrstr);
std::string reverseIPV6_copy(const std::string ipaddrstr);
std::wstring reverseIPV6_copy(const std::wstring ipaddrstr);
std::string firstNumberFromString(const std::string line);
std::wstring firstNumberFromString(const std::wstring line);
std::string wmiObjectFromQuery(const std::string query);
std::wstring wmiObjectFromQuery(const std::wstring query);
std::string wmiQuerySimpleFromObject(const std::string objectName,
	const std::string namespacePath = "ROOT\\CIMV2", const std::vector<std::string> queryFields = { "*" },
	const bool anyQueryFields = false);
std::wstring wmiQuerySimpleFromObject(const std::wstring objectName,
	const std::wstring namespacePath = L"ROOT\\CIMV2", const std::vector<std::wstring> queryFields = { L"*" },
	const bool anyQueryFields = false);
std::string genRandomString(const std::string prefix = "_", const size_t length = 6);
std::wstring genRandomString(const std::wstring prefix = L"_", const size_t length = 6);
bool valInList(const std::vector<std::string> &list, const std::string val, const bool ignoreCase = true,
	const bool exactMatch = false, size_t* posptr = 0);
bool valInList(const std::vector<std::wstring> &list, const std::wstring val, const bool ignoreCase = true,
	const bool exactMatch = false, size_t* posptr = 0);
/* Returns:
   0 - not numeric
   1 - numeric w/o fract part
   2 - numeric with fract part */
unsigned char isNumericDec(const std::wstring str);
/* Returns:
   0 - not numeric
   1 - numeric w/o fract part
   2 - numeric with fract part */
unsigned char isNumericDec(const std::wstring str);
/* Returns:
   0 - ipv4
   1 - ipv6
   2 - dns name
   3 - none */
unsigned char isStringIP(const std::string testStr);
/* Returns:
   0 - ipv4
   1 - ipv6
   2 - dns name
   3 - none */
unsigned char isStringIP(const std::wstring testStr);

#endif // !_STRHELPER_H