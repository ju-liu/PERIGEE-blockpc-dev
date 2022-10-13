#include <unistd.h>
#include "Vec_Tools.hpp"
#include "Vector_3.hpp"
#include "Matrix_3x3.hpp"
#include "SymmMatrix_3x3.hpp"
#include "HDF5_Reader.hpp"
#include "PostVectSolution.hpp"


int main(int argc, char *argv[])
{
  //std::vector<std::string> a;
  //a.push_back("hello a\n");
  //a.push_back("cmame \t");
  //a.push_back("july 31th \n");
  //a.push_back("aug 31th \n");

  //for(auto &out : a) out = "hello\n";

  //for(auto out : a) std::cout<<out;

  SymmMatrix_3x3 A;

  A.gen_rand();

  Matrix_3x3 B( A.xx(), A.xy(), A.xz(), A.yx(), A.yy(), A.yz(), A.zx(), A.zy(), A.zz() );

  std::cout<<A.I1() - B.I1()<<std::endl;
  std::cout<<A.I2() - B.I2()<<std::endl;
  std::cout<<A.I3() - B.I3()<<std::endl;
  std::cout<<A.det() - B.det()<<std::endl;
  std::cout<<A.tr() - B.tr()<<std::endl;

  A.print_in_row();

  A.print();

  A.print_Voigt();

  sleep(1);
  SymmMatrix_3x3 A1;
  A1.gen_rand();
  Matrix_3x3 B1( A1.xx(), A1.xy(), A1.xz(), A1.yx(), A1.yy(), A1.yz(), A1.zx(), A1.zy(), A1.zz() );

  std::cout<<(A1 + A).xx() - (B1 + B).xx()<<std::endl;
  std::cout<<(A1 + A).xy() - (B1 + B).xy()<<std::endl;
  std::cout<<(A1 + A).xz() - (B1 + B).xz()<<std::endl;
  std::cout<<(A1 + A).yy() - (B1 + B).yy()<<std::endl;
  std::cout<<(A1 + A).yz() - (B1 + B).yz()<<std::endl;
  std::cout<<(A1 + A).zz() - (B1 + B).zz()<<std::endl;
  std::cout<<(A1 + A).yx() - (B1 + B).yx()<<std::endl;
  std::cout<<(A1 + A).zx() - (B1 + B).zx()<<std::endl;
  std::cout<<(A1 + A).zy() - (B1 + B).zy()<<std::endl;

  std::cout<<(A1 - A).xx() - (B1 - B).xx()<<std::endl;
  std::cout<<(A1 - A).xy() - (B1 - B).xy()<<std::endl;
  std::cout<<(A1 - A).xz() - (B1 - B).xz()<<std::endl;
  std::cout<<(A1 - A).yy() - (B1 - B).yy()<<std::endl;
  std::cout<<(A1 - A).yz() - (B1 - B).yz()<<std::endl;
  std::cout<<(A1 - A).zz() - (B1 - B).zz()<<std::endl;
  std::cout<<(A1 - A).yx() - (B1 - B).yx()<<std::endl;
  std::cout<<(A1 - A).zx() - (B1 - B).zx()<<std::endl;
  std::cout<<(A1 - A).zy() - (B1 - B).zy()<<std::endl;

  srand(time(NULL));
  double val = ( rand() % 1000 ) * 1.0e-3 - 0.5;

  for (int ii=0; ii<3; ++ii) A1 += A;
  for (int ii=0; ii<3; ++ii) B1 += B;
  std::cout<<A1.xx() - B1.xx()<<std::endl;
  std::cout<<A1.xy() - B1.xy()<<std::endl;
  std::cout<<A1.xz() - B1.xz()<<std::endl;
  std::cout<<A1.yy() - B1.yy()<<std::endl;
  std::cout<<A1.yz() - B1.yz()<<std::endl;
  std::cout<<A1.zz() - B1.zz()<<std::endl;
  std::cout<<A1.yx() - B1.yx()<<std::endl;
  std::cout<<A1.zx() - B1.zx()<<std::endl;
  std::cout<<A1.zy() - B1.zy()<<std::endl;

  for (int ii=0; ii<3; ++ii) A1 -= A;
  for (int ii=0; ii<3; ++ii) B1 -= B;
  std::cout<<A1.xx() - B1.xx()<<std::endl;
  std::cout<<A1.xy() - B1.xy()<<std::endl;
  std::cout<<A1.xz() - B1.xz()<<std::endl;
  std::cout<<A1.yy() - B1.yy()<<std::endl;
  std::cout<<A1.yz() - B1.yz()<<std::endl;
  std::cout<<A1.zz() - B1.zz()<<std::endl;
  std::cout<<A1.yx() - B1.yx()<<std::endl;
  std::cout<<A1.zx() - B1.zx()<<std::endl;
  std::cout<<A1.zy() - B1.zy()<<std::endl;

  A.inverse(); B.inverse();
  std::cout<<A.xx() - B.xx()<<std::endl;
  std::cout<<A.xy() - B.xy()<<std::endl;
  std::cout<<A.xz() - B.xz()<<std::endl;
  std::cout<<A.yy() - B.yy()<<std::endl;
  std::cout<<A.yz() - B.yz()<<std::endl;
  std::cout<<A.zz() - B.zz()<<std::endl;
  std::cout<<A.yx() - B.yx()<<std::endl;
  std::cout<<A.zx() - B.zx()<<std::endl;
  std::cout<<A.zy() - B.zy()<<std::endl;

  for (int ii=0; ii<3; ++ii) A1 *= val;
  for (int ii=0; ii<3; ++ii) B1 *= val;
  std::cout<<A1.xx() - B1.xx()<<std::endl;
  std::cout<<A1.xy() - B1.xy()<<std::endl;
  std::cout<<A1.xz() - B1.xz()<<std::endl;
  std::cout<<A1.yy() - B1.yy()<<std::endl;
  std::cout<<A1.yz() - B1.yz()<<std::endl;
  std::cout<<A1.zz() - B1.zz()<<std::endl;
  std::cout<<A1.yx() - B1.yx()<<std::endl;
  std::cout<<A1.zx() - B1.zx()<<std::endl;
  std::cout<<A1.zy() - B1.zy()<<std::endl;

  A.AXPY(val, A1); B.AXPY(val, B1);
  std::cout<<A.xx() - B.xx()<<std::endl;
  std::cout<<A.xy() - B.xy()<<std::endl;
  std::cout<<A.xz() - B.xz()<<std::endl;
  std::cout<<A.yy() - B.yy()<<std::endl;
  std::cout<<A.yz() - B.yz()<<std::endl;
  std::cout<<A.zz() - B.zz()<<std::endl;
  std::cout<<A.yx() - B.yx()<<std::endl;
  std::cout<<A.zx() - B.zx()<<std::endl;
  std::cout<<A.zy() - B.zy()<<std::endl;

  A.AXPI(val); B.AXPI(val);
  std::cout<<A.xx() - B.xx()<<std::endl;
  std::cout<<A.xy() - B.xy()<<std::endl;
  std::cout<<A.xz() - B.xz()<<std::endl;
  std::cout<<A.yy() - B.yy()<<std::endl;
  std::cout<<A.yz() - B.yz()<<std::endl;
  std::cout<<A.zz() - B.zz()<<std::endl;
  std::cout<<A.yx() - B.yx()<<std::endl;
  std::cout<<A.zx() - B.zx()<<std::endl;
  std::cout<<A.zy() - B.zy()<<std::endl;

  A+=A1; B+=B1;
  std::cout<<A.xx() - B.xx()<<std::endl;
  std::cout<<A.xy() - B.xy()<<std::endl;
  std::cout<<A.xz() - B.xz()<<std::endl;
  std::cout<<A.yy() - B.yy()<<std::endl;
  std::cout<<A.yz() - B.yz()<<std::endl;
  std::cout<<A.zz() - B.zz()<<std::endl;
  std::cout<<A.yx() - B.yx()<<std::endl;
  std::cout<<A.zx() - B.zx()<<std::endl;
  std::cout<<A.zy() - B.zy()<<std::endl;

  A.inverse(); B.inverse();

  Matrix_3x3 C;
  C.gen_rand();

  Matrix_3x3 SC( C + transpose(C) );
  SC *= 0.5;

  const SymmMatrix_3x3 D = gen_symm_part(C);
  std::cout<<SC.xx() - D.xx()<<std::endl;
  std::cout<<SC.yy() - D.yy()<<std::endl;
  std::cout<<SC.zz() - D.zz()<<std::endl;
  std::cout<<SC.xy() - D.xy()<<std::endl;
  std::cout<<SC.xz() - D.xz()<<std::endl;
  std::cout<<SC.yx() - D.yx()<<std::endl;
  std::cout<<SC.yz() - D.yz()<<std::endl;
  std::cout<<SC.zx() - D.zx()<<std::endl;
  std::cout<<SC.zy() - D.zy()<<std::endl;
   
  SymmMatrix_3x3 D1;
  D1.gen_symm(C);
  std::cout<<SC.xx() - D1.xx()<<std::endl;
  std::cout<<SC.yy() - D1.yy()<<std::endl;
  std::cout<<SC.zz() - D1.zz()<<std::endl;
  std::cout<<SC.xy() - D1.xy()<<std::endl;
  std::cout<<SC.xz() - D1.xz()<<std::endl;
  std::cout<<SC.yx() - D1.yx()<<std::endl;
  std::cout<<SC.yz() - D1.yz()<<std::endl;
  std::cout<<SC.zx() - D1.zx()<<std::endl;
  std::cout<<SC.zy() - D1.zy()<<std::endl;
  
  SymmMatrix_3x3 S;
  S(0) = B(0); S(5) = B(1); S(4) = B(2);
  S(1) = B(4); S(3) = B(5); S(2) = B(8);

  std::cout<<S(0) - B(0)<<std::endl;
  std::cout<<S(5) - B(1)<<std::endl;
  std::cout<<S(4) - B(2)<<std::endl;
  std::cout<<S(1) - B(4)<<std::endl;
  std::cout<<S(3) - B(5)<<std::endl;
  std::cout<<S(2) - B(8)<<std::endl;

  const SymmMatrix_3x3 I;
  SymmMatrix_3x3 S2; S2.gen_rand();
  S2(0) = I(0); S2(1) = I(1); S2(2) = I(2);
  S2(3) = I(3); S2(4) = I(4); S2(5) = I(5);
  (S2 - I).print();

  SymmMatrix_3x3 E(D);
  (E - D).print();

  SymmMatrix_3x3 ISM;
  ISM.gen_id();
  Matrix_3x3 IM;
  IM.gen_id();
  std::cout<<ISM.xx() - IM.xx()<<std::endl;
  std::cout<<ISM.xy() - IM.xy()<<std::endl;
  std::cout<<ISM.xz() - IM.xz()<<std::endl;
  std::cout<<ISM.yy() - IM.yy()<<std::endl;
  std::cout<<ISM.yz() - IM.yz()<<std::endl;
  std::cout<<ISM.zz() - IM.zz()<<std::endl;
  std::cout<<ISM.yx() - IM.yx()<<std::endl;
  std::cout<<ISM.zx() - IM.zx()<<std::endl;
  std::cout<<ISM.zy() - IM.zy()<<std::endl;

  SymmMatrix_3x3 OSM;
  OSM.gen_zero();
  Matrix_3x3 OM;
  OM.gen_zero();
  std::cout<<OSM.xx() - OM.xx()<<std::endl;
  std::cout<<OSM.xy() - OM.xy()<<std::endl;
  std::cout<<OSM.xz() - OM.xz()<<std::endl;
  std::cout<<OSM.yy() - OM.yy()<<std::endl;
  std::cout<<OSM.yz() - OM.yz()<<std::endl;
  std::cout<<OSM.zz() - OM.zz()<<std::endl;
  std::cout<<OSM.yx() - OM.yx()<<std::endl;
  std::cout<<OSM.zx() - OM.zx()<<std::endl;
  std::cout<<OSM.zy() - OM.zy()<<std::endl;

  const double pi = 4.0 * atan(1.0);
  double thetay = pi/val;
  double thetax = 2*pi/val;
  double thetaz = 3*pi/val;
  Matrix_3x3 Ry(cos(thetay),   0,   sin(thetay),
                     0,        1,       0,
               -sin(thetay),   0,   cos(thetay));

  Matrix_3x3 Rx(1,         0,           0,
                0,   cos(thetax),  -sin(thetax),
                0,   sin(thetax),   cos(thetax));

  Matrix_3x3 Rz(cos(thetaz), -sin(thetaz),   0,
                sin(thetaz),  cos(thetaz),   0,
                     0,          0,          1);
  Matrix_3x3 Q;
  Q.MatMult(Rx, Ry);
  Q.MatMult(Q, Rz);

  A.MatRot(Q); B.MatRot(Q);
  std::cout<<A.xx() - B.xx()<<std::endl;
  std::cout<<A.yy() - B.yy()<<std::endl;
  std::cout<<A.zz() - B.zz()<<std::endl;
  std::cout<<A.xy() - B.xy()<<std::endl;
  std::cout<<A.xz() - B.xz()<<std::endl;
  std::cout<<A.yx() - B.yx()<<std::endl;
  std::cout<<A.yz() - B.yz()<<std::endl;
  std::cout<<A.zx() - B.zx()<<std::endl;
  std::cout<<A.zy() - B.zy()<<std::endl;

//  Matrix_3x3 QT(transpose(Q));
//  Q.MatMult(Q, QT);
//  Q.print();

  Vector_3 xx(thetax, thetay, thetaz);
  Vector_3 yy(thetay, thetaz, thetax);

  std::cout<<A1.VecMatVec(xx, yy) - B1.VecMatVec(xx, yy)<<std::endl;

  (A1.VecMult(xx) - B1.VecMult(xx)).print();

  SymmMatrix_3x3 A2{};
  A2 = A1;
  (A2 - A1).print();
  
  SymmMatrix_3x3 A3;
  A3.gen_rand();
  
  A3 = A2 = A1;
  A2 = A2;
  (A3 - A2).print();
  (A3 - A1).print();
  (A1 - A2).print();

  A2.MatMultTransposeLeft(B1);
  Matrix_3x3 B2;
  B2.MatMultTransposeLeft(B1);
  std::cout<<A2.xx() - B2.xx()<<std::endl;
  std::cout<<A2.yy() - B2.yy()<<std::endl;
  std::cout<<A2.zz() - B2.zz()<<std::endl;
  std::cout<<A2.xy() - B2.xy()<<std::endl;
  std::cout<<A2.xz() - B2.xz()<<std::endl;
  std::cout<<A2.yx() - B2.yx()<<std::endl;
  std::cout<<A2.yz() - B2.yz()<<std::endl;
  std::cout<<A2.zx() - B2.zx()<<std::endl;
  std::cout<<A2.zy() - B2.zy()<<std::endl;

  A3.MatMultTransposeRight(B1);
  Matrix_3x3 B3;
  B3.MatMultTransposeRight(B1);
  std::cout<<A3.xx() - B3.xx()<<std::endl;
  std::cout<<A3.yy() - B3.yy()<<std::endl;
  std::cout<<A3.zz() - B3.zz()<<std::endl;
  std::cout<<A3.xy() - B3.xy()<<std::endl;
  std::cout<<A3.xz() - B3.xz()<<std::endl;
  std::cout<<A3.yx() - B3.yx()<<std::endl;
  std::cout<<A3.yz() - B3.yz()<<std::endl;
  std::cout<<A3.zx() - B3.zx()<<std::endl;
  std::cout<<A3.zy() - B3.zy()<<std::endl;

  double a2b1 = A2.MatContraction(B1);
  double b2b1 = B2.MatContraction(B1);
  std::cout<<a2b1 - b2b1<<std::endl;

  double a1b2 = A1.MatContraction(A2);
  double b1b2 = B1.MatContraction(B2);
  std::cout<<a1b2 - b1b2<<std::endl;
   
  inverse(A2);   
  inverse(B2);
  std::cout<<A2.xx() - B2.xx()<<std::endl;
  std::cout<<A2.xy() - B2.xy()<<std::endl;
  std::cout<<A2.xz() - B2.xz()<<std::endl;
  std::cout<<A2.yy() - B2.yy()<<std::endl;
  std::cout<<A2.yz() - B2.yz()<<std::endl;
  std::cout<<A2.zz() - B2.zz()<<std::endl;
  std::cout<<A2.yx() - B2.yx()<<std::endl;
  std::cout<<A2.zx() - B2.zx()<<std::endl;
  std::cout<<A2.zy() - B2.zy()<<std::endl;
 
  (A2 * xx - B2 * xx).print();

  Matrix_3x3 A1_A2(A1 * A2);
  Matrix_3x3 B1_B2(B1 * B2);
  (A1_A2 - B1_B2).print();

  B2.gen_rand();
  Matrix_3x3 A1B2(A1 * B2);
  Matrix_3x3 B1B2(B1 * B2);
  (A1B2 - B1B2).print();

  Matrix_3x3 B2A1(B2 * A1);
  Matrix_3x3 B2B1(B2 * B1);
  (B2A1 - B2B1).print();

  Matrix_3x3 F;
  F.gen_rand();
  A3 = gen_right_Cauchy_Green(F);
  B3.MatMultTransposeLeft(F);
  std::cout<<A3.xx() - B3.xx()<<std::endl;
  std::cout<<A3.yy() - B3.yy()<<std::endl;
  std::cout<<A3.zz() - B3.zz()<<std::endl;
  std::cout<<A3.xy() - B3.xy()<<std::endl;
  std::cout<<A3.xz() - B3.xz()<<std::endl;
  std::cout<<A3.yx() - B3.yx()<<std::endl;
  std::cout<<A3.yz() - B3.yz()<<std::endl;
  std::cout<<A3.zx() - B3.zx()<<std::endl;
  std::cout<<A3.zy() - B3.zy()<<std::endl;

  A3 = gen_left_Cauchy_Green(F);
  B3.MatMultTransposeRight(F);
  std::cout<<A3.xx() - B3.xx()<<std::endl;
  std::cout<<A3.yy() - B3.yy()<<std::endl;
  std::cout<<A3.zz() - B3.zz()<<std::endl;
  std::cout<<A3.xy() - B3.xy()<<std::endl;
  std::cout<<A3.xz() - B3.xz()<<std::endl;
  std::cout<<A3.yx() - B3.yx()<<std::endl;
  std::cout<<A3.yz() - B3.yz()<<std::endl;
  std::cout<<A3.zx() - B3.zx()<<std::endl;
  std::cout<<A3.zy() - B3.zy()<<std::endl;

  return EXIT_SUCCESS;
}

// EOF
