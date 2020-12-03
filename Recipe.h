#pragma once
#include<vector>
#include<string>

class Recipe {
public:
    std::vector<int> delta;

    Recipe(int a, int b, int c, int d) {
        delta = { a, b, c, d };
    }

    Recipe() {
        delta = { 0, 0, 0, 0 };
    }

    Recipe(const Recipe& other) {
        delta = { other.delta[0], other.delta[1], other.delta[2], other.delta[3] };
    }

    /*std::string toString() override {
        StringBuffer sb = new StringBuffer();
        if (IntStream.of(this.delta).allMatch(b->b == 0)) {
            return "0";
        }

        if (IntStream.of(this.delta).anyMatch(b->b > 0)) {
            sb.append("+");
            for (int i = 0; i < 4; ++i) {
                for (int k = 0; k < this.delta[i]; ++k) {
                    sb.append(CHARS[i]);
                }
            }
        }

        if (IntStream.of(this.delta).anyMatch(b->b < 0)) {
            if (IntStream.of(this.delta).anyMatch(b->b > 0)) {
                sb.append('\n');
            }
            sb.append("-");
            for (int i = 0; i < 4; ++i) {
                for (int k = this.delta[i]; k < 0; ++k) {
                    sb.append(CHARS[i]);
                }
            }
        }

        return sb.toString();
    }*/

    std::string toPlayerString() {
        return std::to_string(delta[0]) + " " + std::to_string(delta[1]) + " " + std::to_string(delta[2]) + " " + std::to_string(delta[3])+ " ";
    }

    void add(int idx, int x) {
        delta[idx] += x;
    }

    int getTotal() {
        int sum = 0;
        for (int item : delta)
            sum += item;
        return sum;
    }

    int getTotalLoss() {
        int sum = 0;
        for (int item : delta)
            if(item < 0)
                sum += item;
        return sum;
    }

    int getTotalGain() {
        int sum = 0;
        for (int item : delta)
            if (item > 0)
                sum += item;
        return sum;
    }

    bool equals(Recipe other) {
        return delta == other.delta;
        /*if (!(object instanceof Recipe)) {
            return false;
        }*/
    }
};