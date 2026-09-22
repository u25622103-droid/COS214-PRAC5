#include "IncidentRepository.h"
#include "Incident.h"
#include <iostream>

IncidentRepository::IncidentRepository() {}

IncidentRepository::~IncidentRepository() {
    for (size_t i = 0; i < incidents_.size(); ++i) delete incidents_[i];
    incidents_.clear();
}

bool IncidentRepository::addIncident(Incident* incident) {
    if (!incident) return false;
    if (findIncident(incident->getId()) != 0) {
        std::cout << "[Repository] REFUSED: incident " << incident->getId()
                  << " is already registered.\n";
        delete incident;           
        return false;
    }
    incidents_.push_back(incident);
    return true;
}

Incident* IncidentRepository::findIncident(const std::string& id) const {
    for (size_t i = 0; i < incidents_.size(); ++i)
        if (incidents_[i]->getId() == id) return incidents_[i];
    return 0;
}

const std::vector<Incident*>& IncidentRepository::getAllIncidents() const {
    return incidents_;
}

void IncidentRepository::printSummary() const {
    std::cout << "[Repository] " << incidents_.size() << " incident(s) on file:\n";
    for (size_t i = 0; i < incidents_.size(); ++i) {
        std::cout << "    " << incidents_[i]->getId() << " - "
                  << incidents_[i]->getTypeName() << " @ "
                  << incidents_[i]->getLocation() << " - "
                  << incidents_[i]->getStatusName() << "\n";
    }
}
