#include "util_string.h"

#include <codecvt>
#include <locale>

namespace dxvk::str {
  std::string fromws(const WCHAR *ws) {
#ifdef _WIN32
    size_t len = ::WideCharToMultiByte(CP_UTF8,
      0, ws, -1, nullptr, 0, nullptr, nullptr);

    if (len <= 1)
      return "";

    len -= 1;

    std::string result;
    result.resize(len);
    ::WideCharToMultiByte(CP_UTF8, 0, ws, -1,
      &result.at(0), len, nullptr, nullptr);
    return result;
#else
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(ws);
#endif
  }


  void tows(const char* mbs, WCHAR* wcs, size_t wcsLen) {
#ifdef _WIN32
    ::MultiByteToWideChar(
      CP_UTF8, 0, mbs, -1,
      wcs, wcsLen);
#else
    std::wstring result = tows(mbs);
    wcslcpy(wcs, result.c_str(), wcsLen);
#endif
  }

  std::wstring tows(const char* mbs) {
#ifdef _WIN32
    size_t len = ::MultiByteToWideChar(CP_UTF8,
      0, mbs, -1, nullptr, 0);
    
    if (len <= 1)
      return L"";

    len -= 1;

    std::wstring result;
    result.resize(len);
    ::MultiByteToWideChar(CP_UTF8, 0, mbs, -1,
      &result.at(0), len);
    return result;
#else
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(mbs);
#endif
  }

}
