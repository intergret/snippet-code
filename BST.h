#ifndef BinarySearchTree_H
#define BinarySearchTree_H

#include <stack>
using namespace std;


template<typename T>
class TreeNode
{
public:
  T value; // value contained in the node
  TreeNode<T> * parent; // Pointer to the parent
  TreeNode<T> * left; // Pointer to the left child
  TreeNode<T> * right; // Pointer to the right child

  TreeNode() // No-arg constructor
  {
    left = NULL;
    right = NULL;
    parent = NULL;
  }

  TreeNode(T value) // Constructor
  {
    this->value = value;
    left = NULL;
    right = NULL;
    parent = NULL;
  }
};


template < typename T >
class BinarySearchTree
{
public:
  int treeSize;
  BinarySearchTree();
  BinarySearchTree(T values[],int arraySize);
  int insert(T value);
  void inOrder();
  void inOrderNorRec();
  int deleteNode(T value);
  int successor(T value);
  int predecessor(T value);
  void maxValue();
  void minValue();
  int getSize(T value);
  void output();

private:
  TreeNode<T> * treeroot;
  void inOrder(TreeNode<T> *target);
  void inOrderNorRec(TreeNode<T> *target);
  TreeNode<T> * search(T searchvalue);
  int deleteNode(TreeNode<T> *delNode);
  TreeNode<T> * successor(TreeNode<T> *target);
  TreeNode<T> * predecessor(TreeNode<T> *target);
  TreeNode<T> * maxValue(TreeNode<T> *target);
  TreeNode<T> * minValue(TreeNode<T> *target);
  int getSize(TreeNode<T> *target);
  void output(TreeNode<T> *target,int totalSpaces);
};


template < typename T >
BinarySearchTree<T>::BinarySearchTree()
{
  treeroot = NULL;
  treeSize = 0;
}


template < typename T >
BinarySearchTree<T>::BinarySearchTree(T values[],int arraySize)
{
  treeroot = NULL;
  treeSize = 0;
  for(int i=0 ; i<arraySize ; i++){
    this->insert(values[i]);
  }
}


template <typename T>
int BinarySearchTree<T>::insert(T value)
{
  if (treeroot == NULL){
    treeroot = new TreeNode<T>(value); 
  } 
  else
  {
    TreeNode<T> *former = NULL;
    TreeNode<T> *current = treeroot;
    while (current != NULL){
      if (value < current->value){
        former = current;
        current = current->left;
      }
      else if (value > current->value){
        former = current;
        current = current->right;
      }
      else{
        cout << "Node with value "<< value <<" has existed." <<endl;
        return 1;
      }
    }

    if (value < former->value){
      TreeNode<T> *newNode=new TreeNode<T>(value);
      former->left = newNode;
      newNode->parent = former;
    }
    else if(value > former->value){
      TreeNode<T> *newNode=new TreeNode<T>(value);
      former->right = newNode;
      newNode->parent = former;
    }
  }

  treeSize++;
  return 0;
}


template <typename T>
TreeNode<T> * BinarySearchTree<T>::search(T searchvalue)
{
  TreeNode<T> *current = treeroot;
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
int BinarySearchTree<T>::deleteNode(T value){
  TreeNode<T> *delNode = this->search(value);
  if ( delNode == NULL){
    cout << "not find " << endl;
    return 1;
  }
  this->deleteNode(delNode);
  cout << "Node "<< value <<" has been deleted."<< endl;
  return 0;
}


template <typename T>
int BinarySearchTree<T>::deleteNode(TreeNode<T> *delNode){
  TreeNode<T> *deleteTarget;
  if (delNode->left == NULL && delNode->right == NULL){
    deleteTarget = delNode;
  }else if(delNode->left !=NULL){
    deleteTarget = this->predecessor(delNode);
  }else if(delNode->right !=NULL){
    deleteTarget = this->successor(delNode);
  }

  TreeNode<T> *deleteTargetChild = NULL;
  if (deleteTarget->left != NULL){
    deleteTargetChild = deleteTarget->left;
  }else if (deleteTarget->right != NULL){
    deleteTargetChild = deleteTarget->right;
  }

  if (deleteTargetChild != NULL){
    deleteTargetChild->parent = deleteTarget->parent;
  }

  if (deleteTarget->parent == NULL){
    treeroot = deleteTargetChild;
    deleteTargetChild->parent = NULL;
  }else if ( deleteTarget->parent->left == deleteTarget){
    deleteTarget->parent->left = deleteTargetChild;
  }else{
    deleteTarget->parent->right = deleteTargetChild;
  }

  if (deleteTarget != delNode){
    delNode->value = deleteTarget->value;
  }

  treeSize--;
  return 0;
}


template <typename T>
int BinarySearchTree<T>::successor(T value)
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
TreeNode<T> * BinarySearchTree<T>::successor(TreeNode<T> *target)
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
int BinarySearchTree<T>::predecessor(T value)
{
  TreeNode<T> *position = this->search(value);
  if ( position == NULL){
    cout << "not find " << endl;
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
TreeNode<T> * BinarySearchTree<T>::predecessor(TreeNode<T> *target)
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
void BinarySearchTree<T>::maxValue()
{
  TreeNode<T> * max = this->maxValue(treeroot);
  cout << "Max Value is :" << max->value << endl;
}


template <typename T>
TreeNode<T> * BinarySearchTree<T>::maxValue(TreeNode<T> *target)
{
  while (target -> right != NULL){
    target = target -> right;
  }
  return target;
}


template <typename T>
void BinarySearchTree<T>::minValue()
{
  TreeNode<T> * min = this->minValue(treeroot);
  cout << "Min Value is :" << min->value << endl;
}


template <typename T>
TreeNode<T> * BinarySearchTree<T>::minValue(TreeNode<T> *target)
{
  while (target -> left != NULL){
    target = target -> left;
  }
  return target;
}


template <typename T>
int BinarySearchTree<T>::getSize(T value)
{
  TreeNode<T> *target = this->search(value);
  return getSize(target);
}


template <typename T>
int BinarySearchTree<T>::getSize(TreeNode<T> *target)
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
void BinarySearchTree<T>::inOrder()
{
  inOrder(treeroot);
}


template <typename T>
void BinarySearchTree<T>::inOrder(TreeNode<T> *target)
{
  if (target == NULL)
    return ;
  inOrder(target->left);
  cout << target->value << " ";
  inOrder(target->right);
}


template <typename T>
void BinarySearchTree<T>::inOrderNorRec()
{
  inOrderNorRec(treeroot);
}


template <typename T>
void BinarySearchTree<T>::inOrderNorRec(TreeNode<T> *target)
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


template <typename T>
void BinarySearchTree<T>::output()
{
    output(treeroot,0);
}


template <typename T>
void BinarySearchTree<T>::output(TreeNode<T> *target,int totalSpaces)
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
