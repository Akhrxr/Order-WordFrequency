//-----------------------------------------------------------------------------
// Dictionary.cpp
// Cpp file for the Dictionary ADT
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>
#include<cmath>
#include"Dictionary.h"

Dictionary::Node::Node(keyType k, valType v){
   key = k;
   val = v;
   parent = nullptr;
   left = nullptr;
   right = nullptr;
   color = 0; // 0 is red, 1 is black
}

// Helper Functions (Optional) ---------------------------------------------

Dictionary::Node* Dictionary::search(Node* R, keyType k) const {
    if (R == nil) {
      return nil;
    }
    else if (k == R->key) {
        return R;
    }
    else if (k < R->key) {
      return search(R->left, k);
    }
    else { // k > R.key 
      return search(R->right, k);
    }
}

void Dictionary::RB_Transplant(Node* U, Node* V) {
        if (U->parent == nil) {
            this->root = V;
        }
        else if (U == U->parent->left) {
            U->parent->left = V;
        }
        else {
            U->parent->right = V;
        }
        V->parent = U->parent;
}

void Dictionary::LeftRotate(Node* N) {
   // set y
   Node* y = N->right;
   
   // turn y's left subtree into N's right subtree
    N->right = y->left;
   if (y->left != nil) {    // not necessary if using sentinal nil node
    y->left->parent = N;
   }
   
   
   // link y's parent to N
   y->parent = N->parent;
   if (N->parent == nil) {
      this->root = y;
   }
   else if (N == N->parent->left) {
      N->parent->left = y;
   }
   else {
      N->parent->right = y;
   }
   
   // put N on y's left
   y->left = N; 
   N->parent = y;
}

void Dictionary::RightRotate(Node* N) {
   Node* y = N->left;
   
   // turn y's left subtree into N's right subtree
    N->left = y->right;
   if (y->right != nil) {   // not necessary if using sentinal nil node
    y->right->parent = N;
   }
   
   // link y's parent to N
   y->parent = N->parent;
   if (N->parent == nil) {
      this->root = y;
   }
   else if (N == N->parent->right) {
      N->parent->right = y;
   }
   else {
      N->parent->left = y;
   }
   
   // put N on y's left
   y->right = N; 
   N->parent = y;
}

void Dictionary::RB_InsertFixUp(Node* N) {
    while (N->parent->color == 0) {
      if (N->parent == N->parent->parent->left) {
        Node *y = N->parent->parent->right;
        if (y->color == 0) {
            N->parent->color = 1;              // case 1
            y->color = 1;                     // case 1
            N->parent->parent->color = 0;         // case 1
            N = N->parent->parent;  
        }                                   // case 1
        else {
            if (N == N->parent->right) {
               N = N->parent;                    // case 2
               LeftRotate(N);                // case 2
            }
            N->parent->color = 1;              // case 3
            N->parent->parent->color = 0;         // case 3
            RightRotate(N->parent->parent);     // case 3
        }
      }
      else {
         Node* y = N->parent->parent->left;
         if (y->color == 0) {
            N->parent->color = 1;             // case 4
            y->color = 1;                     // case 4
            N->parent->parent->color = 0;         // case 4
            N = N->parent->parent;                 // case 4
         }
         else {
            if (N == N->parent->left) {
               N = N->parent;                     // case 5
               RightRotate(N);                // case 5
            }
            N->parent->color = 1;              // case 6
            N->parent->parent->color = 0;         // case 6
            LeftRotate(N->parent->parent);      // case 6
        }
      }
    }
   root->color = 1;
}

