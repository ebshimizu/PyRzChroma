#include "PyRzChroma.h"

int main(int argc, char** argv)
{
  RzChroma* driver = new RzChroma();

  driver->Initialize();
  driver->setAll(1, 0, 0);

  getchar();

  driver->setKey(RZKEY2::RZKEY_D, 0, 1, 0);
  driver->setKey(RZKEY2::RZKEY_F, 0, 1, 1);

  getchar();

  delete driver;
}
