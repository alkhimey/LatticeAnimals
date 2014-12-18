#include <time.h>
#include <stdio.h>
#include <strings.h>

#define MAX_SIZE  16
#define X_SIZE    (2 * MAX_SIZE - 2)
#define FULL_SIZE (X_SIZE * MAX_SIZE)

long long count [MAX_SIZE + 1];

#define SET_BOARD(_pos, _value)    board [_pos] = (_value)
#define CHECK_BOARD(_pos, _value)  board [_pos] == (_value)
#define GET_SUCC(_pos1, _pos2)     _pos2 = untried [_pos1]
#define SET_SUCC(_pos1, _pos2)     untried [_pos1] = _pos2

typedef struct {
   int n_neis;
   int neis [4];
} neighbors_t;

typedef enum {
   FREE, OCCUPIED, REACHABLE
} cell_t;
cell_t      board    [FULL_SIZE];
int         untried  [FULL_SIZE];
neighbors_t all_neis [FULL_SIZE];

int size = 0;
int n_calls = 0;
int n_saves = 0;

void fixed (
   int hdr
);

#define MAX_STACK 1000
int stack [3*MAX_STACK];
int p_stack = 0;
#define PUSH(_a, _b, _c) { \
   stack [p_stack] = _a; \
   stack [p_stack + 1] = _b; \
   stack [p_stack + 2] = _c; \
   p_stack += 3; \
   /* if (p_stack == MAX_STACK) {printf ("G@vald!\n"); exit (0);} */ \
}
#define POP(_a, _b, _c) { \
   _c = stack [p_stack - 1]; \
   _b = stack [p_stack - 2]; \
   _a = stack [p_stack - 3]; \
   p_stack -= 3; \
}

clock_t clk0;
double ptime;

int main (
   int   argc,
   char* argv []
) {

   int i, j, p, hdr;
   char fn [64];
   FILE* fp;

   /* [Warm restart] */
   if (argc >= 2)
      if (strncmp (argv [1], "-h", 2) == 0) {
         printf ("-h: help\n");
         printf ("-f<file>: continue from data in <file>\n");
         printf ("-p<file>: print data in <file>\n");
         printf ("-n: set MAX_SIZE (nyi)\n");
         exit (1);
      } else if (strncmp (argv [1], "-f", 2) == 0 ||
                 strncmp (argv [1], "-p", 2) == 0) {
         FILE* fp = fopen (argv [1] + 2, "rb");
         if (fp == NULL) {
            printf ("fixed: can't open file %s\n", argv [1] + 2);
            exit (0);
         }
         fread (& i, sizeof (int), 1, fp);
         if (i != MAX_SIZE) {
            printf ("G@VALD!!!! MAX_SIZE: expected=%d found=%d\n",
                    MAX_SIZE, i);
            exit(0);
         }
         fread (& size, sizeof (int), 1, fp);
         fread (& n_saves, sizeof (int), 1, fp);
         fread (count, sizeof (long long), MAX_SIZE + 1, fp);
         fread (board, sizeof (cell_t), FULL_SIZE, fp);
         fread (untried, sizeof (int), FULL_SIZE, fp);
         fread (all_neis, sizeof (neighbors_t), FULL_SIZE, fp);
         fread (& hdr, sizeof (int), 1, fp);
         fread (& p_stack, sizeof (int), 1, fp);
         fread (stack, sizeof (int), p_stack, fp);
         fread (& ptime, sizeof (double), 1, fp);
         fclose (fp);
printf("red: MS=%d sz=%d ns=%d hdr=%d pstk=%d ptime=%f\n",
 i,size,n_saves,hdr,p_stack,ptime);
         printf ("fixed: loaded file %s, MAX_SIZE = %d, n_saves = %d\n",
                 argv [1] + 2, MAX_SIZE, n_saves);
         if (strncmp (argv [1], "-p", 2) == 0) {
            printf ("Temporary count:\n");
            for (p = 1; p <= MAX_SIZE; p ++)
               printf ("Fixed (%d) = %lld\n", p, count [p]);
            exit (1);
         }
         clk0 = clock ();
         goto WARM_RESTART;
      } else if (strncmp (argv [1], "-n", 2) == 0) {
      } else {
         printf ("fixed: unknown option %s\n", argv [1]);
         exit (0);
      }

   /* [Init] */
   ptime = 0.0;
   clk0 = clock ();
   bzero (count, sizeof (count));
   bzero (untried, sizeof (untried));
   bzero (all_neis, sizeof (all_neis));
   for (j = 0; j <= MAX_SIZE - 2; j ++)
      for (i = - MAX_SIZE + 3; i <= MAX_SIZE - 2; i ++) {
         if (j == 0 && i < 0)
            continue;
         p = j * X_SIZE + i + MAX_SIZE - 2;
         all_neis [p].n_neis = 2;
         all_neis [p].neis [0] = p + 1;
         all_neis [p].neis [1] = p + X_SIZE;
         if (j > 0 || (j == 0 && i > 0))
            all_neis [p].neis [all_neis [p].n_neis ++] = p - 1;
         if (j > 1 || (j == 1 && i >= 0))
            all_neis [p].neis [all_neis [p].n_neis ++] = p - X_SIZE;
      }

   /* The algorithm is started with the parent being the empty polyomino, */
   bzero (board, sizeof (board));
   /* and the untried set containing only the origin. */
   hdr = MAX_SIZE - 2;
   WARM_RESTART:
   fixed (hdr);

   /* [Print count] */
   sprintf (fn, "n-fixed-%d-final", MAX_SIZE);
   fp = fopen (fn, "wb");
   if (fp) {
      for (p = 1; p <= MAX_SIZE; p ++)
         fprintf (fp, "Fixed (%d) = %lld\n", p, count [p]);

      ptime += (double) (clock () - clk0) / CLOCKS_PER_SEC;
      fprintf (fp,
              "\nTime: %5.2f seconds, %5.2f minutes, %5.2f hours, %5.2f days\n",
               ptime, ptime / 60.0, ptime / 3600.0, ptime / 86400.0);
      fclose (fp);
   }
}

