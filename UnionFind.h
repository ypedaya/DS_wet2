//
// Created by yaara.p on 07/07/2026.
//

#ifndef UNIONFIND_H
#define UNIONFIND_H
#pragma once
#include <memory>

template<class GroupType, class ElementType>
class UnionFind {
public:
    struct GroupNode {
        std::shared_ptr<GroupType> data;
        GroupNode* parent = nullptr;
        int size = 0;
        GroupNode* next_allocated = nullptr;
    };

    struct ElementNode {
        ElementType* data;
        GroupNode* group;
        ElementNode* next_allocated = nullptr;
    };

private:
    //GroupNode* findRoot(GroupNode* group);
    void (*update_group_function)(GroupNode*, GroupNode*) = nullptr;

    void (*update_element_function)(ElementNode*, GroupNode*) = nullptr;
    GroupNode* allocated_groups = nullptr;
    ElementNode* allocated_elements = nullptr;

public:
    UnionFind(void (*group_function)(GroupNode*, GroupNode*) = nullptr, void (*element_function)(ElementNode*, GroupNode*) = nullptr) :
    update_group_function(group_function),
    update_element_function(element_function)
    {
    }

    UnionFind() {
        while (allocated_elements != nullptr) {
            ElementNode* to_delete = allocated_elements;
            allocated_elements = allocated_elements->next_allocated;
            delete to_delete;
        }

        while (allocated_groups != nullptr) {
            GroupNode* to_delete = allocated_groups;
            allocated_groups = allocated_groups->next_allocated;
            delete to_delete;
        }
    }

    GroupNode* makeSet(const std::shared_ptr<GroupType>& group_data) {
        GroupNode* node = new GroupNode();
        node->next_allocated = allocated_groups;
        allocated_groups = node;
        node->group = group_data;
        node->parent = nullptr;
        return node;
    }

    ElementNode* addElement(ElementType* element_data, GroupNode* group) {
        ElementNode* node = new ElementNode();
        node->next_allocated = allocated_elements;
        allocated_elements = node;
        node->element = element_data;
        node->group = group;
        group->size++;
        return node;
    }

    GroupNode* find(ElementNode* element) {
        if (element == nullptr || element->group == nullptr) {
            return nullptr;
        }
        GroupNode* original_group = element->group;
        GroupNode* current = original_group;
        GroupNode* previous = nullptr;

        while (current->parent != nullptr) {
            GroupNode* next = current->parent;
            current->parent = previous;
            previous = current;
            current = next;
        }

        GroupNode* root = current;
        current = previous;
        GroupNode* updated_parent = root;

        while (current != nullptr) {
            GroupNode* next = current->parent;
            if (update_group_function != nullptr) {
                update_group_function(current, updated_parent);
            }
            current->parent = root;
            updated_parent = current;
            current = next;
        }
        if (original_group != root && update_element_function != nullptr) {
            update_element_function(element, original_group);
        }
        element->group = root;
        return root;
    }

    GroupNode* union_groups(GroupNode* first, GroupNode* second) {
        if(first.size >= second->size) {
            second->parent = first;
            first->size += second->size;
            second->size = 0;
            return first;
        }
        first->parent = second;
        second->size += first->size;
        first->size = 0;
        return  second;
    }

    bool sameSet(ElementNode* first, ElementNode* second) {
        if (find(first) == find(second))
            return true;
        return false;
    }
};

#endif //UNIONFIND_H
