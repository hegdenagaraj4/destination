#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "debug.h"

#define FILENAME "/home/nag/Documents/topology.txt" /*path to the input file*/
#define INFINITE 9999999

typedef struct _adjtab_
{
    int *ptr;
    int count;
}AdjTab;

typedef struct _adjmat_
{
    int no_of_vertices;
    int **matrix;
}AdjMat;

int *parent;
/*Begin input_network_topology*/
void input_network_topology(AdjTab * matrix)
{
    FILE *fp; /*file descriptor for the input file*/
    char ch;
    int count=0;
    // long length=0;
    log_entry();
    fp = fopen(FILENAME,"r");
    // fseek(fp, 0L, SEEK_END);
    // length =ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    while((ch= fgetc(fp))!=EOF)
    {
        myprint("%c",ch);
        if(!isspace(ch) && ch!='-')
        {
            matrix->count++;
        }
    }
    myprint("\n");
    print("%d",matrix->count);
    matrix->ptr = malloc(sizeof(int) * matrix->count);
    count =0;
    fseek(fp, 0L, SEEK_SET);
    while((ch= fgetc(fp))!=EOF)
    {
        if(!isspace(ch))
        {
            if(ch=='-')
            {
                char c;
                c = fgetc(fp);
                matrix->ptr[count] = (-1)*(c -'0');
            }
            else
            {
                matrix->ptr[count] = ch-'0';
            }
            count++;
        }
    }
    log_exit();
    return ;
} /*End input_network_topology()*/

/*Begin convert_to_two_d_array*/
void convert_to_two_d_array(AdjTab * new_matrix, AdjMat *adj_mat)
{
    int length=0,i=0,j=0,new_count=0;
    log_entry();
    length = sqrt(new_matrix->count);
    adj_mat->no_of_vertices = length;
    adj_mat->matrix = (int **)malloc(sizeof(int)*(length));
    for(i=0;i<length;i++)
    {
        adj_mat->matrix[i] = (int *)malloc(sizeof(int)*length);
    }

    for(i=0;i<length;i++)
    {
        for(j=0;j<length;j++)
        {
            if(new_count < new_matrix->count)
                adj_mat->matrix[i][j] = new_matrix->ptr[new_count];
            new_count++;
        }
    }
    for(i=0;i<length;i++)
    {
        for(j=0;j<length;j++)
        {
            myprint("%d ",adj_mat->matrix[i][j] );
        }
        myprint("\n");
    }
    log_exit();
    return ;
} /*End convert_to_two_d_array*/

int minimum_distance(int num_vertices,int d[], bool shortest_path[])
{
    int min = INFINITE;
    int min_index ;
    int i=0;
    log_entry();
    for(i=0;i<num_vertices;i++)
    {
        if(shortest_path[i]==false && (d[i]>=min  ))
        {
            min= d[i] ;
            min_index = i;
        }
    }
    log_exit("retval %d",min_index+1);
    return min_index;
}


int printSolution(int d[], int n, int src)
{
    int i;
    log_entry();

    print("Parent       Child");
    for(i=0;i<n;i++)
    {
        print("parent of %d\t\t%d",i+1 , parent[i]);
    }

    print("Router %d Connection Table", src);
    print("Destination      Cost");
    for(i=0;i<n;i++)
    {
        print("%d\t\t%d",i+1 , d[i]);
    }
    log_exit();
    return 0;
}


char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

