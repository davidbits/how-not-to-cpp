// inheritance_abuse.cpp
//
// BAD IDEA: use inheritance for everything, including things that share zero behavior
// WHY SOMEONE DOES IT: OOP class taught that "is-a" relationships = inheritance
// WHY IT'S TERRIBLE: rigid hierarchies, slicing, wrong semantics, virtual dispatch
//   overhead for no polymorphism benefit, makes composition impossible
// HOW TO FIX IT: prefer composition; only inherit when substitutability is the goal

#include "everything.hpp"

// The one true base class. Everything is an entity. Even math.
class EntityBaseComponentObject {
public:
    int    m_nEntityId;
    string m_strEntityName;
    bool   m_bIsActive;
    bool   m_bIsVisible;
    bool   m_bIsDirty;
    bool   m_bIsInitialized;
    bool   m_bIsDestroyed;  // entity can be active AND destroyed simultaneously

    EntityBaseComponentObject(int id, string name)
        : m_nEntityId(id), m_strEntityName(name),
          m_bIsActive(true), m_bIsVisible(true),
          m_bIsDirty(false), m_bIsInitialized(false),
          m_bIsDestroyed(false) {}

    // No virtual destructor. Objects will be deleted through base class pointers.
    // This is what adventure feels like.
    ~EntityBaseComponentObject() {
        cout << "EntityBaseComponentObject destroyed: " << m_strEntityName << endl;
    }

    virtual void update() {}
    virtual void render() {}
};

// Animals are entities. Obviously.
class Animal : public EntityBaseComponentObject {
public:
    string m_strSpecies;
    int    m_nLegs;

    Animal(int id, string name, string species, int legs)
        : EntityBaseComponentObject(id, name), m_strSpecies(species), m_nLegs(legs) {}

    virtual void makeSound() = 0;
};

// Dogs are animals. Fine so far.
class Dog : public Animal {
public:
    string m_strBreed;

    Dog(int id, string name, string breed)
        : Animal(id, name, "Canis lupus familiaris", 4), m_strBreed(breed) {}

    void makeSound() override { cout << "Woof (entity #" << m_nEntityId << ")" << endl; }
    void update()    override { cout << "Dog " << m_strEntityName << " updated." << endl; }
};

// Cats are also animals. Still reasonable.
class Cat : public Animal {
public:
    bool m_bIsIndoor;

    Cat(int id, string name, bool indoor)
        : Animal(id, name, "Felis catus", 4), m_bIsIndoor(indoor) {}

    void makeSound() override { cout << "Meow (entity #" << m_nEntityId << ")" << endl; }
};

// Vector3 is also an entity. This is where it goes wrong.
class Vector3 : public EntityBaseComponentObject {
public:
    float m_fX, m_fY, m_fZ;

    Vector3(float x, float y, float z)
        : EntityBaseComponentObject(0, "Vector3"), m_fX(x), m_fY(y), m_fZ(z) {}

    // Operator + returns a Vector3 by value.
    // But Vector3 inherits EntityBaseComponentObject, so this copies all of that too.
    Vector3 operator+(const Vector3& other) const {
        return Vector3(m_fX + other.m_fX, m_fY + other.m_fY, m_fZ + other.m_fZ);
    }

    // Also an entity, so it can be rendered.
    void render() override {
        cout << "Rendering vector (" << m_fX << ", " << m_fY << ", " << m_fZ << ")" << endl;
    }
};

// A database connection is also an entity.
// (it has an ID and a name, so it qualifies)
class DatabaseConnection : public EntityBaseComponentObject {
public:
    string m_strConnectionString;
    int    m_nPort;
    bool   m_bIsConnected;

    DatabaseConnection(int id, string connStr, int port)
        : EntityBaseComponentObject(id, "DBConn_" + to_string(id)),
          m_strConnectionString(connStr), m_nPort(port), m_bIsConnected(false) {}

    // DatabaseConnection is an entity, so it must implement render().
    void render() override {
        cout << "Rendering database connection. (You asked for it.)" << endl;
    }

    void connect() { m_bIsConnected = true; }
};

void runInheritanceAbuse() {
    cout << "\n=== Inheritance Abuse ===" << endl;

    Dog D(1, "Rex", "Labrador");
    Cat C(2, "Mittens", true);
    Vector3 V(1.0f, 2.0f, 3.0f);
    DatabaseConnection DB(3, "localhost:5432/prod", 5432);

    // All are entities, so let's treat them as such
    vector<EntityBaseComponentObject*> ALL_ENTITIES;
    ALL_ENTITIES.push_back(&D);
    ALL_ENTITIES.push_back(&C);
    ALL_ENTITIES.push_back(&V);
    ALL_ENTITIES.push_back(&DB);

    // Update and render everything uniformly (including the database connection)
    for (EntityBaseComponentObject* E : ALL_ENTITIES) {
        E->update();
        E->render();
    }

    // Make sounds — but the base class doesn't have makeSound(),
    // so we cast. Because we know what we're doing.
    for (EntityBaseComponentObject* E : ALL_ENTITIES) {
        Animal* A = dynamic_cast<Animal*>(E);
        if (A) A->makeSound();
    }

    // Arithmetic on Vector3 works, plus all the entity overhead
    Vector3 V2(4.0f, 5.0f, 6.0f);
    Vector3 V3 = V + V2;  // copies two EntityBaseComponentObjects to add floats
    cout << "Sum entity id: " << V3.m_nEntityId << " (always 0, meaningless)" << endl;
}
