#ifndef RESPONSE_UNIT_H
#define RESPONSE_UNIT_H

#include <string>


class ResponseUnit {
public:
    virtual ~ResponseUnit() {}

    virtual void deploy(const std::string& location) = 0;
    virtual void standDown() = 0;
    virtual std::string getStatus() const = 0;
    virtual std::string getUnitId() const = 0;
    virtual std::string getUnitType() const = 0;
    virtual bool isAvailable() const = 0;
};

class CampusUnit : public ResponseUnit {
public:
    CampusUnit(const std::string& id, const std::string& unitType);
    ~CampusUnit() override {}

    void deploy(const std::string& location) override;
    void standDown() override;
    std::string getStatus() const override;
    std::string getUnitId() const override;
    std::string getUnitType() const override;
    bool isAvailable() const override;

protected:
    std::string id_;
    std::string unitType_;
    std::string status_;
    bool available_;
};

class SecurityTeam : public CampusUnit {
public:
    explicit SecurityTeam(const std::string& id);
    void deploy(const std::string& location) override;
};

class MedicalTeam : public CampusUnit {
public:
    explicit MedicalTeam(const std::string& id);
    void deploy(const std::string& location) override;
};

class FacilitiesTeam : public CampusUnit {
public:
    explicit FacilitiesTeam(const std::string& id);
    void deploy(const std::string& location) override;
};

#endif
