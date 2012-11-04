#ifndef AVLTree_H
#define AVLTree_H

#include <stack>
using namespace std;

#define MAX(a,b) (a > b ? a : b)


template<typename T>
class TreeNode
{
public:
  T value; // value contained in the node
  TreeNode<T> * parent; // Pointer to the parent
  TreeNode<T> * left; // Pointer to the left child
  TreeNode<T> * right; // Pointer to the right child
  int height;

  TreeNode() // No-arg constructor
  {
    left = NULL;
    right = NULL;
    parent = NULL;
    height = 0;
  }

  TreeNode(T value) // Constructor
  {
    this->value = value;
    left = NULL;
    right = NULL;
    parent = NULL;
    height = 0;
  }
};


template < typename T >
class AVLTree
{
public:
  int treeSize;
  AVLTree();
  AVLTree(T values[],int arraySize);
  int insert(T value);
  void inOrder();
  void inOrderNorRec();
  int deleteNode(T value);
  int successor(T value);
  int predecessor(T value);
  int maxValue();
  int minValue();
  int getSize(T value);
  void output();

private:
  TreeNode<T> * treeroot;
  void LeftRotate(TreeNode<T> * target);
  void RightRotate(TreeNode<T> * target);
  void LeftRightRotate(TreeNode<T> * target);
  void RightLeftRotate(TreeNode<T> * target);
  int insert(TreeNode<T> * targetParent,TreeNode<T> * target,T value);
  void inOrder(TreeNode<T> *target);
  void inOrderNorRec(TreeNode<T> *target);
  TreeNode<T> * search(T searchvalue);
  int deleteNode(TreeNode<T> *target,T value);
  TreeNode<T> * successor(TreeNode<T> *target);
  TreeNode<T> * predecessor(TreeNode<T> *target);
  TreeNode<T> * maxValue(TreeNode<T> *target);
  TreeNode<T> * minValue(TreeNode<T> *target);
  int Height(TreeNode<T> *target);
  int getSize(TreeNode<T> *target);
  void output(TreeNode<T> *target,int totalSpaces);
};


template < typename T >
AVLTree<T>::AVLTree()
{
  treeroot = NULL;
  treeSize = 0;
}


template < typename T >
AVLTree<T>::AVLTree(T values[],int arraySize)
{
  treeroot = NULL;
  treeSize = 0;
  for(int i=0 ; i<arraySize ; i++){
    insert(treeroot,treeroot,values[i]);
  }
}


template< typename T >
void AVLTree<T>::LeftRotate(TreeNode<T> * target)
{
   TreeNode<T> *rightchild = target->right;
   target->right = rightchild->left;
   if (rightchild->left != NULL){
    rightchild->left->parent = target;
   }

   if (target->parent == NULL){
    treeroot = rightchild;
    rightchild->parent = NULL;
   }else if(target->parent->left == target){
    target->parent->left = rightchild;
    rightchild->parent = target->parent;
   }else{
    target->parent->right = rightchild;
    rightchild->parent = target->parent;
   }
   
   rightchild->left = target;
   target->parent = rightchild;

   int leftHeight = Height(target->left);
   int RightHeight = Height(target->right);
   target->height = MAX(leftHeight,RightHeight) + 1;

   leftHeight = Height(rightchild->left);
   RightHeight = Height(rightchild->right);
   rightchild->height = MAX(leftHeight,RightHeight) + 1;
}


template< typename T >
void AVLTree<T>::RightRotate(TreeNode<T> * target)
{
  {
   TreeNode<T> *leftchild = target->left;
   target->left = leftchild->right;
   if (leftchild->right != NULL){
    leftchild->left->parent = target;
   }

   if (target->parent == NULL){
    treeroot = leftchild;
    leftchild->parent = NULL;
   }else if(target->parent->left == target){
    target->parent->left = leftchild;
    leftchild->parent = target->parent;
   }else{
    target->parent->right = leftchild;
    leftchild->parent = target->parent;
   }

   leftchild->right = target;
   target->parent = leftchild;
   
   int leftHeight = Height(target->left);
   int RightHeight = Height(target->right);
   target->height = MAX(leftHeight,RightHeight) + 1;

   leftHeight = Height(leftchild->left);
   RightHeight = Height(leftchild->right);
   leftchild->height = MAX(leftHeight,RightHeight) + 1;
 }
}
 

