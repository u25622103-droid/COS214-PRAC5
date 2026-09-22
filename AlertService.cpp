#include "AlertService.h"
#include <iostream>

AlertService::AlertService() : messageCount_(0) {}

void AlertService::broadcast(const std::string& message) {
    ++messageCount_;
    std::cout << "[AlertService] CAMPUS BROADCAST: " << message << "\n";
}

void AlertService::sendToZone(const std::string& zoneId,
                              const std::string& message) {
    ++messageCount_;
    std::cout << "[AlertService] -> " << zoneId << ": " << message << "\n";
}

void AlertService::retract(const std::string& message) {
    ++messageCount_;
    std::cout << "[AlertService] RETRACTION: \"" << message
              << "\" is no longer in effect.\n";
}

int AlertService::getMessageCount() const { return messageCount_; }
