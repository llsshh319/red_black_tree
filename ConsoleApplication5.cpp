#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <math.h>

enum Color { RED, BLACK };
struct Node {
	int val;
	Color color;
	Node *left, *right, *parent;
};

Node* newnode(int val)
{
	Node *node = (Node*)malloc(sizeof(struct Node));
	node->val = val;
	node->right = NULL;
	node->left = NULL;
	node->parent = NULL;
	node->color = RED;
	return node;
}

struct RBT {
	Node* root;
	int total, nb, bh;
};

RBT* newRBT()
{
	RBT *rbt = (RBT*)malloc(sizeof(struct RBT));
	rbt->root = NULL;
	rbt->total = 0;
	rbt->bh = 0;
	rbt->nb = 0;
	return rbt;
}
void swapcolor(Node* node)
{
	if (node != NULL)
	{
		if (node->color == RED)
			node->color = BLACK;
		else
			node->color = RED;
	}
}
void rotateleft(RBT* rbt, Node* node)
{
	Node* newnode = node->right;
	Node* parent = node->parent;
	if (newnode == NULL)
		return;
	if (newnode->left != NULL)
		newnode->left->parent = node;
	node->right = newnode->left;
	node->parent = newnode;
	newnode->left = node;
	newnode->parent = parent;
	if (parent != NULL && parent->left == node)
		parent->left = newnode;
	else if (parent != NULL && parent->right == node)
		parent->right = newnode;
	if (rbt->root == node)
		rbt->root = newnode;
}
void rotateright(RBT* rbt, Node* node)
{
	Node* newnode = node->left;
	Node* parent = node->parent;
	if (newnode == NULL)
		return;
	if (newnode->right != NULL)
		newnode->right->parent = node;
	node->left = newnode->right;
	node->parent = newnode;
	newnode->right = node;
	newnode->parent = parent;
	if (parent != NULL && parent->right == node)
		parent->right = newnode;
	else if (parent != NULL && parent->left == node)
		parent->left = newnode;
	if (rbt->root == node)
		rbt->root = newnode;
}

void insertcolor(RBT* rbt, Node* node)
{
	if (node == NULL)
		return;
	Node *parent = node->parent, *grand = NULL, *uncle = NULL;
	if (parent != NULL)
	{
		grand = parent->parent;
		if (grand != NULL && grand->left == parent)
			uncle = grand->right;
		else if (grand != NULL && grand->right == parent)
			uncle = grand->left;
	}
	if (parent != NULL && parent->color == RED)
	{
		if (uncle != NULL && uncle->color == RED)
		{
			swapcolor(parent);
			swapcolor(grand);
			swapcolor(uncle);
			insertcolor(rbt, grand);
		}
		else if (uncle == NULL || uncle->color == BLACK)
		{
			if (grand->left == parent && parent->right == node)
			{
				rotateleft(rbt, parent);
				node = node->left;
			}
			else if (grand->right == parent && parent->left == node)
			{
				rotateright(rbt, parent);
				node = node->right;
			}
			parent = node->parent;

			swapcolor(parent);
			swapcolor(grand);
			if (node == parent->left)
				rotateright(rbt, grand);
			else
				rotateleft(rbt, grand);

		}
	}
	rbt->root->color = BLACK;
}

void insertNode(RBT* rbt, int val)
{
	Node* node;
	if (rbt->root == NULL)
	{
		rbt->root = newnode(val);
		node = rbt->root;
	}
	else
	{
		Node* prenode = NULL;
		node = rbt->root;
		while (node != NULL)
		{
			prenode = node;
			if (node->val < val)
				node = node->right;
			else
				node = node->left;
		}
		if (prenode->val < val)
		{
			prenode->right = newnode(val);
			prenode->right->parent = prenode;
			node = prenode->right;
		}
		else
		{
			prenode->left = newnode(val);
			prenode->left->parent = prenode;
			node = prenode->left;
		}
	}
	insertcolor(rbt, node);
	rbt->total++;
}

void transplant(RBT* rbt, Node* from, Node* to)
{
	if (from->parent == NULL)
		rbt->root = to;
	else if (from == from->parent->left)
		from->parent->left = to;
	else
		from->parent->right = to;
	if (to != NULL)
		to->parent = from->parent;
}

Node* succesor(Node* node)
{
	if (node->right == NULL)
	{
		if (node->parent != NULL)
			return node->parent;
		else
			return NULL;
	}
	else
	{
		Node* nextnode = node->right;
		while (nextnode->left != NULL)
			nextnode = nextnode->left;
		return nextnode;
	}
}

