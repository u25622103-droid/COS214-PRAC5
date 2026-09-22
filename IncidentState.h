#ifndef INCIDENT_STATE_H
#define INCIDENT_STATE_H

#include <string>

class Incident;

class IncidentState {
public:
    virtual ~IncidentState() {}

    virtual IncidentState* handle(Incident* incident) = 0;
    virtual std::string getStatusName() const = 0;
    virtual bool allowsDispatch() const = 0;
    virtual bool isTerminal() const { return false; }
};

class ReportedState : public IncidentState {
public:
    IncidentState* handle(Incident* incident) override;
    std::string getStatusName() const override;
    bool allowsDispatch() const override;
};

class DispatchedState : public IncidentState {
public:
    IncidentState* handle(Incident* incident) override;
    std::string getStatusName() const override;
    bool allowsDispatch() const override;
};

class InProgressState : public IncidentState {
public:
    IncidentState* handle(Incident* incident) override;
    std::string getStatusName() const override;
    bool allowsDispatch() const override;
};

class ResolvedState : public IncidentState {
public:
    IncidentState* handle(Incident* incident) override;
    std::string getStatusName() const override;
    bool allowsDispatch() const override;
    bool isTerminal() const override;
};

#endif
