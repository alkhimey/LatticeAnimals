#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NEIGH_FOR_3D 6  



#define MAX_NEIGH MAX_NEIGH_FOR_3D

typedef enum {
  FREE,
  OCCUPIED,
  REACHABLE
} CellState;

typedef struct cell_t {
  CellState state;
  struct cell_t* neigh[MAX_NEIGH]; 
  int neigh_size; // Actual number of neighbors
  struct cell_t* next_untried;
  int x,y,z; // 3D
} Cell;


void initialize_2d_grid(Cell grid[], int n);
//void free_grid();


int main(int argc, char* argv[]) {
  int n = atoi(argv[1]);
  Cell grid[n*n*n];
  printf("r\n");
  initialize_2d_grid(grid, n);

  
  


  return 1;
}


void initialize_2d_grid(Cell grid[], int n) {
  memset((void*)grid, 0, n*n*n*sizeof(Cell));
  
  int i,j,k;
  for(i=0; i<n-1; i++) {
    for(j=0; j<n; j++) {
      for(k=0; k<n; k++) {
	printf("d");
      }
    }
  }

  

}
