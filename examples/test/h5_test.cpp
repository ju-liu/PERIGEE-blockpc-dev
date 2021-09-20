#include "HDF5_Reader.hpp"
#include "HDF5_Writer.hpp"

int main( int argc, char * argv[] )
{
  hid_t file_id = H5Fcreate("test.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  HDF5_Writer * h5w = new HDF5_Writer( file_id );

  int a = -1;
  h5w -> write_intScalar( "aa", a );

  Vector_3 vv;

  vv.gen_rand();

  h5w -> write_Vector_3( "vv", vv );

  hid_t group_id = H5Gcreate(file_id, "/GROUP_TEST",
      H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  h5w -> write_Vector_3( group_id, "vv", vv );

  H5Gclose( group_id );

  delete h5w;
  H5Fclose(file_id);

  hid_t file_id_2 = H5Fopen( "test.h5", H5F_ACC_RDONLY, H5P_DEFAULT);

  HDF5_Reader * h5r = new HDF5_Reader( file_id_2 );

  Vector_3 rr = h5r -> read_Vector_3("/GROUP_TEST", "vv");

  delete h5r; H5Fclose(file_id_2);

  rr.print();

  rr -= vv;

  rr.print();

  return EXIT_SUCCESS;
}

// EOF
