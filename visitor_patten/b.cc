#include <iostream>
using namespace std;

class SpaceShip;
class ApolloSpacecraft;

class Asteroid {
public:
  virtual void CollideWith(SpaceShip &) {
    std::cout << "Asteroid hit a SpaceShip\n";
  }
  virtual void CollideWith(ApolloSpacecraft &) {
    std::cout << "Asteroid hit an ApolloSpacecraft\n";
  }
};

class ExplodingAsteroid : public Asteroid {
public:
  virtual void CollideWith(SpaceShip &) {
    std::cout << "ExplodingAsteroid hit a SpaceShip\n";
  }
  virtual void CollideWith(ApolloSpacecraft &) {
    std::cout << "ExplodingAsteroid hit an ApolloSpacecraft\n";
  }
};

class SpaceShip {
public:
  virtual void CollideWith(Asteroid &inAsteroid) {
    inAsteroid.CollideWith(*this);
  }
};

class ApolloSpacecraft : public SpaceShip {
public:
  virtual void CollideWith(Asteroid &inAsteroid) {
    inAsteroid.CollideWith(*this);
  }
};

int main(void) {
  Asteroid theAsteroid;
  SpaceShip theSpaceShip;

  ExplodingAsteroid theExplodingAsteroid;
  ApolloSpacecraft theApolloSpacecraft;

  Asteroid &theAsteroidReference = theExplodingAsteroid;
  SpaceShip &theSpaceShipReference = theApolloSpacecraft;

  theAsteroid.CollideWith(theSpaceShip);
  theAsteroid.CollideWith(theApolloSpacecraft);
  cout << "----" << endl;

  theExplodingAsteroid.CollideWith(theSpaceShip);
  theExplodingAsteroid.CollideWith(theApolloSpacecraft);
  cout << "----" << endl;

  theAsteroidReference.CollideWith(theSpaceShip);
  theAsteroidReference.CollideWith(theApolloSpacecraft);
  cout << "----" << endl;

  theAsteroid.CollideWith(theSpaceShipReference);
  theAsteroidReference.CollideWith(theSpaceShipReference);
  cout << "----" << endl;

  theSpaceShipReference.CollideWith(theAsteroid);
  theSpaceShipReference.CollideWith(theAsteroidReference);
  cout << "----" << endl;

  return 0;
}
