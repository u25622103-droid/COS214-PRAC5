#ifndef ACCESS_CONTROL_SYSTEM_H
#define ACCESS_CONTROL_SYSTEM_H

#include <string>
#include <map>
#include <vector>

class AccessZone;

/**
 * Subsystem service: the campus access-control installation.
 *
 * Owns every AccessZone. Every operation reports success, so an unknown zone
 * identifier is a handled failure rather than a silent no-op.
 */
class AccessControlSystem {
public:
    AccessControlSystem();
    ~AccessControlSystem();                        // owns all zones

    void registerZone(const std::string& zoneId,
                      const std::string& description);

    bool lockZone(const std::string& zoneId);
    bool unlockZone(const std::string& zoneId);
    bool restrictZone(const std::string& zoneId, int level);

    bool hasZone(const std::string& zoneId) const;
    std::string describeZone(const std::string& zoneId) const;

private:
    AccessZone* find(const std::string& zoneId) const;

    std::map<std::string, AccessZone*> zones_;     // owned
};

#endif
