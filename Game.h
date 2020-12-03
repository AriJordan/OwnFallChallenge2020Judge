#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <cassert>
#include <map>
#include <type_traits>

#include "Player.h"
#include "Deck.h"
#include "DeliveryCompletion.h"
#include "BonusData.h"

struct Game {
	const static int READ_AHEAD_COST = 1;
    const static int STARTING_SCORE = 0;
    const static int MAX_SPACE = 10;
    const static int INGREDIENT_TYPE_COUNT = 4;
    const static int COUNTER_SIZE = 5;
    const static int TOME_SIZE = 6;

    const std::vector<int> STARTING_INGREDIENT_COUNT = { 3, 0, 0, 0 };
    const static int DELIVERY_GOAL = 6;
    
private:
    std::vector<Player*> players;
    Player p0, p1;
    std::vector<TomeSpell> tome;
    std::vector<DeliverySpell> deliveries;
    std::vector<DeliverySpell> newDeliveries;
    std::vector<int> tomeStockGain;
    Deck deck;
    std::vector<int> bonus;
    int bonusValue[2] = { 3, 1 };
    std::set<TomeSpell> learntSpells;
    std::set<DeliveryCompletion> delivered;

    void slideBonus() {
        if (bonus[0] <= 0) {
            bonus[0] = bonus[1];
            bonus[1] = 0;
            bonusValue[0] = 1;
            bonusValue[1] = 0;
        }
    }
    template<typename T>
    std::string actionToStr(T& spell, Player& player) {
        return std::to_string(spell.getId()) + " " + getSpellStr(spell, player) + " " + spell.recipe.toPlayerString() + " " +
            std::to_string(getScoreOf(spell)) + " " + std::to_string(indexOf(tome, spell)) + " " + std::to_string(spell.getStock()) + " " +
            std::to_string(spell.isActive() ? 1 : 0) + " " + std::to_string(spell.isRepeatable() ? 1 : 0) + "\n";
    }

public:
    std::string getCurrentFrameInfoFor(Player player) {

        Player& foe = *players.at(1 - player.getIndex());
        std::string lines;
        lines += std::to_string(deliveries.size() + tome.size() + player.spells.size() + foe.spells.size()) + "\n";
        for (DeliverySpell delivery : deliveries) {
            lines += actionToStr(delivery, player);
        }
        for (TomeSpell spell : tome) {
            lines += actionToStr(spell, player);
        }
        for (PlayerSpell spell : player.spells) {
            lines += actionToStr(spell, player);
        }
        for (PlayerSpell spell : foe.spells) {
            lines += actionToStr(spell, player);
        }
        //std::cerr << "lines :" << lines << "\n";
        // scores
        lines += player.getInventory().toPlayerString() + " " + std::to_string(player.getScore()) + "\n";
        lines += foe.getInventory().toPlayerString() + " " + std::to_string(foe.getScore()) + "\n";
        return lines;
    }

    int getScoreOf(DeliverySpell delivery) {
        int index;
        for (int r = 0; r < 5; r++)
            if (deliveries.at(r).getDelta() == delivery.getDelta())
                index = r;
        int bonusScore = 0;
        if (index < 2) {
            if (bonus[index] > 0) {
                bonusScore = bonusValue[index];
            }
        }
        return delivery.score + bonusScore;
    }

    template<typename T>
    int getScoreOf(T spell) {
        return 0;
    }

public:
    Game(long seed, Player& p0, Player& p1) : deck(Deck()) {
        players.resize(2); players.at(0) = &p0, players.at(1) = &p1;
        bonus = { 4, 4 };
        std::mt19937 random_engine(seed);
        std::shuffle(deck.tome.begin(), deck.tome.end(), random_engine);
        std::shuffle(deck.deliveries.begin(), deck.deliveries.end(), random_engine);

        tome.clear();
        deliveries.clear();
        newDeliveries.clear();
        tomeStockGain.assign(6, 0);

        for (int i = 0; i < TOME_SIZE; ++i) {
            tome.emplace_back(deck.tome.front()); deck.tome.erase(deck.tome.begin());
            tomeStockGain[i] = 0;
        }
        for (int i = 0; i < COUNTER_SIZE; ++i) {
            deliveries.emplace_back(deck.deliveries.front()); deck.deliveries.erase(deck.deliveries.begin());
        }
        p0.inventory.delta = STARTING_INGREDIENT_COUNT;
        p1.inventory.delta = STARTING_INGREDIENT_COUNT;
        p0.initSpells();
        p1.initSpells();
    }

private:
    template<typename S>
    std::string getSpellStr(S& spell, Player& player) {
        if (dynamic_cast<TomeSpell*>(&spell)) {
            return "LEARN";
        }
        else if (dynamic_cast<DeliverySpell*>(&spell)) {
            return "BREW";
        }
        else {
            if (spell.isOwner(player)) {
                return "CAST";
            }
            else {
                return "OPPONENT_CAST";
            }
        }
    }