bool deleteNode(RBT* rbt, int val)
{
	if (rbt->root == NULL)
		return false;
	else
	{
		Node* node = rbt->root;
		while (node->val != val)
		{
			if (node->val < val)
				node = node->right;
			else
				node = node->left;
			if (node == NULL)
				return false;
		}

		Color nodecolor = node->color;
		Node *temp = node, *child, *sib, *parent;
		if (node->left == NULL)
		{
			child = node->right;
			transplant(rbt, node, child);
		}
		else if (node->right == NULL)
		{
			child = node->left;
			transplant(rbt, node, child);
		}
		else
		{
			temp = succesor(node);
			nodecolor = temp->color;
			child = node->right;
			if (temp->parent == node)
				child->parent = temp;
			else
			{
				transplant(rbt, temp, temp->right);
				temp->right = node->right;
				temp->right->parent = temp;
			}
			transplant(rbt, node, temp);
			temp->left = node->left;
			temp->left->parent = temp;
			temp->color = node->color;
			parent = temp;
		}
		if (child == NULL)
			child = node;
		if (nodecolor == BLACK)
		{
			while (child != rbt->root && child->color == BLACK)
			{
				if (child == child->parent->left)
				{
					sib = child->parent->right;
					if (sib->color == RED)
					{
						sib->color == BLACK;
						child->parent->color == RED;
						rotateleft(rbt, child->parent);
						sib = child->parent->right;
					}
					if ((sib->left == NULL || sib->left->color == BLACK) && (sib->right == NULL || sib->right->color == BLACK))
					{
						sib->color = RED;
						child = child->parent;
					}
					else
					{
						if (sib->right == NULL || sib->right->color == BLACK)
						{
							if (sib->left != NULL)
								sib->left->color = BLACK;
							sib->color = RED;
							rotateright(rbt, sib);
							sib = child->parent->right;
						}
						sib->color = child->parent->color;
						child->parent->color = BLACK;
						if (sib->right != NULL)
							sib->right->color = BLACK;
						rotateleft(rbt, child->parent);
						child = rbt->root;
					}
				}
				else
				{
					sib = child->parent->left;
					if (sib->color == RED)
					{
						sib->color == BLACK;
						child->parent->color == RED;
						rotateleft(rbt, child->parent);
						sib = child->parent->left;
					}
					if ((sib->right == NULL || sib->right->color == BLACK) && (sib->left == NULL || sib->left->color == BLACK))
					{
						sib->color = RED;
						child = child->parent;
					}
					else
					{
						if (sib->left == NULL || sib->left->color == BLACK)
						{
							if (sib->right != NULL)
								sib->right->color = BLACK;
							sib->color = RED;
							rotateright(rbt, sib);
							sib = child->parent->left;
						}
						sib->color = child->parent->color;
						child->parent->color = BLACK;
						if (sib->left != NULL)
							sib->left->color = BLACK;
						rotateleft(rbt, child->parent);
						child = rbt->root;
					}

				}
			}
			if (child != NULL)
				child->color = BLACK;
		}

		rbt->total--;
		return true;
	}
}

void inorder(RBT* rbt, Node* node)
{
	if (node != NULL)
	{
		if (node->left != NULL)
			inorder(rbt, node->left);
		if (node->color == BLACK)
			rbt->nb++;
		if (node->right != NULL)
			inorder(rbt, node->right);
	}
}
void printinorder(Node* node)
{
	if (node != NULL)
	{
		if (node->left != NULL)
			printinorder(node->left);
		printf("%d\n", node->val);
		if (node->right != NULL)
			printinorder(node->right);
	}
}
void print(RBT* rbt)
{
	Node* node = rbt->root;
	inorder(rbt, node);
	while (node != NULL)
	{
		if (node->color == BLACK)
			rbt->bh++;
		node = node->left;
	}
	printf("total = %d\n"
		"nb = %d\n"
		"bh = %d\n", rbt->total, rbt->nb, rbt->bh);
	printinorder(rbt->root);
}

int main(int argc, char** argv)
{
	RBT* rbt = newRBT();
	int num;
	FILE *fp;
	fopen_s(&fp, "input.txt", "r");
	if (fp != NULL)
	{
		do
		{
			fscanf_s(fp, "%d", &num, 1);
			if (num > 0)
				insertNode(rbt, num);
			else if (num < 0)
				if (!deleteNode(rbt, -1 * num))
					printf("not found\n");
		} while (num != 0);

		print(rbt);
		fclose(fp);
	}
	return 0;
}
