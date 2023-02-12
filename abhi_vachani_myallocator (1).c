#include<stdio.h>
#include<stddef.h>


/*

Abhi Vachani
Dynamic Memory Allocator

*/


char mymemory[8000];

struct block{
 size_t size;
 int free;
 struct block *next; 
};

typedef struct block block;

 block *list=(void*)mymemory;


void initmemory()
{
  
  /*
  the size of the list should be defined as: the total memory size – size of your metadata block
  List should be initialized as free and the next pointer should point to NULL.
  */


  // Initialize the list size
  list->size = sizeof(mymemory) - sizeof(block);      

  // Init free
  list->free = 1; 

  // Init next
  list->next = NULL;


}

void split(block *slotToBeSplitted,size_t size)
{
  // Split the slot to be splitted and create a new slot 

  // New Slot
  block *newSlot;
  newSlot->size = slotToBeSplitted->size - size - sizeof(struct block);
  newSlot->next = slotToBeSplitted->next;
  newSlot->free = 1;

  // Slot to be Splitted
  slotToBeSplitted->next = newSlot;
  slotToBeSplitted->size = slotToBeSplitted->size - size - sizeof(block);
  slotToBeSplitted->free = 0;



}


void *NewMalloc(size_t allocsSize)
{
// NewMalloc: allocates memory with allocsSize

 block *curr;
 void *result;
 curr=list;
 

 //Find a chunk with enough free space
 while(curr->free != 1 && curr->size < allocsSize)
 {
  curr = curr->next;
 }


 // Print curr size
 printf("curr size is: %d\n\n", curr->size);



 // No split needed
 if (allocsSize == curr->size)
 {
  result = curr;
  curr->free = 0;
  printf("Allocate block without splitting for: %d bytes\n", allocsSize);
  
 }

 // Split Needed 
 else if (allocsSize < curr->size)
 {
  split(curr, allocsSize);
  result = curr;
  printf("Allocate block with splitting for: %d bytes\n", allocsSize);
 }
 

 // Error
 else 
 {
  result = NULL;
  printf("No sufficient memory to allocate %d bytes\n", allocsSize);
  
}


return result;
 
 
}



void coalesce(){
 struct block *curr;
 curr=list;

 
 // Coalesce consequtive blocks that are free
 while (curr->free == 1 && curr->next->free == 1)
 {
  curr->size = curr->size + curr->next->size - sizeof(block);
  curr->next = curr->next->next;
  
    

 }

 // If consequtives are not free, continue as normal
 curr = curr->next;

}




void NewFree(void* ptr){

  // Check if ptr is valid and free if so

  // Turn ptr to a block
  block* Free = (block*)ptr;

  // Valid (not free'd)
  if (Free->free == 0)
  {
    Free->free = 1;
    coalesce(); 
  }

  // Invalid (already free)
  else
    printf("Invalid ptr as this is already free!\n");


}



int main(){
 
 if(!(list->size))
 {
  initmemory();
  printf("\n\n--Memory Initialization Completed--\n");
  printf("Initial size is: %d\n\n", list->size);
 }


 int *p=(int*)NewMalloc(800);
 printf("\n");
 char *q=(char*)NewMalloc(250*sizeof(char));
 printf("\n");
 int *r=(int*)NewMalloc(1000*sizeof(int));
 printf("\n");
 NewFree(p);
 printf("\n");
 char *w=(char*)NewMalloc(1000);
 printf("\n");
 NewFree(r);
 printf("\n");
 int *k=(int*)NewMalloc(1500*sizeof(int));
 printf("\n");
 

}
