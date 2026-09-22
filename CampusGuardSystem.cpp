#include "CampusGuardSystem.h"
#include "IncidentRepository.h"
#include "IncidentMediator.h"
#include "CommandInvoker.h"
#include "AccessControlSystem.h"
#include "AlertService.h"
#include "LegacyServiceAdapter.h"
#include "LegacyService.h"
#include "EmergencyFacade.h"
#include "Colleague.h"
#include "ResponseUnit.h"
#include "Incident.h"
#include "IncidentObserver.h"
#include "IncidentState.h"
#include "Command.h"
#include <iostream>

CampusGuardSystem::CampusGuardSystem()
    : repository_(new IncidentRepository()),
      mediator_(new IncidentMediator()),
      invoker_(new CommandInvoker()),
      accessControl_(new AccessControlSystem()),
      alertService_(new AlertService()),
      externalService_(0),
      facade_(0),
      securityDesk_(0), medicalDesk_(0), facilitiesDesk_(0),
      accessDesk_(0), commsDesk_(0), mutualAidDesk_(0),
      logger_(0)
{
    LegacyServiceAdapter* adapter = new LegacyServiceAdapter(new LegacyService());
    adapter->mapZone("ZONE-A", 101);        // Engineering Building A
    adapter->mapZone("ZONE-LIB", 102);      // Main Library
    externalService_ = adapter;

    facade_ = new EmergencyFacade(repository_, mediator_, invoker_,
                                  accessControl_, alertService_, externalService_);
    buildCampus();
}

CampusGuardSystem::~CampusGuardSystem() {
    // Destroy in reverse order of dependency. Each object below deletes only
    // what it owns: the repository owns incidents, each desk owns its units,
    // the invoker owns commands, access control owns zones, the adapter owns
    // the legacy service.
    delete facade_;
    for (size_t i = 0; i < colleagues_.size(); ++i) delete colleagues_[i];
    colleagues_.clear();
    for (size_t i = 0; i < observers_.size(); ++i) delete observers_[i];
    observers_.clear();
    delete externalService_;
    delete alertService_;
    delete accessControl_;
    delete invoker_;
    delete mediator_;
    delete repository_;
}

void CampusGuardSystem::buildCampus() {
    accessControl_->registerZone("ZONE-A",   "Engineering Building A");
    accessControl_->registerZone("ZONE-LIB", "Main Library");
    accessControl_->registerZone("ZONE-LAB3","Chemistry Lab 3");

    securityDesk_    = new SecurityColleague();
    medicalDesk_     = new MedicalColleague();
    facilitiesDesk_  = new FacilitiesColleague();
    accessDesk_      = new AccessControlColleague(accessControl_);
    commsDesk_       = new CommunicationColleague(alertService_);
    mutualAidDesk_   = new MutualAidColleague(externalService_);

    securityDesk_->addUnit(new SecurityTeam("SEC-01"));
    securityDesk_->addUnit(new SecurityTeam("SEC-02"));
    medicalDesk_->addUnit(new MedicalTeam("MED-01"));      // only one on shift
    facilitiesDesk_->addUnit(new FacilitiesTeam("FAC-01"));

    colleagues_.push_back(securityDesk_);
    colleagues_.push_back(medicalDesk_);
    colleagues_.push_back(facilitiesDesk_);
    colleagues_.push_back(accessDesk_);
    colleagues_.push_back(commsDesk_);
    colleagues_.push_back(mutualAidDesk_);
    for (size_t i = 0; i < colleagues_.size(); ++i)
        mediator_->registerColleague(colleagues_[i]);

    logger_ = new LoggingObserver();
    observers_.push_back(new DashboardObserver());
    observers_.push_back(new NotificationObserver(alertService_));
    observers_.push_back(logger_);

    std::cout << "[Startup] " << mediator_->colleagueCount()
              << " response components registered with the coordinator.\n";
}

void CampusGuardSystem::attachObservers(Incident* incident) {
    for (size_t i = 0; i < observers_.size(); ++i) incident->attach(observers_[i]);
}

