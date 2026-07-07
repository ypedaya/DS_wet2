//
// Created by yaara.p on 07/07/2026.
//

#include "AVLtree.h"
#include "UnionFind.h"
#include "wet2util.h"
#include "GroupType.h"
#pragma once

class GroupsTreeMotivation {
    friend class GroupsTreeId;

    AVLtree<GroupMotivation*>* groups_tree;

public:
    GroupsTreeMotivation() = default;

    ~GroupsTreeMotivation();

    GroupsTreeMotivation(const GroupsTreeMotivation &) = delete;

    GroupsTreeMotivation &operator=(const GroupsTreeMotivation &) = delete;

    void insert(int total_motivation, Group* group);

    void remove(int total_motivation);

    AVLtree<GroupMotivation*>::node* find(int total_motivation) const;


};



