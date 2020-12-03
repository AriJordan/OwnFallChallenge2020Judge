#pragma once
#pragma GCC optimize("O3")
#pragma GCC optimize("inline")
#pragma GCC optimize("omit-frame-pointer")
#pragma GCC optimize("unroll-loops")
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <unordered_set>
#include <chrono>
#include <cassert>
#include <fstream>
#include <bitset>

#define ULL unsigned long long
#define PULL pair<ULL, ULL>

using namespace std;

/////////////// Configuration /////////////////
//ULL SIMULATE_STATES = 2;
ULL BITS_PER_ITEM = 6;
ULL FIRST_24_BITS = 0xffffff; // TODO: test valid_inventory() with this
ULL SIXTH_BITS = 0x820820;
ULL FIFTH_BITS = 0x410410;
///////////////////////////////////////////////

///////////////own spell values////////////////
map<vector<int>, double> LEARNS_MAP =
{ {{-3, 0, 0,  1},  0.0}, {{3, -1, 0, 0},  1.0}, {{1, 1, 0,  0},  0.0}, {{0, 0, 1,  0},  0.0}, {{3, 0, 0,  0},  0.0}, {{2, 3, -2, 0}, -1.0},
{ {2,  1, -2, 1}, -1.0}, {{3, 0, 1, -1},  0.2}, {{3, -2, 1, 0},  0.0}, {{2, -3, 2, 0}, -1.0}, {{2, 2, 0, -1},  0.2}, {{-4, 0, 2, 0},  0.0},
{ {2,  1, 0,  0},  0.5}, {{4, 0, 0,  0},  0.6}, {{0, 0, 0,  1},  0.5}, {{0, 2, 0,  0},  0.0}, {{1, 0, 1,  0},  0.2}, {{-2, 0, 1, 0},  0.9},
{ {-1, -1, 0, 1},  0.5}, {{0, 2, -1, 0},  0.6}, {{2, -2, 0, 1},  0.5}, {{-3, 1, 1, 0},  0.5}, {{0, 2, -2, 1},  0.0}, {{1, -3, 1, 1},  0.4},
{ {0,  3, 0, -1},  0.5}, {{0, -3, 0, 2},  0.4}, {{1, 1, 1, -1},  0.3}, {{1, 2, -1, 0},  0.2}, {{4, 1, -1, 0},  0.2}, {{-5, 0, 0, 2},  0.0},
{ {-4, 0, 1,  1},  0.0}, {{0, 3, 2, -2}, -0.4}, {{1, 1, 3, -2}, -0.5}, {{-5, 0, 3, 0}, -0.5}, {{-2, 0,-1, 2},  0.0}, {{0, 0, -3, 3},  0.5},
{ {0, -3, 3,  0},  0.6}, {{-3, 3, 0, 0},  0.7}, {{-2, 2, 0, 0},  1.9}, {{0, 0, -2, 2},  1.7}, {{0, -2, 2, 0},  1.8}, {{0, 0, 2, -1},  1.7} };

/////////////////// Parameters ////////////////
const int OPPONENT_SIMULATE_TURNS = 9;
const int OPPONENT_MAX_STATES_PER_D = 90;
const int ADDITIONAL_SIMULATE_TURNS = 2; // Number of additional turns to simulate
const int TOTAL_STATES = 10000;
//const int MAX_STATES_PER_D = 400; // Number of different states per distance
const double EXPECTED_SCORE_PER_TURN = 3.1;
const double SHORT_TERM_DIS = 9.5;
const double SPELL_VALUE_MUL = 0.012;
const int EXPECTED_SPELLS_LEARNED = 9;
const double SPELLS_LEARNED_EXP = 1.3;
const double VALUE_DENSITY_MUL = 4.0;
const double OWN_VALUE_MUL = 1.15;
const double SPELLS_SIMILARITY_VALUE_MUL = 3.4;
const double RECIPES_SIMILARITY_VALUE_MUL = 3.0;
const double SPELL_CNT_DIFF_MUL = 0.9;
const double CASTABLE_BONUS_MUL = EXPECTED_SCORE_PER_TURN;
const double COULD_BREW_BONUS[6] = { 0.0, 3.0, 5.0, 6.0, 6.5, 7.0 };
// Note that brewing decreases could_brew and takes a turn but but increases score because nonlinear item value
const double BREWED_CNT_BONUS_MUL = COULD_BREW_BONUS[5] + EXPECTED_SCORE_PER_TURN - 5.0;
const double SIX_BREWED_BONUS = 12.0;
const double OPPONENT_FINISHED_MALUS = 8.0; // applied once for  every turn as opponent finishes and after
const double BLUE_VAL = 1.0; // value of tier 0 item // 1.1 before
const double ITEM_VAL_MUL = 0.9;
// TODO: consider spells left after brewing
///////////////////////////////////////////////

//////////////// Mode ////////////////
const bool DEBUG = true; // Print cerr messages
const bool PRINT_INPUT = false; // Print all input to cerr
#ifdef ARI_LOCAL
bool IS_LOCAL = true;
#else
bool IS_LOCAL = false;
#endif
const int LOCAL_TIME_BONUS = 300; // local addition of milliseconds
//////////////////////////////////////


auto STTMP = chrono::steady_clock::now();
std::vector<int> op_plus_res(4);
inline vector<int> operator+(const vector<int>& v1, const vector<int>& v2)
{
    assert(v1.size() == 4 && v2.size() == 4);
    for (int i = 0; i < 4; i++) {
        op_plus_res[i] = v1[i] + v2[i];
    }
    return op_plus_res;
}

