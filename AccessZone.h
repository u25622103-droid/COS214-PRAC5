#ifndef ACCESS_ZONE_H
#define ACCESS_ZONE_H

#include <string>

/** A controllable physical area of campus. Owned by AccessControlSystem. */
class AccessZone {
public:
    AccessZone(const std::string& zoneId, const std::string& description);

    void lock();
    void unlock();
    void restrict(int level);

    bool isLocked() const;
    int getAccessLevel() const;
    std::string getStatus() const;
    const std::string& getZoneId() const;
    const std::string& getDescription() const;

private:
    std::string zoneId_;
    std::string description_;
    bool locked_;
    int accessLevel_;
};

#endif
