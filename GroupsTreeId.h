
#include "AVLtree.h"
#include "UnionFind.h"
#include "wet2util.h"
#pragma once
#include "GroupType.h"

class GroupsTreeId {
    friend class GroupsTreeMotivation;

    AVLtree<Group*>* groups_tree;

public:
    GroupsTreeId() = default;

    ~GroupsTreeId();

    GroupsTreeId(const GroupsTreeId &) = delete;

    GroupsTreeId &operator=(const GroupsTreeId &) = delete;

    void insert(int groupId);

    void remove(int groupId);

    bool contains(int groupId) const;

    AVLtree<Group*>::node* find(int groupId) const;


};



