/**
 * @file planner_phase.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 19.09.2019
 */

#pragma once

#include <vector>
#include <algorithm>
#include <cerrno>
#include <map>

#include "misc/invocation_wrapper.hpp"

using namespace std;

class Planner_phase
{
private:
    int phase_number;

    map<int, bool> triggers;

    vector<Invocation_wrapper_base<void, void> *> consequences;

public:
    Planner_phase() = default;
    Planner_phase(int phase_number);
    ~Planner_phase() = default;

    /**
     * @brief Add new trigger for this phase, new trigger have value set to false;
     *
     * @param trigger_id    Number of trigger (trigger_id)
     * @return int          Error_code
     */
    int Add_trigger(int trigger_id);

    /**
     * @brief Set selected trigger to given state
     *
     * @param trigger_id    Number of trigger (trigger_id)
     * @param state         New state for trigger
     */
    void Trigger(int trigger_id, bool state);

    template <typename class_T>
    int Add_consequency(Invocation_wrapper<class_T, void, void> *consequency){
        consequences.emplace_back(consequency);
        return consequences.size();
    }
};
