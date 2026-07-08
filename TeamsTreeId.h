
#include "AVLtree.h"
#include "UnionFind.h"
#include "wet2util.h"
#pragma once
#include <memory>

#include "StructsDef.h"

class TeamsTreeId {
    friend class TeamsTreeMotivation;
    AVLtree<std::shared_ptr<Team>> groups_tree;

public:
    TeamsTreeId() = default;

    ~TeamsTreeId() = default;

    TeamsTreeId(const TeamsTreeId &) = delete;

    TeamsTreeId &operator=(const TeamsTreeId &) = delete;

    StatusType insert(int groupId);

    std::shared_ptr<Team> findShared(int groupId) const;

    Team* find(int groupId) const;

    StatusType remove(int teamId);

};



