#ifndef ACCESS_POINT_H
#define ACCESS_POINT_H

#include <memory>
#include "channel.h"

class AccessPoint {
public:
    explicit AccessPoint(double bandwidthMHz);
    std::shared_ptr<Channel> getChannel();

private:
    std::shared_ptr<Channel> channel;
};

#endif
