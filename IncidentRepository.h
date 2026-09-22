#ifndef INCIDENT_REPOSITORY_H
#define INCIDENT_REPOSITORY_H

#include <vector>
#include <string>

class Incident;

class IncidentRepository {
public:
    IncidentRepository();
    ~IncidentRepository();

    bool addIncident(Incident* incident);    
    Incident* findIncident(const std::string& id) const;
    const std::vector<Incident*>& getAllIncidents() const;
    void printSummary() const;

private:
    IncidentRepository(const IncidentRepository&);
    IncidentRepository& operator=(const IncidentRepository&);

    std::vector<Incident*> incidents_;         
};

#endif
