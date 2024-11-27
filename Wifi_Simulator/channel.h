#ifndef CHANNEL_H
#define CHANNEL_H

class Channel {
public:
    explicit Channel(double bandwidthMHz);
    double getBandwidthMHz() const;
    bool isBusy() const;
    void setBusy(bool status);

private:
    double bandwidthMHz;
    bool busy;
};

#endif