void shortest_path_tree( AdjMat *adjMat,int source,int target)
{
    int num_vertices=0;
    int dist[adjMat->no_of_vertices];
    int prev_dist=0;

    bool shortest_path[adjMat->no_of_vertices];
    int i=0,min_dist_vertex,j=0;
    int start, dest;
    int v ;
    log_entry();
    parent = malloc(num_vertices*sizeof(int));
    start = source -1;
    dest  = target -1;
    num_vertices = adjMat->no_of_vertices;
    for(i=0;i<num_vertices;i++)
    {
        dist[i] = INFINITE;
        shortest_path[i] = false;
    }
    v = start;
    dist[start] =0;
    while(shortest_path[v]==false)
    {
        // min_dist_vertex = minimum_distance(num_vertices,dist,shortest_path);
        shortest_path[v] =true;
        for(j=0;j<num_vertices;j++)
        {
            if(dist[j] > dist[v]+adjMat->matrix[v][j])
            {
                dist[j] = dist[v]+adjMat->matrix[v][j];
                parent[j] = v;
            }
            // if(!shortest_path[j]
            //     && adjMat->matrix[min_dist_vertex][j])
            //     // && dist[min_dist_vertex] != INFINITE
            //     // && dist[min_dist_vertex] + adjMat->matrix[min_dist_vertex][j] <= dist[j] )
            // {
            //     dist[j] = dist[min_dist_vertex] + adjMat->matrix[min_dist_vertex][j];

            //     parent[j] = min_dist_vertex;
            //     myprint("***I am here for min_dist_vertex and minimum_distance of %d*\n",min_dist_vertex,dist[min_dist_vertex]);
            // }
            // prev_dist = dist[j];
        }

        v=1;
        prev_dist = INFINITE;
        for(i=0;i<num_vertices;i++)
        {
            if((shortest_path[i]==false)&&(prev_dist>dist[i]))
            {
                prev_dist =dist[i];
                v =i;
            }
        }

    }

    printSolution(dist,num_vertices,source);
    log_exit();
    return;
}



/*

void shortest_path_tree(int source, AdjMat *adjMat)
{
    int num_vertices=0;
    int dist[adjMat->no_of_vertices];
    bool shortest_path[adjMat->no_of_vertices];
    int i=0,min_dist_vertex,j=0;

    log_entry();
    num_vertices = adjMat->no_of_vertices;
    for(i=0;i<num_vertices;i++)
    {
        dist[i] = INFINITE;
        shortest_path[i] = false;
    }
    dist[source] =0;
    for(i=0;i<num_vertices-1;i++)
    {
        min_dist_vertex = minimum_distance(num_vertices,dist,shortest_path);

        shortest_path[min_dist_vertex] =true;
        for(j=0;j<num_vertices;j++)
        {
            if(!shortest_path[j]
                && adjMat->matrix[min_dist_vertex][j]
                && dist[min_dist_vertex] != INFINITE
                && dist[min_dist_vertex] + adjMat->matrix[min_dist_vertex][j] < dist[j] )
            {
                dist[j] = dist[min_dist_vertex] + adjMat->matrix[min_dist_vertex][j];
            }
        }
    }
    // printSolution(d,num_vertices,source);
    log_exit();
    return;
}

*/
/*Begin Main*/
int main(int argc, char const *argv[])
{
    /* code */
    int choice,src,dest; /*choice value */
    AdjTab *new_matrix;
    AdjMat adj_mat;

    new_matrix = malloc(sizeof(AdjTab));
    while(1)
    {
        print("CS542 Link State Routing Simulator");
        myprint("(1) Input Network Topology File\n(2) Build a Connection Table\n(3) Shortest Path to Destination Router\n(4) Exit\n");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
                /*calling a function to parse the input file.*/
                input_network_topology(new_matrix);
                convert_to_two_d_array(new_matrix,&adj_mat);
            break;

            case 2:
                    print("Select source");
                    scanf("%d",&src);

            break;

            case 3:
                    print("Select destination");
                    scanf("%d",&dest);
                    shortest_path_tree(&adj_mat,src,dest);
                    // print("%d",dijsktra(&adj_mat,src,dest));
            break;

            case 4:
                debug("*****************EXITING THE PROJECT****************");
                exit(0);
            break;

            case 5:
            break;

            default:
            break;
        }
    }
    return 0;
} /*End Main*/