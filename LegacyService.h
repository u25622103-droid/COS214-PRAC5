#ifndef LEGACY_SERVICE_H
#define LEGACY_SERVICE_H


class LegacyService {
public:
    LegacyService();
    ~LegacyService();

    // Result codes: 0 = accepted, -1 = unknown code / rejected.
    int legacyDispatch(int unitCode, const char* location);
    int legacyLockdown(int zoneCode);
    int legacyRelease(int zoneCode);

    // Status codes: 0 = open, 1 = locked, -1 = zone not known to mainframe.
    int legacyStatus(int zoneCode) const;

private:
    int lockedZoneCode_;  
};

#endif
