#include "access_point.h"

AccessPoint::AccessPoint(double bandwidthMHz) {
    channel = std::make_shared<Channel>(bandwidthMHz);
}

std::shared_ptr<Channel> AccessPoint::getChannel() {
    return channel;
}
