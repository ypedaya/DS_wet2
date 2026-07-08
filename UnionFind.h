//
// Created by yaara.p on 07/07/2026.
//

#ifndef UNIONFIND_H
#define UNIONFIND_H
#pragma once

template<class GroupType, class ElementType>
class UnionFind {
public:
    struct GroupNode {
        GroupType* group;
        GroupNode* parent = nullptr;
        int size = 0;
    };

    struct ElementNode {
        ElementType* element;
        GroupNode* group;
    };

private:
    //GroupNode* findRoot(GroupNode* group);


public:
    UnionFind() = default;
    ~UnionFind() = default;

    GroupNode* makeSet(GroupType* group_data) {
        GroupNode* node = new GroupNode();
        node->group = group_data;
        node->parent = nullptr;
        return node;
    }

    ElementNode* addElement(ElementType* element_data, GroupNode* group) {
        ElementNode* node = new ElementNode();
        node->element = element_data;
        node->group = group;
        group->size++;
        return node;
    }

    GroupNode* find(ElementNode* element) {
        while (element->group->parent != nullptr) {
            element->group = element->group->parent;
        }
        return element->group;
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
