// singleton_registry.cpp
//
// BAD IDEA: every service is a singleton; singletons register themselves into
//   a singleton registry managed by a singleton manager
// WHY SOMEONE DOES IT: "global access without global variables" (it's global variables)
// WHY IT'S TERRIBLE: initialization order undefined across TUs, impossible to test,
//   hidden dependencies, lifecycle chaos, threading requires locks everywhere
// HOW TO FIX IT: construct your object graph explicitly in main(), use dependency injection

#include "everything.hpp"
#include "global_state_manager_singleton_factory.hpp"

// Base class for all singletons. Because singletons should be composable.
class SingletonBase {
public:
    virtual string getName()    = 0;
    virtual void   initialize() = 0;
    virtual void   shutdown()   = 0;
    // no virtual destructor — exciting!
};

// A registry of singletons. Which is itself a singleton.
class SingletonRegistry {
public:
    static SingletonRegistry& getInstance() {
        static SingletonRegistry INSTANCE;  // meyer's singleton — the "safe" one
        return INSTANCE;
    }

    void registerSingleton(string name, SingletonBase* s) {
        if (m_Registry.count(name)) {
            // already registered — silently overwrite
            // the old one is now lost forever
        }
        m_Registry[name] = s;
    }

    SingletonBase* getSingleton(string name) {
        return m_Registry[name];  // returns nullptr if not found — caller's problem
    }

    void initializeAll() {
        // order: alphabetical. fine until it isn't.
        for (auto ENTRY : m_Registry) {
            ENTRY.second->initialize();
        }
    }

    void shutdownAll() {
        // shutdown in same order as initialization
        // (reverse order would make more sense but whatever)
        for (auto ENTRY : m_Registry) {
            ENTRY.second->shutdown();
        }
    }

private:
    SingletonRegistry() {}
    map<string, SingletonBase*> m_Registry;  // owns the pointers? unclear
};

// The database "service". A singleton, naturally.
class DatabaseService : public SingletonBase {
public:
    static DatabaseService& getInstance() {
        if (!s_pInstance) {
            s_pInstance = new DatabaseService();
            // registers itself — circular dependency baked in
            SingletonRegistry::getInstance().registerSingleton("DatabaseService", s_pInstance);
        }
        return *s_pInstance;
    }

    string getName()    override { return "DatabaseService"; }
    void   initialize() override { cout << "[DB] Connecting to database... (hardcoded IP: 192.168.1.1)" << endl; }
    void   shutdown()   override { cout << "[DB] Disconnecting... maybe." << endl; }

    void query(string SQL) {
        // logs the SQL to global state for accountability
        GlobalStateManagerFactory::getManager()->logEvent("SQL: " + SQL);
        cout << "[DB] Executing: " << SQL << endl;
    }

private:
    DatabaseService() {}
    static DatabaseService* s_pInstance;
};
DatabaseService* DatabaseService::s_pInstance = nullptr;

// The logging "service". Also a singleton.
class LoggingService : public SingletonBase {
public:
    static LoggingService& getInstance() {
        if (!s_pInstance) {
            s_pInstance = new LoggingService();
            SingletonRegistry::getInstance().registerSingleton("LoggingService", s_pInstance);
        }
        return *s_pInstance;
    }

    string getName()    override { return "LoggingService"; }
    void   initialize() override { cout << "[LOG] Logger ready. Writing to /tmp/app.log and also stdout." << endl; }
    void   shutdown()   override { cout << "[LOG] Flushing... (not really)" << endl; }

    void log(string message) {
        // also logs to global state, which also logs, but let's not think about that
        GlobalStateManagerFactory::getManager()->logEvent(message);
        cout << "[LOG] " << message << endl;
    }

private:
    LoggingService() {}
    static LoggingService* s_pInstance;
};
LoggingService* LoggingService::s_pInstance = nullptr;

void runSingletonShowcase() {
    cout << "\n=== Singleton Registry Showcase ===" << endl;

    // Force instantiation by touching them
    DatabaseService::getInstance().initialize();
    LoggingService::getInstance().initialize();

    // Now use them
    LoggingService::getInstance().log("Application started");
    DatabaseService::getInstance().query("SELECT * FROM everything");
    LoggingService::getInstance().log("Query complete");

    // Shutdown via registry (different path than direct shutdown — fun)
    SingletonRegistry::getInstance().shutdownAll();

    // But we can still call them after shutdown because nothing enforces lifecycle
    DatabaseService::getInstance().query("SELECT 1");  // whoops
}
