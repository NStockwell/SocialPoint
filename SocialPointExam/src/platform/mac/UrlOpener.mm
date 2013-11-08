
#include "platform/mac/UrlOpener.hpp"

void UrlOpener::open(const std::string& url) const
{
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString: [NSString stringWithUTF8String:url.c_str()]]];
}