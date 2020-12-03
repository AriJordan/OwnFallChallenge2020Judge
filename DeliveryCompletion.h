#pragma once
#include "Spells.h"

class DeliveryCompletion {
    int index, earned;
public:
    DeliverySpell delivery;
    DeliveryCompletion(DeliverySpell delivery, int index, int earned) : delivery(delivery), index(index), earned(earned) {}
    
    DeliveryCompletion(const DeliveryCompletion& dc) : delivery(dc.delivery), index(dc.index), earned(dc.earned){}

    int getIndex() const {
        return index;
    }

    int getEarned() {
        return earned;
    }

    bool operator<(const DeliveryCompletion& other) const {
        return index < other.index;
    }
};