template< typename T >
void AVLTree<T>::LeftRightRotate(TreeNode<T> * target)
{
   LeftRotate(target->left);
   RightRotate(target);
}


template< typename T >
void AVLTree<T>::RightLeftRotate(TreeNode<T> * target)
{
   RightRotate(target->right);
   LeftRotate(target);
}


template <typename T>
int AVLTree<T>::insert(T value)
{
  this->insert(treeroot,treeroot,value);
  return 0;
}


template <typename T>
int AVLTree<T>::insert(TreeNode<T> * targetParent,TreeNode<T> * target,T value)
{
  if (target == NULL){
    target = new TreeNode<T>(value);
    if (targetParent == NULL){
      this->treeroot = target;
    }else{
      target->parent = targetParent;
      if (targetParent->value > value){
        targetParent->left = target;
      }else{
        targetParent->right = target;
      }
    }
    this->treeSize++;
    return 0;
  }
  else if(target->value > value){
    this->insert(target,target->left,value);
    if(this->Height(target->left) - this->Height(target->right) >= 2){
      if(target->left->value > value){
        this->RightRotate(target);
      }
      else{
        this->LeftRightRotate(target);
      }
    }
  }
  else if(target->value < value){
    this->insert(target,target->right,value);
    if(this->Height(target->right) - this->Height(target->left) >= 2){
      if(target->right->value < value){
        this->LeftRotate(target);
      }
      else{
        this->RightLeftRotate(target);
      }
    }
  }else{
    return 1;
  }

  int leftHeight = Height(target->left);
  int RightHeight = Height(target->right);
  target->height = MAX(leftHeight,RightHeight) + 1;
  return 0;
}


template <typename T>
TreeNode<T> * AVLTree<T>::search(T searchvalue)
{
  TreeNode<T> *current = this->treeroot;
  int find =0;
  while (current != NULL && find == 0){
    if (current->value == searchvalue){
      find = 1;
    }
    else if(current->value > searchvalue){
      current = current->left;
    }else{
      current = current->right;
    }
  }

  if (find == 1){
    return current;
  }else{
    return NULL;
  }
}


template <typename T>
int AVLTree<T>::deleteNode(T value){
  TreeNode<T> *delNode = this->search(value);
  if ( delNode == NULL){
    cout << "not find " << endl;
    return 1;
  }
  this->deleteNode(this->treeroot,value);
  cout << "Node "<< value <<" has been deleted."<< endl;
  return 0;
}


template <typename T>
int AVLTree<T>::deleteNode(TreeNode<T> *target,T value){
  if(target->value > value) deleteNode(target->left,value);
  else if(target->value < value) deleteNode(target->right,value);
  else if(target->left != NULL)
  {
     target->value = this->maxValue(target->left)->value;
     deleteNode(target->left,target->value);
  }
  else if(target->right != NULL)
  {
     target->value = this->minValue(target->right)->value;
     deleteNode(target->right,target->value);
  }
  else
  {
     if(target->parent->left == target){
      target->parent->left = NULL;
     }else{
      target->parent->right = NULL;
     }
     delete target;
     this->treeSize--;
     return 0;
  }

  int leftHeight = Height(target->left);
  int RightHeight = Height(target->right);
  target->height = MAX(leftHeight,RightHeight) + 1;
  if (RightHeight - leftHeight >= 2)
  {
   if(target->right->right==NULL)
   {
    this->RightLeftRotate(target);
   }
   else
   {
    this->LeftRotate(target);
   }
  }
  else if(leftHeight - RightHeight >= 2)
  {
   if(target->left->left == NULL)
   {
    this->LeftRightRotate(target);
   }
   else
   {
    this->RightRotate(target);
   }
  }

  return 0;
}


template <typename T>
int AVLTree<T>::successor(T value)
{
  TreeNode<T> *position = this->search(value);
  if ( position == NULL){
    cout << "not find " << endl;
    return 1;
  }
  TreeNode<T> *successorNode = this->successor(position);
  if ( successorNode != NULL)
    cout << value << " \'s successor is:" << successorNode->value << endl;
  else
    cout << value << " has no successor" << endl;
  return 0;
}


