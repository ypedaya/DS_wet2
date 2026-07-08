//
// Created by yaara.p on 07/07/2026.
//

#include "contestantsArray.h"
contestantsArray::~contestantsArray()
{
    for (int i = 0; i < arr.size(); i++) {
        delete arr[i];
    }
}

StatusType contestantsArray::insert(int id, int teamId, const Skill &skill, int motivation, int missionHad) {

    if(id <= 0 ||  skill.isValid() == false || motivation < 0 || missionHad < 0) {
        return  StatusType::INVALID_INPUT;
    }
    if(arr[id] != nullptr) {
        return  StatusType::FAILURE;
    }
    contestant* to_insert;
    try {
        to_insert = new contestant();
        to_insert->id = id;
        to_insert->motivation = motivation;
        to_insert->skill = *skill;
        to_insert->mission_dif = missionHad;
        to_insert->skill_diff = *skill;
        arr[id] =  to_insert;
    }
    catch (std::exception &e) {
        delete to_insert;
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}
contestant* contestantsArray::find(int id) {
    return arr[id];
}
void contestantsArray::remove(int id) {
    arr.remove(id);
}

