#include <Geode/Geode.hpp>

using namespace geode::prelude;

class SpooferAPI : public CCObject {
    EventListener<web::WebTask> m_listener;
    gd::string m_baseUrl;

public:
    static SpooferAPI* get();
    bool init();
    void toggleDeafen();
};