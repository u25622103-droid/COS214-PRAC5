#include "Colleague.h"
#include "IncidentMediator.h"
#include "Incident.h"
#include "ResponseUnit.h"
#include "AccessControlSystem.h"
#include "AlertService.h"
#include "CampusGuardService.h"
#include <iostream>
#include <algorithm>

// ===================== Colleague =====================
Colleague::Colleague(const std::string& name, IncidentMediator* mediator)
    : name_(name), mediator_(mediator) {}

void Colleague::setMediator(IncidentMediator* mediator) { mediator_ = mediator; }
const std::string& Colleague::getName() const { return name_; }

void Colleague::send(const std::string& event, Incident* incident) {
    if (!mediator_) {
        std::cout << "[" << name_ << "] has no mediator; '" << event
                  << "' cannot be coordinated.\n";
        return;
    }
    mediator_->notify(this, event, incident);
}

// ===================== DispatchColleague =====================
DispatchColleague::DispatchColleague(const std::string& name,
                                     const std::string& unitType,
                                     IncidentMediator* mediator)
    : Colleague(name, mediator), unitType_(unitType) {}

DispatchColleague::~DispatchColleague() {
    for (size_t i = 0; i < units_.size(); ++i) delete units_[i];
    units_.clear();
}

void DispatchColleague::addUnit(ResponseUnit* unit) {
    if (unit) units_.push_back(unit);
}

ResponseUnit* DispatchColleague::findAvailableUnit() const {
    for (size_t i = 0; i < units_.size(); ++i)
        if (units_[i]->isAvailable()) return units_[i];
    return 0;
}

const std::string& DispatchColleague::getUnitType() const { return unitType_; }

int DispatchColleague::availableCount() const {
    int n = 0;
    for (size_t i = 0; i < units_.size(); ++i) if (units_[i]->isAvailable()) ++n;
    return n;
}

ResponseUnit* DispatchColleague::dispatchTo(Incident* incident) {
    if (!incident) return 0;
    if (!incident->canDispatch()) {
        std::cout << "[" << name_ << "] REFUSED: " << incident->getId()
                  << " is " << incident->getStatusName()
                  << "; no further units will be sent.\n";
        return 0;
    }
    ResponseUnit* unit = findAvailableUnit();
    if (!unit) {
        std::cout << "[" << name_ << "] REFUSED: every " << unitType_
                  << " unit is already committed.\n";
        send("resource_exhausted", incident);   // let the mediator find help
        return 0;
    }
    unit->deploy(incident->getLocation());
    if (mediator_) mediator_->coordinateDispatch(this, incident, unit);
    return unit;
}

void DispatchColleague::recall(ResponseUnit* unit, Incident* incident) {
    if (!unit) return;
    unit->standDown();
    if (incident) incident->releaseUnit(unit);
}

void DispatchColleague::onPeerDispatched(Incident* incident) {
    std::cout << "    [" << name_ << "] noted responders on scene for "
              << (incident ? incident->getId() : "?") << ".\n";
}

void DispatchColleague::receive(const std::string& event, Incident* incident) {
    if (event == "unit_dispatched") onPeerDispatched(incident);
}

// ===================== SecurityColleague =====================
SecurityColleague::SecurityColleague(IncidentMediator* mediator)
    : DispatchColleague("Security Desk", "Security", mediator) {}

void SecurityColleague::receive(const std::string& event, Incident* incident) {
    if (event == "unit_dispatched") {
        std::cout << "    [" << name_ << "] clearing an approach route for "
                  << (incident ? incident->getLocation() : "the scene") << ".\n";
    } else if (event == "evacuation_ordered") {
        std::cout << "    [" << name_ << "] marshalling occupants to the "
                     "assembly point.\n";
    } else if (event == "incident_resolved") {
        std::cout << "    [" << name_ << "] standing the perimeter down.\n";
    }
}

// ===================== MedicalColleague =====================
MedicalColleague::MedicalColleague(IncidentMediator* mediator)
    : DispatchColleague("Medical Desk", "Medical", mediator) {}

