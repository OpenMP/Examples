/*
* @@name:	udr.3.c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_4.0
*/

#include <stdio.h>
#define N 100

struct mx_s {
   float value;
   int index;
};

/* prototype functions for combiner and initializer in
   the declare reduction */
void mx_combine(struct mx_s *out, struct mx_s *in);
void mx_init(struct mx_s *priv, struct mx_s *orig);

#pragma omp declare reduction(maxloc: struct mx_s: \
        mx_combine(&omp_out, &omp_in)) \
        initializer(mx_init(&omp_priv, &omp_orig))

void mx_combine(struct mx_s *out, struct mx_s *in)
{
   if ( out->value < in->value ) {
      out->value = in->value;
      out->index = in->index;
   }
}

void mx_init(struct mx_s *priv, struct mx_s *orig)
{
   priv->value = orig->value;
   priv->index = orig->index;
}

int main(void)
{
   struct mx_s mx;
   float val[N], d;
   int i, count = N;

   for (i = 0; i < count; i++) {
      d = (N*0.8f - i);
      val[i] = N * N - d * d;
   }

   mx.value = val[0];
   mx.index = 0;
   #pragma omp parallel for reduction(maxloc: mx)
   for (i = 1; i < count; i++) {
      if (mx.value < val[i]) 
      {
         mx.value = val[i];
         mx.index = i;
      }
   }

   printf("max value = %g, index = %d\n", mx.value, mx.index);
   /* prints 10000, 80 */

   return 0;
}
