
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
//creating a strucutre that has 4 elements to describe a person.
//its like a row in a database table
struct Person {
   char *name;
   int age;
   int height; 
   int weight;
};

//basically how oo programming is
//this only creates a local version and is deleted when main() ends
struct Person Person_crate(char *name, int age, int height, int weight){
   struct Person who;
   who.name = name;
   who.age = age;
   who.height = height;
   who.weight = weight;
   
   return who;

}

//this points to a person structure created in memory if called
struct Person *Person_create(char *name, int age, int height, int weight){

   //ask OS to give me raw piece of memory
   //malloc calculates the size of the structure and allocates the appropriate size of memory

   struct Person *who = malloc(sizeof(struct Person));
   //Assert checks if malloc returns NULL
   assert(who != NULL);


   //then we initialize all the elements of the newly created structure
   //strdup duplicates the string for the name to make sure that this 
   //structure actually owns it
   who->name = strdup(name);
   who->age = age;
   who->height = height;
   who->weight = weight;
   
   return who;
}

void Person_destroy(struct Person *who){

   assert(who != NULL);
   //if you do not free, then memory leak will happen
   free(who->name);
   free(who);

}

//-> structure dereference
void Person_print(struct Person *who){
   printf("Name: %s\n", who->name);
   printf("\tAge: %d\n", who->age);
   printf("\tHeight: %d\n", who->height);
   printf("\tWeight: %d\n", who->weight);

}

int main(int argc, char* argv[]){
//basically creates a structure called person we declared and 
//initialized the following with the appropriate parameters
struct Person *bob = Person_create("Bob Alex", 32, 150, 190);
struct Person *joe = Person_create("Joe Allen", 22, 140, 160);
struct Person alice;
struct Person july = Person_crate("Julie jules", 33, 131, 11);
struct Person jojo;
Person_print(&jojo);
Person_print(&july);
printf("Name: %s\n\tAge: %d\n\tHeight: %d\n\tWeight: %d\n", july.name,july.age,july.height,july.weight);
alice.name = "Alice Jenkins";
alice.age = 44;
alice.height = 150;
alice.weight = 100;

printf("Name: %s\n\tAge: %d\n\tHeight: %d\n\tWeight: %d\n",alice.name, alice.age,alice.height,alice.weight); 

printf("Joe is in memory location: %p\n", joe);
Person_print(joe);
printf("Bob is in memory location: %p\n", bob);
Person_print(bob);

joe->age += 20;
joe->height -= 2;
joe->weight += 40;
Person_print(joe);

bob->age += 20;
bob->weight += 20;
Person_print(bob);

Person_destroy(joe);
Person_destroy(bob);


return 0;
}
