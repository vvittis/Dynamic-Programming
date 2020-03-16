#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// int a[30];

// for update the array a
int counter = 0;

// for update the array nodes
int counter1 = 0;

// structure node
typedef struct node {
    int data;
    int sign;
    struct node *left;
    struct node *right;
} node;

// help array for recursion
//struct node nodes[9] = {-1};


//depth of minimax game tree
int depth(int A,int D,int d){
    double min = (D>d) ? d : D;
    return (A%2 == 0) ? ceil(A/min) : (floor(A/min) + 1);
}


//size of array
int sizeTree(int depth){

    int size=0;
    for (int i = 1; i <=depth; ++i) {
        size+=pow(2,i);
    }
    return size;
}


// Create minimax game tree
node *create(int x,int D,int d) {
    node *p;

    // no expand
    if (x <= 0) {
        p = (node *)malloc(sizeof(node));
        p->data = x;
        p->left = NULL;
        p->right = NULL;
        return p;
    }

    // expand
    p = (node *) malloc(sizeof(node));
    p->data = x;
    p->left = create(x - d,D,d);
    p->right = create(x - D,D,d);

    return p;
}


// Get the level on tree from a specific node
int getLevelCount(node *node) {

    if (node->left == NULL && node->right== NULL) {
        return 0;
    }

    int leftMaxLevel = 1 + getLevelCount(node->left);
    int rightMaxLevel = 1 + getLevelCount(node->right);
    if (leftMaxLevel > rightMaxLevel) {
        return leftMaxLevel;
    } else {
        return rightMaxLevel;
    }

}


// Take the level of node and initialize the value of node to array a
void printLevel(node *node, int level,int *array) {
    if (node != NULL && level == 0) {
        array[counter] = node->data;
        counter = counter + 1;
    } else if (node != NULL) {
        printLevel(node->left, level - 1,array);
        printLevel(node->right, level - 1,array);
    }
}


// Initialize the elements on array a(values of tree) through printLevel, call for root only
void printElements(node *node,int *array) {
    int i;
    int levelCount = getLevelCount(node);
    for (i = 0; i <=levelCount; i++) {
        printLevel(node,i,array);
    }
}


// Same with printElements but for specific k(level of node) , initialize the array nodes for specific k
void printKDistant(struct node *root, int k,struct node *nodes) {
    if (root == NULL) // if node has not child, stop the recursion!!!
        return;
    if (k == 0) { // get the value of node , add to array nodes
        nodes[counter1].data = root->data;
        nodes[counter1].right = root->right;
        nodes[counter1].left = root->left;
        counter1++;
        return;
    }
    else {
        printKDistant(root->left, k - 1,nodes);
        printKDistant(root->right, k - 1,nodes);
    }
}


int main() {

    node *root;

    int A=10;
    int D=5;
    int d=2;
    root = create(A,D,d);

    printf("\nThe preorder traversal of tree is:\n");

    //Initialize the array with A+1!!!
    int sizeOfArray = sizeTree(depth(A,D,d));
    int a[sizeOfArray];
    for (int k = 0; k < sizeOfArray; ++k) {
        a[k]=A+1;
    }

    //Get the elements from game-tree
    printElements(root,a);

    //The level of tree
    int levels = getLevelCount(root);
    printf("Levels: %d\n\n",levels);

    for (int k = 0; k < sizeOfArray ; ++k) {
        printf("The %dth value of a is %d \n",k,a[k]);
    }

    int player = 1;
    //Player1 is maximizer
    //Player2 is minimizer

    for (int i = levels-1; i >= 0; i--){

        if (i % 2 == 0) {
            printf("Level %d: Player 1 is playing and wants to maximize\n", i);
            player = 1;
        }
        else if (i % 2 == 1) {
            printf("Level %d: Player 2 is playing and wants to minimize\n", i);
            player = 2;
        }

        counter1 = 0;
        // initialize array nodes
        int size = pow(2,i+1);
        struct node nodes[size];
        memset(nodes,-1, sizeof(nodes));

        // get the values for (i+1)th level
        printKDistant(root,i,nodes);

        // size of array nodes , maybe not need
        // int size = sizeof(nodes) / sizeof(nodes[0]);

        for (int j = 0; j < size; j++) {

            //last level only-maybe not needed!!!
            /*if( nodes[j].data == 0 && i==levels){
                if (player == 1){
                    printf("The optimal move is d or D\n");
                }
                else{
                    printf("The optimal move is d or D\n");
                }
                break;
            }
            */

            if (nodes[j].data > 0) {
                printf("Node: %d , Distance: %d", j, nodes[j].data);

                if (player == 1) {
                    printf("\nLeft aka node-2(d):  %d Right aka node-5(D): %d\n",nodes[j].left->data,nodes[j].right->data );
                    if (nodes[j].left->data > nodes[j].right->data) {
                        printf("The optimal move is d\n");
                    } else {
                        printf("The optimal move is D\n");
                    }
                }
                if (player == 2) {
                    printf("\nLeft aka node-2(d):  %d Right aka node-5(D): %d\n",nodes[j].left->data,nodes[j].right->data );
                    if (nodes[j].left->data > nodes[j].right->data) {
                        printf("The optimal move is D\n");
                    } else {
                        printf("The optimal move is d\n");
                    }
                }

            }


        }

        printf("\n");
        free(nodes);
    }

    free(root);
    return 0;
}
