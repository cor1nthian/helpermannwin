// #include <iostream>
#if defined(_WIN32) || defined(_WIN64) 
    #include <ws2tcpip.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <arpa/inet.h>
#endif
#include "strhelper.h"

size_t wcslen_c(const wchar_t* line) {
    const wchar_t* end = line;
    if (line) {
        while (*end != L'\0') {
            ++end;
        }
    } else {
        return 0;
    }
    return end - line;
}

bool contentInBuf(const char* buf, size_t bufSz) {
    for (size_t i = 0; i < bufSz; ++i) {
        if (buf[i] != '\0') {
            return true;
        }
    }
    return false;
}

bool contentInBuf(const wchar_t* buf, size_t bufSz) {
    for (size_t i = 0; i < bufSz; ++i) {
        if (buf[i] != L'\0') {
            return true;
        }
    }
    return false;
}

wchar_t* char2wchar(const char* line, wchar_t* buf) {
    const size_t cSize = strlen(line) + 1;
    wchar_t* wc = 0;
    if (buf) {
        wc = buf;
    } else {
        NEW_ARR_NULLIFY_NO_REDEFINE(wc, wchar_t, cSize);
    }
    if (wc) {
        mbstowcs(wc, line, cSize);
        return wc;
    } else {
        return 0;
    }
}

char* wchar2char(const wchar_t* line, char* buf) {
    const size_t cSize = wcslen_c(line) + 1;
    char* wc = 0;
    if (buf) {
        wc = buf;
    } else {
        NEW_ARR_NULLIFY_NO_REDEFINE(wc, char, cSize);
    }
    if (wc) {
        wcstombs(wc, line, cSize);
        return wc;
    } else {
        return 0;
    }
}

std::string ul2hexstr(const unsigned long  val) {
    char buf[16] = { 0 };
    sprintf(buf, "%X", val);
    return buf;
}

std::wstring ul2hexwstr(const unsigned long val) {
    wchar_t buf[16] = { 0 };
    swprintf(buf, L"%X", val);
    return buf;
}

std::string ull2hexstr(const unsigned long long val) {
    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string ret;
    unsigned long long tmp = val;
    do {
        ret += digits[tmp % 16];
        tmp /= 16;
    } while (tmp != 0);
    std::reverse(ret.begin(), ret.end());
    return ret;
}

std::wstring ull2hexwstr(const unsigned long long val) {
    const wchar_t digits[] = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::wstring ret;
    unsigned long long tmp = val;
    do {
        ret += digits[tmp % 16];
        tmp /= 16;
    } while (tmp != 0);
    std::reverse(ret.begin(), ret.end());
    return ret;
}

unsigned long str2ul(const std::string val) {
    if (val.length()) {
        return std::stoul(val);
    } else {
        return 0;
    }
}

unsigned long wstr2ul(const std::wstring val) {
    if (val.length()) {
        return std::stoul(wstr2str(val));
    } else {
        return 0;
    }
}

unsigned long long str2ull(const std::string val) {
    return std::stoull(val);
}

unsigned long long wstr2ull(const std::wstring val) {
    return std::stoull(wstr2str(val));
}

char* replaceChars(const char* source, const char* replaceWhat, const char* replaceWith,
    const size_t srclen, const size_t replacewhatlen, const size_t replacewithlen) {
    size_t srcLen = 0, replaceWhatLen = 0, replaceWithLen = 0;
    if (srclen) {
        srcLen = srclen;
    } else {
        srcLen = strlen(source);
    }
    if (replacewhatlen) {
        replaceWhatLen = replacewhatlen;
    } else {
        replaceWhatLen = strlen(replaceWhat);
    }
    if (replaceWithLen) {
        replaceWithLen = replaceWithLen;
    } else {
        replaceWithLen = strlen(replaceWith);
    }
    if (!(srcLen && replaceWhatLen && replaceWithLen)) {
        return 0;
    }
    if ((replaceWhatLen > srcLen) || (!memcmp(source, replaceWhat, srcLen))) {
        NEW_ARR_NULLIFY(retbuf, char, srcLen);
        if (retbuf) {
            memcpy(retbuf, source, srcLen * sizeof(char));
            return retbuf;
        } else {
            return 0;
        }
    }
    size_t copyMark = 0, newBufSz = 0, stopMark = (srcLen - replaceWhatLen), i = 0;
    for (i = 0; i < stopMark; ++i) {
        if (!memcmp(&source[i], replaceWhat, replaceWhatLen * sizeof(char))) {
            newBufSz += replaceWithLen;
            i += replaceWithLen;
        } else {
            ++newBufSz;
        }
    }
    NEW_ARR_NULLIFY(newBuf, char, newBufSz);
    if (!newBuf) {
        return 0;
    }
    for (i = 0; i < srcLen; ++i) {
        if (memcmp(&source[i], replaceWhat, replaceWhatLen * sizeof(char))) {
            sprintf(&newBuf[copyMark], "%s", &source[i]);
            copyMark += 1;
        } else {
            sprintf(&newBuf[copyMark], "%s", replaceWith);
            copyMark += replaceWithLen;
            i += replaceWhatLen - 1;
        }
    }
    return newBuf;
}

wchar_t* replaceChars(const wchar_t* source, const wchar_t* replaceWhat,
    const wchar_t* replaceWith, const size_t srclen, const size_t replacewhatlen,
    const size_t replacewithlen) {
    size_t srcLen = 0, replaceWhatLen = 0, replaceWithLen = 0;
    if (srclen) {
        srcLen = srclen;
    } else {
        srcLen = wcslen_c(source);
    }
    if (replacewhatlen) {
        replaceWhatLen = replacewhatlen;
    } else {
        replaceWhatLen = wcslen_c(replaceWhat);
    }
    if (replacewithlen) {
        replaceWithLen = replacewithlen;
    } else {
        replaceWithLen = wcslen_c(replaceWith);
    }
    if (!(srcLen && replaceWhatLen && replaceWithLen)) {
        return 0;
    }
    if ((replaceWhatLen > srcLen) || (!memcmp(source, replaceWhat, srcLen))) {
        NEW_ARR_NULLIFY(retbuf, wchar_t, srcLen);
        if (retbuf) {
            memcpy(retbuf, source, srcLen * sizeof(wchar_t));
            return retbuf;
        } else {
            return 0;
        }
    }
    size_t copyMark = 0, newBufSz = 0, stopMark = (srcLen - replaceWhatLen), i = 0;
    for (i = 0; i < stopMark; ++i) {
        if (!memcmp(&source[i], replaceWhat, replaceWhatLen * sizeof(wchar_t))) {
            newBufSz += replaceWithLen;
            i += replaceWithLen;
        } else {
            ++newBufSz;
        }
    }
    NEW_ARR_NULLIFY(newBuf, wchar_t, newBufSz);
    if (!newBuf) {
        return 0;
    }
    memset(newBuf, 0, newBufSz * sizeof(wchar_t));
    for (i = 0; i < srcLen; ++i) {
        if (memcmp(&source[i], replaceWhat, replaceWhatLen * sizeof(wchar_t))) {
            wsprintf(&newBuf[copyMark], L"%s", &source[i]);
            copyMark += 1;
        } else {
            wsprintf(&newBuf[copyMark], L"%s", replaceWith);
            copyMark += replaceWithLen;
            i += replaceWhatLen - 1;
        }
    }
    return newBuf;
}

void lower_chars(char* chrs) {
    size_t slen = strlen(chrs);
    for (size_t i = 0; i < slen; ++i) {
        chrs[i] = tolower(chrs[i]);
    }
}

void lower_chars(wchar_t* chrs) {
    size_t slen = wcslen_c(chrs);
    for (size_t i = 0; i < slen; ++i) {
        chrs[i] = tolower(chrs[i]);
    }
}

char* lower_chars_copy(const char* chrs) {
    size_t buflen = strlen(chrs);
    char* newBuf = 0;
    newBuf = (char*)calloc(buflen, buflen * sizeof(char));
    if (!newBuf) {
        return 0;
    }
    for (size_t i = 0; i < buflen; ++i) {
        newBuf[i] = tolower(chrs[i]);
    }
    return newBuf;
}

wchar_t* lower_chars_copy(const wchar_t* chrs) {
    size_t buflen = wcslen_c(chrs);
    wchar_t* newBuf = 0;
    newBuf = (wchar_t*)calloc(buflen, buflen * sizeof(wchar_t));
    if (!newBuf) {
        return 0;
    }
    for (size_t i = 0; i < buflen; ++i) {
        newBuf[i] = tolower(chrs[i]);
    }
    return newBuf;
}

void upper_chars(char* chrs) {
    size_t slen = strlen(chrs);
    for (size_t i = 0; i < slen; ++i) {
        chrs[i] = toupper(chrs[i]);
    }
}

void upper_chars(wchar_t* chrs) {
    size_t slen = wcslen_c(chrs);
    for (size_t i = 0; i < slen; ++i) {
        chrs[i] = toupper(chrs[i]);
    }
}

char* upper_chars_copy(const char* chrs) {
    size_t buflen = strlen(chrs);
    char* newBuf = 0;
    newBuf = (char*)calloc(buflen, buflen * sizeof(char));
    if (!newBuf) {
        return 0;
    }
    for (size_t i = 0; i < buflen; ++i) {
        newBuf[i] = toupper(chrs[i]);
    }
    return newBuf;
}

wchar_t* upper_chars_copy(const wchar_t* chrs) {
    size_t buflen = wcslen_c(chrs);
    wchar_t* newBuf = 0;
    newBuf = (wchar_t*)calloc(buflen, buflen * sizeof(wchar_t));
    if (!newBuf) {
        return 0;
    }
    for (size_t i = 0; i < buflen; ++i) {
        newBuf[i] = toupper(chrs[i]);
    }
    return newBuf;
}

