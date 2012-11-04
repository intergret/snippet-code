#ifndef RBTree_H
#define RBTree_H

#include <stack>
using namespace std;

#define RED 0
#define BLACK 1


template< typename T>
class TreeNode
{
public:
  T value;
  TreeNode<T> * parent;
  TreeNode<T> * left;
  TreeNode<T> * right;
  int color;

  TreeNode() // No-arg constructor
  {
    left = NULL;
    right = NULL;
    parent = NULL;
    color = RED;
  }

  TreeNode(T value) // Constructor
  {
    this->value = value;
    left = NULL;
    right = NULL;
    parent = NULL;
    color = RED;
  }
};


template < typename T >
class RBTree
{
public:
  int treeSize;
  RBTree();
  RBTree(T values[],int arraySize);
  int insert(T value);
  void inOrder();
  void inOrderNorRec();
  int deleteNode(T value);
  int successor(T value);
  int predecessor(T value);
  int maxValue();
  int minValue();
  int getSize(T value);
  int getHeight();
  void output();

private:
  TreeNode<T> * treeroot;
  TreeNode<T> * treetail;
  void LeftRotate(TreeNode<T> * target);
  void RightRotate(TreeNode<T> * target);
  void LeftRightRotate(TreeNode<T> * target);
  void RightLeftRotate(TreeNode<T> * target);
  int insert(TreeNode<T> * targetParent,TreeNode<T> * target,T value);
  void insertFixUp(TreeNode<T> *target);
  int deleteNode(TreeNode<T> *target);
  void deleteNodeFixUp(TreeNode<T> *fixNode);
  void inOrder(TreeNode<T> *target);
  void inOrderNorRec(TreeNode<T> *target);
  TreeNode<T> * search(T searchvalue);
  TreeNode<T> * successor(TreeNode<T> *target);
  TreeNode<T> * predecessor(TreeNode<T> *target);
  TreeNode<T> * maxValue(TreeNode<T> *target);
  TreeNode<T> * minValue(TreeNode<T> *target);
  int getSize(TreeNode<T> *target);
  int getHeight(TreeNode<T> *target);
  void output(TreeNode<T> *target,int totalSpaces);
};


template <typename T>
RBTree<T>::RBTree()
{
  this->treetail = new TreeNode<T>();
  this->treetail->color = BLACK;
  this->treeroot = this->treetail;
  this->treeSize = 0;
}


template < typename T >
RBTree<T>::RBTree(T values[],int arraySize)
{
  this->treetail = new TreeNode<T>();
  this->treetail->color = BLACK;
  this->treeroot = this->treetail;
  this->treeSize = 0;

  for(int i=0 ; i<arraySize ; i++){
    this->insert(this->treeroot,this->treeroot,values[i]);
  }
}


template< typename T >
void RBTree<T>::LeftRotate(TreeNode<T> * target)
{
   TreeNode<T> *rightchild = target->right;
   target->right = rightchild->left;
   if (rightchild->left != this->treetail){
    rightchild->left->parent = target;
   }

   if (target->parent == this->treetail){
    this->treeroot = rightchild;
    rightchild->parent = this->treetail;
    rightchild->color = BLACK;
   }else if(target->parent->left == target){
    target->parent->left = rightchild;
    rightchild->parent = target->parent;
   }else{
    target->parent->right = rightchild;
    rightchild->parent = target->parent;
   }
   
   rightchild->left = target;
   target->parent = rightchild;

}


template< typename T >
void RBTree<T>::RightRotate(TreeNode<T> * target)
{
  {
   TreeNode<T> *leftchild = target->left;
   target->left = leftchild->right;
   if (leftchild->right != this->treetail){
    leftchild->left->parent = target;
   }

   if (target->parent == this->treetail){
    this->treeroot = leftchild;
    leftchild->parent = this->treetail;
    leftchild->color = BLACK;
   }else if(target->parent->left == target){
    target->parent->left = leftchild;
    leftchild->parent = target->parent;
   }else{
    target->parent->right = leftchild;
    leftchild->parent = target->parent;
   }

   leftchild->right = target;
   target->parent = leftchild;

 }
}


template <typename T>
int RBTree<T>::insert(T value)
{
  this->insert(this->treeroot,this->treeroot,value);
  return 0;
}


