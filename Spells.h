#pragma once
#include<vector>
#include<string>
#include<cassert>

//#include "Player.h"
#include "Recipe.h"
#include "Action.h"

class Player;

class Spell : public Action{
    static int INSTANCE_COUNT; 
public:
    Recipe recipe;

    Spell(Recipe recipe) : Action(INSTANCE_COUNT++), recipe(recipe){}

    int getId() {
        return spellId;
    }

    virtual int getScore() {
        return 0;
    }

    virtual int getStock() {
        return -1;
    }

    virtual bool isActive() {
        return false;
    }

    virtual bool isOwner(Player& player) {
        return false;
    }

    std::vector<int> getDelta() const{
        return recipe.delta;
    }

    virtual bool isRepeatable() {
        assert(0 && "called Spell.isRepeatable");
        return 0;
    }

    bool operator==(const Spell& other) const{
        return spellId == other.spellId;
    }

    virtual Spell operator=(Spell& other) {
        spellId = other.spellId;
        recipe = other.recipe;
        return other;
    }

    /*@override
        String toString() {
        return String.valueOf(spellId);
    }*/
};

class DeliverySpell : public Spell {
public:
    int score;
    DeliverySpell(Recipe need, int score)
        : Spell(Recipe(-need.delta[0], -need.delta[1], -need.delta[2], -need.delta[3]))
        , score(score)
    {
        score = score;
    }

    int getScore() override {
        return score;
    }

    bool isRepeatable() override {
        return false;
    }

    DeliverySpell operator=(const DeliverySpell& other) {
        spellId = other.spellId;
        recipe = other.recipe;
        score = other.score;
        return other;
    }
};

class TomeSpell : public Spell {
public:
    int stock;
    bool repeatable;
    TomeSpell(Recipe recipe) : Spell(recipe), stock(0){
        repeatable = false;
        for (int item : recipe.delta)
            if (item < 0)
                repeatable = true;
    }

    int getStock() override {
        return stock;
    }

    bool isRepeatable() override {
        return repeatable;
    }

    Recipe getRecipe() {
        return recipe;
    }

    TomeSpell operator=(const TomeSpell& other) {
        spellId = other.spellId;
        recipe = other.recipe;
        stock = other.stock;
        repeatable = other.repeatable;
        return other;
    }

    bool operator<(const TomeSpell& other) const {
        return spellId < other.spellId;
    }
};

class PlayerSpell : public Spell {
    bool active;
    bool repeatable;
    Player& owner;
public:
    PlayerSpell(Recipe recipe, Player& owner, bool repeatable) : Spell(recipe), owner(owner), active(true), repeatable(repeatable) {}

    PlayerSpell(Recipe recipe, Player& owner) : Spell(recipe), owner(owner){
        active = true;
        repeatable = false;
    }

    PlayerSpell(TomeSpell learnt, Player& owner) : Spell(learnt.getRecipe()), owner(owner){
        active = true;
        repeatable = learnt.isRepeatable();
    }

    bool isActive() override {
        return active;
    }

    bool isOwner(Player& player) override;

    bool isRepeatable() override {
        return repeatable;
    }

    void activate() {
        active = true;
    }

    void deactivate() {
        active = false;
    }

    PlayerSpell operator=(PlayerSpell& other);
};

enum SpellType {
    CAST,
    OPPONENT_CAST,
    LEARN,
    BREW
};