std::vector<int> op_min_res(4);
inline vector<int> operator-(const vector<int>& v1, const vector<int>& v2)
{
    assert(v1.size() == 4 && v2.size() == 4);
    for (int i = 0; i < 4; i++) {
        op_min_res[i] = v1[i] - v2[i];
    }
    return op_min_res;
}

inline int one_norm(ULL st) {
    assert((st & FIFTH_BITS) == FIFTH_BITS && "state should only have FIFTH_BITS bits set");
    ULL MOD = (1ull << (BITS_PER_ITEM - 2));
    return (int)(st % MOD + (st >> BITS_PER_ITEM) % MOD + (st >> (2 * BITS_PER_ITEM)) % MOD + (st >> (3 * BITS_PER_ITEM)) % MOD); // TODO: test this more efficient
}

struct action {
    int actionId;
    vector<int> deltas;
    int price;
    int repetitions;
    int taxCount;
    bool castable;
    bool repeatable;
    inline action(int actionId, vector<int> deltas, int price, int repetitions, int taxCount, bool castable, bool repeatable) :
        actionId(actionId), deltas(deltas), price(price), repetitions(repetitions), taxCount(taxCount), castable(castable), repeatable(repeatable) {};
};

struct information {
    vector<action> recipes;
    vector<action> learns;
    vector<action> spells;
    vector<int> inventory;
};
template<typename T>
inline string v_to_str(const vector<T>& v) {
    string res = "[";
    for (auto el : v) {
        res += to_string(el) + ",";
    }
    res.pop_back();
    res += "]";
    return res;
}

inline string state_to_str(information state) {
    string res = "[";
    res += "]";
    return res;
}

// POST: Sets the flag bit positions
inline void set_flag_pos(int spls_sz, vector<ULL>& recipes_flag_pos, vector<PULL>& learns_flag_pos, vector<ULL>& spells_flag_pos) {
    unsigned int shift = 4u * (unsigned int)BITS_PER_ITEM;
    for (unsigned int i = shift; i < shift + 5; i++) {
        recipes_flag_pos.push_back((unsigned int)i);
    }
    shift += 5; // recipes.size()
    for (unsigned int i = shift; i < shift + 6; i++) {
        learns_flag_pos.push_back({ (unsigned int)i, i + 6u });
    }
    shift += 12; // 2 * learns.size()
    for (unsigned int i = shift; i < shift + spls_sz; i++) {
        spells_flag_pos.push_back((unsigned int)i);
    }
}

// POST: Sets the bits corresponding to items
inline ULL get_bits(const std::vector<int>& items) {
    assert(items.size() == 4);
    ULL st = 0;
    for (unsigned int i = 0; i < 4ull; i++) {
        st += ((ULL)items.at(i) + (1ull << (BITS_PER_ITEM - 2ull))) << (i * BITS_PER_ITEM);
    }
    return st;
}

// POST: Sets the bits for the items in recipes, spells and learns
inline void set_bits(const vector<action>& recipes, const vector<action>& learns, const vector<action>& spells,
    vector<ULL>& recipes_bits, vector<ULL>& learns_bits, vector<ULL>& spells_bits) {
    for (unsigned int r = 0; r < recipes.size(); r++) {
        recipes_bits.push_back(get_bits(recipes.at(r).deltas));
    }
    for (unsigned int s = 0; s < spells.size(); s++) {
        spells_bits.push_back(get_bits(spells.at(s).deltas));
    }
    for (unsigned int l = 0; l < learns.size(); l++) {
        learns_bits.push_back(get_bits(learns.at(l).deltas));
    }
}

// PRE: No recipes brewed and spells learned
// RETURN: state
inline unsigned long long info_to_state(const information& info) {
    assert(info.recipes.size() == 5);
    assert(info.learns.size() == 6);
    assert(info.spells.size() >= 4);
    assert(info.inventory.size() == 4);
    unsigned long long st = get_bits(info.inventory);
    unsigned long long shift = 4 * BITS_PER_ITEM + info.recipes.size() + 2ull * info.learns.size();
    for (unsigned int i = 0; i < info.spells.size(); i++) {
        st |= (info.spells.at(i).castable * (1ull << (shift + i)));
    }

    for (unsigned long long p = 0; p < 17; p++) { // no recipes brewed and spells learned
        assert(!(st & (1ull << (4ull * BITS_PER_ITEM + p))));
    }
    assert(shift + info.spells.size() <= 64);
    for (unsigned long long p = shift + info.spells.size(); p <= 63; p++) {
        assert(!(st & (1ull << p)));
    }
    return st;
}

//RETURN: Whether state's items is valid
inline bool valid_inventory(ULL st) {
    if ((st & FIFTH_BITS) != FIFTH_BITS)
        return false;
    if (st & SIXTH_BITS)
        return false;
    return one_norm(st) <= 10;
}

inline ULL add_items(ULL state, ULL change) {
    assert(change - (change & FIRST_24_BITS) == 0 && "change should only have item bits set");
    return (state + change - FIFTH_BITS);
}

inline ULL mul_items(ULL state, ULL mul) {
    assert(state - (state & FIRST_24_BITS) == 0 && "state should only have item bits set");
    return state * mul - (mul - 1) * FIFTH_BITS;
}

struct state_info {
    ULL state;
    double score; // total score of state
    int dist;
};

inline std::string type_to_str(int type) {
    if (type == 0) {
        return "BREW";
    }
    else if (type == 1) {
        return "LEARN";
    }
    else if (type == 2) {
        return "CAST";
    }
    else if (type == 3) {
        return "REST";
    }
    else {
        assert(0 && "invalid type");
        return "ERROR";
    }
}

