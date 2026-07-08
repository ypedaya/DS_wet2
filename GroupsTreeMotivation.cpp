//
// Created by yaara.p on 07/07/2026.
//


#include "GroupsTreeMotivation.h"

GroupsTreeMotivation::GroupsTreeMotivation()
    : groups_tree(updateSubSumNode)
{
}

GroupsTreeMotivation::~GroupsTreeMotivation() {

}

StatusType GroupsTreeMotivation::insert(int total_motivation, int groupId)
{
    AVLtree<GroupMotivation*>::node* tree_node = groups_tree.find(total_motivation);

    if (tree_node != nullptr) {
        tree_node->value->num_app++;
        tree_node->value->GroupsId.insert(groupId, groupId);
        AVLtree<GroupMotivation*>::node* current = tree_node;
        while (current != nullptr) {
            updateSubSumNode(current);
            current = current->parent;
        }
        return StatusType::SUCCESS;
    }
    GroupMotivation* to_insert = nullptr;
    try {
        to_insert = new GroupMotivation();
        to_insert->num_app = 1;
        to_insert->sum = 1;
        groups_tree.insert(total_motivation, to_insert);
    }
    catch (const std::bad_alloc&) {
        delete to_insert;
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType GroupsTreeMotivation::remove(int total_motivation, int GroupId) {

}

AVLtree<GroupMotivation*>::node* GroupsTreeMotivation::find(int total_motivation) const {

}

void GroupsTreeMotivation::updateSubSumNode(AVLtree<GroupMotivation*>::node* current)
{
    if (current == nullptr) {
        return;
    }
    int left_sum = 0;
    int right_sum = 0;

    if (current->leftSon != nullptr) {
        left_sum = current->leftSon->value->sum;
    }

    if (current->rightSon != nullptr) {
        right_sum = current->rightSon->value->sum;
    }
    current->value->sum = current->value->num_app+ left_sum + right_sum;
}