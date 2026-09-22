#include "ResponseUnit.h"
#include <iostream>

CampusUnit::CampusUnit(const std::string& id, const std::string& unitType)
    : id_(id), unitType_(unitType), status_("Standing by"), available_(true) {}

void CampusUnit::deploy(const std::string& location) {
    status_ = "On scene at " + location;
    available_ = false;
    std::cout << "[" << unitType_ << " " << id_ << "] " << status_ << ".\n";
}

void CampusUnit::standDown() {
    status_ = "Standing by";
    available_ = true;
    std::cout << "[" << unitType_ << " " << id_ << "] stood down, back at base.\n";
}

std::string CampusUnit::getStatus() const   { return status_; }
std::string CampusUnit::getUnitId() const   { return id_; }
std::string CampusUnit::getUnitType() const { return unitType_; }
bool CampusUnit::isAvailable() const        { return available_; }

SecurityTeam::SecurityTeam(const std::string& id) : CampusUnit(id, "Security") {}
void SecurityTeam::deploy(const std::string& location) {
    CampusUnit::deploy(location);
    std::cout << "    -> perimeter established, crowd held back.\n";
}

MedicalTeam::MedicalTeam(const std::string& id) : CampusUnit(id, "Medical") {}
void MedicalTeam::deploy(const std::string& location) {
    CampusUnit::deploy(location);
    std::cout << "    -> triage started, ambulance bay notified.\n";
}

FacilitiesTeam::FacilitiesTeam(const std::string& id) : CampusUnit(id, "Facilities") {}
void FacilitiesTeam::deploy(const std::string& location) {
    CampusUnit::deploy(location);
    std::cout << "    -> utilities isolated, hazard barriers placed.\n";
}
