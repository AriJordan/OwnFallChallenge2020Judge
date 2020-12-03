#pragma once
#include "Game.h"
#include "Player.h"
#include "Core.h"

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <time.h>
using namespace std;
#define LL long long

LL MOD = 7 + (LL)1e9;


int main()
{
    double parameter = 3.3;
    const int REPS = 100;
    double sum = 0;
    for (int rep = 1; rep <= REPS; rep++) {
        sum += parameter;
        double delta = 0.5; // double 0.0004 * ((double)REPS - rep);
        int p0_wins = 0, p1_wins = 0;
        for (int game_cnt = 0; game_cnt < 1; game_cnt++) {
            LL TMP = (chrono::high_resolution_clock::now().time_since_epoch().count() % MOD + MOD) % MOD;
            LL seed = TMP;
            srand(TMP);
            Player p0;
            Player p1;
            Core c0(parameter + delta);
            Core c1(parameter - delta);
            Game game(seed, p0, p1);
            for (int turn = 1; turn <= 100; turn++) {
                game.resetGameTurnData();
                //p0.set_info();
                {
                    std::string p0_rep = c0.execute_turn(game.getCurrentFrameInfoFor(p0));
                    stringstream ss(p0_rep);
                    std::string type; ss >> type;
                    int spellId; ss >> spellId;
                    int repetitions;
                    if (ss >> repetitions) {
                        p0.action = SpellAction(type, spellId, repetitions);
                    }
                    else {
                        p0.action = SpellAction(type, spellId);
                    }
                }
                //p1.set_info();
                {
                    std::string p1_rep = c1.execute_turn(game.getCurrentFrameInfoFor(p1));
                    stringstream ss1(p1_rep);
                    std::string type; ss1 >> type;
                    int spellId1; ss1 >> spellId1;
                    int repetitions1;
                    if (ss1 >> repetitions1) {
                        p1.action = SpellAction(type, spellId1, repetitions1);
                    }
                    else {
                        p1.action = SpellAction(type, spellId1);
                    }
                }
                game.performGameUpdate();
                if (game.gameOver()) {
                    break;
                }
                assert(c0.glob_m_brewed == c1.glob_o_brewed && c1.glob_m_brewed == c0.glob_o_brewed);
                assert(c0.glob_m_brewed == p0.getDeliveriesCount() || c0.glob_m_brewed + 1 == p0.getDeliveriesCount());
                assert(c1.glob_m_brewed == p1.getDeliveriesCount() || c1.glob_m_brewed + 1 == p1.getDeliveriesCount());
            }
            game.onEnd();
            std::cout << "p0 score: " << p0.getScore() << " p1 score: " << p1.getScore() << "\n";
            if (p0.getScore() > p1.getScore()) {
                p0_wins++;
            }
            else if (p0.getScore() < p1.getScore()) {
                p1_wins++;
            }
            std::cout << "p0 wins: " << p0_wins << " p1 wins: " << p1_wins << "\n";
        }
        if (p0_wins > p1_wins) {
            parameter += delta;
        }
        else if(p0_wins < p1_wins){
            parameter -= delta;
        }
        cout << "parameter: " << parameter << "\n";
        cout << "average: " << sum / rep << "\n";
    }
}
