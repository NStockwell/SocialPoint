
#ifndef __IUrlOpener_H__
#define __IUrlOpener_H__

#include <string>

/**
 * Interface to open an url
 * This is implemented in each platform and loaded into the ServiceLocator
 * @see ServiceLocator
 * @see InstructionsScene
 */
class IUrlOpener
{
public:
    
    virtual ~IUrlOpener(){};
    
    virtual void open(const std::string& url) const = 0;
};

#endif
