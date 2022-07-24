// Author@HamzaMateen
#include <iostream>
#include <string>
#include <algorithm>


// LINKED LIST GENERIC CLASS
template<class T>
class LinkedList
{
public: // ctor / dtor
    LinkedList();
    ~LinkedList();

    LinkedList operator = (LinkedList&) = delete;
    LinkedList(LinkedList&) = delete;

public: // new functions
    bool empty();

    void find(int);
    int  size();

public: // member functions
    void print();

    void push_front(T);
    void push_back(T);
    void push_at(int, T);

    void pop_front();
    void pop_back();
    void pop_at(int);

    void reverse();
    void clear();
    T read_at(int);


private:
    struct Node
    {
        T data;
        Node* next;

        explicit Node(T val)
        : data(val) {
            next = nullptr;
        }
    };

private: // globals
    Node* head ;
    Node* tail ;

    int len ;
};

// we will overload the functions actually for c out.
//template<typename T>
//std::ostream& LinkedList<T>::display(std::ostream &stream)  {
//    if (head == nullptr)
//    {
//        stream << "list is empty" << std::endl;
//        return stream;
//    }
//
//    Node* refToHead = head;
//    while(refToHead != nullptr)
//    {
//        stream << refToHead->data << " " ;
//        refToHead = refToHead->next;
//    }
//    stream << std::endl;
//
//    stream.flush();
//    return stream;
//}

/// C++ FUNCTION OVERLOADING
template<typename T>  /// @author
std::ostream &operator<<(std::ostream& os, const LinkedList<T> list) {
    return list.display(os);
}

// main.cpp driver function
struct Coordinate {
        int x, y;
    };

/*************************************************        MAIN       *************************************************/
int main()
{
    // any qualified-type can be used here
    LinkedList<int> list;

    // push ints
    for(int I = 0; I < 11; I++)
        list.push_at(I, I);

    // print ints
    list.print();

    // inserting at head, end, and n
    list.push_front(230);
    list.push_back(534);
    list.push_at(14, 5000);

    std::cout << list.size() << std::endl;

    // print again
    list.print();

    std::cout << list.size() << std::endl;

    list.pop_at(13);
    // list.pop_back();
    // list.pop_front();
    std::cout << list.size() << std::endl;
    // print once more
    list.print();

    list.find(6);


    // reverse the list
    std::cout << "reverse :";
    list.reverse();
    list.print();

    // clear the list
    list.clear();
    list.print();

    std::cout << "isEmpty : " << list.empty() << std::endl;
    std::cout << "size: " << list.size();
    return 0;
}
/*************************************************        END of MAIN       *******************************************/

// read the value of element at certain given index.
template<class Type> /// @author
Type LinkedList<Type>::read_at(int pos) {
   if (pos < 0 || pos > len -1)
       throw "[ERROR] In read_at(), index out of range.";

   Node* reference = head;
   for(int i = 0; i < pos; i++)
       reference = reference->next;

   return reference->data;
}


/******* DELETION FUNCTIONS ********/
/// 1. delete an element at certain index
template<class T>
void LinkedList<T>::pop_at(int pos)
{
    if(head == nullptr) return;
    if(pos < 0 || pos > len-1)
    {
       std::cout << "[ERROR] index " << pos << " out of range " << std::endl;
       return;
    }
     // only this 3)...
     int iter = 0;
     Node* ref = head;

     while(iter < pos - 1)
     {
         ref = ref->next;
         iter++;
     }

     Node* temp = ref->next;
     ref->next = temp->next;

     delete temp;
     len--;
}

/// 2. delete the last element
template<class T>
void LinkedList<T>::pop_back()
{
    // case 1: there are no elements in the list
   if (head == nullptr)
       return;

    // case 2: there is only on element in the list.
   if (head->next == nullptr)
   {
       Node* ref = head;
       head = tail = nullptr; // it is like saying => x = y = 0

       delete ref;

       len-- ;
   }
   else {
       // case 3: there are more than two elements in the list
       Node *ref = head;

       while (ref->next != tail)
           ref = ref->next;

       ref->next = nullptr;

       delete tail;
       len--;

       tail = ref;
   }
}

