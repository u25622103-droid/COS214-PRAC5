#include "EmergencyFacade.h"
#include "IncidentRepository.h"
#include "IncidentMediator.h"
#include "CommandInvoker.h"
#include "Command.h"
#include "AccessControlSystem.h"
#include "AlertService.h"
#include "CampusGuardService.h"
#include "Incident.h"
#include "IncidentState.h"
#include "Colleague.h"
#include "ResponseUnit.h"
#include <iostream>

EmergencyFacade::EmergencyFacade(IncidentRepository* repository,
                                 IncidentMediator* mediator,
                                 CommandInvoker* invoker,
                                 AccessControlSystem* accessControl,
                                 AlertService* alertService,
                                 CampusGuardService* externalService)
    : repository_(repository), mediator_(mediator), invoker_(invoker),
      accessControl_(accessControl), alertService_(alertService),
      externalService_(externalService) {}

EmergencyFacade::~EmergencyFacade() {
 
}

bool EmergencyFacade::reportIncident(Incident* incident) {
    if (!incident) return false;
    std::cout << "\n--- Facade::reportIncident ---\n";

    if (!repository_->addIncident(incident)) return false;

    incident->setState(new ReportedState());

    alertService_->sendToZone(incident->getZoneId(),
                              "An incident has been reported in your area.");

    std::cout << "[Facade] Zone check: "
              << accessControl_->describeZone(incident->getZoneId()) << "\n";

    return true;
}

bool EmergencyFacade::respondToIncident(const std::string& incidentId,
                                        DispatchColleague* desk) {
    std::cout << "\n--- Facade::respondToIncident ---\n";
    Incident* incident = repository_->findIncident(incidentId);
    if (!incident) {
        std::cout << "[Facade] REFUSED: no incident with id '" << incidentId
                  << "' is on file.\n";
        return false;
    }
    if (!desk) {
        std::cout << "[Facade] REFUSED: no dispatch desk was nominated.\n";
        return false;
    }

    invoker_->issueCommand(new ActivateAlertCommand(
        alertService_, "Emergency response underway for " + incident->getId()
                       + " at " + incident->getLocation()));

    bool dispatched = invoker_->issueCommand(
        new DispatchUnitCommand(desk, incident));

    invoker_->issueCommand(new SecureAreaCommand(accessControl_,
                                                 incident->getZoneId(), 3));

    if (!externalService_->secureZone(incident->getZoneId())) {
        std::cout << "[Facade] City lockdown unavailable for this zone; "
                     "campus access control remains in force.\n";
    }

    if (dispatched) incident->updateStatus();     

    return dispatched;
}

bool EmergencyFacade::resolveIncident(const std::string& incidentId) {
    std::cout << "\n--- Facade::resolveIncident ---\n";
    Incident* incident = repository_->findIncident(incidentId);
    if (!incident) {
        std::cout << "[Facade] REFUSED: no incident with id '" << incidentId
                  << "' is on file.\n";
        return false;
    }

    while (incident->getState() && !incident->getState()->isTerminal())
        incident->updateStatus();

    const std::vector<ResponseUnit*>& units = incident->getAssignedUnits();
    for (size_t i = 0; i < units.size(); ++i) units[i]->standDown();

    mediator_->notify(0, "incident_resolved", incident);

    return true;
}

bool EmergencyFacade::cancelLastAction() {
    std::cout << "\n--- Facade::cancelLastAction ---\n";
    return invoker_->issueCommand(new CancelLastActionCommand(invoker_));
}
