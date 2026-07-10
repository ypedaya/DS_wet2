//
// Created by yaara.p on 07/07/2026.
//

#include "TeamsTreeId.h"

#include "TeamsTreeMotivation.h"


StatusType TeamsTreeId::insert(int groupId)
{
    if (groupId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if (groups_tree.find(groupId) != nullptr)
    {
        return StatusType::FAILURE;
    }
    try
    {
        std::shared_ptr<Team> node = std::make_shared<Team>();
        node->groupId = groupId;
        groups_tree.insert(groupId, node);
    }
    catch (std::exception& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}


Team* TeamsTreeId::find(int groupId) const
{
    AVLtree<std::shared_ptr<Team>>::node* tree_node = groups_tree.find(groupId);
    if (tree_node == nullptr)
    {
        return nullptr;
    }
    return tree_node->value.get();
}

std::shared_ptr<Team> TeamsTreeId::findShared(int groupId) const
{
    AVLtree<std::shared_ptr<Team>>::node* tree_node = groups_tree.find(groupId);
    if (tree_node == nullptr)
    {
        return nullptr;
    }
    return tree_node->value;
}

StatusType TeamsTreeId::remove(int teamId)
{
    if (teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    if (groups_tree.find(teamId) == nullptr)
    {
        return StatusType::FAILURE;
    }

    groups_tree.remove(teamId);

    return StatusType::SUCCESS;
}
