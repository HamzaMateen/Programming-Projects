// Author@HamzaMateen

#include <iostream>
#include <string>

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
    bool valid_push(int);
    bool valid_pop(int);

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
    
private:
    struct Node
    {
        T data;
        Node* next;

        Node(T val) 
        : data(val)
        {
            next = nullptr;
        }
    };
    
private: // globals
    Node* head ;
    Node* tail ;
    
    int len ;
};

// main.cpp driver function
int main()
{
    // any type u can use here
    LinkedList<int> list;
    
    // push ints
    for(int I = 0; I < 11; I++)
        list.push_at(I, I);  
    
    // print ints
    // list.print();
    
    // inserting at head, end, and n
    list.push_front(230);
    list.push_back(534);
    list.push_at(14, 5000);
    
    std::cout << list.size() << std::endl;
    // print again
    list.print(); 
    std::cout << list.size() << std::endl;
    // delete last three inputs
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
    
    
return 0;}


// clear() function
template<class T>
void LinkedList<T>::clear()
{
    Node* ref = nullptr;
    len = 0;
 
 // since we know our list ends with a nullptr 
 // you should use while(cond)..
    while (head != nullptr)
    {
        ref  = head;
        head = head->next;
       
        delete ref;
    }   // done
}

// reverse the liked list 
template<class T>
void LinkedList<T>::reverse()
{
 // good but..
 // i'd like to see a solution without STL
    /*sln without STL */
    if (len < 2) return;
    
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

// deleteAt
// try to stick to conventions :
/*
  what will we expect from an at() method?
  1) the first index is always 0, starting at 1 leads to confusion.
  2) if at is called with an index out of bounds we should trow an error.
  - we should add a check method to let the client manually check, for example isValid(int index)
  So if a client calls at() out of bounds he will be warned
  3) dont use deleteStart and end
*/  // done

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

// deleteEnd
template<class T>
void LinkedList<T>::pop_back()
{
   if (head == nullptr) return;
    
   if (head->next == nullptr) // same as 3)
   {
       Node* ref = head;
       head = tail = nullptr;
       
       delete ref;
       
       len-- ;
       return;
   }
   /*
     here you may save a tail pointer in list members to be used here and in a getter tail method
   */ // done
   Node* ref = head;
       
   while (ref->next != tail)
       ref = ref->next;
   ref->next = nullptr ;
    
   delete tail; 
   len--;
       
   tail = ref;
}

// deleteStart
template <class T>
void LinkedList<T>::pop_front()
{
    if (head == nullptr) return;
    else if (len == 1)
    {
       Node* ref = head;
       head = tail = nullptr;
       
       delete ref;
       
       len-- ;
    }  // tail will not be disturbed
    else  {
    Node* ref = head;
    head = head->next;
    
    delete ref;
    
    len-- ;
    }
}

// insertAt
template<class T>
void LinkedList<T>::push_at(int pos, T val)
{
 // here we have different options..
 /*
  -we can insert before or after an element
   checking and getting it
  - we can trow an error.. go and check what STL do in this case
 */
    /*here sir if u have any other. suggestion please add as i couldn't extract much help from cppreference <stl>, it was hard sorry'*/
    
    if (pos < 0 || pos > len)
    { 
       std::cout << "[ERROR] index " << pos << " out of range" << std::endl;
       return;
    }
    
    if (pos == 0)
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
    // if insertAtlast
    if (pos == len)
    {
        push_back(val);
        return;
    }
    
    Node* posNode = head;
    int iter = 0;
    
    while (iter < pos-1)
    { 
        posNode = posNode->next;
        iter++;
    }
    Node* newNode = new Node(val);
    
    newNode->next = posNode->next;
    posNode->next = newNode;
        
    len++;
}

// insertAtEnd()
template<class T>
void LinkedList<T>::push_back(T val)
{
 // here you may do something like i did in clear
    if(head == nullptr)
    {
       Node* newNode = new Node(val);
       
       head = newNode;
       tail = newNode;
       
       len++ ;
       return;
    }  //else
    
    Node* node = new Node(val);
    
    tail->next = node;
    tail = node; len++;
}

// insertAtStart
template<class T>
void LinkedList<T>::push_front(T data)
{// same as before.. so we can avoid all these if else...
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

// printList
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

// attributed functions
template<class T>
bool LinkedList<T>::empty()
{
return head == nullptr ? true : false;
}

template<class T>
int LinkedList<T>::size()
{
    return len;
} 

template<class T>
void LinkedList<T>::find(int val)
{
    Node* reference = head;
    int index = 1;
    
    while (reference != nullptr)
    {
      if(reference->data == val)
      {
      std::cout << "\nThe value " << val  << " is located at index " << index << std::endl;
      return;    
      }
      reference = reference->next;
      index++;  
    }
    // else
    std::cout << "The value " << val << " does not exist in the list" << std::endl;    
}

// index validation
template<class T>
bool LinkedList<T>::valid_push(int ind)
{
    if (ind < 0 || ind > len )
        return false;
        
    return true;
} 

template<class T>
bool LinkedList<T>::valid_pop(int ind)
{
    if (ind < 0 || ind > len -1)
        return false;

    return true;
}

// ctor dtor
template<class T>
LinkedList<T>::LinkedList()  : len (0) 
{
    head = nullptr;
    tail = nullptr;
}

template <class T>
LinkedList<T>::~LinkedList() {
    // call clear()?... affirmative
    this->clear();
};

