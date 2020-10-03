// list ADT 的链表实现
// 链表实现中，Position 为 Node*，List 为 Node*，链表数据元素为 int
// 链表实现使用表头语义，空表表头既满足 IsEmpty 也满足 IsLast

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "linked_list.h"


// 链表实现的额外操作；查找 pos 节点在 li 中的位置
intmax_t GetIndex(Position pos, List li) {
    intmax_t index = -1;
    Position ptr = li;
    while (li->next) {
        ptr = li->next;
        index++;
        if (ptr == pos)
            return index;
    }
    return -1;
}

// 使用表头语义
List MakeEmpty() {
    List header = (List) malloc(sizeof(struct Node));
    assert(header != NULL);
    header->next = NULL;
    return header;
}

int IsEmpty(List li) {
    return li->next == NULL;
}

int IsLast(Position pos, List li) {
    return pos->next == NULL;
}

ElementType Get(Position pos, List li) {
    return pos->elem;
}

Position Advance(Position curr, List li) {
    Position prev = li, pos = li->next;
    while(pos) {
        if (pos == curr)
            return prev;
        prev = pos;
        pos = pos->next;
    }
    return NULL;
}

// 返回值为 NULL 表示没有找到
Position Find(ElementType e, List li) {
    Position pos = li;
    while (pos->next) {
        pos = pos->next;
        if (pos->elem == e)
            return pos;
    }
    return NULL;
}

Position FindPrevious(ElementType e, List li) {
    Position prev = li, pos = li->next;
    while(pos) {
        if (pos->elem == e)
            return prev;
        prev = pos;
        pos = pos->next;
    }
    return NULL;
}

void Delete(Position pos, List li) {
    if (pos == NULL)
        return;
    Position prev = Advance(pos, li);
    if (prev) {
        prev->next = pos->next;
        free(pos);
    }
}

void DeleteElem(ElementType e, List li) {
    Position prev = FindPrevious(e, li);
    if (prev) {
        Position pos = prev->next; // pos 不为 NULL
        prev->next = pos->next;
        free(pos);
    }
}

// 注意，Insert 语义对于链表而言是在 pos 后面插入
void Insert(Position pos, ElementType e, List li) {
    if (!pos)
        return;
    Position newNode = (Position)malloc(sizeof(struct Node));
    assert(newNode != NULL);
    newNode->elem = e;
    newNode->next = pos->next;
    pos->next = newNode;
}

void DeleteList(List li) {
    Position curr = li, successor = curr->next;
    while (successor) { // successor 不为 NULL
        free(curr);
        curr = successor;
        successor = successor->next;
    }
    free(curr);
}

Position Header(List li) {
    return li;
}

Position First(List li) {
    return li->next;
}

Position Last(List li) {
    PtrToNode ptr = li;
    for (; ptr->next; ptr = ptr->next);
    return ptr;
}
