//
// Created by yaara.p on 07/07/2026.
//

#include "GroupsTreeId.h"

#include "GroupsTreeMotivation.h"

GroupsTreeId::~GroupsTreeId() {

}


StatusType GroupsTreeId::insert(int groupId) {
    if(groupId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if(groups_tree->find(groupId) != nullptr) {
        return StatusType::FAILURE;
    }
    Group* node = new Group();
    node->groupId = groupId;
    try {
        groups_tree->insert(groupId, node);
    }
    catch (std::exception &e) {
        delete node;
        return StatusType::ALLOCATION_ERROR;
    }


}

void GroupsTreeId::remove(int groupId) {

}

bool GroupsTreeId::contains(int groupId) const {

}

Group* GroupsTreeId::find(int groupId) const {
    return groups_tree->find(groupId)->value;
}
