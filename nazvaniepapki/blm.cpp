#include <iostream>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

int getCycleLength(ListNode* meetingPoint) {
    int length = 1;
    ListNode* current = meetingPoint->next;
    while (current != meetingPoint) {
        ++length;
        current = current->next;
    }
    return length;
}

ListNode* findCycleStart(ListNode* head, ListNode* meetingPoint) {
    ListNode* ptr1 = head;
    ListNode* ptr2 = meetingPoint;
    while (ptr1 != ptr2) {
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }
    return ptr1;
}
void analyzeCycle(ListNode* head) {
    if (head == nullptr) {
        std::cout << "Список пуст." << std::endl;
        return;
    }

    ListNode* slow = head;
    ListNode* fast = head;
    ListNode* meetingPoint = nullptr;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            meetingPoint = slow;
            break;
        }
    }
    if (meetingPoint == nullptr) {
        std::cout << "Цикл отсутствует." << std::endl;
        return;
    }

    int cycleLength = getCycleLength(meetingPoint);
    ListNode* cycleStart = findCycleStart(head, meetingPoint);

    std::cout << "  Длина цикла: " << cycleLength << std::endl;
    std::cout << "  Начало цикла: элемент со значением " << cycleStart->val << std::endl;
}
int main() {
    ListNode* n1 = new ListNode(1);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(3);
    ListNode* n4 = new ListNode(4);
    ListNode* n5 = new ListNode(5);

    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;
    n5->next = n3;

    std::cout << "Тест 1:" << std::endl;
    analyzeCycle(n1);

    std::cout << std::endl;

    ListNode* a = new ListNode(10);
    ListNode* b = new ListNode(20);
    ListNode* c = new ListNode(30);
    a->next = b;
    b->next = c;

    std::cout << "Тест 2:" << std::endl;
    analyzeCycle(a);

    return 0;
}