template <typename T>
int RBTree<T>::insert(TreeNode<T> * targetParent,TreeNode<T> * target,T value)
{
  if (this->treeroot == this->treetail){
    this->treeroot = new TreeNode<T>(value); 
    this->treeroot->color = BLACK;
    this->treeroot->parent = this->treetail;
    this->treeroot->left = this->treetail;
    this->treeroot->right = this->treetail;
  } 
  else
  {
    TreeNode<T> *former = NULL;
    TreeNode<T> *current = this->treeroot;
    while (current != this->treetail){
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

    TreeNode<T> *newNode = new TreeNode<T>(value);
    newNode->parent = former;
    newNode->left = this->treetail;
    newNode->right = this->treetail;
    if (value < former->value){
      former->left = newNode;
    }
    else if(value > former->value){
      former->right = newNode;
    }

    this->insertFixUp(newNode);
  }

  this->treeSize++;
  return 0;
}

template <typename T>
void RBTree<T>::insertFixUp(TreeNode<T> *target)
{
  while(target != this->treeroot && target->parent->color == RED){
    if(target->parent == target->parent->parent->left){
      TreeNode<T> *uncle =target->parent->parent->right;
      if (uncle->color == RED){
        target->parent->parent->color = RED;
        target->parent->color = BLACK;
        uncle->color = BLACK;
        target = target->parent->parent;
      }else{
        if(target->parent->right == target){
          target = target->parent;
          this->LeftRotate(target);
        }
        target->parent->color= BLACK;
        target->parent->parent->color= RED;
        this->RightRotate(target->parent->parent);
        break;
      }
    }
    else if(target->parent == target->parent->parent->right){
      TreeNode<T> *uncle =target->parent->parent->left;
      if (uncle->color == RED){
        target->parent->parent->color = RED;
        target->parent->color = BLACK;
        uncle->color = BLACK;
        target = target->parent->parent;
      }else{
        if(target->parent->left == target){
          target = target->parent;
          this->RightRotate(target);
        }
        target->parent->color= BLACK;
        target->parent->parent->color= RED;
        this->LeftRotate(target->parent->parent);
        break;
      }
    }
  }

  this->treeroot->color = BLACK;

}

template <typename T>
TreeNode<T> * RBTree<T>::search(T searchvalue)
{
  TreeNode<T> *current = this->treeroot;
  int find =0;
  while (current != this->treetail && find == 0){
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
    return this->treetail;
  }
}


template <typename T>
int RBTree<T>::deleteNode(T value){
  TreeNode<T> *delNode = this->search(value);
  if ( delNode == this->treetail){
    cout << "not find " << endl;
    return 1;
  }

  this->deleteNode(delNode);
  cout << "Node "<< value <<" has been deleted."<< endl;
  return 0;
}


template <typename T>
int RBTree<T>::deleteNode(TreeNode<T> *delNode){
  TreeNode<T> *deleteTarget;
  if (delNode->left == this->treetail && delNode->right == this->treetail){
    deleteTarget = delNode;
  }else if(delNode->left != this->treetail){
    deleteTarget = this->predecessor(delNode);
  }else if(delNode->right != this->treetail){
    deleteTarget = this->successor(delNode);
  }

  TreeNode<T> *deleteTargetChild = this->treetail;
  if (deleteTarget->left != this->treetail){
    deleteTargetChild = deleteTarget->left;
  }else if (deleteTarget->right != this->treetail){
    deleteTargetChild = deleteTarget->right;
  }

  deleteTargetChild->parent = deleteTarget->parent;

  if (deleteTarget->parent == this->treetail){
    this->treeroot = deleteTargetChild;
    deleteTargetChild->parent = this->treetail;
    deleteTargetChild->color = BLACK;
  }else if ( deleteTarget->parent->left == deleteTarget){
    deleteTarget->parent->left = deleteTargetChild;
  }else{
    deleteTarget->parent->right = deleteTargetChild;
  }

  if (deleteTarget != delNode){
    delNode->value = deleteTarget->value;
  }

  //Delete_Fixup
  if(deleteTarget->color == BLACK){
    if(deleteTargetChild->color == RED){
      deleteTargetChild->color = BLACK;
    }else{
      this->deleteNodeFixUp(deleteTargetChild);
    }
  }

  this->treeSize--;
  return 0;
}


template <typename T>
void RBTree<T>::deleteNodeFixUp(TreeNode<T> *fixNode){
  TreeNode<T> *brother = NULL;
  while(fixNode != this->treeroot && fixNode->color == BLACK){
    if(fixNode->parent->left == fixNode){
      brother = fixNode->parent->right;
      if(brother->color == RED){
        brother->color = BLACK;
        fixNode->parent->color = RED;
        this->LeftRotate(fixNode->parent);
        brother = fixNode->parent->right;
      }else if(brother->left->color == BLACK && brother->right->color == BLACK){
        brother->color = RED;
        fixNode = fixNode->parent;
      }else{
        if(brother->right->color == BLACK){
          brother->color = RED;
          brother->left->color = BLACK;
          this->RightRotate(brother);
          brother = fixNode->parent->right;
        }
        fixNode->parent->color = BLACK;
        brother->color = RED;
        brother->right->color = BLACK;
        this->LeftRotate(fixNode->parent);
        break;
      }
    }
    else{
      brother = fixNode->parent->left;
      if(brother->color == RED){
        brother->color = BLACK;
        fixNode->parent->color = RED;
        this->RightRotate(fixNode->parent);
        brother = fixNode->parent->left;
      }else if(brother->left->color == BLACK && brother->right->color == BLACK){
        brother->color = RED;
        fixNode = fixNode->parent;
      }else{
        if(brother->right->color == BLACK){
          brother->color = RED;
          brother->right->color = BLACK;
          this->LeftRotate(brother);
          brother = fixNode->parent->left;
        }
        fixNode->parent->color = BLACK;
        brother->color = RED;
        brother->left->color = BLACK;
        this->RightRotate(fixNode->parent);
        break;
      }
    }
  }
  fixNode->color = BLACK;
}

template <typename T>
int RBTree<T>::successor(T value)
{
  TreeNode<T> *position = this->search(value);
  if ( position == this->treetail){
    cout << "not find " << endl;
    return 1;
  }
  TreeNode<T> *successorNode = this->successor(position);
  if ( successorNode != this->treetail)
    cout << value << " \'s successor is:" << successorNode->value << endl;
  else
    cout << value << " has no successor" << endl;
  return 0;
}


template <typename T>
TreeNode<T> * RBTree<T>::successor(TreeNode<T> *target)
{
  if ( target->right != this->treetail){
    return minValue(target->right);
  }
  TreeNode<T> * parentNode =target->parent;
  while ( parentNode != this->treetail && parentNode->right == target){
    target = parentNode;
    parentNode = parentNode->parent;
  }
  return parentNode;
}


template <typename T>
int RBTree<T>::predecessor(T value)
{
  TreeNode<T> *position = this->search(value);
  if ( position == this->treetail){
    cout << "Can\'t find " << value <<" in AVL tree." <<endl;
    return 1;
  }
  TreeNode<T> *predecessorNode = this->predecessor(position);
  if ( predecessorNode != this->treetail)
    cout << value << " \'s predecessor is:" << predecessorNode->value << endl;
  else
    cout << value << " has no predecessor" << endl;
  return 0;
}


template <typename T>
TreeNode<T> * RBTree<T>::predecessor(TreeNode<T> *target)
{
  if ( target->left != this->treetail){
    return maxValue(target->left);
  }
  TreeNode<T> * parentNode =target->parent;
  while ( parentNode != this->treetail && parentNode->left == target){
    target = parentNode;
    parentNode = parentNode->parent;
  }
  return parentNode;
}


template <typename T>
int RBTree<T>::maxValue()
{
  TreeNode<T> * max = this->maxValue(this->treeroot);
  return max->value;
}


template <typename T>
TreeNode<T> * RBTree<T>::maxValue(TreeNode<T> *target)
{
  while (target -> right != this->treetail){
    target = target -> right;
  }
  return target;
}


template <typename T>
int RBTree<T>::minValue()
{
  TreeNode<T> * min = this->minValue(this->treeroot);
  return min->value;
}


template <typename T>
TreeNode<T> * RBTree<T>::minValue(TreeNode<T> *target)
{
  while (target -> left != this->treetail){
    target = target -> left;
  }
  return target;
}


template <typename T>
int RBTree<T>::getHeight()
{
  return this->getHeight(this->treeroot);
}


template <typename T>
int RBTree<T>::getHeight(TreeNode<T> *target)
{
  if(target == this->treetail){
    return -1;
  }else{
    int leftHeight = this->getHeight(target->left);
    int rightHeight = this->getHeight(target->right);
    int targetHeight = (leftHeight >= rightHeight)? leftHeight+1:rightHeight+1;
    return targetHeight;
  }
}


template <typename T>
int RBTree<T>::getSize(T value)
{
  TreeNode<T> *target = this->search(value);
  return getSize(target);
}


template <typename T>
int RBTree<T>::getSize(TreeNode<T> *target)
{
  if (target == this->treetail){
    return 0;
  }

  if (target->left == this->treetail && target->left == this->treetail){
    return 1;
  }else {
    return this->getSize(target->left) + 1 + this->getSize(target->right);
  }
}


template <typename T>
void RBTree<T>::inOrder()
{
  inOrder(this->treeroot);
  cout << endl;
}


template <typename T>
void RBTree<T>::inOrder(TreeNode<T> *target)
{
  if (target == this->treetail)
    return ;
  inOrder(target->left);
  cout << target->value << " ";
  inOrder(target->right);
}


template <typename T>
void RBTree<T>::inOrderNorRec()
{
  inOrderNorRec(this->treeroot);
  cout << endl;
}


template <typename T>
void RBTree<T>::inOrderNorRec(TreeNode<T> *target)
{
  stack < TreeNode<T> *> s;
  while ((target != this->treetail) || !s.empty())
  {
      if (target != this->treetail)
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
void RBTree<T>::output()
{
    output(this->treeroot,0);
}

template <typename T>
void RBTree<T>::output(TreeNode<T> *target,int totalSpaces)
{
    if(target != this->treetail)
    {
        output(target->right,totalSpaces+4);
        for(int i=0; i<totalSpaces; i++){
          cout<<' ';
        }
        if (target->parent != this->treetail){
          if(target->color == BLACK){
            cout << target->value  << "[" << target->parent->value << "]"  << "[B]"  << endl;
          }else{
            cout << target->value   << "[" << target->parent->value << "]" << "[R]"  << endl;
          }
        }
        else{
          cout << target->value << "[ROOT]" << "[" << target->color << "]" << endl;
        }
        output(target->left,totalSpaces+4);
    }
};

#endif
