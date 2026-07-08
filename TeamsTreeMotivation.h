//
// Created by yaara.p on 07/07/2026.
//

#include "AVLtree.h"
#include "UnionFind.h"
#include "wet2util.h"
#include "StructsDef.h"
#pragma once

class TeamsTreeMotivation {
    friend class TeamsTreeId;

    AVLtree<TeamMotivation*> teams_tree;
    void deleteValues(AVLtree<TeamMotivation*>::node* current);

public:
    TeamsTreeMotivation();

    ~TeamsTreeMotivation();

    TeamsTreeMotivation(const TeamsTreeMotivation &) = delete;

    TeamsTreeMotivation &operator=(const TeamsTreeMotivation &) = delete;

    StatusType insert(int total_motivation, int groupId);

    StatusType remove(int total_motivation, int TeamId);

    static void updateSubSumNode(AVLtree<TeamMotivation*>::node* current);
    int findByIndex(int i);

};



