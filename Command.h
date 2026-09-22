#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Incident;
class DispatchColleague;
class AccessControlSystem;
class AlertService;
class ResponseUnit;
class CampusGuardService;
class CommandInvoker;


class Command {
public:
    virtual ~Command() {}
    virtual bool execute() = 0;
    virtual void undo() = 0;
    virtual std::string getName() const = 0;
};


class DispatchUnitCommand : public Command {
public:
    DispatchUnitCommand(DispatchColleague* desk, Incident* incident);
    bool execute() override;
    void undo() override;
    std::string getName() const override;
private:
    DispatchColleague* desk_;   
    Incident* incident_;      
    ResponseUnit* deployed_;    
};


class SecureAreaCommand : public Command {
public:
    SecureAreaCommand(AccessControlSystem* accessControl,
                      const std::string& zoneId, int level);
    bool execute() override;
    void undo() override;
    std::string getName() const override;
private:
    AccessControlSystem* accessControl_;   
    std::string zoneId_;
    int level_;
    bool applied_;
};

class EvacuateZoneCommand : public Command {
public:
    EvacuateZoneCommand(AccessControlSystem* accessControl,
                        AlertService* alertService,
                        const std::string& zoneId);
    bool execute() override;
    void undo() override;
    std::string getName() const override;
private:
    AccessControlSystem* accessControl_;   
    AlertService* alertService_;         
    std::string zoneId_;
    bool applied_;
};


class ActivateAlertCommand : public Command {
public:
    ActivateAlertCommand(AlertService* alertService, const std::string& message);
    bool execute() override;
    void undo() override;
    std::string getName() const override;
private:
    AlertService* alertService_;        
    std::string message_;
    bool applied_;
};


class RequestExternalAssistCommand : public Command {
public:
    RequestExternalAssistCommand(CampusGuardService* service,
                                 const std::string& unitType,
                                 const std::string& location);
    bool execute() override;
    void undo() override;
    std::string getName() const override;
private:
    CampusGuardService* service_;         
    std::string unitType_;
    std::string location_;
    bool applied_;
};

/*
  Cancels the most recent action. It asks the invoker to undo, so the invoker
  remains the only owner of command lifetime and no dangling pointer is
  possible.
 */
class CancelLastActionCommand : public Command {
public:
    explicit CancelLastActionCommand(CommandInvoker* invoker);
    bool execute() override;
    void undo() override;
    std::string getName() const override;
private:
    CommandInvoker* invoker_;          
};

#endif
