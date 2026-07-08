//
// Created by yaara.p on 07/07/2026.
//

#include "AVLtree.h"
#include "UnionFind.h"
#include "wet2util.h"
#include "StructsDef.h"
#pragma once

class GroupsTreeMotivation {
    friend class GroupsTreeId;

    AVLtree<GroupMotivation*> groups_tree;

public:
    GroupsTreeMotivation();

    ~GroupsTreeMotivation();

    GroupsTreeMotivation(const GroupsTreeMotivation &) = delete;

    GroupsTreeMotivation &operator=(const GroupsTreeMotivation &) = delete;

    StatusType insert(int total_motivation, int groupId);

    StatusType remove(int total_motivation, int GroupId);

    AVLtree<GroupMotivation*>::node* find(int total_motivation) const;

    static void updateSubSumNode(AVLtree<GroupMotivation*>::node* current);

};



