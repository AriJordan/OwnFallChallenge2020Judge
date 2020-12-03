#pragma once
#include <vector>
#include "Spells.h"
class Deck {
public:
    std::vector<TomeSpell> tome;
    std::vector<DeliverySpell> deliveries;

    Deck() {
        tome.emplace_back(Recipe(-3, 0, 0, 1));
        tome.emplace_back(Recipe(3, -1, 0, 0));
        tome.emplace_back(Recipe(1, 1, 0, 0));
        tome.emplace_back(Recipe(0, 0, 1, 0));
        tome.emplace_back(Recipe(3, 0, 0, 0));
        tome.emplace_back(Recipe(2, 3, -2, 0));
        tome.emplace_back(Recipe(2, 1, -2, 1));
        tome.emplace_back(Recipe(3, 0, 1, -1));
        tome.emplace_back(Recipe(3, -2, 1, 0));
        tome.emplace_back(Recipe(2, -3, 2, 0));
        tome.emplace_back(Recipe(2, 2, 0, -1));
        tome.emplace_back(Recipe(-4, 0, 2, 0));
        tome.emplace_back(Recipe(2, 1, 0, 0));
        tome.emplace_back(Recipe(4, 0, 0, 0));
        tome.emplace_back(Recipe(0, 0, 0, 1));
        tome.emplace_back(Recipe(0, 2, 0, 0));
        tome.emplace_back(Recipe(1, 0, 1, 0));
        tome.emplace_back(Recipe(-2, 0, 1, 0));
        tome.emplace_back(Recipe(-1, -1, 0, 1));
        tome.emplace_back(Recipe(0, 2, -1, 0));
        tome.emplace_back(Recipe(2, -2, 0, 1));
        tome.emplace_back(Recipe(-3, 1, 1, 0));
        tome.emplace_back(Recipe(0, 2, -2, 1));
        tome.emplace_back(Recipe(1, -3, 1, 1));
        tome.emplace_back(Recipe(0, 3, 0, -1));
        tome.emplace_back(Recipe(0, -3, 0, 2));
        tome.emplace_back(Recipe(1, 1, 1, -1));
        tome.emplace_back(Recipe(1, 2, -1, 0));
        tome.emplace_back(Recipe(4, 1, -1, 0));
        tome.emplace_back(Recipe(-5, 0, 0, 2));
        tome.emplace_back(Recipe(-4, 0, 1, 1));
        tome.emplace_back(Recipe(0, 3, 2, -2));
        tome.emplace_back(Recipe(1, 1, 3, -2));
        tome.emplace_back(Recipe(-5, 0, 3, 0));
        tome.emplace_back(Recipe(-2, 0, -1, 2));
        tome.emplace_back(Recipe(0, 0, -3, 3));
        tome.emplace_back(Recipe(0, -3, 3, 0));
        tome.emplace_back(Recipe(-3, 3, 0, 0));
        tome.emplace_back(Recipe(-2, 2, 0, 0));
        tome.emplace_back(Recipe(0, 0, -2, 2));
        tome.emplace_back(Recipe(0, -2, 2, 0));
        tome.emplace_back(Recipe(0, 0, 2, -1));

        deliveries.emplace_back(Recipe(2, 2, 0, 0), 6);
        deliveries.emplace_back(Recipe(3, 2, 0, 0), 7);
        deliveries.emplace_back(Recipe(0, 4, 0, 0), 8);
        deliveries.emplace_back(Recipe(2, 0, 2, 0), 8);
        deliveries.emplace_back(Recipe(2, 3, 0, 0), 8);
        deliveries.emplace_back(Recipe(3, 0, 2, 0), 9);
        deliveries.emplace_back(Recipe(0, 2, 2, 0), 10);
        deliveries.emplace_back(Recipe(0, 5, 0, 0), 10);
        deliveries.emplace_back(Recipe(2, 0, 0, 2), 10);
        deliveries.emplace_back(Recipe(2, 0, 3, 0), 11);
        deliveries.emplace_back(Recipe(3, 0, 0, 2), 11);
        deliveries.emplace_back(Recipe(0, 0, 4, 0), 12);
        deliveries.emplace_back(Recipe(0, 2, 0, 2), 12);
        deliveries.emplace_back(Recipe(0, 3, 2, 0), 12);
        deliveries.emplace_back(Recipe(0, 2, 3, 0), 13);
        deliveries.emplace_back(Recipe(0, 0, 2, 2), 14);
        deliveries.emplace_back(Recipe(0, 3, 0, 2), 14);
        deliveries.emplace_back(Recipe(2, 0, 0, 3), 14);
        deliveries.emplace_back(Recipe(0, 0, 5, 0), 15);
        deliveries.emplace_back(Recipe(0, 0, 0, 4), 16);
        deliveries.emplace_back(Recipe(0, 2, 0, 3), 16);
        deliveries.emplace_back(Recipe(0, 0, 3, 2), 17);
        deliveries.emplace_back(Recipe(0, 0, 2, 3), 18);
        deliveries.emplace_back(Recipe(0, 0, 0, 5), 20);
        deliveries.emplace_back(Recipe(2, 1, 0, 1), 9);
        deliveries.emplace_back(Recipe(0, 2, 1, 1), 12);
        deliveries.emplace_back(Recipe(1, 0, 2, 1), 12);
        deliveries.emplace_back(Recipe(2, 2, 2, 0), 13);
        deliveries.emplace_back(Recipe(2, 2, 0, 2), 15);
        deliveries.emplace_back(Recipe(2, 0, 2, 2), 17);
        deliveries.emplace_back(Recipe(0, 2, 2, 2), 19);
        deliveries.emplace_back(Recipe(1, 1, 1, 1), 12);
        deliveries.emplace_back(Recipe(3, 1, 1, 1), 14);
        deliveries.emplace_back(Recipe(1, 3, 1, 1), 16);
        deliveries.emplace_back(Recipe(1, 1, 3, 1), 18);
        deliveries.emplace_back(Recipe(1, 1, 1, 3), 20);
    }
};