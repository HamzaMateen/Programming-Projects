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

    LinkedList operator = ( LinkedList& ) = delete;
    LinkedList( LinkedList& ) = delete;
    
    
    bool empty();
    bool valid_push( int );
    bool valid_pop( int );
    int  size();
    
    bool contains ( T );
    void print();
    
    void push_front( T );
    void push_back( T );
    void insert_at( int , T );
    
    void pop_front();
    void pop_back();
    void remove_at( int );
    
    void reverse();
    void clear();
    
private:
    struct Node
    {
        T     data;
        Node* next;

        Node( T val ) : data( val )
        {
            next = nullptr;
        }
    };
    

    Node* head ;
    Node* tail ;
    
    int len ;
    
    template < typename R>
    friend ostream& operator<< ( ostream &out, const LinkedList<R>& obj ); 

 

};

template <typename T>
ostream& operator<<( ostream& out, const LinkedList<T>& obj) 
{   
    Node* n = obj.head ;
    out<<"[";
    while ( n->next != nullptr ){
       out<< n->data <<", ";
    }
    out<< n->data << "]";
    return out;
}


// main.cpp driver function
int main()
{
    // any type u can use here
    LinkedList<int> list;
    
    // push ints
    for(int I = 0; I < 11; I++)
        list.push_back ( I);  
    
    // print ints
    // list.print();
    
    // inserting at head, end, and n
    list.push_front(230);
    list.push_back(534);
    std::cout<< list<<std::endl;
    
    
return 0;}



template<class T>
void LinkedList<T>::clear()
{
    Node* ref = nullptr;
    
    while ( head != nullptr )
    {
        ref  = head;
        head = head->next;
       
        delete ref;
    }
    
    len = 0;
}


template<class T>
void LinkedList<T>::reverse()
{
    if ( len < 2 ) return;
    
    Node* previous = nullptr ;
    Node* current  = head ; 
    Node* ahead    = nullptr ;

        
    while ( current  != nullptr )
    {   
        ahead         = current->next;
        current->next = previous;
        previous      = current;
        current       = ahead;       
    }
    
    head = previous;  
    
}



template<class T>
void LinkedList<T>::remove_at(int pos)
{
    if ( pos < 0    || 
         pos >= len ){
       std::cout << "[List::ERROR] index " << pos << " out of range " << std::endl;
       return;
    }
    
 
     int     iter = 0;
     Node*   ref  = head;
        
     while ( iter < pos )
     {
         ref = ref->next;
         iter++;
     }
        
     Node* temp = ref->next;
     ref->next  = temp->next;
        
     delete temp;
     len--;
}


template<class T>
void LinkedList<T>::pop_back()
{
   if ( head == nullptr ) 
       return;
   
  
   Node* ref = head;
       
   while ( ref->next != tail )
       ref = ref->next;
       
   ref->next = nullptr ;
    
   delete tail; 
 
   tail = ref;
   
   len--;
}


template <class T>
void LinkedList<T>::pop_front()
{
    if (head == nullptr)
       return;
    
    
    Node* ref = head;
    head = head->next;
    
    delete ref;
    
    len-- ;

}


template<class T>
void LinkedList<T>::insert_at( int pos , T val )
{
    if ( pos < 0   ||
         pos > len ){ 
       std::cout << "[List::ERROR] index " << pos << " out of range" << std::endl;
       return;
    }
    
    
    Node* node = head;
    int   iter = 0;
    
    while ( iter < pos-1 )
    { 
        node = node->next;
        iter++;
    }
    Node* newNode = new Node(val);
    
    newNode->next = node->next;
    node->next = newNode;
        
    len++;
}


template<class T>
void LinkedList<T>::push_back(T val)
{
    Node* node = new Node(val);
    
    if ( head == nullptr ){
        head = tail = node;
    }
    else {
        tail->next = node;
    }
    
    tail = node; 
    len++;
}


template<class T>
void LinkedList<T>::push_front( T data )
{
    Node* node = new Node( data );
    
    if ( head == nullptr ){
        head = node;
        tail = node;
    
    } 
    else {
        node->next = head;
        head = node;
    }
    
    len++;   
} 

// printList
template<class T>
void LinkedList<T>::print()
{
    //std::cout
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


template<class T>
bool LinkedList<T>::empty()
{
    return head == nullptr ;
}

template<class T>
int LinkedList<T>::size()
{
    return len;
} 

template<class T>
bool LinkedList<T>::contains( T val )
{
    Node* it    = head;
    int   index = 1;
    bool  found = false ;
    
    while ( it != nullptr ){
      if ( it->data == val ){
         found = true ;
         break ;
      }
      it = it->next;
      index++;  
    }
  
    return found ;    
}
/*
template<class T>
int LinkedList<T>::find( T val )
{
    if ( contains( val ) == false ){
       std::cout << "[List::find::ERROR] " << " item not in List." << std::endl;
       
    }
    return 0;
}
*/

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

