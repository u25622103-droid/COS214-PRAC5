#ifndef INCIDENT_OBSERVER_H
#define INCIDENT_OBSERVER_H

#include <string>
#include <vector>

class Incident;
class AlertService;


class IncidentObserver {
public:
    virtual ~IncidentObserver() {}
    virtual void update(Incident* incident) = 0;
};

class DashboardObserver : public IncidentObserver {
public:
    void update(Incident* incident) override;
};

class NotificationObserver : public IncidentObserver {
public:
    explicit NotificationObserver(AlertService* alertService);
    void update(Incident* incident) override;
private:
    AlertService* alertService_;   
};

class LoggingObserver : public IncidentObserver {
public:
    void update(Incident* incident) override;
    const std::vector<std::string>& getLog() const;
private:
    std::vector<std::string> log_;
};

#endif