struct change_info {
    //inline change_info() : action_type("AriError"), action_pos(-1), repetitions((int)-1e9), state_from(0), is_learned_spell(false){}
    int action_type; // std::string action_type; 0 = BREW, 1 = LEARN, 2 = CAST, 3 = REST
    unsigned int action_pos;
    int repetitions;
    ULL state_from;
    bool is_learned_spell;
    double score;
};

struct pq_info {
    change_info change;
    unsigned long long state;
    int dist;
    double score;
};

inline double item_val(int item) {
    return (item + BLUE_VAL) * ITEM_VAL_MUL;;
}

//Returns rupee value of recipes brewed in state
inline double state_recipes_value(ULL state, const vector<action>& recipes) {
    assert(recipes.size() == 5);
    double val = 0;
    for (unsigned int i = 0; i < 5; i++) {
        if ((1ull << (i + 4ull * BITS_PER_ITEM)) & state) {
            val += recipes.at(i).price;
        }
    }
    return val;
}

//Returns value of newly learned spells
inline double state_learned_value(ULL state, const vector<double>& learns_value) {
    assert(learns_value.size() == 6);
    double val = 0;
    for (unsigned int i = 0; i < 6; i++) {
        if ((1ull << (4ull * BITS_PER_ITEM + 5 + i)) & state) {
            val += learns_value.at(i);
        }
    }
    return val;
}

//Returns number of recipes brewed in this state
inline int state_brewed_cnt(ULL state, int brewed_before) {
    int brewed_count = brewed_before;
    for (unsigned int i = 0; i < 5; i++) {
        if ((1ull << (i + 4ull * BITS_PER_ITEM)) & state) {
            brewed_count++;
        }
    }
    return brewed_count;
}

// RETURN: number of recipes that could be brewed
inline double state_could_brew_val(ULL state, const vector<action>& recipes) {
    assert(recipes.size() == 5);
    int could_brew_cnt = 5;
    ULL MOD = (1ull << (BITS_PER_ITEM - 2));
    for (int r = 0; r < 5; r++) {
        if (!(state & (1ull << (4ull * BITS_PER_ITEM + r)))) { // not brewed yet
            for (int type = 0; type < 4; type++) {
                if ((state >> (type * BITS_PER_ITEM)) % MOD < recipes.at(r).deltas.at(type)) {
                    could_brew_cnt--;
                    break;
                }
            }
        }
    }
    return COULD_BREW_BONUS[could_brew_cnt];
}

// RETURN: Number of learnables learned in this state
inline int state_learns_learned_cnt(ULL state, ULL only_to = 6) {
    int learned_cnt = 0;
    for (unsigned int i = 0; i < only_to; i++) {
        if ((1ull << (4ull * BITS_PER_ITEM + 5 + i)) & state) {
            learned_cnt++;
        }
    }
    return learned_cnt;
}

// RETURN: Number of castable (spells + learns)
inline int state_castable_cnt(ULL state) {
    auto bs = std::bitset<64>(state >> (4ull * BITS_PER_ITEM + 5 + 6));
    return bs.count();
}

// RETURN: Approx non-final value of inventory
inline double early_inventory_value(ULL state) {
    double val = 0;
    ULL MOD = (1ull << (BITS_PER_ITEM - 2));
    for (int i = 0; i < 4; i++) {
        val += item_val(i) * ((state >> (i * BITS_PER_ITEM)) % MOD);
    }
    return 1.3 * val - 0.014 * val * val;
}

// RETURN: Final value of inventory
inline double final_inventory_value(ULL state) {
    double val = 0;
    ULL MOD = (1ull << (BITS_PER_ITEM - 2));
    for (int i = 1; i < 4; i++) {
        val += ((state >> (i * BITS_PER_ITEM)) % MOD);
    }
    return val;
}

int glob_m_brewed = 0, glob_o_brewed = 0;
int glob_opp_turns_needed = 100;
double get_state_score_difference(ULL state, const vector<action>& recipes, const vector<double>& learns_value, int spells_cnt, double startvalue, int dist, int mode) {
    double learned_cnt = (double)spells_cnt + state_learns_learned_cnt(state);
    assert(learns_value.size() == 6);
    assert(learned_cnt >= 4);
    int brewed_cnt;
    if (mode == 1) {
        brewed_cnt = state_brewed_cnt(state, glob_m_brewed);
    }
    else {
        brewed_cnt = state_brewed_cnt(state, glob_o_brewed);
    }
    double items_val = early_inventory_value(state);
    if (brewed_cnt >= 5 || glob_m_brewed >= 5 || glob_o_brewed >= 5) items_val = final_inventory_value(state);
    else if (brewed_cnt >= 4 || glob_m_brewed >= 4 || glob_o_brewed >= 4) items_val = (early_inventory_value(state) + final_inventory_value(state)) / 2.0;
    double recipes_val = state_recipes_value(state, recipes);
    double learned_val = state_learned_value(state, learns_value) * EXPECTED_SPELLS_LEARNED / pow((double)learned_cnt, SPELLS_LEARNED_EXP);
    double spells_castable_bonus = (double)state_castable_cnt(state) / learned_cnt * CASTABLE_BONUS_MUL;
    double could_brew_bonus = state_could_brew_val(state, recipes);
    double brewed_cnt_bonus = brewed_cnt * BREWED_CNT_BONUS_MUL;
    double finished_bonus = (brewed_cnt == 6) * SIX_BREWED_BONUS;
    double opp_finished_malus = (double)(mode == 1) * (dist >= glob_opp_turns_needed) * (dist + 1.0 - glob_opp_turns_needed) * OPPONENT_FINISHED_MALUS;
    if (mode == 0 && brewed_cnt == 6) {
        glob_opp_turns_needed = min(glob_opp_turns_needed, dist);
    }
    return items_val + recipes_val + learned_val + spells_castable_bonus + could_brew_bonus + brewed_cnt_bonus + finished_bonus - opp_finished_malus - startvalue;
}

