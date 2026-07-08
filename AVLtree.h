#include "wet2util.h"
#pragma once
template<class T>
class AVLtree {

    friend class GroupsTreeId;
    friend class GroupsTreeMotivation;

private:
    struct node {
        int key = 0;
        int height = 0;

        T value;
        node* leftSon = nullptr;
        node* rightSon = nullptr;
        node* parent = nullptr;
    };
    void (*update_function)(node*) = nullptr;
    int num_node = 0; //total nodes
    node* root = nullptr;

    void updateExtraData(node* current)
    {
        if (current != nullptr && update_function != nullptr) {
            update_function(current);
        }
    }

public:
    explicit AVLtree(void (*func)(node*) = nullptr)
    : update_function(func)
    {
    }
    ~AVLtree() {
        this->clearTree(root);
    }

    AVLtree(const AVLtree& t): num_node(t.num_node), update_function(t.update_function)
    {
        this->root = copyNodes(t.root, nullptr);
    }

    AVLtree &operator=(const AVLtree &t) {
        if (this == &t) {
            return *this;
        }
        node* newRoot = copyNodes(t.root, nullptr);
        clearTree(this->root);
        this->root = newRoot;
        this->num_node = t.num_node;
        this->update_function = t.update_function;
        return *this;
    }

    void insert(int key, T value) {
        if (contains(key)) {
            return;
        }
        node* newNode = new node();
        newNode->key = key;
        newNode->value = value;
        if (root == nullptr) {
            root = newNode;
            num_node++;
            updateExtraData(root);
            return;
        }
        node* current = root;
        bool inserted = false;
        while (!inserted) {
            if (current->key < key) {
                if (current->rightSon == nullptr) {
                    current->rightSon = newNode;
                    newNode->parent = current;
                    inserted = true;
                    continue;
                }
                current = current->rightSon;
                continue;
            }
            if (current->key > key) {
                if (current->leftSon == nullptr) {
                    current->leftSon = newNode;
                    newNode->parent = current;
                    inserted = true;
                    continue;
                }
                current = current->leftSon;
            }
        }
        num_node++;
        updateExtraData(newNode);
        node* temp = newNode->parent;
        while (temp != nullptr) {
            int leftHeight = getHeight(temp->leftSon);
            int rightHeight = getHeight(temp->rightSon);
            if (leftHeight >= rightHeight) {
                temp->height = 1 + leftHeight;
            } else {
                temp->height = 1 + rightHeight;
            }
            updateExtraData(temp);
            int bf = getBF(temp);
            if (bf > 1 || bf < -1) {
                if (bf > 1) {
                    if (getBF(temp->leftSon) >= 0) {
                        temp = rotateLL(temp);
                    } else {
                        temp = rotateLR(temp);
                    }
                } else {
                    if (getBF(temp->rightSon) <= 0) {
                        temp = rotateRR(temp);
                    } else {
                        temp = rotateRL(temp);
                    }
                }
            }
            temp = temp->parent;
        }
    }

    void remove(int key) {
        node* toRemove = root;
        while (toRemove != nullptr && toRemove->key != key) {
            if (key < toRemove->key) {
                toRemove = toRemove->leftSon;
            } else {
                toRemove = toRemove->rightSon;
            }
        }
        if (toRemove == nullptr) return;
        if (toRemove->leftSon != nullptr && toRemove->rightSon != nullptr) {
            node* successor = toRemove->rightSon;
            while (successor->leftSon != nullptr) {
                successor = successor->leftSon;
            }
            toRemove->key = successor->key;
            toRemove->value = successor->value;
            toRemove = successor;
        }

        node* temp = toRemove->parent;
        node* child;
        if (toRemove->leftSon != nullptr) {
            child = toRemove->leftSon;
        } else {
            child = toRemove->rightSon;
        }
        if (temp == nullptr) {
            root = child;
        } else {
            if (temp->leftSon == toRemove) {
                temp->leftSon = child;
            } else {
                temp->rightSon = child;
            }
        }
        if (child != nullptr) {
            child->parent = toRemove->parent;
        }
        delete toRemove;
        num_node--;

        while (temp != nullptr) {
            int leftHeight = getHeight(temp->leftSon);
            int rightHeight = getHeight(temp->rightSon);
            if (leftHeight >= rightHeight) {
                temp->height = 1 + leftHeight;
            } else {
                temp->height = 1 + rightHeight;
            }
            int bf = getBF(temp);
            if (bf > 1 || bf < -1) {
                if (bf > 1) {
                    if (getBF(temp->leftSon) >= 0) {
                        temp = rotateLL(temp);
                    } else {
                        temp = rotateLR(temp);
                    }
                } else {
                    if (getBF(temp->rightSon) <= 0) {
                        temp = rotateRR(temp);
                    } else {
                        temp = rotateRL(temp);
                    }
                }
            }
            temp = temp->parent;
        }
    }

