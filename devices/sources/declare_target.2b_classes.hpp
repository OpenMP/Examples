 #pragma omp begin declare target
 class XOR1
 {    
     int a;
   public:
     XOR1(int arg): a(arg) {};
     int foo();
 };
 #pragma omp end declare target
