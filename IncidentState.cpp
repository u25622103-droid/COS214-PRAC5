#include "IncidentState.h"
#include "Incident.h"
#include <iostream>

//  Reported 
IncidentState* ReportedState::handle(Incident* incident) {
    std::cout << "[State] " << incident->getId()
              << ": Reported -> Dispatched (responders assigned).\n";
    return new DispatchedState();
}
std::string ReportedState::getStatusName() const { return "Reported"; }
bool ReportedState::allowsDispatch() const { return true; }

// Dispatched 
IncidentState* DispatchedState::handle(Incident* incident) {
    std::cout << "[State] " << incident->getId()
              << ": Dispatched -> InProgress (units on scene).\n";
    return new InProgressState();
}
std::string DispatchedState::getStatusName() const { return "Dispatched"; }
bool DispatchedState::allowsDispatch() const { return true; }

// InProgress 
IncidentState* InProgressState::handle(Incident* incident) {
    std::cout << "[State] " << incident->getId()
              << ": InProgress -> Resolved (scene made safe).\n";
    return new ResolvedState();
}
std::string InProgressState::getStatusName() const { return "InProgress"; }
bool InProgressState::allowsDispatch() const { return true; }

// Resolved 
IncidentState* ResolvedState::handle(Incident* incident) {
    std::cout << "[State] " << incident->getId()
              << " is already Resolved; no further transition.\n";
    return 0;                  
}
std::string ResolvedState::getStatusName() const { return "Resolved"; }
bool ResolvedState::allowsDispatch() const { return false; }
bool ResolvedState::isTerminal() const { return true; }
