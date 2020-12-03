#pragma once
class BonusData {
public:
    int value;
    int amount;
    BonusData(int amount, int value) : value(value), amount(amount){}

    BonusData() :value(-1), amount(-1){}
};