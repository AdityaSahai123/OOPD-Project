#ifndef USER_H
#define USER_H

#include "channel.h"

class User {
public:
    explicit User(int id);
    int getUserId() const;
    void setChannel(Channel* channel);
    bool canTransmit() const;

private:
    int userId;
    Channel* assignedChannel;
};

#endif
