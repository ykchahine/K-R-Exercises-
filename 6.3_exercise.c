#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 10           /* max # of lines to count for each word*/

struct tnode {
	char *word;               /* pointer to the text */
	struct tnode *left;       /* left child */
	struct tnode *right;      /* right child */
	unsigned lines[MAXLINES]; /* line numbes */
};

struct tnode *addtree(struct tnode *, char *, int);
void treeprint(struct tnode *);
char *getword();
int isnoise(char *);

/* wf: word frequency count */
int main(void)
{
	struct tnode *root;
	char *word;
	unsigned int line = 1;

	root = NULL;
	while ((word = getword()) != NULL)
		if (isalpha(word[0]) && !isnoise(word))
			root = addtree(root, word, line);
		else if (word[0] == '\n')
			line++;
	treeprint(root);
	return 0;
}

/* isnoise: test if word is noise word */
int isnoise(char * word)
{
	static char *nw[] = {
		"a",
		"an",
		"and",
		"are",
		"for",
		"from",
		"in",
		"is",
		"it",
		"not",
		"of",
		"on",
		"or",
		"that",
		"the",
		"this",
		"to",
		"was",
		"with",
	};

	int cond;
	int low, high, mid;

	if (word[1] == '\0')
		return 1;
	low = 0;
	high = sizeof(nw) / sizeof(char *) - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(word, nw[mid])) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return 1;
	}
	return 0;
}

/* addtree: add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w, int l)
{
	int cond;
	int i;

	/* if a new word has arrived, make a new node */
	if (p == NULL) {
		p = (struct tnode *) malloc(sizeof(struct tnode));
		p->word = strdup(w);
		p->lines[0] = l;
		for (i = 1; i < MAXLINES; i++)
			p->lines[i] = 0;
		p->left = p->right = NULL;
	}
	/* repeated word */
	else if ((cond = strcmp(w, p->word)) == 0) {
		for (i = 0; p->lines[i] && i < MAXLINES; i++);
			;
		p->lines[i] = l;
	}
	/* less than into left subtree */
	else if (cond < 0)
		p->left = addtree(p->left, w, l);
	/* greater than into right subtree */
	else if (cond > 0)
		p->right = addtree(p->right, w, l);
	return p;
}

/* treeprint: in-order print of tree p */
void treeprint(struct tnode *p)
{
	int i;

	if (p != NULL) {
		treeprint(p->left);
		printf("%-16s", p->word);
		for (i = 0; i < MAXLINES && p->lines[i]; i++)
			printf("%s%d", i==0 ? "" : ", ", p->lines[i]);
		putchar('\n');
		treeprint(p->right);
	}
}

/* getword: get next word or character from input */
char *getword()
{
	static char word[100];
	int c, lim = 100;
	char *w = word;

	while (isspace(c = getch()) && c != '\n')
		;
	if (c != EOF)
		*w++ = tolower(c);
	if (isalpha(c)) {
		for ( ; --lim > 1; w++)
			if (!isalnum(*w = tolower(getch())) && *w != '_') {
				ungetch(*w);
				break;
			}
	}
	*w = '\0';
	if (c == EOF)
		return NULL;
	else
		return word;
}
