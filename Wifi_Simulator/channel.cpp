#include "channel.h"

Channel::Channel(double bandwidthMHz) : bandwidthMHz(bandwidthMHz), busy(false) {}

double Channel::getBandwidthMHz() const {
    return bandwidthMHz;
}

bool Channel::isBusy() const {
    return busy;
}

void Channel::setBusy(bool status) {
    busy = status;
}
