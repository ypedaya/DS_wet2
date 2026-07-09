#include "contestantsArray.h"

void contestantsArray::rehash()
{
}

int contestantsArray::hashFunc(int id) const
{
}

contestantsArray::contestantsArray() : currentSize(0), currentCapacity(16), loadFactor(0.75), array(new Node*[currentCapacity]())
{
}

contestantsArray::~contestantsArray()
{
    if (array == nullptr)
    {
        return;
    }
    for (int i = 0; i < currentCapacity; i++)
    {
        if (array[i] != nullptr)
        {
            Node* current = array[i];
            while (current != nullptr)
            {
                Node* nextNode = current->next;
                delete current;
                current = nextNode;
            }
        }
        delete[] array;
    }
}

StatusType contestantsArray::insert(int id, int teamId, const Skill& skill, int motivation, int missionHad)
{
    if (id <= 0 || skill.isValid() == false || motivation < 0 || missionHad < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if (find(id) != nullptr)
    {
        return StatusType::FAILURE;
    }
    Contestant* to_insert;
    try
    {
        to_insert = new Contestant();
        to_insert->id = id;
        to_insert->motivation = motivation;
        to_insert->skill = *skill;
        to_insert->mission_dif = missionHad;
        to_insert->skill_diff = *skill;
        arr[id] = to_insert;
    }
    catch (std::exception& e)
    {
        delete to_insert;
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

bool contestantsArray::contains(int id) const
{
    return !(find(id) == nullptr);
}

Contestant* contestantsArray::find(int id) const
{
    int hashed = hashFunc(id);
    Node* current = array[hashed];
    while (current != nullptr)
    {
        if (current->contestantID == id)
        {
            return &current->contestant;
        }
    }
    return nullptr;
}

void contestantsArray::remove(int id)
{
    arr.remove(id);
}

