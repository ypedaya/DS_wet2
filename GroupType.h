// GroupTypes.h

#pragma once

struct Group;
struct GroupMotivation;

struct Group {
    int groupId = 0;
    Skill total_skill();
    int experience = 0;
    int mission_had = 0;
    int total_motivation = 0;
    Skill Base_skill();
    GroupMotivation* motivation_tree = nullptr;
};

struct GroupMotivation {
    int total_motivation = 0;
    Group* id_tree = nullptr;
};
