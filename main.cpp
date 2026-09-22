#include "CampusGuardSystem.h"
#include <iostream>

int main() {
    std::cout << "==================================================\n"
              << " CampusGuard: Emergency Response Coordination\n"
              << "==================================================\n";

    CampusGuardSystem system;
    system.runScenarioOne();
    system.runScenarioTwo();
    system.printAuditTrail();

    std::cout << "\n=== Shutdown: releasing all owned resources ===\n";
    return 0;
}
