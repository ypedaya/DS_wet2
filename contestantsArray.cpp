#include "contestantsArray.h"

void contestantsArray::rehash()
{
    int oldCapacity = currentCapacity;
    int oldSize = currentSize;
    Node** oldArray = array;

    currentCapacity *= 2;
    currentSize = 0;

    try
    {
        array = new Node*[currentCapacity];

        for (int i = 0; i < oldCapacity; ++i)
        {
            Node* current = oldArray[i];

            while (current != nullptr)
            {
                Node* nextNode = current->next;
                int newIndex = hashFunc(current->contestantID);
                current->next = array[newIndex];
                array[newIndex] = current;
                currentSize++;
                current = nextNode;
            }
        }
        delete[] oldArray;
    }
    catch (std::exception& e)
    {
        array = oldArray;
        currentCapacity = oldCapacity;
        currentSize = oldSize;
    }
}

int contestantsArray::hashFunc(int id) const
{
    return id % currentCapacity;
}

contestantsArray::contestantsArray() : currentSize(0), currentCapacity(10), loadFactor(0.75), array(new Node*[currentCapacity]())
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
    Contestant to_insert;
    try
    {
        to_insert.id = id;
        to_insert.motivation = motivation;
        to_insert.skill = skill;
        to_insert.mission_dif = missionHad;
        to_insert.skill_diff = skill;
        int hashedId = hashFunc(id);
        Node* toInsert = new Node(id, to_insert, array[hashedId]);
        array[hashedId] = toInsert;
        currentSize++;

        if (getSize() > loadFactor * getCapacity())
        {
            rehash();
        }
    }
    catch (std::exception& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

bool contestantsArray::contains(int id) const
{
    return find(id) != nullptr;
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
    int hashedId = hashFunc(id);
    Node* current = array[hashedId];
    Node* prev = nullptr;
    while (current != nullptr && id != current->contestantID)
    {
        prev = current;
        current = current->next;
    }
    if (current == nullptr)
    {
        return;
    }
    if (prev == nullptr)
    {
        array[hashedId] = current->next;
    }
    else
    {
        prev->next = current->next;
    }
    delete current;
    currentSize--;
}

int contestantsArray::getSize() const
{
    return this->currentSize;
}

int contestantsArray::getCapacity() const
{
    return this->currentCapacity;
}
