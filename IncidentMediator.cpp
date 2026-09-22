#include "IncidentMediator.h"
#include "Colleague.h"
#include "Incident.h"
#include "ResponseUnit.h"
#include <iostream>

IncidentMediator::IncidentMediator() {}

IncidentMediator::~IncidentMediator() {
   
}

void IncidentMediator::registerColleague(Colleague* colleague) {
    if (!colleague) return;
    colleagues_.push_back(colleague);
    colleague->setMediator(this);
}

void IncidentMediator::notify(Colleague* sender, const std::string& event,
                              Incident* incident) {
    std::cout << "[Mediator] '" << event << "' raised by "
              << (sender ? sender->getName() : "system")
              << (incident ? (" for " + incident->getId()) : std::string())
              << " -> informing " << (int)colleagues_.size() - (sender ? 1 : 0)
              << " component(s).\n";
    for (size_t i = 0; i < colleagues_.size(); ++i) {
        if (colleagues_[i] && colleagues_[i] != sender)
            colleagues_[i]->receive(event, incident);
    }
}

void IncidentMediator::coordinateDispatch(Colleague* sender, Incident* incident,
                                          ResponseUnit* unit) {
    if (!incident || !unit) {
        std::cout << "[Mediator] Cannot coordinate a dispatch without both an "
                     "incident and a unit.\n";
        return;
    }
    std::cout << "[Mediator] Coordinating " << unit->getUnitType() << " unit "
              << unit->getUnitId() << " on scene at " << incident->getLocation()
              << ".\n";
    incident->assignUnit(unit);
    notify(sender, "unit_dispatched", incident);
}

int IncidentMediator::colleagueCount() const { return (int)colleagues_.size(); }