    SpellType getSpellType(Spell& spell, Player& player) {
        if (dynamic_cast<TomeSpell*>(&spell)) {
            return LEARN;
        }
        else if (dynamic_cast<DeliverySpell*>(&spell)) {
            return BREW;
        }
        else {
            if (spell.isOwner(player)) {
                return CAST;
            }
            else {
                return OPPONENT_CAST;
            }
        }
    }

public:
    void checkSpellActionType(Action& action, SpellType type) {
        std::string expectedStr = "null";

        switch (type) {
        case LEARN:
            expectedStr = "LEARN";
            break;
        case CAST:
            expectedStr = "CAST";
            break;
        case BREW:
            expectedStr = "BREW";
            break;
        case OPPONENT_CAST:
            assert(0 && "Tried to cast an opponent's spell...");
        }

        assert(action.getStr() == expectedStr);
    }

public:
    void performGameUpdate() {
        for (Player* player : players) {
            Action& action = player->action;
            if (action.getStr() == "BREW" || action.getStr() == "LEARN" || action.getStr() == "CAST") {
                int id = action.spellId;
                Spell& spell = getSpellById(id);
                checkSpellActionType(action, getSpellType(spell, *player));
                if (dynamic_cast<TomeSpell*>(&spell)) {
                    doLearn(*player, *dynamic_cast<TomeSpell*>(&spell));
                }
                else if (dynamic_cast<DeliverySpell*>(&spell)) {
                    doDelivery(*player, *dynamic_cast<DeliverySpell*>(&spell));
                }
                else if (dynamic_cast<PlayerSpell*>(&spell)) {
                    doPlayerSpell(*player, *dynamic_cast<PlayerSpell*>(&spell));
                }
                else {
                    assert(0 && "Spell with id does not exist");
                }
            }
            else if (action.getStr() == "REST") {
                doReset(*player);
            }
            else {
                assert(0 && "Action should be spell or reset");
            }
        }
        int z = 0;
        for (const TomeSpell& spell : learntSpells) {
            for (int l = 0; l < tome.size(); l++) {
                if (!(l + 1 >= tome.size() || tome.at(l).getDelta() != tome.at(l + 1).getDelta())) {
                    std::cerr << "Error: Two learns, same\n";
                }
                assert(l + 1 >= tome.size() || tome.at(l).getDelta() != tome.at(l + 1).getDelta());
                if (tome.at(l).getDelta() == spell.getDelta()) {
                    tome.erase(tome.begin() + l); break;
                }
            }
            if (!deck.tome.empty()) {
                tome.emplace_back(deck.tome.front());
                deck.tome.erase(deck.tome.begin());
                int z = 5;
            }
        }

        for (const DeliveryCompletion& delCompletion : delivered) {
            if (delCompletion.getIndex() < bonus.size()) {
                if (bonus[delCompletion.getIndex()] > 0) {
                    bonus[delCompletion.getIndex()]--;
                }
            }
        }

        for (const DeliveryCompletion& dc : delivered) {
            for (int r = 0; r < deliveries.size(); r++) {
                if (deliveries.at(r) == dc.delivery) {
                    deliveries.erase(deliveries.begin() + r);
                    if (!deck.deliveries.empty()) {
                        deliveries.emplace_back(deck.deliveries.front()); deck.deliveries.erase(deck.deliveries.begin());
                    }
                }
            }
        }

        slideBonus();

        for (int i = 0; i < tome.size(); i++) {
            tome.at(i).stock += tomeStockGain[i];
            tomeStockGain[i] = 0;
        }

        //Handeled in Referee
        /*if (gameOver()) {
            
        }*/
    }

