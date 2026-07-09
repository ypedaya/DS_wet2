#include "wet2util.h"
#include "StructsDef.h"

class contestantsArray
{
    struct Node
    {
        int contestantID;
        Contestant contestant;
        Node* next;

        Node(int id, const Contestant& c, Node* node = nullptr) : contestantID(id), contestant(c), next(node)
        {
        }
    };

    int currentSize; //num of keys
    int currentCapacity; //size of array
    float loadFactor;
    Node** array;

    void rehash();
    int hashFunc(int id) const;

public:
    contestantsArray();
    ~contestantsArray();
    contestantsArray(const contestantsArray&) = delete;
    contestantsArray& operator=(const contestantsArray&) = delete;
    StatusType insert(int id, int teamId, const Skill& skill, int motivation, int missionHad);
    bool contains(int id) const;
    Contestant* find(int id) const;
    void remove(int id); // no contestant should be removed
};




