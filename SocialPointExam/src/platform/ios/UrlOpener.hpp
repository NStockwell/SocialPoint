

#ifndef __SocialPointExam__UrlOpener__
#define __SocialPointExam__UrlOpener__

#include "service/IUrlOpener.hpp"

/**
 * iOS implementation to open urls
 * @see IUrlOpener
 */
class UrlOpener : public IUrlOpener
{
public:
    void open(const std::string& url) const;
};

#endif /* defined(__SocialPointExam__UrlOpener__) */
