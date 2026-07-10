//
// Created by yaara.p on 07/07/2026.
//


#include "TeamsTreeMotivation.h"

TeamsTreeMotivation::TeamsTreeMotivation()
    : teams_tree(updateSubSumNode)
{
}

TeamsTreeMotivation::~TeamsTreeMotivation()
{
    deleteValues(teams_tree.root);
}

void TeamsTreeMotivation::deleteValues(
    AVLtree<TeamMotivation*>::node* current)
{
    if (current == nullptr)
    {
        return;
    }

    deleteValues(current->leftSon);
    deleteValues(current->rightSon);

    delete current->value;
    current->value = nullptr;
}

StatusType TeamsTreeMotivation::insert(int total_motivation, int groupId)
{
    AVLtree<TeamMotivation*>::node* tree_node = teams_tree.find(total_motivation);
    if (tree_node != nullptr)
    {
        tree_node->value->num_app++;
        tree_node->value->teamsId.insert(groupId, groupId);
        AVLtree<TeamMotivation*>::node* current = tree_node;
        while (current != nullptr)
        {
            updateSubSumNode(current);
            current = current->parent;
        }
        return StatusType::SUCCESS;
    }
    TeamMotivation* to_insert = nullptr;
    try
    {
        to_insert = new TeamMotivation();
        to_insert->num_app = 1;
        to_insert->sum = 1;
        to_insert->teamsId.insert(groupId, groupId);
        teams_tree.insert(total_motivation, to_insert);
    }
    catch (const std::bad_alloc&)
    {
        delete to_insert;
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType TeamsTreeMotivation::remove(int total_motivation, int TeamId)
{
    if (TeamId <= 0 || total_motivation < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    AVLtree<TeamMotivation*>::node* node = teams_tree.find(total_motivation);
    if (node == nullptr)
    {
        return StatusType::FAILURE;
    }
    if (node->value->num_app == 1)
    {
        TeamMotivation* value_to_delete = node->value;
        teams_tree.remove(total_motivation);
        delete value_to_delete;
    }
    else
    {
        node->value->num_app--;
        node->value->teamsId.remove(TeamId);
        AVLtree<TeamMotivation*>::node* current = node;
        while (current != nullptr)
        {
            updateSubSumNode(current);
            current = current->parent;
        }
    }
    return StatusType::SUCCESS;
}

void TeamsTreeMotivation::updateSubSumNode(AVLtree<TeamMotivation*>::node* current)
{
    if (current == nullptr)
    {
        return;
    }
    int left_sum = 0;
    int right_sum = 0;

    if (current->leftSon != nullptr)
    {
        left_sum = current->leftSon->value->sum;
    }

    if (current->rightSon != nullptr)
    {
        right_sum = current->rightSon->value->sum;
    }
    current->value->sum = current->value->num_app + left_sum + right_sum;
}

int TeamsTreeMotivation::findByIndex(int i)
{
    if (i <= 0 || teams_tree.root == nullptr)
    {
        return -1;
    }
    if (i > teams_tree.root->value->sum)
    {
        return -1;
    }
    AVLtree<TeamMotivation*>::node* current = teams_tree.root;
    while (current != nullptr)
    {
        int left_sum = 0;
        if (current->leftSon != nullptr)
        {
            left_sum = current->leftSon->value->sum;
        }
        if (i <= left_sum)
        {
            current = current->leftSon;
        }
        else if (i <= left_sum + current->value->num_app)
        {
            int rank = i - left_sum;
            AVLtree<int>::node* n = current->value->teamsId.find_min();
            for (int j = 1; j < rank; j++)
                n = current->value->teamsId.nextBiggerNode(n);
            return n->key;
        }
        else
        {
            i -= left_sum + current->value->num_app;
            current = current->rightSon;
        }
    }
    return -1;
}
