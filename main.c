#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order;
};

// Fill out this structure
struct HashType
{
    struct RecordType** records;
    int size;
};

// Compute the hash function (Multiplicative Hashing)
int hash(int x, int hashSz)
{
    double A = 0.618033988749895;
    double scaledValue = x * A;
    double fractionalPart = scaledValue - (int)scaledValue;
    return (int)(hashSz * fractionalPart);
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType* pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);

        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }

        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        if (pHashArray->records[i] != NULL)
        {
            printf("index %d -> %d %c %d\n", i, pHashArray->records[i]->id, pHashArray->records[i]->name, pHashArray->records[i]->order);
        }
    }
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;
    int hashSz = 10; // Define the size of the hash table here.

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    struct HashType hashTable;
    hashTable.records = (struct RecordType**)malloc(sizeof(struct RecordType*) * hashSz);
    hashTable.size = hashSz;

    for (int i = 0; i < hashSz; i++)
    {
        hashTable.records[i] = NULL; // Initialize all hash table entries to NULL (empty).
    }

    for (int i = 0; i < recordSz; i++)
    {
        int index = hash(pRecords[i].id, hashSz); // Compute the hash value for the current record.
        while (hashTable.records[index] != NULL)
        {
            index = (index + 1) % hashSz; // Linear probing for collision resolution.
        }
        hashTable.records[index] = &pRecords[i]; // Insert the record into the hash table.
    }

    // Display the records in the hash table.
    displayRecordsInHash(&hashTable, hashSz);

    free(pRecords); // Don't forget to free the allocated memory.
    free(hashTable.records);
    return 0;
}
