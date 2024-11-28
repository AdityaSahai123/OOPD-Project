#include "user.h"
#include "channel.h"

User::User(int id) : userId(id), assignedChannel(nullptr) {}

int User::getUserId() const {
    return userId;
}

void User::setChannel(Channel* channel) {
    assignedChannel = channel;
}

bool User::canTransmit() const {
    //invoking paranthesis
    return assignedChannel && !assignedChannel->isBusy();
}
