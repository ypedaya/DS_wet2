
#include "DynamicArray.h"
#include "wet2util.h"
#include "StructsDef.h"

class contestantsArray {


    DynamicArray<contestant*> arr;

public:
    contestantsArray() = default;
    ~contestantsArray();
    contestantsArray(const contestantsArray &) = delete;
    contestantsArray &operator=(const contestantsArray &) = delete;
    StatusType insert(int id, int teamId, const Skill &skill, int motivation, int missionHad);
    bool contains(int id) const;
    contestant* find(int id);
    void remove(int id);
};




