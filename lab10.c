//Dominick Tulshi
//Dr. Neslisah Torosdagli
//4/18/2024
//COP 3502

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_LENGTH 26

// Trie structure
struct TrieNode{
    struct TrieNode* children[ALPHABET_LENGTH];
    bool isEndOfWord;
    int count;
};
struct Trie
{
    struct TrieNode* root;
};

// Inserts the word to the trie structure
struct TrieNode* createTrieNode();

void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode* curr = pTrie->root;
    for(int i = 0; word[i] != '\0'; i++){
        int index = word[i] - 'a';
        if(!curr->children[index]){
            curr->children[index] = createTrieNode();
        }
        curr = curr->children[index];
    }
    curr->isEndOfWord = true;
    curr->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct TrieNode* curr = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++){
        int index = word[i] - 'a';
        if(!curr->children[index]){
            return 0;
        }
        curr = curr->children[index];
    }
    return(curr != NULL && curr->isEndOfWord) ? curr->count : 0;
}

// deallocate the trie structure
void deallocateTrieNode(struct TrieNode* node){
    if(!node){
        return;
    }
    for(int i = 0; i < ALPHABET_LENGTH; i++){
        if(node->children[i]){
            deallocateTrieNode(node->children[i]);
        }
    }
    free(node);
}
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if(!pTrie){
        return NULL;
    }
    deallocateTrieNode(pTrie->root);
    free(pTrie);
    return NULL;
}

// Initializes a trie structure
struct TrieNode* createTrieNode(){
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if(newNode){
        newNode->isEndOfWord = false;
        newNode->count = 0;
        for (int i = 0; i < ALPHABET_LENGTH; i++){
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}
struct Trie *createTrie()
{
    struct Trie* trie = (struct Trie*)malloc(sizeof(struct Trie));
    if(trie){
        trie->root = createTrieNode();
    }
    return trie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE* fp = fopen(filename, "r");
    if(fp == NULL){
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }
    int numWords = 0;
    char word[100];
    while(fscanf(fp, "%s", word) != EOF){
        pInWords[numWords] = strdup(word);
        numWords++;
    }
    fclose(fp);
    return numWords;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}