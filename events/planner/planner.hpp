/**
 * @file planner.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 19.09.2019
 */

#pragma once

#include <vector>
#include <string>
#include <cerrno>

#include "globals.hpp"
#include "planner_phase.hpp"

using namespace std;

/**
 * @brief   Serve as support struture during handeling of complicated procedures with dependencies
 *          Planner constains of phases. Each phase has triggers and consequences.
 *              Triggers serves as starting condition. All triggers must be fullfilled Before another phase starts.
 *              Consequencies are function which will be starting at start fof phase.
 *          Special case is phase 0 and last phase.
 *              Zero phase has no triggers and last phase has no consequencies.
 *              Last phase is determinated as phase -1;
 *          Planner contains IRQ handler. IRQ is invocated after last phase is done.
 *          Planner cannot start when is no atleast one trigger and one consequence for every phase (excluding phase 0 and -1)
 */
class Planner
{
private:
    inline static unsigned int trigger_counter = 0;

    bool registred = false;

    bool running = false;

    string name = "Unknown_planner";

    vector<Planner_phase> phases;

public:
    /**
     * @brief   Construct a new Planner objectCreate new planner with given name
     *          Number of phases is counted without first and last phase
     *
     * @param name
     * @param phase_count
     */
    Planner(string name, int phase_count);
    ~Planner();

    /**
     * @brief Create new phase in planner
     *
     * @return int  Actual number of phases in planner
     */
    int Add_phase();

    const int Add_trigger(int phase_number);

    /**
     * @brief   Add new consequency to selected phase
     *          Last consequency cannot be selected
     *
     * @tparam class_T      Class of which is templated parametr of Invocation wrapper
     * @param phase         Number of phase
     * @param consequency   Invocation wrapper, which contains consequence method
     * @return int          Error number
     */
    template<typename class_T>
    int Add_consequency(int phase, Invocation_wrapper<class_T, void, void> consequency){
        if (phase = -1){
            return EINVAL;
        } else if (phase >= (phases.size() - 1)){
            return ENODEV;
        }
        phases[phase].Add_consequency(consequency);
        return 0;
    }

    /**
     * @brief   Change status of given trigger
     *
     * @param trigger_id    Number of trigger, which will be updated
     * @param state         New status of trigger
     * @return int          Number of triggers which are remaining to set on in phase of this trigger
     */
    int Trigger(int trigger_id, bool state);

    /**
     * @brief   Start controlling triggers
     *          Without starting of planner triggers will have no effect
     */
    void Start();

    /**
     * @brief   Disable planner, after this method triggers have no affect
     */
    void Stop();

    /**
     * @brief This method is called when last phase is done
     */
    void Done();

    /**
     * @brief Return name of planner, name of planner should be unique
     *
     * @return string   Name of planner
     */
    string Name(){return name;};

    /**
     * @brief Return actual count of phases
     *
     * @return uint cCount of phases
     */
    unsigned int Phases(){return phases.size();};

    /**
     * @brief   Perform registration under device,
     *          Registred planner is accessible from global scope
     *
     * @return int  Error code
     */
    int Register();

    /**
     * @brief   Remove this planner from registred planers
     *
     * @return int  Error code
     */
    int Unregister();

};
