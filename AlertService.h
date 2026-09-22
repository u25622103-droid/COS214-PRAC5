#ifndef ALERT_SERVICE_H
#define ALERT_SERVICE_H

#include <string>

/** Subsystem service: campus-wide and zone-targeted messaging. */
class AlertService {
public:
    AlertService();

    void broadcast(const std::string& message);
    void sendToZone(const std::string& zoneId, const std::string& message);
    void retract(const std::string& message);

    int getMessageCount() const;

private:
    int messageCount_;
};

#endif