inline double abs_it_dist(const action& a1, const action& a2) {
    double it_dist = 0;
    for (int i = 0; i < 4; i++) {
        it_dist += item_val(i) * abs(a1.deltas.at(i) - a2.deltas.at(i));
    }
    return it_dist;
}

inline double abs_it_val(const action& a) {
    double it_val = 0;
    for (int i = 0; i < 4; i++) {
        it_val += item_val(i) * abs(a.deltas.at(i));
    }
    return it_val;
}

int glob_step_num = 0;
inline int calc_expected_turns() {
    return 2.5 * (6ll - glob_m_brewed) + 2.5 * (6ll - glob_o_brewed) + max(0, 10 - glob_step_num); // 10 turns to learn
}

// Return calculated learns value
inline vector<double> items_learns_val(const vector<action>& learns) {
    vector<double> items_learns_value;
    for (int l = 0; l < 6; l++) {
        double val = 0;
        int tot_neg = 0;
        int tot_pos = 0;
        for (int i = 0; i < 4; i++) {
            //std::cerr << "tot_in_sp: " << tot_it_in_sp << " items_in_spells.at(" << i << "): " << items_in_spells.at(i) << "\n";
            //std::cerr << "multiplier: " << ((tot_it_in_sp - it_in_sp_wei * (items_in_spells.at(i))) / (double)tot_it_in_sp) << "\n";
            //std::cerr << "item_val: " << item_val(i) << " l.deltas.at(i): " << l.deltas.at(i) << "\n";
            double one_val = item_val(i) * learns.at(l).deltas.at(i);// *((tot_it_in_sp - it_in_sp_wei * (items_in_spells.at(i))) / (double)tot_it_in_sp); // last term is what spells we need
            if (learns.at(l).deltas.at(i) > 0) {
                tot_pos += learns.at(l).deltas.at(i);
            }
            else if (learns.at(l).deltas.at(i) < 0) {
                //one_val *= neg_mul;
                tot_neg += abs(learns.at(l).deltas.at(i));
            }
            //std::cerr << "one_val: " << one_val << "\n";
            val += one_val;
        }
        double value_density = val / max(tot_pos, tot_neg);
        //val -= sp_similar_sum(l, old_sps);
        double expected_value;
        if (learns.at(l).repeatable) {
            expected_value = value_density * VALUE_DENSITY_MUL;
        }
        else {
            expected_value = val;
        }
        items_learns_value.push_back(expected_value);
    }
    return items_learns_value;
}

// RETURN: own estimated learns value
inline vector<double> own_learns_val(const vector<action>& learns) {
    vector<double> own_learns_value;
    for (int l = 0; l < 6; l++) {
        own_learns_value.push_back(LEARNS_MAP.at(learns.at(l).deltas) * OWN_VALUE_MUL);
    }
    return own_learns_value;
}

inline double squared_dot_prod(const vector<double>& v1, const vector<double>& v2) {
    assert(v1.size() == 4);
    assert(v2.size() == 4);
    double dot_product = 0;
    for (int i = 0; i < 4; i++) {
        dot_product += v1.at(i) * v2.at(i);
    }
    if (dot_product == 0.0) return 0.0;
    return abs(dot_product) * dot_product;
}

// RETURN: Learns similarity with other spells
inline vector<double> spells_similarity_learns_val(const vector<action>& learns, const vector<action>& spells) {
    assert(learns.size() == 6);
    assert(spells.size() >= 4);
    vector<vector<int>> learns_items, spells_items;
    for (int l = 0; l < learns.size(); l++) {
        learns_items.push_back(learns.at(l).deltas);
    }
    for (int s = 0; s < spells.size() - 4; s++) {
        spells_items.push_back(spells.at(s + 4).deltas);
    }
    vector<vector<double>> learns_items_val(learns.size()), spells_items_val(spells.size());
    vector<vector<double>> learns_items_normalized(learns.size()), spells_items_normalized(spells.size());
    for (int l = 0; l < learns.size(); l++) {
        double vals_squared_sum = 0;
        for (int i = 0; i < 4; i++) {
            double val = item_val(i) * (double)learns_items.at(l).at(i);
            learns_items_val.at(l).push_back(val);
            vals_squared_sum += val * val;
        }
        double norm = sqrt(vals_squared_sum);
        for (int i = 0; i < 4; i++) {
            learns_items_normalized.at(l).push_back(learns_items_val.at(l).at(i) / norm);
        }
    }
    for (int s = 0; s < spells.size() - 4; s++) {
        double vals_squared_sum = 0;
        for (int i = 0; i < 4; i++) {
            double val = item_val(i) * (double)spells_items.at(s).at(i);
            spells_items_val.at(s).push_back(val);
            vals_squared_sum += val * val;
        }
        double norm = sqrt(vals_squared_sum);
        for (int i = 0; i < 4; i++) {
            spells_items_normalized.at(s).push_back(spells_items_val.at(s).at(i) / norm);
        }
    }
    vector<double> similarity_learns_value;
    for (int l = 0; l < learns.size(); l++) {
        double similarity = 0;
        /*if (DEBUG && learns.at(l).actionId == 41 || learns.at(l).actionId == 35) { cerr << "["; }*/
        for (int s = 0; s < spells.size() - 4; s++) {
            similarity += squared_dot_prod(learns_items_normalized.at(l), spells_items_normalized.at(s));
            /*if (DEBUG && learns.at(l).actionId == 41 || learns.at(l).actionId == 35) {
                cerr << "{";
                for (int i = 0; i < 4; i++) { cerr << spells_items_normalized.at(s).at(i) << ", "; }
                cerr << "} " << squared_dot_prod(learns_items_normalized.at(l), spells_items_normalized.at(s)) << ", "; }*/
        }
        /*if (DEBUG && learns.at(l).actionId == 41 || learns.at(l).actionId == 35) {cerr << "]\n";}*/
        similarity_learns_value.push_back(similarity / spells.size() * SPELLS_SIMILARITY_VALUE_MUL);
    }
    return similarity_learns_value;
}

