#ifndef CAMPUSGUARD_SYSTEM_H
#define CAMPUSGUARD_SYSTEM_H

#include <vector>

class IncidentRepository;
class IncidentMediator;
class CommandInvoker;
class AccessControlSystem;
class AlertService;
class CampusGuardService;
class EmergencyFacade;
class Colleague;
class IncidentObserver;
class SecurityColleague;
class MedicalColleague;
class FacilitiesColleague;
class AccessControlColleague;
class CommunicationColleague;
class MutualAidColleague;
class LoggingObserver;

/**
 * Composition root. Builds the object graph, owns everything that has no more
 * natural owner, and runs the two demonstration scenarios.
 */
class CampusGuardSystem {
public:
    CampusGuardSystem();
    ~CampusGuardSystem();

    void runScenarioOne();
    void runScenarioTwo();
    void printAuditTrail() const;

private:
    CampusGuardSystem(const CampusGuardSystem&);
    CampusGuardSystem& operator=(const CampusGuardSystem&);

    void buildCampus();
    void attachObservers(class Incident* incident);

    IncidentRepository* repository_;        // owned
    IncidentMediator* mediator_;            // owned
    CommandInvoker* invoker_;               // owned
    AccessControlSystem* accessControl_;    // owned
    AlertService* alertService_;            // owned
    CampusGuardService* externalService_;   // owned (adapter owns the adaptee)
    EmergencyFacade* facade_;               // owned

    SecurityColleague* securityDesk_;       // owned via colleagues_
    MedicalColleague* medicalDesk_;
    FacilitiesColleague* facilitiesDesk_;
    AccessControlColleague* accessDesk_;
    CommunicationColleague* commsDesk_;
    MutualAidColleague* mutualAidDesk_;

    LoggingObserver* logger_;               // owned via observers_

    std::vector<Colleague*> colleagues_;            // owned
    std::vector<IncidentObserver*> observers_;      // owned
};

#endif
