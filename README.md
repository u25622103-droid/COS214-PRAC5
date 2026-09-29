# COS214-PRAC5

# CampusGuard — Emergency Response Coordination

CampusGuard is a campus emergency-response coordination platform. It handles
the full lifecycle of an incident: reporting, dispatching responders,
coordinating multi-agency response, integrating with an external legacy
service, and resolving the incident. The system is built in C++11 and uses
six Gang of Four design patterns that collaborate in one coherent application.


## Design Patterns Used

Command

    Files: Command.h, CommandInvoker.h

    Purpose: Encapsulates operator actions — dispatch, evacuate, secure, alert, and cancel — as objects with execute() and undo() methods.

Mediator

    Files: IncidentMediator.h, Colleague.h

    Purpose: Coordinates six response desks through a single mediator, eliminating direct dependencies between them.

Adapter

    Files: LegacyServiceAdapter.h

    Purpose: Translates CampusGuard's string-based interface into the legacy city mainframe's integer protocol.

Facade

    Files: EmergencyFacade.h

    Purpose: Provides the operator with three simple operations that internally orchestrate multiple subsystems.

State (chosen)

    Files: IncidentState.h

    Purpose: Models each incident status — Reported, Dispatched, InProgress, Resolved — as its own class with its own transition rules.

Observer (chosen)

    Files: IncidentObserver.h

    Purpose: Notifies three listeners — Dashboard, Notification, and Logging — automatically whenever an incident's status changes.

## Prerequisites

- Docker Desktop (with WSL 2 integration enabled if you are on Windows)
- Docker Compose v2 (`docker compose`, not the old `docker-compose`)

To check:
docker --version
docker compose version


## Build and Run

From the repository root:

docker compose up --build


This builds the image and starts the application. The two demonstration
scenarios run automatically and produce the console output described in
Section 7.

To stop:

docker compose down


To rebuild from scratch:

docker compose down --rmi local --volumes --remove-orphans
docker compose up --build

### Valgrind

docker compose up --build -d
docker compose exec campusguard make valgrind

To save the output to a file:
docker compose exec campusguard valgrind --leak-check=full --log-file=/app/logs/valgrind.txt ./campusGuard


The log will appear in `./logs/valgrind.txt` on the host (the compose file
mounts `./logs` into `/app/logs`).

Expected summary: `All heap blocks were freed -- no leaks are possible`.

### GDB

Inside GDB:
A captured session is stored in `gdb_session.txt` and
`gdb_session1.txt`. It documents a real out-of-bounds read in
`IncidentMediator::notify` where the loop used `i <= colleagues_.size()`
instead of `i < colleagues_.size()`. The bug was fixed after the session.

If GDB cannot attach, add to `docker-compose.yml`:

```yaml
    cap_add:
      - SYS_PTRACE
    security_opt:
      - seccomp:unconfined
```

## What the Application Demonstrates

Two end-to-end scenarios run in sequence.

### Scenario 1 — Fire in Engineering Building A (INC-001)

Happy path. An incident is reported, a Facilities unit is dispatched, the
zone is secured both on campus and via the legacy mainframe, an evacuation
is ordered, security is sent in, and the incident is resolved. All six
patterns participate in this single flow.

### Scenario 2 — Collapse in Chemistry Lab 3 (INC-002)

Failure-heavy path. Six deliberate failure cases are handled:

1. Duplicate incident number — refused by the repository.
2. No medical unit free — the mediator escalates to the legacy mainframe
   through the adapter for external help.
3. Unknown zone (`ZONE-XYZ`) — refused by access control.
4. Cancelling the mistaken action — the command's `undo()` runs.
5. Unknown incident (`INC-999`) — refused by the facade.
6. Dispatch to a resolved incident — refused by the incident's state.

The application ends with an audit trail: repository summary, command
history, observer log, alert count, and external service status.


## Git History

The repository history shows development over time by all three team
members. Commits are made by each member on separate features, not
squashed at the end.

git log --oneline --graph --all
git shortlog -sne