// RETURN: Learns similarity with recipes
inline vector<double> recipes_similarity_learns_val(const vector<action>& learns, const vector<action>& recipes) {
    assert(learns.size() == 6);
    assert(recipes.size() == 5);
    vector<vector<int>> learns_items, recipes_items;
    for (int l = 0; l < learns.size(); l++) {
        learns_items.push_back(learns.at(l).deltas);
    }
    for (int r = 0; r < recipes.size(); r++) {
        recipes_items.push_back(recipes.at(r).deltas);
    }
    vector<vector<double>> learns_items_val(learns.size()), recipes_items_val(recipes.size());
    vector<vector<double>> learns_items_normalized(learns.size()), recipes_items_normalized(recipes.size());
    for (int l = 0; l < learns.size(); l++) {
        double vals_squared_sum = 0;
        for (int i = 0; i < 4; i++) {
            double val = item_val(i) * (double)learns_items.at(l).at(i);
            learns_items_val.at(l).push_back(val);
            vals_squared_sum += val * val;
        }
        double norm = sqrt(vals_squared_sum);
        for (int i = 0; i < 4; i++) {
            learns_items_normalized.at(l).push_back(learns_items_val.at(l).at(i) / norm);
        }
    }
    for (int r = 0; r < recipes.size(); r++) {
        double vals_squared_sum = 0;
        for (int i = 0; i < 4; i++) {
            double val = item_val(i) * (double)recipes_items.at(r).at(i);
            recipes_items_val.at(r).push_back(val);
            vals_squared_sum += val * val;
        }
        double norm = sqrt(vals_squared_sum);
        for (int i = 0; i < 4; i++) {
            recipes_items_normalized.at(r).push_back(recipes_items_val.at(r).at(i) / norm);
        }
    }
    vector<double> similarity_learns_value;
    for (int l = 0; l < learns.size(); l++) {
        double similarity = 0;
        for (int r = 0; r < recipes.size(); r++) {
            similarity += squared_dot_prod(learns_items_normalized.at(l), recipes_items_normalized.at(r));
        }
        similarity_learns_value.push_back(similarity / recipes.size() * RECIPES_SIMILARITY_VALUE_MUL * (((double)glob_m_brewed + glob_o_brewed + 7.5) / 15.0));
    }
    return similarity_learns_value;
}

// RETURN: value of learning spells
int glob_m_spells, glob_o_spells;
vector<double> get_learns_value(const vector<action>& learns, const vector<action>& old_spells, const vector<action>& recipes, int step_num, bool cerr_out) {
    vector<double> items_learns_value = items_learns_val(learns);
    vector<double> own_learns_value = own_learns_val(learns);
    vector<double> spells_similarity_value = spells_similarity_learns_val(learns, old_spells);
    vector<double> recipes_similarity_value = recipes_similarity_learns_val(learns, recipes);
    vector<double> total_learns_value;
    for (int l = 0; l < learns.size(); l++) {
        total_learns_value.push_back(
            (items_learns_value.at(l) + own_learns_value.at(l) - spells_similarity_value.at(l) - recipes_similarity_value.at(l))
            * calc_expected_turns() * SPELL_VALUE_MUL + ((double)glob_o_spells - glob_m_spells) * SPELL_CNT_DIFF_MUL);
    }
    if (DEBUG && cerr_out) {
        cerr << "TOTAL LEARNS VALUE:\n" << v_to_str(total_learns_value) << "\n";
        cerr << "Items value:\n" << v_to_str(items_learns_value) << "\n";
        cerr << "Own value:\n" << v_to_str(own_learns_value) << "\n";
        cerr << "Spells similarity value:\n" << v_to_str(spells_similarity_value) << "\n";
        cerr << "Recipes similarity value:\n" << v_to_str(recipes_similarity_value) << "\n";
    }
    return total_learns_value;
}

// RETURN: best path
vector<change_info> get_best_path(unordered_map<unsigned long long, change_info>& cof, ULL final_state, ULL start_state) {
    ULL cpos = final_state;
    //std::cerr << "cpos: " << cpos << "\n";
    vector<change_info> best_path;
    while (cpos != start_state) {
        best_path.push_back(cof.at(cpos));
        cpos = cof.at(cpos).state_from;
        //std::cerr << "cpos: " << cpos << "\n";
    }
    reverse(best_path.begin(), best_path.end()); // put spells in correct order
    return best_path;
}

inline void cerr_print_path(const vector<change_info>& path, const vector<action>& recipes, const vector<action>& learns, const vector<action>& spells) {
    assert(path.size() > 0);
    assert(recipes.size() == 5);
    assert(learns.size() == 6);
    if (DEBUG) {
        string outp = "### Actions on best path ###\n [";
        for (const auto& change : path) {
            outp += type_to_str(change.action_type) + " ";
            if (change.repetitions > 1) {
                outp += to_string(change.repetitions) + "*";
            }
            if (change.action_type == 0) {
                outp += to_string(recipes.at(change.action_pos).actionId);
            }
            else if (change.action_type == 1) {
                outp += to_string(learns.at(change.action_pos).actionId);
            }
            else if (change.action_type == 2) {
                if (change.is_learned_spell) {
                    outp += to_string(learns.at(change.action_pos).actionId);
                }
                else {
                    outp += to_string(spells.at(change.action_pos).actionId);
                }
            }
            else if (change.action_type == 3) {

            }
            else {
                assert(0);
            }
            outp += ", ";
        }
        outp.pop_back(); outp.pop_back(); outp += "]";
        std::cerr << outp << "\n";
    }
}

