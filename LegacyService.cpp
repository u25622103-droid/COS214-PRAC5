#include "LegacyService.h"
#include <iostream>

LegacyService::LegacyService() : lockedZoneCode_(-1) {}
LegacyService::~LegacyService() {}

int LegacyService::legacyDispatch(int unitCode, const char* location) {
    if (unitCode <= 0 || location == 0) {
        std::cout << "[CityMainframe] DISPATCH REJECTED (code=" << unitCode << ")\n";
        return -1;
    }
    std::cout << "[CityMainframe] DSP|" << unitCode << "|" << location << "|ACK\n";
    return 0;
}

int LegacyService::legacyLockdown(int zoneCode) {
    if (zoneCode <= 0) {
        std::cout << "[CityMainframe] LCK|" << zoneCode << "|ERR-UNKNOWN-ZONE\n";
        return -1;
    }
    lockedZoneCode_ = zoneCode;
    std::cout << "[CityMainframe] LCK|" << zoneCode << "|ACK\n";
    return 0;
}

int LegacyService::legacyRelease(int zoneCode) {
    if (zoneCode <= 0) {
        std::cout << "[CityMainframe] REL|" << zoneCode << "|ERR-UNKNOWN-ZONE\n";
        return -1;
    }
    if (lockedZoneCode_ == zoneCode) lockedZoneCode_ = -1;
    std::cout << "[CityMainframe] REL|" << zoneCode << "|ACK\n";
    return 0;
}

int LegacyService::legacyStatus(int zoneCode) const {
    if (zoneCode <= 0) return -1;
    return (lockedZoneCode_ == zoneCode) ? 1 : 0;
}
