// GroupTypes.h

#pragma once
#include "AVLtree.h"
#include "UnionFind.h"

struct Team;
struct TeamMotivation;
struct contestant;

struct Team {
    int groupId = 0;
    Skill total_skill;
    Skill Base_skill;
    int mission_diff;
    int experience = 0;
    int mission_had = 0;
    int total_motivation = 0;
    UnionFind<Team, contestant>::GroupNode* union_node = nullptr;
};

struct TeamMotivation {
    int num_app = 0;
    int sum = 0;
    AVLtree<int> teamsId;
};

struct contestant {
    Skill skill;
    int motivation = 0;
    int id;
    Skill skill_diff;
    int mission_dif;
    UnionFind<Team, contestant>::ElementNode* union_node = nullptr;
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