    node* find(int key) const {
        if (root == nullptr) {
            return nullptr;
        }
        node* current = root;
        while (current) {
            if (current->key == key) {
                return current;
            }
            if (current->key > key) {
                current = current->leftSon;
            } else {
                current = current->rightSon;
            }
        }
        return nullptr;
    }

    bool contains(int key) const {
        return find(key) != nullptr;
    }

    int getNumNodes() const {
        return num_node;
    }

    static AVLtree<T>* mergeTrees(AVLtree<T>* tree1, AVLtree<T>* tree2) {
        int size1 = tree1->num_node;
        int size2 = tree2->num_node;
        node** tree1_arr = nullptr;
        node** tree2_arr = nullptr;
        node** newTree_arr = nullptr;
        AVLtree<T>* newTree = nullptr;
        try {
            newTree = new AVLtree<T>();
            if (size2 == 0 && size1 == 0) {
                return newTree;
            }
            if (size2 == 0) {
                newTree->root = tree1->root;
                newTree->num_node = tree1->num_node;
                return newTree;
            }
            if (size1 == 0) {
                newTree->root = tree2->root;
                newTree->num_node = tree2->num_node;

                return newTree;
            }
            tree1_arr = new node*[size1];
            tree1->inOrderToArray(tree1_arr);
            tree2_arr = new node*[size2];
            tree2->inOrderToArray(tree2_arr);
            newTree_arr = new node*[size1 + size2];
            merge_sort(tree1_arr, size1, tree2_arr, size2, newTree_arr);
            newTree->root = fill_from_arr(newTree_arr, 0, size1 + size2 - 1, newTree->root);
            newTree->num_node = size1 + size2;
        } catch (const std::exception &e) {
            delete[] tree1_arr;
            delete[] tree2_arr;
            delete[] newTree_arr;
            delete newTree;
            throw;
        }
        delete[] tree1_arr;
        delete[] tree2_arr;
        delete[] newTree_arr;
        return newTree;
    }

    void takeOwnership(AVLtree<T>* otherTree) {
        if (otherTree == nullptr || otherTree == this) {
            return;
        }
        this->clearTree(this->root);
        this->root = otherTree->root;
        this->num_node = otherTree->num_node;
        otherTree->root = nullptr;
        otherTree->num_node = 0;
    }

    node* nextBiggerNode(node* current) {
        if (current == nullptr) {
            return nullptr;
        }
        if (current->rightSon != nullptr) {
            node* temp = current->rightSon;
            while (temp->leftSon != nullptr) {
                temp = temp->leftSon;
            }
            return temp;
        }
        node* parent = current->parent;
        node* temp = current;
        while (parent != nullptr && temp == parent->rightSon) {
            temp = parent;
            parent = parent->parent;
        }
        return parent;
    }

private:
    void clearTree(node* v) {
        if (v == nullptr) {
            return;
        }
        clearTree(v->leftSon);
        clearTree(v->rightSon);
        delete v;
    }

    node* rotateRL(node* v) {
        v->rightSon = rotateLL(v->rightSon);
        return rotateRR(v);
    }

    node* rotateRR(node* v) {
        node* A = v->rightSon;
        A->parent = v->parent;
        if (v->parent != nullptr) {
            if (v->parent->leftSon == v) {
                v->parent->leftSon = A;
            } else {
                v->parent->rightSon = A;
            }
        } else {
            root = A;
        }
        v->parent = A;
        v->rightSon = A->leftSon;
        if (A->leftSon != nullptr) {
            A->leftSon->parent = v;
        }
        A->leftSon = v;
        int vLeftHeight = getHeight(v->leftSon);
        int vRightHeight = getHeight(v->rightSon);
        if (vLeftHeight >= vRightHeight) {
            v->height = 1 + vLeftHeight;
        } else {
            v->height = 1 + vRightHeight;
        }
        int aLeftHeight = getHeight(A->leftSon);
        int aRightHeight = getHeight(A->rightSon);
        if (aLeftHeight >= aRightHeight) {
            A->height = 1 + aLeftHeight;
        } else {
            A->height = 1 + aRightHeight;
        }
        updateExtraData(v);
        updateExtraData(A);
        return A;
    }

    node* rotateLR(node* v) {
        //YAARA
        v->leftSon = rotateRR(v->leftSon);
        return rotateLL(v);
    }