template <typename T>
TreeNode<T> * AVLTree<T>::successor(TreeNode<T> *target)
{
  if ( target->right != NULL){
    return minValue(target->right);
  }
  TreeNode<T> * parentNode =target->parent;
  while ( parentNode != NULL && parentNode->right == target){
    target = parentNode;
    parentNode = parentNode->parent;
  }
  return parentNode;
}


template <typename T>
int AVLTree<T>::predecessor(T value)
{
  TreeNode<T> *position = this->search(value);
  if ( position == NULL){
    cout << "Can\'t find " << value <<" in AVL tree." <<endl;
    return 1;
  }
  TreeNode<T> *predecessorNode = this->predecessor(position);
  if ( predecessorNode != NULL)
    cout << value << " \'s predecessor is:" << predecessorNode->value << endl;
  else
    cout << value << " has no predecessor" << endl;
  return 0;
}


template <typename T>
TreeNode<T> * AVLTree<T>::predecessor(TreeNode<T> *target)
{
  if ( target->left != NULL){
    return maxValue(target->left);
  }
  TreeNode<T> * parentNode =target->parent;
  while ( parentNode != NULL && parentNode->left == target){
    target = parentNode;
    parentNode = parentNode->parent;
  }
  return parentNode;
}


template <typename T>
int AVLTree<T>::maxValue()
{
  TreeNode<T> * max = this->maxValue(treeroot);
  return max->value;
}


template <typename T>
TreeNode<T> * AVLTree<T>::maxValue(TreeNode<T> *target)
{
  while (target -> right != NULL){
    target = target -> right;
  }
  return target;
}


template <typename T>
int AVLTree<T>::minValue()
{
  TreeNode<T> * min = this->minValue(treeroot);
  return min->value;
}


template <typename T>
TreeNode<T> * AVLTree<T>::minValue(TreeNode<T> *target)
{
  while (target -> left != NULL){
    target = target -> left;
  }
  return target;
}


template <typename T>
int AVLTree<T>::getSize(T value)
{
  TreeNode<T> *target = this->search(value);
  return getSize(target);
}


template <typename T>
int AVLTree<T>::getSize(TreeNode<T> *target)
{
  if (target == NULL){
    return 0;
  }

  if (target->left == NULL && target->left == NULL){
    return 1;
  }else {
    return this->getSize(target->left) + 1 + this->getSize(target->right);
  }
}


template <typename T>
void AVLTree<T>::inOrder()
{
  inOrder(treeroot);
  cout << endl;
}


template <typename T>
void AVLTree<T>::inOrder(TreeNode<T> *target)
{
  if (target == NULL)
    return ;
  inOrder(target->left);
  cout << target->value << " ";
  inOrder(target->right);
}


template <typename T>
void AVLTree<T>::inOrderNorRec()
{
  inOrderNorRec(treeroot);
  cout << endl;
}


template <typename T>
void AVLTree<T>::inOrderNorRec(TreeNode<T> *target)
{
  stack < TreeNode<T> *> s;
  while ((target != NULL) || !s.empty())
  {
      if (target != NULL)
      {
          s.push(target);
          target = target->left;
      }
      else
      {
          target = s.top();
          cout << target->value << " ";
          s.pop();
          target = target->right;
      }
  }
}


template< typename T >
int AVLTree<T>::Height(TreeNode<T> *target)
{
  return target == NULL ? -1 : target->height;
}


template <typename T>
void AVLTree<T>::output()
{
    output(treeroot,0);
}


template <typename T>
void AVLTree<T>::output(TreeNode<T> *target,int totalSpaces)
{
    if(target != NULL)
    {
        output(target->right,totalSpaces+4);
        for(int i=0;i<totalSpaces;i++){
          cout<<' ';
        }
        if (target->parent != NULL){
          cout << target->value << "[" << target->parent->value << "]" << endl;
        }
        else{
          cout << target->value << "[ROOT]" << endl;
        }
        output(target->left,totalSpaces+4);
    }
};

#endif
