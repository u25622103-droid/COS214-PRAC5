#ifndef INCIDENT_H
#define INCIDENT_H

#include <string>
#include <vector>

class IncidentState;
class IncidentObserver;
class ResponseUnit;

enum IncidentType { Fire, Medical, Security, Facilities };


class Incident {
public:
    Incident(const std::string& id,
             IncidentType type,
             const std::string& location,
             const std::string& zoneId,
             int severity,
             const std::string& description);
    ~Incident();

    void attach(IncidentObserver* obs);     
    void detach(IncidentObserver* obs);
    void notify();

    void setState(IncidentState* newState); 
    IncidentState* getState() const;
    std::string getStatusName() const;
    void updateStatus();                
    bool canDispatch() const;

    const std::string& getId() const;
    IncidentType getType() const;
    std::string getTypeName() const;
    const std::string& getLocation() const;
    const std::string& getZoneId() const;
    int getSeverity() const;
    const std::string& getDescription() const;

    void assignUnit(ResponseUnit* unit);   
    void releaseUnit(ResponseUnit* unit);
    const std::vector<ResponseUnit*>& getAssignedUnits() const;

private:
    Incident(const Incident&);
    Incident& operator=(const Incident&);

    std::string id_;
    IncidentType type_;
    std::string location_;
    std::string zoneId_;
    int severity_;
    std::string description_;

    IncidentState* state_;                     
    std::vector<IncidentObserver*> observers_; 
    std::vector<ResponseUnit*> units_;         
};

#endif
