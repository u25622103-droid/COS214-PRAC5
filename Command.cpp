#include "Command.h"
#include "CommandInvoker.h"
#include "Incident.h"
#include "Colleague.h"
#include "AccessControlSystem.h"
#include "AlertService.h"
#include "ResponseUnit.h"
#include "CampusGuardService.h"
#include <iostream>

DispatchUnitCommand::DispatchUnitCommand(DispatchColleague* desk, Incident* incident)
    : desk_(desk), incident_(incident), deployed_(0) {}

bool DispatchUnitCommand::execute() {
    if (!desk_ || !incident_) {
        std::cout << "[Command] DispatchUnitCommand is missing a receiver.\n";
        return false;
    }
    std::cout << "[Command] " << getName() << " -> " << desk_->getName() << "\n";
    deployed_ = desk_->dispatchTo(incident_);    
    return deployed_ != 0;
}

void DispatchUnitCommand::undo() {
    if (!deployed_ || !desk_) {
        std::cout << "[Command] Nothing to recall for " << getName() << ".\n";
        return;
    }
    std::cout << "[Command] Undoing " << getName() << "\n";
    desk_->recall(deployed_, incident_);
    deployed_ = 0;
}

std::string DispatchUnitCommand::getName() const { return "DispatchUnitCommand"; }

SecureAreaCommand::SecureAreaCommand(AccessControlSystem* accessControl,
                                     const std::string& zoneId, int level)
    : accessControl_(accessControl), zoneId_(zoneId), level_(level), applied_(false) {}

bool SecureAreaCommand::execute() {
    if (!accessControl_) return false;
    std::cout << "[Command] " << getName() << " -> Access Control (" << zoneId_ << ")\n";
    applied_ = accessControl_->restrictZone(zoneId_, level_);
    return applied_;
}

void SecureAreaCommand::undo() {
    if (!applied_ || !accessControl_) {
        std::cout << "[Command] " << getName() << " was never applied; nothing to undo.\n";
        return;
    }
    std::cout << "[Command] Undoing " << getName() << "\n";
    accessControl_->restrictZone(zoneId_, 0);
    applied_ = false;
}

std::string SecureAreaCommand::getName() const { return "SecureAreaCommand"; }


EvacuateZoneCommand::EvacuateZoneCommand(AccessControlSystem* accessControl,
                                         AlertService* alertService,
                                         const std::string& zoneId)
    : accessControl_(accessControl), alertService_(alertService),
      zoneId_(zoneId), applied_(false) {}

bool EvacuateZoneCommand::execute() {
    if (!accessControl_ || !alertService_) return false;
    std::cout << "[Command] " << getName() << " -> Access Control + Alerts ("
              << zoneId_ << ")\n";
    if (!accessControl_->lockZone(zoneId_)) return false;   
    alertService_->sendToZone(zoneId_, "EVACUATE NOW - use the nearest fire exit.");
    applied_ = true;
    return true;
}

void EvacuateZoneCommand::undo() {
    if (!applied_) {
        std::cout << "[Command] " << getName() << " was never applied; nothing to undo.\n";
        return;
    }
    std::cout << "[Command] Undoing " << getName() << "\n";
    accessControl_->unlockZone(zoneId_);
    alertService_->sendToZone(zoneId_, "Evacuation order withdrawn.");
    applied_ = false;
}

std::string EvacuateZoneCommand::getName() const { return "EvacuateZoneCommand"; }


ActivateAlertCommand::ActivateAlertCommand(AlertService* alertService,
                                           const std::string& message)
    : alertService_(alertService), message_(message), applied_(false) {}

bool ActivateAlertCommand::execute() {
    if (!alertService_) return false;
    std::cout << "[Command] " << getName() << " -> Alert Service\n";
    alertService_->broadcast(message_);
    applied_ = true;
    return true;
}

void ActivateAlertCommand::undo() {
    if (!applied_) return;
    std::cout << "[Command] Undoing " << getName() << "\n";
    alertService_->retract(message_);
    applied_ = false;
}

std::string ActivateAlertCommand::getName() const { return "ActivateAlertCommand"; }


RequestExternalAssistCommand::RequestExternalAssistCommand(CampusGuardService* service,
                                                           const std::string& unitType,
                                                           const std::string& location)
    : service_(service), unitType_(unitType), location_(location), applied_(false) {}

bool RequestExternalAssistCommand::execute() {
    if (!service_) return false;
    std::cout << "[Command] " << getName() << " -> " << service_->getServiceName() << "\n";
    applied_ = service_->dispatchUnit(unitType_, location_);
    return applied_;
}

void RequestExternalAssistCommand::undo() {
    std::cout << "[Command] " << getName()
              << " cannot be undone: the city mainframe has no recall message. "
                 "A cancellation must be phoned through.\n";
}

std::string RequestExternalAssistCommand::getName() const {
    return "RequestExternalAssistCommand";
}


CancelLastActionCommand::CancelLastActionCommand(CommandInvoker* invoker)
    : invoker_(invoker) {}

bool CancelLastActionCommand::execute() {
    if (!invoker_) return false;
    std::cout << "[Command] " << getName() << " -> Operator Console\n";
    return invoker_->cancelLast();
}

void CancelLastActionCommand::undo() {
    std::cout << "[Command] A cancellation cannot itself be cancelled.\n";
}

std::string CancelLastActionCommand::getName() const {
    return "CancelLastActionCommand";
}
