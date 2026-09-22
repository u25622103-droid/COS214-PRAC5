#ifndef LEGACY_SERVICE_ADAPTER_H
#define LEGACY_SERVICE_ADAPTER_H

#include "CampusGuardService.h"
#include "LegacyService.h"
#include <string>
#include <map>

class LegacyServiceAdapter : public CampusGuardService {
public:
    explicit LegacyServiceAdapter(LegacyService* legacy); 
    ~LegacyServiceAdapter() override;

    bool dispatchUnit(const std::string& unitType,
                      const std::string& location) override;
    bool secureZone(const std::string& zoneId) override;
    bool releaseZone(const std::string& zoneId) override;
    std::string getZoneStatus(const std::string& zoneId) const override;
    std::string getServiceName() const override;

    void mapZone(const std::string& zoneId, int zoneCode);

private:
    int toUnitCode(const std::string& unitType) const;
    int toZoneCode(const std::string& zoneId) const;   // -1 when unmapped

    LegacyService* legacy_;             
    std::map<std::string, int> zoneCodes_;
};

#endif