/// 3. delete the first element
template <class T>
void LinkedList<T>::pop_front()
{
    // Case 1: if list is empty, we cannot delete anything, we just return out of the function
    if (head == nullptr)
        return;

    // Case 2: if there is exactly one element in the list.
    else if (head->next = nullptr)
    {
       Node* ref = head;
       head = tail = nullptr;

       delete ref;

       len-- ;

        // tail will not be disturbed
    }
    else  {
        // case 3: there are more than 1 elements in the list
        Node* ref = head;
        head = head->next;

        delete ref;

        len-- ;
    }
}

/***** END OF DELETION FUNCTIONS *****/

/********* INSERTION FUNCTIONS ***********/
/// 1. Insert given value into the list at given index
template<class T>
void LinkedList<T>::push_at(int pos, T val)
{
    // if the index given is not valid
    if (pos < 0 || pos > len)
    {
       std::cout << "[ERROR] index " << pos << " out of range" << std::endl;
       return;
    }

    else if (pos == 0)  // if the value is to be added to the front.
    {
       // push_front(val);
       Node* newNode = new Node(val);

       newNode->next = head;
       head = newNode;

       if (newNode->next == nullptr)
           tail = newNode;

       len++ ;
       return;
    }
    else {
        Node *posNode = head;
        int iter = 0;

        while (iter < pos - 1) {
            posNode = posNode->next;
            iter++;
        }
        Node *newNode = new Node(val);

        newNode->next = posNode->next; /// Remember the 5 sticky notes
        posNode->next = newNode;

        len++;
    }
}

/// 2. Insert given value to end of list
template<class T>
void LinkedList<T>::push_back(T val)
{
    Node* node = new Node(val);

    if(head == nullptr)
    {
       head = tail = node;
       return;
    }
    else {
        tail->next = node;
        tail = node;
    }
    len++ ;
}

/// 3. Insert given value to front of list
template<class T>
void LinkedList<T>::push_front(T data)
{// same as before, so we can avoid all these if else...
    if(head == nullptr)
    {
        Node* node = new Node(data);
        // node->next = nullptr;

        head = node;
        tail = node;

        len++; return;
    } // else
    Node* node = new Node(data);

    node->next = head;
    head = node;
    len++;
}   // done

/************* END OF INSERTION FUNCTIONS ******************/

/*** MISC FUNCTIONS ***/

/// 1. Print the list object
template<class T>
void LinkedList<T>::print()
{
    if (head == nullptr)
    {
        std::cout << "list is empty";
        std::cout << std::endl;
        return;
    }

    Node* refToHead = head;
    while(refToHead != nullptr)
    {
        std::cout << refToHead->data << " " ;
        refToHead = refToHead->next;
    }
    std::cout << std::endl;
}

/// 2. checks if the list is empty.
template<class T>
bool LinkedList<T>::empty() {
    return head == nullptr ? true : false;  // I can also check for the length if it is zero or not.
}

/// 3. gets the size (number of elements) of the list.
template<class T>
int LinkedList<T>::size() {
    return len;
}

/// 4. finds a certain element in the list that is given by user (main)
template<class T>
void LinkedList<T>::find(int val)
{
    Node* reference = head;
    int index = 0;

    while (reference != nullptr)
    {
      if(reference->data == val){
          std::cout << "\nThe value " << val << " is located at index " << index << std::endl;
          return;
      }

      reference = reference->next;
      index++;
    }

    // if value is not found.
    std::cout << "The value " << val << " does not exist in the list" << std::endl;
}

/// 5. Delete all the elements of a list and release the allocated memory.
template<class T>
void LinkedList<T>::clear()
{
    Node* ref = nullptr;

    while (head != nullptr)
    {
        ref  = head;
        head = head->next;

        delete ref;
    }   // done

    // after clearing all the elements, set the length back to 0
    len = 0;
}

/// 6. reverse the list
template<class T> /// @author
void LinkedList<T>::reverse()
{
    /*sln without STL */
    if (len < 2)
        return;

    // else
    Node* previous, *current, *ahead;

    // initialization
    previous = nullptr;
    current  = head;

    while (current  != nullptr)
    {
        // in-place solution
        ahead = current->next;
        current->next = previous;
        previous = current;

        current = ahead;
    }
    // prev at this stage will point to starting node of reversed list
    head = previous;
}

/****** CONSTRUCTOR and DESTRUCTOR **********/
/// Code run upon the birth of object or list
template<class T>
LinkedList<T>::LinkedList()
{
    head = nullptr;
    tail = nullptr;

    len = 0;
}

/// Code run upon the death of object or list
template <class T>
LinkedList<T>::~LinkedList() {
    this->clear();
}