// =====================================================================
// Scenario 1: fire in Engineering Building A.
// Patterns in one flow: Facade -> Command -> Mediator -> (Observer, State)
//                       -> Adapter.
// =====================================================================
void CampusGuardSystem::runScenarioOne() {
    std::cout << "\n==================================================\n"
              << " SCENARIO 1: Fire alarm, Engineering Building A\n"
              << "==================================================\n";

    Incident* fire = new Incident("INC-001", Fire, "Engineering Building A",
                                  "ZONE-A", 5,
                                  "Smoke reported on the third floor");
    attachObservers(fire);

    facade_->reportIncident(fire);
    facade_->respondToIncident("INC-001", facilitiesDesk_);

    std::cout << "\n--- Operator action: order evacuation ---\n";
    invoker_->issueCommand(new EvacuateZoneCommand(accessControl_, alertService_,
                                                   "ZONE-A"));
    securityDesk_->send("evacuation_ordered", fire);

    // Security follows the facilities team in.
    std::cout << "\n--- Operator action: send security as well ---\n";
    invoker_->issueCommand(new DispatchUnitCommand(securityDesk_, fire));

    facade_->resolveIncident("INC-001");
}

// =====================================================================
// Scenario 2: medical emergency while campus medical capacity is exhausted.
// Shows the failure paths and the Adapter carrying the escalation.
// =====================================================================
void CampusGuardSystem::runScenarioTwo() {
    std::cout << "\n==================================================\n"
              << " SCENARIO 2: Collapse in Chemistry Lab 3\n"
              << "==================================================\n";

    Incident* medical = new Incident("INC-002", Medical, "Chemistry Lab 3",
                                     "ZONE-LAB3", 4,
                                     "Student collapsed during a practical");
    attachObservers(medical);

    facade_->reportIncident(medical);
    facade_->respondToIncident("INC-002", medicalDesk_);

    // --- Failure case 1: duplicate incident number ---
    std::cout << "\n--- Operator error: re-filing the same incident number ---\n";
    facade_->reportIncident(new Incident("INC-002", Medical, "Chemistry Lab 3",
                                         "ZONE-LAB3", 4, "Duplicate report"));

    // --- Failure case 2: a second casualty, but MED-01 is already committed.
    // The desk refuses, tells the mediator, and mutual aid escalates through
    // the adapter to the city mainframe. ---
    std::cout << "\n--- Second casualty reported: no medical unit is free ---\n";
    invoker_->issueCommand(new DispatchUnitCommand(medicalDesk_, medical));

    // --- Failure case 3: an unknown zone identifier ---
    std::cout << "\n--- Operator error: securing a zone that does not exist ---\n";
    invoker_->issueCommand(new SecureAreaCommand(accessControl_, "ZONE-XYZ", 3));

    // --- Failure case 4: cancelling that mistaken action ---
    facade_->cancelLastAction();

    // --- Failure case 5: responding to an unknown incident ---
    facade_->respondToIncident("INC-999", securityDesk_);

    facade_->resolveIncident("INC-002");

    // --- Failure case 6: dispatching to an already resolved incident ---
    std::cout << "\n--- Operator error: dispatching to a closed incident ---\n";
    invoker_->issueCommand(new DispatchUnitCommand(securityDesk_, medical));
}

void CampusGuardSystem::printAuditTrail() const {
    std::cout << "\n==================================================\n"
              << " END OF SHIFT SUMMARY\n"
              << "==================================================\n";
    repository_->printSummary();
    invoker_->printHistory();
    std::cout << "[AuditLog] " << logger_->getLog().size()
              << " status change(s) recorded.\n";
    const std::vector<std::string>& entries = logger_->getLog();
    for (size_t i = 0; i < entries.size(); ++i)
        std::cout << "    " << entries[i] << "\n";
    std::cout << "[AlertService] " << alertService_->getMessageCount()
              << " message(s) sent.\n";
    std::cout << "[CityServices] ZONE-A is: "
              << externalService_->getZoneStatus("ZONE-A") << "\n";
}