void MedicalColleague::receive(const std::string& event, Incident* incident) {
    if (event == "unit_dispatched") {
        std::cout << "    [" << name_ << "] preparing a casualty point ("
                  << availableCount() << " unit(s) still free).\n";
    } else if (event == "evacuation_ordered") {
        std::cout << "    [" << name_ << "] positioning at the assembly point "
                     "for walking wounded.\n";
    } else if (event == "incident_resolved" && incident) {
        std::cout << "    [" << name_ << "] closing the patient record for "
                  << incident->getId() << ".\n";
    }
}

// ===================== FacilitiesColleague =====================
FacilitiesColleague::FacilitiesColleague(IncidentMediator* mediator)
    : DispatchColleague("Facilities Desk", "Facilities", mediator) {}

void FacilitiesColleague::receive(const std::string& event, Incident* incident) {
    if (event == "unit_dispatched" && incident) {
        std::cout << "    [" << name_ << "] isolating power and ventilation "
                     "around " << incident->getZoneId() << ".\n";
    } else if (event == "evacuation_ordered") {
        std::cout << "    [" << name_ << "] releasing stairwell doors and "
                     "lighting escape routes.\n";
    } else if (event == "incident_resolved") {
        std::cout << "    [" << name_ << "] scheduling a damage inspection.\n";
    }
}

// ===================== AccessControlColleague =====================
AccessControlColleague::AccessControlColleague(AccessControlSystem* accessControl,
                                               IncidentMediator* mediator)
    : Colleague("Access Control", mediator), accessControl_(accessControl) {}

void AccessControlColleague::receive(const std::string& event, Incident* incident) {
    if (!accessControl_ || !incident) return;
    if (event == "unit_dispatched") {
        accessControl_->restrictZone(incident->getZoneId(), 3);
    } else if (event == "evacuation_ordered") {
        accessControl_->lockZone(incident->getZoneId());
    } else if (event == "incident_resolved") {
        accessControl_->unlockZone(incident->getZoneId());
    }
}

// ===================== CommunicationColleague =====================
CommunicationColleague::CommunicationColleague(AlertService* alertService,
                                               IncidentMediator* mediator)
    : Colleague("Communications", mediator), alertService_(alertService) {}

void CommunicationColleague::receive(const std::string& event, Incident* incident) {
    if (!alertService_ || !incident) return;
    if (event == "unit_dispatched") {
        alertService_->sendToZone(incident->getZoneId(),
                                  "Responders are on scene. Follow their instructions.");
    } else if (event == "evacuation_ordered") {
        alertService_->broadcast("EVACUATE " + incident->getZoneId()
                                 + " immediately.");
    } else if (event == "resource_exhausted") {
        alertService_->broadcast("Response to " + incident->getId()
                                 + " is delayed; external help requested.");
    } else if (event == "incident_resolved") {
        alertService_->broadcast("Incident " + incident->getId()
                                 + " is resolved. Normal operations resume.");
    }
}

// ===================== MutualAidColleague =====================
MutualAidColleague::MutualAidColleague(CampusGuardService* externalService,
                                       IncidentMediator* mediator)
    : Colleague("Mutual Aid Liaison", mediator), externalService_(externalService) {}

bool MutualAidColleague::requestAssistance(const std::string& unitType,
                                           Incident* incident) {
    if (!externalService_ || !incident) return false;
    std::cout << "    [" << name_ << "] escalating to "
              << externalService_->getServiceName() << ".\n";
    return externalService_->dispatchUnit(unitType, incident->getLocation());
}

void MutualAidColleague::receive(const std::string& event, Incident* incident) {
    if (!incident) return;
    if (event == "resource_exhausted") {
        // Campus capacity is gone: pull in the city services through the adapter.
        std::string type = (incident->getType() == Medical) ? "Medical" : "Security";
        if (!requestAssistance(type, incident))
            std::cout << "    [" << name_ << "] external assistance unavailable.\n";
    } else if (event == "incident_resolved" && externalService_) {
        externalService_->releaseZone(incident->getZoneId());
    }
}