std::string wstr2str(const std::wstring str) {
    std::setlocale(LC_ALL, "");
    std::string ret(str.size(), L' ');
    ret.resize(std::wcstombs(&ret[0], str.c_str(), str.size()));
    return ret;
    /*try {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(str);
    } catch (std::range_error& e) {
        size_t len = str.length();
        std::string result;
        result.reserve(len);
        for (size_t i = 0; i < len; i++) {
            result.push_back(str[i]);
        }
        return result;
    }*/
}

std::wstring str2wstr(const std::string str) {
    std::setlocale(LC_ALL, "");
    std::wstring ret(str.size(), L' ');
    ret.resize(std::mbstowcs(&ret[0], str.c_str(), str.size()));
    return ret;
    /*try {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(str);
    } catch (std::range_error& e) {
        size_t len = str.length();
        std::wstring result;
        result.reserve(len);
        for (size_t i = 0; i < len; i++) {
            result.push_back(str[i] & 0xFF);
        }
        return result;
    }*/
}

std::string GUID2str(const GUID guid) {
    char guid_cstr[39] = { 0 };
    snprintf(guid_cstr, sizeof(guid_cstr),
        "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

    return upper_copy(std::string(guid_cstr));
}

#if defined(_WIN32) || defined(_WIN64)
std::string GUID2strMS(const GUID guid) {
    wchar_t tbuf[64] = { 0 };
    if (::StringFromGUID2(guid, tbuf, 64)) {
        std::wstring temp = tbuf;
        return wstr2str(temp);
    } else {
        return "";
    }
}

std::wstring GUID2wstrMS(const GUID guid) {
    wchar_t tbuf[64] = { 0 };
    if (::StringFromGUID2(guid, tbuf, 64)) {
        std::wstring temp = tbuf;
        return temp;
    } else {
        return L"";
    }
}
#endif

std::wstring GUID2wstr(const GUID guid) {
    wchar_t  tbuf[64] = { 0 };
    char guid_cstr[39];
    snprintf(guid_cstr, sizeof(guid_cstr),
        "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

    return upper_copy(str2wstr(std::string(guid_cstr)));
}

GUID str2GUID(const std::string str) {
    GUID ret = { 0 };
    if (str.length() != 38) {
        return ret;
    }
    for (int i = 0; i < str.length(); ++i) {
        char g = str.c_str()[i];
        if (i == 0) {
            if (g != '{') return ret;
        } else if (i == 37) {
            if (g != '}') return ret;
        } else if ((i == 9) || (i == 14) || (i == 19) || (i == 24)) {
            if (g != '-') return ret;
        } else {
            if (!((g >= '0') && (g <= '9')) && !((g >= 'A') && (g <= 'F')) && !((g >= 'a') && (g <= 'f'))) {
                return ret;
            }
        }
    }
    char* pEnd = 0;
    ret.Data1 = strtoul(str.c_str() + 1, &pEnd, 16);
    ret.Data2 = strtoul(str.c_str() + 10, &pEnd, 16);
    ret.Data3 = strtoul(str.c_str() + 15, &pEnd, 16);
    char b[3]; b[2] = 0;
    memcpy(&b[0], str.c_str() + 20, 2 * sizeof(b[0])); ret.Data4[0] = strtoul(&b[0], &pEnd, 16);
    memcpy(&b[0], str.c_str() + 22, 2 * sizeof(b[0])); ret.Data4[1] = strtoul(&b[0], &pEnd, 16);
    for (int i = 0; i < 6; ++i) {
        memcpy(&b[0], str.c_str() + 25 + i * 2, 2 * sizeof(b[0]));
        ret.Data4[2 + i] = strtoul(&b[0], &pEnd, 16);
    }
    return ret;
}

GUID wstr2GUID(const std::wstring str) {
    return (str2GUID(wstr2str(str)));
}

#if defined(_WIN32) || defined(_WIN64)
GUID str2GUIDMS(const std::string str) {
    return wstr2GUIDMS(str2wstr(str));
}

GUID wstr2GUIDMS(const std::wstring str) {
    GUID ret = { 0 };
    if(SUCCEEDED(::CLSIDFromString(str.c_str(), &ret))) {
        return ret;
    } else {
        return GUID_NULL;
    }
}
#endif

// trim from start (in place)
void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

// trim from end (in place)
void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string& s) {
    rtrim(s);
    ltrim(s);
}

// trim from start (copying)
std::string ltrim_copy(const std::string s) {
    std::string temp = s;
    ltrim(temp);
    return temp;
}

// trim from end (copying)
std::string rtrim_copy(const std::string s) {
    std::string temp = s;
    rtrim(temp);
    return temp;
}

// trim from both ends (copying)
std::string trim_copy(const std::string s) {
    std::string temp = s;
    trim(temp);
    return temp;
}

// trim from start (in place)
void ltrim(std::wstring& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

// trim from end (in place)
void rtrim(std::wstring& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// trim from both ends (in place)
void trim(std::wstring& s) {
    rtrim(s);
    ltrim(s);
}

// trim from start (copying)
std::wstring ltrim_copy(const std::wstring s) {
    std::wstring temp = s;
    ltrim(temp);
    return temp;
}

// trim from end (copying)
std::wstring rtrim_copy(const std::wstring s) {
    std::wstring temp = s;
    rtrim(temp);
    return temp;
}

// trim from both ends (copying)
std::wstring trim_copy(const std::wstring s) {
    std::wstring temp = s;
    trim(temp);
    return temp;
}

void lower(std::string& s) {
    std::locale loc;
    for (char &c : s) c = tolower(c, loc);
}

void lower(std::wstring &s) {
    std::locale loc;
    for (wchar_t& c : s) c = tolower(c, loc);
}

void lower(std::vector<std::string> &vec) {
    if (vec.size()) {
        for (size_t i = 0; i < vec.size(); ++i) {
            lower(vec[i]);
        }
    }
}

void lower(std::vector<std::wstring> &vec) {
    if (vec.size()) {
        for (size_t i = 0; i < vec.size(); ++i) {
            lower(vec[i]);
        }
    }
}

void upper(std::string &s) {
    std::locale loc;
    for (char &c : s) c = toupper(c, loc);
}

void upper(std::wstring& s) {
    std::locale loc;
    for (wchar_t& c : s) c = toupper(c, loc);
}

void upper(std::vector<std::string> &vec) {
    if (vec.size()) {
        for (size_t i = 0; i < vec.size(); ++i) {
            upper(vec[i]);
        }
    }
}

void upper(std::vector<std::wstring> &vec) {
    if (vec.size()) {
        for (size_t i = 0; i < vec.size(); ++i) {
            upper(vec[i]);
        }
    }
}

std::string lower_copy(const std::string s) {
    std::locale loc;
    std::string temp = s;
    for (char &c : temp) {
        c = tolower(c, loc);
    }
    return temp;
}

std::wstring lower_copy(const std::wstring s) {
    std::locale loc;
    std::wstring temp = s;
    for (wchar_t& c : temp) c = towlower(c);
    return temp;
}

std::vector<std::string> lower_copy(const std::vector<std::string> vec) {
    std::vector<std::string> ret = vec;
    if (ret.size()) {
        std::locale loc;
        ret.clear();
        for (size_t i = 0; i < vec.size(); ++i) {
            ret.push_back(lower_copy(vec[i]));
        }
    }
    return ret;
}

std::vector<std::wstring> lower_copy(const std::vector<std::wstring> vec) {
    std::vector<std::wstring> ret = vec;
    if (ret.size()) {
        std::locale loc;
        ret.clear();
        for (size_t i = 0; i < vec.size(); ++i) {
            ret.push_back(lower_copy(vec[i]));
        }
    }
    return ret;
}

std::string upper_copy(const std::string s) {
    std::string temp = s;
    for (char &c : temp) {
        c = towupper(c);
    }
    return temp;
}

std::wstring upper_copy(const std::wstring s) {
    std::wstring temp = s;
    for (wchar_t &c : temp) c = towupper(c);
    return temp;
}

std::vector<std::string> upper_copy(const std::vector<std::string> vec) {
    std::vector<std::string> ret = vec;
    if (ret.size()) {
        std::locale loc;
        ret.clear();
        for (size_t i = 0; i < vec.size(); ++i) {
            ret.push_back(upper_copy(vec[i]));
        }
    }
    return ret;
}

std::vector<std::wstring> upper_copy(const std::vector<std::wstring> vec) {
    std::vector<std::wstring> ret = vec;
    if (ret.size()) {
        std::locale loc;
        ret.clear();
        for (size_t i = 0; i < vec.size(); ++i) {
            ret.push_back(upper_copy(vec[i]));
        }
    }
    return ret;
}

void prepRegexString(std::string &regexString) {
    if (std::wstring::npos != regexString.find("+")) {
        regexString = replaceSubstr_copy(regexString, "+", "\\+");
    }
}

void prepRegexString(std::wstring &regexString) {
    if (std::wstring::npos != regexString.find(L"+")) {
        regexString = replaceSubstr_copy(regexString, L"+", L"\\+");
    }
}

std::string prepRegexString_copy(const std::string regexString) {
    std::string ret = regexString;
    if (std::wstring::npos != ret.find("+")) {
        ret = replaceSubstr_copy(ret, "+", "\\+");
    }
    return ret;
}

std::wstring prepRegexString_copy(const std::wstring regexString) {
    std::wstring ret = regexString;
    std::vector<std::wstring> escapedChars;
    escapedChars.push_back(L"+");
    for (size_t i = 0; i < escapedChars.size(); ++i) {
        ret = replaceAll(ret, escapedChars[i], L"\\" + escapedChars[i]);
    }
    /*if (std::wstring::npos != ret.find(L"+")) {
        ret = replaceSubstr_copy(ret, L"+", L"\\+");
    }*/
    return ret;
}

std::string replaceAll(const std::string source, const std::string replaceWhat,
    const  std::string replaceWith) {
    std::string tmp = source;
    if (source.length() >= replaceWhat.length() && !replaceWhat.empty()) {
        std::string tmp = source;
        size_t pos = 0;
        do {
            pos = tmp.find(replaceWhat, pos);
            if (std::wstring::npos == pos) {
                return tmp;
            }
            tmp = tmp.replace(pos, replaceWhat.length(), replaceWith);
            pos += replaceWith.length();
        } while (std::wstring::npos != tmp.find(replaceWhat));
        return tmp;
    }
    return tmp;
}

std::wstring replaceAll(const std::wstring source, const std::wstring replaceWhat,
    const std::wstring replaceWith) {
    std::wstring tmp = source;
    if (source.length() >= replaceWhat.length() && !replaceWhat.empty()) {
        std::wstring tmp = source;
        size_t pos = 0;
        do {
            pos = tmp.find(replaceWhat, pos);
            if (std::wstring::npos == pos) {
                return tmp;
            }
            tmp = tmp.replace(pos, replaceWhat.length(), replaceWith);
            pos += replaceWith.length();
        } while (std::wstring::npos != tmp.find(replaceWhat));
        return tmp;
    }
    return tmp;
}

void removeFromStart(std::string& source, const std::string sequence, const bool ignoreCase) {
    if (ignoreCase) {
        std::string temp = lower_copy(source);
        if (temp != lower_copy(sequence)) {
            if (temp.length() > sequence.length()) {
                if (lower_copy(source).substr(0, sequence.length()) == lower_copy(sequence)) {
                    source.erase(0, sequence.length());
                }
            }
        } else {
            // source = "";
        }
    } else {
        if (source != sequence) {
            if (source.length() > sequence.length()) {
                if (source.substr(0, sequence.length()) == sequence) {
                    source.erase(0, sequence.length());
                }
            }
        } else {
            // source = "";
        }
    }
}

void removeFromStart(std::wstring& source, const std::wstring sequence, const bool ignoreCase) {
    if (ignoreCase) {
        std::wstring temp = lower_copy(source);
        if (temp != lower_copy(sequence)) {
            if (temp.length() > sequence.length()) {
                if (lower_copy(source).substr(0, sequence.length()) == lower_copy(sequence)) {
                    source.erase(0, sequence.length());
                }
            }
        } else {
            // source = L"";
        }
    } else {
        if (source != sequence) {
            if (source.length() > sequence.length()) {
                if (source.substr(0, sequence.length()) == sequence) {
                    source.erase(0, sequence.length());
                }
            }
        } else {
            // source = L"";
        }
    }
}

void removeFromEnd(std::string& source, const std::string sequence, const bool ignoreCase) {
    if (ignoreCase) {
        std::string temp = lower_copy(source);
        if (temp != lower_copy(sequence)) {
            if (temp.length() > sequence.length()) {
                if (temp.substr(temp.length() - sequence.length(), sequence.length()) == lower_copy(sequence)) {
                    source.erase(source.length() - sequence.length(), sequence.length());
                }
            }
        } else {
            source = "";
        }
    } else {
        if (source != sequence) {
            if (source.length() > sequence.length()) {
                if (source.substr(source.length() - sequence.length(), sequence.length()) == sequence) {
                    source.erase(source.length() - sequence.length(), sequence.length());
                }
            }
        } else {
            // source = "";
        }
    }
}

void removeFromEnd(std::wstring& source, const std::wstring sequence, const bool ignoreCase) {
    if (ignoreCase) {
        if (source.length() > sequence.length()) {
            std::wstring temp = lower_copy(source);
            if (temp != lower_copy(sequence)) {
                if (temp.length() > sequence.length()) {
                    if (temp.substr(temp.length() - sequence.length(), sequence.length()) == lower_copy(sequence)) {
                        source.erase(source.length() - sequence.length(), sequence.length());
                    }
                }
            } else {
                 // source = L"";
            }
        }
    } else {
        if (source.length() > sequence.length()) {
            if (source != sequence) {
                if (source.length() > sequence.length()) {
                    if (source.substr(source.length() - sequence.length(), sequence.length()) == sequence) {
                        source.erase(source.length() - sequence.length(), sequence.length());
                    }
                }
            } else {
                source = L"";
            }
        }
    }
}

void removeFromBothSides(std::string& source, const std::string sequence, const bool ignoreCase) {
    removeFromStart(source, sequence, ignoreCase);
    removeFromEnd(source, sequence, ignoreCase);
}

void removeFromBothSides(std::wstring& source, const std::wstring sequence, const bool ignoreCase) {
    removeFromStart(source, sequence, ignoreCase);
    removeFromEnd(source, sequence, ignoreCase);
}

std::string removeFromStart_copy(const std::string source, const std::string sequence, const bool ignoreCase) {
    if (ignoreCase) {
        std::string temp = lower_copy(source);
        if (temp != lower_copy(sequence)) {
            if (temp.length() > sequence.length()) {
                if (temp.substr(0, sequence.length()) == lower_copy(sequence)) {
                    return source.substr(sequence.length(), source.length() - sequence.length());
                } else {
                    return source;
                }
            } else {
                return source;
            }
        } else {
            return source;
        }
    } else {
        if (source != sequence) {
            if (source.length() > sequence.length()) {
                if (source.substr(0, sequence.length()) == sequence) {
                    return source.substr(sequence.length(), source.length() - sequence.length());
                } else {
                    return source;
                }
            } else {
                return source;
            }
        } else {
            return source;
        }
    }
}

std::wstring removeFromStart_copy(const std::wstring source, const std::wstring sequence, const bool ignoreCase) {
    if (ignoreCase) {
        std::wstring temp = lower_copy(source);
        if (temp != lower_copy(sequence)) {
            if (temp.length() > sequence.length()) {
                if (temp.substr(0, sequence.length()) == lower_copy(sequence)) {
                    return source.substr(sequence.length(), source.length() - sequence.length());
                } else {
                    return source;
                }
            } else {
                return source;
            }
        } else {
            return source;
        }
    } else {
        if (source != sequence) {
            if (source.length() > sequence.length()) {
                if (source.substr(0, sequence.length()) == sequence) {
                    return source.substr(sequence.length(), source.length() - sequence.length());
                } else {
                    return source;
                }
            } else {
                return source;
            }
        } else {
            return source;
        }
    }
}

std::string removeFromEnd_copy(const std::string source, const std::string sequence, const bool ignoreCase) {
    if (ignoreCase) {
        std::string temp = lower_copy(source);
        if (temp != lower_copy(sequence) && (temp.length() > sequence.length())) {
            if (temp.substr(temp.length() - sequence.length(), sequence.length()) == lower_copy(sequence)) {
                return source.substr(0, source.length() - sequence.length());
            } else {
                return source;
            }
        } else {
            return source;
        }
    } else {
        if (source != sequence && (source.length() > sequence.length())) {
            if (source.substr(source.length() - sequence.length(), sequence.length()) == sequence) {
                return source.substr(0, source.length() - sequence.length());
            } else {
                return source;
            }
        } else {
            return source;
        }
    }
}

std::wstring removeFromEnd_copy(const std::wstring source, const std::wstring sequence, const bool ignoreCase) {
    if (ignoreCase) {
        std::wstring temp = lower_copy(source);
        if (temp != lower_copy(sequence) && (temp.length() > sequence.length())) {
            if (temp.substr(temp.length() - sequence.length(), sequence.length()) == lower_copy(sequence)) {
                return source.substr(0, source.length() - sequence.length());
            } else {
                return source;
            }
        } else {
            return source;
        }
    } else {
        if (source != sequence && (source.length() > sequence.length())) {
            if (source.substr(source.length() - sequence.length(), sequence.length()) == sequence) {
                return source.substr(0, source.length() - sequence.length());
            } else {
                return source;
            }
        } else {
            return source;
        }
    }
}

std::string removeFromBothSides_copy(const std::string source, const std::string sequence, const bool ignoreCase) {
    std::string tstr = removeFromStart_copy(source, sequence, ignoreCase);
    return removeFromEnd_copy(tstr, sequence, ignoreCase);
}

std::wstring removeFromBothSides_copy(const std::wstring source, const std::wstring sequence, const bool ignoreCase) {
    std::wstring tstr = removeFromStart_copy(source, sequence, ignoreCase);
    return removeFromEnd_copy(tstr, sequence, ignoreCase);
}

bool startsWith(const std::string source, const std::string sequence, const bool ignoreCase) {
    if (ignoreCase) {
        if (source.length()) {
            std::string tempSrc = lower_copy(source);
            std::string tempSeq = lower_copy(sequence);
            if (tempSrc != tempSeq) {
                if (tempSrc.substr(0, tempSeq.length()) == tempSeq) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        } else {
            return false;
        }
    } else {
        if (source.length()) {
            if (source != sequence) {
                if (source.substr(0, sequence.length()) == sequence) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        } else {
            return false;
        }
    }
}

bool startsWith(const std::wstring source, const std::wstring sequence, const bool ignoreCase) {
    if (ignoreCase) {
        if (source.length()) {
            std::wstring tempSrc = lower_copy(source);
            std::wstring tempSeq = lower_copy(sequence);
            if (tempSrc != tempSeq) {
                if (tempSrc.substr(0, tempSeq.length()) == tempSeq) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        } else {
            return false;
        }
    } else {
        if (source.length()) {
            if (source != sequence) {
                if (source.substr(0, sequence.length()) == sequence) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        } else {
            return false;
        }
    }
}

bool endsWith(const std::string source, const std::string sequence, const bool ignoreCase) {
    if (ignoreCase) {
        if (source.length()) {
            std::string tempSrc = lower_copy(source);
            std::string tempSeq = lower_copy(sequence);
            if (tempSrc != tempSeq) {
                if (tempSrc.substr(tempSrc.length() - tempSeq.length(), tempSeq.length()) == tempSeq) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        } else {
            return false;
        }
    } else {
        if (source.length()) {
            if (source != sequence) {
                if (source.substr(source.length() - sequence.length(), sequence.length()) == sequence) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        } else {
            return false;
        }
    }
}

bool endsWith(const std::wstring source, const std::wstring sequence, const bool ignoreCase) {
    if (ignoreCase) {
        if (source.length() > sequence.length()) {
            std::wstring tempSrc = lower_copy(source);
            std::wstring tempSeq = lower_copy(sequence);
            if (tempSrc != tempSeq) {
                if (tempSrc.substr(tempSrc.length() - tempSeq.length(), tempSeq.length()) == tempSeq) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        } else {
            return false;
        }
    } else {
        if (source.length()) {
            if (source != sequence) {
                if (source.substr(source.length() - sequence.length(), sequence.length()) == sequence) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        } else {
            return false;
        }
    }
}

bool partialMatch(const std::string source, const std::string compare, const bool ignoreCase, size_t *position) {
    if (ignoreCase) {
        size_t pos = lower_copy(source).find(lower_copy(compare));
        if (std::wstring::npos != pos) {
            if (position) {
                *position = pos;
            }
            return true;
        } else {
            return false;
        }
    } else {
        size_t pos = source.find(compare);
        if (std::wstring::npos != pos) {
            if (position) {
                *position = pos;
            }
            return true;
        } else {
            return false;
        }
    }
}

bool partialMatch(const std::wstring source, const std::wstring compare, const bool ignoreCase, size_t *position) {
    if (ignoreCase) {
        size_t pos = lower_copy(source).find(lower_copy(compare));
        if (std::wstring::npos != pos) {
            if (position) {
                *position = pos;
            }
            return true;
        } else {
            return false;
        }
    } else {
        size_t pos = source.find(compare);
        if (std::wstring::npos != pos) {
            if (position) {
                *position = pos;
            }
            return true;
        } else {
            return false;
        }
    }
}

size_t countOccurence(const std::string source, const std::string sequence) {
    size_t occurrences = 0;
    std::string::size_type pos = 0;
    std::string tgt = sequence;
    size_t tgtlen = tgt.length();
    while ((pos = source.find(tgt, pos)) != std::string::npos) {
        ++occurrences;
        pos += tgtlen;
    }
    return occurrences;
}

size_t countOccurence(const std::wstring source, const std::wstring sequence) {
    size_t occurrences = 0;
    std::wstring::size_type pos = 0;
    std::wstring tgt = sequence;
    size_t tgtlen = tgt.length();
    while ((pos = source.find(tgt, pos)) != std::string::npos) {
        ++occurrences;
        pos += tgtlen;
    }
    return occurrences;
}

std::vector<std::string> splitStr(const std::string str, const char* delimiter, bool includeEmpty) {
    std::vector<std::string> ret;
    if (std::string::npos == str.find(delimiter)) {
        ret.push_back(str);
        return ret;
    }
    size_t dc = countOccurence(str, delimiter);
    if (dc) {
        size_t pos = 0;
        std::string token;
        std::string tstr = str;
        std::string delim = delimiter;
        while ((pos = tstr.find(delim)) != std::string::npos) {
            token = tstr.substr(0, pos);
            if (token.length()) {
                ret.push_back(token);
            } else {
                if (includeEmpty) {
                    ret.push_back(token);
                }
            }
            tstr.erase(0, pos + delim.length());
        }
        if (tstr.length()) {
            ret.push_back(tstr);
        } else {
            if (includeEmpty) {
                ret.push_back(tstr);
            }
        }
    }
    return ret;
}

std::vector<std::wstring> splitStr(const std::wstring str, const wchar_t* delimiter, bool includeEmpty) {
    std::vector<std::wstring> ret;
    if (std::string::npos == str.find(delimiter)) {
        ret.push_back(str);
        return ret;
    }
    size_t dc = countOccurence(str, delimiter);
    if (dc) {
        size_t pos = 0;
        std::wstring token;
        std::wstring tstr = str;
        std::wstring delim = delimiter;
        while ((pos = tstr.find(delim)) != std::string::npos) {
            token = tstr.substr(0, pos);
            if (token.length()) {
                ret.push_back(token);
            } else {
                if (includeEmpty) {
                    ret.push_back(token);
                }
            }
            tstr.erase(0, pos + delim.length());
        }
        if (tstr.length()) {
            ret.push_back(tstr);
        } else {
            if (includeEmpty) {
                ret.push_back(tstr);
            }
        }
    }
    return ret;
}

std::map<std::string, std::vector<std::string>> splitStr(const std::string str,
    const std::vector<const char*> delimiters, const bool ignoreCase, const bool includeEmpty) {
    std::map<std::string, std::vector<std::string>> ret;
    std::vector<std::string> temp;
    auto& delim_unconst = const_cast<std::vector<const char*>&>(delimiters);
    if (std::find(delim_unconst.begin(), delim_unconst.end(), "?") == delim_unconst.end()) {
        delim_unconst.push_back((char*)"?");
    }
    if (ignoreCase) {
        std::vector<std::string> delims;
        for (auto &rec : delim_unconst) {
            std::string recl = lower_copy(rec);
            std::string recu = upper_copy(rec);
            if (std::find(delims.begin(), delims.end(), rec) == delims.end()) {
                delims.push_back(rec);
            }
            if (std::find(delims.begin(), delims.end(), recl) == delims.end()) {
                delims.push_back(recl);
            }
            if (std::find(delims.begin(), delims.end(), recu) == delims.end()) {
                delims.push_back(recu);
            }
        }
        for (unsigned int i = 0; i < delims.size(); ++i) {
            temp.clear();
            size_t dc = countOccurence(str, delims[i]);
            if (dc) {
                size_t pos = 0;
                std::string token;
                std::string tstr = str;
                std::string delim = delims[i];
                while ((pos = tstr.find(delim)) != std::string::npos) {
                    token = tstr.substr(0, pos);
                    if (token.length()) {
                        temp.push_back(token);
                    } else {
                        if (includeEmpty) {
                            temp.push_back(token);
                        }
                    }
                    tstr.erase(0, pos + delim.length());
                }
                if (tstr.length()) {
                    temp.push_back(tstr);
                } else {
                    if (includeEmpty) {
                        temp.push_back(tstr);
                    }
                }
                
            }
            if (temp.size()) {
                ret[delims[i]] = temp;
            } else {
                if (includeEmpty) {
                    ret[delims[i]] = temp;
                }
            }
        }
    } else {
        for (unsigned int i = 0; i < delimiters.size(); ++i) {
            temp.clear();
            size_t dc = countOccurence(str, delimiters[i]);
            if (dc) {
                size_t pos = 0;
                std::string token;
                std::string tstr = str;
                std::string delim = delimiters[i];
                while ((pos = tstr.find(delim)) != std::string::npos) {
                    token = tstr.substr(0, pos);
                    if (token.length()) {
                        temp.push_back(token);
                    } else {
                        if (includeEmpty) {
                            temp.push_back(token);
                        }
                    }
                    tstr.erase(0, pos + delim.length());
                }
                temp.push_back(tstr);
            }
            if (temp.size()) {
                ret[delimiters[i]] = temp;
            } else {
                if (includeEmpty) {
                    ret[delimiters[i]] = temp;
                }
            }
        }
    }
    return ret;
}

std::map<std::wstring, std::vector<std::wstring>> splitStr(const std::wstring str,
    const std::vector<const wchar_t*> delimiters, const bool ignoreCase, const bool includeEmpty) {
    std::map<std::wstring, std::vector<std::wstring>> ret;
    std::vector<std::wstring> temp;
    auto& delim_unconst = const_cast<std::vector<const wchar_t*>&>(delimiters);
    if (std::find(delim_unconst.begin(), delim_unconst.end(), L"?") == delim_unconst.end()) {
        delim_unconst.push_back(L"?");
    }
    if (ignoreCase) {
        std::vector<std::wstring> delims;
        for (auto& rec : delim_unconst) {
            std::wstring recl = lower_copy(rec);
            std::wstring recu = upper_copy(rec);
            if (std::find(delims.begin(), delims.end(), rec) == delims.end()) {
                delims.push_back(rec);
            }
            if (std::find(delims.begin(), delims.end(), recl) == delims.end()) {
                delims.push_back(recl);
            }
            if (std::find(delims.begin(), delims.end(), recu) == delims.end()) {
                delims.push_back(recu);
            }
        }
        for (unsigned int i = 0; i < delims.size(); ++i) {
            temp.clear();
            size_t dc = countOccurence(str, delims[i]);
            if (dc) {
                size_t pos = 0;
                std::wstring token;
                std::wstring tstr = str;
                std::wstring delim = delims[i];
                while ((pos = tstr.find(delim)) != std::string::npos) {
                    token = tstr.substr(0, pos);
                    if (token.length()) {
                        temp.push_back(token);
                    } else {
                        if (includeEmpty) {
                            temp.push_back(token);
                        }
                    }
                    tstr.erase(0, pos + delim.length());
                }
                if (tstr.length()) {
                    temp.push_back(tstr);
                } else {
                    if (includeEmpty) {
                        temp.push_back(tstr);
                    }
                }

            }
            if (temp.size()) {
                ret[delims[i]] = temp;
            } else {
                if (includeEmpty) {
                    ret[delims[i]] = temp;
                }
            }
        }
    } else {
        for (unsigned int i = 0; i < delimiters.size(); ++i) {
            temp.clear();
            size_t dc = countOccurence(str, delimiters[i]);
            if (dc) {
                size_t pos = 0;
                std::wstring token;
                std::wstring tstr = str;
                std::wstring delim = delimiters[i];
                while ((pos = tstr.find(delim)) != std::string::npos) {
                    token = tstr.substr(0, pos);
                    if (token.length()) {
                        temp.push_back(token);
                    } else {
                        if (includeEmpty) {
                            temp.push_back(token);
                        }
                    }
                    tstr.erase(0, pos + delim.length());
                }
                temp.push_back(tstr);
            }
            if (temp.size()) {
                ret[delimiters[i]] = temp;
            } else {
                if (includeEmpty) {
                    ret[delimiters[i]] = temp;
                }
            }
        }
    }
    return ret;
}

std::map<size_t, std::string> tokenFromString(const std::string source, const std::string separator,
    const bool includeSeparators, const bool ignoreCase, const bool uniqueValues) {
    std::map<size_t, std::string> ret;
    if ((!source.length() || !separator.length()) || source.length() < 3 || separator.length() > source.length()) {
        return ret;
    }
    std::string workStr, sep;
    if (ignoreCase) {
        workStr = lower_copy(source);
        sep = lower_copy(separator);
    } else {
        workStr = source;
        sep = separator;
    }
    size_t sepnum = countOccurence(workStr, sep);
    if (!(sepnum % 2 == 0)) {
        return ret;
    }
    size_t findpos = 0;
    for (size_t i = 0; i < source.length(); ++i) {
        findpos = workStr.find(sep, i);
        if (std::string::npos == findpos) {
            return ret;
        }
        if (includeSeparators) {
            if (findpos != i) {
                if (uniqueValues) {
                    bool noelem = true;
                    std::string frag = source.substr(i - 1, (findpos - i) + sep.length() + 1);
                    for(auto &it : ret) {
                        if (frag == it.second) {
                            noelem = false;
                            break;
                        }
                    }
                    if (noelem) {
                        ret[i - sep.length()] = frag;
                    }
                } else {
                    ret[i - sep.length()] = source.substr(i - 1, (findpos - i) + sep.length() + 1);
                    // ret.push_back(source.substr(i - 1, (findpos - i) + sep.length() + 1));
                }
            }
        } else {
            if (findpos != i) {
                if (uniqueValues) {
                    bool noelem = true;
                    std::string frag = source.substr(i + sep.length() - 1, (findpos - i) - sep.length() + 1);
                    for (auto &it : ret) {
                        if (frag == it.second) {
                            noelem = false;
                            break;
                        }
                    }
                    if (noelem) {
                        ret[i - sep.length()] = frag;
                    }
                } else {
                    ret[i - sep.length()] = source.substr(i + sep.length() - 1, (findpos - i) - sep.length() + 1);
                    // ret.push_back(source.substr(i + sep.length() - 1, (findpos - i) - sep.length() + 1));
                }
            }
        }
        if (std::string::npos != findpos) {
            i = findpos;
        } else {
            return ret;
        }
    }
    return ret;
}

std::map<size_t, std::wstring> tokenFromString(const std::wstring source, const std::wstring separator,
    const bool includeSeparators, const bool ignoreCase, const bool uniqueValues) {
    std::map<size_t, std::wstring> ret;
    if ((!source.length() || !separator.length()) || source.length() < 3 || separator.length() > source.length()) {
        return ret;
    }
    std::wstring workStr, sep;
    if (ignoreCase) {
        workStr = lower_copy(source);
        sep = lower_copy(separator);
    } else {
        workStr = source;
        sep = separator;
    }
    size_t sepnum = countOccurence(workStr, sep);
    if (!(sepnum % 2 == 0)) {
        return ret;
    }
    size_t findpos = 0;
    for (size_t i = 0; i < source.length(); ++i) {
        findpos = workStr.find(sep, i);
        if (std::string::npos == findpos) {
            return ret;
        }
        if (includeSeparators) {
            if (findpos != i) {
                if (uniqueValues) {
                    bool noelem = true;
                    std::wstring frag = source.substr(i - 1, (findpos - i) + sep.length() + 1);
                    for (auto& it : ret) {
                        if (frag == it.second) {
                            noelem = false;
                            break;
                        }
                    }
                    if (noelem) {
                        ret[i - sep.length()] = frag;
                        // ret[i] = frag;
                    }
                } else {
                    ret[i - sep.length()] = source.substr(i - 1, (findpos - i) + sep.length() + 1);
                    // ret[i] = source.substr(i - 1, (findpos - i) + sep.length() + 1);
                    // ret.push_back(source.substr(i - 1, (findpos - i) + sep.length() + 1));
                }
            }
        } else {
            if (findpos != i) {
                if (uniqueValues) {
                    bool noelem = true;
                    std::wstring frag = source.substr(i + sep.length() - 1, (findpos - i) - sep.length() + 1);
                    for (auto& it : ret) {
                        if (frag == it.second) {
                            noelem = false;
                            break;
                        }
                    }
                    if (noelem) {
                        ret[i - sep.length()] = frag;
                        // ret[i] = frag;
                    }
                } else {
                    ret[i - sep.length()] = source.substr(i + sep.length() - 1, (findpos - i) - sep.length() + 1);
                    // ret[i] = source.substr(i + sep.length() - 1, (findpos - i) - sep.length() + 1);
                    // ret.push_back(source.substr(i + sep.length() - 1, (findpos - i) - sep.length() + 1));
                }
            }
        }
        if (std::string::npos != findpos) {
            i = findpos;
        } else {
            return ret;
        }
    }
    return ret;
}

std::string joinStrs(const std::vector<std::string> strs, const std::string delimiter,
    const size_t startIdx, const size_t endIdx) {
    std::string ret;
    if (strs.size()) {
        size_t si = startIdx, ei = endIdx;
        if (!endIdx || endIdx <= startIdx || endIdx > strs.size()) {
            ei = strs.size();
        } else {
            ei = endIdx;
        }
        /*if (startIdx >= strs.size() || startIdx >= ei) {
            si = 0;
        } else {
            si = startIdx;
        }*/
        if (si >= strs.size()) {
            return "";
        }
        for (size_t i = si; i < ei; ++i) {
            ret += strs[i] + delimiter;
        }
        removeFromEnd(ret, delimiter);
    }
    return ret;
}

std::wstring joinStrs(const std::vector<std::wstring> strs, const std::wstring delimiter,
    const size_t startIdx, const size_t endIdx) {
    std::wstring ret;
    if (strs.size()) {
        size_t si = startIdx, ei = endIdx;
        if (!endIdx || endIdx <= startIdx || endIdx >= strs.size()) {
            ei = strs.size();
        } else {
            ei = endIdx;
        }
        /*if (startIdx >= strs.size() || startIdx >= ei) {
            si = 0;
        } else {
            si = startIdx;
        }*/
        if (si >= strs.size()) {
            return L"";
        }
        for (size_t i = si; i < ei; ++i) {
            ret = ret + strs[i] + delimiter;
        }
        removeFromEnd(ret, delimiter);
    }
    return ret;
}

std::string reverseJoinStrs(const std::vector<std::string> strs, const std::string delimiter,
    const size_t startIdx, const size_t endIdx) {
    std::string ret;
    if (strs.size()) {
        size_t si = 0, ei = 0;
        if (!endIdx || endIdx <= startIdx || endIdx >= strs.size()) {
            ei = strs.size() - 1;
        } else {
            ei = endIdx;
        }
        if (startIdx >= strs.size() || startIdx >= ei) {
            si = 0;
        } else {
            si = startIdx;
        }
        long long i = ei;
        do {
            ret += strs[i] + delimiter;
            if (i == si) {
                break;
            } else {
                --i;
            }
        } while(i != si);
        removeFromEnd(ret, delimiter);
    }
    return ret;
}

std::wstring reverseJoinStrs(const std::vector<std::wstring> strs, const std::wstring delimiter,
    const size_t startIdx, const size_t endIdx) {
    std::wstring ret;
    if (strs.size()) {
        size_t si = 0, ei = 0;
        if (!endIdx || endIdx <= startIdx || endIdx >= strs.size()) {
            ei = strs.size() - 1;
        } else {
            ei = endIdx;
        }
        if (startIdx >= strs.size() || startIdx >= ei) {
            si = 0;
        } else {
            si = startIdx;
        }
        long long i = ei;
        do {
            ret += strs[i] + delimiter;
            if (i == si) {
                break;
            } else {
                --i;
            }
        } while (i >= si);
        removeFromEnd(ret, delimiter);
    }
    return ret;
}

void removeSubstr(std::string &str, const std::string substr, const bool ignoreCase) {
    size_t pos = 0;
    if (ignoreCase) {
        size_t posLow = 0;
        size_t posUp = 0;
        std::string substrLow = lower_copy(substr);
        std::string substrUp = upper_copy(substr);
        while (true) {
            pos = str.find(substr, pos);
            if (pos != std::string::npos) {
                str.erase(pos, substr.length());
            }
            posLow = str.find(substrLow, posLow);
            if (posLow != std::string::npos) {
                str.erase(posLow, substrLow.length());
            }
            posUp = str.find(substrUp, posUp);
            if (posUp != std::string::npos) {
                str.erase(posUp, substrUp.length());
            }
            if ((pos == std::string::npos && posLow == std::string::npos && posUp == std::string::npos) ||
                (pos == 0 && posLow == 0 && posUp == 0)) {
                break;
            }
            pos = 0;
            posLow = 0;
            posUp = 0;
        }
    } else {
        while (true) {
            pos = str.find(substr, pos);
            if (pos != std::string::npos) {
                str.erase(pos, substr.length());
            } else {
                break;
            }
        }
    }
}

void removeSubstr(std::wstring &str, const std::wstring substr, const bool ignoreCase) {
    size_t pos = 0;
    if (ignoreCase) {
        size_t posLow = 0;
        size_t posUp = 0;
        std::wstring substrLow = lower_copy(substr);
        std::wstring substrUp = upper_copy(substr);
        while (true) {
            pos = str.find(substr, pos);
            if (pos != std::string::npos) {
                str.erase(pos, substr.length());
            }
            posLow = str.find(substrLow, posLow);
            if (posLow != std::string::npos) {
                str.erase(posLow, substrLow.length());
            }
            posUp = str.find(substrUp, posUp);
            if (posUp != std::string::npos) {
                str.erase(posUp, substrUp.length());
            }
            if ((pos == std::string::npos && posLow == std::string::npos && posUp == std::string::npos) ||
                (pos == 0 && posLow == 0 && posUp == 0)) {
                break;
            }
            pos = 0;
            posLow = 0;
            posUp = 0;
        }
    } else {
        while (true) {
            pos = str.find(substr, pos);
            if (pos != std::string::npos) {
                str.erase(pos, substr.length());
            } else {
                break;
            }
        }
    }
}

void removeSubstr(std::string &str, const std::vector<std::string> sequences, const bool ignoreCase) {
    for (unsigned int i = 0; i < sequences.size(); ++i) {
        size_t pos = 0;
        if (ignoreCase) {
            size_t posLow = 0;
            size_t posUp = 0;
            std::string substrLow = lower_copy(sequences[i]);
            std::string substrUp = upper_copy(sequences[i]);
            while (true) {
                pos = str.find(sequences[i], pos);
                if (pos != std::string::npos) {
                    str.erase(pos, sequences[i].length());
                }
                posLow = str.find(substrLow, posLow);
                if (posLow != std::string::npos) {
                    str.erase(posLow, substrLow.length());
                }
                posUp = str.find(substrUp, posUp);
                if (posUp != std::string::npos) {
                    str.erase(posUp, substrUp.length());
                }
                if ((pos == std::string::npos && posLow == std::string::npos && posUp == std::string::npos) ||
                    (pos == 0 && posLow == 0 && posUp == 0)) {
                    break;
                }
                pos = 0;
                posLow = 0;
                posUp = 0;
            }
        } else {
            while (true) {
                pos = str.find(sequences[i], pos);
                if (pos != std::string::npos) {
                    str.erase(pos, sequences[i].length());
                }
                else {
                    break;
                }
            }
        }
    }
}

void removeSubstr(std::wstring &str, const std::vector<std::wstring> sequences, const bool ignoreCase) {
    for (unsigned int i = 0; i < sequences.size(); ++i) {
        size_t pos = 0;
        if (ignoreCase) {
            size_t posLow = 0;
            size_t posUp = 0;
            std::wstring substrLow = lower_copy(sequences[i]);
            std::wstring substrUp = upper_copy(sequences[i]);
            while (true) {
                pos = str.find(sequences[i], pos);
                if (pos != std::string::npos) {
                    str.erase(pos, sequences[i].length());
                }
                posLow = str.find(substrLow, posLow);
                if (posLow != std::string::npos) {
                    str.erase(posLow, substrLow.length());
                }
                posUp = str.find(substrUp, posUp);
                if (posUp != std::string::npos) {
                    str.erase(posUp, substrUp.length());
                }
                if ((pos == std::string::npos && posLow == std::string::npos && posUp == std::string::npos) ||
                    (pos == 0 && posLow == 0 && posUp == 0)) {
                    break;
                }
                pos = 0;
                posLow = 0;
                posUp = 0;
            }
        } else {
            while (true) {
                pos = str.find(sequences[i], pos);
                if (pos != std::string::npos) {
                    str.erase(pos, sequences[i].length());
                } else {
                    break;
                }
            }
        }
    }
}

std::string removeSubstr_copy(const std::string str, const std::string substr, const bool ignoreCase) {;
    std::string strSrc = str;
    size_t pos = 0;
    if (ignoreCase) {
        size_t posLow = 0;
        size_t posUp = 0;
        std::string substrLow = lower_copy(substr);
        std::string substrUp = upper_copy(substr);
        while (true) {
            pos = strSrc.find(substr, pos);
            if (pos != std::string::npos) {
                strSrc.erase(pos, substr.length());
            }
            posLow = strSrc.find(substrLow, posLow);
            if (posLow != std::string::npos) {
                strSrc.erase(posLow, substrLow.length());
            }
            posUp = strSrc.find(substrUp, posUp);
            if (posUp != std::string::npos) {
                strSrc.erase(posUp, substrUp.length());
            }
            if ((pos == std::string::npos && posLow == std::string::npos && posUp == std::string::npos) ||
                (pos == 0 && posLow == 0 && posUp == 0)) {
                break;
            }
            pos = 0;
            posLow = 0;
            posUp = 0;
        }
    } else {
        while (true) {
            pos = strSrc.find(substr, pos);
            if (pos != std::string::npos) {
                strSrc.erase(pos, substr.length());
            } else {
                break;
            }
        }
    }
    return strSrc;
}

std::wstring removeSubstr_copy(const std::wstring str, const std::wstring substr, const bool ignoreCase) {
    std::wstring strSrc = str;
    size_t pos = 0;
    if (ignoreCase) {
        if (str.length() >= substr.length()) {
            std::wstring ret = lower_copy(str);
            size_t pos = ret.find(lower_copy(substr));
            if (std::wstring::npos != pos) {
                ret.erase(pos, substr.length());
                return ret;
            } else {
                return str;
            }
        } else {
            return str;
        }
    } else {
        while (true) {
            pos = strSrc.find(substr, pos);
            if (pos != std::string::npos) {
                strSrc.erase(pos, substr.length());
            } else {
                break;
            }
        }
    }
    return strSrc;
}

std::string removeSubstr_copy(const std::string str, const std::vector <std::string> sequences, const bool ignoreCase) {
    std::string strSrc = str;
    for (unsigned int i = 0; i < sequences.size(); ++i) {
        size_t pos = 0;
        if (ignoreCase) {
            size_t posLow = 0;
            size_t posUp = 0;
            std::string substrLow = lower_copy(sequences[i]);
            std::string substrUp = upper_copy(sequences[i]);
            while (true) {
                pos = strSrc.find(sequences[i], pos);
                if (pos != std::string::npos) {
                    strSrc.erase(pos, sequences[i].length());
                }
                posLow = strSrc.find(substrLow, posLow);
                if (posLow != std::string::npos) {
                    strSrc.erase(posLow, substrLow.length());
                }
                posUp = strSrc.find(substrUp, posUp);
                if (posUp != std::string::npos) {
                    strSrc.erase(posUp, substrUp.length());
                }
                if ((pos == std::string::npos && posLow == std::string::npos && posUp == std::string::npos) ||
                    (pos == 0 && posLow == 0 && posUp == 0)) {
                    break;
                }
                pos = 0;
                posLow = 0;
                posUp = 0;
            }
        } else {
            while (true) {
                pos = strSrc.find(sequences[i], pos);
                if (pos != std::string::npos) {
                    strSrc.erase(pos, sequences[i].length());
                } else {
                    break;
                }
            }
        }
    }
    return strSrc;
}

std::wstring removeSubstr_copy(const std::wstring str, const std::vector <std::wstring> sequences, const bool ignoreCase) {
    std::wstring strSrc = str;
    for (unsigned int i = 0; i < sequences.size(); ++i) {
        size_t pos = 0;
        if (ignoreCase) {
            size_t posLow = 0;
            size_t posUp = 0;
            std::wstring substrLow = lower_copy(sequences[i]);
            std::wstring substrUp = upper_copy(sequences[i]);
            while (true) {
                pos = strSrc.find(sequences[i], pos);
                if (pos != std::string::npos) {
                    strSrc.erase(pos, sequences[i].length());
                }
                posLow = strSrc.find(substrLow, posLow);
                if (posLow != std::string::npos) {
                    strSrc.erase(posLow, substrLow.length());
                }
                posUp = strSrc.find(substrUp, posUp);
                if (posUp != std::string::npos) {
                    strSrc.erase(posUp, substrUp.length());
                }
                if ((pos == std::string::npos && posLow == std::string::npos && posUp == std::string::npos) ||
                    (pos == 0 && posLow == 0 && posUp == 0)) {
                    break;
                }
                pos = 0;
                posLow = 0;
                posUp = 0;
            }
        } else {
            while (true) {
                pos = strSrc.find(sequences[i], pos);
                if (pos != std::string::npos) {
                    strSrc.erase(pos, sequences[i].length());
                } else {
                    break;
                }
            }
        }
    }
    return strSrc;
}

void replaceSubstr(std::string& source,
    const std::string replaceWhat,
    const std::string replaceWith,
    const bool ignoreCase) {
    if (ignoreCase) {
        if ((replaceWhat.length() <= source.length()) && !replaceWhat.empty()) {
            size_t start_pos = 0;
            std::string sourcelow = lower_copy(source);
            std::string replacewhatlow = lower_copy(replaceWhat);
            std::string replacewithlow = lower_copy(replaceWith);
            while ((start_pos = sourcelow.find(replacewhatlow, start_pos)) !=
                std::string::npos) {
                source.replace(start_pos, replaceWhat.length(), replacewithlow);
                start_pos += replaceWith.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
        }
    } else {
        if ((replaceWhat.length() <= source.length()) && !replaceWhat.empty()) {
            size_t start_pos = 0;
            while ((start_pos = source.find(replaceWhat, start_pos)) != std::string::npos) {
                source.replace(start_pos, replaceWhat.length(), replaceWith);
                start_pos += replaceWith.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
        }
    }
}

void replaceSubstr(std::wstring& source,
    const std::wstring replaceWhat,
    const std::wstring replaceWith,
    const bool ignoreCase) {
    if (ignoreCase) {
        if ((replaceWhat.length() <= source.length()) && !replaceWhat.empty()) {
            size_t start_pos = 0;
            std::wstring sourcelow = lower_copy(source);
            std::wstring replacewhatlow = lower_copy(replaceWhat);
            std::wstring replacewithlow = lower_copy(replaceWith);
            while ((start_pos = sourcelow.find(replacewhatlow, start_pos)) !=
                std::string::npos) {
                source.replace(start_pos, replaceWhat.length(), replacewithlow);
                start_pos += replaceWith.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
        }
    } else {
        if ((replaceWhat.length() <= source.length()) && !replaceWhat.empty()) {
            size_t start_pos = 0;
            while ((start_pos = source.find(replaceWhat, start_pos)) != std::string::npos) {
                source.replace(start_pos, replaceWhat.length(), replaceWith);
                start_pos += replaceWith.length();
            }
        }
    }
}

std::string replaceSubstr_copy(const std::string source,
    const std::string replaceWhat,
    const std::string replaceWith,
    const bool ignoreCase) {
    std::string replaced = source;
    if (ignoreCase) {
        if ((replaceWhat.length() <= source.length()) && !replaceWhat.empty()) {
            size_t start_pos = 0;
            std::string sourcelow = lower_copy(source);
            std::string replacewhatlow = lower_copy(replaceWhat);
            std::string replacewithlow = lower_copy(replaceWith);
            while ((start_pos = sourcelow.find(replacewhatlow, start_pos)) !=
                std::string::npos) {
                replaced.replace(start_pos, replaceWhat.length(), replacewithlow);
                start_pos += replaceWith.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
        }
    } else {
        if ((replaceWhat.length() <= source.length()) && !replaceWhat.empty()) {
            size_t start_pos = 0;
            while ((start_pos = source.find(replaceWhat, start_pos)) != std::string::npos) {
                replaced.replace(start_pos, replaceWhat.length(), replaceWith);
                start_pos += replaceWith.length();
            }
        }
    }
    return replaced;
}

std::wstring replaceSubstr_copy(const std::wstring source,
    const std::wstring replaceWhat,
    const std::wstring replaceWith,
    const bool ignoreCase) {
    std::wstring replaced = source;
    if (ignoreCase) {
        if ((replaceWhat.length() <= source.length()) && !replaceWhat.empty()) {
            size_t start_pos = 0;
            std::wstring sourcelow = lower_copy(source);
            std::wstring replacewhatlow = lower_copy(replaceWhat);
            std::wstring replacewithlow = lower_copy(replaceWith);
            while ((start_pos = sourcelow.find(replacewhatlow, start_pos)) !=
                std::string::npos) {
                replaced.replace(start_pos, replacewhatlow.length(), replaceWith);
                start_pos += replacewithlow.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
        }
    } else {
        if ((replaceWhat.length() <= source.length()) && !replaceWhat.empty()) {
            size_t start_pos = 0;
            while ((start_pos = source.find(replaceWhat, start_pos)) != std::string::npos) {
                replaced.replace(start_pos, replaceWhat.length(), replaceWith);
                start_pos += replaceWith.length();
            }
        }
    }
    return replaced;
}

void reverseString(std::string &str) {
    if (str.length()) {
        std::reverse(str.begin(), str.end());
    }
}

void reverseString(std::wstring &str) {
    if (str.length()) {
        std::reverse(str.begin(), str.end());
    }
}

std::string reverseString_copy(const std::string str) {
    if (str.length()) {
        std::string tstr = str;
        std::reverse(tstr.begin(), tstr.end());
        return tstr;
    } else {
        return "";
    }
}

std::wstring reverseString_copy(const std::wstring str) {
    if (str.length()) {
        std::wstring tstr = str;
        std::reverse(tstr.begin(), tstr.end());
        return tstr;
    } else {
        return L"";
    }
}

void reverseIPV4(std::string &ipAddrStr) {
    std::vector<std::string> strSpl = splitStr(ipAddrStr, ".", false);
    if (4 == strSpl.size()) {
        ipAddrStr = reverseJoinStrs(strSpl, ".");
    }
}

void reverseIPV4(std::wstring &ipAddrStr) {
    std::vector<std::wstring> strSpl = splitStr(ipAddrStr, L".", false);
    if (4 == strSpl.size()) {
        ipAddrStr = reverseJoinStrs(strSpl, L".");
    }
}

std::string reverseIPV4_copy(const std::string ipAddrStr) {
    std::vector<std::string> strSpl = splitStr(ipAddrStr, ".", false);
    if (4 != strSpl.size()) {
        return ipAddrStr;
    }
    return reverseJoinStrs(strSpl, ".");
}

std::wstring reverseIPV4_copy(const std::wstring ipAddrStr) {
    std::vector<std::wstring> strSpl = splitStr(ipAddrStr, L".", false);
    if (4 != strSpl.size()) {
        return ipAddrStr;
    }
    return reverseJoinStrs(strSpl, L".");
}

void reverseIPV6(std::string &ipAddrStr) {
    char dot = '.';
    std::string ret;
    struct in6_addr addr;
    inet_pton(AF_INET6, ipAddrStr.c_str(), &addr);
    char buf[INET6_ADDRSTRLEN] = { 0 };
    sprintf(buf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        addr.s6_addr[0], addr.s6_addr[1],
        addr.s6_addr[2], addr.s6_addr[3],
        addr.s6_addr[4], addr.s6_addr[5],
        addr.s6_addr[6], addr.s6_addr[7],
        addr.s6_addr[8], addr.s6_addr[9],
        addr.s6_addr[10], addr.s6_addr[11],
        addr.s6_addr[12], addr.s6_addr[13],
        addr.s6_addr[14], addr.s6_addr[15]);
    std::string addrstr = buf;
    for (size_t i = 0; i < addrstr.size(); ++i) {
        if (i < addrstr.size() - 1) {
            ret = ret + addrstr[i] + ".";
        } else {
            ret = ret + addrstr[i];
        }
    }
    ipAddrStr = reverseString_copy(ret);
}

void reverseIPV6(std::wstring &ipAddrStr) {
    wchar_t dot = L'.';
    std::wstring ret;
    struct in6_addr addr;
    inet_pton(AF_INET6, wstr2str(ipAddrStr).c_str(), &addr);
    wchar_t buf[INET6_ADDRSTRLEN] = { 0 };
    wsprintf(buf, L"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        addr.s6_addr[0], addr.s6_addr[1],
        addr.s6_addr[2], addr.s6_addr[3],
        addr.s6_addr[4], addr.s6_addr[5],
        addr.s6_addr[6], addr.s6_addr[7],
        addr.s6_addr[8], addr.s6_addr[9],
        addr.s6_addr[10], addr.s6_addr[11],
        addr.s6_addr[12], addr.s6_addr[13],
        addr.s6_addr[14], addr.s6_addr[15]);
    std::wstring addrstr = buf;
    for (size_t i = 0; i < addrstr.size(); ++i) {
        if (i < addrstr.size() - 1) {
            ret = ret + addrstr[i] + L".";
        } else {
            ret = ret + addrstr[i];
        }
    }
    ipAddrStr = reverseString_copy(ret);
}

std::string reverseIPV6_copy(const std::string ipAddrStr) {
    char dot = '.';
    std::string ret;
    struct in6_addr addr;
    inet_pton(AF_INET6, ipAddrStr.c_str(), &addr);
    char buf[INET6_ADDRSTRLEN] = { 0 };
    sprintf(buf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        addr.s6_addr[0], addr.s6_addr[1],
        addr.s6_addr[2], addr.s6_addr[3],
        addr.s6_addr[4], addr.s6_addr[5],
        addr.s6_addr[6], addr.s6_addr[7],
        addr.s6_addr[8], addr.s6_addr[9],
        addr.s6_addr[10], addr.s6_addr[11],
        addr.s6_addr[12], addr.s6_addr[13],
        addr.s6_addr[14], addr.s6_addr[15]);
    std::string addrstr = buf;
    for (size_t i = 0; i < addrstr.size(); ++i) {
        if (i < addrstr.size() - 1) {
            ret = ret + addrstr[i] + ".";
        } else {
            ret = ret + addrstr[i];
        }
    }
    return reverseString_copy(ret);
}

std::wstring reverseIPV6_copy(const std::wstring ipAddrStr) {
    wchar_t dot = L'.';
    std::wstring ret;
    struct in6_addr addr;
    inet_pton(AF_INET6, wstr2str(ipAddrStr).c_str(), &addr);
    wchar_t buf[INET6_ADDRSTRLEN] = { 0 };
    wsprintf(buf, L"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        addr.s6_addr[0], addr.s6_addr[1],
        addr.s6_addr[2], addr.s6_addr[3],
        addr.s6_addr[4], addr.s6_addr[5],
        addr.s6_addr[6], addr.s6_addr[7],
        addr.s6_addr[8], addr.s6_addr[9],
        addr.s6_addr[10], addr.s6_addr[11],
        addr.s6_addr[12], addr.s6_addr[13],
        addr.s6_addr[14], addr.s6_addr[15]);
    std::wstring addrstr = buf;
    for (size_t i = 0; i < addrstr.size(); ++i) {
        if (i < addrstr.size() - 1) {
            ret = ret + addrstr[i] + L".";
        } else {
            ret = ret + addrstr[i];
        }
    }
    return reverseString_copy(ret);
}

std::string firstNumberFromString(const std::string line) {
    const char* digits = "0123456789";
    size_t pos = line.find_first_of(digits);
    if (std::string::npos != pos) {
        size_t epos = line.find_first_of(digits, pos);
        return line.substr(pos, epos);
    }
    return line;
}

std::wstring firstNumberFromString(const std::wstring line) {
    const wchar_t* digits = L"0123456789";
    size_t pos = line.find_first_of(digits);
    if (std::string::npos != pos) {
        size_t epos = line.find_first_of(digits, pos);
        return line.substr(pos, epos);
    }
    return line;
}

std::string wmiObjectFromQuery(const std::string query) {
    std::string querylow = lower_copy(query);
    if (!startsWith(querylow, "get-wmiobject")) {
        if (!startsWith(querylow, "gwmi")) {
            if (partialMatch(querylow, "associators")) {
                size_t opbpos = querylow.find("{");
                if (std::string::npos != opbpos) {
                    size_t cpbpos = querylow.find("}");
                    if (std::string::npos != cpbpos) {
                        size_t lng = cpbpos - opbpos - 1;
                        if (query.length() > (cpbpos + lng)) {
                            return query.substr(opbpos + 1, lng);
                        } else {
                            return query;
                        }
                    } else {
                        return query;
                    }
                } else {
                    return query;
                }
            } else {
                size_t frompos = querylow.find(" from ");
                if (std::string::npos != frompos) {
                    std::vector<std::string> qspl = splitStr(query, " ", false);
                    for (size_t i = 0; i < qspl.size(); ++i) {
                        if ("from" == lower_copy(qspl[i])) {
                            if (qspl.size() - 1 > i) {
                                return qspl[i + 1];
                            } else {
                                return query;
                            }
                        }
                    }
                } else {
                    return query;
                }
            }
        } else {
            if (querylow.length() > 6) {
                size_t qstrippos = querylow.find(" ", 6);
                if (std::string::npos != qstrippos) {
                    size_t lng = (querylow.length() - 6) - qstrippos;
                    return query.substr(6, lng);
                } else {
                    size_t lng = querylow.length() - 5;
                    return query.substr(5, lng);
                }
            } else {
                return query;
            }
        }
    } else {
        if (querylow.length() > 14) {
            size_t qstrippos = querylow.find(" ", 15);
            if (std::string::npos != qstrippos) {
                size_t lng = (querylow.length() - 15) - qstrippos;
                return query.substr(15, lng);
            } else {
                size_t lng = querylow.length() - 14;
                return query.substr(14, lng);
            }
        } else {
            return query;
        }
    }
}

std::wstring wmiObjectFromQuery(const std::wstring query) {
    std::wstring querylow = lower_copy(query);
    if (!startsWith(querylow, L"get-wmiobject")) {
        if (!startsWith(querylow, L"gwmi")) {
            if (partialMatch(querylow, L"associators")) {
                size_t opbpos = querylow.find(L"{");
                if (std::string::npos != opbpos) {
                    size_t cpbpos = querylow.find(L"}");
                    if (std::string::npos != cpbpos) {
                        size_t lng = cpbpos - opbpos - 1;
                        if (query.length() > (cpbpos + lng)) {
                            return query.substr(opbpos + 1, lng);
                        } else {
                            return query;
                        }
                    } else {
                        return query;
                    }
                } else {
                    return query;
                }
            } else {
                size_t frompos = querylow.find(L" from ");
                if (std::string::npos != frompos) {
                    std::vector<std::wstring> qspl = splitStr(query, L" ", false);
                    for (size_t i = 0; i < qspl.size(); ++i) {
                        if (L"from" == lower_copy(qspl[i])) {
                            if (qspl.size() - 1 > i) {
                                return qspl[i + 1];
                            }
                            else {
                                return query;
                            }
                        }
                    }
                } else {
                    return query;
                }
            }
        } else {
            if (querylow.length() > 6) {
                size_t qstrippos = querylow.find(L" ", 6);
                if (std::string::npos != qstrippos) {
                    size_t lng = (querylow.length() - 6) - qstrippos;
                    return query.substr(6, lng);
                } else {
                    size_t lng = querylow.length() - 5;
                    return query.substr(5, lng);
                }
            } else {
                return query;
            }
        }
    } else {
        if (querylow.length() > 14) {
            size_t qstrippos = querylow.find(L" ", 15);
            if (std::string::npos != qstrippos) {
                size_t lng = (querylow.length() - 15) - qstrippos;
                return query.substr(15, lng);
            } else {
                size_t lng = querylow.length() - 14;
                return query.substr(14, lng);
            }
        } else {
            return query;
        }
    }
}

std::string wmiQuerySimpleFromObject(const std::string objectName, const std::string namespacePath,
    const std::vector<std::string> queryFields, const bool anyQueryFields) {
    std::string queryfields;
    std::wstring queryfieldsw;
    std::vector<std::wstring> queryFlds;
    if (valInList(queryFields, "*")) {
        if (anyQueryFields) {
            queryfields = " * ";
        } else {
            WMIHandler wmih;
            if (WMIOpResult::Success == wmih.GetFieldsFromObject(queryFlds, str2wstr(objectName),
                str2wstr(namespacePath), false, true, true)) {
                queryfieldsw = joinStrs(queryFlds, L", ");
                queryfields = " " + wstr2str(queryfieldsw) + " ";
            } else {
                return objectName;
            }
        }
        return ("select" + queryfields + "from " + objectName);
    }
}

std::wstring wmiQuerySimpleFromObject(const std::wstring objectName, const std::wstring namespacePath,
    const std::vector<std::wstring> queryFields, const bool anyQueryFields) {
    std::wstring queryfieldsw;
    std::vector<std::wstring> queryFlds;
    if (valInList(queryFields, L"*")) {
        if (anyQueryFields) {
            queryfieldsw = L" * ";
        } else {
            WMIHandler wmih;
            if (WMIOpResult::Success == wmih.GetFieldsFromObject(queryFlds, objectName,
                namespacePath, false, true, true)) {
                queryfieldsw = L" " + joinStrs(queryFlds, L", ") + L" ";
            } else {
                return objectName;
            }
        }
        return (L"select" + queryfieldsw + L"from " + objectName);
    }
}

std::string genRandomString(const std::string prefix, const size_t length) {
    const char alphanum[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string tmp;
    tmp.reserve(length);
    for (int i = 0; i < length; ++i) {
        tmp = tmp + alphanum[rnd(0, 62)];
    }
    return (prefix + tmp);
}

std::wstring genRandomString(const std::wstring prefix, const size_t length) {
    const wchar_t alphanum[] =
        L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::wstring tmp;
    tmp.reserve(length);
    for (int i = 0; i < length; ++i) {
        tmp = tmp + alphanum[rnd(0, 62)];
    }
    return (prefix + tmp);
}

bool valInList(const std::vector<std::string> &list, const std::string val,
    const bool exactMatch, const bool ignoreCase, size_t *posptr) {
    if (ignoreCase) {
        for (size_t i = 0; i < list.size(); ++i) {
            if (exactMatch) {
                if (lower_copy(list[i]) == lower_copy(val)) {
                    if (posptr) {
                        *posptr = i;
                    }
                    return true;
                }
            } else {
                if (std::wstring::npos != lower_copy(list[i]).find(lower_copy(val))) {
                    if (posptr) {
                        *posptr = i;
                    }
                    return true;
                }
            }
        }
        return false;
    } else {
        for (size_t i = 0; i < list.size(); ++i) {
            if (exactMatch) {
                if (list[i] == val) {
                    if (posptr) {
                        *posptr = i;
                    }
                    return true;
                }
            } else {
                if (std::wstring::npos != list[i].find(val)) {
                    if (posptr) {
                        *posptr = i;
                    }
                    return true;
                }
            }
        }
        return false;
    }
}

bool valInList(const std::vector<std::wstring> &list, const std::wstring val,
    const bool ignoreCase, const bool exactMatch, size_t *posptr) {
    if (ignoreCase) {
        for (size_t i = 0; i < list.size(); ++i) {
            if (exactMatch) {
                if (lower_copy(list[i]) == lower_copy(val)) {
                    if (posptr) {
                        *posptr = i;
                    }
                    return true;
                }
            } else {
                if (std::wstring::npos != lower_copy(list[i]).find(lower_copy(val))) {
                    if (posptr) {
                        *posptr = i;
                    }
                    return true;
                }
            }
        }
        return false;
    } else {
        for (size_t i = 0; i < list.size(); ++i) {
            if (exactMatch) {
                if (list[i] == val) {
                    if (posptr) {
                        *posptr = i;
                    }
                    return true;
                }
            } else {
                if (std::wstring::npos != list[i].find(val)) {
                    if (posptr) {
                        *posptr = i;
                    }
                    return true;
                }
            }
        }
        return false;
    }
}

/* Returns:
   0 - not numeric
   1 - numeric w/o fract part
   2 - numeric with fract part */
unsigned char isNumericDec(const std::string str) {
    short int ret = 1; bool f = false;
    for (size_t i = 0; i < str.size(); ++i) {
        if (!(std::isdigit(str[i]))) {
            if (str[i] == 44 || str[i] == 46) {
                if (!f) {
                    f = true;
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        }
    }
    if (f) ret = 2;
    return ret;
}

/* Returns:
   0 - not numeric
   1 - numeric w/o fract part
   2 - numeric with fract part */
unsigned char isNumericDec(const std::wstring str) {
    short int ret = 1; bool f = false;
    for (size_t i = 0; i < str.size(); ++i) {
        if (!(std::isdigit(str[i]))) {
            if (str[i] == 44 || str[i] == 46) {
                if (!f) {
                    f = true;
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        }
    }
    if (f) ret = 2;
    return ret;
}

/* Returns:
   0 - ipv4
   1 - ipv6
   2 - dns name
   3 - none */
unsigned char isStringIP(const std::string testStr) {
    std::regex ipv4regex("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
    if (!(std::regex_match(testStr, ipv4regex))) {
        std::regex ipv6regex("(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))");
        if (!(std::regex_match(testStr, ipv6regex))) {
            std::regex domainregex("^(([a-zA-Z]{1})|([a-zA-Z]{1}[a-zA-Z]{1})|([a-zA-Z]{1}[0-9]{1})|([0-9]{1}[a-zA-Z]{1})|([a-zA-Z0-9][a-zA-Z0-9-_]{1,61}[a-zA-Z0-9]))\.([a-zA-Z]{2,6}|[a-zA-Z0-9-]{2,30}\.[a-zA-Z]{2,3})$");
            if (!(std::regex_match(testStr, domainregex))) {
                return 3;
            } else {
                return 2;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

/* Returns:
   0 - ipv4
   1 - ipv6
   2 - dns name
   3 - none */
unsigned char isStringIP(const std::wstring testStr) {
    std::basic_regex<wchar_t> ipv4regex(L"(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
    if (!(std::regex_match(testStr, ipv4regex))) {
        std::basic_regex<wchar_t> ipv6regex(L"(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))");
        if (!(std::regex_match(testStr, ipv6regex))) {
            // std::regex domainregex("^((?!-))(xn--)?[a-z0-9][a-z0-9-_]{0,61}[a-z0-9]{0,1}\.(xn--)?([a-z0-9\-]{1,61}|[a-z0-9-]{1,30}\.[a-z]{2,})$");
            // std::regex domainregex("[a-zA-Z0-9][-a-zA-Z0-9]+[a-zA-Z0-9].[a-z]{2,5}(.[a-z]{3,5})?(.[a-z]{2,5})?$");
            std::basic_regex<wchar_t> domainregex(L"^(([a-zA-Z]{1})|([a-zA-Z]{1}[a-zA-Z]{1})|([a-zA-Z]{1}[0-9]{1})|([0-9]{1}[a-zA-Z]{1})|([a-zA-Z0-9][a-zA-Z0-9-_]{1,61}[a-zA-Z0-9]))\.([a-zA-Z]{2,6}|[a-zA-Z0-9-]{2,30}\.[a-zA-Z]{2,3})$");
            if (!(std::regex_match(testStr, domainregex))) {
                return 3;
            } else {
                return 2;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}