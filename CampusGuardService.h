#ifndef CAMPUSGUARD_SERVICE_H
#define CAMPUSGUARD_SERVICE_H

#include <string>

/**
 * Target interface (Adapter pattern).
 *
 * This is the interface CampusGuard *wants* to talk to: string identifiers,
 * boolean success reporting and readable status text. Any external or legacy
 * provider must be made to fit this shape by an Adapter.
 */
class CampusGuardService {
public:
    virtual ~CampusGuardService() {}

    virtual bool dispatchUnit(const std::string& unitType,
                              const std::string& location) = 0;
    virtual bool secureZone(const std::string& zoneId) = 0;
    virtual bool releaseZone(const std::string& zoneId) = 0;
    virtual std::string getZoneStatus(const std::string& zoneId) const = 0;
    virtual std::string getServiceName() const = 0;
};

#endif
