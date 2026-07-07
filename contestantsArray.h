
#include "DynamicArray.h"
#include "wet2util.h"


class contestantsArray {

    struct contestant {
        Skill skill;
        int motivation = 0;
        int id;
    };

    DynamicArray<contestant*>;

public:
    contestantsArray() = default;

    ~contestantsArray();

    contestantsArray(const contestantsArray &) = delete;

    contestantsArray &operator=(const contestantsArray &) = delete;

    void insert(int id);

    bool contains(int id) const;



};




