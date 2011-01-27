#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define INPUT_HEADER "n=%d timestep=%lf iters=%d "
#define INPUT_BODY   "p=( %lf , %lf , %lf ) v=( %lf , %lf , %lf ) m= %lf "
#define OUTPUT_BODY  "p=(% 1.4e,% 1.4e,% 1.4e) v=(% 1.4e,% 1.4e,% 1.4e) m=% 1.4e\n"

 typedef struct
  {
    double position[3];
    double velocity[3];
    int mass;
  } PLANET;

void populatefromfile();
void Build_derived_type(PLANET* indata, MPI_Datatype* message_type_ptr);
void printPlanet(PLANET* planet);
 
int main(int argc, char *argv[])
{
  int rank, p, n = 0;
  double startTime, endTime;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  PLANET dataType;
  MPI_Datatype TYPE_MSG;
  Build_derived_type(&dataType, &TYPE_MSG);

  // scan input with proc 0
  if (rank == 0)
    populatefromfile();


  PLANET planet; // = malloc(sizeof(PLANET));
  if (rank == 0)
    {
      planet.position[0] = 3;
      planet.position[1] = 2;
      planet.position[2] = 1;
      //      printPlanet(&planet);
    }

  MPI_Bcast(&planet, 1, TYPE_MSG, 0, MPI_COMM_WORLD);

  printf("rank:\n%d\n", rank); printPlanet(&planet);



  
  MPI_Finalize();
  return 0;
}

void printPlanet(PLANET* planet)
{
  printf("PLANET ***:\n");
  printf("Position:\n%lf %lf %lf\n", planet->position[0], planet->position[1], planet->position[2]);
}

 void Build_derived_type(PLANET* indata, MPI_Datatype* message_type_ptr) 
 { 
   int block_lengths[3]; 
   MPI_Aint displacements[3]; 
   MPI_Aint addresses[4]; 
   MPI_Datatype typelist[3]; 

   typelist[0] = MPI_DOUBLE; 
   typelist[1] = MPI_DOUBLE; 
   typelist[2] = MPI_INT; 
   // Number of elements of each type 
   block_lengths[0]= 3; 
   block_lengths[1]= 3; 
   block_lengths[2]= 1; 

   MPI_Address(indata, &addresses[0]); 
   MPI_Address(&(indata->position), &addresses[1]); 
   MPI_Address(&(indata->velocity), &addresses[2]); 
   MPI_Address(&(indata->mass), &addresses[3]); 
   displacements[0] = addresses[1] - addresses[0]; 
   displacements[1] = addresses[2] - addresses[0]; 
   displacements[2] = addresses[3] - addresses[0]; 

   /* Create the derived type */ 
   MPI_Type_struct(3, block_lengths, displacements, typelist, message_type_ptr); 

   /* Commit it so that it can be used */ 
   MPI_Type_commit(message_type_ptr);   
 }

void populatefromfile()
{
  int result, i;
 
  int n, iters;
  double timestep;
 
  result = scanf(INPUT_HEADER, &n, &timestep, &iters);
  if (result != 3) { /* scanf() returns the number of elements it reads successfully */
    fprintf(stderr, "error reading header\n");
    exit(0);
  }
 
  /* you should allocate space for the bodies here
     now that you know how many bodies there will be */
 
  for (i = 0; i < n; i++) {
    double x, y, z, vx, vy, vz, m;
 
    result = scanf(INPUT_BODY, &x, &y, &z, &vx, &vy, &vz, &m);
    if (result != 7) {
      fprintf(stderr, "error reading body %d\n", i);
      exit(0);
    }
    /* add code here to add each body to your internal data structures */
    fprintf(stderr, OUTPUT_BODY, x, y, z, vx, vy, vz, m);
  }
 
  fprintf(stderr, "got n=%d timestep=%f iters=%d\n", n, timestep, iters);
}