inline void cerr_print_step_scores(const vector<change_info>& path) {
    string outp = "### Step scores on best path ###\n [";
    outp += to_string(path.at(0).score) + ", ";
    for (int ch = 1; ch < path.size(); ch++) {
        outp += to_string(path.at(ch).score - path.at(ch - 1).score) + ", ";
    }
    outp.pop_back(); outp.pop_back(); outp += "]";
    cerr << outp << "\n";
}

void print_first_action(const vector<change_info>& path, const vector<action>& recipes, const vector<action>& learns, const vector<action>& spells, double best_score_per_turn) {
    assert(path.size() > 0);
    assert(recipes.size() == 5);
    assert(learns.size() == 6);
    string outp = "";
    change_info change = path.at(0);
    outp += type_to_str(change.action_type) + " ";
    if (change.action_type == 0) {
        outp += to_string(recipes.at(change.action_pos).actionId);
    }
    else if (change.action_type == 1) {
        outp += to_string(learns.at(change.action_pos).actionId);
    }
    else if (change.action_type == 2) {
        outp += to_string(spells.at(change.action_pos).actionId);
    }
    else if (change.action_type == 3) {

    }
    else {
        assert(0);
    }
    outp += " ";
    if (change.repetitions > 1) {
        outp += to_string(change.repetitions) + " ";
    }
    cout << outp;
    auto ENTMP = chrono::steady_clock::now(); int ELTIME = (int)(chrono::duration_cast<chrono::milliseconds>(ENTMP - STTMP).count());
    cout << "s:" << best_score_per_turn << " t: " << ELTIME << " ms" << endl;
}

auto pq_cmp = [](const pq_info& info1, const pq_info& info2) {
    if (info1.dist != info2.dist) {
        return info1.dist > info2.dist;
    }
    else {
        return info1.score < info2.score;
    }
};

inline double adj_score_per_turn(double new_value_d, int dist) {
    return new_value_d / (dist + 0.001) / (SHORT_TERM_DIS * 1.0 / dist + 1.0);
}

int nodetot = 0;
int addtot = 0;


