#pragma once

#include<vector>
#include<string>

#include "Action.h"
#include "Spells.h"

class Player {
    int deliveriesCount = 0;
    std::string message;
    int score = 0;
    static int INDEX_COUNT;
    int index;
public:
    std::vector<PlayerSpell> spells;
    Action action;
    Recipe inventory;
    Player() : score(0), index((INDEX_COUNT++) % 2) {
    }

    int getExpectedOutputLines() {
        return 1;
    }

    void initSpells() {
        spells.emplace_back(PlayerSpell(Recipe(2, 0, 0, 0), *this));
        spells.emplace_back(PlayerSpell(Recipe(-1, 1, 0, 0), *this));
        spells.emplace_back(PlayerSpell(Recipe(0, -1, 1, 0), *this));
        spells.emplace_back(PlayerSpell(Recipe(0, 0, -1, 1), *this));
    }

    void reset() {
        action.resetStr();
        message = "";
    }

    std::string getMessage() {
        return message;
    }

    bool canAfford(Recipe recipe, int repeats) {
        for (int i = 0; i < 4; ++i) {
            if (getInventory().delta[i] + recipe.delta[i] * repeats < 0) {
                return false;
            }
        }
        return true;
    }

    bool enoughSpace(Recipe recipe, int repeats) {
        return recipe.getTotal() * repeats + getInventory().getTotal() <= 10;
    }

    bool canDeliver(Recipe need) {
        for (int i = 0; i < 4; ++i) {
            if (getInventory().delta[i] + need.delta[i] < 0) {
                return false;
            }
        }
        return true;
    }

    int getIndex() {
        return index;
    }

    void addScore(int n) {
        setScore(score + n);
    }
    
    void setScore(int n) {
        score = n;
    }

    int getScore() {
        return score;
    }

    void addDelivery() {
        deliveriesCount++;
    }

    int getDeliveriesCount() {
        return deliveriesCount;
    }

    Recipe getInventory() {
        return inventory;
    }

    Action getAction() {
        return action;
    }

    void setAction(Action action) {
        action = action;
    }

    std::vector<int> getDelta() {
        return inventory.delta;
    }

    void setMessage(std::string message) {
        message = message;
    }

    bool equals(Player& p2) {
        return spells == p2.spells;
    }

    Player operator=(Player& other) {
        deliveriesCount = other.deliveriesCount;
        message = other.message;
        score = other.score;
        index = other.index;
        spells = other.spells;
        action = other.action;
        inventory = other.inventory;
        return other;
    }

    /*bool operator==(Player& other) {
        return spells == other.spells;
    }*/
};

