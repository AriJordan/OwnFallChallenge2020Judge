#pragma once
#include <string>

class Action {
public:
    int spellId;
    std::string str;
    Action() : spellId(-1) {
        str = "NO_ACTION";
    }
    Action(int id) : spellId(id){
        str = "NO_ACTION";
    }
    Action(int id, std::string str) : spellId(id), str(str){
        
    }

    std::string getStr() {
        return str;
    }

    void resetStr() {
        str = "NO_ACTION";
    }

    virtual bool isSpell() {
        return false;
    }

    virtual bool isReset() {
        return false;
    }

    virtual bool isWait() {
        return false;
    }

    virtual int getRepeats() {
        return 1;
    }

};

class SpellAction : public Action {
    int param;
public:
    SpellAction(std::string str, int spellId, int param=1) : Action(spellId, str), param(param) {
    }

    bool isSpell() override {
        return true;
    }

    int getRepeats() override {
        return param;
    }
};

class ResetAction : Action {

    bool isReset() override {
        return true;
    }
};

class WaitAction : Action {
public:
    bool isWait() override {
        return true;
    }
};