void Dictionary::RB_DeleteFixUp(Node* N){
    while (N != root && N->color == 1) {
        if (N == N->parent->left) {
            Node* w = N->parent->right;
            if (w->color == 0) {
                w->color = 1;                        // case 1
                N->parent->color = 0;                   // case 1
                LeftRotate(N->parent);                // case 1
                w = N->parent->right;  
            }
            if (w->left->color == 1 && w->right->color == 1){
                w->color = 0;                          // case 2
                N = N->parent;                           // case 2
            }
            else {
                if (w->right->color == 1) {
                    w->left->color = 1;                // case 3
                    w->color = 0;                       // case 3
                    RightRotate(w);                   // case 3
                    w = N->parent->right;                  // case 3
                }
                w->color = N->parent->color;               // case 4
                N->parent->color = 1;                 // case 4
                w->right->color = 1;                  // case 4
                LeftRotate(N->parent);                // case 4
                N = root;                             // case 4
            }
        }
        else  {
            Node* w = N->parent->left;
            if (w->color == 0) {
                w->color = 1;                        // case 5
                N->parent->color = 0;                   // case 5
                RightRotate(N->parent);               // case 5
                w = N->parent->left;                      // case 5
            }
            if (w->right->color == 1 && w->left->color == 1) {
                w->color = 0;                          // case 6
                N = N->parent;                           // case 6
            }
            else {
                if (w->left->color == 1){
                    w->right->color = 1;               // case 7
                    w->color = 0;                       // case 7
                    LeftRotate(w);                    // case 7
                    w = N->parent->left;                   // case 7
                }
                w->color = N->parent->color;               // case 8
                N->parent->color = 1;                 // case 8
                w->left->color = 1;                   // case 8
                RightRotate(N->parent);               // case 8
                N = root;                             // case 8
            }
      }
    }
    
   N->color = 1;
}

void Dictionary::RB_Delete(Node* N){
    Node* y = N;
    Node* x;
    int y_original_color = y->color;
    if (N->left == nil){
      x = N->right;
      RB_Transplant(N, N->right);
    }
    else if (N->right == nil){
      x = N->left;
      RB_Transplant(N, N->left);
    }
    else {
      y = Dictionary::findMin(N->right);
      y_original_color = y->color;
      x = y->right;
      if (y->parent == N) {
         x->parent = y;
      }
      else {
         RB_Transplant(y, y->right);
         y->right = N->right;
         y->right->parent = y;
      }
      RB_Transplant(N, y);
      y->left = N->left;
      y->left->parent = y;
      y->color = N->color;
   }
   if (y_original_color == 1){
      RB_DeleteFixUp(x);
   }
}



void Dictionary::postOrderDelete(Node* R) {
    if (R !=nil && R) {
      postOrderDelete(R->left);
      postOrderDelete(R->right);
      delete(R);
    }
}

void Dictionary::inOrderString(std::string& s, Node* R) const{
    if (R !=nil  && R) {
        inOrderString(s, R->left);
        s.append(R->key);
        s.append(" : ");
        std::string vals = std::to_string(R->val);
        s.append(vals);
        s.append("\n");
        inOrderString(s, R->right);
    }
}

void Dictionary::preOrderCopy(Node* R, Node* N){
    if (R != nil && R) {
        if (R != N) {
            Dictionary::setValue(R->key, R->val);
            Dictionary::preOrderCopy(R->left, N);
            Dictionary::preOrderCopy(R->right, N);
        }
    }
}

void Dictionary::preOrderString(std::string& s, Node* R) const{
    if (R!=nil && R) {
        s.append(R->key);
        s.append("\n");
        Dictionary::preOrderString(s, R->left);
        Dictionary::preOrderString(s, R->right);
    }
}

Dictionary::Node* Dictionary::findMin(Node* R){
    if (R != nil && R) {
        while (R->left != nil) {
            R = R->left;
        }
    }
    //else std::cerr << "R is nil in findMin \n";
    return R;
}

Dictionary::Node* Dictionary::findMax(Node* R){
    if (R != nil && R) {
        while (R->right != nil) {
            R = R->right;
        }
    }
    //else std::cerr << "R is nil in findMax\n";
    return R;
}


Dictionary::Node* Dictionary::findNext(Node* N){
    if (N == nil || N == Dictionary::findMax(this->root)) return nil;
    if (N->right != nil)                 // case 1
        return Dictionary::findMin(N->right);
    Node* y = N->parent;                 // case 2
    while (y != nil && N == y->right) {
        N = y;
        y = y->parent;
    }
    return y;
}

Dictionary::Node* Dictionary::findPrev(Node* N){
    if (N == nil || N == Dictionary::findMin(this->root)) return nil;
    if (N->left != nil)                 // case 1
        return Dictionary::findMax(N->left);
    Node* y = N->parent;                 // case 2
    while (y != nil && N == y->left) {
        N = y;
        y = y->parent;
    }
    return y;
}
// Class Constructors & Destructors ----------------------------------------

