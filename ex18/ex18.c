#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

//set constants
#define MAX_DATA 512
#define MAX_ROWS 100

//structure named address with 2 int and char[] set with a limit of 512 bytes
struct Address{
   int id;
   int set;
   char name[MAX_DATA];
   char email[MAX_DATA];
};

//Database structure containing an array of Address structures of size 100 addresses
struct Database {
   struct Address rows[MAX_ROWS];
};

//Connection structure containing a file pointer and a pointer to a database
//structure with 100 structures of Addresses
struct Connection{
   FILE *file;
   struct Database *db;

};


//Our error message function
void die(const char *message){
   if(errno){
      perror(message);
   }
   else
      printf("Error: %s\n", message);

   exit(1);
}

//prints out elements contained in the pointer to an address structure
void Address_print(struct Address *address){
   printf("%d %s %s\n", address->id, address->name, address->email);
}


//rc returns  the total number of elements, in our case, 1 if successful
//fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
//ptr is the pointer to a block of memory with a minimum size of nmemb bytes
//size is the size of byes of each element to be read.
//nmemb is the number of elements,each one with the size of *size* bytes
//stream is a pointer to a file object that specifies an input stream
void Database_load(struct Connection *conn){
   int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
   if(rc != 1){
      die("Failed to load database");
   }
}


//returns a connection structure
struct Connection *Database_open(const char *filename, char mode){
   //conn is a pointer to a connection structure with an allocated size of a
   //connection structure
   struct Connection *conn = malloc(sizeof(struct Connection));
   //check if conn is null or not
   if(!conn)
      die("Memory error");

   //create a chunk of memory of the size of a Database struct to connection
   conn->db = malloc(sizeof(struct Database));
   //check if allocation of memory is successful or not
   if(!conn->db){
      die("Memory error");
   //if mode c, we write to file
   if(mode == 'c'){
      conn->file = fopen(filename, "w");
   }
   //fopen r+ opens a text file for update(for both reading and writing
   //r opens it for reading the file
   //w opens a file for writing, truncating an existing file to 0 length, or
   //creates a file if it does not exist
   //w+ opens a test file for update, same as w+
   else{
      conn->file = fopen(filename, "r+");


      if(conn->file){
         Database_load(conn);
      }

   }
   //failed to open file
   if(!conn->file){
      die("Failed to open the file");
       }
   }
   return conn;
}

//close db
void Database_close(struct Connection *conn){
   if(conn){
      if(conn->file)
         fclose(conn->file);
      if(conn->db)
         free(conn->db);
      free(conn);
   }
}


void Database_write(struct Connection *conn){
   //sets file position to the beginning of the file given filestream
   rewind(conn->file);

   //writes data from the array pointed to by ptr to the given stream
   int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
   if(rc != 1)
      die("Failed to write database.");
   //flushes output buffer of a stream
   //return 0 if successful else -1
   rc = fflush(conn->file);
   if(rc == -1){
      die("Cannot flush database.");
   }
}

//creates a database of MAX_ROWS(ie: 100) rows all unset with id 1-100
// and sets it to the db structure with an array of 100 address structures
void Database_create(struct Connection *conn){
   int i = 0;

   for(i = 0; i < MAX_ROWS; i++){
      //make prototype to initialize it
      struct Address addr ={.id = i, .set = 0 };
      //then assign it
      conn->db->rows[i] = addr;
   }
}


//set db elements
void Database_set(struct Connection *conn, int id, const char *name, const char *email){
   //points to an structure address in a database
   struct Address *addr = &conn->db->rows[id];
   //if the following connection structure has a database with an address
   // of id, and is set, print out error message
   if(addr->set){
      die("Already set, delete it first");
   }

   //else set it to 1 and copy the parameters to the following address
   addr->set = 1;

   char *res = strncpy(addr->name, name, MAX_DATA);
   // demonstrate the strncpy bug
   if(!res)
      die("Name copy failed");

   res = strncpy(addr->email, email, MAX_DATA);
   if(!res)
      die("Email copy failed");
}

//prints the id in the db
void Database_get(struct Connection *conn, int id){
   struct Address *addr = &conn->db->rows[id];
   if(addr->set){
      Address_print(addr);
   }
   else{
      die("ID is not set yet");
   }
}

//delete a tuple from the database
void Database_delete(struct Connection *conn, int id){
   //create new address struct with no parameters except id and make it unset
   struct Address addr = {.id = id, .set = 0 };
   conn->db->rows[id] = addr;
}

//lists all elements in the db
void Database_list(struct Connection *conn){
   int i = 0;
   struct Database *db = conn->db;

   for(i = 0; i < MAX_ROWS; i++){
      struct Address *current = &db->rows[i];

      if(current->set){
         Address_print(current);
      }
   }
}


int main(int argc, char *argv[]){
   if(argc < 3)
      die("USAGE: ex18 <dbfile> <action> [action params]");

   char *filename = argv[1];
   char action = argv[2][0];
   struct Connection *conn = Database_open(filename, action);
   int id = 0;

   if(argc > 3)
      id= atoi(argv[3]);
   if(id >= MAX_ROWS)
      die("There's not that many records.");

   switch(action){
      case 'c':
         Database_create(conn);
         Database_write(conn);
         break;
      case 'g':
         if(argc != 4)
            die("Need an id to get");
         Database_get(conn, id);
         break;
      case 's':
         if(argc != 6)
            die("Need id, name, email to set");

         Database_set(conn, id, argv[4], argv[5]);
         Database_write(conn);
         break;
      case 'd':
         if(argc != 4)
            die("Need id to delete");

         Database_delete(conn, id);
         Database_write(conn);
         break;
      case 'l':
         Database_list(conn);
         break;
      default:
         die("Invalid action: c=create, g=get, s=set, d=delete, l=list");

   }

   Database_close(conn);

   return 0;
   }
