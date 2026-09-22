#include "Incident.h"
#include "IncidentState.h"
#include "IncidentObserver.h"
#include "ResponseUnit.h"
#include <iostream>
#include <algorithm>

Incident::Incident(const std::string& id, IncidentType type,
                   const std::string& location, const std::string& zoneId,
                   int severity, const std::string& description)
    : id_(id), type_(type), location_(location), zoneId_(zoneId),
      severity_(severity), description_(description), state_(0) {}

Incident::~Incident() {
    delete state_;     
}

// ---- Observer ----
void Incident::attach(IncidentObserver* obs) {
    if (!obs) return;
    if (std::find(observers_.begin(), observers_.end(), obs) != observers_.end())
        return;                             
    observers_.push_back(obs);
}

void Incident::detach(IncidentObserver* obs) {
    std::vector<IncidentObserver*>::iterator it =
        std::find(observers_.begin(), observers_.end(), obs);
    if (it != observers_.end()) observers_.erase(it);
}

void Incident::notify() {
    for (size_t i = 0; i < observers_.size(); ++i)
        if (observers_[i]) observers_[i]->update(this);
}

// ---- State ----
void Incident::setState(IncidentState* newState) {
    if (!newState || newState == state_) { delete newState; return; }
    delete state_;            
    state_ = newState;
    notify();                   
}

IncidentState* Incident::getState() const { return state_; }

std::string Incident::getStatusName() const {
    return state_ ? state_->getStatusName() : "Unknown";
}

void Incident::updateStatus() {
    if (!state_) {
        std::cout << "[Incident] " << id_ << " has no state; cannot advance.\n";
        return;
    }
    IncidentState* next = state_->handle(this);   
    if (next) setState(next);                    
}

bool Incident::canDispatch() const {
    return state_ != 0 && state_->allowsDispatch();
}

// ---- Domain ----
const std::string& Incident::getId() const { return id_; }
IncidentType Incident::getType() const { return type_; }

std::string Incident::getTypeName() const {
    switch (type_) {
        case Fire:       return "Fire";
        case Medical:    return "Medical";
        case Security:   return "Security";
        case Facilities: return "Facilities";
    }
    return "Unknown";
}

const std::string& Incident::getLocation() const { return location_; }
const std::string& Incident::getZoneId() const { return zoneId_; }
int Incident::getSeverity() const { return severity_; }
const std::string& Incident::getDescription() const { return description_; }

void Incident::assignUnit(ResponseUnit* unit) {
    if (!unit) return;
    if (std::find(units_.begin(), units_.end(), unit) != units_.end()) return;
    units_.push_back(unit);
}

void Incident::releaseUnit(ResponseUnit* unit) {
    std::vector<ResponseUnit*>::iterator it =
        std::find(units_.begin(), units_.end(), unit);
    if (it != units_.end()) units_.erase(it);
}

const std::vector<ResponseUnit*>& Incident::getAssignedUnits() const {
    return units_;
}
