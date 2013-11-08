
#include "platform/win/UrlOpener.hpp"
#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>

void UrlOpener::open(const std::string& url) const
{
   ShellExecute(NULL, "open", url.c_str(),NULL, NULL, SW_SHOWDEFAULT);
}