void fixed (
   int hdr
) {

   /* [Local variables] */
   register int cur, save_hdr, n, i;
   int* p;

   RESTART:

   n_calls ++;
   if (n_calls == 1000000000) {
      char fn [64];
      FILE* fp;

      n_saves ++;
      sprintf (fn, "n-fixed-%d-%d", MAX_SIZE, n_saves);
      fp = fopen (fn, "wb");
      if (fp) {
         int ms;
         ptime += (double) (clock () - clk0) / CLOCKS_PER_SEC;
         ms = MAX_SIZE;
         fwrite (& ms, sizeof (int), 1, fp);
         fwrite (& size, sizeof (int), 1, fp);
         fwrite (& n_saves, sizeof (int), 1, fp);
         fwrite (count, sizeof (long long), MAX_SIZE + 1, fp);
         fwrite (board, sizeof (cell_t), FULL_SIZE, fp);
         fwrite (untried, sizeof (int), FULL_SIZE, fp);
         fwrite (all_neis, sizeof (neighbors_t), FULL_SIZE, fp);
         fwrite (& hdr, sizeof (int), 1, fp);
         fwrite (& p_stack, sizeof (int), 1, fp);
         fwrite (stack, sizeof (int), p_stack, fp);
         fwrite (& ptime, sizeof (double), 1, fp);
         fclose (fp);
printf("wrt: MS=%d sz=%d ns=%d hdr=%d pstk=%d ptime=%f\n",
 i,size,n_saves,hdr,p_stack,ptime);
         printf ("fixed: saved file %s, MAX_SIZE = %d, n_saves = %d\n",
                 fn, MAX_SIZE, n_saves);
         /* exit (1); */
         n_calls = 0;
         clk0 = clock ();
      }
   }

   /* The following steps are repeated until the untried set is exhausted. */
   while (hdr) {

      /* 1. Remove an arbitrary element from the untried set. */
      cur = hdr;
      GET_SUCC (cur, hdr);

      /* 2. Place a cell at this point. */
      SET_BOARD (cur, OCCUPIED);

      /* 3. Count this new polyomino. */
      count [++ size] ++;

      /* 4. If the size is less than P: */
      if (size < MAX_SIZE) {

         /* (a) Add new neighbors to the untried set. */
         save_hdr = hdr;
         for (i = 0, p = all_neis [cur].neis;
              i < all_neis [cur].n_neis; i ++) {
            n = * p ++;
            if (CHECK_BOARD (n, FREE)) {
               SET_SUCC (n, hdr);
               hdr = n;
               SET_BOARD (n, REACHABLE);
            }
         }

         /* (b) Call this algorithm recursively with the new parent being   */
         /*     the current polyomino, and the new untried set being a copy */
         /*     of the current one.                                         */
         PUSH (hdr, save_hdr, cur);
         goto RESTART;
         RESUME:

         /* (c) Remove the new neighbors from the untried set. */
         while (hdr != save_hdr) {
            SET_BOARD (hdr, FREE);
            GET_SUCC (hdr, hdr);
         }
      }

      /* 5. Remove newest cell. */
      size --;
   }

   if (p_stack) {
      POP (hdr, save_hdr, cur);
      goto RESUME;
   }
}
