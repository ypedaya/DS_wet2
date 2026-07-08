// GroupTypes.h

#pragma once
#include "UnionFind.h"

struct Group;
struct GroupMotivation;
struct contestant;

struct Group {
    int groupId = 0;
    Skill total_skill();
    Skill Base_skill();
    int experience = 0;
    int mission_had = 0;
    int total_motivation = 0;
    UnionFind<Group, contestant>::GroupNode* union_node = nullptr;
};

struct GroupMotivation {
    int num_app = 0;
    int sum = 0;
    AVLtree<int> GroupsId;
};

struct contestant {
    Skill skill;
    int motivation = 0;
    int id;
    Skill skill_diff;
    int mission_dif;
    UnionFind<Group, contestant>::ElementNode* union_node = nullptr;
};


/*struct return_Group {
    StatusType status;
    Group* group;
};

struct return_Contestant {
    StatusType status;
    contestant* contestant;
};
*/





