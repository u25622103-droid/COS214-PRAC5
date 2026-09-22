#include "IncidentObserver.h"
#include "Incident.h"
#include "AlertService.h"
#include <iostream>
#include <sstream>

void DashboardObserver::update(Incident* incident) {
    if (!incident) return;
    std::cout << "[Dashboard] " << incident->getId() << " ("
              << incident->getTypeName() << ", severity "
              << incident->getSeverity() << ") @ " << incident->getLocation()
              << " -> " << incident->getStatusName() << "\n";
}

NotificationObserver::NotificationObserver(AlertService* alertService)
    : alertService_(alertService) {}

void NotificationObserver::update(Incident* incident) {
    if (!incident || !alertService_) return;
    alertService_->sendToZone(incident->getZoneId(),
                              "Incident " + incident->getId() + " is now "
                              + incident->getStatusName());
}

void LoggingObserver::update(Incident* incident) {
    if (!incident) return;
    std::ostringstream oss;
    oss << incident->getId() << " | " << incident->getStatusName()
        << " | " << incident->getLocation();
    log_.push_back(oss.str());
    std::cout << "[AuditLog] recorded: " << oss.str() << "\n";
}

const std::vector<std::string>& LoggingObserver::getLog() const { return log_; }
