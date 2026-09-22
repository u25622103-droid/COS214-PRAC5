#include "AccessControlSystem.h"
#include "AccessZone.h"
#include <iostream>

AccessControlSystem::AccessControlSystem() {}

AccessControlSystem::~AccessControlSystem() {
    std::map<std::string, AccessZone*>::iterator it;
    for (it = zones_.begin(); it != zones_.end(); ++it) delete it->second;
    zones_.clear();
}

void AccessControlSystem::registerZone(const std::string& zoneId,
                                       const std::string& description) {
    if (zones_.find(zoneId) != zones_.end()) return;
    zones_[zoneId] = new AccessZone(zoneId, description);
}

AccessZone* AccessControlSystem::find(const std::string& zoneId) const {
    std::map<std::string, AccessZone*>::const_iterator it = zones_.find(zoneId);
    return (it == zones_.end()) ? 0 : it->second;
}

bool AccessControlSystem::hasZone(const std::string& zoneId) const {
    return find(zoneId) != 0;
}

bool AccessControlSystem::lockZone(const std::string& zoneId) {
    AccessZone* z = find(zoneId);
    if (!z) {
        std::cout << "[AccessControl] REFUSED: no zone '" << zoneId
                  << "' is registered on campus.\n";
        return false;
    }
    z->lock();
    return true;
}

bool AccessControlSystem::unlockZone(const std::string& zoneId) {
    AccessZone* z = find(zoneId);
    if (!z) {
        std::cout << "[AccessControl] REFUSED: no zone '" << zoneId
                  << "' is registered on campus.\n";
        return false;
    }
    z->unlock();
    return true;
}

bool AccessControlSystem::restrictZone(const std::string& zoneId, int level) {
    AccessZone* z = find(zoneId);
    if (!z) {
        std::cout << "[AccessControl] REFUSED: no zone '" << zoneId
                  << "' is registered on campus.\n";
        return false;
    }
    z->restrict(level);
    return true;
}

std::string AccessControlSystem::describeZone(const std::string& zoneId) const {
    AccessZone* z = find(zoneId);
    if (!z) return "unregistered zone";
    return z->getDescription() + " [" + z->getStatus() + "]";
}
