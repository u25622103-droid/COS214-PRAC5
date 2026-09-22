#ifndef EMERGENCY_FACADE_H
#define EMERGENCY_FACADE_H

#include <string>

class IncidentMediator;
class CommandInvoker;
class AccessControlSystem;
class AlertService;
class CampusGuardService;
class IncidentRepository;
class Incident;
class DispatchColleague;


class EmergencyFacade {
public:
    EmergencyFacade(IncidentRepository* repository,
                    IncidentMediator* mediator,
                    CommandInvoker* invoker,
                    AccessControlSystem* accessControl,
                    AlertService* alertService,
                    CampusGuardService* externalService);
    ~EmergencyFacade();

    /* Repository + State + Observer + Alert + Mediator. */
    bool reportIncident(Incident* incident);

    /* Command x3 + Mediator + Adapter + State + Observer. */
    bool respondToIncident(const std::string& incidentId, DispatchColleague* desk);

    /* Alert + Access control + Adapter + units stood down + State. */
    bool resolveIncident(const std::string& incidentId);

    /* Deliberate operator mistake handling. */
    bool cancelLastAction();

private:
    IncidentRepository* repository_;       
    IncidentMediator* mediator_;          
    CommandInvoker* invoker_;            
    AccessControlSystem* accessControl_;    
    AlertService* alertService_;            
    CampusGuardService* externalService_;  
};

#endif
