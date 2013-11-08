
#include "platform/ios/UrlOpener.hpp"

void UrlOpener::open(const std::string& url) const
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString: [NSString stringWithUTF8String:url.c_str()]]];
}