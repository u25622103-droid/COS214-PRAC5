#include "AccessZone.h"
#include <iostream>
#include <sstream>

AccessZone::AccessZone(const std::string& zoneId, const std::string& description)
    : zoneId_(zoneId), description_(description), locked_(false), accessLevel_(0) {}

void AccessZone::lock() {
    locked_ = true;
    std::cout << "[AccessZone " << zoneId_ << "] doors LOCKED.\n";
}

void AccessZone::unlock() {
    locked_ = false;
    accessLevel_ = 0;
    std::cout << "[AccessZone " << zoneId_ << "] doors UNLOCKED, access restored.\n";
}

void AccessZone::restrict(int level) {
    accessLevel_ = level;
    std::cout << "[AccessZone " << zoneId_ << "] access restricted to level "
              << level << " (responders only).\n";
}

bool AccessZone::isLocked() const { return locked_; }
int AccessZone::getAccessLevel() const { return accessLevel_; }

std::string AccessZone::getStatus() const {
    std::ostringstream oss;
    oss << (locked_ ? "Locked" : "Unlocked") << ", level " << accessLevel_;
    return oss.str();
}

const std::string& AccessZone::getZoneId() const { return zoneId_; }
const std::string& AccessZone::getDescription() const { return description_; }