    bool gameOver() {
        for (Player* player : players) {
            if (player->getDeliveriesCount() >= DELIVERY_GOAL) {
                return true;
            }
        }
        return false;
    }

    Spell& getSpellById(int id) {
        for (Spell& spell : deliveries) {
            if (spell.getId() == id) {
                return spell;
            }
        }
        for (Spell& spell : tome) {
            if (spell.getId() == id) {
                return spell;
            }
        }
        for (const DeliveryCompletion& spell : delivered) {
            if (spell.getIndex() == id) {
                return const_cast<DeliverySpell&>(spell.delivery);
            }
        }
        for (Player* player : players) {
            for (Spell& spell : player->spells) {
                if (spell.getId() == id) {
                    return spell;
                }
            }
        }
        assert(0 && "Spell not found");
        return tome.at(0);
    }

    void doReset(Player& p) {
        for (Spell& spell : p.spells) {
            (*dynamic_cast<PlayerSpell*>(&spell)).activate();
        }
    }

    template<typename S, typename T>
    int indexOf(S& vec, T& to_find) {
        for (int r = 0; r < vec.size(); r++)
            if (vec.at(r).getDelta() == to_find.getDelta())
                return r;
    }

    void doDelivery(Player& p, DeliverySpell& del) {
        if (!p.canDeliver(del.recipe)) {
            assert(0 && "Not enough ingredients for order ");
        }
        for (int i = 0; i < INGREDIENT_TYPE_COUNT; ++i) {
            p.inventory.add(i, del.recipe.delta[i]);
        }
        delivered.insert(DeliveryCompletion(del, indexOf(deliveries, del), getScoreOf(del)));
        p.addDelivery();
        p.addScore(getScoreOf(del));
    }

    void doLearn(Player& p, TomeSpell& spell){
        int index = indexOf(tome, spell);
        if (p.getInventory().delta[0] < index * READ_AHEAD_COST) {
            assert(0 && ("Not enough ingredients to learn " || spell.getId()));
        }
        for (int i = 0; i < index; ++i) {
            tomeStockGain[i] += READ_AHEAD_COST;
            p.getInventory().delta[0] -= READ_AHEAD_COST;
        }
        p.spells.emplace_back(spell, p);

        int maxToGet = MAX_SPACE - p.getInventory().getTotal();
        int ingredientsGot = std::min(maxToGet, spell.stock);
        p.inventory.delta[0] += ingredientsGot; //TODO by reference everywhere

        learntSpells.insert(spell);
    }

private:
    void doPlayerSpell(Player& p, PlayerSpell& spell){
        int repeats = p.action.getRepeats();
        if (repeats < 1) {
            assert(0 && "Repeat can't be zero (on " && spell.getId() && ")");
        }
        if (repeats > 1 && !spell.isRepeatable()) {
            assert(0 && "Spell " && spell.getId() && " is not repeatable");
        }
        if (!spell.isActive()) {
            assert(0 && "Spell " && spell.getId() && " is exhausted");
        }
        if (!p.canAfford(spell.recipe, repeats)) {
            assert(0 && "Not enough ingredients for spell " && spell.getId());
        }
        if (!p.enoughSpace(spell.recipe, repeats)) {
            assert(0 && "Not enough space in inventory for spell " && spell.getId());
        }

        //do spell
        for (int k = 0; k < repeats; ++k) {
            for (int i = 0; i < INGREDIENT_TYPE_COUNT; ++i) {
                p.inventory.add(i, spell.getDelta()[i]);
            }
        }
        spell.deactivate();
    }
public:
    void resetGameTurnData() {
        learntSpells.clear();
        delivered.clear();
        newDeliveries.clear();
    }

    std::string getExpected() {
        return "BREW <id> | CAST <id> [<repeat>] | LEARN <id> | REST | WAIT";
    }

    void onEnd() {
        for (Player* player : players) {
            for (int i = 1; i < INGREDIENT_TYPE_COUNT; i++) {
                player->addScore(player->getInventory().delta[i]);
            }
        }
    }

    std::map<int, BonusData> getBonusData() {
        std::map<int, BonusData> bonusData;
        for (int i = 0; i < 2; ++i) {
            if (bonus[i] > 0) {
                bonusData[i] = BonusData(bonus[i], bonusValue[i]);
            }
        }

        return bonusData;
    }

};