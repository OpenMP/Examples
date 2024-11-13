/*
* @@name:	induction.2
* @@type:	C++
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
#include <cmath>

class Point {
  float x, y, m;
  char color;
public:
  Point(float x, float y, float m) : x(x), y(y), m(m) {
    color = (int)(x+y) % 256;
  }
  Point nextPoint(float distance) {
    // return a Point that is 'distance' away along slope m
    //   in the x direction
    float deltaX = distance/(sqrtf(1.0f + m * m));
    float deltaY = m * deltaX;
    Point NewPoint(x+deltaX, y+deltaY, m);
    return NewPoint;
  }
};

#pragma omp declare induction(next : (Point, float))                 \
                    inductor (omp_var = omp_var.nextPoint(omp_step)) \
                    collector(omp_step * omp_idx)

extern void process(Point P);

void processPointsInLine(Point Start, int NumberOfPoints, 
                         float Separation) {
  Point P = Start;
  #pragma omp parallel for induction(step(Separation), next : P)
  for (int i = 0; i < NumberOfPoints; ++i) {
    process(P);
    P = P.nextPoint(Separation);
  }
}

int main() {
  Point Start(1.0f, -2.0f, 0.5f);
  processPointsInLine(Start, 100, 0.25f);
  return 0;
}
