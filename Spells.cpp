#include "Player.h"
#include "Spells.h"

bool PlayerSpell::isOwner(Player& player) {
	return (player.equals(owner));
}

PlayerSpell PlayerSpell::operator=(PlayerSpell& other) {
    spellId = other.spellId;
    recipe = other.recipe;
    active = other.active;
    repeatable = other.repeatable;
    owner = other.owner;
    return other;
}

int Spell::INSTANCE_COUNT = 0;

//Spell::Spell(Recipe recipe) : recipe(recipe), id(SPELL_INSTANCE_COUNT++){}