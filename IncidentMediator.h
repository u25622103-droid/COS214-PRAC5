#ifndef INCIDENT_MEDIATOR_H
#define INCIDENT_MEDIATOR_H

#include <vector>
#include <string>

class Colleague;
class Incident;
class ResponseUnit;


class IncidentMediator {
public:
    IncidentMediator();
    ~IncidentMediator();                             

    void registerColleague(Colleague* colleague);

    void notify(Colleague* sender, const std::string& event, Incident* incident);

    void coordinateDispatch(Colleague* sender, Incident* incident, ResponseUnit* unit);

    int colleagueCount() const;

private:
    IncidentMediator(const IncidentMediator&);
    IncidentMediator& operator=(const IncidentMediator&);

    std::vector<Colleague*> colleagues_;                
};

#endif
