#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int pathLengthRight(Node * r, int length);
int pathLengthLeft(Node * r, int length);
int pathLength(Node * r, int length);

bool equalPaths(Node * root)
{
   int plr=0;
   int pll=0;
    if(root == NULL){
        return true;
    }
    if(root->right == NULL && root->left == NULL){
        return true;
    }
    if(root->right == NULL){
        return equalPaths(root->left);
    }
    if(root->left == NULL){
        return equalPaths(root->right);
    }
    plr = pathLengthRight(root,0);
    pll = pathLengthLeft(root,0);
    if(pathLengthRight(root,0) != pathLengthLeft(root,0)){
        return false;
    }
    else {
        return true;
    }
}


int pathLength(Node * root, int length){
     if(root == NULL)
    {
        return 0;
    }
    if(root->right == NULL && root->left == NULL){
        return length+1;
    }

    if(root->right == NULL){
            return pathLength(root->left, length);
        }
        if(root->left == NULL){
            return pathLength(root->right, length);
        }
        else{

            if(pathLength(root->left, length) == pathLength(root->right, length)){
                return length + 1;
            }
            else return -1;

        }
 }
int pathLengthRight(Node * root, int length)
{
    if(root == NULL)
    {
        return 0;
    }
    return (length+pathLengthRight(root->right,length+1));
}
int pathLengthLeft(Node * root, int length)
{
    if(root == NULL)
    {
        return 0;
    }
    return (length+pathLengthLeft(root->left,length+1));
}

