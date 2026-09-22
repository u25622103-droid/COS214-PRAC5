#include "CommandInvoker.h"
#include "Command.h"
#include <iostream>

CommandInvoker::CommandInvoker() : undoing_(false) {}

CommandInvoker::~CommandInvoker() {
    for (size_t i = 0; i < history_.size(); ++i) delete history_[i];
    history_.clear();
}

bool CommandInvoker::issueCommand(Command* cmd) {
    if (!cmd) return false;
    bool ok = cmd->execute();
    history_.push_back(cmd);         
    if (!ok) {
        std::cout << "[Console] " << cmd->getName()
                  << " did not complete; logged for the incident report.\n";
    }
    return ok;
}

bool CommandInvoker::cancelLast() {
    if (undoing_) {
        std::cout << "[Console] A cancellation is already in progress.\n";
        return false;
    }
    if (history_.empty()) {
        std::cout << "[Console] REFUSED: there is no action to cancel.\n";
        return false;
    }
    undoing_ = true;
    Command* last = history_.back();
    history_.pop_back();            
    last->undo();
    delete last;
    undoing_ = false;
    return true;
}

int CommandInvoker::historySize() const { return (int)history_.size(); }

void CommandInvoker::printHistory() const {
    std::cout << "[Console] action history (" << history_.size() << "):\n";
    for (size_t i = 0; i < history_.size(); ++i)
        std::cout << "    " << (i + 1) << ". " << history_[i]->getName() << "\n";
}
