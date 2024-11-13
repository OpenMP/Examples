/*
* @@name:	udr.5
* @@type:	C++
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
class V {
   float *p;
   int n;

public:
   V( int _n )     : n(_n)  { p = new float[n]; }
   V( const V& m ) : n(m.n) { p = new float[n]; } 
   ~V() { delete[] p; }
    
   V& operator+= ( const V& );
    
   #pragma omp declare reduction( + : V ) combiner( omp_out += omp_in ) \
           initializer(omp_priv(omp_orig))
};