void bfs(const unsigned long long start, const vector<action>& recipes, const vector<action>& learns, const vector<action>& spells, int mode, unsigned int turns, unsigned int max_states) {
    if (DEBUG) { cerr << "Starting bfs in mode: " << mode << "\n"; }
    assert(recipes.size() == 5);
    assert(spells.size() <= 23);
    assert(learns.size() == 6);
    vector<double> learns_value = get_learns_value(learns, spells, recipes, glob_step_num, (mode == 1) && true);;
    double start_value = get_state_score_difference(start, recipes, learns_value, spells.size(), 0, 0, mode);
    vector<unsigned long long> recipes_flag_pos, spells_flag_pos;
    vector<pair<unsigned long long, unsigned long long>> learns_flags_pos;
    set_flag_pos((int)spells.size(), recipes_flag_pos, learns_flags_pos, spells_flag_pos);
    vector<unsigned long long> recipes_bits, learns_bits, spells_bits;
    set_bits(recipes, learns, spells, recipes_bits, learns_bits, spells_bits);
    assert(recipes_flag_pos.size() == recipes_bits.size() && learns_flags_pos.size() == learns_bits.size() && spells_flag_pos.size() == spells_bits.size());
    unordered_set<unsigned long long> vis;
    unordered_set<unsigned long long> processed;
    unordered_map<unsigned long long, change_info> cof; //state to {action_type, {position (in vector), repetitions}}
    vector<int> dist_amount(turns + 1);
    priority_queue<pq_info, vector<pq_info>, decltype(pq_cmp)> pq(pq_cmp); pq.push({ {-1, 666u, -2, (ULL)1e9, false}, start, 0, 0.0 });
    state_info best_state = { start, -1e9, 1 };
    double best_score_per_turn = 0;
    while (!pq.empty()) {
        auto [from, cur_state, dist, cur_score] = pq.top(); pq.pop();
        if (dist == turns)break;
        assert(dist_amount.at(dist) <= max_states);
        if (dist_amount.at(dist) == max_states) continue;
        if (processed.count(cur_state)) continue;
        processed.insert(cur_state);
        if (dist_amount.at(dist) == 0) {
            auto ENTMP = chrono::steady_clock::now(); int ELTIME = (int)(chrono::duration_cast<chrono::milliseconds>(ENTMP - STTMP).count());
            if (ELTIME > 40 - (pq.size() / 700) + IS_LOCAL * LOCAL_TIME_BONUS)
                break;
            learns_value = get_learns_value(learns, spells, recipes, dist + glob_step_num, false);
            assert(learns_value.size() == 6);
        }
        dist_amount.at(dist)++;
        if (dist > 0 && adj_score_per_turn(cur_score, dist) > adj_score_per_turn(best_state.score, best_state.dist)) { // new best (unnecessary to put here because only happens for first state with certain distance)
            best_score_per_turn = adj_score_per_turn(cur_score, dist);
            best_state = { cur_state, cur_score, dist };
        }
        cof[cur_state] = from;
        for (unsigned int r = 0; r < recipes.size(); r++) {
            if (!(cur_state & (1ull << recipes_flag_pos.at(r)))) { // recipe not brewed yet
                ULL new_state = add_items(cur_state, recipes_bits.at(r));
                new_state += (1ull << recipes_flag_pos.at(r));
                if (valid_inventory(new_state)) { // can brew recipe r
                    double new_score_d = get_state_score_difference(new_state, recipes, learns_value, spells.size(), start_value, dist + 1, mode);
                    if (!vis.count(new_state)) {
                        vis.insert(new_state);
                        pq.push({ {0, r, 0, cur_state, false, new_score_d}, new_state, dist + 1, new_score_d });
                    }
                }
            }
        }
        for (unsigned int l = 0; l < learns.size(); l++) {
            if (cur_state & (1ull << learns_flags_pos.at(l).first)) {
                if (cur_state & (1ull << learns_flags_pos.at(l).second)) {
                    for (unsigned int repetitions = 1;; repetitions++) {
                        if (repetitions == 1 || learns.at(l).repeatable) {
                            ULL new_state = cur_state ^ (1ull << learns_flags_pos.at(l).second);
                            assert(repetitions <= 6);
                            ULL change = mul_items(learns_bits.at(l), repetitions);
                            new_state = add_items(new_state, change);
                            if (valid_inventory(new_state)) {
                                double new_score_d = get_state_score_difference(new_state, recipes, learns_value, spells.size(), start_value, dist + 1, mode);
                                if (!vis.count(new_state)) {
                                    vis.insert(new_state);
                                    pq.push({ {2, l, (int)repetitions, cur_state, true, new_score_d}, new_state, dist + 1, new_score_d });
                                }
                            }
                            else {
                                break;
                            }
                        }
                        else {
                            break;
                        }
                    }
                }
            }
            else { // Not learned yet
                ULL gone = state_learns_learned_cnt(cur_state, l);
                assert(gone >= 0 && gone <= 5);
                assert(!(cur_state & (1ull << learns_flags_pos.at(l).first)));
                ULL new_state = cur_state ^ (1ull << learns_flags_pos.at(l).first); // bit represents spell learned
                assert(!(new_state & (1ull << learns_flags_pos.at(l).second)));
                new_state = new_state ^ (1ull << learns_flags_pos.at(l).second); // learned spell is castable
                new_state = add_items(new_state, FIFTH_BITS - (l - gone)); // pay tax // TODO: handle when I increased the tax
                if (valid_inventory(new_state)) {
                    new_state = add_items(new_state, FIFTH_BITS + min(learns.at(l).taxCount, 10 - one_norm(new_state))); // receive tax
                    double new_score_d = get_state_score_difference(new_state, recipes, learns_value, spells.size(), start_value, dist + 1, mode);
                    if (!vis.count(new_state)) {
                        vis.insert(new_state);
                        pq.push({ {1, l, 0, cur_state, true, new_score_d}, new_state, dist + 1, new_score_d });
                    }
                }
            }
        }
        for (unsigned int s = 0; s < spells.size(); s++) {
            if (cur_state & (1ull << spells_flag_pos.at(s))) {
                for (unsigned int repetitions = 1;; repetitions++) {
                    if (repetitions == 1 || spells.at(s).repeatable) {
                        ULL new_state = cur_state ^ (1ull << spells_flag_pos.at(s));
                        assert(repetitions <= 6);
                        ULL change = mul_items(spells_bits.at(s), repetitions);
                        new_state = add_items(new_state, change);
                        if (valid_inventory(new_state)) {
                            double new_score_d = get_state_score_difference(new_state, recipes, learns_value, spells.size(), start_value, dist + 1, mode);
                            if (!vis.count(new_state)) {
                                vis.insert(new_state);
                                pq.push({ {2, s, (int)repetitions, cur_state, false, new_score_d}, new_state, dist + 1, new_score_d });
                            }
                        }
                        else {
                            break;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
        }
        {
            ULL new_state = cur_state;
            for (auto b = learns_flags_pos.at(0).first; b <= learns_flags_pos.at(5).first; b++) {
                if (cur_state & (1ull << b)) {
                    new_state |= (1ull << (b + 6));
                }
                else {
                    assert(!(new_state & (1ull << (b + 6))));
                }
            }
            for (auto b = spells_flag_pos.at(0); b <= spells_flag_pos.back(); b++) {
                new_state |= (1ull << b);
            }
            double new_score_d = get_state_score_difference(new_state, recipes, learns_value, spells.size(), start_value, dist + 1, mode);
            if (!vis.count(new_state)) {
                vis.insert(new_state);
                pq.push({ {3, 666u, 0, cur_state, false, new_score_d}, new_state, dist + 1, new_score_d });
            }
        }
    }
    assert(best_state.dist != -1);
    vector<change_info> best_path = get_best_path(cof, best_state.state, start);
    if (DEBUG) {
        if (mode == 0) { cerr << "### OPPONENT: ###\n Needed turns: " << glob_opp_turns_needed << "\n"; }
        else if (mode == 1) { cerr << "##### ME: #####\n Taking turns: " << best_path.size() << "\n"; }
    }
    if (DEBUG) { cerr_print_path(best_path, recipes, learns, spells); }
    if (DEBUG) { cerr_print_step_scores(best_path); }
    if (mode == 1) {
        print_first_action(best_path, recipes, learns, spells, best_score_per_turn);
    }
    if (DEBUG) { cerr << " end of bfs "; }
    //nodetot += nodes;
}

int main()
{
    ios_base::sync_with_stdio(0);
    //std::cin.tie(0);
    std::cout.precision(3);
    srand(42);
#ifdef ARI_LOCAL
    ifstream cin("ch2020_output.txt");
#endif // 
    vector<int> old_minv, old_oinv; // old iventories
    vector<action> old_recipes;
    // game loop
    while (1) {
        glob_step_num++;
        std::vector<action> recipes, mspells, ospells, learns;
        int actionCount; // the number of spells and recipes in play
        map<int, int> recId_to_pos;
        map<int, int> pos_to_recId;
        cin >> actionCount;
        STTMP = chrono::steady_clock::now();
        if (PRINT_INPUT) { cerr << " INPUT_BEGIN_HERE "; }
        if (PRINT_INPUT) { std::cerr << actionCount << " "; }
        for (int i = 0; i < actionCount; i++) {
            int actionId; // the unique ID of this spell or recipe
            string actionType; // in the first league: BREW; later: CAST, OPPONENT_CAST, LEARN, BREW
            int delta0, delta1, delta2, delta3;
            int price; // the price in rupees if this is a potion
            int tomeIndex; // in the first two leagues: always 0; later: the index in the tome if this is a tome spell, equal to the read-ahead tax
            int taxCount; // in the first two leagues: always 0; later: the amount of taxed tier-0 ingredients you gain from learning this spell
            bool castable; // in the first league: always 0; later: 1 if this is a castable player spell
            bool repeatable; // for the first two leagues: always 0; later: 1 if this is a repeatable player spell
            cin >> actionId >> actionType >> delta0 >> delta1 >> delta2 >> delta3 >> price >> tomeIndex >> taxCount >> castable >> repeatable;
            if (PRINT_INPUT) { cerr << actionId << " " << actionType << " " << delta0 << " " << delta1 << " " << delta2 << " " << delta3 << " " << price << " " << tomeIndex << " " << taxCount << " " << castable << " " << repeatable << " "; }
            action act = { actionId, {delta0, delta1, delta2, delta3}, price, 1, taxCount, castable, repeatable };
            if (actionType == "BREW") { pos_to_recId[(int)recipes.size()] = actionId; recId_to_pos[actionId] = (int)recipes.size(); recipes.push_back(act); }
            else if (actionType == "CAST") { mspells.push_back(act); }
            else if (actionType == "OPPONENT_CAST") { ospells.push_back(act); }
            else if (actionType == "LEARN") { learns.push_back(act); }
            else { if (DEBUG) { std::cerr << "Unknown action in input\n"; }; }
        }
        int mlearned_num = mspells.size();
        glob_m_spells = mspells.size();
        glob_o_spells = ospells.size();
        /*vector<int> items_on_spells = { -2, 0, 0, 0 };
        int tot_it_in_sp = 0;
        for (int sp_pos = mspells.size() - 1; sp_pos >= 0; sp_pos--) {
            action max_spell = mspells.at(sp_pos);
            if (mspells.at(sp_pos).repeatable) {
                for (int mul = 2; mul <= 6; mul++) {
                    if (mul > 2)
                        max_spell = mspells.back();
                    action rep_spell = mult_spell(mspells.at(sp_pos), mul);
                    if (small_enough(rep_spell)) {
                        mspells.push_back(rep_spell);
                    }
                    else {
                        break;
                    }
                }
            }
            for (int item = 0; item < 4; item++) {
                items_on_spells.at(item) += max_spell.deltas.at(item);
                tot_it_in_sp += abs(max_spell.deltas.at(item));
            }
        }
        sort(mspells.begin(), mspells.end(), my_cmp);*/
        vector<int> minvs, oinvs;
        int minv_tot = 0, oinv_tot = 0;
        int inv;
        for (int inv_id = 0; inv_id < 4; inv_id++) {
            cin >> inv; minvs.push_back(inv); minv_tot += inv;
            if (PRINT_INPUT) { cerr << inv << " "; }
        }
        int mscore; cin >> mscore;
        if (PRINT_INPUT) { cerr << mscore << " "; }
        for (int inv_id = 0; inv_id < 4; inv_id++) {
            cin >> inv; oinvs.push_back(inv); oinv_tot += inv;
            if (PRINT_INPUT) { cerr << inv << " "; }
        }
        int oscore; cin >> oscore;
        if (PRINT_INPUT) { cerr << oscore << " "; }
        if (PRINT_INPUT) { cerr << " INPUT_END_HERE "; }

        if (mspells.size() == 0) { // FOR LOCAL
            return 0;
        }

        if (glob_step_num > 1) {
            vector<int> delta_minv = minvs - old_minv, delta_oinv = oinvs - old_oinv;
            for (auto& rec : old_recipes) {
                if (delta_minv == rec.deltas) {
                    glob_m_brewed++;
                }
                if (delta_oinv == rec.deltas) {
                    glob_o_brewed++;
                }
            }
        }
        old_minv = minvs; old_oinv = oinvs; old_recipes = recipes;
        if (DEBUG) { std::cerr << "m_brewed: " << glob_m_brewed << " glob_o_brewed: " << glob_o_brewed << "\n"; }
        glob_opp_turns_needed = 100;
        bfs(info_to_state({ recipes, learns, ospells, oinvs }), recipes, learns, ospells, 0, OPPONENT_SIMULATE_TURNS, OPPONENT_MAX_STATES_PER_D);
        //if (DEBUG) { cerr << " out of bfs "; }
        auto ENTMP = chrono::steady_clock::now(); int ELTIME = (int)(chrono::duration_cast<chrono::milliseconds>(ENTMP - STTMP).count());
        if (DEBUG) { cerr << "Time for opponent: " << ELTIME << " ms\n"; }
        int turns = calc_expected_turns() + ADDITIONAL_SIMULATE_TURNS;
        int states_per_d = TOTAL_STATES / turns;
        if (DEBUG) { std::cerr << "Simulating " << turns << " turns with " << states_per_d << " states each\n"; }
        bfs(info_to_state({ recipes, learns, mspells, minvs }), recipes, learns, mspells, 1, turns, states_per_d);
    }
}