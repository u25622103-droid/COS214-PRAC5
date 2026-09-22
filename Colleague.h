#ifndef COLLEAGUE_H
#define COLLEAGUE_H

#include <string>
#include <vector>

class IncidentMediator;
class Incident;
class ResponseUnit;
class AccessControlSystem;
class AlertService;
class CampusGuardService;

/**
 * Colleague (Mediator pattern).
 *
 * A colleague knows its mediator and nothing about its peers. send() is
 * deliberately non-virtual: every colleague raises events the same way, only
 * the reaction in receive() differs.
 */
class Colleague {
public:
    Colleague(const std::string& name, IncidentMediator* mediator = 0);
    virtual ~Colleague() {}

    void setMediator(IncidentMediator* mediator);
    const std::string& getName() const;

    void send(const std::string& event, Incident* incident);
    virtual void receive(const std::string& event, Incident* incident) = 0;

protected:
    std::string name_;
    IncidentMediator* mediator_;   // non-owning
};

/**
 * A dispatch desk: owns a roster of response units and is the RECEIVER of
 * DispatchUnitCommand.
 */
class DispatchColleague : public Colleague {
public:
    DispatchColleague(const std::string& name,
                      const std::string& unitType,
                      IncidentMediator* mediator = 0);
    ~DispatchColleague() override;              // owns its units

    void addUnit(ResponseUnit* unit);           // takes ownership
    ResponseUnit* findAvailableUnit() const;
    const std::string& getUnitType() const;
    int availableCount() const;

    // Receiver actions
    ResponseUnit* dispatchTo(Incident* incident);
    void recall(ResponseUnit* unit, Incident* incident);

    void receive(const std::string& event, Incident* incident) override;

protected:
    virtual void onPeerDispatched(Incident* incident);

    std::string unitType_;
    std::vector<ResponseUnit*> units_;          // owned
};

class SecurityColleague : public DispatchColleague {
public:
    explicit SecurityColleague(IncidentMediator* mediator = 0);
    void receive(const std::string& event, Incident* incident) override;
};

class MedicalColleague : public DispatchColleague {
public:
    explicit MedicalColleague(IncidentMediator* mediator = 0);
    void receive(const std::string& event, Incident* incident) override;
};

class FacilitiesColleague : public DispatchColleague {
public:
    explicit FacilitiesColleague(IncidentMediator* mediator = 0);
    void receive(const std::string& event, Incident* incident) override;
};

/** Reacts to coordination events by changing building access. */
class AccessControlColleague : public Colleague {
public:
    AccessControlColleague(AccessControlSystem* accessControl,
                           IncidentMediator* mediator = 0);
    void receive(const std::string& event, Incident* incident) override;
private:
    AccessControlSystem* accessControl_;   // non-owning
};

/** Reacts to coordination events by messaging staff and the public. */
class CommunicationColleague : public Colleague {
public:
    CommunicationColleague(AlertService* alertService,
                           IncidentMediator* mediator = 0);
    void receive(const std::string& event, Incident* incident) override;
private:
    AlertService* alertService_;           // non-owning
};

/** Reacts by escalating to the external mutual-aid provider (via Adapter). */
class MutualAidColleague : public Colleague {
public:
    MutualAidColleague(CampusGuardService* externalService,
                       IncidentMediator* mediator = 0);
    void receive(const std::string& event, Incident* incident) override;
    bool requestAssistance(const std::string& unitType, Incident* incident);
private:
    CampusGuardService* externalService_;  // non-owning
};

#endif
