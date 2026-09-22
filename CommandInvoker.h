#ifndef COMMAND_INVOKER_H
#define COMMAND_INVOKER_H

#include <vector>
#include <string>

class Command;


class CommandInvoker {
public:
    CommandInvoker();
    ~CommandInvoker();

    bool issueCommand(Command* cmd);     
    bool cancelLast();                 
    int historySize() const;
    void printHistory() const;

private:
    CommandInvoker(const CommandInvoker&);
    CommandInvoker& operator=(const CommandInvoker&);

    std::vector<Command*> history_;   
    bool undoing_;                     
};

#endif
