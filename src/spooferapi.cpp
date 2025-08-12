#include "spooferapi.hpp"

SpooferAPI* SpooferAPI::get() {
	static SpooferAPI instance;
	return &instance;
}

bool SpooferAPI::init() {
    m_baseUrl = "http://localhost:18498/";

    m_listener.bind([](web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            log::info("Deafener response: {}", res->string().unwrapOr("Uh oh!"));
        } else if (web::WebProgress* p = e->getProgress()) {
            log::info("progress: {}", p->downloadProgress().value_or(0.f));
        } else if (e->isCancelled()) {
            log::info("The request was cancelled... So sad :(");
        }
    });

    return true;
}

void SpooferAPI::toggleDeafen() {
    log::info("Toggling deafen state...");
    auto req = web::WebRequest();
    m_listener.setFilter(req.post(m_baseUrl + "toggle"));
}