Dictionary::Dictionary() {
    nil = new Node("nillrt" , 0);
    nil->color = 1;
    this->current = this->root = nil;
    num_pairs = 0;
}

Dictionary::Dictionary(const Dictionary& D) {
    nil = new Node("nillrt" , 0);
    nil->color = 1;
    this->current = this->root = nil;
    num_pairs = 0;
    Dictionary::preOrderCopy(D.root, D.nil);
}

Dictionary::~Dictionary() {
    Dictionary::clear();
    root = current = nil = nullptr;
    delete root;
    delete current;
    delete nil;
}

// Access functions --------------------------------------------------------

int Dictionary::size() const {
    return this->num_pairs;
}

bool Dictionary::contains(keyType k) const {
    return (search(root, k) != nil);
}

valType& Dictionary::getValue(keyType k) const{
    if (!Dictionary::contains(k)) {
        throw std::logic_error("This dictionary does not contain key k. \n");
    }
    Node* R = root;
    while (R->key != k){
        if (R->key > k) {
            R = R->left;
        }
        else {
            R = R->right;
        }
    }
    return R->val;
}

bool Dictionary::hasCurrent() const{
    return (this->current != nil && this->current);
}

keyType Dictionary::currentKey() const{
    if (!Dictionary::hasCurrent()) {
        throw std::logic_error ("Cannot call currentKey() as current is not defined \n");
    }
    return current->key;
}

valType& Dictionary::currentVal() const{
    if (!Dictionary::hasCurrent()) {
        throw std::logic_error ("Cannot call currentVal() as current is not defined \n");
    }
    return current->val;
}

// Manipulation procedures -------------------------------------------------

void Dictionary::clear(){
    if (num_pairs != 0) {
        postOrderDelete(this->root);
    }
    this->num_pairs = 0;
    this->root = this->current = nil;
}

void Dictionary::setValue(keyType k, valType v){
    Node* z = new Node(k,v);
    Node* y = nil;
    Node* x = root;
    while (x != nil && x->key != z->key){
      y = x;
      if (z->key < x->key)
         x = x->left;
      else 
         x = x->right;
    }
    if (x->key == z->key) { //rewrites it if its equal to it.
        x->val = v;
        return;
    }
    z->parent = y;
    if (y == nil) {
      root = z;
    }
    else if (z->key < y->key) {
      y->left = z;
    }
    else {
      y->right = z;
    }
    z->left = nil;
    z->right = nil;
    z->color = 0;
    RB_InsertFixUp(z);
    ++num_pairs;
}

void Dictionary::remove(keyType k){
    Node* z = search(root, k);
    if (z == nil) {
        return;
    }
    if (current == z) {
        current = nil;
    }
    RB_Delete(z);
    --num_pairs;
    delete z;
}

void Dictionary::begin(){
    this->current = findMin(this->root);
}

void Dictionary::end(){
    this->current = findMax(this->root);
}

void Dictionary::next(){
    if (!Dictionary::hasCurrent()) {
        throw std::logic_error ("Cannot call next() as current is not defined \n");
    }
    this->current = findNext(this->current);
}

void Dictionary::prev(){
    if (!Dictionary::hasCurrent()) {
        throw std::logic_error ("Cannot call prev() as current is not defined \n");
    }
    this->current = findPrev(this->current);
}

// Other Functions ---------------------------------------------------------

std::string Dictionary::to_string() const{
    std::string s = "";
    Dictionary::inOrderString(s, this->root);
    return s;
}

std::string Dictionary::pre_string() const{
    std::string s = "";
    Dictionary::preOrderString(s, this->root);
    return s;
}

bool Dictionary::equals(const Dictionary& D) const{
    std::string s = to_string();
    std::string p = D.to_string();
    return (s == p);
}

std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    return stream << D.to_string();
}

bool operator==( const Dictionary& A, const Dictionary& B ){
    return (A.equals(B));
}

Dictionary& Dictionary::operator=( const Dictionary& D ){
    if( this != &D ){ // not self assignment
      // make a copy of Q
      Dictionary temp = D;

      // then swap the copy's fields with fields of this
      std::swap(nil, temp.nil);
      std::swap(root, temp.root);
      std::swap(current, temp.current);
      std::swap(num_pairs, temp.num_pairs);
   }

   // return this with the new data installed
   return *this;

   // the copy, if there is one, is deleted upon return
}
