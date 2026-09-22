#include "LegacyServiceAdapter.h"
#include <iostream>

LegacyServiceAdapter::LegacyServiceAdapter(LegacyService* legacy)
    : legacy_(legacy) {}

LegacyServiceAdapter::~LegacyServiceAdapter() {
    delete legacy_;               
}

void LegacyServiceAdapter::mapZone(const std::string& zoneId, int zoneCode) {
    zoneCodes_[zoneId] = zoneCode;
}

int LegacyServiceAdapter::toUnitCode(const std::string& unitType) const {
    if (unitType == "Security")   return 11;
    if (unitType == "Medical")    return 22;
    if (unitType == "Facilities") return 33;
    return -1;
}

int LegacyServiceAdapter::toZoneCode(const std::string& zoneId) const {
    std::map<std::string, int>::const_iterator it = zoneCodes_.find(zoneId);
    if (it == zoneCodes_.end()) return -1;
    return it->second;
}

bool LegacyServiceAdapter::dispatchUnit(const std::string& unitType,
                                        const std::string& location) {
    if (!legacy_) return false;
    int code = toUnitCode(unitType);
    if (code < 0) {
        std::cout << "[Adapter] Cannot translate unit type '" << unitType
                  << "' for the city mainframe - request refused.\n";
        return false;
    }
    std::cout << "[Adapter] dispatchUnit(\"" << unitType << "\", \"" << location
              << "\") -> legacyDispatch(" << code << ", ...)\n";
    return legacy_->legacyDispatch(code, location.c_str()) == 0;
}

bool LegacyServiceAdapter::secureZone(const std::string& zoneId) {
    if (!legacy_) return false;
    int code = toZoneCode(zoneId);
    if (code < 0) {
        std::cout << "[Adapter] Zone '" << zoneId
                  << "' has no city mainframe code - lockdown refused.\n";
        return false;
    }
    std::cout << "[Adapter] secureZone(\"" << zoneId
              << "\") -> legacyLockdown(" << code << ")\n";
    return legacy_->legacyLockdown(code) == 0;
}

bool LegacyServiceAdapter::releaseZone(const std::string& zoneId) {
    if (!legacy_) return false;
    int code = toZoneCode(zoneId);
    if (code < 0) {
        std::cout << "[Adapter] Zone '" << zoneId
                  << "' has no city mainframe code - release refused.\n";
        return false;
    }
    std::cout << "[Adapter] releaseZone(\"" << zoneId
              << "\") -> legacyRelease(" << code << ")\n";
    return legacy_->legacyRelease(code) == 0;
}

std::string LegacyServiceAdapter::getZoneStatus(const std::string& zoneId) const {
    if (!legacy_) return "Unavailable";
    int code = toZoneCode(zoneId);
    if (code < 0) return "Unknown to city services";
    int status = legacy_->legacyStatus(code);      // int -> readable text
    if (status == 1)  return "Locked by city services";
    if (status == 0)  return "Open";
    return "Unknown to city services";
}

std::string LegacyServiceAdapter::getServiceName() const {
    return "City Emergency Services (legacy mainframe)";
}