    node* rotateLL(node* v) {
        //YAARA
        node* original_parent = v->parent;
        node* c = v->leftSon->rightSon;
        node* b = v->leftSon;
        b->rightSon = v;
        v->leftSon = c;
        if (c != nullptr)
            c->parent = v;
        v->parent = b;
        b->parent = original_parent;
        if (original_parent != nullptr) {
            if (original_parent->leftSon == v) {
                original_parent->leftSon = b;
            } else {
                original_parent->rightSon = b;
            }
        } else {
            root = b;
        }
        int v_left_height;
        if (c != nullptr) {
            v_left_height = c->height;
        } else {
            v_left_height = -1;
        }
        int v_right_height;
        if (v->rightSon != nullptr) {
            v_right_height = v->rightSon->height;
        } else {
            v_right_height = -1;
        }
        if (v_left_height > v_right_height) {
            v->height = 1 + v_left_height;
        } else {
            v->height = 1 + v_right_height;
        }

        int b_left_height;
        if (b->leftSon != nullptr) {
            b_left_height = b->leftSon->height;
        } else {
            b_left_height = -1;
        }

        if (b_left_height > v->height) {
            b->height = 1 + b_left_height;
        } else {
            b->height = 1 + v->height;
        }
        updateExtraData(v);
        updateExtraData(b);
        return b;
    }

    static node* fill_from_arr(node** newTree_arr, int start, int end, node* parent) {
        if (start > end) {
            return nullptr;
        }
        int mid = start + (end - start) / 2;
        node* Root = newTree_arr[mid];
        Root->parent = parent;
        Root->leftSon = fill_from_arr(newTree_arr, start, mid - 1, Root);
        Root->rightSon = fill_from_arr(newTree_arr, mid + 1, end, Root);
        int left_height;
        if (Root->leftSon != nullptr) {
            left_height = Root->leftSon->height;
        } else {
            left_height = -1;
        }
        int right_height;
        if (Root->rightSon != nullptr) {
            right_height = Root->rightSon->height;
        } else {
            right_height = -1;
        }
        int max_height;
        if (left_height > right_height) {
            max_height = left_height;
        } else {
            max_height = right_height;
        }
        Root->height = 1 + max_height;

        return Root;
    }

    static void merge_sort(node** arr1, int size1, node** arr2, int size2, node** dest) {
        int index1 = 0;
        int index2 = 0;
        int indexDest = 0;
        while (size1 != 0 && size2 != 0) {
            if (arr1[index1]->key < arr2[index2]->key) {
                dest[indexDest] = arr1[index1];
                size1--;
                index1++;
            } else {
                dest[indexDest] = arr2[index2];
                size2--;
                index2++;
            }
            indexDest++;
        }
        while (size1 != 0) {
            dest[indexDest] = arr1[index1];
            size1--;
            index1++;
            indexDest++;
        }
        while (size2 != 0) {
            dest[indexDest] = arr2[index2];
            size2--;
            index2++;
            indexDest++;
        }
    }

    void inOrderToArray(node** arr) {
        if (this->root == nullptr)
            return;
        node* current = this->root;
        node* last = nullptr;
        int arr_index = 0;
        while (current != nullptr) {
            if (last == nullptr || last == current->parent) {
                //on our way down
                if (current->leftSon != nullptr) {
                    //go left
                    last = current;
                    current = current->leftSon;
                } else {
                    // no more left
                    arr[arr_index] = current;
                    arr_index++;
                    if (current->rightSon != nullptr) {
                        //go right
                        last = current;
                        current = current->rightSon;
                    } else {
                        //finished, go up
                        last = current;
                        current = current->parent;
                    }
                }
            } else if (current->leftSon == last) {
                //on our way up from the left
                arr[arr_index] = current;
                arr_index++;
                if (current->rightSon != nullptr) {
                    //go right
                    last = current;
                    current = current->rightSon;
                } else {
                    //finished, go up
                    last = current;
                    current = current->parent;
                }
            } else if (current->rightSon == last) {
                //on our way up from the right
                last = current;
                current = current->parent;
            }
        }
    }

    int getHeight(node* v) const {
        if (v == nullptr) {
            return -1;
        }
        return v->height;
    }

    int getBF(node* v) const {
        if (v == nullptr) {
            return 0;
        }
        return getHeight(v->leftSon) - getHeight(v->rightSon);
    }

    node* copyNodes(node* source, node* parent) {
        if (source == nullptr) {
            return nullptr;
        }
        node* newNode = new node();
        newNode->key = source->key;
        newNode->value = source->value;
        newNode->height = source->height;
        newNode->parent = parent;
        newNode->leftSon = copyNodes(source->leftSon, newNode);
        newNode->rightSon = copyNodes(source->rightSon, newNode);
        return newNode;
    }
};
