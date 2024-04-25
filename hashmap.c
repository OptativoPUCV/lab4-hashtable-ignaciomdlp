#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  if (map == NULL){
    exit(EXIT_FAILURE);
  }

  long index = hash(key, map->capacity) % map->capacity;

  while (map->buckets[index] != NULL && map->buckets[index]->key != NULL && strcmp(map->buckets[index]->key, key) != 0){
    index = (index + 1) % map->capacity;
  }

  if (map->buckets[index] == NULL || map->buckets[index]->key == NULL){
    Pair *newPair = malloc(sizeof(Pair));
    if (newPair == NULL){
      exit(EXIT_FAILURE);
    }

    newPair->key = strdup(key);
    if (newPair->key == NULL){
      exit(EXIT_FAILURE);
    }

    newPair->value = value;
    map->buckets[index] = newPair;
    map->size++;
    map->current = index;
  }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
  if (capacity == 0){
    return NULL;
  }

  HashMap *map = malloc(sizeof(HashMap));
  if (map == NULL){
    exit(EXIT_FAILURE);
  }

  map->capacity = capacity;
  map->size = 0;
  map->buckets = malloc(sizeof(Pair *) * capacity);
  if (map->buckets == NULL){
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < capacity; i++){
    map->buckets[i] = NULL;
  }

  map->current = -1;

  return map;
}

void eraseMap(HashMap * map,  char * key) {    
  if (map == NULL){
    exit(EXIT_FAILURE);
  }

  Pair *pair = searchMap(map, key);

  if (pair != NULL){
    pair->key = NULL;
    map->size--;
  }
}

Pair * searchMap(HashMap * map,  char * key) {
  if (map == NULL){
    exit(EXIT_FAILURE);
  }

  long index = hash(key, map->capacity) % map->capacity;

  while(map->buckets[index] != NULL && (map->buckets[index]->key == NULL || strcmp(map->buckets[index]->key, key)) != 0){
    index = (index + 1) % map->capacity;
  }

  if (map->buckets[index] != NULL && (map->buckets[index]->key == NULL || strcmp(map->buckets[index]->key, key) == 0)){
    map->current = index;
    return map->buckets[index];
  }

  return NULL;
}

Pair * firstMap(HashMap * map) {
  if (map == NULL){
    exit(EXIT_FAILURE);
  }

  map->current = NULL;

  return nextMap(map);
}
/*
Pair * nextMap(HashMap * map) {
  if (map == NULL){
    exit(EXIT_FAILURE);
  }

  if (map->current == -1){
    for(int i = 0; i < map->capacity; i++){
      if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
        map->current = i;
        return map->buckets[i];
      }
    }
    return NULL;
  }
  else {
    int nextIndex = (map->current + 1) % map->capacity;

    while (nextIndex != map->current || (map->buckets[nextIndex] == NULL && map->buckets[nextIndex]->key == NULL)){
      nextIndex = (nextIndex + 1) % map->capacity;
    }
    map->current = -1;
    return NULL;
    
  }
}
*/

Pair * nextMap(HashMap * map) {
  if (map == NULL ) return NULL;
  long pos = ((map->current) + 1) % map->capacity; // controlar que no se salga del mapa
  while(pos < map->capacity && pos != 0)
    {
      if(map->buckets[pos] != NULL && map->buckets[pos]->key != NULL)
      {
        map->current = pos;
        return map->buckets[pos];
      }
      pos++;;
    }
  return NULL;
}
